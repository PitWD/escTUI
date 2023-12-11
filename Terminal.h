
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
#include <string.h>
#include <wchar.h>

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
int TERM_CursorPosXsave = 0;				// CursorPos Save / Restore
int TERM_CursorPosYsave = 0;				// CursorPos Save / Restore
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
int TermIsWaitingForESC27 = 0;

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

typedef struct {
	int mode;			// 0 = B/W, 1 = 16, 2 = 255, 3 = RGB
	char *groupName;
	char *colorName;
	int groupID;
	int colorID;
	union ColorFG {
		uint32_t Color;	// !! 32 bit !!
		uint8_t dummy;	// ZERO - Then direct int32 -> Hex will work for RGB
		uint8_t R;
		uint8_t G;
		uint8_t B;
	}fg;						// ForeGround
	union ColorBG {
		uint32_t Color;
		uint8_t dummy;					
		uint8_t R;
		uint8_t G;
		uint8_t B;
	}bg;						// BackGround
	union ColorUL {
		uint32_t Color;	// UL - color just relevant in modes 1 & 2 (in mode '0': 'ul' == 'fg')
		uint8_t dummy;
		uint8_t R;					
		uint8_t G;
		uint8_t B;
	}ul;						// Underline
} EscColorSTRUCT;

typedef struct {
	/*		Actual State in [0]
			Default in [1]
			[2] - [n] free for user
	*/
	// 5 int
	char *fontName;
	char *styleName;
	int fontID;
	int styleID;
	uint32_t bold				:1;
	uint32_t faint				:1;
	uint32_t italic			:1;
	uint32_t blink				:1;
	uint32_t fast				:1;
	uint32_t reverse			:1;
	uint32_t invisible			:1;
	uint32_t strike			:1;

	uint32_t superscript		:1;
	uint32_t subscript			:1;
	uint32_t proportional		:1;
	uint32_t framed			:1;
	uint32_t encircled			:1;
	uint32_t overline			:1;
	uint32_t ideo_right		:1;		// CSI60m (Ideogram 'rarely supported')
	uint32_t ideo_dbl_right	:1;		// CSI61m ( "    "    "   "   "     "  )
	
	uint32_t ideo_left			:1;		// CSI62m ( "    "    "   "   "     "  )
	uint32_t ideo_dbl_left		:1;		// CSI63m ( "    "    "   "   "     "  )
	uint32_t ideo_stress		:1;		// CSI64m ( "    "    "   "   "     "  )
	uint32_t dbl_width			:1;
	uint32_t dbl_height		:1;
	uint32_t dummy1			:1;		// Fill the 3rd int
	uint32_t dummy2			:1;		// ( "   "   "   " ) 
	uint32_t dummy3			:1;		// ( "   "   "   " )
	
	uint32_t font				:4;		// CSI10m (Standard = 0) - CSI20m (Fraktur = 10) 
	uint32_t underline			:4;		// 0 = None, 1 = single, 2 = double, 3 = curl, 4 = dot, 5 = dash 
											// 6 = dashdot, 7 = dbl_curl, 8 = dbl_dot, 9 = dbl_dash
	// EscColorSTRUCT *pColor;
} EscStyleSTRUCT; 

struct CanvasSTRUCT {
    EscColorSTRUCT *color;
    EscStyleSTRUCT *style;
    wchar_t c;
    int width;
}***myCanvas;
int canvasMaxX = 0;
int canvasMaxY = 0;
#define canvasMaxZ 4
#define CANVAS_Term 0
#define CANVAS_TermNext 1
#define CANVAS_Screen 2
#define CANVAS_Menu 3

// *** 3rd Party START ***
struct width_interval {
	// from https://github.com/PitWD/termux_wcwidth
	// which is a copy of https://github.com/termux/wcwidth
	// all MIT licensed
	int start;
	int end;
};

