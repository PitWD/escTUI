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
	
	switch (event){
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
		// Terminal Size
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
		switch (mouseButton){
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
	case 192:
		// Click
		switch (mouseButton){
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
	case 193:
		// DblClick
		switch (mouseButton){
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
	case 194:
		// Area
		switch (mouseButton){
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
	case 195:
		// Terminal-Size Changed
		break;
	default:
		break;
	}
}

void EventDayChange(void){

}

void EventHourChange(void){	
	// Check on DayChange
	if (dayChanged){
		EventDayChange();
	}
}

void EventHour2Change(void){}
void EventHour3Change(void){}
void EventHour4Change(void){}
void EventHour6Change(void){}
void EventHour8Change(void){}
void EventHour12Change(void){}

void EventMinuteChange(void){

	// Check on HourChange
	if (hourChanged){
		EventHourChange();
		if (hour2Changed){
			EventHour2Change();
			if (hour4Changed){
				EventHour4Change();
				if (hour8Changed){
					EventHour8Change();
				}
			}
		}
		if (hour3Changed){
			EventHour3Change();
			if (hour6Changed){
				EventHour6Change();
				if (hour12Changed){
					EventHour12Change();
				}
			}
		}
	}
}

void EventMinute2Change(){}
void EventMinute3Change(){}
void EventMinute4Change(){}
void EventMinute5Change(){}
void EventMinute6Change(){}
void EventMinute10Change(){}
void EventMinute12Change(){}
void EventMinute15Change(){}
void EventMinute20Change(){}
void EventMinute30Change(){}

void EventSecondChange(void){	
	
	// Check on MinuteChange
	if (minuteChanged){
		EventMinuteChange();
		if (min2Changed){
			EventMin2Change();
			if (min4Changed){
				EventMin4Change();
				if (min20Changed){
					EventMin20Change();
				}
			}
		}
		if (min3Changed){
			EventMin3Changed();
			if (min6Changed){
				EventMin6Changed();
				if (min12Changed){
					EventMin12Change();
				}
			}
			if (min15Changed){
				EventMin15Changed();
				if (min30Changed){
					EventMin30Change();
				}
			}			
		}
	}
}

void MonitorGetESC27(void){

	int i = 0;
	int r = 0;
	char c = 0;


	ClearScreen();


	TrapMouse(1);

	// ScreenSize
	// printf("%s18t", CSI);

	
	r = WaitForESC27("\x1B[18t",0.2);
	if (r > 0){
		printf("r: %d  :%s\n", r, &streamInESC27[1]);
	}
	r = 0;

	
	int quatsch = 0;

	// Recognize manual ESC
		_Bool isOnESC27 = 0;
		clock_t timeOnUsrEsc;
	// Recognize Click & DblClick / Area-Selection
		_Bool isOnClick = 0;
		clock_t timeOnClick;
		int posXonClick = 0;
		int posYonClick = 0;

	// Loop Minimum
	while (i != 10 && i != 13){
		
			i = getch();

			// Recognize manual ESC
			if (isOnESC27 && i < 1){
				if (clock() > timeOnUsrEsc){
					// UsrESC
					i = 27;
				}
			}
			else if (i == 27){
				isOnESC27 = 1;
				timeOnUsrEsc = clock() + userEscTimeout;	// Timing interacts with Loop-Sleep...
			}
			else{
				isOnESC27 = 0;
			}

			
	// Loop Minimum

	if (!quatsch){
		// printf("\x1B[20t");		// IconLabel
		printf("\x1B[21t\n");		// WinTitle
		// printf("\x1B[18t");		// TerminalSize		
		quatsch = 1;
	}
	
	// printf("loop");

	// Loop Minimum
		if (i > 0){
	// Loop Minimum
			/*
			if(i == 32){
				printf("\n");
			}
			
			else{
			*/	
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
				
				// Loop Minimum
				
					r = GetESC27(i);
					switch (r){
					case 0:
						// Nothing
						break;
					case -1:
						// Regular Key - No ESC-Sequence related stuff
						break;
					case 117:
						// Mouse UP (Left / Wheel / Right)
						if ((mouseSelX == mousePosX) && (mouseSelY == mousePosY)){
							// it's a (dbl)click
							if (isOnClick && clock() < timeOnClick){
								// dblClick
								EventESC27(193);
								isOnClick = 0;
								printf("dblClick\n");
							}
							else{
								// 1st Click
								isOnClick = 1;
								timeOnClick = clock() + mouseClickTimeout;
							}							
						}
						else{
							// it's an area
							EventESC27(194);
							isOnClick = 0;
							printf("Area\n");
						}
						break;
					case 109:
						// Terminal-Size
						if ((screenWidth != screenWidthPrev) || (screenHeight != screenHeightPrev)){
							// Terminal Size changed... (Event)
							screenHeightPrev = screenHeight;
							screenWidthPrev = screenWidth;
							EventESC27(195);
						}
						break;
					case 158:
						// GotFocus
						GetTerminalSize();
						EventESC27(158);
						break;
					case -2:
						// Unknown Termination Char
					case -4:
						// Overflow, Too Long
					case -5:
						// Unexpected End Of Text
						break;
					default:
						EventESC27(r);
						break;
					}
				// Loop Minimum
				
				if (r > 0){
					isOnESC27 = 0;
					TxtBold(1);
					printf("  : %s\t%s\n\n\n", KeyID2String[r], &streamInESC27[1]);
					TxtBold(0);					
				}
			// }

	// Loop Minimum
		}

		// Recognize Single Click
		if (isOnClick && (clock() > timeOnClick)){
			// click
			EventESC27(192);
			isOnClick = 0;
			printf("Click\n");
		}

		CheckOnTimeChange();
		if (secondChanged){
			// A Second (ore more) is over
			EventSecondChange();	// Activates all other Minute/Hour/DayChange Events
		}
		
		// DO STUFF HERE START

		// DO STUFF HERE STOP

		// End Of Loop
			/* ust for one loop active !
			 Reset as last Action of the loop! */
			EraseTimeChange();


		usleep(100);

	}
	// Loop Minimum

	// Reset Mouse	
	TrapMouse(0);

	// Reset old Terminal Settings
	#ifdef __unix__
		tcsetattr(STDIN_FILENO, TCSANOW, &old_io);
	#endif
}

void MonitorGetESC27II(void){

	int i = 0;
	int r = 0;
	char c = 0;


	ClearScreen();

	TrapMouse(1);

	// ScreenSize
	// printf("%s18t", CSI);
	r = WaitForESC27("\x1B[18t",0.2);


	// Loop Minimum

	// Recognize manual ESC
	_Bool isOnESC27 = 0;
	clock_t timeOnUsrEsc;
	// Recognize Click & DblClick / Area-Selection
	_Bool isOnClick = 0;
	clock_t timeOnClick;

	while (i != 10 && i != 13){
		
		i = getch();

		// Recognize manual ESC
		if (isOnESC27 && i < 1){
			if (clock() > timeOnUsrEsc){
				// UsrESC
				i = 27;
			}
		}
		else if (i == 27){
			isOnESC27 = 1;
			timeOnUsrEsc = clock() + userEscTimeout;	// Timing interacts with Loop-Sleep...
		}
		else{
			isOnESC27 = 0;
		}
			
	// Loop Minimum

	// Loop Minimum

		if (i > 0){

			r = GetESC27(i);

			switch (r){
			case 0:
				// Nothing
				break;
			case -1:
				// Regular Key - No ESC-Sequence related stuff
				printf("%c",i);
				fflush(stdout);
				break;
			case 117:
				// Mouse UP (Left / Wheel / Right)
				if ((mouseSelX == mousePosX) && (mouseSelY == mousePosY)){
					// it's a (dbl)click
					if (isOnClick && clock() < timeOnClick){
						// dblClick
						EventESC27(193);
						isOnClick = 0;
					}
					else{
						// 1st Click
						isOnClick = 1;
						timeOnClick = clock() + mouseClickTimeout;
					}							
				}
				else{
					// it's an area
					EventESC27(194);
					isOnClick = 0;
				}
				break;
			case 109:
				// Terminal-Size
				if ((screenWidth != screenWidthPrev) || (screenHeight != screenHeightPrev)){
					// Terminal Size changed... (Event)
					screenHeightPrev = screenHeight;
					screenWidthPrev = screenWidth;
					EventESC27(195);
				}
				break;
			case 158:
				// GotFocus
				GetTerminalSize();
				EventESC27(158);
				break;
			case -2:
				// Unknown Termination Char
			case -4:
				// Overflow, Too Long
			case -5:
				// Unexpected End Of Text
				break;
			default:
				EventESC27(r);
				break;
			}
		}

		// Recognize Single Click
		if (isOnClick && (clock() > timeOnClick)){
			// click
			EventESC27(192);
			isOnClick = 0;
		}

		CheckOnTimeChange();
		if (secondChanged){
			// A Second (ore more) is over
			EventSecondChange();	// Activates all other Minute/Hour/DayChange Events
		}
		
		// DO STUFF HERE START

		// DO STUFF HERE STOP

		// End Of Loop
			/* Just for one loop active !
			 Reset as last Action of the loop! */
			secondChanged = 0;
			minuteChanged = 0;
			hourChanged = 0;
			dayChanged = 0;

		usleep(100);
	}
	// Loop Minimum

	// Reset Mouse	
	TrapMouse(0);

	// Reset old Terminal Settings
	#ifdef __unix__
		tcsetattr(STDIN_FILENO, TCSANOW, &old_io);
	#endif
}

int main() {
	
	/*
	strcpy(symbolMisc[29].str, "☜");	// \u261C	WHITE LEFT POINTING INDEX
	strcpy(symbolMisc[30].str, "☝");	// \u261D	WHITE UP POINTING INDEX
	strcpy(symbolMisc[31].str, "☞");	// \u261E	WHITE RIGHT POINTING INDEX
	strcpy(symbolMisc[32].str, "☟");	// \u261F	WHITE DOWN POINTING INDEX
	*/
	InitTiming();

	printf("Hello New Project\n");
	
	InitEscSeq(); InitColors();
	
	printf("After Inits\n");
	
	ClearScreen();
	
	printf("After ClrScr\n");
		
	GetTerminalSize();

	printf("After Size\n");

	Locate(screenWidth / 2 - 10, screenHeight / 2);
	printf("Width: %d  Height: %d\n", screenWidth, screenHeight);

	/*	const BD_lADR = "╭";		// \x256D	LIGHT ARC DOWN AND RIGHT
	const BD_lADL = "╮";		// \x256E	LIGHT ARC DOWN AND LEFT
	const BD_lAUL = "╯";		// \x256F	LIGHT ARC UP AND LEFT
	const BD_lAUR = "╰";		// \x2570	LIGHT ARC UP AND RIGHT
	*/

	printf("\u2500");
	printf("%s\n", "╮");
	printf("╰");
	printf("%s\n", "╯");

	ClearScreen();

	// *************************************************************
	MonitorGetESC27II();
	return 0;
	// *************************************************************

	int i = 0;
	int r = 0;

	TrapMouse(1);

	// Recognize manual ESC
	_Bool isOnESC27 = 0;
	clock_t timeOnUsrEsc;

	do{
		
		i = getch();

		// Recognize manual ESC
		if (isOnESC27 && i < 1){
			if (clock() > timeOnUsrEsc){
				// UsrESC
				i = 27;
			}
		}
		else if (i == 27){
			isOnESC27 = 1;
			timeOnUsrEsc = clock() + userEscTimeout;	// Timing interacts with Loop-Sleep...
		}
		else{
			isOnESC27 = 0;
		}
			
		if (i > 0){
			r = GetESC27(i);
		}

		usleep(100);

	} while(1);
	
	// Reset Mouse	
	TrapMouse(0);

	// Reset old Terminal Settings
	#ifdef __unix__
		tcsetattr(STDIN_FILENO, TCSANOW, &old_io);
	#endif
	// **************************************************************

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



