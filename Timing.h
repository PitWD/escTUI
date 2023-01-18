
//		       Timer Functions - See detailed description at the EOF !
//				! Using 3rd party MIT/GPL/"FreeSoftware" technologies !
//				  (see related declarations/functions and at the EOF)
// This is a  ! p_o_o_r !  Header by (c) Pit Demmer. (PIT-Licensed 20.08.2022 -           )

#include <time.h>

#define IS_REVERSE_DEBUG 0
#define IS_TIME_EVENT_DEBUG 1
#define IS_TERMINAL_EVENT_DEBUG 0

// CLOCKS_PER_SEC you can't trust - main app has to call CalcClocksPerSecond at least ones!
// !!! Initial 45000 is just fine with MY development environment !!!
// !!! On WIN it seems to be constant 1000 - like it's on most Posix 1000000
clock_t gClocksPerSecond = CLOCKS_PER_SEC;
static clock_t lClockLast;

// Real System Time
static time_t lTimeNow;
static time_t lTimeLast;
time_t gTime = 0;              // THE Serial Time (time() but shifted by user)
static time_t lTimeShift = 946677600; // 01/01/2000
int gHour = 0;
int gMin = 0;
int gSec = 0;
int gDay = 0;
int gMonth = 0;
int gYear = 0;
char gStrDate[] = "01.01.1970";
char gStrTime[] = "00:00:00      ";     // Trailing Spaces To Prevent Mac-Sleep-Overflow

// Events

// Runtime Counter
time_t gRunTime = 0;
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
int TIME_EventsTime = 10000;

/**
 * @brief Declaration FUNCTIONS in Timing.h
 */
void InitTiming(void);
void CheckOnTimeChange(void);

// Events
static void TimeDummyEvent(void){
    volatile int i;
}

void (*TimeSecChanged)(void) = TimeDummyEvent;
void (*TimeMinChanged)(void) = TimeDummyEvent;
void (*TimeHourChanged)(void) = TimeDummyEvent;
void (*TimeDayChanged)(void) = TimeDummyEvent;

void (*TimeSec2Changed)(void) = TimeDummyEvent;
void (*TimeSec3Changed)(void) = TimeDummyEvent;
void (*TimeSec4Changed)(void) = TimeDummyEvent;
void (*TimeSec5Changed)(void) = TimeDummyEvent;
void (*TimeSec6Changed)(void) = TimeDummyEvent;
void (*TimeSec10Changed)(void) = TimeDummyEvent;
void (*TimeSec12Changed)(void) = TimeDummyEvent;
void (*TimeSec15Changed)(void) = TimeDummyEvent;
void (*TimeSec20Changed)(void) = TimeDummyEvent;
void (*TimeSec30Changed)(void) = TimeDummyEvent;

void (*TimeMin2Changed)(void) = TimeDummyEvent;
void (*TimeMin3Changed)(void) = TimeDummyEvent;
void (*TimeMin4Changed)(void) = TimeDummyEvent;
void (*TimeMin5Changed)(void) = TimeDummyEvent;
void (*TimeMin6Changed)(void) = TimeDummyEvent;
void (*TimeMin10Changed)(void) = TimeDummyEvent;
void (*TimeMin12Changed)(void) = TimeDummyEvent;
void (*TimeMin15Changed)(void) = TimeDummyEvent;
void (*TimeMin20Changed)(void) = TimeDummyEvent;
void (*TimeMin30Changed)(void) = TimeDummyEvent;

void (*TimeHour2Changed)(void) = TimeDummyEvent;
void (*TimeHour3Changed)(void) = TimeDummyEvent;
void (*TimeHour4Changed)(void) = TimeDummyEvent;
void (*TimeHour6Changed)(void) = TimeDummyEvent;
void (*TimeHour8Changed)(void) = TimeDummyEvent;
void (*TimeHour12Changed)(void) = TimeDummyEvent;