static struct width_interval ZERO_WIDTH[] = {
	// from https://github.com/PitWD/termux_wcwidth
	// which is a copy of https://github.com/termux/wcwidth
	// all MIT licensed
	{0x00300, 0x0036f},  // Combining Grave Accent  ..Combining Latin Small Le
	{0x00483, 0x00489},  // Combining Cyrillic Titlo..Combining Cyrillic Milli
	{0x00591, 0x005bd},  // Hebrew Accent Etnahta   ..Hebrew Point Meteg
	{0x005bf, 0x005bf},  // Hebrew Point Rafe       ..Hebrew Point Rafe
	{0x005c1, 0x005c2},  // Hebrew Point Shin Dot   ..Hebrew Point Sin Dot
	{0x005c4, 0x005c5},  // Hebrew Mark Upper Dot   ..Hebrew Mark Lower Dot
	{0x005c7, 0x005c7},  // Hebrew Point Qamats Qata..Hebrew Point Qamats Qata
	{0x00610, 0x0061a},  // Arabic Sign Sallallahou ..Arabic Small Kasra
	{0x0064b, 0x0065f},  // Arabic Fathatan         ..Arabic Wavy Hamza Below
	{0x00670, 0x00670},  // Arabic Letter Superscrip..Arabic Letter Superscrip
	{0x006d6, 0x006dc},  // Arabic Small High Ligatu..Arabic Small High Seen
	{0x006df, 0x006e4},  // Arabic Small High Rounde..Arabic Small High Madda
	{0x006e7, 0x006e8},  // Arabic Small High Yeh   ..Arabic Small High Noon
	{0x006ea, 0x006ed},  // Arabic Empty Centre Low ..Arabic Small Low Meem
	{0x00711, 0x00711},  // Syriac Letter Superscrip..Syriac Letter Superscrip
	{0x00730, 0x0074a},  // Syriac Pthaha Above     ..Syriac Barrekh
	{0x007a6, 0x007b0},  // Thaana Abafili          ..Thaana Sukun
	{0x007eb, 0x007f3},  // Nko Combining Short High..Nko Combining Double Dot
	{0x007fd, 0x007fd},  // Nko Dantayalan          ..Nko Dantayalan
	{0x00816, 0x00819},  // Samaritan Mark In       ..Samaritan Mark Dagesh
	{0x0081b, 0x00823},  // Samaritan Mark Epentheti..Samaritan Vowel Sign A
	{0x00825, 0x00827},  // Samaritan Vowel Sign Sho..Samaritan Vowel Sign U
	{0x00829, 0x0082d},  // Samaritan Vowel Sign Lon..Samaritan Mark Nequdaa
	{0x00859, 0x0085b},  // Mandaic Affrication Mark..Mandaic Gemination Mark
	{0x00898, 0x0089f},  // Arabic Small High Word A..Arabic Half Madda Over M
	{0x008ca, 0x008e1},  // Arabic Small High Farsi ..Arabic Small High Sign S
	{0x008e3, 0x00902},  // Arabic Turned Damma Belo..Devanagari Sign Anusvara
	{0x0093a, 0x0093a},  // Devanagari Vowel Sign Oe..Devanagari Vowel Sign Oe
	{0x0093c, 0x0093c},  // Devanagari Sign Nukta   ..Devanagari Sign Nukta
	{0x00941, 0x00948},  // Devanagari Vowel Sign U ..Devanagari Vowel Sign Ai
	{0x0094d, 0x0094d},  // Devanagari Sign Virama  ..Devanagari Sign Virama
	{0x00951, 0x00957},  // Devanagari Stress Sign U..Devanagari Vowel Sign Uu
	{0x00962, 0x00963},  // Devanagari Vowel Sign Vo..Devanagari Vowel Sign Vo
	{0x00981, 0x00981},  // Bengali Sign Candrabindu..Bengali Sign Candrabindu
	{0x009bc, 0x009bc},  // Bengali Sign Nukta      ..Bengali Sign Nukta
	{0x009c1, 0x009c4},  // Bengali Vowel Sign U    ..Bengali Vowel Sign Vocal
	{0x009cd, 0x009cd},  // Bengali Sign Virama     ..Bengali Sign Virama
	{0x009e2, 0x009e3},  // Bengali Vowel Sign Vocal..Bengali Vowel Sign Vocal
	{0x009fe, 0x009fe},  // Bengali Sandhi Mark     ..Bengali Sandhi Mark
	{0x00a01, 0x00a02},  // Gurmukhi Sign Adak Bindi..Gurmukhi Sign Bindi
	{0x00a3c, 0x00a3c},  // Gurmukhi Sign Nukta     ..Gurmukhi Sign Nukta
	{0x00a41, 0x00a42},  // Gurmukhi Vowel Sign U   ..Gurmukhi Vowel Sign Uu
	{0x00a47, 0x00a48},  // Gurmukhi Vowel Sign Ee  ..Gurmukhi Vowel Sign Ai
	{0x00a4b, 0x00a4d},  // Gurmukhi Vowel Sign Oo  ..Gurmukhi Sign Virama
	{0x00a51, 0x00a51},  // Gurmukhi Sign Udaat     ..Gurmukhi Sign Udaat
	{0x00a70, 0x00a71},  // Gurmukhi Tippi          ..Gurmukhi Addak
	{0x00a75, 0x00a75},  // Gurmukhi Sign Yakash    ..Gurmukhi Sign Yakash
	{0x00a81, 0x00a82},  // Gujarati Sign Candrabind..Gujarati Sign Anusvara
	{0x00abc, 0x00abc},  // Gujarati Sign Nukta     ..Gujarati Sign Nukta
	{0x00ac1, 0x00ac5},  // Gujarati Vowel Sign U   ..Gujarati Vowel Sign Cand
	{0x00ac7, 0x00ac8},  // Gujarati Vowel Sign E   ..Gujarati Vowel Sign Ai
	{0x00acd, 0x00acd},  // Gujarati Sign Virama    ..Gujarati Sign Virama
	{0x00ae2, 0x00ae3},  // Gujarati Vowel Sign Voca..Gujarati Vowel Sign Voca
	{0x00afa, 0x00aff},  // Gujarati Sign Sukun     ..Gujarati Sign Two-circle
	{0x00b01, 0x00b01},  // Oriya Sign Candrabindu  ..Oriya Sign Candrabindu
	{0x00b3c, 0x00b3c},  // Oriya Sign Nukta        ..Oriya Sign Nukta
	{0x00b3f, 0x00b3f},  // Oriya Vowel Sign I      ..Oriya Vowel Sign I
	{0x00b41, 0x00b44},  // Oriya Vowel Sign U      ..Oriya Vowel Sign Vocalic
	{0x00b4d, 0x00b4d},  // Oriya Sign Virama       ..Oriya Sign Virama
	{0x00b55, 0x00b56},  // Oriya Sign Overline     ..Oriya Ai Length Mark
	{0x00b62, 0x00b63},  // Oriya Vowel Sign Vocalic..Oriya Vowel Sign Vocalic
	{0x00b82, 0x00b82},  // Tamil Sign Anusvara     ..Tamil Sign Anusvara
	{0x00bc0, 0x00bc0},  // Tamil Vowel Sign Ii     ..Tamil Vowel Sign Ii
	{0x00bcd, 0x00bcd},  // Tamil Sign Virama       ..Tamil Sign Virama
	{0x00c00, 0x00c00},  // Telugu Sign Combining Ca..Telugu Sign Combining Ca
	{0x00c04, 0x00c04},  // Telugu Sign Combining An..Telugu Sign Combining An
	{0x00c3c, 0x00c3c},  // Telugu Sign Nukta       ..Telugu Sign Nukta
	{0x00c3e, 0x00c40},  // Telugu Vowel Sign Aa    ..Telugu Vowel Sign Ii
	{0x00c46, 0x00c48},  // Telugu Vowel Sign E     ..Telugu Vowel Sign Ai
	{0x00c4a, 0x00c4d},  // Telugu Vowel Sign O     ..Telugu Sign Virama
	{0x00c55, 0x00c56},  // Telugu Length Mark      ..Telugu Ai Length Mark
	{0x00c62, 0x00c63},  // Telugu Vowel Sign Vocali..Telugu Vowel Sign Vocali
	{0x00c81, 0x00c81},  // Kannada Sign Candrabindu..Kannada Sign Candrabindu
	{0x00cbc, 0x00cbc},  // Kannada Sign Nukta      ..Kannada Sign Nukta
	{0x00cbf, 0x00cbf},  // Kannada Vowel Sign I    ..Kannada Vowel Sign I
	{0x00cc6, 0x00cc6},  // Kannada Vowel Sign E    ..Kannada Vowel Sign E
	{0x00ccc, 0x00ccd},  // Kannada Vowel Sign Au   ..Kannada Sign Virama
	{0x00ce2, 0x00ce3},  // Kannada Vowel Sign Vocal..Kannada Vowel Sign Vocal
	{0x00d00, 0x00d01},  // Malayalam Sign Combining..Malayalam Sign Candrabin
	{0x00d3b, 0x00d3c},  // Malayalam Sign Vertical ..Malayalam Sign Circular
	{0x00d41, 0x00d44},  // Malayalam Vowel Sign U  ..Malayalam Vowel Sign Voc
	{0x00d4d, 0x00d4d},  // Malayalam Sign Virama   ..Malayalam Sign Virama
	{0x00d62, 0x00d63},  // Malayalam Vowel Sign Voc..Malayalam Vowel Sign Voc
	{0x00d81, 0x00d81},  // Sinhala Sign Candrabindu..Sinhala Sign Candrabindu
	{0x00dca, 0x00dca},  // Sinhala Sign Al-lakuna  ..Sinhala Sign Al-lakuna
	{0x00dd2, 0x00dd4},  // Sinhala Vowel Sign Ketti..Sinhala Vowel Sign Ketti
	{0x00dd6, 0x00dd6},  // Sinhala Vowel Sign Diga ..Sinhala Vowel Sign Diga
	{0x00e31, 0x00e31},  // Thai Character Mai Han-a..Thai Character Mai Han-a
	{0x00e34, 0x00e3a},  // Thai Character Sara I   ..Thai Character Phinthu
	{0x00e47, 0x00e4e},  // Thai Character Maitaikhu..Thai Character Yamakkan
	{0x00eb1, 0x00eb1},  // Lao Vowel Sign Mai Kan  ..Lao Vowel Sign Mai Kan
	{0x00eb4, 0x00ebc},  // Lao Vowel Sign I        ..Lao Semivowel Sign Lo
	{0x00ec8, 0x00ece},  // Lao Tone Mai Ek         ..(nil)
	{0x00f18, 0x00f19},  // Tibetan Astrological Sig..Tibetan Astrological Sig
	{0x00f35, 0x00f35},  // Tibetan Mark Ngas Bzung ..Tibetan Mark Ngas Bzung
	{0x00f37, 0x00f37},  // Tibetan Mark Ngas Bzung ..Tibetan Mark Ngas Bzung
	{0x00f39, 0x00f39},  // Tibetan Mark Tsa -phru  ..Tibetan Mark Tsa -phru
	{0x00f71, 0x00f7e},  // Tibetan Vowel Sign Aa   ..Tibetan Sign Rjes Su Nga
	{0x00f80, 0x00f84},  // Tibetan Vowel Sign Rever..Tibetan Mark Halanta
	{0x00f86, 0x00f87},  // Tibetan Sign Lci Rtags  ..Tibetan Sign Yang Rtags
	{0x00f8d, 0x00f97},  // Tibetan Subjoined Sign L..Tibetan Subjoined Letter
	{0x00f99, 0x00fbc},  // Tibetan Subjoined Letter..Tibetan Subjoined Letter
	{0x00fc6, 0x00fc6},  // Tibetan Symbol Padma Gda..Tibetan Symbol Padma Gda
	{0x0102d, 0x01030},  // Myanmar Vowel Sign I    ..Myanmar Vowel Sign Uu
	{0x01032, 0x01037},  // Myanmar Vowel Sign Ai   ..Myanmar Sign Dot Below
	{0x01039, 0x0103a},  // Myanmar Sign Virama     ..Myanmar Sign Asat
	{0x0103d, 0x0103e},  // Myanmar Consonant Sign M..Myanmar Consonant Sign M
	{0x01058, 0x01059},  // Myanmar Vowel Sign Vocal..Myanmar Vowel Sign Vocal
	{0x0105e, 0x01060},  // Myanmar Consonant Sign M..Myanmar Consonant Sign M
	{0x01071, 0x01074},  // Myanmar Vowel Sign Geba ..Myanmar Vowel Sign Kayah
	{0x01082, 0x01082},  // Myanmar Consonant Sign S..Myanmar Consonant Sign S
	{0x01085, 0x01086},  // Myanmar Vowel Sign Shan ..Myanmar Vowel Sign Shan
	{0x0108d, 0x0108d},  // Myanmar Sign Shan Counci..Myanmar Sign Shan Counci
	{0x0109d, 0x0109d},  // Myanmar Vowel Sign Aiton..Myanmar Vowel Sign Aiton
	{0x0135d, 0x0135f},  // Ethiopic Combining Gemin..Ethiopic Combining Gemin
	{0x01712, 0x01714},  // Tagalog Vowel Sign I    ..Tagalog Sign Virama
	{0x01732, 0x01733},  // Hanunoo Vowel Sign I    ..Hanunoo Vowel Sign U
	{0x01752, 0x01753},  // Buhid Vowel Sign I      ..Buhid Vowel Sign U
	{0x01772, 0x01773},  // Tagbanwa Vowel Sign I   ..Tagbanwa Vowel Sign U
	{0x017b4, 0x017b5},  // Khmer Vowel Inherent Aq ..Khmer Vowel Inherent Aa
	{0x017b7, 0x017bd},  // Khmer Vowel Sign I      ..Khmer Vowel Sign Ua
	{0x017c6, 0x017c6},  // Khmer Sign Nikahit      ..Khmer Sign Nikahit
	{0x017c9, 0x017d3},  // Khmer Sign Muusikatoan  ..Khmer Sign Bathamasat
	{0x017dd, 0x017dd},  // Khmer Sign Atthacan     ..Khmer Sign Atthacan
	{0x0180b, 0x0180d},  // Mongolian Free Variation..Mongolian Free Variation
	{0x0180f, 0x0180f},  // Mongolian Free Variation..Mongolian Free Variation
	{0x01885, 0x01886},  // Mongolian Letter Ali Gal..Mongolian Letter Ali Gal
	{0x018a9, 0x018a9},  // Mongolian Letter Ali Gal..Mongolian Letter Ali Gal
	{0x01920, 0x01922},  // Limbu Vowel Sign A      ..Limbu Vowel Sign U
	{0x01927, 0x01928},  // Limbu Vowel Sign E      ..Limbu Vowel Sign O
	{0x01932, 0x01932},  // Limbu Small Letter Anusv..Limbu Small Letter Anusv
	{0x01939, 0x0193b},  // Limbu Sign Mukphreng    ..Limbu Sign Sa-i
	{0x01a17, 0x01a18},  // Buginese Vowel Sign I   ..Buginese Vowel Sign U
	{0x01a1b, 0x01a1b},  // Buginese Vowel Sign Ae  ..Buginese Vowel Sign Ae
	{0x01a56, 0x01a56},  // Tai Tham Consonant Sign ..Tai Tham Consonant Sign
	{0x01a58, 0x01a5e},  // Tai Tham Sign Mai Kang L..Tai Tham Consonant Sign
	{0x01a60, 0x01a60},  // Tai Tham Sign Sakot     ..Tai Tham Sign Sakot
	{0x01a62, 0x01a62},  // Tai Tham Vowel Sign Mai ..Tai Tham Vowel Sign Mai
	{0x01a65, 0x01a6c},  // Tai Tham Vowel Sign I   ..Tai Tham Vowel Sign Oa B
	{0x01a73, 0x01a7c},  // Tai Tham Vowel Sign Oa A..Tai Tham Sign Khuen-lue
	{0x01a7f, 0x01a7f},  // Tai Tham Combining Crypt..Tai Tham Combining Crypt
	{0x01ab0, 0x01ace},  // Combining Doubled Circum..Combining Latin Small Le
	{0x01b00, 0x01b03},  // Balinese Sign Ulu Ricem ..Balinese Sign Surang
	{0x01b34, 0x01b34},  // Balinese Sign Rerekan   ..Balinese Sign Rerekan
	{0x01b36, 0x01b3a},  // Balinese Vowel Sign Ulu ..Balinese Vowel Sign Ra R
	{0x01b3c, 0x01b3c},  // Balinese Vowel Sign La L..Balinese Vowel Sign La L
	{0x01b42, 0x01b42},  // Balinese Vowel Sign Pepe..Balinese Vowel Sign Pepe
	{0x01b6b, 0x01b73},  // Balinese Musical Symbol ..Balinese Musical Symbol
	{0x01b80, 0x01b81},  // Sundanese Sign Panyecek ..Sundanese Sign Panglayar
	{0x01ba2, 0x01ba5},  // Sundanese Consonant Sign..Sundanese Vowel Sign Pan
	{0x01ba8, 0x01ba9},  // Sundanese Vowel Sign Pam..Sundanese Vowel Sign Pan
	{0x01bab, 0x01bad},  // Sundanese Sign Virama   ..Sundanese Consonant Sign
	{0x01be6, 0x01be6},  // Batak Sign Tompi        ..Batak Sign Tompi
	{0x01be8, 0x01be9},  // Batak Vowel Sign Pakpak ..Batak Vowel Sign Ee
	{0x01bed, 0x01bed},  // Batak Vowel Sign Karo O ..Batak Vowel Sign Karo O
	{0x01bef, 0x01bf1},  // Batak Vowel Sign U For S..Batak Consonant Sign H
	{0x01c2c, 0x01c33},  // Lepcha Vowel Sign E     ..Lepcha Consonant Sign T
	{0x01c36, 0x01c37},  // Lepcha Sign Ran         ..Lepcha Sign Nukta
	{0x01cd0, 0x01cd2},  // Vedic Tone Karshana     ..Vedic Tone Prenkha
	{0x01cd4, 0x01ce0},  // Vedic Sign Yajurvedic Mi..Vedic Tone Rigvedic Kash
	{0x01ce2, 0x01ce8},  // Vedic Sign Visarga Svari..Vedic Sign Visarga Anuda
	{0x01ced, 0x01ced},  // Vedic Sign Tiryak       ..Vedic Sign Tiryak
	{0x01cf4, 0x01cf4},  // Vedic Tone Candra Above ..Vedic Tone Candra Above
	{0x01cf8, 0x01cf9},  // Vedic Tone Ring Above   ..Vedic Tone Double Ring A
	{0x01dc0, 0x01dff},  // Combining Dotted Grave A..Combining Right Arrowhea
	{0x020d0, 0x020f0},  // Combining Left Harpoon A..Combining Asterisk Above
	{0x02cef, 0x02cf1},  // Coptic Combining Ni Abov..Coptic Combining Spiritu
	{0x02d7f, 0x02d7f},  // Tifinagh Consonant Joine..Tifinagh Consonant Joine
	{0x02de0, 0x02dff},  // Combining Cyrillic Lette..Combining Cyrillic Lette
	{0x0302a, 0x0302d},  // Ideographic Level Tone M..Ideographic Entering Ton
	{0x03099, 0x0309a},  // Combining Katakana-hirag..Combining Katakana-hirag
	{0x0a66f, 0x0a672},  // Combining Cyrillic Vzmet..Combining Cyrillic Thous
	{0x0a674, 0x0a67d},  // Combining Cyrillic Lette..Combining Cyrillic Payer
	{0x0a69e, 0x0a69f},  // Combining Cyrillic Lette..Combining Cyrillic Lette
	{0x0a6f0, 0x0a6f1},  // Bamum Combining Mark Koq..Bamum Combining Mark Tuk
	{0x0a802, 0x0a802},  // Syloti Nagri Sign Dvisva..Syloti Nagri Sign Dvisva
	{0x0a806, 0x0a806},  // Syloti Nagri Sign Hasant..Syloti Nagri Sign Hasant
	{0x0a80b, 0x0a80b},  // Syloti Nagri Sign Anusva..Syloti Nagri Sign Anusva
	{0x0a825, 0x0a826},  // Syloti Nagri Vowel Sign ..Syloti Nagri Vowel Sign
	{0x0a82c, 0x0a82c},  // Syloti Nagri Sign Altern..Syloti Nagri Sign Altern
	{0x0a8c4, 0x0a8c5},  // Saurashtra Sign Virama  ..Saurashtra Sign Candrabi
	{0x0a8e0, 0x0a8f1},  // Combining Devanagari Dig..Combining Devanagari Sig
	{0x0a8ff, 0x0a8ff},  // Devanagari Vowel Sign Ay..Devanagari Vowel Sign Ay
	{0x0a926, 0x0a92d},  // Kayah Li Vowel Ue       ..Kayah Li Tone Calya Plop
	{0x0a947, 0x0a951},  // Rejang Vowel Sign I     ..Rejang Consonant Sign R
	{0x0a980, 0x0a982},  // Javanese Sign Panyangga ..Javanese Sign Layar
	{0x0a9b3, 0x0a9b3},  // Javanese Sign Cecak Telu..Javanese Sign Cecak Telu
	{0x0a9b6, 0x0a9b9},  // Javanese Vowel Sign Wulu..Javanese Vowel Sign Suku
	{0x0a9bc, 0x0a9bd},  // Javanese Vowel Sign Pepe..Javanese Consonant Sign
	{0x0a9e5, 0x0a9e5},  // Myanmar Sign Shan Saw   ..Myanmar Sign Shan Saw
	{0x0aa29, 0x0aa2e},  // Cham Vowel Sign Aa      ..Cham Vowel Sign Oe
	{0x0aa31, 0x0aa32},  // Cham Vowel Sign Au      ..Cham Vowel Sign Ue
	{0x0aa35, 0x0aa36},  // Cham Consonant Sign La  ..Cham Consonant Sign Wa
	{0x0aa43, 0x0aa43},  // Cham Consonant Sign Fina..Cham Consonant Sign Fina
	{0x0aa4c, 0x0aa4c},  // Cham Consonant Sign Fina..Cham Consonant Sign Fina
	{0x0aa7c, 0x0aa7c},  // Myanmar Sign Tai Laing T..Myanmar Sign Tai Laing T
	{0x0aab0, 0x0aab0},  // Tai Viet Mai Kang       ..Tai Viet Mai Kang
	{0x0aab2, 0x0aab4},  // Tai Viet Vowel I        ..Tai Viet Vowel U
	{0x0aab7, 0x0aab8},  // Tai Viet Mai Khit       ..Tai Viet Vowel Ia
	{0x0aabe, 0x0aabf},  // Tai Viet Vowel Am       ..Tai Viet Tone Mai Ek
	{0x0aac1, 0x0aac1},  // Tai Viet Tone Mai Tho   ..Tai Viet Tone Mai Tho
	{0x0aaec, 0x0aaed},  // Meetei Mayek Vowel Sign ..Meetei Mayek Vowel Sign
	{0x0aaf6, 0x0aaf6},  // Meetei Mayek Virama     ..Meetei Mayek Virama
	{0x0abe5, 0x0abe5},  // Meetei Mayek Vowel Sign ..Meetei Mayek Vowel Sign
	{0x0abe8, 0x0abe8},  // Meetei Mayek Vowel Sign ..Meetei Mayek Vowel Sign
	{0x0abed, 0x0abed},  // Meetei Mayek Apun Iyek  ..Meetei Mayek Apun Iyek
	{0x0fb1e, 0x0fb1e},  // Hebrew Point Judeo-spani..Hebrew Point Judeo-spani
	{0x0fe00, 0x0fe0f},  // Variation Selector-1    ..Variation Selector-16
	{0x0fe20, 0x0fe2f},  // Combining Ligature Left ..Combining Cyrillic Titlo
	{0x101fd, 0x101fd},  // Phaistos Disc Sign Combi..Phaistos Disc Sign Combi
	{0x102e0, 0x102e0},  // Coptic Epact Thousands M..Coptic Epact Thousands M
	{0x10376, 0x1037a},  // Combining Old Permic Let..Combining Old Permic Let
	{0x10a01, 0x10a03},  // Kharoshthi Vowel Sign I ..Kharoshthi Vowel Sign Vo
	{0x10a05, 0x10a06},  // Kharoshthi Vowel Sign E ..Kharoshthi Vowel Sign O
	{0x10a0c, 0x10a0f},  // Kharoshthi Vowel Length ..Kharoshthi Sign Visarga
	{0x10a38, 0x10a3a},  // Kharoshthi Sign Bar Abov..Kharoshthi Sign Dot Belo
	{0x10a3f, 0x10a3f},  // Kharoshthi Virama       ..Kharoshthi Virama
	{0x10ae5, 0x10ae6},  // Manichaean Abbreviation ..Manichaean Abbreviation
	{0x10d24, 0x10d27},  // Hanifi Rohingya Sign Har..Hanifi Rohingya Sign Tas
	{0x10eab, 0x10eac},  // Yezidi Combining Hamza M..Yezidi Combining Madda M
	{0x10efd, 0x10eff},  // (nil)                   ..(nil)
	{0x10f46, 0x10f50},  // Sogdian Combining Dot Be..Sogdian Combining Stroke
	{0x10f82, 0x10f85},  // Old Uyghur Combining Dot..Old Uyghur Combining Two
	{0x11001, 0x11001},  // Brahmi Sign Anusvara    ..Brahmi Sign Anusvara
	{0x11038, 0x11046},  // Brahmi Vowel Sign Aa    ..Brahmi Virama
	{0x11070, 0x11070},  // Brahmi Sign Old Tamil Vi..Brahmi Sign Old Tamil Vi
	{0x11073, 0x11074},  // Brahmi Vowel Sign Old Ta..Brahmi Vowel Sign Old Ta
	{0x1107f, 0x11081},  // Brahmi Number Joiner    ..Kaithi Sign Anusvara
	{0x110b3, 0x110b6},  // Kaithi Vowel Sign U     ..Kaithi Vowel Sign Ai
	{0x110b9, 0x110ba},  // Kaithi Sign Virama      ..Kaithi Sign Nukta
	{0x110c2, 0x110c2},  // Kaithi Vowel Sign Vocali..Kaithi Vowel Sign Vocali
	{0x11100, 0x11102},  // Chakma Sign Candrabindu ..Chakma Sign Visarga
	{0x11127, 0x1112b},  // Chakma Vowel Sign A     ..Chakma Vowel Sign Uu
	{0x1112d, 0x11134},  // Chakma Vowel Sign Ai    ..Chakma Maayyaa
	{0x11173, 0x11173},  // Mahajani Sign Nukta     ..Mahajani Sign Nukta
	{0x11180, 0x11181},  // Sharada Sign Candrabindu..Sharada Sign Anusvara
	{0x111b6, 0x111be},  // Sharada Vowel Sign U    ..Sharada Vowel Sign O
	{0x111c9, 0x111cc},  // Sharada Sandhi Mark     ..Sharada Extra Short Vowe
	{0x111cf, 0x111cf},  // Sharada Sign Inverted Ca..Sharada Sign Inverted Ca
	{0x1122f, 0x11231},  // Khojki Vowel Sign U     ..Khojki Vowel Sign Ai
	{0x11234, 0x11234},  // Khojki Sign Anusvara    ..Khojki Sign Anusvara
	{0x11236, 0x11237},  // Khojki Sign Nukta       ..Khojki Sign Shadda
	{0x1123e, 0x1123e},  // Khojki Sign Sukun       ..Khojki Sign Sukun
	{0x11241, 0x11241},  // (nil)                   ..(nil)
	{0x112df, 0x112df},  // Khudawadi Sign Anusvara ..Khudawadi Sign Anusvara
	{0x112e3, 0x112ea},  // Khudawadi Vowel Sign U  ..Khudawadi Sign Virama
	{0x11300, 0x11301},  // Grantha Sign Combining A..Grantha Sign Candrabindu
	{0x1133b, 0x1133c},  // Combining Bindu Below   ..Grantha Sign Nukta
	{0x11340, 0x11340},  // Grantha Vowel Sign Ii   ..Grantha Vowel Sign Ii
	{0x11366, 0x1136c},  // Combining Grantha Digit ..Combining Grantha Digit
	{0x11370, 0x11374},  // Combining Grantha Letter..Combining Grantha Letter
	{0x11438, 0x1143f},  // Newa Vowel Sign U       ..Newa Vowel Sign Ai
	{0x11442, 0x11444},  // Newa Sign Virama        ..Newa Sign Anusvara
	{0x11446, 0x11446},  // Newa Sign Nukta         ..Newa Sign Nukta
	{0x1145e, 0x1145e},  // Newa Sandhi Mark        ..Newa Sandhi Mark
	{0x114b3, 0x114b8},  // Tirhuta Vowel Sign U    ..Tirhuta Vowel Sign Vocal
	{0x114ba, 0x114ba},  // Tirhuta Vowel Sign Short..Tirhuta Vowel Sign Short
	{0x114bf, 0x114c0},  // Tirhuta Sign Candrabindu..Tirhuta Sign Anusvara
	{0x114c2, 0x114c3},  // Tirhuta Sign Virama     ..Tirhuta Sign Nukta
	{0x115b2, 0x115b5},  // Siddham Vowel Sign U    ..Siddham Vowel Sign Vocal
	{0x115bc, 0x115bd},  // Siddham Sign Candrabindu..Siddham Sign Anusvara
	{0x115bf, 0x115c0},  // Siddham Sign Virama     ..Siddham Sign Nukta
	{0x115dc, 0x115dd},  // Siddham Vowel Sign Alter..Siddham Vowel Sign Alter
	{0x11633, 0x1163a},  // Modi Vowel Sign U       ..Modi Vowel Sign Ai
	{0x1163d, 0x1163d},  // Modi Sign Anusvara      ..Modi Sign Anusvara
	{0x1163f, 0x11640},  // Modi Sign Virama        ..Modi Sign Ardhacandra
	{0x116ab, 0x116ab},  // Takri Sign Anusvara     ..Takri Sign Anusvara
	{0x116ad, 0x116ad},  // Takri Vowel Sign Aa     ..Takri Vowel Sign Aa
	{0x116b0, 0x116b5},  // Takri Vowel Sign U      ..Takri Vowel Sign Au
	{0x116b7, 0x116b7},  // Takri Sign Nukta        ..Takri Sign Nukta
	{0x1171d, 0x1171f},  // Ahom Consonant Sign Medi..Ahom Consonant Sign Medi
	{0x11722, 0x11725},  // Ahom Vowel Sign I       ..Ahom Vowel Sign Uu
	{0x11727, 0x1172b},  // Ahom Vowel Sign Aw      ..Ahom Sign Killer
	{0x1182f, 0x11837},  // Dogra Vowel Sign U      ..Dogra Sign Anusvara
	{0x11839, 0x1183a},  // Dogra Sign Virama       ..Dogra Sign Nukta
	{0x1193b, 0x1193c},  // Dives Akuru Sign Anusvar..Dives Akuru Sign Candrab
	{0x1193e, 0x1193e},  // Dives Akuru Virama      ..Dives Akuru Virama
	{0x11943, 0x11943},  // Dives Akuru Sign Nukta  ..Dives Akuru Sign Nukta
	{0x119d4, 0x119d7},  // Nandinagari Vowel Sign U..Nandinagari Vowel Sign V
	{0x119da, 0x119db},  // Nandinagari Vowel Sign E..Nandinagari Vowel Sign A
	{0x119e0, 0x119e0},  // Nandinagari Sign Virama ..Nandinagari Sign Virama
	{0x11a01, 0x11a0a},  // Zanabazar Square Vowel S..Zanabazar Square Vowel L
	{0x11a33, 0x11a38},  // Zanabazar Square Final C..Zanabazar Square Sign An
	{0x11a3b, 0x11a3e},  // Zanabazar Square Cluster..Zanabazar Square Cluster
	{0x11a47, 0x11a47},  // Zanabazar Square Subjoin..Zanabazar Square Subjoin
	{0x11a51, 0x11a56},  // Soyombo Vowel Sign I    ..Soyombo Vowel Sign Oe
	{0x11a59, 0x11a5b},  // Soyombo Vowel Sign Vocal..Soyombo Vowel Length Mar
	{0x11a8a, 0x11a96},  // Soyombo Final Consonant ..Soyombo Sign Anusvara
	{0x11a98, 0x11a99},  // Soyombo Gemination Mark ..Soyombo Subjoiner
	{0x11c30, 0x11c36},  // Bhaiksuki Vowel Sign I  ..Bhaiksuki Vowel Sign Voc
	{0x11c38, 0x11c3d},  // Bhaiksuki Vowel Sign E  ..Bhaiksuki Sign Anusvara
	{0x11c3f, 0x11c3f},  // Bhaiksuki Sign Virama   ..Bhaiksuki Sign Virama
	{0x11c92, 0x11ca7},  // Marchen Subjoined Letter..Marchen Subjoined Letter
	{0x11caa, 0x11cb0},  // Marchen Subjoined Letter..Marchen Vowel Sign Aa
	{0x11cb2, 0x11cb3},  // Marchen Vowel Sign U    ..Marchen Vowel Sign E
	{0x11cb5, 0x11cb6},  // Marchen Sign Anusvara   ..Marchen Sign Candrabindu
	{0x11d31, 0x11d36},  // Masaram Gondi Vowel Sign..Masaram Gondi Vowel Sign
	{0x11d3a, 0x11d3a},  // Masaram Gondi Vowel Sign..Masaram Gondi Vowel Sign
	{0x11d3c, 0x11d3d},  // Masaram Gondi Vowel Sign..Masaram Gondi Vowel Sign
	{0x11d3f, 0x11d45},  // Masaram Gondi Vowel Sign..Masaram Gondi Virama
	{0x11d47, 0x11d47},  // Masaram Gondi Ra-kara   ..Masaram Gondi Ra-kara
	{0x11d90, 0x11d91},  // Gunjala Gondi Vowel Sign..Gunjala Gondi Vowel Sign
	{0x11d95, 0x11d95},  // Gunjala Gondi Sign Anusv..Gunjala Gondi Sign Anusv
	{0x11d97, 0x11d97},  // Gunjala Gondi Virama    ..Gunjala Gondi Virama
	{0x11ef3, 0x11ef4},  // Makasar Vowel Sign I    ..Makasar Vowel Sign U
	{0x11f00, 0x11f01},  // (nil)                   ..(nil)
	{0x11f36, 0x11f3a},  // (nil)                   ..(nil)
	{0x11f40, 0x11f40},  // (nil)                   ..(nil)
	{0x11f42, 0x11f42},  // (nil)                   ..(nil)
	{0x13440, 0x13440},  // (nil)                   ..(nil)
	{0x13447, 0x13455},  // (nil)                   ..(nil)
	{0x16af0, 0x16af4},  // Bassa Vah Combining High..Bassa Vah Combining High
	{0x16b30, 0x16b36},  // Pahawh Hmong Mark Cim Tu..Pahawh Hmong Mark Cim Ta
	{0x16f4f, 0x16f4f},  // Miao Sign Consonant Modi..Miao Sign Consonant Modi
	{0x16f8f, 0x16f92},  // Miao Tone Right         ..Miao Tone Below
	{0x16fe4, 0x16fe4},  // Khitan Small Script Fill..Khitan Small Script Fill
	{0x1bc9d, 0x1bc9e},  // Duployan Thick Letter Se..Duployan Double Mark
	{0x1cf00, 0x1cf2d},  // Znamenny Combining Mark ..Znamenny Combining Mark
	{0x1cf30, 0x1cf46},  // Znamenny Combining Tonal..Znamenny Priznak Modifie
	{0x1d167, 0x1d169},  // Musical Symbol Combining..Musical Symbol Combining
	{0x1d17b, 0x1d182},  // Musical Symbol Combining..Musical Symbol Combining
	{0x1d185, 0x1d18b},  // Musical Symbol Combining..Musical Symbol Combining
	{0x1d1aa, 0x1d1ad},  // Musical Symbol Combining..Musical Symbol Combining
	{0x1d242, 0x1d244},  // Combining Greek Musical ..Combining Greek Musical
	{0x1da00, 0x1da36},  // Signwriting Head Rim    ..Signwriting Air Sucking
	{0x1da3b, 0x1da6c},  // Signwriting Mouth Closed..Signwriting Excitement
	{0x1da75, 0x1da75},  // Signwriting Upper Body T..Signwriting Upper Body T
	{0x1da84, 0x1da84},  // Signwriting Location Hea..Signwriting Location Hea
	{0x1da9b, 0x1da9f},  // Signwriting Fill Modifie..Signwriting Fill Modifie
	{0x1daa1, 0x1daaf},  // Signwriting Rotation Mod..Signwriting Rotation Mod
	{0x1e000, 0x1e006},  // Combining Glagolitic Let..Combining Glagolitic Let
	{0x1e008, 0x1e018},  // Combining Glagolitic Let..Combining Glagolitic Let
	{0x1e01b, 0x1e021},  // Combining Glagolitic Let..Combining Glagolitic Let
	{0x1e023, 0x1e024},  // Combining Glagolitic Let..Combining Glagolitic Let
	{0x1e026, 0x1e02a},  // Combining Glagolitic Let..Combining Glagolitic Let
	{0x1e08f, 0x1e08f},  // (nil)                   ..(nil)
	{0x1e130, 0x1e136},  // Nyiakeng Puachue Hmong T..Nyiakeng Puachue Hmong T
	{0x1e2ae, 0x1e2ae},  // Toto Sign Rising Tone   ..Toto Sign Rising Tone
	{0x1e2ec, 0x1e2ef},  // Wancho Tone Tup         ..Wancho Tone Koini
	{0x1e4ec, 0x1e4ef},  // (nil)                   ..(nil)
	{0x1e8d0, 0x1e8d6},  // Mende Kikakui Combining ..Mende Kikakui Combining
	{0x1e944, 0x1e94a},  // Adlam Alif Lengthener   ..Adlam Nukta
	{0xe0100, 0xe01ef},  // Variation Selector-17   ..Variation Selector-256
};

