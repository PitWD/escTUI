
//		       Terminal Functions - See detailed description at the EOF !
//				! Using 3rd party MIT/GPL/"FreeSoftware" technologies !
//				  (see related declarations/functions and at the EOF)
// This is a  ! p_o_o_r !  Header by (c) Pit Demmer. (PIT-Licensed 01.04.2022 -           )

const char TerminalVersion [] = "1.00pa";

#include <stdio.h>
#include <stdlib.h>
#include "Timing.h"
#include <signal.h>

#if __WIN32__ || _MSC_VER || __WIN64__
    #include <conio.h>
    #include <windows.h>
#else // __unix__
    #include <termios.h>
    #include <unistd.h>
    #include <sys/ioctl.h>
#endif

#define ESC27_EXCHANGE_SIZE 33          // Has to be at least 1 greater than greatest to expect Command/Response !!
#define ESC27_STREAM_IN_SIZE 33         // Has to be >= ESC27_EXCHANGE_SIZE !!
//unsigned char streamInESC27[ESC27_STREAM_IN_SIZE];
char gStreamInESC27[ESC27_STREAM_IN_SIZE];

// Screen
int gScreenWidth = 0;
int gScreenHeight = 0;
int gScreenWidthPrev = 0;
int gScreenHeightPrev = 0;
int gScreenSizeInCursorPos = 0;

// Keyboard
int gCursorPosX = 0;
int gCursorPosY = 0;
int gCursorSelX = 0;                     // 1st point of selection rectangle
int gCursorSelY = 0;                     // or same as CursorPosX&Y = No selection
int gCursorWaitFor = 0;					// Shift OR Alt OR Ctrl + F3 
										// Has 7 overlapping Settings with CursorPos
										// Set cursorIsAsked true when asking 'manually' for the cursor position !!
_Bool gCursorIsSelecting = 0;            // (Moving with Shift)

// Mouse
int gMousePosX = 0;
int gMousePosY = 0;
int gMouseSelX = 0;                     // 1st point of selection rectangle
int gMouseSelY = 0;                     // or same as MousePosX&Y = No selection
int gMouseButton = 0;				   // 1 = left, 2 = wheel, 4 = right

// ESC27 Reading
_Bool isWaitingForESC27 = 0;

// Signals
int gSignalCtrlC = 0;
int gSignalTerminalSize = 0;
int gSignalInterval = 0;

/**
 * @brief Declaration FUNCTIONS
 */
int SetVT(int set);
int InKey(void);
void FlushInKey(void);
int ClearScreen(int set);
int GetTerminalSize(int set);
int ScreenSizeChanged(void);
int GetESC27 (int c);
int WaitForESC27(char *pStrExchange, int waitForID, float timeOut);
void SignalHandler(int sig);
void TrapMouse(int set);
void TrapFocus(int set);

/**
 * @brief 	Set/Reset output mode to handle virtual terminal sequences
 * 			and for (LINUX/Mac) to (re)set c_break mode
 * 
 * @param 	set 
 * 			1 = Set
 * 			0 = Reset
 * 
 * @return 	int 
 * 			1 = Success
 * 			0 = Failed
 */

int SetVT(int set){

	#if __WIN32__ || _MSC_VER || __WIN64__

		static isSet = 0;
		static DWORD dwStartModeOut = 0;
		static DWORD dwStartModeIn = 0;

		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hOut == INVALID_HANDLE_VALUE){
			return 0;
		}
		HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
		if (hIn == INVALID_HANDLE_VALUE){
			return 0;
		}

		DWORD dwOriginalOutMode = 0;
		DWORD dwOriginalInMode = 0;
		if (!GetConsoleMode(hOut, &dwOriginalOutMode)){
			return 0;
		}
		if (!GetConsoleMode(hIn, &dwOriginalInMode)){
			return 0;
		}

		if (!isSet){
			dwStartModeOut = dwOriginalOutMode;
			dwStartModeIn = dwOriginalInMode;
			isSet = 1;
		}
		
		DWORD dwRequestedOutModes = 0;
		DWORD dwRequestedInModes = 0;
		DWORD dwOutMode = 0;
		
		if (set){
			dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
			dwRequestedInModes = ENABLE_VIRTUAL_TERMINAL_INPUT;
			dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
		}
		else{
			dwRequestedInModes = dwStartModeIn;
			dwOutMode = dwStartModeOut;
		}
		
		
		if (!SetConsoleMode(hOut, dwOutMode)){
			// we failed to set both modes, try to step down mode gracefully.
			
			if (set){
				dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING;
				dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
			}
			
			if (!SetConsoleMode(hOut, dwOutMode)){
				// Failed to set any VT mode, can't do anything here.
				return 0;
			}
		}

		DWORD dwInMode = 0;
		if (set){
			dwInMode = dwOriginalInMode | dwRequestedInModes;
		}
		else{
			dwInMode = dwStartModeIn;
		}
		
		
		if (!SetConsoleMode(hIn, dwInMode)){
			// Failed to set VT input mode, can't do anything here.
			return 0;
		}

		return 1;
	
	#else

		static struct termios new_io;
		static struct termios old_io;
		
		// Set = 1		Switch to cbreak mode (Disable ECHO and ICANON)
		// Set = 0		Switch Back

		if (set){
			// Save actual Terminal
			if((tcgetattr(STDIN_FILENO, &old_io)) == -1){
				return 0;
			}
			new_io = old_io;
			
			// Change Flags to cbreak-Mode
			new_io.c_lflag = new_io.c_lflag & ~(ECHO|ICANON);
			new_io.c_cc[VMIN] = 1;
			new_io.c_cc[VTIME]= 0;
			
			// Set cbreak-Mode
			if((tcsetattr(STDIN_FILENO, TCSAFLUSH, &new_io)) == -1){
				return 0;
			}
		}
		else{
			if((tcsetattr(STDIN_FILENO, TCSANOW, &old_io)) == -1){
				return 0;
			}
		}
		return 1;
	#endif
}	