void TimeInitTime(void){
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

    struct tm *pLocalTime;

    time(&lTimeLast);
    time(&lTimeNow);
    
    gTime = lTimeNow - lTimeShift;

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

void TimeCheckOnChange(void){
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

    int sec00Overflow = 0;
    double timeDiff = 0;
    struct tm *pLocalTime;
    clock_t clockNow;

    time(&lTimeNow);
    timeDiff = difftime(lTimeNow,lTimeLast);

    // A Second (ore more) is over
    if (timeDiff){

        // THE Serial Time
        gTime = lTimeNow - lTimeShift;

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
        TimeSecChanged();

        //System Runtime
        gRunTimeSec += timeDiff;
        gRunTime += timeDiff;
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
            TimeSec2Changed();
            TimeSec3Changed();
            TimeSec4Changed();
            TimeSec5Changed();
            TimeSec6Changed();
            TimeSec10Changed();
            TimeSec12Changed();
            TimeSec15Changed();
            TimeSec20Changed();
            TimeSec30Changed();

            TimeMinChanged();
            gMin = pLocalTime->tm_min;
            
            if (gMin == 0){
                TimeMin2Changed();
                TimeMin3Changed();
                TimeMin4Changed();
                TimeMin5Changed();
                TimeMin6Changed();
                TimeMin10Changed();
                TimeMin12Changed();
                TimeMin15Changed();
                TimeMin20Changed();
                TimeMin30Changed();

                TimeHourChanged();

                if (gHour == 0){
                    TimeHour2Changed();
                    TimeHour3Changed();
                    TimeHour4Changed();
                    TimeHour6Changed();
                    TimeHour8Changed();
                    TimeHour12Changed();

                    TimeDayChanged();

                    gDay = pLocalTime->tm_mday;
                    gMonth = pLocalTime->tm_mon + 1;
                    gYear = pLocalTime->tm_year + 1900;
                    sprintf(gStrDate,"%02d.%02d.%04d", gDay, gMonth, gYear);
                }
                else{
                    if (!(gHour % 2)){
                        TimeHour2Changed();
                        if (!(gHour % 4)){
                            TimeHour4Changed();
                            if (!(gHour % 8)){
                                TimeHour8Changed();
                            }
                        }
                    }
                    if (!(gHour % 3)){
                        TimeHour3Changed();
                        if (!(gHour % 6)){
                            TimeHour6Changed();
                            if (!(gHour % 12)){
                                TimeHour12Changed();
                            }                  
                        }
                    }
                }               
            }
            else{
                if (!(gMin % 2)){
                    TimeMin2Changed();
                    if (!(gMin % 4)){
                        TimeMin4Changed();
                    }
                }
                if (!(gMin % 3)){
                    TimeMin3Changed();
                    if (!(gMin % 6)){
                        TimeMin6Changed();
                        if (!(gMin % 12)){
                            TimeMin12Changed();
                        }
                    }
                }
                if (!(gMin % 5)){
                    TimeMin5Changed();
                    if (!(gMin % 10)){
                        TimeMin10Changed();
                        if (!(gMin % 20)){
                            TimeMin20Changed();
                        }
                    }
                    if (!(gMin % 15)){
                        TimeMin15Changed();
                        if (!(gMin % 30)){
                            TimeMin30Changed();
                        }
                    }
                }
            }              
        } 
        else{
            if (!(gSec % 2)){
                TimeSec2Changed();
                if (!(gSec % 4)){
                    TimeSec4Changed();
                }
            }
            if (!(gSec % 3)){
                TimeSec3Changed();
                if (!(gSec % 6)){
                    TimeSec6Changed();
                    if (!(gSec % 12)){
                        TimeSec12Changed();
                    }
                }
            }
            if (!(gSec % 5)){
                TimeSec5Changed();
                if (!(gSec % 10)){
                    TimeSec10Changed();
                    if (!(gSec % 20)){
                        TimeSec20Changed();
                    }
                }
                if (!(gSec % 15)){
                    TimeSec15Changed();
                    if (!(gSec % 30)){
                        TimeSec30Changed();
                    }
                }
            }
        }
        
        sprintf(gStrTime,"%02d:%02d:%02d", gHour, gMin, gSec);

    }
}


