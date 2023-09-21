
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

// Collector for all strings of EscColor and EscStyle

// Standard, but optional, ESC Sequences
char RUL[] = "24";				// 'ResetUnderLine' alternativ: "4:0" 

// Actual Settings / States
static EscStyleSTRUCT ActTxtStyle;
static EscColorSTRUCT ActTxtColor;

// global access on colors and styles
EscColorSTRUCT *userColors = NULL;
EscStyleSTRUCT *userStyles = NULL;

void ResFBU(void);
void SetColorStyle(EscColorSTRUCT *pColor, int set);
void SetTxtStyle(EscStyleSTRUCT *pStyle, int set);
void LocateX(int x);
void SetFg255(int c);
void SetBg255(int c);
void TxtDblWidth(int set);
void TxtDblTop(int set);
void TxtDblBot(int set);

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

void ESCinit(void) {

	// The 24 Greys (+ Black/White) from 256 Palette	
	ESC_Grey[1] = 15; ESC_Grey[26] = 0;
	int p = 1;
	for (int t = 255; t > 231; t--) {
		p++;
		ESC_Grey[p] = t;
	}

	// Default ColorStyle
	// ActTxtStyle.pColor = &ActTxtColor;
}

int ESCinitColors(char *strFile, EscColorSTRUCT **userColor){

	// Users Colors - Helper
	char strSearch[STR_SMALL_SIZE];
	char strGroupName[STR_SMALL_SIZE];
	char strColorName[STR_SMALL_SIZE];
	char strHLP[STR_SMALL_SIZE];

	// count of color groups
	int colorsGroupsCount = IniGetInt(strFile, "global.colors.GroupCount", 0);
	// groups color model
	int colorsModel = IniGetInt(strFile, "global.colors.ColorModel", 0);
	
	// memory to store count of colors in each group
	int *colorsCount;
	colorsCount = (int*)malloc(colorsGroupsCount * sizeof(int));
	
	int colorsCountSum = 0;

	// read counts of colors and calc sum
	for (int i = 0; i < colorsGroupsCount; i++){
		sprintf(strSearch, "global.colors.group%d.Count", i + 1);
		colorsCount[i] = IniGetInt(strFile, strSearch, 0);
		colorsCountSum += colorsCount[i];
	}
	
	// memory to store all colors
	*userColor = (EscColorSTRUCT*)malloc(colorsCountSum * sizeof(EscColorSTRUCT));

	colorsCountSum = 0;

	for (int i = 0; i < colorsGroupsCount; i++){
		sprintf(strSearch, "global.colors.group%d.Name", i + 1);
		sprintf(strHLP, "Group%d", i + 1);
		IniGetStr(strFile, strSearch, strHLP, strGroupName);
		for (int j = 0; j < colorsCount[i]; j++){

			(*userColor)[colorsCountSum].groupName = IniStrToMem(strGroupName, 0);
			(*userColor)[colorsCountSum].groupID = i + 1;

			sprintf(strSearch, "global.colors.group%d.%d.Name", i + 1, j + 1);
			sprintf(strHLP, "Color%d", j + 1);
			IniGetStr(strFile, strSearch, strHLP, strColorName);
			(*userColor)[colorsCountSum].colorName = IniStrToMem(strColorName, 0);
			(*userColor)[colorsCountSum].colorID = j + 1;

			sprintf(strSearch, "global.colors.group%d.%d.ForeGround", i + 1, j + 1);
			(*userColor)[colorsCountSum].fg.Color = IniGetInt(strFile, strSearch, 15);
			sprintf(strSearch, "global.colors.group%d.%d.BackGround", i + 1, j + 1);
			(*userColor)[colorsCountSum].bg.Color = IniGetInt(strFile, strSearch, 0);
			sprintf(strSearch, "global.colors.group%d.%d.UnderLine", i + 1, j + 1);
			(*userColor)[colorsCountSum].ul.Color = IniGetInt(strFile, strSearch, 15);
			
			(*userColor)[colorsCountSum].mode = colorsModel;
			
			// printf("%04d. %s_%s:\n",colorsCountSum + 1, strGroupName, strColorName);
			printf("%04d. %s_%s: ", colorsCountSum , (*userColor)[colorsCountSum].groupName, (*userColor)[colorsCountSum].colorName);

			colorsCountSum++;

			LocateX(36);
			SetColorStyle(&(*userColor)[colorsCountSum - 1], 1);
			//printf("Res - Done\n");
			//return;
			printf("->   (%03d:%03d)   <-", (*userColor)[colorsCountSum - 1].fg.Color, (*userColor)[colorsCountSum - 1].bg.Color);
			fflush(stdout);
			ResFBU();
			printf("\n");
			fflush(stdout);
		}
		printf("\n");
		fflush(stdout);		
	}

	/*
	for (size_t i = 0; i < colorsCountSum; i++)
	{
		SetColorStyle(&(*userColor)[i], 1);
		printf("%04d. %s.%s\n", i, (*userColor)[i].groupName, (*userColor)[i].colorName);
	}
	*/

	free(colorsCount);
	return colorsCountSum;
}

