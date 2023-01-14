
//		   ANSI-ESC - Terminal Functions - See detailed description at the EOF !
//				! Using 3rd party MIT/GPL/"FreeSoftware" technologies !
//				  (see related declarations/functions and at the EOF)
// This is a  ! p_o_o_r !  Header by (c) Pit Demmer. (PIT-Licensed 01.04.2022 -           )

const char AnsiESCVersion [] = "1.00pa";

// WIN & Linux
#include "Terminal.h"
#include <string.h>

// #include "UTFconst.h"

typedef struct {
	unsigned char mode;			// 0 = 16, 1 = 255, 2 = RGB
	union ColorFG {
		uint32_t Color;	// !! 32 bit !!
		unsigned char dummy;	// ZERO - Then direct int32 -> Hex will work for RGB
		unsigned char R;
		unsigned char G;
		unsigned char B;
	}fg;						// ForeGround (16 & 255 on fg.R)
	union ColorBG {
		uint32_t Color;
		unsigned char dummy;					
		unsigned char R;
		unsigned char G;
		unsigned char B;
	}bg;						// BackGround
	union ColorUL {
		uint32_t Color;	// UL - color just relevant in modes 1 & 2 (in mode '0': 'ul' == 'fg')
		unsigned char dummy;
		unsigned char R;					
		unsigned char G;
		unsigned char B;
	}ul;						// Underline
} TxtColorSTRUCT;


typedef struct {
	/*		Actual State in [0]
			Default in [1]
			[2] - [n] free for user
	*/
	// 5 unsigned char
	unsigned char bold				:1;
	unsigned char faint				:1;
	unsigned char italic			:1;
	unsigned char blink				:1;
	unsigned char fast				:1;
	unsigned char reverse			:1;
	unsigned char invisible			:1;
	unsigned char strike			:1;

	unsigned char superscript		:1;
	unsigned char subscript			:1;
	unsigned char proportional		:1;
	unsigned char framed			:1;
	unsigned char encircled			:1;
	unsigned char overline			:1;
	unsigned char ideo_right		:1;		// CSI60m (Ideogram 'rarely supported')
	unsigned char ideo_dbl_right	:1;		// CSI61m ( "    "    "   "   "     "  )
	
	unsigned char ideo_left			:1;		// CSI62m ( "    "    "   "   "     "  )
	unsigned char ideo_dbl_left		:1;		// CSI63m ( "    "    "   "   "     "  )
	unsigned char ideo_stress		:1;		// CSI64m ( "    "    "   "   "     "  )
	unsigned char dbl_width			:1;
	unsigned char dbl_height		:1;
	unsigned char dummy1			:1;		// Fill the 3rd unsigned char
	unsigned char dummy2			:1;		// ( "   "   "   " ) 
	unsigned char dummy3			:1;		// ( "   "   "   " )
	
	unsigned char font				:4;		// CSI10m (Standard = 0) - CSI20m (Fraktur = 10) 
	unsigned char underline			:4;		// 0 = None, 1 = single, 2 = double, 3 = curl, 4 = dot, 5 = dash 
											// 6 = dashdot, 7 = dbl_curl, 8 = dbl_dot, 9 = dbl_dash
	TxtColorSTRUCT *pColor;
} TxtStyleSTRUCT; 


// Standard ESC Sequences
char CSI[] = "\x1B[";			
char RUL[] = "24";				// 'ResetUnderLine' alternativ: "4:0" 
char FP3[] = "\x1B#";			// 3Fp (private-use) used for text width / height
char OSC[] = "\x1B]";

// Actual Settings / States
TxtStyleSTRUCT ActTxtStyle;
TxtColorSTRUCT ActTxtColor;

void ResFBU(void);

void InitEscSeq(void) {

}

// 'VGA'16-Pallet - colors
enum {
	fgBlack = 30, fgRed, fgGreen, fgYellow, fgBlue, fgMagenta, fgCyan, fgWhite,
	bgBlack = 40, bgRed, bgGreen, bgYellow, bgBlue, bgMagenta, bgCyan, bgWhite,
	fgBlackB = 90, fgRedB, fgGreenB, fgYellowB, fgBlueB, fgMagentaB, fgCyanB, fgWhiteB,
	bgBlackB = 100, bgRedB, bgGreenB, bgYellowB, bgBlueB, bgMagentaB, bgCyanB, bgWhiteB
};

// 'VGAs' as 256 - colors
enum {
	cBlack, cRed, cGreen, cYellow, cBlue, cMagenta, cCyan, cWhite,
	cBlackB, cRedB, cGreenB, cYellowB, cBlueB, cMagentaB, cCyanB, cWhiteB
};

// 256 Grey's White -> Black 
unsigned char cGrey[27];		// initialization in InitColors

