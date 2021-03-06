#include <Board.h>
#include <Log.h>
#include <Ax12.h>
#include <Uart.h>

int main() {
	log << "startup" << endl;
	Zigbee_UART
		.enable()
		.setMantissa(0x10e)
		.setFraction(0xc)
		.enableReceive()
		.enableTransmitter();

	Zigbee_TX
		.setPushPull()
		.setAlternate(Gpio::USART4_6)
		.setDirection(Gpio::OUTPUT)
		.setSpeed(Gpio::SPEED_100MHz);

	Zigbee_RX
		.setAlternate(Gpio::USART4_6)
		.setDirection(Gpio::INPUT)
		.setSpeed(Gpio::SPEED_100MHz)
		.setResistor(Gpio::PULL_UP);

	DmaStream uartDma(1, 4, 7);
	//Configure the pin
	auto ax12_pin = GpioB[10];
	Ax12 ax12_broadcast(ax12_pin, Uart(3, &uartDma), 0xfe);
	Ax12 ax12_left(ax12_pin, Uart(3, &uartDma), 0x83);
	Ax12 ax12_right(ax12_pin, Uart(3, &uartDma), 0x82);
	Ax12 ax12_under(ax12_pin, Uart(3, &uartDma), 0x81);
	//Enable torque
	ax12_broadcast.setSpeed(0x80);
	ax12_broadcast.enable();

	while(1) {
		char c;
		Zigbee_UART << "Going to startup position...";
		//Closed
		{
			ax12_left.goTo(0xb0);
			time.msleep(10);
			ax12_right.goTo(0x90);
			time.msleep(10);
			time.msleep(300);
			ax12_under.goTo(0x330);
			time.msleep(10);
		}

		Zigbee_UART << "done." << endl;
		Zigbee_UART >> c;


		Zigbee_UART << "Going to open position...";
		//To go forward
		{
			ax12_under.goTo(0x280);
			time.msleep(10);
			time.msleep(300);

			int delta = 0x40;
			ax12_left.goTo(0x200+delta);
			time.msleep(10);
			ax12_right.goTo(0x200-delta);
			time.msleep(10);
		}
		Zigbee_UART << "done." << endl;

		Zigbee_UART >> c;

		Zigbee_UART << "Going to closed position...";
		//To go backward
		{
			//First close braces, then the container
			int delta = 0x15;
			ax12_left.goTo(0x200-delta);
			time.msleep(10);
			ax12_right.goTo(0x200+delta);
			time.msleep(10);
			ax12_under.goTo(0x280);
			time.msleep(10);
		}
		Zigbee_UART << "done." << endl;

		Zigbee_UART >> c;
	}

	/*
	while(1) {
		ax12_broadcast.goTo(0x200);
		time.msleep(5000);
		ax12_broadcast.goTo(0x330);
		time.msleep(5000);
	}
	*/

	/*
	while(1) {
		ax12_broadcast.goTo(0x200);
		time.msleep(5000);
		ax12_broadcast.goTo(0xb0);
		time.msleep(5000);
	}*/ //bac plus eleve
	/*
	 * Ouvre en cone
	 * ferme symetrique
	 * //recule
	while(1) {
		ax12_left.goTo(0x200);
		time.msleep(10);
		ax12_right.goTo(0x200);
		time.msleep(10);
		ax12_under.goTo(0x200);
		time.msleep(10);

		time.msleep(5000);
		
		ax12_left.goTo(0xb0);
		time.msleep(10);
		ax12_right.goTo(0x90);
		time.msleep(10);
		ax12_under.goTo(0x330);
		time.msleep(10);
		time.msleep(5000);
	}*/
}
