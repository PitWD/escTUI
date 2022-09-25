
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
	// 12 unsigned char
	/*		Actual State in [0]
			Default in [1]
			[2] - [n] free for user
	*/
	unsigned char mode;			// 0 = 16, 1 = 255, 2 = RGB
	union ColorFG {
		unsigned long Color;	// !! 32 bit !!
		unsigned char dummy;	// ZERO - Then direct int32 -> Hex will work for RGB
		unsigned char R;
		unsigned char G;
		unsigned char B;
	}fg;						// ForeGround (16 & 255 on fg.R)
	union ColorBG {
		unsigned long Color;
		unsigned char dummy;					
		unsigned char R;
		unsigned char G;
		unsigned char B;
	}bg;						// BackGround
	union ColorUL {
		unsigned long Color;	// UL - color just relevant in modes 1 & 2 (in mode '0': 'ul' == 'fg')
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
	unsigned char under_right		:1;		// CSI60m (Ideogram 'rarely supported')
	unsigned char under_dbl			:1;		// CSI61m ( "    "    "   "   "     "  )
	
	unsigned char over_left			:1;		// CSI62m ( "    "    "   "   "     "  )
	unsigned char over_dbl			:1;		// CSI63m ( "    "    "   "   "     "  )
	unsigned char stress			:1;		// CSI64m ( "    "    "   "   "     "  )
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
	printf("%s%d;%dH", CSI, y, x);
}
void LocateX(int x) {
	printf("%s%dG", CSI, x);
}
void CursorUp(int y) {
	printf("%s%dA", CSI, y);
}
void CursorUp1st(int y) {
	printf("%s%dF", CSI, y);
}
void CursorDown(int y) {
	printf("%s%dB", CSI, y);
}
void CursorDown1st(int y) {
	printf("%s%dE", CSI, y);
}
void CursorRight(int x) {
	printf("%s%dC", CSI, x);
}
void CursorLeft(int x) {
	printf("%s%dD", CSI, x);
}
void GetAnsiCursorPos(void){
	cursorWaitFor = 1;
	printf("%s6n", CSI);
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
	printf("%s2K", CSI);
}
void ClrLineL(void) {
	// ClearLine from cursor to Left
	printf("%s1K", CSI);
}
void ClrLineR(void) {
	// ClearLine from cursor to Right
	printf("%sK", CSI);
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
	printf("%s1J", CSI);
}
void ClrScrR(void) {
	// Cls from cursor to LowerRight
	printf("%sJ", CSI);
	// printf("%s0J", CSI);	// The '0' isn't needed.
}

// Set 24 bit Color
void SetFgRGB(unsigned char r, unsigned char g, unsigned char b) {

	ActTxtColor.fg.R = r; ActTxtColor.fg.G = g; ActTxtColor.fg.B = b; ActTxtColor.mode = 2; ActTxtStyle.pColor = NULL;
	printf("%s38;2;%d;%d;%dm", CSI, r, g, b);
}
void SetBgRGB(unsigned char r, unsigned char g, unsigned char b) {

	ActTxtColor.bg.R = r; ActTxtColor.bg.G = g; ActTxtColor.bg.B = b; ActTxtColor.mode = 2; ActTxtStyle.pColor = NULL;
	printf("%s48;2;%d;%d;%dm", CSI, r, g, b);
}
void SetUlRGB(unsigned char r, unsigned char g, unsigned char b) {

	ActTxtColor.ul.R = r; ActTxtColor.ul.G = g; ActTxtColor.ul.B = b; ActTxtColor.mode = 2; ActTxtStyle.pColor = NULL;
	printf("%s58;2;%d;%d;%dm", CSI, r, g, b);
}
void SetFBrgb(unsigned char fgR, unsigned char fgG, unsigned char fgB, unsigned char bgR, unsigned char bgG, unsigned char bgB) {
	//SetFgRGB(fgR, fgG, fgB);
	//SetBgRGB(bgR, bgG, bgB);
	ActTxtColor.fg.R = fgR; ActTxtColor.fg.G = fgG; ActTxtColor.fg.B = fgB;
	ActTxtColor.bg.R = bgR; ActTxtColor.bg.G = bgR; ActTxtColor.bg.B = bgB; ActTxtColor.mode = 2; ActTxtStyle.pColor = NULL;
	printf("%s38;2;%d;%d;%d;48;2;%d;%d;%dm", CSI, fgR, fgG, fgB, bgR, bgG, bgB);
}
void SetFBUrgb(unsigned char fgR, unsigned char fgG, unsigned char fgB, unsigned char bgR, unsigned char bgG, unsigned char bgB, unsigned char ulR, unsigned char ulG, unsigned char ulB) {
	//SetFgRGB(fgR, fgG, fgB);
	//SetBgRGB(bgR, bgG, bgB);
	ActTxtColor.ul.R = ulR; ActTxtColor.ul.G = ulG; ActTxtColor.ul.B = ulB;
	ActTxtColor.fg.R = fgR; ActTxtColor.fg.G = fgG; ActTxtColor.fg.B = fgB;
	ActTxtColor.bg.R = bgR; ActTxtColor.bg.G = bgR; ActTxtColor.bg.B = bgB; ActTxtColor.mode = 2; ActTxtStyle.pColor = NULL;
	printf("%s38;2;%d;%d;%d;48;2;%d;%d;%d;58;2;%d;%d;%dm", CSI, fgR, fgG, fgB, bgR, bgG, bgB, ulR, ulG, ulB);
}

