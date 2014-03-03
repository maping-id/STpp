#include <Board.h>
#include <tr1/functional>
#include <Log.h>
#include <Debug.h>
#include <Exti.h>
#include <UsbSerial.h>
#include <Shell.h>
#include <ShellGpio.h>
#include <ShellPwm.h>
#include <ShellHBridgeST.h>
#include <ShellIncrementalEncoder.h>
#include <ShellTimer.h>
#include <Asserv.h>

extern "C" void vTaskDelay(int);
static Shell shell;
int main() {
	UsbSerial usb;
	usb << "Hello !" << endl;
	shell.setStream(&usb, &usb);

	//Use _ prefix as a hideme for autocompletion
	shell << "_UserButton" << UserButton;

	shell << "_Prop0A" << Prop0A;
	shell << "_Prop0B" << Prop0B;

	shell << "_Prop1A" << Prop1A;
	shell << "_Prop1B" << Prop1B;

	shell << "HBridge0" << HBridge0;
	shell << "HBridge1" << HBridge1;

	Asserv a(Encoder0, Encoder0, Tim13, HBridge0, HBridge0);

	a.setTargetDist(40960);
	shell << "Encoder0" << Encoder0;
	shell << "Tim1" << Tim1;
	shell.exec();
	while(1);
}