// Solarized Colors from Ethan Schoonover : 	github.com/altercation/solarized
// As 256 Palette
enum {
	scBase03 = 234, scBase02 = 235, scBase01 = 240, scBase00 = 241, scBase0 = 244, scBase1 = 245, scBase2 = 254, scBase3 = 230
};

enum {
	scYellow = 136, scOrange = 166, scRed = 160, scMagenta = 125, scViolet = 61, scBlue = 33, scCyan = 37, scGreen = 64
};
// darker 
enum {
	scYellowD = 64, scOrangeD = 94, scRedD = 88, scMagentaD = 53, scVioletD = 56, scBlueD = 26, scCyanD = 30, scGreenD = 58
};
// brighter
enum {
	scYellowB = 208, scOrangeB = 202, scRedB = 196, scMagentaB = 197, scVioletB = 133, scBlueB = 39, scCyanB = 43, scGreenB = 70
};

void InitColors(void) {

	// The 24 Greys (+ Black/White) from 256 Palette	
	cGrey[1] = 15; cGrey[26] = 0;
	int p = 1;
	for (int t = 255; t > 231; t--) {
		p++;
		cGrey[p] = t;
	}

	// Default ColorStyle
	ActTxtStyle.pColor = &ActTxtColor;

}

char *KeyID2String[] = {
	"0=NA",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"6=NA",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"12=NA",
	"F11",
	"F12",
	"ShiftF1",
	"ShiftF2",
	"ShiftF3",
	"ShiftF4",
	"ShiftF5",
	"20=NA",
	"ShiftF6",
	"ShiftF7",
	"ShiftF8",
	"ShiftF9",
	"ShiftF10",
	"26=NA",
	"ShiftF11",
	"ShiftF12",
	"CtrlF1",
	"CtrlF2",
	"CtrlF3",
	"CtrlF4",
	"CtrlF5",
	"34=NA",
	"CtrlF6",
	"CtrlF7",
	"CtrlF8",
	"CtrlF9",
	"CtrlF10",
	"40=NA",
	"CtrlF11",
	"CtrlF12",
	"Up",
	"Down",
	"Right",
	"Left",
	"Center",
	"End",
	"49=NA",
	"Pos1",
	"Ins",
	"Del",
	"53=NA",
	"PgUp",
	"PgDown",
	"ShiftUp",
	"ShiftDown",
	"ShiftRight",
	"ShiftLeft",
	"60=NA",
	"ShiftEnd",
	"62=NA",
	"ShiftPos1",
	"CtrlUp",
	"CtrlDown",
	"CtrlRight",
	"CtrlLeft",
	"CtrlCenter",
	"CtrlEnd",
	"70=NA",
	"CtrlPos1",
	"AltUp",
	"AltDown",
	"AltRight",
	"AltLeft",
	"AltCenter",
	"AltEnd",
	"78=NA",
	"AltPos1",
	"AltA",
	"AltB",
	"AltC",
	"AltD",
	"AltE",
	"AltF",
	"AltG",
	"AltH",
	"AltI",
	"AltJ",
	"AltK",
	"AltL",
	"AltM",
	"AltN",
	"AltO",
	"AltP",
	"AltQ",
	"AltR",
	"AltS",
	"AltT",
	"AltU",
	"AltV",
	"AltW",
	"AltX",
	"AltY",
	"AltZ",
	"Back",
	"Cursor",
	"UsrESC",
	"TermSize",
	"TermIconLabel",
	"TermWinName",
	"TermERR",
	"TAB",
	"ShiftTAB",
	"ENTER",
	"MouseDown",
	"MouseUp",
	"WheelDown",
	"WheelUp",
	"MouseMove",
	"MouseDownMove",
	"WheelDownMove",
	"WheelScrollUp",
	"WheelScrollDown",
	"UnknownMouseObject",
	"RMouseDown",
	"RMouseDownMove",
	"RMouseUp",
	"ShiftAltA",
	"ShiftAltB",
	"ShiftAltC",
	"ShiftAltD",
	"ShiftAltE",
	"ShiftAltF",
	"ShiftAltG",
	"ShiftAltH",
	"ShiftAltI",
	"ShiftAltJ",
	"ShiftAltK",
	"ShiftAltL",
	"ShiftAltM",
	"ShiftAltN",
	"ShiftAltO",
	"ShiftAltP",
	"ShiftAltQ",
	"ShiftAltR",
	"ShiftAltS",
	"ShiftAltT",
	"ShiftAltU",
	"ShiftAltV",
	"ShiftAltW",
	"ShiftAltX",
	"ShiftAltY",
	"ShiftAltZ",
	"UsrEscLenErr",
	"ByteModePositionErr",
	"AltBack",
	"GotFocus",
	"LostFocus",
	"AltF1",
	"AltF2",
	"AltF3",
	"AltF4",
	"AltF5",
	"165=NA",
	"AltF6",
	"AltF7",
	"AltF8",
	"AltF9",
	"AltF10",
	"171=NA",
	"AltF11",
	"AltF12",
	"ShiftAltF1",
	"ShiftAltF2",
	"ShiftAltF3",
	"ShiftAltF4",
	"ShiftAltF5",
	"179=NA",
	"ShiftAltF6",
	"ShiftAltF7",
	"ShiftAltF8",
	"ShiftAltF9",
	"ShiftAltF10",
	"185=NA",
	"ShiftAltF11",
	"ShiftAltF12",
	"AltCtrlF1",
	"AltCtrlF2",
	"AltCtrlF3",
	"AltCtrlF4",
	"AltCtrlF5",
	"193=NA",
	"AltCtrlF6",
	"AltCtrlF7",
	"AltCtrlF8",
	"AltCtrlF9",
	"AltCtrlF10",
	"199=NA",
	"AltCtrlF11",
	"AltCtrlF12",
	"ShiftCtrlF1",
	"ShiftCtrlF2",
	"ShiftCtrlF3",
	"ShiftCtrlF4",
	"ShiftCtrlF5",
	"207=NA",
	"ShiftCtrlF6",
	"ShiftCtrlF7",
	"ShiftCtrlF8",
	"ShiftCtrlF9",
	"ShiftCtrlF10",
	"213=NA",
	"ShiftCtrlF11",
	"ShiftCtrlF12",
	"ShiftAltCtrlF1",
	"ShiftAltCtrlF2",
	"ShiftAltCtrlF3",
	"ShiftAltCtrlF4",
	"ShiftAltCtrlF5",
	"221=NA",
	"ShiftAltCtrlF6",
	"ShiftAltCtrlF7",
	"ShiftAltCtrlF8",
	"ShiftAltCtrlF9",
	"ShiftAltCtrlF10",
	"227=NA",
	"ShiftAltCtrlF11",
	"ShiftAltCtrlF12",
	"Ctrl-A",
	"Ctrl-B",
	"Ctrl-C",
	"Ctrl-D",
	"Ctrl-E",
	"Ctrl-F",
	"Ctrl-G",
	"Ctrl-H",
	"Ctrl-I",
	"Ctrl-J",
	"Ctrl-K",
	"Ctrl-L",
	"Ctrl-M",
	"Ctrl-N",
	"Ctrl-O",
	"Ctrl-P",
	"Ctrl-Q",
	"Ctrl-R",
	"Ctrl-S",
	"Ctrl-T",
	"Ctrl-U",
	"Ctrl-V",
	"Ctrl-W",
	"Ctrl-X",
	"Ctrl-Y",
	"Ctrl-Z",
};

