#pragma once

//		Static TUI - Terminal Functions - See detailed description at the EOF !
//				! Using 3rd party MIT/GPL/"FreeSoftware" technologies !
//				  (see related declarations/functions and at the EOF)
// This is a  ! p_o_o_r !  Header by (c) Pit Demmer. (PIT-Licensed 16.01.2022 -          )

const char PoorMenueVersion[] = "1.00pa";

#include "AnsiESC.h"	
						

typedef struct {
	union {
		int x, left;
	};
	union {
		int y, top;
	};
}point2D;

typedef struct {
	union {
		int x, width;
	};
	union {
		int y, height;
	};
}size2D;

enum scrollENUM {

	Off, On, Auto
};
enum stickENUM {

	// Off
	Top = 1, Bottom, Left, Right
};

// Menue Stuff
struct NavStyleNameSTRUCT {
	char str[32]; // @STATIC@
}; struct NavStyleNameSTRUCT navStyleName[255];		// @STATIC@
struct FrameStyleNameSTRUCT {
	char str[32]; // @STATIC@
}; struct FrameStyleNameSTRUCT frameStyleName[255];	// @STATIC@
struct FrameNameSTRUCT {
	char str[256]; // (that long, cause it could be a pathname)@STATIC@
}; struct FrameNameSTRUCT frameName[32];	// @STATIC@

struct ValToTxtSTRUCT {
	
	_Bool isFloat;
	_Bool hasChanged;			// gets true if value formatting loop recognizes change - set false after print
	byte precision;				// decimal places
	byte leading;				// full length including leading 0
	byte style;					// 1 = MSB-IOOIIIOI | 2 = LSB-IOIIIOOI 
								// 3 = MSB - I O O I I I O I  |  4 = LSB - I O I I I O O I 
								// 5 = Hex : FFFF | 6 = Hex: ffff
								// 7 = 1e | 8 = 1E | 9 = I/O | 10 = On/Off | 11 = True/False
	int intVal;
	float floatVal;
	int intVallast;
	float floatVallast;
};
struct StringVal16STRUCT {

	struct ValToTxtSTRUCT val2txt;
	char str[16];
}; struct StringVal16STRUCT val16[255];			// @STATIC@
struct StringVal32STRUCT {

	struct ValToTxtSTRUCT val2txt;
	char str[32];
}; struct StringVal32STRUCT val32[255];			// @STATIC@
struct StringVal64STRUCT {

	struct ValToTxtSTRUCT val2txt;
	char str[64];
}; struct StringVal64STRUCT val64[255];			// @STATIC@

struct StringArray16STRUCT {
	char str[16]; // @STATIC@
}; struct StringArray16STRUCT str16[255];			// @STATIC@
struct StringArray32STRUCT {
	char str[32]; // @STATIC@
}; struct StringArray32STRUCT str32[255];			// @STATIC@
struct StringArray64STRUCT {
	char str[64]; // @STATIC@
}; struct StringArray64STRUCT str64[255];			// @STATIC@
struct StringArray128STRUCT {
	char str[128]; // @STATIC@
}; struct StringArray128STRUCT str128[255];			// @STATIC@
struct StringArray256STRUCT {
	char str[256]; // @STATIC@
}; struct StringArray256STRUCT str256[255];			// @STATIC@
struct StringArray512STRUCT {
	char str[512]; // @STATIC@
}; struct StringArray512STRUCT str512[1];			// @STATIC@
struct StringArray1024STRUCT {
	char str[1024]; // @STATIC@
}; struct StringArray1024STRUCT str1024[1];		// @STATIC@
struct StringArray2048STRUCT {
	char str[2048]; // @STATIC@
}; struct StringArray2048STRUCT str2048[1];	   	// @STATIC@

struct FrameStyleSTRUCT {


	byte Head;										// 0 = // N/A (no borders at all)
													// 1 = // SPACES (side lines too)
													// 1 = // --------------------------------------------------
													// 2 = // ---------- Game Of Life  -  Math Menue -----------
													// 3 = // --------------------------------------------------	// OldSchool
													//        |          Game Of Life  -  Math Menue           |
													//        --------------------------------------------------

	/*
	


	
	*/