static struct width_interval WIDE_EASTASIAN[] = {
	// from https://github.com/PitWD/termux_wcwidth
	// which is a copy of https://github.com/termux/wcwidth
	// all MIT licensed
	{0x01100, 0x0115f},  // Hangul Choseong Kiyeok  ..Hangul Choseong Filler
	{0x0231a, 0x0231b},  // Watch                   ..Hourglass
	{0x02329, 0x0232a},  // Left-pointing Angle Brac..Right-pointing Angle Bra
	{0x023e9, 0x023ec},  // Black Right-pointing Dou..Black Down-pointing Doub
	{0x023f0, 0x023f0},  // Alarm Clock             ..Alarm Clock
	{0x023f3, 0x023f3},  // Hourglass With Flowing S..Hourglass With Flowing S
	{0x025fd, 0x025fe},  // White Medium Small Squar..Black Medium Small Squar
	{0x02614, 0x02615},  // Umbrella With Rain Drops..Hot Beverage
	{0x02648, 0x02653},  // Aries                   ..Pisces
	{0x0267f, 0x0267f},  // Wheelchair Symbol       ..Wheelchair Symbol
	{0x02693, 0x02693},  // Anchor                  ..Anchor
	{0x026a1, 0x026a1},  // High Voltage Sign       ..High Voltage Sign
	{0x026aa, 0x026ab},  // Medium White Circle     ..Medium Black Circle
	{0x026bd, 0x026be},  // Soccer Ball             ..Baseball
	{0x026c4, 0x026c5},  // Snowman Without Snow    ..Sun Behind Cloud
	{0x026ce, 0x026ce},  // Ophiuchus               ..Ophiuchus
	{0x026d4, 0x026d4},  // No Entry                ..No Entry
	{0x026ea, 0x026ea},  // Church                  ..Church
	{0x026f2, 0x026f3},  // Fountain                ..Flag In Hole
	{0x026f5, 0x026f5},  // Sailboat                ..Sailboat
	{0x026fa, 0x026fa},  // Tent                    ..Tent
	{0x026fd, 0x026fd},  // Fuel Pump               ..Fuel Pump
	{0x02705, 0x02705},  // White Heavy Check Mark  ..White Heavy Check Mark
	{0x0270a, 0x0270b},  // Raised Fist             ..Raised Hand
	{0x02728, 0x02728},  // Sparkles                ..Sparkles
	{0x0274c, 0x0274c},  // Cross Mark              ..Cross Mark
	{0x0274e, 0x0274e},  // Negative Squared Cross M..Negative Squared Cross M
	{0x02753, 0x02755},  // Black Question Mark Orna..White Exclamation Mark O
	{0x02757, 0x02757},  // Heavy Exclamation Mark S..Heavy Exclamation Mark S
	{0x02795, 0x02797},  // Heavy Plus Sign         ..Heavy Division Sign
	{0x027b0, 0x027b0},  // Curly Loop              ..Curly Loop
	{0x027bf, 0x027bf},  // Double Curly Loop       ..Double Curly Loop
	{0x02b1b, 0x02b1c},  // Black Large Square      ..White Large Square
	{0x02b50, 0x02b50},  // White Medium Star       ..White Medium Star
	{0x02b55, 0x02b55},  // Heavy Large Circle      ..Heavy Large Circle
	{0x02e80, 0x02e99},  // Cjk Radical Repeat      ..Cjk Radical Rap
	{0x02e9b, 0x02ef3},  // Cjk Radical Choke       ..Cjk Radical C-simplified
	{0x02f00, 0x02fd5},  // Kangxi Radical One      ..Kangxi Radical Flute
	{0x02ff0, 0x02ffb},  // Ideographic Description ..Ideographic Description
	{0x03000, 0x0303e},  // Ideographic Space       ..Ideographic Variation In
	{0x03041, 0x03096},  // Hiragana Letter Small A ..Hiragana Letter Small Ke
	{0x03099, 0x030ff},  // Combining Katakana-hirag..Katakana Digraph Koto
	{0x03105, 0x0312f},  // Bopomofo Letter B       ..Bopomofo Letter Nn
	{0x03131, 0x0318e},  // Hangul Letter Kiyeok    ..Hangul Letter Araeae
	{0x03190, 0x031e3},  // Ideographic Annotation L..Cjk Stroke Q
	{0x031f0, 0x0321e},  // Katakana Letter Small Ku..Parenthesized Korean Cha
	{0x03220, 0x03247},  // Parenthesized Ideograph ..Circled Ideograph Koto
	{0x03250, 0x04dbf},  // Partnership Sign        ..Cjk Unified Ideograph-4d
	{0x04e00, 0x0a48c},  // Cjk Unified Ideograph-4e..Yi Syllable Yyr
	{0x0a490, 0x0a4c6},  // Yi Radical Qot          ..Yi Radical Ke
	{0x0a960, 0x0a97c},  // Hangul Choseong Tikeut-m..Hangul Choseong Ssangyeo
	{0x0ac00, 0x0d7a3},  // Hangul Syllable Ga      ..Hangul Syllable Hih
	{0x0f900, 0x0faff},  // Cjk Compatibility Ideogr..(nil)
	{0x0fe10, 0x0fe19},  // Presentation Form For Ve..Presentation Form For Ve
	{0x0fe30, 0x0fe52},  // Presentation Form For Ve..Small Full Stop
	{0x0fe54, 0x0fe66},  // Small Semicolon         ..Small Equals Sign
	{0x0fe68, 0x0fe6b},  // Small Reverse Solidus   ..Small Commercial At
	{0x0ff01, 0x0ff60},  // Fullwidth Exclamation Ma..Fullwidth Right White Pa
	{0x0ffe0, 0x0ffe6},  // Fullwidth Cent Sign     ..Fullwidth Won Sign
	{0x16fe0, 0x16fe4},  // Tangut Iteration Mark   ..Khitan Small Script Fill
	{0x16ff0, 0x16ff1},  // Vietnamese Alternate Rea..Vietnamese Alternate Rea
	{0x17000, 0x187f7},  // (nil)                   ..(nil)
	{0x18800, 0x18cd5},  // Tangut Component-001    ..Khitan Small Script Char
	{0x18d00, 0x18d08},  // (nil)                   ..(nil)
	{0x1aff0, 0x1aff3},  // Katakana Letter Minnan T..Katakana Letter Minnan T
	{0x1aff5, 0x1affb},  // Katakana Letter Minnan T..Katakana Letter Minnan N
	{0x1affd, 0x1affe},  // Katakana Letter Minnan N..Katakana Letter Minnan N
	{0x1b000, 0x1b122},  // Katakana Letter Archaic ..Katakana Letter Archaic
	{0x1b132, 0x1b132},  // (nil)                   ..(nil)
	{0x1b150, 0x1b152},  // Hiragana Letter Small Wi..Hiragana Letter Small Wo
	{0x1b155, 0x1b155},  // (nil)                   ..(nil)
	{0x1b164, 0x1b167},  // Katakana Letter Small Wi..Katakana Letter Small N
	{0x1b170, 0x1b2fb},  // Nushu Character-1b170   ..Nushu Character-1b2fb
	{0x1f004, 0x1f004},  // Mahjong Tile Red Dragon ..Mahjong Tile Red Dragon
	{0x1f0cf, 0x1f0cf},  // Playing Card Black Joker..Playing Card Black Joker
	{0x1f18e, 0x1f18e},  // Negative Squared Ab     ..Negative Squared Ab
	{0x1f191, 0x1f19a},  // Squared Cl              ..Squared Vs
	{0x1f200, 0x1f202},  // Square Hiragana Hoka    ..Squared Katakana Sa
	{0x1f210, 0x1f23b},  // Squared Cjk Unified Ideo..Squared Cjk Unified Ideo
	{0x1f240, 0x1f248},  // Tortoise Shell Bracketed..Tortoise Shell Bracketed
	{0x1f250, 0x1f251},  // Circled Ideograph Advant..Circled Ideograph Accept
	{0x1f260, 0x1f265},  // Rounded Symbol For Fu   ..Rounded Symbol For Cai
	{0x1f300, 0x1f320},  // Cyclone                 ..Shooting Star
	{0x1f32d, 0x1f335},  // Hot Dog                 ..Cactus
	{0x1f337, 0x1f37c},  // Tulip                   ..Baby Bottle
	{0x1f37e, 0x1f393},  // Bottle With Popping Cork..Graduation Cap
	{0x1f3a0, 0x1f3ca},  // Carousel Horse          ..Swimmer
	{0x1f3cf, 0x1f3d3},  // Cricket Bat And Ball    ..Table Tennis Paddle And
	{0x1f3e0, 0x1f3f0},  // House Building          ..European Castle
	{0x1f3f4, 0x1f3f4},  // Waving Black Flag       ..Waving Black Flag
	{0x1f3f8, 0x1f43e},  // Badminton Racquet And Sh..Paw Prints
	{0x1f440, 0x1f440},  // Eyes                    ..Eyes
	{0x1f442, 0x1f4fc},  // Ear                     ..Videocassette
	{0x1f4ff, 0x1f53d},  // Prayer Beads            ..Down-pointing Small Red
	{0x1f54b, 0x1f54e},  // Kaaba                   ..Menorah With Nine Branch
	{0x1f550, 0x1f567},  // Clock Face One Oclock   ..Clock Face Twelve-thirty
	{0x1f57a, 0x1f57a},  // Man Dancing             ..Man Dancing
	{0x1f595, 0x1f596},  // Reversed Hand With Middl..Raised Hand With Part Be
	{0x1f5a4, 0x1f5a4},  // Black Heart             ..Black Heart
	{0x1f5fb, 0x1f64f},  // Mount Fuji              ..Person With Folded Hands
	{0x1f680, 0x1f6c5},  // Rocket                  ..Left Luggage
	{0x1f6cc, 0x1f6cc},  // Sleeping Accommodation  ..Sleeping Accommodation
	{0x1f6d0, 0x1f6d2},  // Place Of Worship        ..Shopping Trolley
	{0x1f6d5, 0x1f6d7},  // Hindu Temple            ..Elevator
	{0x1f6dc, 0x1f6df},  // (nil)                   ..Ring Buoy
	{0x1f6eb, 0x1f6ec},  // Airplane Departure      ..Airplane Arriving
	{0x1f6f4, 0x1f6fc},  // Scooter                 ..Roller Skate
	{0x1f7e0, 0x1f7eb},  // Large Orange Circle     ..Large Brown Square
	{0x1f7f0, 0x1f7f0},  // Heavy Equals Sign       ..Heavy Equals Sign
	{0x1f90c, 0x1f93a},  // Pinched Fingers         ..Fencer
	{0x1f93c, 0x1f945},  // Wrestlers               ..Goal Net
	{0x1f947, 0x1f9ff},  // First Place Medal       ..Nazar Amulet
	{0x1fa70, 0x1fa7c},  // Ballet Shoes            ..Crutch
	{0x1fa80, 0x1fa88},  // Yo-yo                   ..(nil)
	{0x1fa90, 0x1fabd},  // Ringed Planet           ..(nil)
	{0x1fabf, 0x1fac5},  // (nil)                   ..Person With Crown
	{0x1face, 0x1fadb},  // (nil)                   ..(nil)
	{0x1fae0, 0x1fae8},  // Melting Face            ..(nil)
	{0x1faf0, 0x1faf8},  // Hand With Index Finger A..(nil)
	{0x20000, 0x2fffd},  // Cjk Unified Ideograph-20..(nil)
	{0x30000, 0x3fffd},  // Cjk Unified Ideograph-30..(nil)
};

