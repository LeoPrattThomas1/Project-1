# Project Two- Automatic Headlight Control System 

## System Behavior Description 
Leo Pratt-Thomas and Catie Amirault                                                                                                                                       

This system takes a driver's education car and implements a headlight system that functions based off of the brightness outside. In accordance with the New York State Department of Motor Vehicle's rules the ignition can only turn on when both the passenger and 
the driver are sitting down and both seatbelts are fastened. If the conditions are met a green LED will turn on and then will turn off when the igntion is lit and a blue LED turns on instead.If one or more of those conditions are not true then the ignition will 
fail to start and an alarm will sound. In addition the driver wil get an error message explaning why the car will not turn on. The driver can use this error message to fix the problem and try again to turn on the ignition.  If the driver pushes
the ignition button while the ignition is on, it will turn off. The headlight system is implemented so that the driver of the car can set the headlights to auto and won't have to manually turn the headlights on or off, although that is an option. 
If on auto and it's bright outside the lights turn/ stay off. If on auto and its dark outide than the lights turn/ stay on. IF the driver turns the car off the healdights will also turn off. 

## Project Behavior Table 

|Specification                                                                            |        Test Result          |         Comment
|------------------|-----------------------------|----------------------
| When the driver sits down, display the message, “Welcome to enhanced alarm system model 218-W25”|         pass    |         
| Start the engine when the ignition is enabled and ignition button is pushed                     |         pass  |      Engine stays on when seatbelts are off or seat(s) are not occupied 
| If the ignition is lit, normal ignition occurs, the blue LED turns on and the green LED off . Display the message, “Engine started.”    | pass| Only happens when both seats are occupied and both seatbelts on                                                                     
| If the ignition is not enabled then ignition is inhibited. Sound the alarm buzzer and display the message, “Ignition inhibited” and why the ignition was inhibited | pass | Also allows for additional start attempts 
| Stop the engine when the ignition button is pushed and released when running                 | pass | 
| If head lights are turned on using the dial, both low beam lamps turn on and if the dial turn them off, both lamps turn off | pass|
| When on auto the the light level is above a “daylight” level, headlights turn off after 2 seconds. | pass |
| When the light level is below a “dusk” level the headlights turn on after 1 second| pass|
| When between these two levels, the headlights remain on or off| pass|
| Turn off all headlights when the engine is off | pass| 
