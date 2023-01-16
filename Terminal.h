
//		       Terminal Functions - See detailed description at the EOF !
//				! Using 3rd party MIT/GPL/"FreeSoftware" technologies !
//				  (see related declarations/functions and at the EOF)
// This is a  ! p_o_o_r !  Header by (c) Pit Demmer. (PIT-Licensed 01.04.2022 -           )

#define TerminalVersion "1.00pa"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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


int TERM_RunCoreLoop = 1;

// Dummy_Events to cover events the user hasn't defined
static void TermDummyEvent(){
	volatile static int i;
}
static void TermDummyEventChar(char c){
    volatile static int i;
}
static void TermDummyEventInt(int i){
    volatile static int a;
}
static void TermDummyEventIntInt(int x, int y){
    volatile static int a;
}
static void TermDummyEventIntIntInt(int x, int y, int button){
    volatile static int a;
}
static void TermDummyEventIntIntIntIntInt(int x1, int y1, int x2, int y2, int button){
    volatile static int a;
}
static void TermDummyEventStr(char *strIN){
	volatile static int i;
}

// Real Events
void (*TermGotChar)(char c) = TermDummyEventChar;

#define TERM_Event_NavUp 0
#define TERM_Event_NavDown 1
#define TERM_Event_NavRight 2
#define TERM_Event_NavLeft 3
#define TERM_Event_NavCenter 4
#define TERM_Event_NavEnd 5
#define TERM_Event_NavUnknown 6
#define TERM_Event_NavPos1 7
#define TERM_Event_NavIns 8
#define TERM_Event_NavDel 9
#define TERM_Event_NavPgUp 10
#define TERM_Event_NavPgDown 11
#define TERM_Event_NavBack 12
#define TERM_Event_NavTab 13
void (*TermNavPress[14])(int shiftAltCtrl) = {TermDummyEventInt};

#define TERM_Event_F1 0
#define TERM_Event_F2 1
#define TERM_Event_F3 2
#define TERM_Event_F4 3
#define TERM_Event_F5 4
#define TERM_Event_F6 5
#define TERM_Event_F7 6
#define TERM_Event_F8 7
#define TERM_Event_F9 8
#define TERM_Event_F10 9
#define TERM_Event_F11 10
#define TERM_Event_F12 11
void (*TermFkeyPress[12])(int shiftAltCtrl) = {TermDummyEventInt};

#define TERM_Event_A 0
#define TERM_Event_B 1
#define TERM_Event_C 2
#define TERM_Event_D 3
#define TERM_Event_E 4
#define TERM_Event_F 5
#define TERM_Event_G 6
#define TERM_Event_H 7
#define TERM_Event_I 8
#define TERM_Event_J 9
#define TERM_Event_K 10
#define TERM_Event_L 11
#define TERM_Event_M 12
#define TERM_Event_N 13
#define TERM_Event_O 14
#define TERM_Event_P 15
#define TERM_Event_Q 16
#define TERM_Event_R 17
#define TERM_Event_S 18
#define TERM_Event_T 19
#define TERM_Event_U 20
#define TERM_Event_V 21
#define TERM_Event_W 22
#define TERM_Event_X 23
#define TERM_Event_Y 24
#define TERM_Event_Z 25
void (*TermAltPress[26])(int shift) = {TermDummyEventInt};

void (*TermEscPress)() = TermDummyEvent;

#define TERM_Event_GotFocus 0
#define TERM_Event_LostFocus 1
void (*TermFocus[2])() = {TermDummyEvent, TermDummyEvent};

void (*TermCtrlPress[26])() = {TermDummyEvent};

#define TERM_Event_RawMouseLeftDown 0
#define TERM_Event_RawMouseWheelDown 1
#define TERM_Event_RawMouseRightDown 2
#define TERM_Event_RawMouseUp 3
#define TERM_Event_RawMouseMove 4
#define TERM_Event_RawMouseMoveLeft 5
#define TERM_Event_RawMouseMoveWheel 6
#define TERM_Event_RawMouseMoveRight 7
#define TERM_Event_RawMouseScrollUp 8
#define TERM_Event_RawMouseScrollDown 9
#define TERM_Event_RawMouseUnknown 10
void (*TermRawMouse[11])(int x, int y) = {TermDummyEventIntInt};

#define TERM_Event_MouseClick 0
#define TERM_Event_MouseDblClick 1
void (*TermMouseClicks[2])(int x, int y, int button) = {TermDummyEventIntIntInt, TermDummyEventIntIntInt};

void (*TermMouseArea)(int x1, int y1, int x2, int y2, int button) = TermDummyEventIntIntIntIntInt;

void(*TermSizeChanged)(int x, int y) = TermDummyEventIntInt;

