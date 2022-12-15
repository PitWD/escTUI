/* PoorTui.h

       Primary Definitions, Structures and Functions to generate a  ! p o o r ! - ! T U I !

		     	! Could contain 3rd party MIT/GPL/"FreeSoftware" technologies !
		    (see detailed comments in related declarations/functions and at the EOF)

	This is a  ! p_o_o_r !  Application by (c) Pit Demmer. (PIT-Licensed 01.04.2022 - xx.xx.xxxx)
	 PIT-licensed" isn't a joke! - Right now I do not know about the license model I wanna use!
		
        			    			But Sources will be free!
	 	
           I'm just struggling with professional 3rd parties distributing binaries
	    (or executing a compiling process on customer machines) without contributing to
			                           "FreeSoftware"
           So, do not distribute binaries/hexdumps without being in contact with me!
*/

#include "AnsiESC.h"

typedef struct TabStyleSTRUCT {
    unsigned char Left;                                     // unscaled / unmoved start of Tab
    unsigned char Width;
    TxtStyleSTRUCT *pStyleEnabledOdd;
	TxtStyleSTRUCT *pStyleDisabledOdd;          
    TxtStyleSTRUCT *pStyleEnabledEven;
	TxtStyleSTRUCT *pStyleDisabledEven;          
	struct TabStyleSTRUCT *NextTab;
    unsigned char FixedLeft :1;
    unsigned char FixedWidth :1;
    unsigned char AddFrontSpace :1;
    unsigned char AddBackSpace :1;
    unsigned char Align :2;									// 0 = Left  |  1 = Center  |  2 = Right
} TabStyleSTRUCT;

typedef struct {
	// 
	TxtStyleSTRUCT *pTxtEnabled;
	TxtStyleSTRUCT *pTxtDisabled;
	char *pHorzLineChar, *pVertLineChar;				    //      or Null for disabling the couple
	char *pTopLeftChar, *pTopRightChar;
	char *pBottomLeftChar, *pBottomRightChar;
	unsigned char TopLine :1;								// If line get drawn (if not both disabled (see above))
	unsigned char BottomLine :1;							// ""  ""   "    ""
	unsigned char LeftLine :1;								// ""  ""   "    ""
	unsigned char RightLine :1;								// ""  ""   "    ""
} FrameStyleSTRUCT;

typedef struct {
	// 5 (10) Byte
	TxtStyleSTRUCT *pTxtEnabledID;
	TxtStyleSTRUCT *pTxtDisabledID;
	char *pScrollHorzCharID, *pScrollVertCharID, *pScrollBlankCharID;
} ScrollStyleSTRUCT;

struct AreaSTRUCT {
	// at least xx (xx) Byte

	struct AreaSTRUCT *pParentArea;								// (Null = Terminal)
															/* If PopUp has Width = 0 Then
																	FullScreen
																else
																	Centered
															*/
	
	unsigned char TopBot, LeftRight, Height, Width;			/* Virtual(vectorial) Position and Dimensions
															   Safest way is to do the virtial screen design 
															   in the smallest to exepect real-size.
															   So, 255 x 255 is far enough for the Definition
															   */
	unsigned char MinHeight, MinWidth;						// If reality gets smaller - use scroll-bars

// Frame
	FrameStyleSTRUCT *pFrameStyle;    	        			// or NULL for frameless area
// Scrollbars
	ScrollStyleSTRUCT *pScrollStyle;						// or Null for both all time disabled
// Color & Text Style	
	TxtStyleSTRUCT *pTxtEnabled;
	TxtStyleSTRUCT *pTxtDisabled;

// to calculate visible Area for Child draw
	unsigned int ScrollPosHorz, ScrollPosVert;				// !! must have at least 32 Bit !!

