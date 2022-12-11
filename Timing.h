
//		       Timer Functions - See detailed description at the EOF !
//				! Using 3rd party MIT/GPL/"FreeSoftware" technologies !
//				  (see related declarations/functions and at the EOF)
// This is a  ! p_o_o_r !  Header by (c) Pit Demmer. (PIT-Licensed 20.08.2022 -           )

#include <time.h>

#define IS_REVERSE_DEBUG 0
#define IS_TIME_EVENT_DEBUG 0
#define IS_TERMINAL_EVENT_DEBUG 0

// CLOCKS_PER_SEC you can't trust - main app has to call CalcClocksPerSecond at least ones!
// !!! Initial 45000 is just fine with MY development environment !!!
// !!! On WIN it seems to be constant 1000 - like it's on most Posix 1000000
clock_t gClocksPerSecond = CLOCKS_PER_SEC;
static clock_t lClockLast;

// Real System Time
static time_t lTimeNow;
static time_t lTimeLast;
int gHour = 0;
int gMin = 0;
int gSec = 0;
int gDay = 0;
int gMonth = 0;
int gYear = 0;
char gStrDate[] = "01.01.1970";
char gStrTime[] = "00:00:00      ";     // Trailing Spaces To Prevent Mac-Sleep-Overflow

// Events
int gSecondChanged = 0;  // A Second Is Over
int gMinuteChanged = 0;  // A Minute
int gHourChanged = 0;    // A Hour 
int gDayChanged = 0;     // A Day
int gMin2Changed = 0;    // All 2 minutes...
int gMin3Changed = 0;
int gMin4Changed = 0;
int gMin5Changed = 0;
int gMin6Changed = 0;
int gMin10Changed = 0;
int gMin12Changed = 0;
int gMin15Changed = 0;
int gMin20Changed = 0;
int gMin30Changed = 0;
int gHour2Changed = 0;   // All 2 hours...
int gHour3Changed = 0;
int gHour4Changed = 0;
int gHour6Changed = 0;
int gHour8Changed = 0;
int gHour12Changed = 0;

// Counter
int gRunTimeSec = 0;
int gRunTimeMin = 0;
int gRunTimeHour = 0;
int gRunTimeDays = 0;
char gStrRunTime[] = "00000d 00:00:00      ";   // Trailing Spaces To Prevent Mac-Sleep-Overflow

// TimeIn / TimeOut to recognize ESC from User/Keyboard
clock_t gUserEscTimeout; // = 0.1 * 45000;
// TimeIn / TimeOut to recognize MouseClick as a single Click (not part of a DoubleClick)
clock_t gMouseClickTimeout;  // = 0.25 * 45000;
// DoEventsTime for usleep() (Linux & Mac)
#define DoEventsTime 10000

// 01/01/2000
time_t JAN_01_2000 = 946677600;

/**
 * @brief Declaration FUNCTIONS in Timing.h
 */
void InitTiming(void);
void EraseTimeChange(void);
void CheckOnTimeChange(void);

/**
 * @brief   Setup Timing Constants / Variables / Settings
 *          Elementary to get called before using
 *          Terminal.h or Timing.h functions!
 * 
 * @private *pLocalTime (struct pm) helper to get RealTime 00:00:00
 * 
 * @internal    lClockLast (clock_t) helper for CheckOnTimeChange()
 * 
 * @internal    lTimeLast (time_t) helper for CheckOnTimeChange()
 * 
 * @internal    lTimeNow (time_t) helper to (also) get RealTime 00:00:00
 * 
 * @public  gDay, gMonth, gYear, gHour, gMin, gSec, gStrDate, gStrTime
 * 
 * @public  gClocksPerSecond, gUserEscTimeOut, gMouseClickTimeOut
*/
void InitTiming(void){

    struct tm *pLocalTime;

    time(&lTimeLast);
    time(&lTimeNow);
    lClockLast = clock();

    pLocalTime = localtime(&lTimeNow);

    gSec = pLocalTime->tm_sec;
    gMin = pLocalTime->tm_min;
    gHour = pLocalTime->tm_hour;
    gDay = pLocalTime->tm_mday;
    gMonth = pLocalTime->tm_mon + 1;
    gYear = pLocalTime->tm_year + 1900;

    sprintf(gStrTime,"%02d:%02d:%02d", gHour, gMin, gSec);
    sprintf(gStrDate,"%02d.%02d.%04d", gDay, gMonth, gYear);
    
    #if __WIN32__ || _MSC_VER || __WIN64__

        gClocksPerSecond = CLOCKS_PER_SEC;
    #else

        gClocksPerSecond = 45000;
    #endif
    
    gUserEscTimeout = 0.1 * gClocksPerSecond;
    gMouseClickTimeout = 0.25 * gClocksPerSecond;
}