static int intable(struct width_interval* table, int table_length, int c) {
	// from https://github.com/PitWD/termux_wcwidth
	// which is a copy of https://github.com/termux/wcwidth
	// all MIT licensed

	// First quick check for Latin1 etc. characters.
	if (c < table[0].start) return 0;

	// Binary search in table.
	int bot = 0;
	int top = table_length - 1;
	while (top >= bot) {
			int mid = (bot + top) / 2;
			if (table[mid].end < c) {
					bot = mid + 1;
			} else if (table[mid].start > c) {
					top = mid - 1;
			} else {
					return 1;
			}
	}
	return 0;
}

int CharLen(wchar_t ucs) {
	// from https://github.com/PitWD/termux_wcwidth
	// which is a MODYFIED copy of https://github.com/termux/wcwidth
	// all MIT licensed

	// NOTE: created by hand, there isn't anything identifiable other than
	// general Cf category code to identify these, and some characters in Cf
	// category code are of non-zero width.
	if (ucs == 0 ||
					ucs == 0x034F ||
					(ucs > 0x200A && ucs < 0x2010) ||
					(ucs > 0x2027 && ucs < 0x202F) ||
					(ucs > 0x205F && ucs < 0x2064)) {
			return 0;
	}

	// C0/C1 control characters.
	if (ucs < 32 || (ucs > 0x07E && ucs < 0x0A0)) return -1;

	// Combining characters with zero width.
	if (intable(ZERO_WIDTH, sizeof(ZERO_WIDTH)/sizeof(struct width_interval), ucs)) return 0;

	return intable(WIDE_EASTASIAN, sizeof(WIDE_EASTASIAN)/sizeof(struct width_interval), ucs) ? 2 : 1;
}
// *** 3rd Party END ***