	// Selected/Active
	char tLc[3], tRc[3], bLc[3], bRc[3];			// corner elements
	char lT[3], rT[3];								// left/right T's (e.g. 2nd line OldSchool Header)
	char tH[3], bH[3];								// horz top & bottom lines
	char lV[3], rV[3];								// sidelines
	char xL[3], xR[3];								// sidelines touching horz. seperator
	char xT[3], xB[3];								// top & bottom touching vert. seperator

	/*
	Nope, we do this just via colors

	// Not Selected/Active / Disabled.
	char tLcD[3], tRcD[3], bLcD[3], bRcD[3];		// corner elements
	char lTD[3], rTD[3];							// left/right T's (e.g. 2nd line OldSchool Header)
	char tHD[3], bHD[3];							// horz top & bottom lines
	char lVD[3], rVD[3];							// sidelines
	char xLD[3], xRD[3];							// sidelines touching horz. seperator
	char xTD[3], xBD[3];							// top & bottom touching vert. seperator
	*/
	char xVHt[3], xVHb[3];							// T-x-points seperators top & bottom
	char xVHl[3], xVHr[3];							// T-x-points seperators left & right
	char xX[3];										// straight through + point
	char sepH[3];									// horz. seperation line
	char sepV[3];									// vert. seperation line
	char scrV[3], scrH[3];							// scroll - elements
	char scrU[3], scrD[3];
	char scrL[3], scrR[3];

	int navID;										// navigation ID on navigationStyleSTRUCT[]

	int cName;										// color ID on TxtColorSTRUCT[]
	int cFrame;										// Appearance when active
	int cFrameDis;									// Appearance when inactive
	int cBack;
	int cSep;
	int cScroll;

};struct FrameStyleSTRUCT frameStyle[32];		// @STATIC@

struct NavigationStyleSTRUCT {

	char navMin[3], navMax[3], navNorm[3],		// navigation icons
		navClose[3], navSizeH[3], navSizeV[3],
		navMoveH[3], navMoveV[3];
	int navMin_C, navMax_C, navNorm_C,			// navigation color-ID (active)
		navClose_C, navSize_C, navMove_C;
	/*
	int navMin_D, navMax_D, navClose_D,			// navigation colors (inactive) @NONSENSE@ - we do same as Frame
		navSizeH_D, navSizeV_D,
		navMoveH_D, navMoveV_D;
	*/

	char hKL[3], hKR[3];						// HotKey (inner) Brackets
	char mnuL[3], mnuR[3];						// Menue (outer) Brackets
	int navHk_C;								// ID to TxTStyleSTRUCT HotKey Brackets
	int mnu_C;									// ID to TxTStyleSTRUCT Menue Brackets 
}; struct NavigationStyleSTRUCT naviStyle[8];	// @STATIC@

struct FrameStruct {

	// unsigned char[3] hotKey;						// TAB  /  F1 - F12  /  ^char  /  char  /  
		// Ctrl-TAB jumps in/out window | TAB jumps to next window (if outside)
		// Window hotKeys getting active on window selection (no jump-in needed)
		// active/inactive Hotkeys are (actually) the only optical sign on active/inactive

	int name;											// ID to FrameNameSTRUCT
	int style;											// ID to FrameStyleStruct[]
	char hK[3];											// HotKey to select this frame 
	int parent;											// ID to parent frame
	int childStart;										// ID to 1st child frame
	int childStop;

	_Bool modal;

	// 'Icon' Navigation Elements
	_Bool navMin, navMax, navClose, navSize, navMove;	// if they get rendered
	_Bool canMax, canMin, canMove, canClose, canSize;	// if they're useable

	int start[10];										// First Line of FrameContentStruct
	int stop[10];										// Last Line

	point2D pos;										// Position (== 0 - then we center)
	size2D size;										// Size (== 0 - then we min (defined by elements) auto-size)
	size2D minSize;
	size2D maxSize;										// >9999 = 'zoom' to max

	byte scrollV, scrollH;								// 0 = Off | 1 = ON | 2 = Auto

	byte stick;											// 0 = Off | 1 = stickTop | 2 = stickBottom
														// 3 = stickLeft | 4 = stickRight


	point2D actPos;										// last rendering
	size2D actSize;

}; struct FrameStruct frame[32];	// @STATIC@

struct FrameContentSTRUCT {