/**
 * @brief   Reset Global TimeEventFlags
 */
void EraseTimeChange(void){
    gSecondChanged = 0;  // A Second Is Over
    gMinuteChanged = 0;  // A Minute
    gHourChanged = 0;    // A Hour 
    gDayChanged = 0;     // A Day
    gMin2Changed = 0;    // All 2 minutes...
    gMin3Changed = 0;
    gMin4Changed = 0;
    gMin5Changed = 0;
    gMin6Changed = 0;
    gMin10Changed = 0;
    gMin12Changed = 0;
    gMin15Changed = 0;
    gMin20Changed = 0;
    gMin30Changed = 0;
    gHour2Changed = 0;   // All 2 hours...
    gHour3Changed = 0;
    gHour4Changed = 0;
    gHour6Changed = 0;
    gHour8Changed = 0;
    gHour12Changed = 0;
}

/**
 * @brief   (Re)Calculate gClocksPerSecond depending on CPU-Power
 *          to then (Re)Calculate User-ESC & MouseClick TimeOut
 *          Calculate and set Runtime globals
 *          Set Realtime globals
 *          Calculate TimeEvents / Set Global TimeEvent-Flags
 * 
 * @private sec00Overflow (int)
 *          helper to handle TimeDiff > 1 sec right
 *          (call function at least twice per second to get ALL
 *           events with a +/- 1sec. tolerance.)
 * 
 * @private timeDiff (double) helper to calculate time since last call
 *          it's just full seconds - but difftime()-result is double...
 * 
 * @private *pLocalTime (struct tm) helper to calculate time since last call
 * 
 * @private clockNow (clock_t) helper to calculate clocks() since last call
 * 
 * @internal    lClockLast (clock_t) helper to calculate clocks() since last call
 * 
 * @internal    lTimeLast (clock_t) helper to calculate time since last call
 * 
 * @public  gRunTimeDays, gRuntimeHours, gRunTimeMin, gRunTimeSec, gStrRunTime
 * 
 * @public  gClocksPerSecond, gUserEscTimeOut, gMouseClickTimeOut
 * 
 * @public  gDay, gMonth, gYear, gHour, gMin, gSec, gStrDate, gStrTime
 * 
 * @public  gSecondChanged, gDayChanged + all gMin*Changed, gHour*Changed
 *  
 */