int ESCinitTxtStyles(char *strFile, EscStyleSTRUCT **userTxtStyles){

	// Users Colors
	char strSearch[STR_SMALL_SIZE];
	char strGroupName[STR_SMALL_SIZE];
	char strStyleName[STR_SMALL_SIZE];
	char strHLP[STR_SMALL_SIZE];

	//char strFile[] = "desktops.ini";
	int stylesGroupsCount = IniGetInt(strFile, "global.txtStyles.GroupCount", 0);
	
	int *stylesCount;
	stylesCount = (int*)malloc(stylesGroupsCount * sizeof(int));
	
	int stylesCountSum = 0;

	for (int i = 0; i < stylesGroupsCount; i++){
		sprintf(strSearch, "global.txtStyles.group%d.Count", i + 1);
		stylesCount[i] = IniGetInt(strFile, strSearch, 0);
		stylesCountSum += stylesCount[i];
	}
	
	*userTxtStyles = (EscStyleSTRUCT*)malloc(stylesCountSum * sizeof(EscStyleSTRUCT));
	
	stylesCountSum = 0;

	for (int i = 0; i < stylesGroupsCount; i++){
		sprintf(strSearch, "global.txtStyles.group%d.Name", i + 1);
		sprintf(strHLP, "Font%d", i + 1);
		IniGetStr(strFile, strSearch, strHLP, strGroupName);
		for (int j = 0; j < stylesCount[i]; j++){

			(*userTxtStyles)[stylesCountSum].fontName = IniStrToMem(strGroupName, 0);
			(*userTxtStyles)[stylesCountSum].fontID = i + 1;

			sprintf(strSearch, "global.txtStyles.group%d.%d.Name", i + 1, j + 1);
			sprintf(strHLP, "Style%d", j + 1);
			IniGetStr(strFile, strSearch, strHLP, strStyleName);
			(*userTxtStyles)[stylesCountSum].styleName = IniStrToMem(strStyleName, 0);
			(*userTxtStyles)[stylesCountSum].styleID = j + 1;

			sprintf(strSearch, "global.txtStyles.group%d.%d.bold", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].bold = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.faint", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].faint = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.italic", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].italic = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.blink", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].blink = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.fastblink", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].fast = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.reverse", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].reverse = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.invisible", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].invisible = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.strikethrough", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].strike = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.superscript", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].superscript = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.subscript", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].subscript = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.proportional", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].proportional = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.framed", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].framed = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.encircled", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].encircled = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.overline", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].overline = IniGetBool(strFile, strSearch, 0);

			/*
			sprintf(strSearch, "global.txtStyles.group%d.%d.ideoright", i + 1, j + 1);
			userTxtStyles[stylesCountSum].ideo_right = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.ideoleft", i + 1, j + 1);
			userTxtStyles[stylesCountSum].ideo_left = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.ideodblright", i + 1, j + 1);
			userTxtStyles[stylesCountSum].ideo_dbl_right = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.ideodblleft", i + 1, j + 1);
			userTxtStyles[stylesCountSum].ideo_dbl_left = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.ideostress", i + 1, j + 1);
			userTxtStyles[stylesCountSum].ideo_stress = IniGetBool(strFile, strSearch, 0);
			*/

			sprintf(strSearch, "global.txtStyles.group%d.%d.dblwidth", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].dbl_width = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.dblheight", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].dbl_height = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.font", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].font = IniGetInt(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.underline", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].underline = IniGetInt(strFile, strSearch, 0);

		    // *userTxtStyles)[stylesCountSum].pColor = &ActTxtColor;

			printf("%04d. %s_%s: ",stylesCountSum + 1, (*userTxtStyles)[stylesCountSum].fontName, (*userTxtStyles)[stylesCountSum].styleName);

			stylesCountSum++;

			LocateX(45);
			SetTxtStyle(&(*userTxtStyles)[stylesCountSum - 1], 1);
			if ((*userTxtStyles)[stylesCountSum - 1].dbl_height){
				printf("\n");
				TxtDblTop(1);
				printf("->   ( TEST )   <-\n");
				TxtDblBot(1);
				printf("->   ( TEST )   <-");
			}
			else if ((*userTxtStyles)[stylesCountSum - 1].dbl_width){
				printf("\n");
				TxtDblWidth(1);
				printf("->   ( TEST )   <-");				
			}
			else{
				printf("->   ( TEST )   <-");
			}					
			fflush(stdout);
			SetTxtStyle(&(*userTxtStyles)[stylesCountSum - 1], 0);
			printf("\n");
			fflush(stdout);
		}
		printf("\n");
		fflush(stdout);		
	}

	free(stylesCount);
	
	return stylesCountSum;
}