	char hK[3];						
	_Bool hKleft;					// Line starts with HotKey 
	_Bool disabled;					// Grey the Val / disable Hotkey (independent of window enabled/disabled)
	_Bool invisible;				// don't show (but react on) HotKey
							  

	// Values and 'Captions'
	int valStr;						// id of Array-Pos with val2text
	int valArr;						// id of used val2text Array [16] - [32] - [64]... 
	int textStr;					// id of Array-Pos with text
	int textArr;					// id of used Array

	// Actually rendered positions - for fast changing values / HotKey 
	point2D hKpos;
	size2D hKsize;
	point2D valPos;
	size2D valSize;
	
	char on_I[3], off_O[3];			// replace I/0 with given chars (in binary vals)
	int onStyle, offStyle;			// ID to TxtStyleSTRUCT

	int txtStyle, valStyle;			// ID to TxtStyleSTRUCT´s


	// Use content
	byte contentUse;				//    0 = // |                                                | // Space/Empty
									//    1 = // | text_str                                       | // just txt
									//    2 = // | text_str                             : val_str | // txt & val
									//    4 = // | text_str                                : (hK) | // txt & hK
									//    8 = // | text_str       : val_str                  (hK) | // txt & hK
									//  +32 = // | text_str       : (hk) | ...........    : (...) | // Multiple Val/Option/Info
									//   64 = // |------------------------------------------------| // Horizontal Split (touch) 	
									//   65 = // | ---------------------------------------------- | // Horizontal Split 
									//   66 = // |================================================| //	(connected) like the 2nd 'OldScool' split-line
									// +128 = // |        [(t)ext_str] [(.)...] [Help(?)]         | // Menue (first char in str == hK[] get the HotKeyBrackets
																								 // If hK[] is missing hK gets attached

	byte multiPos;					// pos of this content in multi-line/menue
	byte multiCnt;					// count of all contents in this line
	byte multiLen;					// full len of menue (without spaces, brackets, hK)

}; struct FrameContentSTRUCT frameContent[255]; // @STATIC@


// mnuDef ID's for a AutoMenueFunction
int MainMenueStart = 0;
int MainMenueStop = 0;
_Bool autoMenuON = 0;

struct MenuesRunningSTRUCT {

	int mnuID;										// Link on menueDef

	int top, left;									// Collision control on rendered sizes // Delete
	int height, width;								

	_Bool active;									

	_Bool dirty;									// dirty (got touched by overlaps/moves/deletions)

}; struct MenuesRunningSTRUCT runningMenues[32];	// @STATIC@

int stickTop = 0, stickLeft = 0;					// Already used Lines & Rows in sticking generation
int stickBottom = 0, stickRight = 0;
int stickSplitTop = 0, stickSplitLeft = 0;			// Already used width/height while split generation
int stickSplitBottom = 0, stickSplitRight = 0;

int mnuActive = 0;									// Link on active menueDef when GetValidMenueKey returns
													// Link on last active window while GetValidMenueKey is runnig



unsigned char GetValidMenueKey(void) {

	unsigned char usrChar = 0;

	// Manages the whole window select/deselect 
	// Returns with a valid HotKey only (and mnuActive)

	// @MISSING@ Jump to a User - Function 

	// Set last on Disabled

	return usrChar;
}

void PrintMenues(int idStart, int idStop, int mode) {

	//            @SLOW@ Prognosis for mode 1 + 2 @SLOW@
	// (If it's not 'accidently' getting fast by itself while coding.)

	// mode = 0 Full Generation (last as active)
	//  ''  = 1 Active HotKeys (just for one)
	//  ''  = 2 Disabled HotKeys (for all if more)
	//  ''  = 3 Highlight Value (just for one)

}

void DeleteMenues(int idStart, int idStop) {

	//            @SLOW@ Prognosis for mode 1 + 2 @SLOW@
	// (If it's not 'accidently' getting fast by itself while coding.)

	// mode = 0 Full Generation (as active)
	//  ''  = 1 Active HotKeys
	//  ''  = 2 Disabled HotKeys

}

void UpdateMenueValues(int idStart, int idStop) {

}