// Set 256 Colors
void SetFg255(unsigned char c) {

	ActTxtColor.fg.R = c; ActTxtColor.mode = 1; ActTxtStyle.pColor = NULL;
	printf("%s38;5;%dm", CSI, c);
}
void SetBg255(unsigned char c) {

	ActTxtColor.bg.R = c; ActTxtColor.mode = 1; ActTxtStyle.pColor = NULL;
	printf("%s48;5;%dm", CSI, c);
}
void SetUl255(unsigned char c) {

	ActTxtColor.ul.R = c; ActTxtColor.mode = 1; ActTxtStyle.pColor = NULL;
	printf("%s58;5;%dm", CSI, c);
}
void SetFB255(unsigned char fg, unsigned char bg) {

	ActTxtColor.fg.R = fg;
	ActTxtColor.bg.R = bg; ActTxtColor.mode = 1; ActTxtStyle.pColor = NULL;
	printf("%s38;5;%d;48;5;%dm", CSI, fg, bg);
}
void SetFBU255(unsigned char fg, unsigned char bg, unsigned char ul) {

	ActTxtColor.fg.R = fg; ActTxtColor.ul.R = ul;
	ActTxtColor.bg.R = bg; ActTxtColor.mode = 1; ActTxtStyle.pColor = NULL;
	printf("%s38;5;%d;48;5;%d;58;5;%dm", CSI, fg, bg, ul);
}

//Set 16 Colors
void SetFg16(unsigned char c) {

	ActTxtColor.fg.R = c; ActTxtColor.mode = 0; ActTxtStyle.pColor = NULL;
	printf("%s%dm", CSI, c);
}
void SetBg16(unsigned char c) {
	
	ActTxtColor.bg.R = c; ActTxtColor.mode = 0; ActTxtStyle.pColor = NULL;
	//SetForeColor(c);					// just the value of c differs between fg and bg)
	printf("%s%dm", CSI, c);
}
void SetFB16(unsigned char fg, unsigned char bg) {

	ActTxtColor.fg.R = fg; ActTxtColor.bg.R = bg; ActTxtColor.mode = 0; ActTxtStyle.pColor = NULL;
	printf("%s%d;%dm", CSI, fg, bg);
}