#define TERM_Event_InfoName 0
#define TERM_Event_InfoIcon 1
#define TERM_Event_InfoUnknown 2
void(*TermInfo[3])(char *strIN) = {TermDummyEventStr, TermDummyEventStr, TermDummyEventStr};



#define ESC27_EXCHANGE_SIZE 33          // Has to be at least 1 greater than greatest to expect Command/Response !!
#define ESC27_STREAM_IN_SIZE 33         // Has to be >= ESC27_EXCHANGE_SIZE !!
//unsigned char streamInESC27[ESC27_STREAM_IN_SIZE];
char gStreamInESC27[ESC27_STREAM_IN_SIZE];

// Screen
int TERM_ScreenWidth = 0;
int TERM_ScreenHeight = 0;
int TERM_ScreenWidthPrev = 0;
int TERM_ScreenHeightPrev = 0;
int TERM_ScreenSizeInCursorPos = 0;

// Keyboard
int TERM_CursorPosX = 0;
int TERM_CursorPosY = 0;
int TERM_CursorSelX = 0;                     // 1st point of selection rectangle
int TERM_CursorSelY = 0;                     // or same as CursorPosX&Y = No selection
int TERM_CursorWaitFor = 0;					// Shift OR Alt OR Ctrl + F3 
										// Has 7 overlapping Settings with CursorPos
										// Set cursorIsAsked true when asking 'manually' for the cursor position !!
int TERM_CursorIsSelecting = 0;             // (Moving with Shift)

int TERM_KeyShift = 0;
int TERM_KeyAlt = 0;
int TERM_KeyCtrl = 0;
int TERM_KeyMeta = 0;

// Mouse
int TERM_MousePosX = 0;
int TERM_MousePosY = 0;
int TERM_MouseSelX = 0;                     // 1st point of selection rectangle
int TERM_MouseSelY = 0;                     // or same as MousePosX&Y = No selection
int TERM_MouseButton = 0;				   // 1 = left, 2 = wheel, 4 = right

// ESC27 Reading
_Bool TermIsWaitingForESC27 = 0;

// Signals
int TERM_SignalCtrlC = 0;
int TERM_SignalTerminalSize = 0;
int TERM_SignalInterval = 0;

/**
 * @brief Declaration FUNCTIONS
 */
int TermSetVT(int set);
int TermInKey(void);
void TermFlushInKey(void);
int TermClearScreen(int set);
int TermGetSize(int set);
int TermSizeIsChanged(void);
int TermGetESC27 (int c);
static int GetESC27_CheckOnF112Key(int r, int posInStream);
static int GetESC27_CheckOnF512(void);
int TermWaitForESC27(char *pStrExchange, int waitForID, float timeOut);
void TermSignalHandler(int sig);
void TermTrapMouse(int set);
void TermTrapFocus(int set);

void TermInitEvents(){
	int i = 0;

	for (i = 0; i < 14; i++) {
		TermNavPress[i] = TermDummyEventInt;
	}
	for (i = 0; i < 12; i++) {
		TermFkeyPress[i] = TermDummyEventInt;
	}
	for (i = 0; i < 26; i++) {
		TermAltPress[i] = TermDummyEventInt;
		TermCtrlPress[i] = TermDummyEvent;
	}
	for (i = 0; i < 11; i++) {
		TermRawMouse[i] = TermDummyEventIntInt;
	}
}