	struct AreaSTRUCT *pNextArea;

// Which edge is Referenz (UpperLeft is standard)
	unsigned char AlignRight :1;
	unsigned char AlignBottom :1;

// Options / Properties (8 Byte)
	unsigned char HotKey :6;								/* To make Area the active area
																see list in AnsiESC.h
																*/
	unsigned char HotKeyShow :2;							/*  0		= Don't show
																1 		= Left
																2		= Center
																3		= Right
																*/

// ScrollBars to Draw:
	unsigned char ScrollX :2;								// 0 = Never | 1 = Automatic | 2 = Ever
	unsigned char ScrollY :2;								// 0 = Never | 1 = Automatic | 2 = Ever

//	How To Scale & Move										// can fix areas on reference edge
	unsigned char MoveX :1;
	unsigned char MoveY :1;
	unsigned char ScaleX :1;
	unsigned char ScaleY :1;

//  PopUp Options (PopUps - of course using Scale & Move Options, too)
    unsigned char IsPopUp :1;
    unsigned char IsMaximized :1;                           
    unsigned char IsCentered :1;

 };

typedef struct {

	// 7 Byte
	unsigned char *pTxt;                                    // (1st) Text to print 
	unsigned char TxtCnt;                                   // Lines to print (0 = All (if more than 255))
	unsigned char *pArea;									// Parent area
	unsigned char *pTxtStyle;
	unsigned char VerticalAlign :4;							/*	0 = On Frame
																1 = 1st Line
																2 = 2nd Line
																3 = Pre Last Line
																4 = Last Line
																5 = Center
																6 = Next
																7 = Next (with space-line in front)
															*/
	unsigned char HorizontalAlign :4;						/*	0 = On Frame
																1 = Left 
																2 = Left (with space in front)
																3 = Right (on Frame)
																4 = Right
																5 = Right (with added space)
																6 = (Next) Tab
																*/
} TextLineSTRUCT; 

