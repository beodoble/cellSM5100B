cellSM5100B
===========

arduino library mainly to send/receive SMSs and interact directly to this shield.

I started this library to control my parents country house heaters and inform them what temperature is in there through SMSs messages. It is currently in beta version (also this library  XD  )
Feel free to comment in a constructive way!  ;)

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