int TermInit(){

	int r = 0;

	printf("\nHello New Project\n\n");

	printf("Initializing Timing... ");
	TimeInitTime();
	printf("OK\n");
	printf("%s - %s\n\n", gStrDate, gStrTime);

	printf("Try To Enable Video Terminal Mode... ");
	if (!TermSetVT(1)){
		printf("ERROR!\n");
		printf("STOP PROJECT\n");
		return 0;
	}
	printf("OK\n\n");

	printf("Try To Clear Screen...\n");
	r = TermClearScreen(0);
	if (!r){
		// Can't happen....
		printf("Clear Screen... ERROR!\n");
		printf("STOP PROJECT\n");
		return 0;
	}
	printf("Clear Screen... OK, CLS-Mode: %d\n\n",r);

	printf("Try To Fetch Terminal Size... ");
	r = TermGetSize(0);
	if (!r){
		printf("ERROR!\n");
		printf("STOP PROJECT\n");
		return 0;
	}
	printf("OK, Size-Mode: %d\n",r);
	#if __WIN32__ || _MSC_VER || __WIN64__
		printf("BillyOS: Screen Size Changes Will Be Polled...Sorry.\n");
	#else
		signal(SIGWINCH, TermSignalHandler);
		printf("PosiX-OS: Screen Size Changes Getting Signaled.\n");
	#endif
	printf("Width: %d  Height: %d\n\n", TERM_ScreenWidth, TERM_ScreenHeight);

	printf("Synchronize CLS-Mode With Size-Mode...\n");
	r = TermClearScreen(r);
	printf("Synchronized CLS-Mode With Size-Mode... OK, Mode: %d\n\n",r);
	
	printf("Enable Trap Mouse Mode... ");
	TermTrapMouse(1);
	printf("(probably) OK\n\n");

	printf("Enable Trap Focus Change... ");
	TermTrapFocus(1);
	printf("(probably) OK\n\n");

	printf("Catch Ctrl-C... ");
	signal(SIGINT, TermSignalHandler);
	printf("(probably) OK\n\n");

	#if __WIN32__ || _MSC_VER || __WIN64__
		printf("BillyOS: Check On Real-Time Changes Get Polled...\n");
		printf("         The whole timer stuff is kind a fixed... Sorry... not.\n\n");
	#else
		signal(SIGALRM, TermSignalHandler);
		
		// 333333 == we'll hit THE second in between +0,0 to +0,65 sec. after RealTime...
		// but (little depending on Time_EventsTime) from one SecondEvent to the next SecondEvent, we're within a mS tolerance!
		// lower value to raise precision
		ualarm(333333,333333);
		printf("PosiX-OS: Check On Real-Time Changes Get Signaled.\n");

		// Lower = more "UserLoops" vs. Higher = less CPU-time...
		TIME_EventsTime = 10000;
		printf("PosiX-OS: TIME_DoEventsTime is set.\n\n");

	#endif

	TermInitEvents();

	return 1;

}
int TermExit(){

	TermClearScreen(0);
	printf("Disable Trap Mouse Mode... ");
	TermTrapMouse(0);
	printf("OK\n\n");

	printf("Disable Trap Focus Change... ");
	TermTrapFocus(0);
	printf("OK\n\n");

	printf("Try To Set Back Terminal From Video Mode... ");
	if (!TermSetVT(0)){
		printf("ERROR!\n");
		return 0;
	}
	printf("OK\n\n");

	printf("%s - %s\n", gStrDate, gStrTime);
	printf("Runtime: %s\n\n", gStrRunTime);

	return 1;

}

