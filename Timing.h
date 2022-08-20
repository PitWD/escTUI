
//		       Timer Functions - See detailed description at the EOF !
//				! Using 3rd party MIT/GPL/"FreeSoftware" technologies !
//				  (see related declarations/functions and at the EOF)
// This is a  ! p_o_o_r !  Header by (c) Pit Demmer. (PIT-Licensed 20.08.2022 -           )

#include <time.h>

//CLOCKS_PER_SEC you can't trust - main app has to call CalcClocksPerSecond at least ones!
// !!! Initial 45000 is just fine with MY development environment !!!
clock_t clocksPerSecond = 45000;

// Real System Time
clock_t clockLast;
clock_t clockNow;
time_t timeLast;
time_t timeNow;
double timeDiff;
struct tm *localTime;

// Events
int secondChanged = 0;  // A Second Is Over
int minuteChanged = 0;  // A Minute
int hourChanged = 0;    // A Hour 
int dayChanged = 0;     // A Day

// Counter
char runTimeSeconds = 0;
char runTimeMinutes = 0;
char runTimeHours = 0;
int runTimeDays = 0;


// TimeIn / TimeOut to recognize ESC from User/Keyboard
clock_t userEscTimeout = 0.1 * 45000;
// TimeIn / TimeOut to recognize MouseClick as a single Click (not part of a DoubleClick)
clock_t mouseClickTimeout = 0.25 * 45000;

// 01/01/2000
time_t JAN_01_2000 = 946677600;

void InitTiming(void){
    time(&timeLast);
    clockLast = clock();
}

void CheckOnTimeChange(void){

    time(&timeNow);
    timeDiff = difftime(timeNow,timeLast);
    if (timeDiff){

        // Adjust clocksPerSecond
        clockNow = clock();
        clocksPerSecond = (clockNow - clockLast) / timeDiff;
        clockLast = clockNow;
        clock_t userEscTimeout = 0.1 * clocksPerSecond;
        clock_t mouseClickTimeout = 0.25 * clocksPerSecond;


        // A Second (ore more) is over
        timeLast = timeNow;
        secondChanged = 1;

        //System Runtime
        runTimeSeconds++;
        if (runTimeSeconds = 60){
            runTimeSeconds = 0;
            runTimeMinutes++;
            if (runTimeMinutes == 60){
                runTimeMinutes = 0;
                runTimeHours++;
                if (runTimeHours == 24){
                    runTimeHours = 0;
                    runTimeDays++;
                }
            }
        }

        // Real Time
        localTime = localtime(&timeNow);
        if (localTime->tm_sec == 0){
            minuteChanged = 1;
            if (localTime->tm_min == 0){
                hourChanged = 1;
                if (localTime->tm_hour == 0){
                    dayChanged = 1;
                }  
            }  
        }  
    }
}


