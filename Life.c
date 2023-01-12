/*		   (Simple) Version Of The Game Of Life (Originally by John Horton Conway)
      	It' a test-application to test and develop PoorTUI- Terminal Functions
					       See detailed description at the EOF !

		 	! Could contain 3rd party MIT/GPL/"FreeSoftware" technologies !
		(see detailed comments in related declarations/functions and at the EOF)

	This is a  ! p_o_o_r !  Application by (c) Pit Demmer. (PIT-Licensed 01.04.2022 - x.x.xxxx)
	PIT-licensed" isn't a joke! - Right now I do not know about the license model I wanna use!
								But Sources will be free!
	 	I'm just struggling with professional 3rd parties distributing binaries
	 (or executing a compiling process on customer machines) without contributing to
									"FreeSoftware"
*/

const char LifeVersion[] = "1.00pa";

#include "PoorTui.h"	


void DoTimerEvents(void){	

	#if IS_TIME_EVENT_DEBUG
		SetFg16(fgWhiteB);
	#endif
	
	EventSecondChange();

	#if IS_TIME_EVENT_DEBUG
		SetFg16(fgWhite);
	#endif

	if (gSec2Changed){
		EventSecond2Change();
		if (gSec4Changed){
			EventSecond4Change();
		}
	}
	if (gSec3Changed){
		EventSecond3Change();
		if (gSec6Changed){
			EventSecond6Change();
			if (gSec12Changed){
				EventSecond12Change();
			}
		}
	}
	if (gSec5Changed){
		EventSecond5Change();
		if (gSec15Changed){
			EventSecond15Change();
			if (gSec30Changed){
				EventSecond30Change();
			}
		}			
		if (gSec10Changed){
			EventSecond10Change();
			if (gSec20Changed){
				EventSecond20Change();
			}
		}			
	}

	// Check on MinuteChange
	if (gMinuteChanged){
		#if IS_TIME_EVENT_DEBUG
			SetFg16(fgGreenB);
		#endif
		EventMinuteChange();
		#if IS_TIME_EVENT_DEBUG
			SetFg16(fgGreen);
		#endif
		if (gMin2Changed){
			EventMinute2Change();
			if (gMin4Changed){
				EventMinute4Change();
			}
		}
		if (gMin3Changed){
			EventMinute3Change();
			if (gMin6Changed){
				EventMinute6Change();
				if (gMin12Changed){
					EventMinute12Change();
				}
			}
		}
		if (gMin5Changed){
			EventMinute5Change();
			if (gMin15Changed){
				EventMinute15Change();
				if (gMin30Changed){
					EventMinute30Change();
				}
			}			
			if (gMin10Changed){
				EventMinute10Change();
				if (gMin20Changed){
					EventMinute20Change();
				}
			}			
		}

		// Check on HourChange
		if (gHourChanged){
			#if IS_TIME_EVENT_DEBUG
				SetFg16(fgBlueB);
			#endif
			EventHourChange();
			#if IS_TIME_EVENT_DEBUG
				SetFg16(fgBlue);
			#endif
			if (gHour2Changed){
				EventHour2Change();
				if (gHour4Changed){
					EventHour4Change();
					if (gHour8Changed){
						EventHour8Change();
					}
				}
			}
			if (gHour3Changed){
				EventHour3Change();
				if (gHour6Changed){
					EventHour6Change();
					if (gHour12Changed){
						EventHour12Change();
					}
				}
			}
			// Check on DayChange
			if (gDayChanged){
				#if IS_TIME_EVENT_DEBUG
					SetFg16(fgYellowB);
				#endif
				EventDayChange();
			}
		}
	}

	#if IS_TIME_EVENT_DEBUG
		ResFg();
	#endif

}