void CheckOnTimeChange(void){

    int sec00Overflow = 0;
    double timeDiff = 0;
    struct tm *pLocalTime;
    clock_t clockNow;

    time(&lTimeNow);
    timeDiff = difftime(lTimeNow,lTimeLast);

    // A Second (ore more) is over
    if (timeDiff){

        // Adjust clocksPerSecond
        clockNow = clock();
        gClocksPerSecond = (clockNow - lClockLast) / timeDiff;
        lClockLast = clockNow;
        gUserEscTimeout = 0.1 * gClocksPerSecond;
        #if __APPLE__
            // MAC is sending MouseDown together with MouseUp when user releases button
            // Win & Linux is sending MouseDown when user presses MouseDown
            gMouseClickTimeout = 0.33 * gClocksPerSecond;
        #else
            gMouseClickTimeout = 0.25 * gClocksPerSecond;
        #endif

        if (timeDiff > 1){
            // More than a second...
            pLocalTime = localtime(&lTimeLast);
            if (pLocalTime->tm_sec + timeDiff > 60){
                // We would have missed xx:xx:00
                // We'll have to "simulate" to get all TimeChangeEvents right
                sec00Overflow = 1;
            }
        }

        lTimeLast = lTimeNow;
        gSecondChanged = 1;

        //System Runtime
        gRunTimeSec += timeDiff;
        if (gRunTimeSec >= 60){
            gRunTimeSec -= 60;
            gRunTimeMin++;
            if (gRunTimeMin >= 60){
                gRunTimeMin -= 60;
                gRunTimeHour++;
                if (gRunTimeHour >= 24){
                    gRunTimeHour -= 24;
                    gRunTimeDays++;
                }
            }
        }
        sprintf(gStrRunTime,"%5dd %02d:%02d:%02d", gRunTimeDays, gRunTimeHour, gRunTimeMin, gRunTimeSec);

        // Real Time
        pLocalTime = localtime(&lTimeNow);

        gSec = pLocalTime->tm_sec;
        
        // Real-Time Events
        if (gSec == 0 || sec00Overflow){
            gMinuteChanged = 1;
            gMin = pLocalTime->tm_min;
            if (gMin == 0){
                gMin2Changed = 1;
                gMin3Changed = 1;
                gMin4Changed = 1;
                gMin5Changed = 1;
                gMin6Changed = 1;
                gMin10Changed = 1;
                gMin12Changed = 1;
                gMin15Changed = 1;
                gMin20Changed = 1;
                gMin30Changed = 1;

                gHourChanged = 1;
                gHour = pLocalTime->tm_hour;

                if (gHour == 0){
                    gHour2Changed = 1;
                    gHour3Changed = 1;
                    gHour4Changed = 1;
                    gHour6Changed = 1;
                    gHour8Changed = 1;
                    gHour12Changed = 1;

                    gDayChanged = 1;

                    gDay = pLocalTime->tm_mday;
                    gMonth = pLocalTime->tm_mon + 1;
                    gYear = pLocalTime->tm_year + 1900;
                    sprintf(gStrDate,"%02d.%02d.%04d", gDay, gMonth, gYear);
                }
                else{
                    if (!(gHour % 2)){
                        gHour2Changed = 1;
                        if (!(gHour % 4)){
                            gHour4Changed = 1;
                            if (!(gHour % 8)){
                                gHour8Changed = 1;
                            }
                        }
                    }
                    if (!(gHour % 3)){
                        gHour3Changed = 1;
                        if (!(gHour % 6)){
                            gHour6Changed = 1;
                            if (!(gHour % 12)){
                                gHour12Changed = 1;
                            }                  
                        }
                    }
                }               
            }
            else{
                if (!(gMin % 2)){
                    gMin2Changed = 1;
                    if (!(gMin % 4)){
                        gMin4Changed = 1;
                    }
                }
                if (!(gMin % 3)){
                    gMin3Changed = 1;
                    if (!(gMin % 6)){
                        gMin6Changed = 1;
                        if (!(gMin % 12)){
                            gMin12Changed = 1;
                        }
                    }
                }
                if (!(gMin % 5)){
                    gMin5Changed = 1;
                    if (!(gMin % 10)){
                        gMin10Changed = 1;
                        if (!(gMin % 20)){
                            gMin20Changed = 1;
                        }
                    }
                    if (!(gMin % 15)){
                        gMin15Changed = 1;
                        if (!(gMin % 30)){
                            gMin30Changed = 1;
                        }
                    }
                }
            }              
        } 

        sprintf(gStrTime,"%02d:%02d:%02d", gHour, gMin, gSec);

    }
}