int StrLen(wchar_t *strIN){
	// Get terminal print-len of strIN
	
	int cLen = wcslen(strIN);		// cnt of wide chars

	int r = 0;

	for (int i = 0; i < cLen; i++){
		int wcLen = CharLen(strIN[i]);		// terminal len of wchar_t
		if (wcLen > 0) r += wcLen;
	}

	return r;

}

int SizeCanvas(int x, int y) {
    static int isSized = 0;

    // If x or y is zero, free the array, reset isSized, and return
    if (x == 0 || y == 0) {
        for (int i = 0; i < canvasMaxX; i++) {
            for (int j = 0; j < canvasMaxY; j++) {
                free(myCanvas[i][j]);
            }
            free(myCanvas[i]);
        }
        free(myCanvas);
        isSized = 0;
        canvasMaxX = canvasMaxY = 0;
        return 0;
    }

    // If x and y are not zero, allocate or realloc the array and return
    if (x > 0 && y > 0) {
		
        if (isSized) {
            myCanvas = (struct CanvasSTRUCT ***)realloc(myCanvas, x * sizeof(struct CanvasSTRUCT **));
        } else {
            myCanvas = (struct CanvasSTRUCT ***)malloc(x * sizeof(struct CanvasSTRUCT **));
        }
        isSized = 0;
		if (!myCanvas) {
            // Handle memory allocation error
            return -1;
        }

        for (int i = 0; i < x; i++) {
			myCanvas[i] = (struct CanvasSTRUCT **)malloc(y * sizeof(struct CanvasSTRUCT *));
			if (!myCanvas[i]) {
				// Handle memory allocation error
				return -1;
			}

            for (int j = 0; j < y; j++) {
				myCanvas[i][j] = (struct CanvasSTRUCT *)malloc(canvasMaxZ * sizeof(struct CanvasSTRUCT));
				if (!myCanvas[i][j]) {
					// Handle memory allocation error
					return -1;
				}

                // Initialize each element to the defaultCanvas
                for (int k = 0; k < canvasMaxZ; k++) {
                    //memcpy(&myCanvas[i][j][k], &defaultCanvas, elementSize);
					memset(&myCanvas[i][j][k], 0, sizeof(struct CanvasSTRUCT));
                }
            }
        }

        canvasMaxX = x;
        canvasMaxY = y;
        isSized = 1;
        return 0;
    }

    return -1; // Return -1 for invalid input
}

