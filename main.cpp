//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

//=====[Defines]===============================================================

//Null

//=====[Declarations (prototypes) of public functions]=========================


//=====[Main function, the program entry point after power on or reset]========

int main()
{
    inputsInit();
    outputsInit();
    while (true) {
        alarmActivationUpdate();
        alarmDeactivationUpdate();
        uartTask();
        delay(TIME_INCREMENT_MS);
    }
}