// Keys
enum {
	keyF1 = 1, keyF2, keyF3, keyF4, keyF5, keyF6 = 7, keyF7, keyF8, keyF9, keyF10, keyF11 = 13, keyF12
};
enum {
	keyShiftF1 = 15, keyShiftF2, keyShiftF3, keyShiftF4, keyShiftF5, keyShiftF6 = 21, keyShiftF7, keyShiftF8, keyShiftF9, keyShiftF10, keyShiftF11 = 27, keyShiftF12
};
enum{
	keyCtrlF1 = 29, keyCtrlF2, keyCtrlF3, keyCtrlF4, keyCtrlF5, keyCtrlF6 = 35, keyCtrlF7, keyCtrlF8, keyCtrlF9, keyCtrlF10, keyCtrlF11 = 41, keyCtrlF12
};
enum{
	keyUp = 43, keyDown, keyRight, keyLeft, keyCenter, keyEnd, keyPos1 = 50, keyIns, keyDel, keyPgUp = 54, keyPgDown
};
enum{
	keyShiftUp = 56, keyShiftDown, keyShiftRight, keyShiftLeft, keyShiftEnd = 61, keyShiftPos1 = 63
};
enum{
	keyCtrlUp = 64, keyCtrlDown, keyCtrlRight, keyCtrlLeft, keyCtrlCenter, keyCtrlEnd, keyCtrlPos1 = 71
};
enum{
	keyAltUp = 72, keyAltDown, keyAltRight, keyAltLeft, keyAltCenter, keyAltEnd, keyAltPos1 = 79
};
enum{
	keyAltA = 80, keyAltB, keyAltC, keyAltD, keyAltE, keyAltF, keyAltG, keyAltH, keyAltI, keyAltJ, keyAltK, keyAltL, keyAltM, keyAltN, keyAltO, keyAltP, keyAltQ,
	keyAltR, keyAltS, keyAltT, keyAltU, keyAltV, keyAltW, keyAltX, keyAltY, keyAltZ, keyBack, keyESC = 108
};
enum{trmScrSize = 109, trmIconLabel, trmWinTitle, trmERR
};

