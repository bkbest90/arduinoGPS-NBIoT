#include "TinyGPS++.h"
#include "SoftwareSerial.h"
#include "Magellan.h"

Magellan magel;
char auth[]="5d788b90-4ead-11e8-bd62-15911fb825ae";     //Token Key you can get from magellan platform
String payload,latitude = "",longitude = "";

SoftwareSerial serial_connection(10, 11); //TX=pin 10, RX=pin 11
TinyGPSPlus gps;//This is the GPS object that will pretty much do all the grunt work with the NMEA data
void setup()
{
  Serial.begin(9600);//This opens up communications to the Serial monitor in the Arduino IDE
  Serial.println("Start device");
}

void loop()
{
  serial_connection.begin(9600);//This opens up communications to the GPS
  while(serial_connection.available())//While there are characters to come from the GPS
  {
    gps.encode(serial_connection.read());//This feeds the serial NMEA data into the library one char at a time
  }
  
  if(gps.location.isUpdated())//This will pretty much be fired all the time anyway but will at least reduce it to only after a package of NMEA data comes in
  {
    //Get the latest info from the gps object which it derived from the data sent by the GPS unit
    Serial.println("Get latitude & longitude values");
   
    latitude=String(gps.location.lat(), 6);
    longitude=String(gps.location.lng(), 6);
    Serial.println("GPS communication Stoped");
    serial_connection.end();//This stop the GPS communications
    
  }
    if(latitude!=""){
      magel.begin(auth);//Start communication of magellan
      payload="{\"latitude\":"+latitude+",\"longitude\":"+longitude+"}";   
      magel.post(payload); //Push to magellan
      delay(1000); 
      latitude = "";
      longitude="";
      }
}
