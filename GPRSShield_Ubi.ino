/*
 Basic sketch for GPRS shield sim900

 This is a basic example on how to post a value to Ubidots, with just the function "save_value".

 Pins' connection
 Arduino       WiFly
  7    <---->    TX
  8    <---->    RX

 created 20 Aug. 2014
 by Mateo Velez - Metavix

 This example code is in the public domain.

*/

//--------------------------------------------------------------
//------------------------------Libraries-----------------------
//--------------------------------------------------------------
#include <SoftwareSerial.h>
#include <String.h>
SoftwareSerial mySerial(7,8);     //your pins to serial communication
int valor;
//-------------------------------------------------------------
//---------------------Ubidots Configuration-------------------
//-------------------------------------------------------------
String token = "YRWsXoUOptheqaDFN2T58o65hyHe4m";      //your token to post a value
String idvariable = "5408256b76254210e3acafbf";       //ID of your variable
void setup()
{

  mySerial.begin(19200);                                //the GPRS baud rate
  Serial.begin(19200);                                  //the serial communication baud rate
  Serial.println("Starting");
  delay(2000);
}

void loop()
{
    Serial.print("Sending to Ubidots Value=");
    int value = analogRead(A0);                 //read pin A0 of the Arduino
    Serial.println(value);
    save_value(String(value));                  //call the save_value function
    if (mySerial.available())
    Serial.write(mySerial.read());
    delay(10000);
}
/*this function is to send the sensor data to the Ubidots, 
you can see the new value in the Ubidots after execute this function*/
void save_value(String value)
{
  Serial.print("1");
  int num;
  String le;
  String var;
  var="{\"value\":"+ value + "}";
  num=var.length();
  le=String(num);
  for(int i = 0;i<7;i++)
  {
    mySerial.println("AT+CGATT?");           //this is better made repeatedly because it is unstable
    delay(2000);
    ShowSerialData();
  }
  //Serial.print("2");
  mySerial.println("AT+CSTT=\"internet.wind\""); //start task and set the APN
  delay(2000);
  ShowSerialData();
  mySerial.println("AT+CIICR");                  //bring up wireless connection
  delay(3000);
  ShowSerialData();
  mySerial.println("AT+CIFSR");                   //get local IP adress
  delay(2000);
  ShowSerialData();
  mySerial.println("AT+CIPSPRT=0");
  delay(3000);
  ShowSerialData();
  mySerial.println("AT+CIPSTART=\"tcp\",\"things.ubidots.com\",\"80\"");//start up connection
  delay(4000);
  ShowSerialData();
  mySerial.println("AT+CIPSEND");              //begin sendiing data to the remote server
  delay(7000);
  ShowSerialData();
  mySerial.print("POST /api/v1.6/variables/"+idvariable);
  delay(100);
  ShowSerialData();
  mySerial.println("/values HTTP/1.1");
  delay(100);
  ShowSerialData();
  mySerial.println("Content-Type: application/json");
  delay(100);
  ShowSerialData();
  mySerial.println("Content-Length: "+le);
  delay(100);
  ShowSerialData();
  mySerial.print("X-Auth-Token: ");
  delay(100);
  ShowSerialData();
  mySerial.println(token);
  delay(100);
  ShowSerialData();
  mySerial.println("Host: things.ubidots.com");
  delay(100);
  ShowSerialData();
  mySerial.println();
  delay(100);
  ShowSerialData();
  mySerial.println(var);
  delay(100);
  ShowSerialData();
  mySerial.println();
  delay(100);
  ShowSerialData();
  mySerial.println((char)26);
  delay(7000);
  mySerial.println();
  ShowSerialData();
  mySerial.println("AT+CIPCLOSE");            //close communication
  delay(1000);
  ShowSerialData();
  Serial.println("3");
}

void ShowSerialData()
{
  //Serial.print("4");
  while(mySerial.available()!=0)
  Serial.write(mySerial.read());
}