// User EVENTS
int EventESC27 (int event){
	
	int r = 0;		// Return Value
	
	// Modification Keys
	int keyState = (gKeyShift) + (gKeyAlt * 2) + (gKeyCtrl * 4);

	if (gKeyShift){
		#if IS_TERMINAL_EVENT_DEBUG
			printf("Shift-");
		#endif
	}
	if (gKeyAlt){
		#if IS_TERMINAL_EVENT_DEBUG
			printf("Alt-");
		#endif
	}
	if (gKeyCtrl){
		#if IS_TERMINAL_EVENT_DEBUG
			printf("Ctrl-");
		#endif
	}
	
	#if IS_TERMINAL_EVENT_DEBUG
		TxtBold(1);
	#endif

	if (event == -1){
		// A regular Key got pressed
	}
	else if (event > 143 && event < 157){
		// Navigation Keys
		switch (event){
		case 144:
			// Up
			#if IS_TERMINAL_EVENT_DEBUG
				printf("Up");
			#endif
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
			#if IS_TERMINAL_EVENT_DEBUG
				printf("Down");
			#endif
			break;
		case 146:
			// Right
			#if IS_TERMINAL_EVENT_DEBUG
				printf("Right");
			#endif
			break;
		case 147:
			// Left
			#if IS_TERMINAL_EVENT_DEBUG
				printf("Left");
			#endif
			break;
		case 148:
			// Center
			#if IS_TERMINAL_EVENT_DEBUG
				printf("Center");
			#endif
			break;
		case 149:
			// End
			#if IS_TERMINAL_EVENT_DEBUG
				printf("End");
			#endif
			break;
		case 150:
			// Unknown
			#if IS_TERMINAL_EVENT_DEBUG
				printf("150");
			#endif
			break;
		case 151:
			// Pos1
			#if IS_TERMINAL_EVENT_DEBUG
				printf("Pos1");
			#endif
			break;
		case 152:
			// Ins
			#if IS_TERMINAL_EVENT_DEBUG
				printf("Ins");
			#endif
			break;
		case 153:
			// Del
			#if IS_TERMINAL_EVENT_DEBUG
				printf("Del");
			#endif
			break;
		case 155:
			// PgUp
			#if IS_TERMINAL_EVENT_DEBUG
				printf("PgUp");
			#endif
			break;
		case 156:
			// PgDown
			#if IS_TERMINAL_EVENT_DEBUG
				printf("PgDown");
			#endif
			break;
		}
	}
	else if (event > 126 && event < 140){
		// Delete & F1 - F12
		
		#if IS_TERMINAL_EVENT_DEBUG
			if (event != 127){
				printf("F%d", event - 127);
			}
		#endif
		
		switch (event){
		case 127:
			#if IS_TERMINAL_EVENT_DEBUG
				printf("Back");
			#endif
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
		#if IS_TERMINAL_EVENT_DEBUG
			printf("%c", (char)event);
		#endif
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
		#if IS_TERMINAL_EVENT_DEBUG
			printf("CC: %c", (char)event+64);
		#endif
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
			#if IS_TERMINAL_EVENT_DEBUG
				printf("CursorPos");
			#endif
			break;
		case 177:
			// Terminal Size received (ESC-Sequence) /polled (WIN) / signaled (Mac/Linux)
			#if IS_TERMINAL_EVENT_DEBUG
				printf("ScreenSize");
			#endif
			if (ScreenSizeChanged()){
				#if IS_TERMINAL_EVENT_DEBUG
					printf(" (Changed)");
				#endif
			}
			break;
		case 179:
			// Terminal Icon Label
			#if IS_TERMINAL_EVENT_DEBUG
				printf("IconLabel");
			#endif
			break;
		case 178:
			// Terminal Name
			#if IS_TERMINAL_EVENT_DEBUG
				printf("TerminalName");
			#endif
			break;
		case 176:
			// Unknown Terminal Info Object
			#if IS_TERMINAL_EVENT_DEBUG
				printf("UTO");
			#endif
			break;

		// Terminal GotFocus / LostFocus
		case 160:
			// Got
			#if IS_TERMINAL_EVENT_DEBUG
				printf("GotFocus");
			#endif
			break;
		case 161:
			// Lost
			#if IS_TERMINAL_EVENT_DEBUG
				printf("LostFocus");
			#endif
			break;

		// 1st Level (From GetESC27()) Mouse-Events
		case 166:
			// Mouse Move
			#if IS_TERMINAL_EVENT_DEBUG
				printf("MouseMove");
			#endif
			break;
		case 165:
			// MouseUp
			switch (gMouseButton){
			case 1:
				// Left
				#if IS_TERMINAL_EVENT_DEBUG
					printf("MouseLeftUp");
				#endif
				break;
			case 4:
				// Right
				#if IS_TERMINAL_EVENT_DEBUG
					printf("MouseRightUp");
				#endif
				break;
			case 2:
				// Wheel
				#if IS_TERMINAL_EVENT_DEBUG
					printf("MouseWheelUp");
				#endif
				break;
			default:
				// MultiKey (never seen in reality)
				#if IS_TERMINAL_EVENT_DEBUG
					printf("MouseMultiKeyUp");
				#endif
				break;
			}
			break;
		case 162:
			// Left Mouse Down
			#if IS_TERMINAL_EVENT_DEBUG
				printf("MouseLeftDown");
			#endif
			break;
		case 167:
			// Left Down Mouse Move
			#if IS_TERMINAL_EVENT_DEBUG
				printf("MouseLeftDownMove");
			#endif
			break;
		case 164:
			// Right Mouse Down
			#if IS_TERMINAL_EVENT_DEBUG
				printf("MouseRightDown");
			#endif
			break;
		case 169:
			// Right Down Mouse Move
			#if IS_TERMINAL_EVENT_DEBUG
				printf("MouseRightDownMove");
			#endif
			break;
		case 163:
			// Wheel Mouse Down
			#if IS_TERMINAL_EVENT_DEBUG
				printf("MouseWheelDown");
			#endif
			break;
		case 168:
			// Wheel Down Mouse Move
			#if IS_TERMINAL_EVENT_DEBUG
				printf("MouseWheelDownMove");
			#endif
			break;
		case 170:
			// WheelScrollUp
			#if IS_TERMINAL_EVENT_DEBUG
				printf("MouseWheelScrollUp");
			#endif
			break;
		case 171:
			// WheelScrollDown
			#if IS_TERMINAL_EVENT_DEBUG
				printf("MouseWheelScrollDown");
			#endif
			break;
		case 172:
			// Unknown Mouse Object
			#if IS_TERMINAL_EVENT_DEBUG
				printf("UMO");
			#endif
			break;
		}
	}
	else if (event > 199 && event < 203){
		// 2nd Level (From Loop()) Mouse-Events
		
		switch (gMouseButton){
		case 1:
			// Left
			#if IS_TERMINAL_EVENT_DEBUG
				printf("Left-");
			#endif
			break;
		case 4:
			// Right
			#if IS_TERMINAL_EVENT_DEBUG
				printf("Right-");
			#endif
			break;
		case 2:
			// Wheel
			#if IS_TERMINAL_EVENT_DEBUG
				printf("Wheel-");
			#endif
			break;
		default:
			// MultiKey (never seen in reality)
			#if IS_TERMINAL_EVENT_DEBUG
				printf("Multi-");
			#endif
			break;
		}

		switch(event){
		case 200:
			// Click
			#if IS_TERMINAL_EVENT_DEBUG
				printf("Click");
			#endif
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
			#if IS_TERMINAL_EVENT_DEBUG
				printf("DblClick");
			#endif
			break;
		case 202:
			// Area
			#if IS_TERMINAL_EVENT_DEBUG
				printf("Area");
			#endif
			break;

		}
	}
	else{
		// Errors
		switch(event){
		case -3:
			// TimeOut of a broken, or valid but unknown sequence
			#if IS_TERMINAL_EVENT_DEBUG
				printf("ERR -3 TimeOut");
			#endif
			break;
		case -2:
			// Unknown Termination/Identification Char
			#if IS_TERMINAL_EVENT_DEBUG
				printf("ERR -2 Termination/Identification");
			#endif
			break;
		case -4:
			// Overflow, Too Long
			#if IS_TERMINAL_EVENT_DEBUG
				printf("ERR -4 Overflow");
			#endif
			break;
		case -5:
			// Unexpected End Of Text
			#if IS_TERMINAL_EVENT_DEBUG
				printf("ERR -5 UnexpectedEOT");
			#endif
			break;
		case -6:
			// Overlapping Sequence - already done in CoreLoop
			#if IS_TERMINAL_EVENT_DEBUG
				printf("ERR -6 Overlapping");
			#endif
			break;
		case -7:
			// ByteMouse Out Of Range
			#if IS_TERMINAL_EVENT_DEBUG
				printf("ERR -7 ByteMouse");
			#endif
			break;

		default:
			// Unexpected...
			#if IS_TERMINAL_EVENT_DEBUG
				printf("ERR - WTF");
			#endif
			break;
		}
	}
	gKeyAlt = 0; gKeyCtrl = 0; gKeyMeta = 0; gKeyShift = 0;

	if (event > 31){
		#if IS_TERMINAL_EVENT_DEBUG
			printf("  : %s\n", &gStreamInESC27[1]);
		#endif
	}
	else{
		#if IS_TERMINAL_EVENT_DEBUG
			printf("\n");
		#endif
	}

	#if IS_TERMINAL_EVENT_DEBUG
		// To prevent Debug-PrintOut irritations under WIN
		// (while ScreenSize - Polling)
		gStreamInESC27[1] = 0;

		if (event != -1){
			/* code */
			printf("Event: %d", event);

			printf("\n\n\n");

		}

		TxtBold(0);					
	#endif

	return r;
}

