//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include <string>

//=====[Defines]===============================================================

//Butons
DigitalIn driver_occupancy(D4);
DigitalIn passenger_occupancy(D5);
DigitalIn ignition_BTN(D3);

//Swiches
DigitalIn driver_seatbelt(D8);
DigitalIn passenger_seatbelt(D9);



DigitalOut good_passenger_LED(LED1);
DigitalOut ignition(LED2);
/*
Driver seatbelt sensor 
Passenger seatbelt sensor 
*/

//Alarm buzzer 
DigitalInOut alarm_buzzer(PE_10);

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//=====[Declarations (prototypes) of public functions]=========================

void inputsInit();
void outputsInit();
void car();
void print_to_uart(string);

void part1();
void part2();
void part3();

//=====[Main function, the program entry point after power on or reset]========

int main()
{
    inputsInit();

    part1();
    part2();

    part3();
    /*while (true){ 
        //alarm_buzzer.output();  
    }*/
}

void inputsInit()
{
    driver_occupancy.mode(PullDown);
    passenger_occupancy.mode(PullDown);
    driver_seatbelt.mode(PullDown);
    passenger_seatbelt.mode(PullDown);

    ignition_BTN.mode(PullDown);

    alarm_buzzer.mode(OpenDrain);
    alarm_buzzer.input();
}

/*
 When the driver sits down, display the message, “Welcome to enhanced alarm system model 218-W24”.
Light the green LED only when both seats are occupied and both seatbelts are fastened.
 When the ignition button is pushed:
-        If the green LED is lit, then normal ignition occurs. Light the blue LED and extinguish the green LED. Display the message, “Engine started.”
-        If the green LED is not lit, then ignition is inhibited. Sound the alarm buzzer; display the message, “Ignition inhibited” and display all the reasons why the ignition was inhibited: “Passenger seat not occupied,” “Driver seatbelt not fastened,” etc.
*/




void part1(){
    //When the driver sits down, //display the message, “Welcome to enhanced alarm system model 218-W24”.
    

    //while the button is not presseed wait till the button is pressed.
    while (driver_occupancy == OFF){
        //None
    }
    uartUsb.write( "Welcome to enhanced alarm system model 218-W24\r\n", 48 );

}

void part2(){
    //Light the green LED only when both seats are occupied and both seatbelts are fastened.
    if ((driver_occupancy && passenger_occupancy) && (driver_seatbelt && passenger_seatbelt)){
        good_passenger_LED = ON;
    } else {
        good_passenger_LED = OFF;
    }
}


// When the ignition button is pushed:

//  If the green LED is lit, 
//      then normal ignition occurs. 
//      Light the blue LED and extinguish the green LED. 
//      Display the message, “Engine started.”

// If the green LED is not lit, 
//      then ignition is inhibited. 
//      Sound the alarm buzzer; 
//      display the message, “Ignition inhibited” 
//      and display all the reasons why the ignition was inhibited: 
//              “Passenger seat not occupied,” “Driver seatbelt not fastened,” etc.
void part3(){
    while (ignition_BTN == OFF){
        //None
    }

    if (good_passenger_LED == ON){
        //normal ignition occurs. 
        ignition = ON;

        //Light the blue LED and extinguish the green LED. 
        good_passenger_LED = OFF;

        //Display the message, “Engine started.”  
        uartUsb.write( "Engine started.\r\n", 15+2);



    } else {
//      then ignition is inhibited. 


//      Sound the alarm buzzer; 
        alarm_buzzer.output(); 

//      display the message, “Ignition inhibited” 
        uartUsb.write( "Ignition inhibited\r\n", 18+2);
//      and display all the reasons why the ignition was inhibited: 


        if (!driver_occupancy){// driver_occupancy
            uartUsb.write( "Driver seat not occupied\r\n", 19+6+2);
        } if (!passenger_occupancy){// paxwssenger_occupancy
            uartUsb.write( "Passenger seat not occupied\r\n", 19+9+2);
        } if (!driver_seatbelt){// driver_seatbelt
            uartUsb.write( "Driver seatbelt not fastened\r\n", 22+6+2);
        } if (!passenger_seatbelt){// passenger_seatbelt
            uartUsb.write( "Passenger seatbelt not fastened\r\n", 22+9+2);
//              “Passenger seat not occupied,” “Driver seatbelt not fastened,” etc. 
        }
    }

}







//Light the green LED 
//  only when 
//      (both seats are occupied) AND (both seatbelts are fastened)



/*
void alarmActivationUpdate()
{
    static int lm35SampleIndex = 0;
    int i = 0;

    lm35ReadingsArray[lm35SampleIndex] = lm35.read();
    lm35SampleIndex++;
    if ( lm35SampleIndex >= NUMBER_OF_AVG_SAMPLES) {
        lm35SampleIndex = 0;
    }
    
       lm35ReadingsSum = 0.0;
    for (i = 0; i < NUMBER_OF_AVG_SAMPLES; i++) {
        lm35ReadingsSum = lm35ReadingsSum + lm35ReadingsArray[i];
    }
    lm35ReadingsAverage = lm35ReadingsSum / NUMBER_OF_AVG_SAMPLES;
       lm35TempC = analogReadingScaledWithTheLM35Formula ( lm35ReadingsAverage );    
    
    if ( lm35TempC > OVER_TEMP_LEVEL ) {
        overTempDetector = ON;
    } else {
        overTempDetector = OFF;
    }

    if( !mq2) {
        gasDetectorState = ON;
        alarmState = ON;
    }
    if( overTempDetector ) {
        overTempDetectorState = ON;
        alarmState = ON;
    }
    if( alarmTestButton ) {             
        overTempDetectorState = ON;
        gasDetectorState = ON;
        alarmState = ON;
    }    
    if( alarmState ) { 
        accumulatedTimeAlarm = accumulatedTimeAlarm + TIME_INCREMENT_MS;
        sirenPin.output();                                     
        sirenPin = LOW;                                        
    
        if( gasDetectorState && overTempDetectorState ) {
            if( accumulatedTimeAlarm >= BLINKING_TIME_GAS_AND_OVER_TEMP_ALARM ) {
                accumulatedTimeAlarm = 0;
                alarmLed = !alarmLed;
            }
        } else if( gasDetectorState ) {
            if( accumulatedTimeAlarm >= BLINKING_TIME_GAS_ALARM ) {
                accumulatedTimeAlarm = 0;
                alarmLed = !alarmLed;
            }
        } else if ( overTempDetectorState ) {
            if( accumulatedTimeAlarm >= BLINKING_TIME_OVER_TEMP_ALARM  ) {
                accumulatedTimeAlarm = 0;
                alarmLed = !alarmLed;
            }
        }
    } else{
        alarmLed = OFF;
        gasDetectorState = OFF;
        overTempDetectorState = OFF;
        sirenPin.input();                                  
    }
}
*/

/*
Typical Driver’s Ed. vehicles are already equipped with the following devices: 
Driver seat occupancy sensor (provides contact closure when occupied);

BTN


Passenger seat occupancy sensor (provides contact closure when occupied);

BTN


Driver seatbelt sensor (provides contact closure when fastened);

SW


Passenger seatbelt sensor (provides contact closure when fastened);

SW


Ignition push button (provides contact closure to request ignition);

BTN



Alarm buzzer (sounds when ignition is inhibited);

buzzer


Green indicator LED (lights when ignition is enabled);
 Blue indicator LED (lights when the engine has started);
Display monitor (displays welcome and error messages);
*/


