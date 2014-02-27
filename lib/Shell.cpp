#include "Shell.h"

Shell::Shell(IStream& in, OStream& out): in(in), out(out),
	cbs_name{}, n_cbs(0) {

	current_object = 0;
	got_name = false;

	add([](Stack &s) {
			int a = s.pop().toInt();
			int b = s.pop().toInt();
			s.push(a+b);
		}, "+");

	add([&out](Stack &s) {
			Object& o = s.pop();
			out << "# " << o.toInt() << endl;
		}, ".");

	add([&out](Stack &s) {
			Object& o = s.pop();
			out << "? " << o.toInt() << endl;
		}, ".", ".");
}

void Shell::add(Callback cb, const char *name1, const char *name2) {
	cbs_name[n_cbs][0] = name1;
	cbs_name[n_cbs][1] = name2;
	cbs[n_cbs] = cb;
	n_cbs++;
}

bool Shell::isValue(const char *str) {
	if(*str >= '0' && *str <= '9')
		return true;
	if(*str == '"')
		return true;
	return false;
}

void Shell::pushInt(const char *str) {
	if(str[0] != '0' || str[1] != 'x')
		while(1);
	int v = 0;
	for(int i = 2; str[i]; ++i) {
		char c = str[i];
		if(c >= '0' && c <= '9')
			c = c - '0';
		else if(c >= 'a' && c <= 'f')
			c = c - 'a' + 10;
		else if(c >= 'A' && c <= 'F')
			c = c - 'A' + 10;
		else
			while(1);
		v <<= 4;
		v |= c;
	}
	s.push(v);
}

void Shell::pushStr(const char *str) {
	while(1);
}

void Shell::pushValue(const char *str) {
	if(*str >= '0' && *str <= '9')
		pushInt(str);
	else if(*str == '"')
		pushStr(str);
}

static int str_cmp_split(const char *str, const char *to, char key=':') {
	for(int i=0;str[i] || to[i]; ++i) {
		if(str[i] == key && to[i] == 0)
			return i+1;
		if(str[i] != to[i])
			return 0;
	}
	return -1;
}

static bool fnc_eq(const char *str, const char **name) {
	int ret;
	ret = str_cmp_split(str, name[0]);
	if(!ret)
		return false;
	if(ret == -1 && name[1] == 0)
		return true;
	if(!name[1])
		return false;
	return !!str_cmp_split(str+ret, name[1], 0);
}

void Shell::call(const char *str) {
	for(int i=0; i<n_cbs; ++i) {
		if(fnc_eq(str, cbs_name[i])) {
			cbs[i](s);
			return;
		}
	}
	out << "Function not found !" << endl;
}

void Shell::exec(const char* prompt) {
	char cmd[256];
	while(true) {
		out << prompt;
		in >> cmd;
		if(isValue(cmd)) {
			pushValue(cmd);
		} else {
			call(cmd);
		}
	}
}

Shell& Shell::operator<<(const char* name) {
	if(!got_name) {
		got_name = true;
		current_object = name;
	} else {
		while(1);
	}
	return *this;
}