/*
	Check Stream from Terminal/User on known ESC-Sequences
	See detailed stream-list of recognized sequences at the EOF
*/


// Cursor Positions
void Locate(int x, int y) {
	// printf("%c[%d;%df", 0x01b, y, x);		//('f' instead 'H') is working, at least on WIN, too. (more... see wikipedia 'ANSI-ESC') 
	printf("\x1B[%d;%dH", y, x);
}
void LocateX(int x) {
	printf("\x1B[%dG", x);
}
void CursorUp(int y) {
	printf("\x1B[%dA", y);
}
void CursorUp1st(int y) {
	printf("\x1B[%dF", y);
}
void CursorDown(int y) {
	printf("\x1B[%dB", y);
}
void CursorDown1st(int y) {
	printf("\x1B[%dE", y);
}
void CursorRight(int x) {
	printf("\x1B[%dC", x);
}
void CursorLeft(int x) {
	printf("\x1B[%dD", x);
}
void GetAnsiCursorPos(void){
	TERM_CursorWaitFor = 1;
	printf("\x1B[6n");
}

// Clear Lines
void ClrLineA(int xS, int xE){
	// ClearLine from Start(xS) to End(xE)

	int x = 0;

	// Normalize  
	if (xS > xE){
		x = xS;
		xS = xE;
		xE = x;
	}
	// Line Width
	x = xE - xS + 1;

	ResFBU();
	LocateX(xS);
	printf("%*s", x, "");
}
void ClrLine(void) {
	// ClearLine
	printf("\x1B[2K");
}
void ClrLineL(void) {
	// ClearLine from cursor to Left
	printf("\x1B[1K");
}
void ClrLineR(void) {
	// ClearLine from cursor to Right
	printf("\x1B[K");
	// printf("%s0K", CSI);	// The '0' isn't needed.
}

// Clear Screens
void ClrScrA(int xS, int yS, int xE, int yE){
	// Area CLS from Start(xS/Ys) to End(xE/yE)
	
	if (yS == yE){
		// just one Line
		Locate(xS, yS);
		ClrLineA(xS, xE);
	}
	else{
		int x = 0;
		int y = 0;
		int i = 0;

		// Normalize  
		if (xS > xE){
			x = xS;
			xS = xE;
			xE = x;
		}
		if (yS > yE){
			y = yS;
			yS = yE;
			yE = y;
		}
		//Area Width / Height
		x = xE - xS + 1;
		y = yE - yS + 1;

		// Reset all colors
		ResFBU();

		for (i = 0; i < y; i++){
			Locate(xS, yS + i);
			printf("%*s", x, "");
		}
	}
}
void ClrScrL(void) {
	// Cls from cursor to Upper Left
	printf("\x1B[1J");
}
void ClrScrR(void) {
	// Cls from cursor to LowerRight
	printf("\x1B[J");
	// printf("%s0J", CSI);	// The '0' isn't needed.
}

// Reset To Default Colors
void ResFg(void) {
	ActTxtColor.fg.R = 39; ActTxtColor.mode = 0; ActTxtStyle.pColor = NULL;
	printf("\x1B[39m");
}
void ResBg(void) {
	ActTxtColor.bg.R = 49; ActTxtColor.mode = 0; ActTxtStyle.pColor = NULL;
	printf("\x1B[49m");
}
void ResUl(void) {
	ActTxtColor.ul.R = 59; ActTxtColor.mode = 0; ActTxtStyle.pColor = NULL;
	printf("\x1B[59m");
}
void ResFB(void) {
	ActTxtColor.fg.R = 39; ActTxtColor.bg.R = 49;
	ActTxtColor.mode = 0; ActTxtStyle.pColor = 0;
	printf("\x1B[39;49");
}
void ResFBU(void) {
	ActTxtColor.fg.R = 39; ActTxtColor.bg.R = 49; ActTxtColor.ul.R = 59;
	ActTxtColor.mode = 0; ActTxtStyle.pColor = 0;
	printf("\x1B[39;49;59m");
}

