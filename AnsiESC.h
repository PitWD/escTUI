
//		   ANSI-ESC - Terminal Functions - See detailed description at the EOF !
//				! Using 3rd party MIT/GPL/"FreeSoftware" technologies !
//				  (see related declarations/functions and at the EOF)
// This is a  ! p_o_o_r !  Header by (c) Pit Demmer. (PIT-Licensed 01.04.2022 -           )

#define AnsiESCVersion "1.00pa"

// WIN & Linux
// #include "Terminal.h"
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "ini.h"

typedef struct {
	int mode;			// 0 = B/W, 1 = 16, 2 = 255, 3 = RGB
	union ColorFG {
		uint32_t Color;	// !! 32 bit !!
		int dummy;	// ZERO - Then direct int32 -> Hex will work for RGB
		int R;
		int G;
		int B;
	}fg;						// ForeGround (16 & 255 on fg.R)
	union ColorBG {
		uint32_t Color;
		int dummy;					
		int R;
		int G;
		int B;
	}bg;						// BackGround
	union ColorUL {
		uint32_t Color;	// UL - color just relevant in modes 1 & 2 (in mode '0': 'ul' == 'fg')
		int dummy;
		int R;					
		int G;
		int B;
	}ul;						// Underline
} EscColorSTRUCT;

typedef struct {
	/*		Actual State in [0]
			Default in [1]
			[2] - [n] free for user
	*/
	// 5 int
	int bold				:1;
	int faint				:1;
	int italic			:1;
	int blink				:1;
	int fast				:1;
	int reverse			:1;
	int invisible			:1;
	int strike			:1;

	int superscript		:1;
	int subscript			:1;
	int proportional		:1;
	int framed			:1;
	int encircled			:1;
	int overline			:1;
	int ideo_right		:1;		// CSI60m (Ideogram 'rarely supported')
	int ideo_dbl_right	:1;		// CSI61m ( "    "    "   "   "     "  )
	
	int ideo_left			:1;		// CSI62m ( "    "    "   "   "     "  )
	int ideo_dbl_left		:1;		// CSI63m ( "    "    "   "   "     "  )
	int ideo_stress		:1;		// CSI64m ( "    "    "   "   "     "  )
	int dbl_width			:1;
	int dbl_height		:1;
	int dummy1			:1;		// Fill the 3rd int
	int dummy2			:1;		// ( "   "   "   " ) 
	int dummy3			:1;		// ( "   "   "   " )
	
	int font				:4;		// CSI10m (Standard = 0) - CSI20m (Fraktur = 10) 
	int underline			:4;		// 0 = None, 1 = single, 2 = double, 3 = curl, 4 = dot, 5 = dash 
											// 6 = dashdot, 7 = dbl_curl, 8 = dbl_dot, 9 = dbl_dash
	EscColorSTRUCT *pColor;
} EscStyleSTRUCT; 

// Standard, but optional, ESC Sequences
char RUL[] = "24";				// 'ResetUnderLine' alternativ: "4:0" 

// Actual Settings / States
static EscStyleSTRUCT ActTxtStyle;
static EscColorSTRUCT ActTxtColor;

void ResFBU(void);
void SetColorStyle(EscColorSTRUCT *pColor, int set);
void LocateX(int x);

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
int ESC_Grey[27];		// initialization in InitColors

// Solarized Colors from Ethan Schoonover : 	github.com/altercation/solarized
// D(ark) & B(right) versions from Pit
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

