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

unsigned char ModeBin = 1;		// Periods of being Alive/Dead get counted in all modes! 
								// So, deads getting negative... 
unsigned char ModeAge = 0;		// If there is a lifetime, where degeneration starts.
								// (per period we're then lowering, instead of increasing the reproduction vitality)
int AgeingStart = 0;			// '>' Max Periods to start degrading

int LiveMinStart = 2;			// '>' Min Periods/Nodes around to start
int LiveMaxStart = 4;			// '<' Max  "   "   " "   "  "   "  " "
int LiveMinStop = 2;			// '<' Less than n Periods/Nodes to stop
int LiveMaxStop = 4;			// '>' More   "  "  "   "   " "   "   "


struct LifeSTRUCT {
	unsigned char aging;		// if true then age-count / period counts down 
	int age;
	int age_next;
	unsigned char state_changed;
}; struct LifeSTRUCT life[255][255];

char cmdESC27[ESC27_EXCHANGE_SIZE];

// EVENTS
int EventESC27 (int event){
	
	int r = 0;		// Return Value
	
	// Modification Keys
	int keyState = (gKeyShift) + (gKeyAlt * 2) + (gKeyCtrl * 4);

	if (gKeyShift){
		printf("Shift-");
	}
	if (gKeyAlt){
		printf("Alt-");
	}
	if (gKeyCtrl){
		printf("Ctrl-");
	}
	
	TxtBold(1);

	if (event == -1){
		// A regular Key got pressed
	}
	else if (event > 143 && event < 157){
		// Navigation Keys
		switch (event){
		case 144:
			// Up
			printf("Up");
			switch (keyState){
			case 1:
				// Shift
				break;
			case 2:
				// Alt
				break;
			case 4:
				// Ctrl
				break;
			default:
				// Just the Key
				break;
			}
			break;
		case 145:
			// Down
			printf("Down");
			break;
		case 146:
			// Right
			printf("Right");
			break;
		case 147:
			// Left
			printf("Left");
			break;
		case 148:
			// Center
			printf("Center");
			break;
		case 149:
			// End
			printf("End");
			break;
		case 150:
			// Unknown
			printf("150");
			break;
		case 151:
			// Pos1
			printf("Pos1");
			break;
		case 152:
			// Ins
			printf("Ins");
			break;
		case 153:
			// Del
			printf("Del");
			break;
		case 155:
			// PgUp
			printf("PgUp");
			break;
		case 156:
			// PgDown
			printf("PgDown");
			break;
		}
	}
	else if (event > 126 && event < 140){
		// Delete & F1 - F12
		if (event != 127){
			printf("F%d", event - 127);
		}
		
		switch (event){
		case 127:
			printf("Back");
			// Back
			if (gKeyAlt){
				/* code */
			}
			else{
				/* code */
			}	
			break;	
		case 128:
			// F1
			switch (keyState){
			case 0:
				// Just the Key
				break;
			case 1:
				// Shift
				break;
			case 2:
				// Ctrl
				break;
			case 3:
				// Shift-Ctrl
				break;
			case 4:
				// Alt
				break;
			case 5:
				// Shift-Alt
				break;
			case 6:
				// Alt-Ctrl
				break;
			case 7:
				// Shift-Alt-Ctrl
				break;
			default:
				break;
			}
			break;
		case 129:
			// F2
			break;
		case 130:
			// F3
			break;
		case 131:
			// F4
			break;
		case 132:
			// F5
			break;
		case 133:
			// F6
			break;
		case 134:
			// F7
			break;
		case 135:
			// F8
			break;
		case 136:
			// F9
			break;
		case 137:
			// F10
			break;
		case 138:
			// F11
			break;
		case 139:
			// F12
			break;
		}
	}
	else if (event > 64 && event < 91){
		// (Shift)ALT-A - (Shift)ALT-Z
		printf("%c", (char)event);
		switch(event){
		case 65:
			// A
			if (gKeyShift){
				// Shift-Alt
			}
			else{
				// Alt
			}
			break;
		case 66:
			// B
			break;
		case 67:
			// C
			break;
		case 68:
			// D
			break;
		case 69:
			// E
			break;
		case 70:
			// F
			break;
		case 71:
			// G
			break;
		case 72:
			// H
			break;
		case 73:
			// I
			break;
		case 74:
			// J
			break;
		case 75:
			// K
			break;
		case 76:
			// L
			break;
		case 77:
			// M
			break;
		case 78:
			// N
			break;
		case 79:
			// O
			break;
		case 80:
			// P
			break;
		case 81:
			// Q
			break;
		case 82:
			// R
			break;
		case 83:
			// S
			break;
		case 84:
			// T
			break;
		case 85:
			// U
			break;
		case 86:
			// V
			break;
		case 87:
			// W
			break;
		case 88:
			// X
			break;
		case 89:
			// Y
			break;
		case 90:
			// Z
			break;
		}
	}
	else if (event > 0 && event < 28){
		// Ctrl-A - Ctrl-Z
		// (but a lot are special - see cases above
		//  a lot are also not supported on all OSs)
		printf("CC: %c", (char)event+64);
		switch(event){
		case 1:
			// Ctrl-A
			break;
		case 2:
			// Ctrl-B
			break;
		case 3:
			// Ctrl-C
			break;
		case 4:
			// Ctrl-D
			break;
		case 5:
			// Ctrl-E
			break;
		case 6:
			// Ctrl-F
			break;
		case 7:
			// Ctrl-G
			break;
		case 8:
			// Ctrl-H
			break;
		case 9:
			// TAB
			if (gKeyShift){
				// Shift-Tab
			}
			else{
				// Tab
			}
			break;
		case 10:
			// LF
			break;
		case 11:
			// Ctrl-K
			break;
		case 12:
			// Ctrl-L
			break;
		case 13:
			// CR
			break;
		case 14:
			// Ctrl-N
			break;
		case 15:
			// Ctrl-O
			break;
		case 16:
			// Ctrl-P
			break;
		case 17:
			// Ctrl-Q
			break;
		case 18:
			// Ctrl-R
			break;
		case 19:
			// Ctrl-S
			break;
		case 20:
			// Ctrl-T
			break;
		case 21:
			// Ctrl-U
			break;
		case 22:
			// Ctrl-V
			break;
		case 23:
			// Ctrl-W
			break;
		case 24:
			// Ctrl-X
			break;
		case 25:
			// Ctrl-Y
			break;
		case 26:
			// Ctrl-Z
			break;
		case 27:
			// ESC
			break;
		}
	}
	else if (event > 159 && event < 181){
		// Mouse and Terminal-Answers
		switch(event){
		case 190:
			// Cursor Position
			printf("CursorPos");
			break;
		case 177:
			// Terminal Size received (ESC-Sequence) /polled (WIN) / signaled (Mac/Linux)
			printf("ScreenSize");
			if (ScreenSizeChanged()){
				printf(" (Changed)");
			}
			break;
		case 179:
			// Terminal Icon Label
			printf("IconLabel");
			break;
		case 178:
			// Terminal Name
			printf("TerminalName");
			break;
		case 176:
			// Unknown Terminal Info Object
			printf("UTO");
			break;

		// Terminal GotFocus / LostFocus
		case 160:
			// Got
			printf("GotFocus");
			break;
		case 161:
			// Lost
			printf("LostFocus");
			break;

		// 1st Level (From GetESC27()) Mouse-Events
		case 166:
			// Mouse Move
			printf("MouseMove");
			break;
		case 165:
			// MouseUp
			switch (gMouseButton){
			case 1:
				// Left
				printf("MouseLeftUp");
				break;
			case 4:
				// Right
				printf("MouseRightUp");
				break;
			case 2:
				// Wheel
				printf("MouseWheelUp");
				break;
			default:
				// MultiKey (never seen in reality)
				printf("MouseMultiKeyUp");
				break;
			}
			break;
		case 162:
			// Left Mouse Down
			printf("MouseLeftDown");
			break;
		case 167:
			// Left Down Mouse Move
			printf("MouseLeftDownMove");
			break;
		case 164:
			// Right Mouse Down
			printf("MouseRightDown");
			break;
		case 169:
			// Right Down Mouse Move
			printf("MouseRightDownMove");
			break;
		case 163:
			// Wheel Mouse Down
			printf("MouseWheelDown");
			break;
		case 168:
			// Wheel Down Mouse Move
			printf("MouseWheelDownMove");
			break;
		case 170:
			// WheelScrollUp
			printf("MouseWheelScrollUp");
			break;
		case 171:
			// WheelScrollDown
			printf("MouseWheelScrollDown");
			break;
		case 172:
			// Unknown Mouse Object
			printf("UMO");
			break;
		}
	}
	else if (event > 199 && event < 203){
		// 2nd Level (From Loop()) Mouse-Events
		
		switch (gMouseButton){
		case 1:
			// Left
			printf("Left-");
			break;
		case 4:
			// Right
			printf("Right-");
			break;
		case 2:
			// Wheel
			printf("Wheel-");
			break;
		default:
			// MultiKey (never seen in reality)
			printf("Multi-");
			break;
		}

		switch(event){
		case 200:
			// Click
			printf("Click");
			switch (gMouseButton){
			case 1:
				// Left
				break;
			case 4:
				// Right
				break;
			case 2:
				// Wheel
				break;
			default:
				// MultiKey (never seen in reality)
				break;
			}
			break;
		case 201:
			// DblClick
			printf("DblClick");
			break;
		case 202:
			// Area
			printf("Area");
			break;

		}
	}
	else{
		// Errors
		switch(event){
		case -3:
			// TimeOut of a broken, or valid but unknown sequence
			printf("ERR -3 TimeOut");
			break;
		case -2:
			// Unknown Termination/Identification Char
			printf("ERR -2 Termination/Identification");
			break;
		case -4:
			// Overflow, Too Long
			printf("ERR -4 Overflow");
			break;
		case -5:
			// Unexpected End Of Text
			printf("ERR -5 UnexpectedEOT");
			break;
		case -6:
			// Overlapping Sequence - already done in CoreLoop
			printf("ERR -6 Overlapping");
			break;
		case -7:
			// ByteMouse Out Of Range
			printf("ERR -7 ByteMouse");
			break;

		default:
			// Unexpected...
			printf("ERR - WTF");
			break;
		}
	}
	gKeyAlt = 0; gKeyCtrl = 0; gKeyMeta = 0; gKeyShift = 0;

	if (event > 31){
		printf("  : %s\n", &gStreamInESC27[1]);
	}
	else{
		printf("\n");
	}

	if (event != -1){
		/* code */
		printf("Event: %d", event);

		printf("\n\n\n");

	}

	TxtBold(0);					

	return r;
}

