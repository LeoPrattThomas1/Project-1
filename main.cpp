//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

//=====[Defines]===============================================================


#define DAYLIGHT_DELAY                          2000
#define NIGHTTIME_DELAY                         1000
#define TIME_INCREMENT_MS                       10

//Butons
DigitalIn driver_occupancy(D4);
DigitalIn passenger_occupancy(D5);
DigitalIn ignition_BTN(BUTTON1);

//Swiches
DigitalIn driver_seatbelt(D8);
DigitalIn passenger_seatbelt(D9);

//Potentemeter
AnalogIn potentiometer(A0);
AnalogIn lightSensor(A1);


DigitalOut good_passenger_LED(LED1);
DigitalOut ignition(LED2);
DigitalOut headlights(D7);
/*
Driver seatbelt sensor 
Passenger seatbelt sensor 
*/

//Alarm buzzer 
DigitalInOut alarm_buzzer(PE_10);

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//=====[Declarations (prototypes) of public functions]=========================

void inputsInit();

void car();
void print_to_uart(string);
void attemptEngineStart();
void readLedUpdate();

void initWelecome();
void updateIgnition();

//headlight code
void updateHeadlights();
void pickHeadlightMode();
void runHeadlights();

void runAuto();


typedef enum {
    ON_STATE,
    OFF_STATE,
    AUTO,
} Headlight_t;





Headlight_t headlightState;
float potentiometerReading = 0.0;
int accumulatedTimeAlarm = 0;
bool lastHeadlightState;
bool lastIgnitionState;

//=====[Main function, the program entry point after power on or reset]========

int main()
{
    inputsInit();
    initWelecome();

    while (true){
        updateIgnition();
        updateHeadlights();
        delay(TIME_INCREMENT_MS);
    }
}


void inputsInit()
{
    driver_occupancy.mode(PullDown);
    passenger_occupancy.mode(PullDown);
    // driver_seatbelt.mode(PullDown);
    // passenger_seatbelt.mode(PullDown);

    ignition_BTN.mode(PullDown);

    alarm_buzzer.mode(OpenDrain);
    alarm_buzzer.input();

    
}

void initWelecome(){
    //PART 1
    //while the button is not presseed wait till the button is pressed.
    while (driver_occupancy == OFF){     }

    //display the message, “Welcome to enhanced alarm system model 218-W24”.
    uartUsb.write( "Welcome to enhanced alarm system model 218-W24\r\n", 48 );

}

//Keep the engine running even if the driver or passenger should remove their seat belts or exit the vehicle.
//When the engine is running, stop the engine when the ignition button is pushed and then released. (note this change from Project 1 requirements)
void updateIgnition(){
    //When the engine is running, stop the engine when the ignition button is pushed and then released.
    if (ignition == ON){

        //wait for button down
        lastIgnitionState = false;
        while (ignition_BTN == ON) { 
            lastIgnitionState = true;
        }

        if (lastIgnitionState){
            ignition = OFF;
        }
        
    // Once the error messages are displayed, the system allows additional start attempts.
    } else {
        readLedUpdate();
        attemptEngineStart();
    }

    lastIgnitionState = ignition_BTN;
}
    
void readLedUpdate(){
    if ((driver_occupancy && passenger_occupancy) && (driver_seatbelt && passenger_seatbelt)){
        good_passenger_LED = ON;
    } else {
        good_passenger_LED = OFF;
    }
}

void attemptEngineStart(){
    if (ignition_BTN == ON){
        if (good_passenger_LED == ON){

            //normal ignition occurs. 
            ignition = ON;

            alarm_buzzer.input();

            //Light the blue LED and extinguish the green LED. 
            good_passenger_LED = OFF;

            //Display the message, “Engine started.”  
            uartUsb.write( "Engine started.\r\n", 15+2);

        } else {

            // Sound the alarm buzzer; 
            alarm_buzzer.output(); 

            // display the message, “Ignition inhibited” 
            uartUsb.write( "Ignition inhibited\r\n", 18+2);

            // and display all the reasons why the ignition was inhibited: 
            if (!driver_occupancy){// driver_occupancy
                uartUsb.write( "Driver seat not occupied\r\n", 19+6+2);

            } if (!passenger_occupancy){// pasenger_occupancy
                uartUsb.write( "Passenger seat not occupied\r\n", 19+9+2);

            } if (!driver_seatbelt){// driver_seatbelt
                uartUsb.write( "Driver seatbelt not fastened\r\n", 22+6+2);

            } if (!passenger_seatbelt){// passenger_seatbelt
                uartUsb.write( "Passenger seatbelt not fastened\r\n", 22+9+2);
            }
        }
    }

    // only run once
    while (ignition_BTN == ON) { }
}


// Headlight subsystem
// If the engine is running, and the user selects: 
// ON, turn on both low beam lamps; if they select OFF, turn off all lamps.
void updateHeadlights(){
    pickHeadlightMode();
    runHeadlights();

}

//Lets user pick current state
void pickHeadlightMode(){
    potentiometerReading = potentiometer.read();

    //ON STATE
    if (potentiometerReading >= 0.6){
        headlightState = ON_STATE;
        // uartUsb.write( "On\r\n", 4);

    // the user selects auto 
    } if (0.6 > potentiometerReading >= 0.3) {
        headlightState = AUTO;
        // uartUsb.write( "Auto\r\n", 6);
    
    //OFF STATE
    } if (0.3 > potentiometerReading){
        headlightState = OFF_STATE;
        // uartUsb.write( "Off\r\n", 5);
    }

}

void runHeadlights(){

        if (ignition == ON){
        switch (headlightState) {
    
        // ON, turn on both low beam lamps;
        case ON_STATE:
            headlights = ON;
            lastHeadlightState = ON;
            break;
        
        //if they select OFF, turn off all lamps.
        case OFF_STATE:
            headlights = OFF;
            lastHeadlightState = OFF;
            break;
        

        // If the user selects the AUTO position: 
        // When the light level is above a “daylight” level, switch headlights off after a delay of 2 seconds.
        // When the light level is below a “dusk” level, switch headlights on after a delay of 1 second.
        // When between these two levels, the headlights should hold their previous state. 
        case AUTO: 
        
            runAuto();

            break;
        //ON
        //OFF
        //Auto
        }
    }
    else{
        headlights = OFF;
        // Turn off all headlights when the engine is off. 
    }
}

void runAuto(){
    float read;
    int stringLength;
    char str[100];

    read = lightSensor.read();

    accumulatedTimeAlarm += TIME_INCREMENT_MS;

    //testing purposesqa
            
    if (lightSensor > 0.7){
        // delay(DAYLIGHT_DELAY);
        
        if (lastHeadlightState == ON){
            accumulatedTimeAlarm = 0;
            // uartUsb.write( "Light Low\r\n",11);
        }

        if (accumulatedTimeAlarm >= DAYLIGHT_DELAY){
            headlights = OFF;
        }

        lastHeadlightState = OFF;
    
    } else if (lightSensor < 0.5) {
        // delay(NIGHTTIME_DELAY);
        // headlights = ON;

        if (lastHeadlightState == OFF){
            accumulatedTimeAlarm = 0;
            // uartUsb.write( "Light High\r\n", 12);
        
        } if (accumulatedTimeAlarm >= NIGHTTIME_DELAY){
            headlights = ON;
        }
        

        lastHeadlightState = ON;
    }
}