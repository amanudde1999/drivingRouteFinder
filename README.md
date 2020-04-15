# Driving Route Finder

Using the joystick and the cursor that moves on the TFT screen you can now find the shortest route between two locations in Edmonton!


Files included are:
--------------------------------------

	client:
    -client.cpp
    -const_and_types.h
    -draw_route.cpp
    -draw_route.h
    -lcd_image.cpp
    -lcd_image.h
    -Makefile
    -map_drawing.cpp
    -map_drawing.h
    -serial_handling.cpp
    -serial_handling.h

    server:
    -digraph.cpp
    -digraph.h
    -dijkstra.h
    -edmonton-roads-2.0.1.txt
    -heap.h
    -Makefile
    -serialport.cpp
    -serialport.h
    -server.cpp
    -wdigraph.h

    - README

Wiring Instructions:
---------------------
- Joystick:

| Display Pins | Arduino Pins |
| ------------ | ------------ |
| VRx | A8 |
| VRy | A9 |
| SW  | Digital Pin 53|
| GND | GND |
| +5V | 5V |

- Buttons:

| Display Pins | Arduino Pins |
| ------------ | ------------ |
| Zoom Out Button | Digital Pin 47 |
| Zoom In Button  | Digital Pin 45 |

- Screen:

| Display Pins | Arduino Pins |
| ------------ | ------------ |
|TFT Screen  | Arduino|


Running Instructions:
---------------------

- Wire up the arduino to the computer as per the wiring instructions.
- Go to the client folder and type: 
                        
                        make upload
                        
- Open a new terminal and go to server folder type: 
                      
                      make && serial-mon
                      
- Once everything finishes up loading, the display will show a map of edmonton. Use the Joystick to hover over the    map.
- Now press the Joystick button by bringing the cursor to that point and clicking the Joystick button. Then go to   the point wherever you want the route to and press the button again.
- This will draw the line from start to the end which gives the shortest path.


References:
-------------
- instructions.txt from eclass
- serialport communications within C++ from eclass
- CMPUT275 assignment 1 part 2: Restaraunt Finder
- CMPUT274 assignment 2 part 1: 
- For the split function: // https://stackoverflow.com/questions/11719538/how-to-use-stringstream-to-separate-comma-separated-strings
- Library for sstream: http://www.cplusplus.com/reference/sstream/stringstream/