// Set 24 bit Color
void SetFgRGB(unsigned char r, unsigned char g, unsigned char b) {
	ActTxtStyle.pColor->fg.R = r; ActTxtStyle.pColor->fg.G = g; ActTxtStyle.pColor->fg.B = b; ActTxtStyle.pColor->mode = 2;
	printf("\x1B[38;2;%d;%d;%dm", r, g, b);
}
void SetBgRGB(unsigned char r, unsigned char g, unsigned char b) {
	ActTxtStyle.pColor->bg.R = r; ActTxtStyle.pColor->bg.G = g; ActTxtStyle.pColor->bg.B = b; ActTxtStyle.pColor->mode = 2;
	printf("\x1B[48;2;%d;%d;%dm", r, g, b);
}
void SetUlRGB(unsigned char r, unsigned char g, unsigned char b) {
	ActTxtStyle.pColor->ul.R = r; ActTxtStyle.pColor->ul.G = g; ActTxtStyle.pColor->ul.B = b; ActTxtStyle.pColor->mode = 2;
	printf("\x1B[58;2;%d;%d;%dm", r, g, b);
}
void SetFBrgb(unsigned char fgR, unsigned char fgG, unsigned char fgB, unsigned char bgR, unsigned char bgG, unsigned char bgB) {
	SetFgRGB(fgR, fgG, fgB);
	SetBgRGB(bgR, bgG, bgB);
}
void SetFBUrgb(unsigned char fgR, unsigned char fgG, unsigned char fgB, unsigned char bgR, unsigned char bgG, unsigned char bgB, unsigned char ulR, unsigned char ulG, unsigned char ulB) {
	SetFgRGB(fgR, fgG, fgB);
	SetBgRGB(bgR, bgG, bgB);
	SetUlRGB(ulR, ulG, ulB);
}

// Set 256 Colors
void SetFg255(unsigned char c) {
	ActTxtStyle.pColor->fg.R = c; ActTxtStyle.pColor->mode = 1;
	printf("\x1B[38;5;%dm", c);
}
void SetBg255(unsigned char c) {
	ActTxtStyle.pColor->bg.R = c; ActTxtStyle.pColor->mode = 1;
	printf("\x1B[48;5;%dm", c);
}
void SetUl255(unsigned char c) {
	ActTxtStyle.pColor->ul.R = c; ActTxtStyle.pColor->mode = 1;
	printf("\x1B[58;5;%dm", c);
}
void SetFB255(unsigned char fg, unsigned char bg) {
	SetFg255(fg);
	SetBg255(bg);
}
void SetFBU255(unsigned char fg, unsigned char bg, unsigned char ul) {
	SetFg255(fg);
	SetBg255(bg);
	SetUl255(ul);
}

//Set 16 Colors
void SetFg16(unsigned char c){
	// ForeGround	
	if (!((c > 29 && c < 38) || (c > 89 && c < 98))){
		ResFg();
	}
	else{
		ActTxtStyle.pColor->fg.R = c; ActTxtStyle.pColor->mode = 0;
		printf("\x1B[%dm", c);
	}	
}
void SetBg16(unsigned char c) {	
	// BackGround
	if (!((c > 39 && c < 48) || (c > 99 && c < 108))){
		ResBg();
	}
	else{
		ActTxtStyle.pColor->bg.R = c; ActTxtStyle.pColor->mode = 0;
		printf("\x1B[%dm", c);
	}
}
void SetFB16(unsigned char fg, unsigned char bg) {
	SetFg16(fg);
	SetBg16(bg);
}

// TXT Styles - single commands
void TxtBold(unsigned char set) {
	ActTxtStyle.bold = set;
	ActTxtStyle.faint = 0;
	if (set) {
		// Set
		printf("\x1B[1m");
	}
	else {
		// Reset
		printf("\x1B[22m");
	}
}
void TxtFaint(int set) {
	ActTxtStyle.faint = set;
	ActTxtStyle.bold = 0;
	if (set) {
		// Set
		printf("\x1B[2m");
	}
	else {
		// Reset
		printf("\x1B[22m");
	}
}
void TxtItalic(int set) {
	ActTxtStyle.italic = set;
	if (set) {
		// Set
		printf("\x1B[3m");
	}
	else {
		// Reset
		printf("\x1B[23m");
	}
}
void TxtUnder(int set) {
	ActTxtStyle.underline = set;
	if (set) {
		// Set
		printf("\x1B[4m");
	}
	else {
		// Reset
		printf("\x1B[%sm", RUL);
	}
}
void TxtDblUnder(int set) {
	if (set) {
		// Set
		ActTxtStyle.underline = 2;
		printf("\x1B[21m");
	}
	else {
		// Reset
		TxtUnder(0);
	}
}
void TxtCurlUnder(int set) {
	if (set) {
		// Set
		ActTxtStyle.underline = 3;
		printf("\x1B[4:3m");
	}
	else {
		// Reset
		TxtUnder(0);
	}
}
void TxtDotUnder(int set) {
	if (set) {
		// Set
		ActTxtStyle.underline = 4;
		printf("\x1B[4:4m");
	}
	else {
		// Reset
		TxtUnder(0);
	}
}
void TxtDashUnder(int set) {
	if (set) {
		// Set
		ActTxtStyle.underline = 5;
		printf("\x1B[4:5m");
	}
	else {
		// Reset
		TxtUnder(0);
	}
}
void TxtDashDotUnder(int set) {
	// Undocumented DashDot - just a prognosis
	if (set) {
		// Set
		ActTxtStyle.underline = 6;
		printf("\x1B[4:6m");
	}
	else {
		// Reset
		TxtUnder(0);
	}
}
void TxtDblCurlUnder(int set) {
	// Undocumented DoubleCurl - just a prognosis
	if (set) {
		// Set
		ActTxtStyle.underline = 7;
		printf("\x1B[4:7m");
	}
	else {
		// Reset
		TxtUnder(0);
	}
}
void TxtDblDotUnder(int set) {
	// Undocumented DoubleDotted - just a prognosis
	if (set) {
		// Set
		ActTxtStyle.underline = 8;
		printf("\x1B[4:8m");
	}
	else {
		// Reset
		TxtUnder(0);
	}
}
void TxtDblDashUnder(int set) {
	// Undocumented DoubleDashed - just a prognosis
	if (set) {
		// Set
		ActTxtStyle.underline = 9;
		printf("\x1B[4:9m");
	}
	else {
		// Reset
		TxtUnder(0);
	}
}