int TermSetVT(int set){
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

int TermInKey(void){
/**
 * @brief	Non-Blocking GetChar
 * 
 * @return 	int 
 * 			1-255 	= char 
 * 			0		= there was no char in buffer
 */

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

void TermFlushInKey(void){
/**
 * @brief 	Flush buffer the "hard (and all time successful) way"
 */
	while (TermInKey()){
		// Flush
	}
}

/**
 * @brief 	DoEvents() - release CPU during idle times
 */
#if __WIN32__ || _MSC_VER || __WIN64__
	#define DoEvents() Sleep(1);
#else
	#define DoEvents() usleep(TIME_EventsTime);
#endif 

int TermClearScreen(int set){
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

int TermGetSize(int set){
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
		TERM_ScreenSizeInCursorPos = 1;
		printf("\0337\x1B[999;9999H\x1B[6n\0338");
		break;
	case 0:
		// 1st run
		r = TermWaitForESC27("\x1B[18t",177,0.5);
		if (TERM_ScreenWidth > 0 && TERM_ScreenHeight > 0){
			isSet = 1;
			TERM_ScreenWidthPrev = TERM_ScreenWidth;
			TERM_ScreenHeightPrev = TERM_ScreenHeight;
			break;
		}
		TERM_ScreenSizeInCursorPos = 1;
		r = TermWaitForESC27("\0337\x1B[999;9999H\x1B[6n\0338",180,0.5);
		TERM_ScreenSizeInCursorPos = 0;
		if (TERM_ScreenWidth > 0 && TERM_ScreenHeight > 0){
			isSet = 2;
			TERM_ScreenWidthPrev = TERM_ScreenWidth;
			TERM_ScreenHeightPrev = TERM_ScreenHeight;
			break;
		}
	case 3:
		// System
        #if __WIN32__ || _MSC_VER || __WIN64__
	        /* Billy OS */
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
            TERM_ScreenWidth = (int)(csbi.srWindow.Right - csbi.srWindow.Left + 1);
            TERM_ScreenHeight = (int)(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
        #else 
			/* Mac & Linux */
            ioctl(fileno(stdout), TIOCGWINSZ, &w);
            TERM_ScreenWidth = (int)(w.ws_col);
            TERM_ScreenHeight = (int)(w.ws_row);
        #endif
		if (!isSet){
			if (TERM_ScreenWidth > 0 && TERM_ScreenHeight > 0){
				isSet = 3;
				TERM_ScreenWidthPrev = TERM_ScreenWidth;
				TERM_ScreenHeightPrev = TERM_ScreenHeight;
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

int TermSizeIsChanged(void){
/**
 * @brief 	Check On If ScreenSize Has Changed
 * 
 * @return	int	1 = Changed
 * 				0 = Not Changed
 */

	if ((TERM_ScreenWidth != TERM_ScreenWidthPrev) || (TERM_ScreenHeight != TERM_ScreenHeightPrev)){
		TERM_ScreenHeightPrev = TERM_ScreenHeight;
		TERM_ScreenWidthPrev = TERM_ScreenWidth;
		return 1;
	}
	return 0;
}

int TermGetESC27 (int c){

/**
 * @brief 	Analyze stream (char by char) on ESC-Sequences
 * 
 * @param 	c 1-255 = (next) char
 * 				-1	= CoreLoop recognized a TimeOut (reset statics/function)
 * 				-2	= CoreLoop recognized ShiftAlt-O (conflicting with F1-F4)
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
 * @return	int	-2	= just the Termination/Identification Char was unknown.
				-1	= Unknown (almost regular Keyboard)
				-4	= overflow - too long
				-5	= unexpected EOT
				-3	= timeOut of a broken, or valid but unknown sequence
				-6	= Overlapping Shift-Alt-O
				-7	= ByteMouse Out Of Range (!! Better don't do ByteMouseMode !!)
				-8	= Valid but unknown text-sequence
				-9	= Useless ESC - unknown reason...
				-27	= 1st (Non-User)ESC (just internal - real return will be 0)
				 0	= Valid, but waiting for more Bytes to identify/finish sequence
				 n	= The ESC-Sequence (see related enum's)
 */

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

	int r = 0;		

	if (c == 127 && streamPos < 1){
		// User Back
		r = c;
		if (isValid){
			// Alt-Back
			TERM_KeyAlt = 1;
		}
	}
	else if (c < 32){
		r = c;
		switch (c){
		case 9:
			// TAB
		case 10:
			// LF
		case 13:
			// CR
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
				}
				else{
					// looks like UserESC - but is BS (e.g. Overflow Mouse in ByteMode)
					r = -9;
				}
				break;
			}
			
			// Valid Start
			streamPos = -1;
			r = -27;
			break;
		case -1:
			// TimeOut of a broken, or valid but unknown sequence
			r = -3;
			break;
		case -2:
			// ShiftAlt-O (conflicting with F1-F4) recognized
			TERM_KeyAlt = 1; TERM_KeyShift = 1;
			r = 79;
			break;
		default:
			// Ctrl-A - Ctrl-Z
			// (but a lot are special - see cases above
			//  a lot are also not supported on all OSs)
			break;
		}
	}
	else {
		if (!isValid){
			// Something not related to ESC (all regular User Pressed Stuff...)
			return -1;
		}
	}

	streamPos++;
	
	if (streamPos < ESC27_STREAM_IN_SIZE - 1){
		if (!r || r == -27){
			gStreamInESC27[streamPos] = c;
			gStreamInESC27[streamPos + 1] = 0;
		}
		else{
			streamPos--;
		}
	}
	else{
		// Overflow
		r = -4;
	}

	if (!r){
		if (allowTxt){
			if (waitForEOT){
				if (isOSC){
					r = -5;
					if (c == 92){
						// EndOfText
						switch (gStreamInESC27[2]){
						case 76:
							// Window title
							r = 178;
							break;
						case 108:
							// Icon label
							r = 179;
						default:
							// valid but unknown text
							r = 180;
						}
					}
				}			
			}
			else{
				return 0;
			}		
		}
		
		if (!isMouse){
			// Fixed Length Sequences
			switch (streamPos){
			case 1:
				if (c > 96 && c < 123){
					// Alt + a-z
					TERM_KeyAlt = 1;
					r = c - 32;
				}
				else if (c == 79){
					// (Shift)Alt + O
					// could expand to F1-F4
					r = -6;
				}
				else if (c > 64 && c < 91){
					// (Shift)Alt + A-Z
					// Except ShiftAlt + O, could expand to F1-F4
					TERM_KeyAlt = 1;
					TERM_KeyShift = 1;
					r = c;
				}
				break;
			case 2:
				if (gStreamInESC27[1] == 79){
					TERM_KeyAlt = 0; TERM_KeyShift = 0;		// Could be False-True cause of overlapping ShiftAlt-O and F1-F4
					if (c > 79 && c < 84){
						// F1 - F4
						r = c + 48;
					}
					else if (c == 49){
						// Messed Up CSI in LX-Terminal for Shift/Ctrl F1-F4
						isCSI = 1;
					}
				}
				else if (gStreamInESC27[1] == 91){
					isCSI = 1;
					if (c > 64 && c < 73){
						// Up / Down / Right / Left / Center / End / UNKNOWN / Pos1
						r = c + 79;
					}
					else{
						switch (c){
						case 90:
							// Shift-TAB
							TERM_KeyShift = 1;
							r = 9;
						case 60:
							// Mouse Trapping Start
							isMouse = 1;
							return 0;
						case 77:
							// Byte-Mouse Trapping Start
							isByteMouse = 1;
							return 0;
						case 73:
							// GotFocus
							r = 160;
							break;
						case 79:
							// LostFocus
							r = 161;
							break;
						}
					}
				}
				else if (gStreamInESC27[1] == 93){
					if (c == 76 || c == 108){
						// Icon label & Window title
						allowTxt = 1;
					}			
					isOSC = 1;
				}
				break;

			case 3:
				if (c == 126){
					r = gStreamInESC27[2];
					if (r > 49 && r < 55){
						// Ins / Del / PgUp / PgDown
						r += 102;
					}
				}
				break;
			
			case 4:
				if (c == 126){
					// F5 - F12
					r = GetESC27_CheckOnF512();
				}
				break;

			case 5:
				if (isByteMouse){
					// Mouse in ByteMode...
					// ByteMode is dangerous! XY-Positions > 223 may crash the Terminal ! 
					// But ByteMode has just ~40% of the data volume....
					TERM_MousePosX = 0; TERM_MousePosY = 0;
					if (gStreamInESC27[4]>32 && gStreamInESC27[5]>32){
						TERM_MousePosX = gStreamInESC27[4] - 32;
						TERM_MousePosY = gStreamInESC27[5] - 32;
					}
					else{
						// Mouse Out Of Range
						r = -7;
						break;
					}
									
					r = gStreamInESC27[3];
					// Switch off Shift / Alt / Ctrl
					r &= ~((1 << 2) | (1 << 3) | (1 << 4));

					switch (r){
					case 32:
						// LeftDown
					case 33:
						// WheelDown
					case 34:
						// RightDown
						TERM_MouseButton = 1 << (r - 32);
						TERM_MouseSelX = TERM_MousePosX; TERM_MouseSelY = TERM_MousePosY;
						r += 130;
						break;
					case 35:
						// Mouse Up (Wheel / Right / Left)
						r = 165;
						break;
					case 67:
						// MouseMove - (All Keys Up)
						r = 166;
						break;
					case 64:
						// MouseMove LeftDown
						r = 167;
						break;
					case 66:
						// MouseMove RightDown
						r = 169;
						break;
					case 65:
						// MouseMove WheelDown
						r = 168;
						break;
					case 96:
						// ScrollWheelUp
						r = 170;
						break;
					case 97:
						// ScrollWheelDown
						r = 171;
						break; 
					default:
						// Unknown Mouse
						r = 172;
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
					if (!(c == 82 && TERM_ScreenSizeInCursorPos || TERM_CursorWaitFor)){
						// Not F3 while waiting for CursorPos
						r = GetESC27_CheckOnF112Key(c + 48, 4);
					}
				}
				else if (c > 64 && c < 73){
					// Shift OR Ctrl OR Alt + Up / Down / Right / Left / Center / End / UNKNOWN / Pos1
					// !! Never seen a working combination of Alt+Shift or Alt+Ctrl or Shift+Ctrl...
					r = c + 79;
					switch(gStreamInESC27[4]){
					case 53:
						TERM_KeyCtrl = 1;
						break;
					case 51:
						TERM_KeyAlt = 1;
						break;
					default:
						TERM_KeyShift = 1;
					}
				}
				else if (c == 126){
					// Shift-DEL ([Ins]/PgUp/PgDown)
					r = gStreamInESC27[2];
					if (r > 49 && r < 55 && r != 52){
						// Del 
						// Ins - just WIN ?
						r += 102;
						if (gStreamInESC27[4] == 50){
							TERM_KeyShift = 1;
						}
						else{
							TERM_KeyCtrl = 1;
						}
					}
				}
				break;
			case 6:
				if(c == 126){
					r = GetESC27_CheckOnF512();
					r = GetESC27_CheckOnF112Key(r,5);
				}		
				break;		
			};
		}
		
		if (isByteMouse){
			// is fixed length und we're still waiting for (a) char(s)
			return 0;
		}
		
		if (!r){
			// Unknown OR valid OR flexible Sequence found
			// -6 (79) = Overlapping ShiftAlt-O with F1-F4
			if (isCSI){
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
				}
				else{
					// Any known terminator of a flexible length sequence ?
					switch (c){
					case 82:
						// Actual Cursor Position
						if (TERM_ScreenSizeInCursorPos){
							// but as TerminalSize substitute
							TERM_ScreenWidth = atoi(pNumPos[2]);
							TERM_ScreenHeight = atoi(pNumPos[1]);
							TERM_ScreenSizeInCursorPos = 0;
							r = 177;
						}
						else{
							TERM_CursorPosY = atoi(pNumPos[1]);
							TERM_CursorPosX = atoi(pNumPos[2]);
							TERM_CursorWaitFor = 0;
							r = 190;
						}
						break;				
					case 77:
						// Mouse Move  /  Mouse Down / Wheel Down
					case 109:
						// Mouse Up / Wheel Up
						if  (isMouse){
							TERM_MousePosX = atoi(pNumPos[2]);
							TERM_MousePosY = atoi(pNumPos[3]);
							r = (atoi(pNumPos[1]));
							// Switch off Shift / Alt / Ctrl
							r &= ~((1 << 2) | (1 << 3) | (1 << 4));
							
							if (c == 77){
								// Mouse Move  /  Mouse Down / Wheel Down
								switch(r){
								case 35:
									// Move
									r = 166;
									break;
								case 32:
									// Move Button Pressed
									r = 167;
									break;
								case 34:
									// Move Right Button Pressed
									r = 169;
									break;
								case 64:
									// Wheel Scroll Up
									r = 170;
									break;
								case 65:
									// Wheel Scroll Down
									r = 171;
									break;
								case 0:
									// Button Down
								case 1:
									// Wheel Down
								case 2:
									// Right Button Down
									TERM_MouseButton = 1 << r;
									TERM_MouseSelX = TERM_MousePosX; TERM_MouseSelY = TERM_MousePosY;
									r += 162;
									break;
								case 33:
									// Move Wheel Pressed
									r = 168;
									break;
								default:
									// UMO - Unknown Mouse Object
									r = 172;
									break;
								}
							}
							else{
								// Mouse Up / Wheel Up
								if (r < 3){
									// Mouse Up (Wheel / Right / Left)
									r = 165;
								}
								else{
									// UMO - Unknown Mouse Object
									r = 172;
								}
							}
						}
						break;
					case 116:
						// Terminal Infos
						switch (atoi(pNumPos[1])){
						case 8:
							// ScreenSize
							TERM_ScreenHeight = atoi(pNumPos[2]);
							TERM_ScreenWidth = atoi(pNumPos[3]);
							r = 177;						
							break;
						
						default:
							// Unknown terminal-info sequence
							r = 176;
						}
						break;
					default:
						// Unknown or broken sequence
						// r = -2;
						break;
					}
				}
			}	
		}
	}

	if (r < -1 && r != -6){
		// more or less fucked up - or valid Start
		gStreamInESC27[0] = 0;
		TERM_KeyAlt = 0; TERM_KeyCtrl = 0; TERM_KeyMeta = 0; TERM_KeyShift = 0;
		gStreamInESC27[1] = 0;
		isCSI = 0; isOSC = 0; allowTxt = 0; numCnt = 0; waitForEOT = 0;
		isMouse = 0; isByteMouse = 0; TERM_ScreenSizeInCursorPos = 0; TERM_CursorWaitFor = 0;
		streamPos = 0;
		if (r == -27){
			isValid = 1;
			r = 0;
		}
		else{
			isValid = 0;
			TermFlushInKey();
		}
	}
	else if (r > 0){
		// valid sequence
		if (r == 27){
			/* code */
		}
		isValid = 0;
	}
	else{
		// 0 = Wait for more
		// -1 = valid - but not ESC - related...
		// -6 = ShiftAlt-O overlapping with F1-F4
	}
	
	return r;
}

static int GetESC27_CheckOnF512(void){
/**
 * @brief Extract F5 - F12 from gStreamInESC27[]
 * 
 * @return int 			132 - 139 	F5 - F8
 * 						-1			Error
 */

	int r = gStreamInESC27[3];
	switch (r){
	case 48:
	case 49:
		// F9 & F10
		r += 88;
		break;
	case 51:
	case 52:
		// F11 & F12
		r += 87;
		break;
	case 53:
		// F5
		r = 132;
		break;
	case 55:
	case 56:
	case 57:
		// F6 & F7 & F8
		r += 78;
		break;
	default:
		r = 0;
		break;
	}
	return r;
}

static int GetESC27_CheckOnF112Key(int r, int posInStream){
/**
 * @brief Extract Shift/Alt/Ctrl from gStreamInESC27[]
 * 
 * @param	r			for return
 * 
 * @param	posInStream	Position of to switch char
 * 
 * @return	int			r 	= 	success ( and TERM_KeyXYZ get set)
 * 						-1	=	Fail 
 */

	switch (gStreamInESC27[posInStream]){
	case 50:
		TERM_KeyShift = 1;
		break;
	case 51:
		TERM_KeyAlt = 1;
		break;
	case 53:
		TERM_KeyCtrl = 1;
		break;
	case 54:
		TERM_KeyShift = 1; TERM_KeyCtrl = 1;
		break;
	case 52:
		TERM_KeyShift = 1; TERM_KeyAlt = 1;
		break;
	case 55:
		TERM_KeyAlt = 1; TERM_KeyCtrl = 1;
		break;
	case 56:
		TERM_KeyShift = 1; TERM_KeyAlt = 1; TERM_KeyCtrl = 1;
		break;
	default:
		r= 0;
		break;
	}				
	return r;
}

int TermWaitForESC27(char *pStrExchange, int waitForID, float timeOut){
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

    int r = 0;
    int i = 0;
	int gotChar = 0;
	clock_t timeExit;

	fflush(stdin);
	TermFlushInKey();
    printf("%s",pStrExchange);
	fflush(stdout);

	timeExit = clock() + (long)(timeOut * gClocksPerSecond);

    while (clock() < timeExit){
		
		i = TermInKey();

        if (i > 0){
			gotChar = 1;
            r = TermGetESC27(i);
            if (r > 0){
				if ((r == waitForID) || (waitForID == 0)){
					// Valid
					TermFlushInKey();				
					return r;
				}
            }
			else if (r < 0){
				// Error
                TermFlushInKey();
				return r;
			}
        }
    }

    // TimeOut
	TermFlushInKey();
	if (gotChar){
        // With char(s) received
        return -3;
    }
    else{
        // Without char(s) received
       	return 0;
    }
}

void TermSignalHandler(int sig){
/**
 * @brief Signal-Handler for signal()
 * 
 * @param sig (int)
 */

	if (SIGINT == sig){
		// Ctrl-C pressed
		TERM_SignalCtrlC = 1;
		#if __WIN32__ || _MSC_VER || __WIN64__
			// Catch Ctrl-C again...
			signal(SIGINT, SignalHandler);
		#endif
	}
	#if __WIN32__ || _MSC_VER || __WIN64__
	#else	
		else if (SIGWINCH == sig){
			// Terminal-Size Changed
			TERM_SignalTerminalSize = 1;
		}
		else if (SIGALRM == sig){
			// 31/3 Interval
			TERM_SignalInterval = 1;
		}
	#endif
	/*else if (SIGURG == sig){
		// IO - Ready
		printf("Yeah IO-READY");
	}*/
}

void TermTrapMouse(int set){
/**
 * @brief Enable / Disable Mouse Trapping
 * 
 * @param set (int / bool)	1 = Enable
 * 							0 = Disable
 * 
 * @private c (char)		helper for set
 */

	static int trapMode = 1002;

	char c = 'l';
	if (set){
		c = 'h';
		if (set == 2){
			trapMode = 1003;
		}
		else{
			trapMode = 1002;
		}
	}
	// Any Event (1003) / Decimal Values (1006)
	// 1002 instead of 1003 reports position only if Mouse Button is pressed
	printf("\x1B[?%d%c\x1B[?1006%c", trapMode, c, c);
}

void TermTrapFocus(int set){
/**
 * @brief Enable / Disable TerminalFocus Trapping
 * 
 * @param set (int / bool)	1 = Enable
 * 							0 = Disable
 * 
 * @private c (char)		helper for set
 */

	char c = 'l';
	if (set){
		c = 'h';
	}
	printf("\x1B[?1004%c", c);
}

// EVENTS
int TermEventESC27 (int event){
	
	int r = 0;		// Return Value
	
	// Modification Keys
	int keyState = 0;

	switch (event){
	case -1:
		// Received (part) of regular char
		TermGotChar(gStreamInESC27[0]);
		break;
	case 9:
		// (shift) Tab
		TermNavPress[TERM_Event_NavTab](TERM_KeyShift);
		break;
	case 27:
		// ESC
		TermEscPress();
		break;
	case 127:
		// Back
		TermNavPress[TERM_Event_NavBack](TERM_KeyAlt);
		break;
	case 202:
		// Mouse Area
		TermMouseArea(TERM_MouseSelX, TERM_MouseSelY, TERM_MousePosX, TERM_MousePosY, TERM_MouseButton);
		break;
	case 177:
		// Terminal Size received
		if (TermSizeIsChanged()){
			TermSizeChanged(TERM_ScreenWidth, TERM_ScreenHeight);
		}
		break;
	case 176:
		// Unknown Terminal-Info
		TermInfo[TERM_Event_InfoUnknown](gStreamInESC27);
		break;
	default:
		keyState = (TERM_KeyShift) + (TERM_KeyAlt * 2) + (TERM_KeyCtrl * 4);
		if (event > 143 && event < 157){
			// Navigation Keys
			TermNavPress[event - 144](keyState);
		}
		else if (event > 127 && event < 140){
			// F1 - F12
			TermFkeyPress[event - 128](keyState);	
		}
		else if (event > 64 && event < 91){
			// (Shift)ALT-A - (Shift)ALT-Z
			TermAltPress[event - 65](TERM_KeyShift);
		}
		else if (event > 0 && event < 27){
			// Ctrl-A - Ctrl-Z
			TermCtrlPress[event - 1];
		}
		else if (event > 161 && event < 173){
			// Raw Mouse
			TermRawMouse[event - 162](TERM_MousePosX, TERM_MousePosY);
		}
		else if (event > 199 && event < 202){
			// Mouse Click & DblClick
			TermMouseClicks[event - 200](TERM_MousePosX, TERM_MousePosY, TERM_MouseButton);
		}
		else if (event > 159 && event < 162){
			// Got 'n' Lost Focus
			TermCtrlPress[event - 160];
		}
		else if (event > 177 && event < 180){
			// Terminal Name and Label
			TermInfo[event - 178](gStreamInESC27);
		}
		break;
	}

	#if IS_TERMINAL_EVENT_DEBUG
		if (TERM_KeyShift){
			printf("Shift-");
		}
		if (TERM_KeyAlt){
			printf("Alt-");
		}
		if (TERM_KeyCtrl){
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
			// Back & F1 - F12
			if (event != 127){
				printf("F%d", event - 127);
			}
			switch (event){
			case 127:
				printf("Back");
				// Back
				if (TERM_KeyAlt){
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
				if (TERM_KeyShift){
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
				if (TERM_KeyShift){
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
				if (TermSizeChanged()){
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
				switch (TERM_MouseButton){
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
			switch (TERM_MouseButton){
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
				switch (TERM_MouseButton){
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

		if (event > 31){
			printf("  : %s\n", &gStreamInESC27[1]);
		}
		else{
			printf("\n");
		}

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

	TERM_KeyAlt = 0; TERM_KeyCtrl = 0; TERM_KeyMeta = 0; TERM_KeyShift = 0;
	return r;
}

void TermCoreLoop(void(*UserLoop)()){

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
			int terminalSizeChangeCnt = Terminal_Size_Change_Trigger;
		#endif

	// Loop Minimum
	// while (i != 10 && i != 13){
	while (TERM_RunCoreLoop){
		
		i = TermInKey();

		// Recognize manual ESC
		if (isOnUsrESC27 && !i){
			if (clock() > timeOnUsrEsc){
				// UsrESC
				i = 27;
			}
		}

		// Recognize timeout while receiving ESC
		else if (isOnESC27 && (!isOnUsrESC27 && !i)){
			// The !isOnUsrESC27 signals, that we already got more chars than just the 1st ESC
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
			// Enter (User)ESC-Sequences recognition
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
			
			r = TermGetESC27(i);

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
				TermEventESC27(165);
				isOnESC27 = 0;
				r = 0;
				if ((TERM_MouseSelX == TERM_MousePosX) && (TERM_MouseSelY == TERM_MousePosY)){
					// it's a (dbl)click
					if (isOnClick && clock() < timeOnClick){
						// dblClick
						TermEventESC27(201);
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
						TermEventESC27(201);
						isOnClick = 0;
					}
					else{
						// finally area
						TermEventESC27(202);
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
				TermEventESC27(r);
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
			TermEventESC27(200);
			isOnClick = 0;
		}

		if (!i){
			// We did not received anything
			// Time to check for RealTime things
			#if __WIN32__ || _MSC_VER || __WIN64__
				CheckOnTimeChange();
			#else
				if (TERM_SignalInterval){
					TimeCheckOnChange();
				}
			#endif
		}				

		
		// DO USER STUFF HERE START
		UserLoop();
		// DO USER STUFF HERE STOP

		
		if (!i){
			// We did not received anything
			// Time to do 2nd Time idle-time things
			#if __WIN32__ || _MSC_VER || __WIN64__
				terminalSizeChangeCnt--;
				if (!terminalSizeChangeCnt){
					// Every xx idle-times check on ScreenSize
					terminalSizeChangeCnt = Terminal_Size_Change_Trigger;
					GetTerminalSize(3);
					TermEventESC27(177);
				}
				else {
					DoEvents();
				}	
			#else
				if (TERM_SignalInterval){
					if (TERM_SignalTerminalSize){
						TermGetSize(3);
						TermEventESC27(177);
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