/**
 * @brief	Non-Blocking GetChar
 * 
 * @return 	int 
 * 			1-255 	= char 
 * 			0		= there was no char in buffer
 */
int InKey(void){

	// Returns next char in stdin
	// or zero.

	#if __WIN32__ || _MSC_VER || __WIN64__

		if (kbhit()){
			return getch();
		}
		else {
			return 0;
		}

	#else

		static int cnt = 0;
		int c = 0;

		if(cnt < 1){	// Just this 'IF' is the 023. FIX
			// Seems silly to read buffer-size each time
			if((ioctl(0, FIONREAD , &c)) > -1){
				// and to then eventually (re)set cnt
				if (c > 0){
					cnt = c;
				}
				// but it's a MustHave... Direct FIONREAD into cnt only if !cnt - fails (sometimes) with scary effects.
			}
			else{
				cnt = 0;
			}
		}				// 023. FIX
		
		if (cnt){
			cnt--;
			return getchar();
		}
		else {
			return 0;
		}

	#endif
}

/**
 * @brief 	Flush buffer the "hard (and all time successful) way"
 */
void FlushInKey(void){
	while (InKey()){
		// Flush
	}
}

/**
 * @brief 	DoEvents() - release CPU during idle times
 */
#if __WIN32__ || _MSC_VER || __WIN64__
	#define DoEvents() Sleep(1);
#else
	#define DoEvents() usleep(DoEventsTime);
#endif 

/**
 * @brief 	Clear Screen - ESC & OS
 * 
 * @param 	set 0 = Use last used way to CLS
 * 				1 = Use ESC (Xterm)
 * 				2 = Use ESC (Dumb)
 * 				3 = Use OS
 * 			If set isn't valid it gets 3 (OperatingSystem)
 * 			There is no real (Xterm/Dumb)-Version like with GetTerminalSize!
 * 
 * @private	isSet (static int) holds the last used way to CLS
 * 
 * @return	int 1-3 = used way to CLS
 */
int ClearScreen(int set){
	
	static int isSet = 3;
	
	if (set){
		if (set < 1 || set > 3){
			isSet = 3;
		}
		isSet = set;
	}
	
	switch (isSet){
	case 1:
	case 2:
		printf("\x1B[2J");
		printf("\x1B[1;1H");
		break;
	case 3:
		#if __WIN32__ || _MSC_VER || __WIN64__
			system ("cls");
		#else 
			system ("clear");
		#endif
		break;
	default:
		break;
	}
	return isSet;
}

/**
 * @brief	Get Terminal Size - ESC & OS
 * 
 * @param 	set	0 = determine on 1st call "HowTo Get The Terminal-Size"
 * 				0 = from 2nd call "Use determined HowTo"
 * 				1 = ESC-Xterm
 * 				2 = ESC-Dumb (Billy-OS, some Linux)
 * 				3 = OS
 * 
 * @private	isSet (static int) holds the determined HowTo GetTerminalSize
 * @private r (dynamic int) just a helper
 * 
 * @return	int 1-3 = used way to GetTerminalSize
 * 			      0 = unable to GetTerminalSize
 */