void CoreLoop(void){

	int i = 0;		// Input From stdin
	int r = 0;		// Result From GetESC27
	char c = 0;

	
	// Recognize manual ESC
		int isOnUsrESC27 = 0;
		clock_t timeOnUsrEsc;
	// Recognize broken Sequences by timeout
		int isOnESC27 = 0;
		clock_t timeOnEsc;
	// Recognize Click & DblClick / Area-Selection
		int isOnClick = 0;
		clock_t timeOnClick;
		int posXonClick = 0;
		int posYonClick = 0;
	// Poll for TerminalSizeChange
		#if __WIN32__ || _MSC_VER || __WIN64__
			#define Terminal_Size_Change_Trigger 10
		#else
			#define Terminal_Size_Change_Trigger 10
		#endif
		int terminalSizeChangeCnt = Terminal_Size_Change_Trigger;

	// Loop Minimum
	// while (i != 10 && i != 13){
	while (TUI_RunCoreLoop){
		
		i = InKey();

		// Recognize manual ESC
		if (isOnUsrESC27 && !i){
			if (clock() > timeOnUsrEsc){
				// UsrESC
				i = 27;
			}
		}
		// Recognize timeout while receiving ESC
		else if (isOnESC27 && (!isOnUsrESC27 && !i)){
			// The !isOnUsrESC27 signals we already got more chars than just the 1st ESC
			if (clock() > timeOnEsc){
				isOnESC27 = 0;
				if (r == -6){
					// ShiftAlt-O (overlapping with F1-F4)
					i = -2;
				}
				else{
					// Broken, or valid and unknown, Sequence
					#if IS_TERMINAL_EVENT_DEBUG
						printf("TimeOutCase");
					#endif
					i = -1;
				}				
			}		
		}
		else if (i == 27){
			// Entrance into (User)ESC-Sequences recognition
			isOnUsrESC27 = 1; isOnESC27= 1;
			timeOnUsrEsc = clock() + gUserEscTimeout;
			// timeout for broken sequence twice as regular UserESC
			timeOnEsc = timeOnUsrEsc + gUserEscTimeout;
		}
		else{
			// any char after 1st ESC immediately disables possibility on UserESC
			isOnUsrESC27 = 0;
		}

		if (TERM_SignalCtrlC){
			// Ctrl-C
			i = 3;
			TERM_SignalCtrlC = 0;
		}
			
	// Loop Minimum
	
	// Loop Minimum
		if (i){
	// Loop Minimum
			#if IS_REVERSE_DEBUG
				if (i > 0){
					TxtItalic(1);
					SetFg16(fgRed);
					printf("%d", i);
					TxtItalic(0);
					SetFg16(0);
					if (i > 31){// && i < 128){
						c = i;
						printf(": %c",c);
					}
					else if (i == 27){
						TxtBold(1);
						printf(": ESC");
						TxtBold(0);
					}
					
					printf("\n");
					fflush(stdout);
				}
			#endif
			// Loop Minimum
			
			r = GetESC27(i);

			switch (r){
			case 0:
				// Nothing (waiting for more...)
				break;
			case -1:
				// Regular Key - No ESC-Sequence/SpecialKey related stuff
				break;
			case 162:
			case 163:
			case 164:
				// Mouse Down
				if (isOnClick){
					// refresh timeout for MAC dblClick issues...
					timeOnClick = clock() + gMouseClickTimeout;
				}
				break;					
			case 165:
				// Mouse UP (Left / Wheel / Right)
				EventESC27(165);
				isOnESC27 = 0;
				r = 0;
				if ((TERM_MouseSelX == TERM_MousePosX) && (TERM_MouseSelY == TERM_MousePosY)){
					// it's a (dbl)click
					if (isOnClick && clock() < timeOnClick){
						// dblClick
						EventESC27(201);
						isOnClick = 0;
					}
					else{
						// 1st Click
						isOnClick = 1;
						timeOnClick = clock() + gMouseClickTimeout;
					}							
				}
				else{
					// it's an area
					if (isOnClick && clock() < timeOnClick){
						// but nobody can define an area that fast,
						// so we decide for dblClick
						EventESC27(201);
						isOnClick = 0;
					}
					else{
						// finally area
						EventESC27(202);
						isOnClick = 0;
					}
				}
				break;
			default:
				break;
			}
			
			if (r && r != -6){
				// Disable timeouts
				isOnUsrESC27 = 0; isOnESC27 = 0;
				EventESC27(r);
				r = 0;
			}
			else if(r == -6){
				// The case that F1-F4 and ShiftAlt-O are overlapping...
				// So, we push the Event when TimeOut occurs (see above)
			}
				
				
				
			// Loop Minimum
			

	// Loop Minimum
		}

		// Recognize Single Click
		if (isOnClick && (clock() > timeOnClick)){
			// click
			EventESC27(200);
			isOnClick = 0;
		}

		if (!i){
			// We did not received anything
			// Time to check for RealTime things
			#if __WIN32__ || _MSC_VER || __WIN64__
				CheckOnTimeChange();
			#else
				if (TERM_SignalInterval){
					CheckOnTimeChange();
				}
			#endif
			if (gSecondChanged){
				// A Second (ore more) is over
				DoTimerEvents();	// Activates all other Minute/Hour/DayChange Events
			}
		}				

		
		// DO USER STUFF HERE START

		// DO USER STUFF HERE STOP

		// Erase Timer Events		
		if (gSecondChanged){
			EraseTimeChange();
		}
		
		if (!i){
			// We did not received anything
			// Time to do 2nd Time idle-time things
			#if __WIN32__ || _MSC_VER || __WIN64__
				terminalSizeChangeCnt--;
				if (!terminalSizeChangeCnt){
					// Every xx idle-times check on ScreenSize
					terminalSizeChangeCnt = Terminal_Size_Change_Trigger;
					GetTerminalSize(3);
					EventESC27(177);
				}
				else {
					DoEvents();
				}	
			#else
				if (TERM_SignalInterval){
					if (TERM_SignalTerminalSize){
						GetTerminalSize(3);
						EventESC27(177);
						TERM_SignalTerminalSize = 0;
					}
					TERM_SignalInterval = 0;
				}
				else {
					DoEvents();
					//TERM_KeyAlt = 0; TERM_KeyCtrl = 0; TERM_KeyShift = 0; TERM_KeyMeta = 0;
				}	
			#endif
		}
	}
	
	// Loop Minimum

}

