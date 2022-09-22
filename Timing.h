
//		       Timer Functions - See detailed description at the EOF !
//				! Using 3rd party MIT/GPL/"FreeSoftware" technologies !
//				  (see related declarations/functions and at the EOF)
// This is a  ! p_o_o_r !  Header by (c) Pit Demmer. (PIT-Licensed 20.08.2022 -           )

#include <time.h>

// CLOCKS_PER_SEC you can't trust - main app has to call CalcClocksPerSecond at least ones!
// !!! Initial 45000 is just fine with MY development environment !!!
// !!! On WIN it seems to be constant 1000 - like it's on most Posix 1000000
clock_t clocksPerSecond = CLOCKS_PER_SEC;

// Real System Time
clock_t clockLast;
clock_t clockNow;
time_t timeNow;
time_t timeLast;
double timeDiff;
struct tm *localTime;

// Events
int secondChanged = 0;  // A Second Is Over
int minuteChanged = 0;  // A Minute
int hourChanged = 0;    // A Hour 
int dayChanged = 0;     // A Day
int min2Changed = 0;    // All 2 minutes...
int min3Changed = 0;
int min4Changed = 0;
int min5Changed = 0;
int min6Changed = 0;
int min10Changed = 0;
int min12Changed = 0;
int min15Changed = 0;
int min20Changed = 0;
int min30Changed = 0;
int hour2Changed = 0;   // All 2 hours...
int hour3Changed = 0;
int hour4Changed = 0;
int hour6Changed = 0;
int hour8Changed = 0;
int hour12Changed = 0;

// Counter
char runTimeSeconds = 0;
char runTimeMinutes = 0;
char runTimeHours = 0;
int runTimeDays = 0;

// TimeIn / TimeOut to recognize ESC from User/Keyboard
clock_t userEscTimeout; // = 0.1 * 45000;
// TimeIn / TimeOut to recognize MouseClick as a single Click (not part of a DoubleClick)
clock_t mouseClickTimeout;  // = 0.25 * 45000;
// DoEventsTime for usleep() (Linux & Mac)
#define DoEventsTime 10000

// 01/01/2000
time_t JAN_01_2000 = 946677600;

void InitTiming(void);
void EraseTimeChange(void);
void CheckOnTimeChange(void);

void InitTiming(void){
    time(&timeLast);
    clockLast = clock();

    #if __WIN32__ || _MSC_VER || __WIN64__

        clocksPerSecond = CLOCKS_PER_SEC;
    #else

        clocksPerSecond = 45000;
    #endif
    
    userEscTimeout = 0.1 * clocksPerSecond;
    mouseClickTimeout = 0.25 * clocksPerSecond;
}

void EraseTimeChange(void){
    secondChanged = 0;  // A Second Is Over
    minuteChanged = 0;  // A Minute
    hourChanged = 0;    // A Hour 
    dayChanged = 0;     // A Day
    min2Changed = 0;    // All 2 minutes...
    min3Changed = 0;
    min4Changed = 0;
    min5Changed = 0;
    min6Changed = 0;
    min10Changed = 0;
    min12Changed = 0;
    min15Changed = 0;
    min20Changed = 0;
    min30Changed = 0;
    hour2Changed = 0;   // All 2 hours...
    hour3Changed = 0;
    hour4Changed = 0;
    hour6Changed = 0;
    hour8Changed = 0;
    hour12Changed = 0;
}

void CheckOnTimeChange(void){

    time(&timeNow);
    timeDiff = difftime(timeNow,timeLast);
    if (timeDiff){

        // Adjust clocksPerSecond
        clockNow = clock();
        clocksPerSecond = (clockNow - clockLast) / timeDiff;
        clockLast = clockNow;
        userEscTimeout = 0.1 * clocksPerSecond;
        mouseClickTimeout = 0.25 * clocksPerSecond;

        int sec00Overflow = 0;

        // A Second (ore more) is over
        if (timeDiff > 1){
            // More...
            localTime = localtime(&timeLast);
            if (localTime->tm_sec + timeDiff > 60){
                // We would have missed xx:xx:00
                // We'll "simulate" to get all TimeChangeEvents right
                sec00Overflow = 1;
            }
        }
    
        timeLast = timeNow;
        secondChanged = 1;

        //System Runtime
        runTimeSeconds += timeDiff;
        if (runTimeSeconds >= 60){
            runTimeSeconds -= 60;
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
        // Real-Time Events
        if (localTime->tm_sec == 0 || sec00Overflow){
            minuteChanged = 1;
            if (localTime->tm_min == 0){
                min2Changed = 1;
                min3Changed = 1;
                min4Changed = 1;
                min5Changed = 1;
                min6Changed = 1;
                min10Changed = 1;
                min12Changed = 1;
                min15Changed = 1;
                min20Changed = 1;
                min30Changed = 1;

                hourChanged = 1;

                if (localTime->tm_hour == 0){
                    hour2Changed = 1;
                    hour3Changed = 1;
                    hour4Changed = 1;
                    hour6Changed = 1;
                    hour8Changed = 1;
                    hour12Changed = 1;

                    dayChanged = 1;
                }
                else{
                    if (!(localTime->tm_hour % 2)){
                        hour2Changed = 1;
                    }
                    if (!(localTime->tm_hour % 3)){
                        hour3Changed = 1;
                    }
                    if (!(localTime->tm_hour % 4)){
                        hour4Changed = 1;
                    }
                    if (!(localTime->tm_hour % 6)){
                        hour6Changed = 1;
                    }
                    if (!(localTime->tm_hour % 8)){
                        hour8Changed = 1;
                    }
                    if (!(localTime->tm_hour % 12)){
                        hour12Changed = 1;
                    }                  
                }               
            }
            else{
                if (!(localTime->tm_min % 2)){
                    min2Changed = 1;
                }
                if (!(localTime->tm_min % 3)){
                    min3Changed = 1;
                }
                if (!(localTime->tm_min % 4)){
                    min4Changed = 1;
                }
                if (!(localTime->tm_min % 5)){
                    min5Changed = 1;
                }
                if (!(localTime->tm_min % 6)){
                    min6Changed = 1;
                }
                if (!(localTime->tm_min % 10)){
                    min10Changed = 1;
                }
                if (!(localTime->tm_min % 12)){
                    min12Changed = 1;
                }
                if (!(localTime->tm_min % 15)){
                    min15Changed = 1;
                }
                if (!(localTime->tm_min % 20)){
                    min20Changed = 1;
                }
                if (!(localTime->tm_min % 30)){
                    min30Changed = 1;
                }
            }              
        }  
    }
}


