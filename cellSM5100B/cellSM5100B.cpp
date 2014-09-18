/*
Read header file
*/
#include "Arduino.h"
#include "cellSM5100B.h"

#define cell Serial1

cellSM5100B::cellSM5100B(){
	_cell=NULL;
	_prefix="";
}

void cellSM5100B::cellCall(String number, unsigned int ms){
//make a call and hang up after 'dealy' ms
//  print("ATD")+print(numero)+write((byte)13)=println("ATD"+number)
//  print("ATH")+write((byte)13)=println("ATH")
  _cell->println("ATD"+_prefix+number);
	cellGetMessage(5000); //should give +SIND: 5,1
	cellGetMessage(5000); //should give +SIND: 2
  delay(ms);
  _cell->println("ATH");
  cellWaitTil("OK");
// after ATH we receive OK (if all worked)
}

boolean cellSM5100B::cellCheck4Data(){
// returns true if the sm5100b shields is saying something
  if(_cell->available()>0){    
    return true;
  }
  else{    
    return false;
  }
}

// TO-DO:implement error code
int cellSM5100B::cellDeleteAllSMS(){
  _cell->println("AT+CMGD=1,4");
  cellWaitFor("OK");
  
  return 0;
}

String cellSM5100B::cellGetMessage(){
// TO-DO: implement a cleaner ERROR return
// Timeout of 1000ms (default). Maybe an argument in getMessage() to call
  String s="";
  _cell->setTimeout(1000);  // default timeout of 1000ms
  s=_cell->readStringUntil('\n');
  if (s.length()>=1 && s[s.length()-1]=='\r'){ // if last 2 chars are \r\n
    if (s==" \r" || s=="\r") { // skip these, move on
      return "";
    }
    else { // message is returned WITHOUT \r\n and printed to Serial
      Serial.println(s.substring(0,s.length()-1));
      return s.substring(0,s.length()-1);
    }
  }
  else
    return "";
}

String cellSM5100B::cellGetMessage(unsigned int timeout){
// Idem as before but with the possibility of different timeout values
  String s="";
  _cell->setTimeout(timeout);
  s=_cell->readStringUntil('\n');
  if (s.length()>=1 && s[s.length()-1]=='\r'){
    if (s==" \r" || s=="\r") {
      return "";
    }
    else { // message is returned WITHOUT \r\n and printed to Serial
      Serial.println(s.substring(0,s.length()-1));
      return s.substring(0,s.length()-1);
    }
  }
  else
    return "";
}

void cellSM5100B::cellSMSSetup(){
  _cell->println("AT+CMGF=1"); // set SMS mode to text
  cellWaitFor("OK");
  _cell->println("AT+CNMI=3,3,0,0"); // set module to send SMS data to serial out upon receipt 
  cellWaitFor("OK");
}

void cellSM5100B::cellSendSMS(String message, String number){
//  print("AT+CMGS=")	//AT command to start SMS
//  write((byte)34)		//double quotations
//  print(number)			//phone number
//  write((byte)34)		//double quotations
//  write((byte)13)		//carriage return
//  print(message)		//message
//  delay(500)
//  write((byte)26)		//Ctrl-Z
//  delay(15000)			//waiting time needed?
  _cell->print("AT+CMGS=\""+_prefix+number+"\"");
  _cell->write((byte)13);
// TO-DO: check if _cell->println("AT+CMGS=\"+_prefix+number+"\""); is possible
  _cell->print(message);
  delay(500);
  _cell->write((byte)26);
  cellWaitTil("OK");  // TO-DO: check if waitFor is possible instead of Til
}

void cellSM5100B::cellSerialCom(){
// Direct communication between Serial and shield
  String s="";
  boolean quit=false;
	
  Serial.println("Entering direct comm mode...");
  Serial.println("Write 'quit' to quit");
  
  while (quit==false){
    if (Serial.available()>0){
      s=Serial.readStringUntil('\n');
      if (s.substring(0,s.length()-1)=="quit"){
        quit=true;
  			Serial.println("'quit' received. Quitting direct comm mode...");
        break; // to avoid writting 'quit' to shield. Not really a big issue.
      }
      _cell->print(s+"\n");  // sm5100b needs to receive \r\n. \r was not previously deleted
      Serial.print(s+"\n");  // ECHO
    }
    if (cellCheck4Data()){
      s=cellGetMessage(); // cellGetMessage prints the message to Serial. Assignation is not really needed
    }
  }
}

void cellSM5100B::cellSetPrefix(String s){
// sets the default prefix to each phone number. Default 0034 (Spain)
	_prefix=s;
	Serial.println("Prefix is now: "+_prefix);
}

void cellSM5100B::cellSetup(HardwareSerial *c){
// receives the HW serial port ADDRESS to use with the shield i.e. c.cellSetup(&Serial1)
  Serial.println("Starting SM5100B Communication...");
	_cell=c;
  _cell->begin(9600); 
  // Waits till ready. Nothing will be done if +SIND: 4 doesn't reach.
  cellWaitTil("+SIND: 4");
	cellSetPrefix("0034");
  Serial.println("Module ready...");
}

// **********************************

// Every single message comes with a blank line before. 
// I read this blank and then the expected message in this "dirty" way
// in order not to modifiy getMessage method that has now a timeout implemented.
String cellSM5100B::cellWaitFor(String s){
  String message=cellGetMessage();
  if (message == "") // this is to receive and discard the previous blank
    message=cellGetMessage();
  if (message != s && message!="") {
    Serial.println("Wait, \""+message+"\" is not what we were expecting. We wanted \""+s+"\"");
    //cellOutputForever();
  }
  delay(100); // wait for a tiny bit before sending the next command
  return message;
}

String cellSM5100B::cellWaitFor(String s, unsigned int timeout){
  String message=cellGetMessage(timeout);
  if (message == "") // this is to receive and discard the previous blank
    message=cellGetMessage(timeout);
  if (message != s && message!="") {
    Serial.println("Wait, \""+message+"\" is not what we were expecting. We wanted \""+s+"\"");
    //cellOutputForever();
  }
  delay(100); // wait for a tiny bit before sending the next command
  return message;
}

// Careful with this method. It blocks everything til the String is received...if it is not received it enters an infinite loop.
void cellSM5100B::cellWaitTil(String s){
  String message;
  while (1) {
    message = cellGetMessage();
    if (message == s){
      delay(100); // cause we're probably about to send another command
      return;
    }
  }
}