void EventDayChange(void){
	#if IS_TIME_EVENT_DEBUG
		printf("    DayChange: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}

void EventHourChange(void){	
	#if IS_TIME_EVENT_DEBUG
		printf("   HourChange: %s - %s\n",gStrRunTime, gStrTime);
	#endif		
}
void EventHour2Change(void){
	#if IS_TIME_EVENT_DEBUG
		printf("  Hour2Change: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}
void EventHour3Change(void){
	#if IS_TIME_EVENT_DEBUG	
		printf("  Hour3Change: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}
void EventHour4Change(void){
	#if IS_TIME_EVENT_DEBUG	
		printf("  Hour4Change: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}
void EventHour6Change(void){
	#if IS_TIME_EVENT_DEBUG
		printf("  Hour6Change: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}
void EventHour8Change(void){
	#if IS_TIME_EVENT_DEBUG
		printf("  Hour8Change: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}
void EventHour12Change(void){
	#if IS_TIME_EVENT_DEBUG
		printf(" Hour12Change: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}

void EventMinuteChange(void){
	#if IS_TIME_EVENT_DEBUG
		printf("    MinChange: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}
void EventMinute2Change(void){
	#if IS_TIME_EVENT_DEBUG
		printf("   Min2Change: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}
void EventMinute3Change(void){
	#if IS_TIME_EVENT_DEBUG
		printf("   Min3Change: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}
void EventMinute4Change(void){
	#if IS_TIME_EVENT_DEBUG
		printf("   Min4Change: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}
void EventMinute5Change(void){
	#if IS_TIME_EVENT_DEBUG
		printf("   Min5Change: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}
void EventMinute6Change(void){
	#if IS_TIME_EVENT_DEBUG
		printf("   Min6Change: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}
void EventMinute10Change(void){
	#if IS_TIME_EVENT_DEBUG
		printf("  Min10Change: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}
void EventMinute12Change(void){
	#if IS_TIME_EVENT_DEBUG
		printf("  Min12Change: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}
void EventMinute15Change(void){
	#if IS_TIME_EVENT_DEBUG
		printf("  Min15Change: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}
void EventMinute20Change(void){
	#if IS_TIME_EVENT_DEBUG
		printf("  Min20Change: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}
void EventMinute30Change(void){
	#if IS_TIME_EVENT_DEBUG
		printf("  Min30Change: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}

void EventSecondChange(void){
	#if IS_TIME_EVENT_DEBUG
		printf("    SecChange: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}
void EventSecond2Change(void){
	#if IS_TIME_EVENT_DEBUG
		printf("   Sec2Change: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}
void EventSecond3Change(void){
	#if IS_TIME_EVENT_DEBUG
		printf("   Sec3Change: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}
void EventSecond4Change(void){
	#if IS_TIME_EVENT_DEBUG
		printf("   Sec4Change: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}
void EventSecond5Change(void){
	#if IS_TIME_EVENT_DEBUG
		printf("   Sec5Change: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}
void EventSecond6Change(void){
	#if IS_TIME_EVENT_DEBUG
		printf("   Sec6Change: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}
void EventSecond10Change(void){
	#if IS_TIME_EVENT_DEBUG
		printf("  Sec10Change: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}
void EventSecond12Change(void){
	#if IS_TIME_EVENT_DEBUG
		printf("  Sec12Change: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}
void EventSecond15Change(void){
	#if IS_TIME_EVENT_DEBUG
		printf("  Sec15Change: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}
void EventSecond20Change(void){
	#if IS_TIME_EVENT_DEBUG
		printf("  Sec20Change: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}
void EventSecond30Change(void){
	#if IS_TIME_EVENT_DEBUG
		printf("  Sec30Change: %s - %s\n",gStrRunTime, gStrTime);
	#endif
}


/*
										EOF - Detailed Description

                8-Bit AVR's are the main target (with SerialPort as stdin and stdout)

				App-State   : Pre Alpha
				App-Version : 1.00pa
				App-Date    :

				Dev-Start   : 01.04.2022
				1st Alpha   :
				1st Beta    :
				1st Release :

STRUCTURES		            Creation Date   Alpha Date      BetaDate        Creator
    AreaStruct              03.04.2022                                      Pit
    ScrollStyleSTRUCT       04.04.2022                                      Pit
    TextLineStruct          04.04.2022                                      Pit
    FrameStyleSTRUCT        05.04.2022                                      Pit
    TabStyleSTRUCT          07.04.2022                                      Pit

Files			            Creation Date   Alpha Date      BetaDate        Creator
    <PoorTui.h>		        01.04.2022		                                Pit

BugID		Description									Found by / Date		Fixed by / Date
*/