void TxtResetBlink(void){
	printf("\x1B[25m");
}
void TxtBlink(int set) {
	ActTxtStyle.blink = set;
	ActTxtStyle.fast = 0;
	if (set) {
		// Set
		printf("\x1B[5m");
	}
	else {
		// Reset
		TxtResetBlink();
	}
}
void TxtFastBlink(int set) {
	ActTxtStyle.fast = set;
	ActTxtStyle.blink = 0;
	if (set) {
		// Set
		printf("\x1B[6m");
	}
	else {
		// Reset
		TxtResetBlink();
	}
}
void TxtReverse(int set) {
	ActTxtStyle.reverse = set;
	if (set) {
		// Set
		printf("\x1B[7m");
	}
	else {
		// Reset
		printf("\x1B[27m");
	}
}
void TxtInvisible(int set) {
	ActTxtStyle.invisible = set;
	if (set) {
		// Set
		printf("\x1B[8m");
	}
	else {
		// Reset
		printf("\x1B[28m");
	}
}
void TxtStrike(int set) {
	ActTxtStyle.strike = set;
	if (set) {
		// Set
		printf("\x1B[9m");
	}
	else {
		// Reset
		printf("\x1B[29m");
	}
}
void TxtResetScript(void){
	printf("\x1B[75m");
}
void TxtSuperscript(int set) {
	ActTxtStyle.superscript = set;
	ActTxtStyle.subscript = 0;
	if (set) {
		// Set
		printf("\x1B[73m");
	}
	else {
		// Reset
		TxtResetScript();
	}
}
void TxtSubscript(int set) {
	ActTxtStyle.subscript = set;
	ActTxtStyle.superscript = 0;
	if (set) {
		// Set
		printf("\x1B[74m");
	}
	else {
		// Reset
		TxtResetScript();
	}
}
void TxtProportional(int set){
	ActTxtStyle.proportional = set;
	if (set) {
		// Set
		printf("\x1B[26m");
	}
	else {
		// Reset
		printf("\x1B[50m");
	}
}
void TxtFramed(int set) {
	ActTxtStyle.framed = set;
	ActTxtStyle.encircled = 0;
	if (set) {
		// Set
		printf("\x1B[51m");
	}
	else {
		// Reset
		printf("\x1B[54m");
	}
}
void TxtEncircled(int set) {
	ActTxtStyle.encircled = set;
	ActTxtStyle.framed = 0;
	if (set) {
		// Set
		printf("\x1B[52m");
	}
	else {
		// Reset
		printf("\x1B[54m");
	}
}
void TxtOverline(int set) {
	ActTxtStyle.overline = set;
	if (set) {
		// Set
		printf("\x1B[53m");
	}
	else {
		// Reset
		printf("\x1B[55m");
	}
}