// Cursor Positions
void Locate(int x, int y) {
	// printf("%c[%d;%df", 0x01b, y, x);		//('f' instead 'H') is working, at least on WIN, too. (more... see wikipedia 'ANSI-ESC') 
	printf("\x1B[%d;%dH", y, x);
	TERM_CursorPosY = y;
	TERM_CursorPosX = x;
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
void CursorMoveY(int y){
	if (y < 0){
		// Down
		CursorDown(y * -1);
	}
	else if (y > 0){
		// Up
		CursorUp(y);
	}
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
void CursorMoveX(int x){
	if (x < 0){
		// Left
		CursorLeft(x * -1);
	}
	else if (x > 0){
		// Right
		CursorRight(x);
	}
}
void GetAnsiCursorPos(void){
	TERM_CursorWaitFor = 1;
	printf("\x1B[6n");
}

#define CursorMoveXY(x, y) CursorMoveX(x); CursorMoveY(y)

// DEC BoxDraw
/*
DEC char to BoxDraw:
		printf("a b c d e f g h i j k l m n o p q r s t u v w x y z\n");
		printf("\x1B(0"); // enable BoxDraw
		printf("a b c d e f g h i j k l m n o p q r s t u v w x y z\n");
		printf("\x1B(B"); // disable BoxDraw
		a b c d e f g h i j k l m n o p q r s t u v w x y z
		▒ ␉ ␌ ␍ ␊ ° ± ␤ ␋ ┘ ┐ ┌ └ ┼ ⎺ ⎻ ─ ⎼ ⎽ ├ ┤ ┴ ┬ │ ≤ ≥

If frames are overlapping, it's just a simple bitwise OR to get the
right char for displaying the crossing point right.

(00)  = n/a					
(01)  = n/a					(Top)
(02)  = n/a					(Bottom)
(03) │ = Top-Bottom			
(04)  = n/a					(Right)
(05) └ = Top-Right
(06) ┌ = Bottom-Right
(07) ├ = Top-Right-Bottom
(08)  = n/a					(Left)
(09) ┘ = Top-Left
(10) ┐ = Bottom-Left
(11) ┤ = Top-Bottom-Left
(12) ─ = Left-Right
(13) ┴ = Left-Right-Top
(14) ┬ = Bottom-Left-Right
(15) ┼ = Top-Right-Bottom-Left

*/
static char TuiDecBoxDraw[16] = {' ', ' ', ' ', 'x', ' ', 'm',
									'l', 't', ' ', 'j', 'k', 'u',
									'q', 'v', 'w', 'n' };

void DECboxMode(int set){
	if (set){
		printf("\x1B(0"); // enable BoxDraw Mode
	}
	else{
		printf("\x1B(B"); // disable BoxDraw Mode
	}
	fflush(stdout);
}
#define DECboxON DECboxMode(1)
#define DECboxOFF DECboxMode(0)
void DEClineX(int len){

	int isLastLine = 0;

	#if __APPLE__
		// we're fine
	//#elif __WIN32__ || _MSC_VER || __WIN64__
		// no clue actually 
	#else
		// we can't print regularly, if we're on the last column 
		if (TERM_CursorPosX == TERM_ScreenWidth){
			isLastLine = 1;
		}		
	#endif

	DECboxON;

	if (len < 0){
		// Right to left
		for (int i = 0; i > len; i--){
			printf("q");
			if (!isLastLine){
				CursorLeft(2);
			}
			else{
				CursorLeft(1);
				isLastLine = 0;
			}			
		}
	}
	else{
		// Left to right
		StrPrintChars('q', len);
	}
	
	DECboxOFF;

}
void DEClineY(int len){
	
	DECboxON;

	int isLastLine = 0;

	#if __APPLE__
		// we're fine
	//#elif __WIN32__ || _MSC_VER || __WIN64__
		// like linux 
	#else
		// we can't print regularly, if we're on the last column 
		if (TERM_CursorPosX == TERM_ScreenWidth){
			isLastLine = 1;
		}		
	#endif

	if (len < 0){
		// bottom to top
		for (int i = 0; i > len; i--){
			printf("x");
			if (!isLastLine){
				CursorLeft(1);
			}
			CursorUp(1);
		}
	}
	else{
		// top to bottom
		while (len--){
			printf("x");
			if (!isLastLine){
				CursorLeft(1);
			}
			CursorDown(1);
		}
	}
	
	DECboxOFF;
		
}
void DEClineXY(int startX, int startY, int stopX, int stopY, int newLine){
	
	// if newLine = true 
	//	startX & startY are working like "moveTo"
	//	stopX & stopY are working like "lineTo"
	// if newLine = false
	// 	startX & startY getting replaced by lastX & lastY
	//	intersection style of connection points will be calculated

	// EDGE logic:
	// 1st element	from where in X
	//				to where in Y
	// 2nd element	from where in Y
	//				to where in X

	int isLastLine = 0;			// to handle the last column - issue

	static double lastX = 0;
	static double lastY = 0;
	static double lastXcut = 0;
	static double lastYcut = 0;
	static int lastDirection = 0;	// 0 = NA
									// 1 = left to right
									// 2 = right to left
									// 4 = top to down
									// 8 = down to top
	if (newLine){
		// (1st) line
		lastX = 0;
		lastY = 0;
		lastXcut = 0;
		lastYcut = 0;
	}
	else{
		// lineTo from last point
		startX = lastX;
		startY = lastY;
	}
	
	double spX = startX;
	double spY = startY;
	double epX = stopX;
	double epY = stopY;
	
	int r = LineInRect(&spX, &spY, &epX, &epY, 1, 1, TERM_ScreenWidth, TERM_ScreenHeight);

//printf("r: %d\n");

	if (r){
		// We have a line to draw

		// back to int
		startX = (spX + 0.5);
		startY = (spY + 0.5);
		stopX = (epX + 0.5);
		stopY = (epY + 0.5);

		Locate(startX, startY);

		#if __APPLE__
			// we're fine
		//#elif __WIN32__ || _MSC_VER || __WIN64__
			// like linux 
		#else
			// we can't print regularly, if we're on the last column 
			if (TERM_CursorPosX == TERM_ScreenWidth){
				isLastLine = 1;
			}		
		#endif

		int deltaY = stopY - startY;			
		int deltaX = stopX - startX;

		if (r == 1){
			// horizontal line
			lastX = startX + deltaX;
			lastY = startY;
			if (lastY > TERM_ScreenHeight){
				lastY = TERM_ScreenHeight;
			}
			else if (lastY < 1){
				lastY = 1;
			}
			if (lastX > TERM_ScreenWidth){
				lastX = TERM_ScreenWidth;
			}
			else if (lastX < 1){
				lastX = 1;
			}
			if (deltaX > 0){
				deltaX++;
				lastDirection = 1;		// left to right
			}
			else{
				deltaX--;
				lastDirection = 2;		// right to left
			}
			DEClineX(deltaX);
		}
		else if (r == 2){
			// vertical line
			lastY = startY + deltaY;
			lastX = startX;
			if (deltaY > 0){
				deltaY++;
			}
			else{
				deltaY--;
			}
			DEClineY(deltaY);
		}
		else if (r == 3){
			// diagonal line
//printf("YES\n");
			int absDeltaY = abs(deltaY);
			int absDeltaX = abs(deltaX);

			// Define the to use stepping edges - see binary direction coding in: char TuiDecBoxDraw[16]
			int decEdge1st = 5;		// Right to left & down to top pre-defined
			int decEdge2nd = 10;	// Right to left & down to top pre-defined
			int moveX = -2;
			int moveY = 1;

			if (deltaX > 0){
				// left to right
				decEdge1st += 4;
				decEdge2nd -= 4;
				moveX = 0;
			}
			if (deltaY > 0){
				// top to down
				decEdge1st += 1;
				decEdge2nd -= 1;
				moveY = -1;
			}

			int steps = absDeltaX;					// The shorter delta is needed - more vertical predefined
			int missing = absDeltaY - absDeltaX; 	// more vertical predefined...
			int misPerStep = 0;						// count of connectors on all steps
			int misRest = 0;						// count of missing connectors
			int misStep = 0;						// every misStep steps we add an additional connector
			int insertAt = 1;						// 0 = insert connectors pre 1st edge
													// 1 = insert connectors past 1st edge

			int decConnector = 3;					// top-bottom (more vertical)

			if (absDeltaX > absDeltaY){
				// more horizontal
				steps = absDeltaY;
				missing = absDeltaX - absDeltaY;
				decConnector = 12;
				insertAt = 0;
			}

			if (missing){
				// we're not 45°
				misPerStep = missing / steps;
				misRest = missing % steps;
				if (misRest){
					misStep = steps / misRest;
				}
			}
			else{
				decConnector = 0;
			}
			
			DECboxON;
			
			if (misRest && !insertAt){
				// misRest pre 1st edge
				misRest--;
				printf("%c",TuiDecBoxDraw[decConnector]);
				CursorMoveX(moveX);
			}

			printf("%c", TuiDecBoxDraw[decEdge1st]);
			CursorMoveXY(-1, moveY);

			int misRestStep = misStep;
			for (int i = 0; i < steps; i++){
				misRestStep--;
				
				
				if (misPerStep && insertAt){
					// regular connectors after 1st edge
					for (int j = 0; j < misPerStep; j++){
						printf("%c",TuiDecBoxDraw[decConnector]);
						CursorMoveXY(-1, moveY);
					}
				}
				

				// 2nd edge
				printf("%c", TuiDecBoxDraw[decEdge2nd]);
				CursorMoveX(moveX);
				//CursorMoveXY(moveX, moveY);
				
				
				if (misPerStep && !insertAt){
					// regular connectors pre 1st edge
					for (int j = 0; j < misPerStep; j++){
						printf("%c",TuiDecBoxDraw[decConnector]);
						CursorMoveX(moveX);
					}
				}
				if (misRest && !insertAt && !misRestStep){
					// misRest pre 1st edge
					misRest--;
					printf("%c",TuiDecBoxDraw[decConnector]);
					CursorMoveXY(moveX, 0);
				}
				

				// 1st edge
				printf("%c", TuiDecBoxDraw[decEdge1st]);
				CursorMoveXY(-1, moveY);

				if (misRest && insertAt && !misRestStep){
					// misRest after 1st edge
					misRest--;
					printf("%c",TuiDecBoxDraw[decConnector]);
					CursorMoveXY(-1, moveY);
				}

				if (!misRestStep){
					misRestStep = misStep;
				}
				
			}
			DECboxOFF;
//printf("moveX: %d  moveY: %d", moveX, moveY);
		}
	}
}
void DECrect(int startX, int startY, int stopX, int stopY){
	
	// normalize rect

	int temp = 0;
	if (stopX < startX){
		temp = stopX;
		stopX = startX;
		startX = temp;
	}
	if (stopY < startY){
		temp = stopY;
		stopY = startY;
		startY = temp;
	}

	if (PointInRect(startX, startY, 1, 1, TERM_ScreenWidth, TERM_ScreenHeight) && PointInRect(stopX, stopY, 1, 1, TERM_ScreenWidth, TERM_ScreenHeight)){
		// rect is fully on screen - simplified creation
		Locate(startX, startY);
		DECboxON;
		printf("l");					// top-left edge
		DEClineX(stopX - startX - 1);
		DECboxON;
		printf("k");
		
		Locate(stopX, startY + 1);
		DEClineY(stopY - startY - 1);
		DECboxON;
		printf("j");					// bottom-right edge
				
		Locate(stopX - 1, stopY);
		DEClineX(startX - stopX + 1);
		DECboxON;
		printf("m");					// bottom-left edge
				
		Locate(startX, stopY - 1);
		DEClineY(startY - stopY + 1);
	}
	else{
		// (parts of) the rect are out of screen
		
		// Horz top
		double spX1 = startX;
		double spY1 = startY;
		double epX1 = stopX;
		double epY1 = startY;

		// Vert right
		double spX2 = stopX;
		double spY2 = startY;
		double epX2 = stopX;
		double epY2 = stopY;

		// Horz bottom
		double spX3 = stopX;
		double spY3 = stopY;
		double epX3 = startX;
		double epY3 = stopY;

		// Vert left
		double spX4 = startX;
		double spY4 = stopY;
		double epX4 = startX;
		double epY4 = startY;

		int horzTop = LineInRect(&spX1, &spY1, &epX1, &epY1, 1, 1, TERM_ScreenWidth, TERM_ScreenHeight);
		int vertRight = LineInRect(&spX2, &spY2, &epX2, &epY2, 1, 1, TERM_ScreenWidth, TERM_ScreenHeight);
		int horzBot = LineInRect(&spX3, &spY3, &epX3, &epY3, 1, 1, TERM_ScreenWidth, TERM_ScreenHeight);
		int vertLeft = LineInRect(&spX4, &spY4, &epX4, &epY4, 1, 1, TERM_ScreenWidth, TERM_ScreenHeight);

		// round to 'int'...
		spX1 += 0.5; spX2 += 0.5; spX3 += 0.5; spX4 += 0.5; 
		spY1 += 0.5; spY2 += 0.5; spY3 += 0.5; spY4 += 0.5; 

		// Draw valid lines
		if (horzTop){
			Locate(spX1, spY1);
			DEClineX(epX1 - spX1 + 2);
		}
		if (vertRight){
			Locate(spX2, spY2);
			DEClineY(epY2 - spY2 + 2);
		}
		if (horzBot){
			Locate(spX3, spY3);
			DEClineX(epX3 - spX3);
		}
		if (vertLeft){
			Locate(spX4, spY4);
			DEClineY(epY4 - spY4 - 1);
		}

		// draw valid edges
		DECboxON;
		if (horzTop && vertLeft && (int)spX1 == startX && (int)spY1 == startY){
			// top-left edge
			Locate(startX, startY);
			printf("l");
		}
		if (horzTop && vertRight && (int)epX1 == stopX && (int)epY1 == startY){
			// top-right edge
			Locate(stopX, startY);
			printf("k");
		}
		if (horzBot && vertRight && (int)spX3 == stopX && (int)spY3 == stopY){
			// bottom-right edge
			Locate(stopX, stopY);
			printf("j");
		}
		if (horzBot && vertLeft && (int)spX4 == startX && (int)spY4 == stopY){
			// bottom left edge
			Locate(startX, stopY);
			printf("m");
		}
		DECboxOFF;
	}
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
	ActTxtColor.fg.Color = 39; ActTxtColor.mode = 1; // ActTxtStyle.pColor = &ActTxtColor;
	printf("\x1B[39m");
}
void ResBg(void) {
	ActTxtColor.bg.Color = 49; ActTxtColor.mode = 1; // ActTxtStyle.pColor = &ActTxtColor;
	printf("\x1B[49m");
}
void ResUl(void) {
	ActTxtColor.ul.Color = 59; ActTxtColor.mode = 1; // ActTxtStyle.pColor = &ActTxtColor;
	printf("\x1B[59m");
}
void ResFB(void) {
	ResFg();
	ResBg();
}
void ResFBU(void) {
	ResFg();
	ResBg();
	ResUl();
}

// Set 24 bit Color
void SetFgRGB(int r, int g, int b) {
	// ActTxtStyle.pColor->fg.R = r; ActTxtStyle.pColor->fg.G = g; ActTxtStyle.pColor->fg.B = b; ActTxtStyle.pColor->mode = 3;
	ActTxtColor.fg.R = r; ActTxtColor.fg.G = g; ActTxtColor.fg.B = b;  ActTxtColor.mode = 3;
	printf("\x1B[38;2;%d;%d;%dm", r, g, b);
}
void SetBgRGB(int r, int g, int b) {
	// ActTxtStyle.pColor->bg.R = r; ActTxtStyle.pColor->bg.G = g; ActTxtStyle.pColor->bg.B = b; ActTxtStyle.pColor->mode = 3;
	ActTxtColor.bg.R = r; ActTxtColor.bg.G = g; ActTxtColor.bg.B = b;  ActTxtColor.mode = 3;
	printf("\x1B[48;2;%d;%d;%dm", r, g, b);
}
void SetUlRGB(int r, int g, int b) {
	// ActTxtStyle.pColor->ul.R = r; ActTxtStyle.pColor->ul.G = g; ActTxtStyle.pColor->ul.B = b; ActTxtStyle.pColor->mode = 3;
	ActTxtColor.ul.R = r; ActTxtColor.ul.G = g; ActTxtColor.ul.B = b; ActTxtColor.mode = 3;
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
	// ActTxtStyle.pColor->fg.Color = c; ActTxtStyle.pColor->mode = 2;
	ActTxtColor.fg.Color = c; ActTxtColor.mode = 2;
	printf("\x1B[38;5;%dm", c);
}
void SetBg255(int c) {
	// ActTxtStyle.pColor->bg.Color = c; ActTxtStyle.pColor->mode = 2;
	ActTxtColor.bg.Color = c; ActTxtColor.mode = 2;
	printf("\x1B[48;5;%dm", c);
}
void SetUl255(int c) {
	// ActTxtStyle.pColor->ul.Color = c; ActTxtStyle.pColor->mode = 2;
	ActTxtColor.ul.Color = c; ActTxtColor.mode = 2;
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
		// ActTxtStyle.pColor->fg.Color = c; ActTxtStyle.pColor->mode = 0;
		ActTxtColor.fg.Color = c; ActTxtColor.mode = 0;
		printf("\x1B[%dm", c);
	}	
}
void SetBg16(int c) {	
	// BackGround
	if (!((c > 39 && c < 48) || (c > 99 && c < 108))){
		ResBg();
	}
	else{
		// ActTxtStyle.pColor->bg.Color = c; ActTxtStyle.pColor->mode = 0;
		ActTxtColor.bg.Color = c; ActTxtColor.mode = 0;
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
	ActTxtStyle.dbl_width = 0;
	ActTxtStyle.dbl_height = 0;
	printf("\x1B#5");
}
void TxtDblTop(int set) {
	// DoubleHeight + DoubleWidth Top-Part
	ActTxtStyle.dbl_height = set;
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

		/*
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
		*/

		if (pTxtStyle->font != ActTxtStyle.font) {
			if (pTxtStyle->font > 10) {
				pTxtStyle->font = 10;
			}
			TxtFont(pTxtStyle->font);
		}

		// SetColorStyle(pTxtStyle->pColor, 1);
		
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