void EventDayChange(void){
	SetFg16(fgYellowB);
	printf("    DayChange: %s - %s\n",gStrRunTime, gStrTime);
}

void EventHourChange(void){	
	SetFg16(fgBlueB);
	printf("   HourChange: %s - %s\n",gStrRunTime, gStrTime);
	// Check on DayChange
	if (gDayChanged){
		EventDayChange();
	}
	SetFg16(fgBlue);
}

void EventHour2Change(void){
	printf("  Hour2Change: %s - %s\n",gStrRunTime, gStrTime);
}
void EventHour3Change(void){
	printf("  Hour3Change: %s - %s\n",gStrRunTime, gStrTime);
}
void EventHour4Change(void){
	printf("  Hour4Change: %s - %s\n",gStrRunTime, gStrTime);
}
void EventHour6Change(void){
	printf("  Hour6Change: %s - %s\n",gStrRunTime, gStrTime);
}
void EventHour8Change(void){
	printf("  Hour8Change: %s - %s\n",gStrRunTime, gStrTime);
}
void EventHour12Change(void){
	printf(" Hour12Change: %s - %s\n",gStrRunTime, gStrTime);
}

void EventMinuteChange(void){
	printf("    MinChange: %s - %s\n",gStrRunTime, gStrTime);
	// Check on HourChange
	if (gHourChanged){
		EventHourChange();
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
	}
	ResFg();
}