// Reset To Default Colors
void ResFg(void) {

	ActTxtColor.fg.R = 39; ActTxtColor.mode = 0; ActTxtStyle.pColor = NULL;
	printf("%s39m", CSI);
}
void ResBg(void) {

	ActTxtColor.bg.R = 49; ActTxtColor.mode = 0; ActTxtStyle.pColor = NULL;
	printf("%s49m", CSI);
}
void ResUl(void) {

	ActTxtColor.ul.R = 59; ActTxtColor.mode = 0; ActTxtStyle.pColor = NULL;
	printf("%s59m", CSI);
}
void ResFB(void) {

	ActTxtColor.fg.R = 39; ActTxtColor.bg.R = 49;
	ActTxtColor.mode = 0; ActTxtStyle.pColor = 0;
	printf("%s39;49;59m", CSI);
}
void ResFBU(void) {

	ActTxtColor.fg.R = 39; ActTxtColor.bg.R = 49; ActTxtColor.ul.R = 59;
	ActTxtColor.mode = 0; ActTxtStyle.pColor = 0;
	printf("%s39;49;59m", CSI);
}

// TXT Styles - single commands
void TxtBold(unsigned char set) {

	ActTxtStyle.bold = set;
	ActTxtStyle.faint = 0;
	if (set) {
		// Set
		printf("%s1m", CSI);
	}
	else {
		// Reset
		printf("%s22m", CSI);
	}
}
void TxtFaint(_Bool set) {

	ActTxtStyle.faint = set;
	ActTxtStyle.bold = 0;
	if (set) {
		// Set
		printf("%s2m", CSI);
	}
	else {
		// Reset
		printf("%s22m", CSI);
	}
}
void TxtItalic(_Bool set) {
	
	ActTxtStyle.italic = set;
	if (set) {
		// Set
		printf("%s3m", CSI);
	}
	else {
		// Reset
		printf("%s23m", CSI);
	}
}
void TxtUnder(_Bool set) {

	ActTxtStyle.underline = set;
	if (set) {
		// Set
		printf("%s4m", CSI);
	}
	else {
		// Reset
		printf("%s%sm", CSI, RUL);
	}
}
void TxtDblUnder(_Bool set) {

	if (set) {
		// Set
		ActTxtStyle.underline = 2;
		printf("%s21m", CSI);
	}
	else {
		// Reset
		ActTxtStyle.underline = 0;
		printf("%s%sm", CSI, RUL);
	}
}
void TxtCurlUnder(_Bool set) {

	if (set) {
		// Set
		ActTxtStyle.underline = 3;
		printf("%s4:3m", CSI);
	}
	else {
		// Reset
		ActTxtStyle.underline = 0;
		printf("%s%sm", CSI, RUL);
	}
}
void TxtDotUnder(_Bool set) {

	if (set) {
		// Set
		ActTxtStyle.underline = 4;
		printf("%s4:4m", CSI);
	}
	else {
		// Reset
		ActTxtStyle.underline = 0;
		printf("%s%sm", CSI, RUL);
	}
}
void TxtDashUnder(_Bool set) {

	if (set) {
		// Set
		ActTxtStyle.underline = 5;
		printf("%s4:5m", CSI);
	}
	else {
		// Reset
		ActTxtStyle.underline = 0;
		printf("%s%sm", CSI, RUL);
	}
}
void TxtDashDotUnder(_Bool set) {

	// Undocumented DashDot - just a prognosis
	if (set) {
		// Set
		ActTxtStyle.underline = 6;
		printf("%s4:6m", CSI);
	}
	else {
		// Reset
		ActTxtStyle.underline = 0;
		printf("%s%sm", CSI, RUL);
	}
}
void TxtDblCurlUnder(_Bool set) {

	// Undocumented DoubleCurl - just a prognosis
	if (set) {
		// Set
		ActTxtStyle.underline = 7;
		printf("%s4:7m", CSI);
	}
	else {
		// Reset
		ActTxtStyle.underline = 0;
		printf("%s%sm", CSI, RUL);
	}
}
void TxtDblDotUnder(_Bool set) {

	// Undocumented DoubleDotted - just a prognosis
	if (set) {
		// Set
		ActTxtStyle.underline = 8;
		printf("%s4:8m", CSI);
	}
	else {
		// Reset
		ActTxtStyle.underline = 0;
		printf("%s%sm", CSI, RUL);
	}
}
void TxtDblDashUnder(_Bool set) {

	// Undocumented DoubleDashed - just a prognosis
	if (set) {
		// Set
		ActTxtStyle.underline = 9;
		printf("%s4:9m", CSI);
	}
	else {
		// Reset
		ActTxtStyle.underline = 0;
		printf("%s%sm", CSI, RUL);
	}
}
void TxtBlink(_Bool set) {

	ActTxtStyle.blink = set;
	ActTxtStyle.fast = 0;
	if (set) {
		// Set
		printf("%s5m", CSI);
	}
	else {
		// Reset
		printf("%s25m", CSI);
	}
}
void TxtFastBlink(_Bool set) {

	ActTxtStyle.fast = set;
	ActTxtStyle.blink = 0;
	if (set) {
		// Set
		printf("%s6m", CSI);
	}
	else {
		// Reset
		printf("%s25m", CSI);
	}
}
void TxtReverse(_Bool set) {

	ActTxtStyle.reverse = set;
	if (set) {
		// Set
		printf("%s7m", CSI);
	}
	else {
		// Reset
		printf("%s27m", CSI);
	}
}
void TxtInvisible(_Bool set) {

	ActTxtStyle.invisible = set;
	if (set) {
		// Set
		printf("%s8m", CSI);
	}
	else {
		// Reset
		printf("%s28m", CSI);
	}
}
void TxtStrike(_Bool set) {

	ActTxtStyle.strike = set;
	if (set) {
		// Set
		printf("%s9m", CSI);
	}
	else {
		// Reset
		printf("%s29m", CSI);
	}
}
void TxtSuperscript(_Bool set) {

	ActTxtStyle.superscript = set;
	ActTxtStyle.subscript = 0;
	if (set) {
		// Set
		printf("%s73m", CSI);
	}
	else {
		// Reset
		printf("%s75m", CSI);
	}
}
void TxtSubscript(_Bool set) {

	ActTxtStyle.subscript = set;
	ActTxtStyle.superscript = 0;
	if (set) {
		// Set
		printf("%s74m", CSI);
	}
	else {
		// Reset
		printf("%s75m", CSI);
	}
}
void TxtProportional(_Bool set){

	ActTxtStyle.proportional = set;
	if (set) {
		// Set
		printf("%s26m", CSI);
	}
	else {
		// Reset
		printf("%s50m", CSI);
	}
}
void TxtFramed(_Bool set) {

	ActTxtStyle.framed = set;
	ActTxtStyle.encircled = 0;
	if (set) {
		// Set
		printf("%s51m", CSI);
	}
	else {
		// Reset
		printf("%s54m", CSI);
	}
}
void TxtEncircled(_Bool set) {

	ActTxtStyle.encircled = set;
	ActTxtStyle.framed = 0;
	if (set) {
		// Set
		printf("%s52m", CSI);
	}
	else {
		// Reset
		printf("%s54m", CSI);
	}
}
void TxtOverline(_Bool set) {

	ActTxtStyle.overline = set;
	if (set) {
		// Set
		printf("%s53m", CSI);
	}
	else {
		// Reset
		printf("%s55m", CSI);
	}
}
void TxtUnderRight(void) {

	ActTxtStyle.under_right = 1;
	printf("%s60m", CSI);
}
void TxtUnderDbl(void) {

	ActTxtStyle.under_dbl = 1;
	printf("%s61m", CSI);
}
void TxtOverLeft(void) {

	ActTxtStyle.over_left = 1;
	printf("%s62m", CSI);
}
void TxtOverDbl(void) {

	ActTxtStyle.over_dbl = 1;
	printf("%s63m", CSI);
}
void TxtStress(void) {

	ActTxtStyle.stress = 1;
	printf("%s64m", CSI);
}
void TxtOffUOS(void) {

	ActTxtStyle.under_right = 0; ActTxtStyle.under_dbl = 0;
	ActTxtStyle.over_left = 0; ActTxtStyle.over_dbl = 0;
	ActTxtStyle.stress = 0;
	printf("%s65m", CSI);
}
void TxtDblTop(_Bool set) {

	// DoubleHeight + DoubleWidth Top-Part
	ActTxtStyle.dbl_height = set;
	ActTxtStyle.dbl_width = 0;
	if (set) {
		// Set
		printf("%s3", FP3);
	}
	else {
		// Reset
		printf("%s5", CSI);
	}
}
void TxtDblBot(_Bool set) {

	// DoubleHeight + DoubleWidth Bottom-Part
	ActTxtStyle.dbl_height = set;
	ActTxtStyle.dbl_width = 0;
	if (set) {
		// Set
		printf("%s4", FP3);
	}
	else {
		// Reset
		printf("%s5", CSI);
	}
}
void TxtDblWidth(_Bool set) {

	// DoubleWidth 
	ActTxtStyle.dbl_width = set;
	ActTxtStyle.dbl_height = 0;
	if (set) {
		// Set
		printf("%s6", FP3);
	}
	else {
		// Reset
		printf("%s5", CSI);
	}
}
void TxtFont(unsigned char fnt) {

	// 10 = Default Font
	// 11 - 19 User Fonts @INVESTIGATE@
	// 20 = Fraktur

	if (fnt < 10 || fnt > 20 ) { fnt = 10; }
	ActTxtStyle.font = fnt;
	printf("%s%dm", CSI, fnt);
}