int main() {
	
	/*
	strcpy(symbolMisc[29].str, "☜");	// \u261C	WHITE LEFT POINTING INDEX
	strcpy(symbolMisc[30].str, "☝");	// \u261D	WHITE UP POINTING INDEX
	strcpy(symbolMisc[31].str, "☞");	// \u261E	WHITE RIGHT POINTING INDEX
	strcpy(symbolMisc[32].str, "☟");	// \u261F	WHITE DOWN POINTING INDEX
	*/

	int r = 0;

	printf("\nHello New Project\n\n");

	printf("Initializing Timing... ");
	InitTiming();
	printf("OK\n");
	printf("%s - %s\n\n", gStrDate, gStrTime);

	printf("Try To Enable Video Terminal Mode... ");
	if (!SetVT(1)){
		printf("ERROR!\n");
		printf("STOP PROJECT\n");
		return 0;
	}
	printf("OK\n\n");

	printf("Try To Clear Screen...\n");
	r = ClearScreen(0);
	if (!r){
		// Can't happen....
		printf("Clear Screen... ERROR!\n");
		printf("STOP PROJECT\n");
		return 0;
	}
	printf("Clear Screen... OK, CLS-Mode: %d\n\n",r);

	printf("Try To Fetch Terminal Size... ");
	r = GetTerminalSize(0);
	if (!r){
		printf("ERROR!\n");
		printf("STOP PROJECT\n");
		return 0;
	}
	printf("OK, Size-Mode: %d\n",r);
	#if __WIN32__ || _MSC_VER || __WIN64__
		printf("Billy-OS: Screen Size Changes Get Polled...Sorry.\n");
	#else
		signal(SIGWINCH, TermSignalHandler);
		printf("PosiX-OS: Screen Size Changes Get Signaled.\n");
	#endif
	printf("Width: %d  Height: %d\n\n", TERM_ScreenWidth, TERM_ScreenHeight);

	printf("Synchronize CLS-Mode With Size-Mode...\n");
	r = ClearScreen(r);
	printf("Synchronized CLS-Mode With Size-Mode... OK, Mode: %d\n",r);
	printf("Width: %d  Height: %d\n\n", TERM_ScreenWidth, TERM_ScreenHeight);

	printf("Enable Trap Mouse Mode... ");
	TrapMouse(1);
	printf("(probably) OK\n\n");

	printf("Enable Trap Focus Change... ");
	TrapFocus(1);
	printf("(probably) OK\n\n");

	printf("Catch Ctrl-C... ");
	signal(SIGINT, TermSignalHandler);
	printf("OK\n\n");

	#if __WIN32__ || _MSC_VER || __WIN64__
		printf("Billy-OS: Check On Real-Time Changes Get Polled...Sorry.\n\n");
	#else
		signal(SIGALRM, TermSignalHandler);
		ualarm(333333,333333);
		printf("PosiX-OS: Check On Real-Time Changes Get Signaled.\n\n");
	#endif
	
	InitEscSeq();
	InitColors();
	
	// *************************************************************
	//ClearScreen(0);
	CoreLoop();
	// *************************************************************

	ClearScreen(0);
	printf("Disable Trap Mouse Mode... ");
	TrapMouse(0);
	printf("OK\n\n");

	printf("Disable Trap Focus Change... ");
	TrapFocus(0);
	printf("OK\n\n");

	printf("Try To Set Back Terminal From Video Mode... ");
	if (!SetVT(0)){
		printf("ERROR!\n");
		return -1;
	}
	printf("OK\n\n");

	printf("%s - %s\n", gStrDate, gStrTime);
	printf("Runtime: %s\n\n", gStrRunTime);


	return 0;

};

/*
										EOF - Detailed Description

				Included *.h having 8-Bit AVR's as main target!

				App-State   : Pre Alpha
				App-Version : 1.00pa
				App-Date    :

				Dev-Start   : 01.04.2022
				1st Alpha   :
				1st Beta    :
				1st Release :

STRUCTURES		            Creation Date   Alpha Date      BetaDate        Creator


Files			    		Creation Date   Alpha Date      BetaDate        Creator
	<Life.c>		    	01.04.2022		                                Pit

BugID		Description									Found by / Date		Fixed by / Date
*/