void TxtResetIdeo(void) {
	ActTxtStyle.ideo_right = 0; ActTxtStyle.ideo_dbl_right = 0;
	ActTxtStyle.ideo_left = 0; ActTxtStyle.ideo_dbl_left = 0;
	ActTxtStyle.ideo_stress = 0;
	printf("\x1B[65m");
}
void TxtIdeoRight(int set) {
	if (set){
		ActTxtStyle.ideo_right = 1;
		printf("\x1B[60m");
	}
	else{
		TxtResetIdeo();
	}
}
void TxtIdeoDblRight(int set) {
	if (set){
		ActTxtStyle.ideo_dbl_right = 1;
		printf("\x1B[61m");
	}
	else{
		TxtResetIdeo();
	}
}
void TxtIdeoLeft(int set) {
	if (set){
		ActTxtStyle.ideo_left = 1;
		printf("\x1B[62m");
	}
	else{
		TxtResetIdeo();
	}
}
void TxtIdeoDblLeft(int set) {
	if (set){
		ActTxtStyle.ideo_dbl_left = 1;
		printf("\x1B[63m");
	}
	else{
		TxtResetIdeo();
	}
}
void TxtIdeoStress(int set) {
	if (set){
		ActTxtStyle.ideo_stress = 1;
		printf("\x1B[64m");
	}
	else{
		TxtResetIdeo();
	}
}

void TxtResetDblTBW(void){
	printf("\x1B#5");
}
void TxtDblTop(int set) {
	// DoubleHeight + DoubleWidth Top-Part
	ActTxtStyle.dbl_height = set;
	ActTxtStyle.dbl_width = 0;
	if (set) {
		// Set
		printf("\x1B#3");
	}
	else {
		// Reset
		TxtResetDblTBW();
	}
}
void TxtDblBot(int set) {
	// DoubleHeight + DoubleWidth Bottom-Part
	ActTxtStyle.dbl_height = set;
	ActTxtStyle.dbl_width = 0;
	if (set) {
		// Set
		printf("\x1B#4");
	}
	else {
		// Reset
		TxtResetDblTBW();
	}
}
void TxtDblWidth(int set) {
	// DoubleWidth 
	ActTxtStyle.dbl_width = set;
	ActTxtStyle.dbl_height = 0;
	if (set) {
		// Set
		printf("\x1B#6");
	}
	else {
		// Reset
		TxtResetDblTBW();
	}
}

void TxtFont(int fnt) {

	// 10 = Default Font
	// 11 - 19 User Fonts @INVESTIGATE@
	// 20 = Fraktur

	if (fnt > 10){
		fnt = 10;
	}
	if (fnt < 0){
		fnt = 0;
	}
	
	ActTxtStyle.font = fnt;
	printf("%s%dm", CSI, fnt + 10);
}