// TXT Style - combined from Structure
void SetTxtStyle(TxtStyleSTRUCT *pTxtStyle, _Bool set) {

	// set == False -> Reset all to default

	// ActTxtStyle containing the 'last' state and is updated after return

	char strOut[256]; strcat(strOut, CSI);
	_Bool changed = 0;

	if (set) {
		if (pTxtStyle->bold != ActTxtStyle.bold) {

			changed = 1;
			ActTxtStyle.bold = pTxtStyle->bold;
			ActTxtStyle.faint = 0;
			if (ActTxtStyle.bold) {
				strcat(strOut, "1;");
			}
			else {
				strcat(strOut, "22;");
			}
		}
		if (pTxtStyle->faint != ActTxtStyle.faint) {

			changed = 1;
			ActTxtStyle.faint = pTxtStyle->faint;
			ActTxtStyle.bold = 0;
			if (ActTxtStyle.faint) {
				strcat(strOut, "2;");
			}
			else {
				strcat(strOut, "22;");
			}
		}
		if (pTxtStyle->italic != ActTxtStyle.italic) {

			changed = 1;
			ActTxtStyle.italic = pTxtStyle->italic;
			if (ActTxtStyle.italic) {
				strcat(strOut, "3;");
			}
			else {
				strcat(strOut, "23;");
			}
		}
		if (pTxtStyle->underline != ActTxtStyle.underline) {

			changed = 1;
			ActTxtStyle.underline = pTxtStyle->underline;
			switch (ActTxtStyle.underline) {
			case 0: // None
				strcat(strOut, RUL);
				strcat(strOut, ";");
				break;
			case 1: // Single
				strcat(strOut, "4;"); break;
			case 2: // Double
				strcat(strOut, "21;"); break;
			case 3: // Curly
				strcat(strOut, "4:3;"); break;
			case 4: // Dot
				strcat(strOut, "4:4;"); break;
			case 5: // Dash
				strcat(strOut, "4:5;"); break;
			case 6: // DashDot
				strcat(strOut, "4:6;"); break;
			case 7: // Double Curly
				strcat(strOut, "4:7;"); break;
			case 8: // Double Dot
				strcat(strOut, "4:8;"); break;
			case 9: // Double Dash
				strcat(strOut, "4:9;"); break;
			default:
				// This would be an error !!
				strcat(strOut, RUL);
				strcat(strOut, ";");
				pTxtStyle->underline = 0;
				ActTxtStyle.underline = 0;
				break;
			}
		}
		if (pTxtStyle->blink != ActTxtStyle.blink) {

			changed = 1;
			ActTxtStyle.blink = pTxtStyle->blink;
			ActTxtStyle.fast = 0;
				if (ActTxtStyle.blink) {
					strcat(strOut, "5;");
				}
				else {
					strcat(strOut, "25;");
				}
		}
		if (pTxtStyle->fast != ActTxtStyle.fast) {

			changed = 1;
			ActTxtStyle.fast = pTxtStyle->fast;
			ActTxtStyle.blink = 0;
				if (ActTxtStyle.fast) {
					strcat(strOut, "6;");
				}
				else {
					strcat(strOut, "25;");
				}
		}
		if (pTxtStyle->reverse != ActTxtStyle.reverse) {

			changed = 1;
			ActTxtStyle.reverse = pTxtStyle->reverse;
			if (ActTxtStyle.reverse) {
				strcat(strOut, "7;");
			}
			else {
				strcat(strOut, "27;");
			}
		}
		if (pTxtStyle->invisible != ActTxtStyle.invisible) {

			changed = 1;
			ActTxtStyle.invisible = pTxtStyle->invisible;
			if (ActTxtStyle.invisible) {
				strcat(strOut, "8;");
			}
			else {
				strcat(strOut, "28;");
			}
		}
		if (pTxtStyle->strike != ActTxtStyle.strike) {

			changed = 1;
			ActTxtStyle.strike = pTxtStyle->strike;
			if (ActTxtStyle.strike) {
				strcat(strOut, "9;");
			}
			else {
				strcat(strOut, "29;");
			}
		}
		if (pTxtStyle->superscript != ActTxtStyle.superscript) {

			changed = 1;
			ActTxtStyle.superscript = pTxtStyle->superscript;
			ActTxtStyle.subscript = 0;
				if (ActTxtStyle.superscript) {
					strcat(strOut, "73;");
				}
				else {
					strcat(strOut, "75;");
				}
		}
		if (pTxtStyle->subscript != ActTxtStyle.subscript) {

			changed = 1;
			ActTxtStyle.subscript = pTxtStyle->subscript;
			ActTxtStyle.superscript = 0;
				if (ActTxtStyle.subscript) {
					strcat(strOut, "74;");
				}
				else {
					strcat(strOut, "75;");
				}
		}
		if (pTxtStyle->proportional != ActTxtStyle.proportional) {

			changed = 1;
			ActTxtStyle.proportional = pTxtStyle->proportional;
			if (ActTxtStyle.proportional) {
				strcat(strOut, "26;");
			}
			else {
				strcat(strOut, "50;");
			}
		}
		if (pTxtStyle->framed != ActTxtStyle.framed) {

			changed = 1;
			ActTxtStyle.framed = pTxtStyle->framed;
			ActTxtStyle.encircled = 0;
				if (ActTxtStyle.framed) {
					strcat(strOut, "51;");
				}
				else {
					strcat(strOut, "54;");
				}
		}
		if (pTxtStyle->encircled != ActTxtStyle.encircled) {

			changed = 1;
			ActTxtStyle.encircled = pTxtStyle->encircled;
			ActTxtStyle.framed = 0;
				if (ActTxtStyle.encircled) {
					strcat(strOut, "52;");
				}
				else {
					strcat(strOut, "54;");
				}
		}
		if (pTxtStyle->overline != ActTxtStyle.overline) {

			changed = 1;
			ActTxtStyle.overline = pTxtStyle->overline;
			if (ActTxtStyle.overline) {
				strcat(strOut, "53;");
			}
			else {
				strcat(strOut, "55;");
			}
		}
		if ((pTxtStyle->under_right != ActTxtStyle.under_right) ||
			(pTxtStyle->under_dbl != ActTxtStyle.under_dbl) ||
			(pTxtStyle->over_left != ActTxtStyle.over_left) ||
			(pTxtStyle->over_dbl != ActTxtStyle.over_dbl) ||
			(pTxtStyle->stress != ActTxtStyle.stress)) {

			changed = 1;
			strcat(strOut, "65;");		// 1st... Reset them all...

			ActTxtStyle.under_right = pTxtStyle->under_right;
			if (ActTxtStyle.under_right) {
				strcat(strOut, "60;");
			}
			ActTxtStyle.under_dbl = pTxtStyle->under_dbl;
			if (ActTxtStyle.under_dbl) {
				strcat(strOut, "61;");
			}
			ActTxtStyle.over_left = pTxtStyle->over_left;
			if (ActTxtStyle.over_left) {
				strcat(strOut, "62;");
			}
			ActTxtStyle.over_dbl = pTxtStyle->over_dbl;
			if (ActTxtStyle.over_dbl) {
				strcat(strOut, "63;");
			}
			ActTxtStyle.stress = pTxtStyle->stress;
			if (ActTxtStyle.stress) {
				strcat(strOut, "64;");
			}
		}
		if (pTxtStyle->font != ActTxtStyle.font) {

			changed = 1;
			if (pTxtStyle->font < 10 || pTxtStyle->font > 20) {
				pTxtStyle->font = 10;
			}
			ActTxtStyle.font = pTxtStyle->font;
			char buffer[3];
			snprintf(buffer, 3, "%d;", ActTxtStyle.font);
			strcat(strOut, buffer);
		}
		if (pTxtStyle->pColor != ActTxtStyle.pColor) {

			changed = 1;
			char buffer[18];
			switch (pTxtStyle->pColor->mode) {
			case 0:		// 16 (has no UnderLineColor)
				if (pTxtStyle->pColor->fg.R != ActTxtColor.fg.R) {
					unsigned char fg = pTxtStyle->pColor->fg.R;
					if ((fg < 30 || fg > 97) || (fg > 37 && fg < 90)) {
						fg = 39;	// ERR, so we set default
						pTxtStyle->pColor->fg.R = 39;
					}
					snprintf(buffer, 18, "%d;", fg);
					strcat(strOut, buffer);
					ActTxtColor.fg.R = fg;
				}
				if (pTxtStyle->pColor->bg.R != ActTxtColor.bg.R) {
					unsigned char bg = pTxtStyle->pColor->bg.R;
					if ((bg < 40 || bg > 107) || (bg > 47 && bg < 100)) {
						bg = 49;	// ERR, so we set default
						pTxtStyle->pColor->bg.R = 49;
					}
					snprintf(buffer, 18, "%d;", bg);
					strcat(strOut, buffer);
					ActTxtColor.bg.R = bg;
				}
				break;
			
			case 1:		// 255
				if (pTxtStyle->pColor->fg.R != ActTxtColor.fg.R) {
					unsigned char fg = pTxtStyle->pColor->fg.R;
					snprintf(buffer, 18, "38;5;%d;", fg);
					strcat(strOut, buffer);
					ActTxtColor.fg.R = fg;
				}
				if (pTxtStyle->pColor->bg.R != ActTxtColor.bg.R) {
					unsigned char bg = pTxtStyle->pColor->bg.R;
					snprintf(buffer, 18, "48;5;%d;", bg);
					strcat(strOut, buffer);
					ActTxtColor.bg.R = bg;
				}
				if (pTxtStyle->pColor->ul.R != ActTxtColor.ul.R) {
					unsigned char ul = pTxtStyle->pColor->ul.R;
					snprintf(buffer, 18, "58;5;%d;", ul);
					strcat(strOut, buffer);
					ActTxtColor.ul.R = ul;
				}
				break;
			
			case 2:		// RGB
				if (pTxtStyle->pColor->fg.Color != ActTxtColor.fg.Color) {
					ActTxtColor.fg.Color = pTxtStyle->pColor->fg.Color;
					snprintf(buffer, 18, "38;2;%d;%d;%d;", ActTxtColor.fg.R, ActTxtColor.fg.G, ActTxtColor.fg.B);
					strcat(strOut, buffer);
				}
				if (pTxtStyle->pColor->bg.Color != ActTxtColor.bg.Color) {
					ActTxtColor.bg.Color = pTxtStyle->pColor->bg.Color;
					snprintf(buffer, 18, "48;2;%d;%d;%d;", ActTxtColor.bg.R, ActTxtColor.bg.G, ActTxtColor.bg.B);
					strcat(strOut, buffer);
				}
				if (pTxtStyle->pColor->ul.Color != ActTxtColor.ul.Color) {
					ActTxtColor.ul.Color = pTxtStyle->pColor->ul.Color;
					snprintf(buffer, 18, "58;2;%d;%d;%d;", ActTxtColor.ul.R, ActTxtColor.ul.G, ActTxtColor.ul.B);
					strcat(strOut, buffer);
				}
				break;
			}
			ActTxtStyle.pColor = pTxtStyle->pColor;
		}
		if (changed) {
			// replace last ';' with 'm' (CSI/SGI termination)
			int last = strlen(strOut) - 1;
			strOut[last] = 0x6D;
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