int CompareCanvasPos(int x, int y, int z1, int z2) {
    // Check if x, y, z1, and z2 are within the valid range
    if (x < 0 || x >= canvasMaxX || y < 0 || y >= canvasMaxY || z1 < 0 || z1 >= canvasMaxZ || z2 < 0 || z2 >= canvasMaxZ) {
        // Invalid indices, return -1 to indicate an error
        return -1;
    }


    if (myCanvas[x][y][z1].c != myCanvas[x][y][z2].c) {
        return 1;
    }
    else if (myCanvas[x][y][z1].style != myCanvas[x][y][z2].style) {
        return 2;
    }
    else if (myCanvas[x][y][z1].color != myCanvas[x][y][z2].color) {
        return 4;
    }
    // else if (myCanvas[x][y][z1].width != myCanvas[x][y][z2].width) {
    //    // will never happen - already covered by comparing .c
	//	return 8;
    // }

    return 0;
}
int CompareCanvasPosUnsafe(int x, int y, int z1, int z2) {

    if (myCanvas[x][y][z1].c != myCanvas[x][y][z2].c) {
        return 1;
    }
    else if (myCanvas[x][y][z1].style != myCanvas[x][y][z2].style) {
        return 2;
    }
    else if (myCanvas[x][y][z1].color != myCanvas[x][y][z2].color) {
        return 4;
    }
    // else if (myCanvas[x][y][z1].width != myCanvas[x][y][z2].width) {
    //    // will never happen - already covered by comparing .c
	//	return 8;
    // }

    return 0;
}