// TXT Style - combined from Structure
void SetTxtStyle(TxtStyleSTRUCT *pTxtStyle, int set) {

	// set == False -> Reset all to default

	if (set) {
		if (pTxtStyle->bold != ActTxtStyle.bold) {
			TxtBold(pTxtStyle->bold);
		}
		if (pTxtStyle->faint != ActTxtStyle.faint) {
			TxtFaint(pTxtStyle->faint);
		}
		if (pTxtStyle->italic != ActTxtStyle.italic) {
			TxtItalic(pTxtStyle->italic);
		}
		if (pTxtStyle->underline != ActTxtStyle.underline) {
			ActTxtStyle.underline = pTxtStyle->underline;
			switch (pTxtStyle->underline) {
			case 0: // None
			case 1: // Single
				TxtUnder(pTxtStyle->underline);
				break;
			case 2: // Double
				TxtDblUnder(1);
				break;
			case 3: // Curly
				TxtCurlUnder(1);
				break;
			case 4: // Dot
				TxtUnder(1);
				break;
			case 5: // Dash
				TxtDashUnder(1);
				break;
			case 6: // DashDot
				TxtDashDotUnder(1);
				break;
			case 7: // Double Curly
				TxtDblCurlUnder(1);
				break;
			case 8: // Double Dot
				TxtDblDotUnder(1);
				break;
			case 9: // Double Dash
				TxtDblDashUnder(1);
				break;
			default:
				// This would be an error !!
				pTxtStyle->underline = 0;
				TxtUnder(0);
				break;
			}
		}

		if (pTxtStyle->blink != ActTxtStyle.blink) {
			if (!pTxtStyle->fast){
				TxtBlink(pTxtStyle->blink);
			}			
		}
		if (pTxtStyle->fast != ActTxtStyle.fast) {
			TxtFastBlink(pTxtStyle->fast);
		}

		if (pTxtStyle->reverse != ActTxtStyle.reverse) {
			TxtReverse(pTxtStyle->reverse);
		}
		
		if (pTxtStyle->invisible != ActTxtStyle.invisible) {
			TxtInvisible(pTxtStyle->invisible);
		}

		if (pTxtStyle->strike != ActTxtStyle.strike) {
			TxtStrike(pTxtStyle->strike);
		}

		if (pTxtStyle->superscript != ActTxtStyle.superscript) {
			if (!pTxtStyle->subscript){
				TxtSuperscript(pTxtStyle->superscript);
			}			
		}
		if (pTxtStyle->subscript != ActTxtStyle.subscript) {
			TxtSubscript(pTxtStyle->subscript);
		}

		if (pTxtStyle->proportional != ActTxtStyle.proportional) {
			TxtProportional(pTxtStyle->proportional);
		}

		if (pTxtStyle->framed != ActTxtStyle.framed) {
			if (!pTxtStyle->encircled){
				TxtFramed(pTxtStyle->framed);
			}			
		}
		if (pTxtStyle->encircled != ActTxtStyle.encircled) {
			TxtEncircled(pTxtStyle->encircled);
		}

		if (pTxtStyle->overline != ActTxtStyle.overline) {
			TxtOverline(pTxtStyle->overline);
		}

		if ((pTxtStyle->ideo_right != ActTxtStyle.ideo_right) ||
			(pTxtStyle->ideo_dbl_right != ActTxtStyle.ideo_dbl_right) ||
			(pTxtStyle->ideo_left != ActTxtStyle.ideo_left) ||
			(pTxtStyle->ideo_dbl_left != ActTxtStyle.ideo_dbl_left) ||
			(pTxtStyle->ideo_stress != ActTxtStyle.ideo_stress)) {

			TxtResetIdeo();		// 1st... Reset them all...

			if (pTxtStyle->ideo_right) {
				TxtIdeoRight(1);
			}
			else if (pTxtStyle->ideo_dbl_right) {
				TxtIdeoDblRight(1);
			}
			else if (pTxtStyle->ideo_left) {
				TxtIdeoLeft(1);
			}
			else if (pTxtStyle->ideo_dbl_left) {
				TxtIdeoDblLeft(1);
			}
			else if (pTxtStyle->ideo_stress) {
				TxtIdeoStress(1);
			}
		}

		if (pTxtStyle->font != ActTxtStyle.font) {
			if (pTxtStyle->font > 10) {
				pTxtStyle->font = 10;
			}
			TxtFont(pTxtStyle->font);
		}
		
		switch (pTxtStyle->pColor->mode) {
		case 0:		// 16 (has no UnderLineColor)
			if (pTxtStyle->pColor->fg.R != ActTxtColor.fg.R) {
				if (!((pTxtStyle->pColor->fg.R > 29 && pTxtStyle->pColor->fg.R < 38) || (pTxtStyle->pColor->fg.R > 89 && pTxtStyle->pColor->fg.R < 98))){
					pTxtStyle->pColor->fg.R = 39;
					ResFg();
				}
				else{
					SetFg16(pTxtStyle->pColor->fg.R);
				}
			}
			if (pTxtStyle->pColor->bg.R != ActTxtColor.bg.R) {
				if (!((pTxtStyle->pColor->bg.R > 39 && pTxtStyle->pColor->bg.R < 48) || (pTxtStyle->pColor->bg.R > 99 && pTxtStyle->pColor->bg.R < 108))){
					pTxtStyle->pColor->bg.R = 49;
					ResBg();
				}
				else{
					SetBg16(pTxtStyle->pColor->bg.R);
				}
			}
			break;
		
		case 1:		// 255
			if (pTxtStyle->pColor->fg.R != ActTxtColor.fg.R) {
				SetFg255(pTxtStyle->pColor->fg.R);
			}
			if (pTxtStyle->pColor->bg.R != ActTxtColor.bg.R) {
				SetBg255(pTxtStyle->pColor->bg.R);
			}
			if (pTxtStyle->pColor->ul.R != ActTxtColor.ul.R) {
				SetUl255(pTxtStyle->pColor->ul.R);
			}
			break;
		
		case 2:		// RGB
			if (pTxtStyle->pColor->fg.Color != ActTxtColor.fg.Color) {
				SetFgRGB(pTxtStyle->pColor->fg.R, pTxtStyle->pColor->fg.G, pTxtStyle->pColor->fg.B);
			}
			if (pTxtStyle->pColor->bg.Color != ActTxtColor.bg.Color) {
				SetBgRGB(pTxtStyle->pColor->bg.R, pTxtStyle->pColor->bg.G, pTxtStyle->pColor->bg.B);
			}
			if (pTxtStyle->pColor->ul.Color != ActTxtColor.ul.Color) {
				SetUlRGB(pTxtStyle->pColor->ul.R, pTxtStyle->pColor->ul.G, pTxtStyle->pColor->ul.B);
			}
			break;
		}
	}
	else {
		// Reset colors, fonts and styles to their defaults
		memset(&ActTxtStyle, 0, sizeof(ActTxtStyle));
		memset(&ActTxtColor, 0, sizeof(ActTxtColor));
		printf("%s10;0;39;49;59m", CSI);
	}
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
	<AnsiESC.h>		    	01.04.2022		                                Pit

BugID		Description									Found by / Date		Fixed by / Date


*/