int GetTerminalSize(int set){

	static int isSet = 0;

	int r = 0;

    #if __WIN32__ || _MSC_VER || __WIN64__
	#else
		struct winsize w;
	#endif

	if (!set){
		// use detected mode
		set = isSet;
	}
	else if (set < 0 || set > 3){
		// Correct bad 'set' values
		set = 3; 
	}
	
	switch (set){
	case 1:
		// Xterm
		printf("\x1B[18t");
		break;
	case 2:
		// Dumb
		gScreenSizeInCursorPos = 1;
		printf("\0337\x1B[999;9999H\x1B[6n\0338");
		break;
	case 0:
		// 1st run
		r = WaitForESC27("\x1B[18t",109,0.5);
		if (gScreenWidth > 0 && gScreenHeight > 0){
			isSet = 1;
			gScreenWidthPrev = gScreenWidth;
			gScreenHeightPrev = gScreenHeight;
			break;
		}
		gScreenSizeInCursorPos = 1;
		r = WaitForESC27("\0337\x1B[999;9999H\x1B[6n\0338",107,0.5);
		gScreenSizeInCursorPos = 0;
		if (gScreenWidth > 0 && gScreenHeight > 0){
			isSet = 2;
			gScreenWidthPrev = gScreenWidth;
			gScreenHeightPrev = gScreenHeight;
			break;
		}
	case 3:
		// System
        #if __WIN32__ || _MSC_VER || __WIN64__
	        /* Billy OS */
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
            screenWidth = (int)(csbi.srWindow.Right - csbi.srWindow.Left + 1);
            screenHeight = (int)(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
        #else 
			/* Mac & Linux */
            ioctl(fileno(stdout), TIOCGWINSZ, &w);
            gScreenWidth = (int)(w.ws_col);
            gScreenHeight = (int)(w.ws_row);
        #endif
		if (!isSet){
			if (gScreenWidth > 0 && gScreenHeight > 0){
				isSet = 3;
				gScreenWidthPrev = gScreenWidth;
				gScreenHeightPrev = gScreenHeight;
				break;
			}
			else {
				// Hell - No way to get the TerminalSize ?!?!
				return 0;
			}
		}
		break;
	}	
	
	if (set){
		return set;
	}
	else{
		return isSet;
	}
	
}

/**
 * @brief 	Check On If ScreenSize Has Changed
 * 
 * @return	int	1 = Changed
 * 				0 = Not Changed
 */
int ScreenSizeChanged(void){
	if ((gScreenWidth != gScreenWidthPrev) || (gScreenHeight != gScreenHeightPrev)){
		gScreenHeightPrev = gScreenHeight;
		gScreenWidthPrev = gScreenWidth;
		return 1;
	}
	return 0;
}

/**
 * @brief 	Analyze stream (char by char) on ESC-Sequences
 * 
 * @param 	c 1-255 = (next) char
 * 				-1	= CoreLoop recognized a TimeOut (reset statics/function)
 * 
 * @private	isValid (static int / bool)
 * 			True as long it could be a valid ESC sequence
 * 
 * @private	isCSI (static int / bool)
 * 			Becomes True if sequence starts with a CSI
 * 
 * @private isOSC (static int / bool)
 * 			Becomes True if sequence starts with an OSC
 * 
 * @private	isMouse (static int / bool)
 * 			Becomes True if sequence should be a DECIMAL mouse sequence
 
 * @private	isByteMouse (static int / bool)
 * 			Becomes True if sequence should be a BYTE mouse sequence
 * 			!! Do NEVER Use Byte Mode Terminals with height or width > 223 !!
 * 
 * @private allowTxt (static int / bool)
 * 			Becomes True if sequence should be a TEXT containing sequence
 * 
 * @private waitForEOT (static int / bool)
 * 			Becomes True (after ESC) when the next char must be a EndOfText
 * 
 * @private numCnt (static int)
 * 			counts the colon and semi-colon separated decimal numbers in a stream
 * 
 * @private *pNumPos[] (static char pointer)
 * 			Start(s) of recognized numbers
 * 
 * @private isNumGroup[] (static int / bool)
 * 			if number is/was a group (colon separated)
 * 
 * @private streamPos (static int)
 * 			counts the actual chars/position in the stream
 * 
 * @private r (dynamic int) helper for return
 * 			
 * @return	int	-2	= just the Termination Char was unknown.
				-1	= Unknown / Illegal
				-4	= overflow - too long
				-5	= unexpected EOT
				-3	= timeOut of a broken, or valid but unknown sequence
				 0	= Valid, but waiting for more Bytes to identify/finish sequence
				 n	= The ESC-Sequence (see related enum's)
 */
int GetESC27 (int c){

	static int isValid = 0;
	static int isCSI = 0;
	static int isOSC = 0;
	static int isMouse = 0;
	static int isByteMouse = 0;
	static int allowTxt = 0;
	static int waitForEOT = 0;
	static int numCnt = 0;

	static char *pNumPos[ESC27_STREAM_IN_SIZE];
	static int isNumGroup[ESC27_STREAM_IN_SIZE];
	static int streamPos = 0;

	int r = -1;		

	if (c == 127){
		// Back
		r = 106;
		if (isValid){
			// Alt-Back
			r += 51;
		}
		gStreamInESC27[1] = 0;
		goto SaveGetESC27ErrReturn;
	}
	else if (c < 32){
		switch (c){
		case 9:
			// TAB
			gStreamInESC27[1] = 0;
			r = 113;
			goto SaveGetESC27ErrReturn;
			break;
		case 10:
		case 13:
			// ENTER
			gStreamInESC27[1] = 0;
			r = 115;
			goto SaveGetESC27ErrReturn;
			break;
		case 27:
			// (Re)entering ESC Mode	
			if (allowTxt){
				// expecting "EndOfText" '\' as next char
				waitForEOT = 1;
				return 0;
			}
			
			if (isValid){
				// ESC came twice... p			
				if (!streamPos){
					// User ESC
					r = 108;
				}
				else{
					// looks like UserESC - but is BS (e.g. Overflow Mouse in ByteMode)
					r = 155;
				}
				goto SaveGetESC27ErrReturn;
			}
			
			isValid = 1;
			isCSI = 0;
			isOSC = 0;
			isMouse = 0;
			isByteMouse = 0;
			allowTxt = 0;
			waitForEOT = 0;
			streamPos = 0;
			numCnt = 0;
			gStreamInESC27[0] = 27;
			gStreamInESC27[1] = 0;		
			return 0;
			break;
		case -1:
			// TimeOut of a broken, or valid but unknown sequence
			r = -3;
			goto SaveGetESC27ErrReturn;

		default:
			// Ctrl-A - Ctrl-Z
			// (but a lot are special - see cases above
			//  a lot are also not supported on all OSs)
			r = 229 + c;
			goto SaveGetESC27ErrReturn;
			break;
		}
	}
	else {
		if (!isValid){
			// Something not related to ESC (all regular User Pressed Stuff...)
			return -1;
		}
	}

/*	
	else if (c == 9){
		// TAB
		streamInESC27[1] = 0;
		r = 113;
		goto SaveGetESC27ErrReturn;
	}
	else if (c == 10 || c == 13){
		// ENTER
		streamInESC27[1] = 0;
		r = 115;
		goto SaveGetESC27ErrReturn;
	}
	else if (c == 27){
		// (Re)entering ESC Mode	
		if (allowTxt){
			// expecting "EndOfText" '\' as next char
			waitForEOT = 1;
			return 0;
		}
		
		if (isValid){
			// ESC came twice... p			
			if (!streamPos){
				// User ESC
				r = 108;
			}
			else{
				// looks like UserESC - but is BS (e.g. Overflow Mouse in ByteMode)
				r = 155;
			}
			goto SaveGetESC27ErrReturn;
		}
		
		isValid = 1;
		isCSI = 0;
		isOSC = 0;
	 	isMouse = 0;
	 	isByteMouse = 0;
		allowTxt = 0;
		waitForEOT = 0;
		streamPos = 0;
		numCnt = 0;
		streamInESC27[0] = 27;
		streamInESC27[1] = 0;		
		return 0;
	
	}
	else if(c == -1){
		// TimeOut of a broken, or valid but unknown sequence
		r = -3;
		goto SaveGetESC27ErrReturn;
	}
	
	else if (!isValid){
		// Something not related to ESC 
		return -1;
	}
*/
	streamPos++;
	
	if (streamPos < ESC27_STREAM_IN_SIZE - 1)
	{
		gStreamInESC27[streamPos] = c;
		gStreamInESC27[streamPos + 1] = 0;
	}
	else{
		// Overflow
		r = -4;
		goto SaveGetESC27ErrReturn;
	}

	if (allowTxt){
		if (waitForEOT){
			if (isOSC){
				if (c == 92){
					// EndOfText
					switch (gStreamInESC27[2]){
					case 76:
						// Window title
						r = 110;
						break;
					case 108:
						// Icon label
						r = 111;
					default:
						break;
					}
					goto SaveGetESC27ErrReturn;
				}
				// EOT Error
				r = -5;
				goto SaveGetESC27ErrReturn;
			}			
		}
		return 0;
	}
	
	if (!isMouse){
		// Fixed Length Sequences
		switch (streamPos){
		case 1:
			if (c > 96 && c < 123){
				// Alt + a-z
				r = c - 17;
			}
			else if (c > 64 && c < 79 || c > 79 && c < 91){
				// ShiftAlt + A-Z
				// Except ShiftAlt + O, cause it's occupied by F1-F4
				r = c + 64;
			}
			else{
				// Waiting for more...
				r = 0;
			}
			
		case 2:
			if (gStreamInESC27[1] == 79){
				if (c > 79 && c < 84){
					// F1 - F4
					r = c - 79;
				}
				else if (c == 49){
					// Messed Up CSI in LX-Terminal for Shift/Ctrl F1-F4
					isCSI = 1;
				}
			}
			else if (gStreamInESC27[1] == 91){
				if (c > 64 && c < 73 && c != 71){
					// Up / Down / Right / Left / Center / End / UNKNOWN / Pos1
					r = c - 22;
				}
				else if (c == 90){
					// Shift-TAB
					r = 114;
				}
				else if (c == 60){
					// Mouse Trapping Start
					isMouse = 1;
					return 0;
				}
				else if (c == 77){
					// Byte-Mouse Trapping Start
					isByteMouse = 1;
					return 0;
				}
				else if (c == 73){
					// GotFocus
					r = 158;
				}
				else if (c == 79){
					// LostFocus
					r = 159;
				}
				isCSI = 1;
			}
			else if (gStreamInESC27[1] == 93){
				if (c == 76 || c == 108){
					// Icon label & Window title
					allowTxt = 1;
					r = 0;
				}			
				isOSC = 1;
			}
			else{
				// Illegal / Unknown char on streamInESC27[1]
				isValid = 0;
			}
			break;

		case 3:
			if (c == 126){
				if (gStreamInESC27[2] > 49 && gStreamInESC27[2] < 55){
					// Ins / Del / PgUp / PgDown
					r = gStreamInESC27[2] + 1;
				}
			}
			break;
		
		case 4:
			if (c == 126){
				if (gStreamInESC27[3] < 53){
					// F9 / F10 / F11 / F12
					r = gStreamInESC27[3] - 38;
				}
				else if (gStreamInESC27[3] < 58){
					// F5 / F6 / F7  F8
					r = gStreamInESC27[3] - 48;
				}
			}
			break;

		case 5:
			if (isByteMouse){
				// Mouse in ByteMode...
				// ByteMode is dangerous! XY-Positions > 223 may crash the Terminal ! 
				// But ByteMode has just ~40% of the data volume....
				gMousePosX = 0; gMousePosY = 0;
				if (gStreamInESC27[4]>32){
					gMousePosX = gStreamInESC27[4] - 32;
				}
				else{
					// Mouse-X Out Of Range
					r = 156;
					goto SaveGetESC27ErrReturn;
				}
				
				if (gStreamInESC27[5]>32){
					gMousePosY = gStreamInESC27[5] - 32;
				}
				else{
					// Mouse-Y Out Of Range
					r = 156;
					goto SaveGetESC27ErrReturn;
				}
				
				r = gStreamInESC27[3];
				// Switch off Shift / Alt / Ctrl
				r &= ~((1 << 2) | (1 << 3) | (1 << 4));

				switch (r){
				case 32:
					// LeftDown
					r = 116;
					gMouseButton = 1;
					gMouseSelX = gMousePosX; gMouseSelY = gMousePosY;
					break;
				case 34:
					// RightDown
					r = 126;
					gMouseButton = 4;
					gMouseSelX = gMousePosX; gMouseSelY = gMousePosY;
					break;
				case 35:
					// Mouse Up (Wheel / Right / Left)
					r = 117;
					break;
				case 33:
					// WheelDown
					r = 118;
					gMouseButton = 2;
					gMouseSelX = gMousePosX; gMouseSelY = gMousePosY;
					break;
				case 67:
					// MouseMove - (All Keys Up)
					r = 120;
					break;
				case 64:
					// MouseMove LeftDown
					r = 121;
					break;
				case 66:
					// MouseMove RightDown
					r = 127;
					break;
				case 65:
					// MouseMove WheelDown
					r = 122;
					break;
				case 96:
					// ScrollWheelUp
					r = 123;
					break;
				case 97:
					// ScrollWheelDown
					r = 124;
					break; 
				default:
					// Unknown Mouse
					r = 125;
				}
				isByteMouse = 0;
			}
			else if (c > 79 && c < 84 && gStreamInESC27[2] == 49){
				// Shift OR Alt OR Ctrl + F1 - F4
				// F3 could be CurserPos, too... WTF !
				// 	CSI 1;2 R  -  Shift
				// 	CSI 1;3 R  -  Alt
				// 	CSI 1;4 R  -  ShiftAlt
				// 	CSI 1;5 R  -  Ctrl
				// 	CSI 1;6 R  -  ShiftCtrl
				// 	CSI 1;7 R  -  AltCtrl
				// 	CSI 1;8 R  -  ShiftAltCtrl
				// Previous 7 Combinations Are CursorPositions, too... WTF !
				if (!(c == 82 && gScreenSizeInCursorPos || gCursorWaitFor)){
					// Not F3 while waiting for CursorPos
					switch (gStreamInESC27[4]){
					case 50:
						// Shift
						r = c - 65;
						break;
					case 51:
						// Alt
						r = c + 80;
						break;
					case 53:
						//Ctrl
						r = c - 51;
						break;
					case 54:
						// Shift+Ctrl
						r = c + 122;
						break;
					case 52:
						// Shift+Alt
						r = c + 94;
						break;
					case 55:
						// Alt+Ctrl
						r = c + 108;
						break;
					case 56:
						// Shift+Alt+Ctrl
						r = c + 136;
						break;
					default:
						break;
					}				
				}
				
			}
			else if (c > 64 && c < 73 && c != 71){
				// Shift OR Ctrl OR Alt + Up / Down / Right / Left / Center / End / UNKNOWN / Pos1
				r = c - 9;
				if (gStreamInESC27[4] == 53){
					// Ctrl
					r+= 8;
				}
				if (gStreamInESC27[4] == 51){
					// Alt
					r+= 16;
				}
			}		
			break;

		case 6:
			if (c == 126){
				// Shift OR Alt OR Ctrl + F5 - F12
				switch (gStreamInESC27[5])				{
				case 50:
					/* shift */
					r = gStreamInESC27[3] - 24;
					break;
				case 51:
					/* alt */
					r = gStreamInESC27[3] + 121; 
					break;
				case 53:
					/* ctrl */
					r = gStreamInESC27[3] - 10;
					break;
				case 54:
					// Shift+Ctrl
					r = gStreamInESC27[3] + 163; 
					break;
				case 52:
					// Shift+Alt
					r = gStreamInESC27[3] + 135; 
					break;
				case 55:
					// Alt+Ctrl
					r = gStreamInESC27[3] + 149; 
					break;
				case 56:
					// Shift+Alt+Ctrl
					r = gStreamInESC27[3] + 177; 
					break;
				default:
					break;
				}

				if (gStreamInESC27[3] > 52){
					// F5 - F8
					r-= 10;
				}
			}
			break;		
		};
	}
	
	if (isByteMouse){
		return 0;
	}
	
	if (r){
		// Unknown OR valid OR flexible Sequence found
		isValid = 0;
		if (r == -1 && isCSI){
			if (c > 46 && c < 60){
				// All numbers 0-9 and ; and : are valid (values and separators)
				if (!numCnt){
					// starts with separator and 1st Number was 0... (cause missing)
					// OR
					// it's starting with a digit...					
					numCnt = 1;
					isNumGroup[1] = 0;
					pNumPos[1] = &(gStreamInESC27[streamPos]);
				}
				if (c > 57){
					// colon & semi-colon
					numCnt++;
					pNumPos[numCnt] = &(gStreamInESC27[streamPos + 1]);
					isNumGroup[numCnt] = 0;
					if (c == 58){
						// previous and next number are a colon separated group
						isNumGroup[numCnt - 1] = 1;
					}
				}
				r = 0;
				isValid = 1;
			}
			else{
				// Any known terminator of a flexible length sequence ?
				switch (c){
				case 82:
					// Actual Cursor Position
					if (gScreenSizeInCursorPos){
						// but as TerminalSize substitute
						gScreenWidth = atoi(pNumPos[2]);
						gScreenHeight = atoi(pNumPos[1]);
						gScreenSizeInCursorPos = 0;
						r = 109;
					}
					else{
						gCursorPosY = atoi(pNumPos[1]);
						gCursorPosX = atoi(pNumPos[2]);
						gCursorWaitFor = 0;
						r = 107;
					}
					break;				
				case 77:
					// Mouse Move  /  Mouse Down / Wheel Down
				case 109:
					// Mouse Up / Wheel Up
					if  (isMouse){
						gMousePosX = atoi(pNumPos[2]);
						gMousePosY = atoi(pNumPos[3]);
						r = (atoi(pNumPos[1]));
						// Switch off Shift / Alt / Ctrl
						r &= ~((1 << 2) | (1 << 3) | (1 << 4));
						
						if (c == 77){
							// Mouse Move  /  Mouse Down / Wheel Down
							switch(r){
							case 35:
								// Move
								r = 120;
								break;
							case 32:
								// Move Button Pressed
								r = 121;
								break;
							case 34:
								// Move Right Button Pressed
								r = 127;
								break;
							case 64:
								// Wheel Scroll Up
								r = 123;
								break;
							case 65:
								// Wheel Scroll Down
								r = 124;
								break;
							case 0:
								// Button Down
								gMouseButton = 1;
								gMouseSelX = gMousePosX; gMouseSelY = gMousePosY;
								r = 116;
								break;
							case 2:
								// Right Button Down
								gMouseButton = 4;
								gMouseSelX = gMousePosX; gMouseSelY = gMousePosY;
								r = 126;
								break;
							case 1:
								// Wheel Down
								gMouseButton = 2;
								gMouseSelX = gMousePosX; gMouseSelY = gMousePosY;
								r = 118;
								break;
							case 33:
								// Move Wheel Pressed
								r = 122;
								break;
							default:
								// UMO - Unknown Mouse Object
								r = 125;
								break;
							}
						}
						else{
							// Mouse Up / Wheel Up
							if (r < 3){
								// Mouse Up (Wheel / Right / Left)
								r = 117;
							}
							else{
								// UMO - Unknown Mouse Object
								r = 125;
							}
						}
					}
					break;
				case 116:
					// Terminal Infos
                    switch (atoi(pNumPos[1])){
                    case 8:
                        // ScreenSize
                        gScreenHeight = atoi(pNumPos[2]);
                        gScreenWidth = atoi(pNumPos[3]);
                        r = 109;
                        break;
                    
                    default:
                        // Unknown terminal sequence
						r = 112;
                    }
					break;
				default:
					// Unknown or broken sequence
					r = -2;
					break;
				}
			}
		}	
	}
	return r;

	SaveGetESC27ErrReturn: 
	isValid = 0; allowTxt = 0; isMouse = 0;	isByteMouse = 0;gScreenSizeInCursorPos = 0; gCursorWaitFor = 0;
	return r;

}

/**
 * @brief Send command to Terminal and wait for an answer
 * 
 * @param	*pStrExchange (Command)
 * 
 * @param	waitForID	0 = accept all valid answers
 * 						n = accept just answer n
 * 
 * @param	timeOut 	(float in seconds)
 * 						wait max this time for an answer
 * 
 * @private	r (int)		helper for return
 * 
 * @private i (int)		helper for InKey()
 * 
 * @private gotChar		if we received something (TimeOut-Helper)
 * 			(int / bool)
 * 
 * @private timeExit	helper for TimeOut
 * 			(clock_t)
 * 			
 * @return	int 		n = received sequence (see GetESC27-return)
						0 = TimeOut (without chars) or wrong answer
 */
int WaitForESC27(char *pStrExchange, int waitForID, float timeOut){

    int r = 0;
    int i = 0;
	int gotChar = 0;
	clock_t timeExit;

	fflush(stdin);
	FlushInKey();
    printf("%s",pStrExchange);
	fflush(stdout);

	timeExit = clock() + (long)(timeOut * gClocksPerSecond);

    while (clock() < timeExit){
		
		i = InKey();

        if (i > 0){
			gotChar = 1;
            r = GetESC27(i);
            if (r > 0){
				if ((r == waitForID) || (waitForID == 0)){
					// Valid
					FlushInKey();				
					return r;
				}
            }
			else if (r < 0){
				// Error
                FlushInKey();
				return r;
			}			
        }
    }

    // TimeOut
	FlushInKey();
	if (gotChar){
        // With char(s) received
        return -3;
    }
    else{
        // Without char(s) received
       	return 0;
    }
}

/**
 * @brief Signal-Handler for signal()
 * 
 * @param sig (int)
 */
void SignalHandler(int sig){
	if (SIGINT == sig){
		// Ctrl-C pressed
		gSignalCtrlC = 1;
		#if __WIN32__ || _MSC_VER || __WIN64__
			// Catch Ctrl-C again...
			signal(SIGINT, SignalHandler);
		#endif
	}
	#if __WIN32__ || _MSC_VER || __WIN64__
	#else	
		else if (SIGWINCH == sig){
			// Terminal-Size Changed
			gSignalTerminalSize = 1;
		}
		else if (SIGALRM == sig){
			// 31/3 Interval
			gSignalInterval = 1;
		}
	#endif
	/*else if (SIGURG == sig){
		// IO - Ready
		printf("Yeah IO-READY");
	}*/
}

/**
 * @brief Enable / Disable Mouse Trapping
 * 
 * @param set (int / bool)	1 = Enable
 * 							0 = Disable
 * 
 * @private c (char)		helper for set
 */
void TrapMouse(int set){

	char c = 'l';
	if (set){
		c = 'h';
	}
	// Any Event (1003) / Decimal Values (1006)
	// 1002 instead of 1003 reports position only if Mouse Button is pressed
	printf("\x1B[?1002%c\x1B[?1006%c", c, c);
}

/**
 * @brief Enable / Disable TerminalFocus Trapping
 * 
 * @param set (int / bool)	1 = Enable
 * 							0 = Disable
 * 
 * @private c (char)		helper for set
 */
void TrapFocus(int set){

	char c = 'l';
	if (set){
		c = 'h';
	}
	printf("\x1B[?1004%c", c);
}

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
	<Terminal.h>	    	01.04.2022		                                Pit

BugID		Description									Found by / Date		Fixed by / Date

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		Bytestream Structure to identify ESC27 Sequences- see: int GetESC27(int)
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
								-------------------------
ID	shift/ctrl	key				0	1	2	3	4	5	6

1				F1				27	79	(80)
2				F2				27	79	(81)
3				F3				27	79	(82)
4				F4				27	79	(83)

5				F5				27	91	49	(53)126

7				F6				27	91	49	(55)126
8				F7				27	91	49	(56)126
9				F8				27	91	49	(57)126

10				F9				27	91	50	(48)126
11				F10				27	91	50	(49)126

13				F11				27	91	50	(51)126
14				F12				27	91	50	(52)126


15		shift	F1				27	91	49	59	50	(80)
16		shift	F2				27	91	49	59	50	(81)
17		shift	F3				27	91	49	59	50	(82)
18		shift	F4				27	91	49	59	50	(83)

19		shift	F5				27	91	49	(53)59	50	126

21		shift	F6				27	91	49	(55)59	50	126
22		shift	F7				27	91	49	(56)59	50	126
23		shift	F8				27	91	49	(57)59	50	126

24		shift	F9				27	91	50	(48)59	50	126
25		shift	F10				27	91	50	(49)59	50	126

27		shift	F11				27	91	50	(51)59	50	126
28		shift	F12				27	91	50	(52)59	50	126


29		ctrl	F1				27	91	49	59	53	(80)
30		ctrl	F2				27	91	49	59	53	(81)
31		ctrl	F3				27	91	49	59	53	(82)
32		ctrl	F4				27	91	49	59	53	(83)

33		ctrl	F5				27	91	49	(53)59	53	126

35		ctrl	F6				27	91	49	(55)59	53	126
36		ctrl	F7				27	91	49	(56)59	53	126
37		ctrl	F8				27	91	49	(57)59	53	126

38		ctrl	F9				27	91	50	(48)59	53	126
39		ctrl	F10				27	91	50	(49)59	53	126

41		ctrl	F11				27	91	50	(51)59	53	126
42		ctrl	F12				27	91	50	(52)59	53	126


43				UP				27	91	(65)
44				DOWN			27	91	(66)
45				Right			27	91	(67)
46				Left			27	91	(68)
47				Center			27	91	(69)
48				END				27	91	(70)

50				Pos1			27	91	(72)

51				Ins				27	91	(50)126
52				Del				27	91	(51)126

54				PgUp			27	91	(53)126
55				PgDown			27	91	(54)126

56		shift	UP				27	91	49	59	50	(65)
57		shift	DOWN			27	91	49	59	50	(66)
58		shift	RIGHT			27	91	49	59	50	(67)
59		shift	LEFT			27	91	49	59	50	(68)

61		shift	END				27	91	49	59	50	(70)
63		shift	Pos1			27	91	49	59	50	(72)
				
64		ctrl	UP				27	91	49	59	53	(65)
65		ctrl	DOWN			27	91	49	59	53	(66)
66		ctrl	RIGHT			27	91	49	59	53	(67)
67		ctrl	LEFT			27	91	49	59	53	(68)
68		ctrl	Center			27	91	49	59	53	(69)
69		ctrl	END				27	91	49	59	53	(70)

71		ctrl	Pos1			27	91	49	59	53	(72)		

72		alt		UP				27	91	49	59	50	(65)
73		alt		DOWN			27	91	49	59	50	(66)
74		alt		RIGHT			27	91	49	59	50	(67)
75		alt		LEFT			27	91	49	59	50	(68)
76		alt		Center			27	91	49	59	50	(69)
77		alt		END				27	91	49	59	50	(70)

79		alt		Pos1			27	91	49	59	50	(72)
		
80-105	alt		A-Z				27	97-122

106				Back			127
		
107				Cursor			27	91	Yrows	59	Xcolumns	82

108				UsrESC			27	(with an timeout - function in progress...)

109				TermSize		27	91	Yrows	59	Xcolumns	116
110				TermIconLabel	27	93	76	ABCtxt	27	92
111				TermWinName		27	93	108	ABCtxt	27	92
112				TermERR			27	91	unknown

113				TAB				9
114		shift	TAB				27	91	90

115				ENTER			10(13)

116				MouseDown		27	91	60	(id-0)	59	Xcolumn	59	Yrow	77
117				MouseUp			27	91	60	(id-0)	59	Xcolumn	59	Yrow	109
118				WheelDown		27	91	60	(id-1)	59	Xcolumn	59	Yrow	77
119				WheelUp			27	91	60	(id-1)	59	Xcolumn	59	Yrow	109
120				MouseMove		27	91	60	(id-35)	59	Xcolumn	59	Yrow	77
121				MouseDownMove	27	91	60	(id-32)	59	Xcolumn	59	Yrow	77
122				WheelDownMove	27	91	60	(id-33)	59	Xcolumn	59	Yrow	77
123				ScrollUp		27	91	60	(id-64)	59	Xcolumn	59	Yrow	77
124				ScrollDown		27	91	60	(id-65)	59	Xcolumn	59	Yrow	77
125				UMO				27	91	60	Unknown Mouse Object	77 / 109

Mouse in ByteMode (e.g. LX-Terminal)
116				MouseDown		27	91	77	32/34	Xcolumn		Yrow
117				MouseUp			27	91	77	35		Xcolumn		Yrow
118				WheelDown		27	91	77	33		Xcolumn		Yrow
119		NA 		Cause is same with MouseUp
120				MouseMove		27	91	77	67		Xcolumn		Yrow
121				MouseDownMove	27	91	77	64/66	Xcolumn		Yrow
122				WheelDownMove	27	91	77	65		Xcolumn		Yrow
123				ScrollUp		27	91	77	96		Xcolumn		Yrow
124				ScrollDown		27	91	77	97		Xcolumn		Yrow
125				UMO				27	91	77	UMO		Xcolumn		Yrow


Added Right Mouse
126				RMouseDown		27	91	77	34		Xcolumn		Yrow
127				RMouseDownMove	27	91	77	66		Xcolumn		Yrow
128				RMouseUp		27	91	77	66		Xcolumn		Yrow

Added ShiftAlt A-Z
129-155	ShiftAlt	a-z			27	65-90

++++++++Bytestream Structure to identify ESC27 sequences - see: int GetESC27(int)+++++++

*/
