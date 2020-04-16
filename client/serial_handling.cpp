//------------------------------------------------------
// Names: Amro Amanuddein and Pushkar Sabharwal
// ID's: 1572498 (Amro) 1588927 (Pushkar)
// CMPUT275, Winter 2020
//
// Assignment 2 Part 2: Driving Route Finder
//------------------------------------------------------


#include "serial_handling.h"
#include <Arduino.h>
#include "consts_and_types.h"
extern shared_vars shared;


// Sends the Acknowledgement to the server
void sendAcknowledgement(){
  Serial.println("A");
}

// requestRoute will send start longitute and latitude and end longitute and latitude from Arduino
// to the server 
void requestRoute(const lon_lat_32& start, const lon_lat_32& end) {
  Serial.flush();
  Serial.print("R ");
  Serial.print(start.lat);
  Serial.print(" ");
  Serial.print(start.lon);
  Serial.print(" ");
  Serial.print(end.lat);
  Serial.print(" ");
  Serial.print(end.lon);
  Serial.println();
  Serial.flush();
}


// Reads data send from destop to Serial and returns true if there is no timeout and false otherwise
bool get_num(String& input, unsigned long long timeout = 1000){
  uint8_t bufferindex = 0;
  char chars_input;
  char buffer[40];
  auto start = millis();
  while((timeout+start) >= millis()){
    // if there is a message in serial
    if(Serial.available()){
      chars_input = Serial.read();
      buffer[bufferindex]=chars_input;
      bufferindex++;
    }
    // end of the message
    if(chars_input=='\r'|| chars_input == '\n'){
      // terminate the array with newline
      input = String(buffer);
      return true;
    }
  }
  // timeout
  return false;
}


// Function taken from CMPUT 274's major
bool wait_on_serial( uint8_t nbytes, long timeout){
  unsigned long deadline = millis() + timeout;
  while (Serial.available()<nbytes && (timeout < 0 || millis() < deadline))
  {
    delay(1); 
  }
  return Serial.available() >= nbytes;
}

// Parses the given waypoints to take the latitude and longitude using string operations
// and converts them to integers.
lon_lat_32 read_nums(String s){
  lon_lat_32 point;
  String substring1;
  String substring2;
  substring1 = (s.substring(0, s.indexOf(" ")));
  int32_t latitude = (substring1).toInt();
  substring2 = (s.substring(s.indexOf(" ")+1));
  int32_t longitude = (substring2).toInt();
  point.lat = latitude;
  point.lon = longitude;
  return point;
};


// Comunicates over serial port using the communication protocol 
uint8_t get_waypoints(const lon_lat_32& start, const lon_lat_32& end) {
  String input;
  requestRoute(start,end);
  status_message("Recieving path...");
  if(get_num(input)){
    // When all the waypoints are printed, E is printed at the end for the end of request
    if (input[0]== 'E'){
      // Return 1 for successful data exchange
      return 1;
    }
  }
  if(get_num(input)){
    //
    if(input[0]=='N'){
      String num = input.substring(2);
      shared.num_waypoints = num.toInt();
      sendAcknowledgement();
    }
    else{
      return 0;
    }
  }
  else{
    return 0;
  }
  // When there is no path because there are no waypoints or if there are greater than 500 waypoints
  if(shared.num_waypoints==0 || shared.num_waypoints >= 500){
    status_message("NO PATH!");
  }
  //  returns all of the waypoints
  for(int16_t i = 0; i < shared.num_waypoints; i++){
    if(get_num(input)){
      if(input[0]=='W'){
        String s = input.substring(2);
        shared.waypoints[i] = read_nums(s);
        sendAcknowledgement();
      }
      else{
        return 0;
      }
    }
    else{
      return 0;
    }
  }
  // Returns 1 for successful communication
  return 1;
}












