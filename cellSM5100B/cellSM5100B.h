/*
HARDWARE needs:
- sm5100b Shield
- Hardware Serial port from ArduinoMega to sm5100b shield (in my case: ArduinoMega.pin19 to sm5100b.pin2 & 		ArduinoMega.pin18 to sm5100b.pin3). RECOMENDATION: bend pins 2 and 3 of sm5100b in order to avoid 
	electrical connection with ArduinoMega board. If not you will feed those pins with Hardware Serial port 
	previously connected.
SOFTWARE needs:
- In order to use Serial Monitor set it to 9600bps and add 'CR and NL'

It uses Hardware serial port to communicate with the GSM shield. It receives the adress of a HW Serial 
port from the Arduino. In my case Serial1 from my Mega1280.
SoftWare Serial is not yet implemented in this library. My recommendation in case you prefer to use 
SW Serial take advantage of "serialGSM" already programmed by 'meirm' and availabel at GitHub.
cellSM5100B is a library I programmed based on different codes and libraries I found 
in the Internet. I must thank 'tobek' for his SM5100B-GPRS code and 'meirm' for his 'SerialGSM' library.

My programming skills are not very good so use this code at your own risk.
Right now it makes calls, send and receive SMSs.
*/

#ifndef _cellSM5100B_H
#define _cellSM5100B_H

#include "Arduino.h"


class cellSM5100B
{
	public:
		cellSM5100B(); //constructor
		void cellCall(String number, unsigned int ms); //make a call and hang up after ms
    boolean cellCheck4Data(); //checks if the sm5100b shields is saying something
    int cellDeleteAllSMS(); //delete all SMSs stored in the SIM card
    String cellGetMessage(); //returns a message from the shield with a default timeout (1000 ms)
		String cellGetMessage(unsigned int timeout); //returns a message from the shield with a timeout (X ms)
    void cellSMSSetup(); //configures the shield to work in text mode and plot SMS as received
    void cellSendSMS(String message, String number); //sends SMS to a telephone number
    void cellSerialCom(); //direct communication with shield (to send AT commands)
		void cellSetPrefix(String s); //sets the default prefix to each phone number. Default 0034 (Spain)
    void cellSetup(HardwareSerial *c); //receives the HW serial port ADDRESS to use with the shield
		String cellWaitFor(String s); //returns the expected String or an error message with default timeout
		String cellWaitFor(String s, unsigned int timeout); //returns the expected String or an error message with a defined timeout
		void cellWaitTil(String s); //waits until expected String while it prints every message from the shield
	private:
		HardwareSerial *_cell; //Hardware Serial port
		String _prefix; //00+Country. By default 0034 (Spain)
};

#endif
		
		
		
		
		

		