void ESCinit(void) {

	// The 24 Greys (+ Black/White) from 256 Palette	
	ESC_Grey[1] = 15; ESC_Grey[26] = 0;
	int p = 1;
	for (int t = 255; t > 231; t--) {
		p++;
		ESC_Grey[p] = t;
	}

	// Default ColorStyle
	ActTxtStyle.pColor = &ActTxtColor;

	// Users Colors
	char strSearch[STR_SMALL_SIZE];
	char strGroupName[STR_SMALL_SIZE];
	char strColorName[STR_SMALL_SIZE];

	char strFile[] = "desktops.ini";
	int colorsGroupsCount = IniGetInt(strFile, "global.colors.GroupCount", 0);
	int colorsModel = IniGetInt(strFile, "global.colors.ColorModel", 0);
	int colorsCount[colorsGroupsCount];
	int colorsCountSum = 0;

	for (int i = 0; i < colorsGroupsCount; i++){
		sprintf(strSearch, "global.colors.group%d.Count", i + 1);
		colorsCount[i] = IniGetInt(strFile, strSearch, 0);
		colorsCountSum += colorsCount[i];
	}
	
	EscColorSTRUCT UserColor[colorsCountSum];
	colorsCountSum = 0;

	for (int i = 0; i < colorsGroupsCount; i++){
		sprintf(strSearch, "global.colors.group%d.Name", i + 1);
		IniGetStr(strFile, strSearch, "NoGroupName", strGroupName);
		for (int j = 0; j < colorsCount[i]; j++){

			sprintf(strSearch, "global.colors.group%d.%d.Name", i + 1, j + 1);
			IniGetStr(strFile, strSearch, "NoColorName", strColorName);

			sprintf(strSearch, "global.colors.group%d.%d.ForeGround", i + 1, j + 1);
			UserColor[colorsCountSum].fg.Color = (int)IniGetInt(strFile, strSearch, 15);
			sprintf(strSearch, "global.colors.group%d.%d.BackGround", i + 1, j + 1);
			UserColor[colorsCountSum].bg.Color = (int)IniGetInt(strFile, strSearch, 0);
			sprintf(strSearch, "global.colors.group%d.%d.UnderLine", i + 1, j + 1);
			UserColor[colorsCountSum].ul.Color = (int)IniGetInt(strFile, strSearch, 15);
			
			UserColor[colorsCountSum].mode = colorsModel;

			colorsCountSum++;

			
			printf("%04d. %s_%s: ",colorsCountSum, strGroupName, strColorName);
			LocateX(30);
			SetColorStyle(&UserColor[colorsCountSum - 1], 1);
			//printf("Res - Done\n");
			//return;
			printf("->   Some obligatory text :-)   <-");
			ResFBU();
			printf("\n");
		}
		printf("\n");
	}
}


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
	ActTxtColor.fg.Color = 39; ActTxtColor.mode = 1; ActTxtStyle.pColor = &ActTxtColor;
	printf("\x1B[39m");
}
void ResBg(void) {
	ActTxtColor.bg.Color = 49; ActTxtColor.mode = 1; ActTxtStyle.pColor = &ActTxtColor;
	printf("\x1B[49m");
}
void ResUl(void) {
	ActTxtColor.ul.Color = 59; ActTxtColor.mode = 1; ActTxtStyle.pColor = &ActTxtColor;
	printf("\x1B[59m");
}
void ResFB(void) {
	ActTxtColor.fg.R = 39; ActTxtColor.bg.R = 49;
	ActTxtColor.mode = 0; ActTxtStyle.pColor = &ActTxtColor;
	printf("\x1B[39;49");
}
void ResFBU(void) {
	ActTxtColor.fg.R = 39; ActTxtColor.bg.R = 49; ActTxtColor.ul.R = 59;
	ActTxtColor.mode = 0; ActTxtStyle.pColor = &ActTxtColor;
	printf("\x1B[39;49;59m");
}

// Set 24 bit Color
void SetFgRGB(int r, int g, int b) {
	ActTxtStyle.pColor->fg.R = r; ActTxtStyle.pColor->fg.G = g; ActTxtStyle.pColor->fg.B = b; ActTxtStyle.pColor->mode = 3;
	printf("\x1B[38;2;%d;%d;%dm", r, g, b);
}
void SetBgRGB(int r, int g, int b) {
	ActTxtStyle.pColor->bg.R = r; ActTxtStyle.pColor->bg.G = g; ActTxtStyle.pColor->bg.B = b; ActTxtStyle.pColor->mode = 3;
	printf("\x1B[48;2;%d;%d;%dm", r, g, b);
}
void SetUlRGB(int r, int g, int b) {
	ActTxtStyle.pColor->ul.R = r; ActTxtStyle.pColor->ul.G = g; ActTxtStyle.pColor->ul.B = b; ActTxtStyle.pColor->mode = 3;
	printf("\x1B[58;2;%d;%d;%dm", r, g, b);
}
void SetFBrgb(int fgR, int fgG, int fgB, int bgR, int bgG, int bgB) {
	SetFgRGB(fgR, fgG, fgB);
	SetBgRGB(bgR, bgG, bgB);
}
void SetFBUrgb(int fgR, int fgG, int fgB, int bgR, int bgG, int bgB, int ulR, int ulG, int ulB) {
	SetFgRGB(fgR, fgG, fgB);
	SetBgRGB(bgR, bgG, bgB);
	SetUlRGB(ulR, ulG, ulB);
}