int CopyCanvasPos(int x, int y, int z1, int z2) {
    // Check if x, y, z1, and z2 are within the valid range
    if (x < 0 || x >= canvasMaxX || y < 0 || y >= canvasMaxY || z1 < 0 || z1 >= canvasMaxZ || z2 < 0 || z2 >= canvasMaxZ) {
        // Invalid indices, return 0 to indicate an error
        return 0;
    }

    // Copy the content of myCanvas[x][y][z1] to myCanvas[x][y][z2]
    myCanvas[x][y][z2] = myCanvas[x][y][z1];

    // Return 1 to indicate a successful copy
    return 1;
}
void CopyCanvasPosUnsafe(int x, int y, int z1, int z2) {

    // Copy the content of myCanvas[x][y][z1] to myCanvas[x][y][z2]
    myCanvas[x][y][z2] = myCanvas[x][y][z1];

}

int CopyCanvasAll(int z1, int z2) {

    if (z1 < 0 || z1 >= canvasMaxZ || z2 < 0 || z2 >= canvasMaxZ || canvasMaxX <= 0 || canvasMaxY <= 0) {
        // Invalid indices, return 0 to indicate an error
        return 0;
    }

	size_t size = sizeof(struct CanvasSTRUCT) * canvasMaxX * canvasMaxY;
    // Use a single memcpy to copy the entire array
    memcpy(&myCanvas[0][0][z2], &myCanvas[0][0][z1], size);

    // Return 1 to indicate a successful copy
    return size;
}



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
	TERM_CursorPosX = 1;
	TERM_CursorPosY = 1;
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
		if (!SizeCanvas(TERM_ScreenWidth, TERM_ScreenHeight)){
			//Locate(1,1);
			printf("Width: %d   Height: %d",TERM_ScreenWidth, TERM_ScreenHeight);
		}
		
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
			signal(SIGINT, TermSignalHandler);
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
			TermCtrlPress[event - 1]();
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
			TermFocus[event - 160]();
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
				TimeCheckOnChange();
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
					TermGetSize(3);
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