void EventMinute2Change(void){
	printf("   Min2Change: %s - %s\n",gStrRunTime, gStrTime);
}
void EventMinute3Change(void){
	printf("   Min3Change: %s - %s\n",gStrRunTime, gStrTime);
}
void EventMinute4Change(void){
	printf("   Min4Change: %s - %s\n",gStrRunTime, gStrTime);
}
void EventMinute5Change(void){
	printf("   Min5Change: %s - %s\n",gStrRunTime, gStrTime);
}
void EventMinute6Change(void){
	printf("   Min6Change: %s - %s\n",gStrRunTime, gStrTime);
}
void EventMinute10Change(void){
	printf("  Min10Change: %s - %s\n",gStrRunTime, gStrTime);
}
void EventMinute12Change(void){
	printf("  Min12Change: %s - %s\n",gStrRunTime, gStrTime);
}
void EventMinute15Change(void){
	printf("  Min15Change: %s - %s\n",gStrRunTime, gStrTime);
}
void EventMinute20Change(void){
	printf("  Min20Change: %s - %s\n",gStrRunTime, gStrTime);
}
void EventMinute30Change(void){
	printf("  Min30Change: %s - %s\n",gStrRunTime, gStrTime);
}

void EventSecondChange(void){	
	SetFg16(fgWhite);
	printf("    SecChange: %s - %s\n",gStrRunTime, gStrTime);
	ResFg();
	// Check on MinuteChange
	if (gMinuteChanged){
		SetFg16(fgGreenB);
		EventMinuteChange();
		SetFg16(fgGreen);
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
	}

	ResFg();	
	EraseTimeChange();

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
	while (i != 10 && i != 13){
		
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
					printf("TimeOutCase");
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

		if (gSignalCtrlC){
			// Ctrl-C
			i = 3;
			gSignalCtrlC = 0;
		}
			
	// Loop Minimum
	
	// Loop Minimum
		if (i){
	// Loop Minimum
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
					if ((gMouseSelX == gMousePosX) && (gMouseSelY == gMousePosY)){
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
				if (gSignalInterval){
					CheckOnTimeChange();
				}
			#endif
			if (gSecondChanged){
				// A Second (ore more) is over
				EventSecondChange();	// Activates all other Minute/Hour/DayChange Events
			}
		}				

		
		// DO STUFF HERE START

		// DO STUFF HERE STOP

		
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
				if (gSignalInterval){
					if (gSignalTerminalSize){
						GetTerminalSize(3);
						EventESC27(177);
						gSignalTerminalSize = 0;
					}
					gSignalInterval = 0;
				}
				else {
					DoEvents();
					//gKeyAlt = 0; gKeyCtrl = 0; gKeyShift = 0; gKeyMeta = 0;
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
		signal(SIGWINCH, SignalHandler);
		printf("PosiX-OS: Screen Size Changes Get Signaled.\n");
	#endif
	printf("Width: %d  Height: %d\n\n", gScreenWidth, gScreenHeight);

	printf("Synchronize CLS-Mode With Size-Mode...\n");
	r = ClearScreen(r);
	printf("Synchronized CLS-Mode With Size-Mode... OK, Mode: %d\n",r);
	printf("Width: %d  Height: %d\n\n", gScreenWidth, gScreenHeight);

	printf("Enable Trap Mouse Mode... ");
	TrapMouse(1);
	printf("(probably) OK\n\n");

	printf("Enable Trap Focus Change... ");
	TrapFocus(1);
	printf("(probably) OK\n\n");

	printf("Catch Ctrl-C... ");
	signal(SIGINT, SignalHandler);
	printf("OK\n\n");

	#if __WIN32__ || _MSC_VER || __WIN64__
		printf("Billy-OS: Check On Real-Time Changes Get Polled...Sorry.\n\n");
	#else
		signal(SIGALRM, SignalHandler);
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