void PrintMathMenue(void) {
	/*
	012345678901234567890123456789012345678901234567890 (50 x 13)
	1--------------------------------------------------
	2|          Game Of Life  -  Math Menue           |
	3|------------------------------------------------|
	4| Binary Vitality   : true                   (0) |
	5| Aging Mode        : false                  (1) |
	6| Aging Start '> x' : 00000                  (2) |
	7|                                                |
	8| Birth Start '> x' : 00000                  (3) |
	9| Birth Stop  '< x' : 00000                  (4) |
	0| Dead Start  '> x' : 00000                  (5) |
	1|                                                |
	2|        (Back) (Resume) (Start) (Help)          |
	3--------------------------------------------------
	*/
}

void PrintMainMenue(void) {

	/*
	012345678901234567890123456789012345678901234567890 (50 x 8)
	1--------------------------------------------------
	2|          Game Of Life  -  Main Menue           |
	3|------------------------------------------------|
	4| Math            : (0) | Screen         :   (1) |
	5| StartConditions : (2) | StopConditions :   (3) |
	6|                                                |
	7|        (Quit) (Resume) (Start) (Help)          |
	8|------------------------------------------------|
	*/

	ClrScr();
	GetTerminalSize();

	int offset_X = (ScrX - 50) / 2;
	int offset_Y = (ScrY - 10) / 2;


}

/*
										EOF - Detailed Description

<PoorMenue.h>	is the main/only Header to include into your project to get the whole  ! p_o_o_r ! - MENUE functionality.
				
				This library is part of something bigger!
				Partly made in a way that I can later easy downgrade/reuse parts of it for AVR.

				Lib-State   : @DEV@@Pit@
				Lib-Version : 1.00pa
				Lib-Date    :

				Dev-Start   : 16.01.2022
				1st Alpha   :
				1st Beta    :
				1st Release :
*/

/*
Development Rules & KeyWords:

	They're actually acting like manually compiling directives... So, welcome to the 60s - lol
	DO NEVER REMOVE the @SOMETHING@ remarks! Even not in your destination apps!
	It is part of my MIT License conditions to not remove them from my sources!

	@STATIC@
		This Software is made with a STATIC Plain-C Library. But the .C file should not contain it.
		Look for more in the .h files.

	@WIN@ @LINUX@ @MAC@ @AVR@ @ARM@
		There is something to respect for that OS

	@NONCRITICAL@ see example:
		// @NONCRITICAL@@SECURITY@ Pit, 17.01.2022
		// Works perfect - But on 2nd day - I got the idea it could be (in todays times) an security issue to do such system-calls ??
		// @Pit
		// @NONCRITICAL@@SECURITY@

	@SECURITY@
		Self explaining - most time used together with a 'rating' - see previous example

	@CRITICAL@
		A critical part (not really buggy or not working but 'something' is or could be there)

	@BUG@
		There is a known and documented bug. With a need to get fixed.

	@BUGGY@
		There is a known and documented bug. With low priority to get fixed.

	@WTF@
		Something is wrong - we've no real clue why. And we stick this (with tracking) allover where we
		recognizing, expecting or knowing something about that problem.

	@DIRTY@
		Here is a dirty made fix.
		with @OFF@: Here is/was a dirty fix.

	@NEW@
		Totally new code/function

	@ALPHA@
		Developer tests were looking good.

	@BETA@
		Alpha survid the public, other developer, new implementations and major fixes directly interacting with this alpha code.

	@FIX@
		Something got documented and the right way fixed. It then starts with @Alpha@ and same rules like new code.

	@MISSING@
		We miss something in the kind of extending/upgrading existing funtionality.

	@FUTURE@
		We miss something elementary we would like to have in the next major version step.

	@LEAVEOUT@
		We miss something for tactical/time development reasons...

	@FEATURE@
		A given functionality/behaviour could be misinterpreted as a bug - but it's a feature...

	@OFF@
		Attached to a 'rating' = we disabled the function.
		If there is a dirty fix instead there will/must be a kind of tracking number.

	@DEV@@Pit@
		Active in Pits Developing

	@TEST@
		Pre-Alpha state. Developer ist testing and expects has at least alpha quality.

	@SLOW@
		Perfect functional code - but (too) slow

	@INVESTIGATE@
		Research needed to understand 'something' or to fulfill standards or 'something like that'

	@0000-000@
		Tracking number. Up to 999 tries to solve 1 out of 9999 bugs.

*/


