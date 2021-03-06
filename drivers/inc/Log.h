#ifndef _LOG_H
#define _LOG_H
#include "OStream.h"

class Log : public OStream {
	private:
		volatile char log[32][128];
		int line;
		int position;
	public:
		virtual Log& put(char);
		virtual Log& endl();
		void dump(OStream& o);
};

extern Log log;
#endif /* _LOG_H */