// Set 256 Colors
void SetFg255(int c) {
	ActTxtStyle.pColor->fg.Color = c; ActTxtStyle.pColor->mode = 2;
	printf("\x1B[38;5;%dm", c);
}
void SetBg255(int c) {
	ActTxtStyle.pColor->bg.Color = c; ActTxtStyle.pColor->mode = 2;
	printf("\x1B[48;5;%dm", c);
}
void SetUl255(int c) {
	ActTxtStyle.pColor->ul.Color = c; ActTxtStyle.pColor->mode = 2;
	printf("\x1B[58;5;%dm", c);
}
void SetFB255(int fg, int bg) {
	SetFg255(fg);
	SetBg255(bg);
}
void SetFBU255(int fg, int bg, int ul) {
	SetFg255(fg);
	SetBg255(bg);
	SetUl255(ul);
}

//Set 16 Colors
void SetFg16(int c){
	// ForeGround	
	if (!((c > 29 && c < 38) || (c > 89 && c < 98))){
		ResFg();
	}
	else{
		ActTxtStyle.pColor->fg.Color = c; ActTxtStyle.pColor->mode = 0;
		printf("\x1B[%dm", c);
	}	
}
void SetBg16(int c) {	
	// BackGround
	if (!((c > 39 && c < 48) || (c > 99 && c < 108))){
		ResBg();
	}
	else{
		ActTxtStyle.pColor->bg.Color = c; ActTxtStyle.pColor->mode = 0;
		printf("\x1B[%dm", c);
	}
}
void SetFB16(int fg, int bg) {
	SetFg16(fg);
	SetBg16(bg);
}

// TXT Styles - single commands
void TxtBold(int set) {
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
	printf("\x1B[%dm", fnt + 10);
}

void SetColorStyle(EscColorSTRUCT *pColor, int set){

	if (set){
		switch (pColor->mode) {
		case 1:		// 16 (has no UnderLineColor)
			if (pColor->fg.Color != ActTxtColor.fg.Color) {
				if (!((pColor->fg.Color > 29 && pColor->fg.Color < 38) || (pColor->fg.Color > 89 && pColor->fg.Color < 98))){
					pColor->fg.Color = 39;
					ResFg();
				}
				else{
					SetFg16(pColor->fg.Color);
				}
			}
			if (pColor->bg.Color != ActTxtColor.bg.Color) {
				if (!((pColor->bg.Color > 39 && pColor->bg.Color < 48) || (pColor->bg.Color > 99 && pColor->bg.Color < 108))){
					pColor->bg.Color = 49;
					ResBg();
				}
				else{
					SetBg16(pColor->bg.Color);
				}
			}
			break;
		
		case 2:		// 255
			if (pColor->fg.Color != ActTxtColor.fg.Color) {
				SetFg255(pColor->fg.Color);
			}
			if (pColor->bg.Color != ActTxtColor.bg.Color) {
				SetBg255(pColor->bg.Color);
			}
			if (pColor->ul.Color != ActTxtColor.ul.Color) {
				//SetUl255(pColor->ul.Color);
			}
			break;
		
		case 3:		// RGB
			if (pColor->fg.Color != ActTxtColor.fg.Color) {
				SetFgRGB(pColor->fg.R, pColor->fg.G, pColor->fg.B);
			}
			if (pColor->bg.Color != ActTxtColor.bg.Color) {
				SetBgRGB(pColor->bg.R, pColor->bg.G, pColor->bg.B);
			}
			if (pColor->ul.Color != ActTxtColor.ul.Color) {
				SetUlRGB(pColor->ul.R, pColor->ul.G, pColor->ul.B);
			}
			break;
		}
	}
	else{
		memset(&ActTxtColor, 0, sizeof(ActTxtColor));
		ResFBU();
	}
}

// TXT Style - combined from Structure
void SetTxtStyle(EscStyleSTRUCT *pTxtStyle, int set) {

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

		SetColorStyle(pTxtStyle->pColor, 1);
		
	}
	else {
		// Reset colors, fonts and styles to their defaults
		memset(&ActTxtStyle, 0, sizeof(ActTxtStyle));
		memset(&ActTxtColor, 0, sizeof(ActTxtColor));
		printf("\x1B[10;0;39;49;59m");
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
