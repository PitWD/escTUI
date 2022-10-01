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
	switch (event){
	// A regular Key got pressed
	case -1:
		break;
	// F1 - F12
	case 1:
		// F1
		break;
	case 2:
		// F2
		break;
	case 3:
		// F3
		break;
	case 4:
		// F4
		break;
	case 5:
		// F5
		break;
	case 7:
		// F6
		break;
	case 8:
		// F7
		break;
	case 9:
		// F8
		break;
	case 10:
		// F9
		break;
	case 11:
		// F10
		break;
	case 13:
		// F11
		break;
	case 14:
		// F11
		break;

	// Shift F1 - F12
	case 15:
		// F1
		break;
	case 16:
		// F2
		break;
	case 17:
		// F3
		break;
	case 18:
		// F4
		break;
	case 19:
		// F5
		break;
	case 21:
		// F6
		break;
	case 22:
		// F7
		break;
	case 23:
		// F8
		break;
	case 24:
		// F9
		break;
	case 25:
		// F10
		break;
	case 27:
		// F11
		break;
	case 28:
		// F11
		break;

	// CTRL F1 - F12
	case 29:
		// F1
		break;
	case 30:
		// F2
		break;
	case 31:
		// F3
		break;
	case 32:
		// F4
		break;
	case 33:
		// F5
		break;
	case 35:
		// F6
		break;
	case 36:
		// F7
		break;
	case 37:
		// F8
		break;
	case 38:
		// F9
		break;
	case 39:
		// F10
		break;
	case 41:
		// F11
		break;
	case 42:
		// F11
		break;

// Alt F1 - F12
	case 160:
		// F1
		break;
	case 161:
		// F2
		break;
	case 162:
		// F3
		break;
	case 163:
		// F4
		break;
	case 164:
		// F5
		break;
	case 166:
		// F6
		break;
	case 167:
		// F7
		break;
	case 168:
		// F8
		break;
	case 169:
		// F9
		break;
	case 170:
		// F10
		break;
	case 172:
		// F11
		break;
	case 173:
		// F11
		break;

// ShiftAlt F1 - F12
	case 174:
		// F1
		break;
	case 175:
		// F2
		break;
	case 176:
		// F3
		break;
	case 177:
		// F4
		break;
	case 178:
		// F5
		break;
	case 180:
		// F6
		break;
	case 181:
		// F7
		break;
	case 182:
		// F8
		break;
	case 183:
		// F9
		break;
	case 184:
		// F10
		break;
	case 186:
		// F11
		break;
	case 187:
		// F11
		break;

// AltCtrl F1 - F12
	case 188:
		// F1
		break;
	case 189:
		// F2
		break;
	case 190:
		// F3
		break;
	case 191:
		// F4
		break;
	case 192:
		// F5
		break;
	case 194:
		// F6
		break;
	case 195:
		// F7
		break;
	case 196:
		// F8
		break;
	case 197:
		// F9
		break;
	case 198:
		// F10
		break;
	case 200:
		// F11
		break;
	case 201:
		// F12
		break;

// ShiftCtrl F1 - F12
	case 202:
		// F1
		break;
	case 203:
		// F2
		break;
	case 204:
		// F3
		break;
	case 205:
		// F4
		break;
	case 206:
		// F5
		break;
	case 208:
		// F6
		break;
	case 209:
		// F7
		break;
	case 210:
		// F8
		break;
	case 211:
		// F9
		break;
	case 212:
		// F10
		break;
	case 214:
		// F11
		break;
	case 215:
		// F12
		break;

// AltCtrl F1 - F12
	case 216:
		// F1
		break;
	case 217:
		// F2
		break;
	case 218:
		// F3
		break;
	case 219:
		// F4
		break;
	case 220:
		// F5
		break;
	case 222:
		// F6
		break;
	case 223:
		// F7
		break;
	case 224:
		// F8
		break;
	case 225:
		// F9
		break;
	case 226:
		// F10
		break;
	case 228:
		// F11
		break;
	case 229:
		// F12
		break;

	// Ctrl-A - Ctrl-Z
	// (but a lot are special - see cases above
	//  a lot are also not supported on all OSs)
	case 230:
		// Ctrl-A
		break;
	case 231:
		// Ctrl-B
		break;
	case 232:
		// Ctrl-C
		break;
	case 233:
		// Ctrl-D
		break;
	case 234:
		// Ctrl-E
		break;
	case 235:
		// Ctrl-F
		break;
	case 236:
		// Ctrl-G
		break;
	case 237:
		// Ctrl-H
		break;
	case 238:
		// Ctrl-I
		break;
	case 239:
		// Ctrl-J
		break;
	case 240:
		// Ctrl-K
		break;
	case 241:
		// Ctrl-L
		break;
	case 242:
		// Ctrl-M
		break;
	case 243:
		// Ctrl-N
		break;
	case 244:
		// Ctrl-O
		break;
	case 245:
		// Ctrl-P
		break;
	case 246:
		// Ctrl-Q
		break;
	case 247:
		// Ctrl-R
		break;
	case 248:
		// Ctrl-S
		break;
	case 249:
		// Ctrl-T
		break;
	case 250:
		// Ctrl-U
		break;
	case 251:
		// Ctrl-V
		break;
	case 252:
		// Ctrl-W
		break;
	case 253:
		// Ctrl-X
		break;
	case 254:
		// Ctrl-Y
		break;
	case 255:
		// Ctrl-Z
		break;

	// Navigation
	case 43:
		// Up
		break;
	case 44:
		// Down
		break;
	case 45:
		// Right
		break;
	case 46:
		// Left
		break;
	case 47:
		// Center
		break;
	case 48:
		// End
		break;
	case 50:
		// Pos1
		break;
	case 51:
		// Ins
		break;
	case 52:
		// Del
		break;
	case 54:
		// PgUp
		break;
	case 55:
		// PgDown
		break;

	// Shift Navigation
	case 56:
		// Up
		break;
	case 57:
		// Down
		break;
	case 58:
		// Right
		break;
	case 59:
		// Left
		break;
	case 61:
		// End
		break;
	case 63:
		// Pos1
		break;

	// Ctrl Navigation
	case 64:
		// Up
		break;
	case 65:
		// Down
		break;
	case 66:
		// Right
		break;
	case 67:
		// Left
		break;
	case 68:
		// Center
		break;
	case 69:
		// End
		break;
	case 71:
		// Pos1
		break;

	// ALT Navigation
	case 72:
		// Up
		break;
	case 73:
		// Down
		break;
	case 74:
		// Right
		break;
	case 75:
		// Left
		break;
	case 76:
		// Center
		break;
	case 77:
		// End
		break;
	case 79:
		// Pos1
		break;

	// ALT-A - ALT-Z
	case 80:
		// A
		break;
	case 81:
		// B
		break;
	case 82:
		// C
		break;
	case 83:
		// D
		break;
	case 84:
		// E
		break;
	case 85:
		// F
		break;
	case 86:
		// G
		break;
	case 87:
		// H
		break;
	case 88:
		// I
		break;
	case 89:
		// J
		break;
	case 90:
		// K
		break;
	case 91:
		// L
		break;
	case 92:
		// M
		break;
	case 93:
		// N
		break;
	case 94:
		// O
		break;
	case 95:
		// P
		break;
	case 96:
		// Q
		break;
	case 97:
		// R
		break;
	case 98:
		// S
		break;
	case 99:
		// T
		break;
	case 100:
		// U
		break;
	case 101:
		// V
		break;
	case 102:
		// W
		break;
	case 103:
		// X
		break;
	case 104:
		// Y
		break;
	case 105:
		// Z
		break;

	// ShiftALT-A - ShiftALT-Z
	case 129:
		// A
		break;
	case 130:
		// B
		break;
	case 131:
		// C
		break;
	case 132:
		// D
		break;
	case 133:
		// E
		break;
	case 134:
		// F
		break;
	case 135:
		// G
		break;
	case 136:
		// H
		break;
	case 137:
		// I
		break;
	case 138:
		// J
		break;
	case 139:
		// K
		break;
	case 140:
		// L
		break;
	case 141:
		// M
		break;
	case 142:
		// N
		break;
	case 143:
		// O - but is occupied by F1-F4
		break;
	case 144:
		// P
		break;
	case 145:
		// Q
		break;
	case 146:
		// R
		break;
	case 147:
		// S
		break;
	case 148:
		// T
		break;
	case 149:
		// U
		break;
	case 150:
		// V
		break;
	case 151:
		// W
		break;
	case 152:
		// X
		break;
	case 153:
		// Y
		break;
	case 154:
		// Z
		break;

	// 'Single'-Keys
	case 106:
		// Back
		break;
	case 108:
		// ESC
		break;
	case 113:
		// TAB
		break;
	case 114:
		// Shift-TAB
		break;
	case 115:
		// ENTER
		break;
	
	// Answers
	case 107:
		// Cursor Position
		break;
	case 109:
		// Terminal Size received (ESC-Sequence) /polled (WIN) / signaled (Mac/Linux)
		if (ScreenSizeChanged()){
			// Terminal Size has changed...
		}
		break;
	case 110:
		// Terminal Icon Label
		break;
	case 111:
		// Terminal Name
		break;

	// Errors 
	case 112:
		// Terminal Error
		break;
	case 155:
		// Len Err By Too Early Following ESC
		break;
	case 156:
		// Position Overflow In Mouse Byte-Mode 
		break;
	case 125:
		// Unknown Mouse Object
		break;

	// Terminal GotFocus / LostFocus
	case 158:
		// Got
		break;
	case 159:
		// Lost
		break;

	// 1st Level (From GetESC27()) Mouse-Events
	case 120:
		// Mouse Move
		break;
	case 117:
		// MouseUp
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
	case 116:
		// Left Mouse Down
		break;
	case 121:
		// Left Down Mouse Move
		break;
	case 126:
		// Right Mouse Down
		break;
	case 127:
		// Right Down Mouse Move
		break;
	case 118:
		// Wheel Mouse Down
		break;
	case 122:
		// Wheel Down Mouse Move
		break;
	case 123:
		// WheelScrollUp
		break;
	case 124:
		// WheelScrollDown
		break;

	// 2nd Level (From Loop()) Mouse-Events
	case 512:
		// Click
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
	case 513:
		// DblClick
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
	case 514:
		// Area
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

	// Errors
	case -3:
		// TimeOut of a broken, or valid but unknown sequence
		break;
	case -2:
		// Unknown Termination Char
		break;
	case -4:
		// Overflow, Too Long
		break;
	case -5:
		// Unexpected End Of Text
		break;

	default:
		break;
	}
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
	ResFg();	
	}
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
		if (isOnUsrESC27 && i < 1){
			if (clock() > timeOnUsrEsc){
				// UsrESC
				i = 27;
			}
		}
		// Recognize timeout while receiving ESC
		else if (isOnESC27 && !isOnUsrESC27 && i < 0){
			// The !isOnUsrESC27 signals we already got more chars than just the 1st ESC
			if (clock() > timeOnEsc){
				// Broken, or valid and unknown, Sequence
				i = -1;
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
				case 117:
					// Mouse UP (Left / Wheel / Right)
					if ((gMouseSelX == gMousePosX) && (gMouseSelY == gMousePosY)){
						// it's a (dbl)click
						if (isOnClick && clock() < timeOnClick){
							// dblClick
							EventESC27(513);
							isOnClick = 0;
							printf("dblClick\n");
						}
						else{
							// 1st Click
							isOnClick = 1;
							timeOnClick = clock() + gMouseClickTimeout;
						}							
					}
					else{
						// it's an area
						EventESC27(514);
						isOnClick = 0;
						printf("Area\n");
					}
					break;
				default:
					break;
				}
				if (r){
					// Disable timeouts
					isOnUsrESC27 = 0; isOnESC27 = 0;
					EventESC27(r);
				}
				
			// Loop Minimum
			
			if (r > 0){
				TxtBold(1);
				printf("  : %s\t%s\n\n\n", KeyID2String[r], &gStreamInESC27[1]);
				TxtBold(0);					
			}

	// Loop Minimum
		}

		// Recognize Single Click
		if (isOnClick && (clock() > timeOnClick)){
			// click
			EventESC27(512);
			isOnClick = 0;
			printf("Click\n");
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
					EventESC27(109);
				}
				else {
					DoEvents();
				}	
			#else
				if (gSignalInterval){
					if (gSignalTerminalSize){
						GetTerminalSize(3);
						EventESC27(109);
						gSignalTerminalSize = 0;
					}
					gSignalInterval = 0;
				}
				else {
					DoEvents();
				}	
			#endif
		}

		/* just for one loop active !
			Reset as last Action of the loop! */
		if (gSecondChanged){
			EraseTimeChange();
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



