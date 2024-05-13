
//		   ANSI-ESC - Terminal Functions - See detailed description at the EOF !
//				! Using 3rd party MIT/GPL/"FreeSoftware" technologies !
//				  (see related declarations/functions and at the EOF)
// This is a  ! p_o_o_r !  Header by (c) Pit Demmer. (PIT-Licensed 01.04.2022 -           )

#ifndef _AnsiESC_H
#define _ANSIESC_H

#define AnsiESCVersion "1.00pa"

// WIN & Linux
#include <stdio.h>
#include <stdlib.h>
#include "Terminal.h"

#define EscColorSTRUCT TermColorSTRUCT
#define EscStyleSTRUCT TermStyleSTRUCT

FILE *LOG_ColorInit = NULL;
FILE *LOG_StyleInit = NULL;

// Collector for all strings of EscColor and EscStyle

// Standard, but optional, ESC Sequences
char RUL[] = "24";				// 'ResetUnderLine' alternativ: "4:0" 

// Actual Settings / States
static EscStyleSTRUCT ESC_ActTxtStyle;
static EscColorSTRUCT ESC_ActTxtColor;


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

void ESCinit(void);
int ESCinitColors(char *strFile, EscColorSTRUCT **userColor);
int ESCinitTxtStyles(char *strFile, EscStyleSTRUCT **userTxtStyles);
void ESClocate(FILE *file, int x, int y);
void ESClocateX(FILE *file, int x);
void ESCcursorUp(FILE *file, int y);
void ESCcursorUp1st(FILE *file, int y);
void ESCcursorDown(FILE *file, int y);
void ESCursorDown1st(FILE *file, int y);
void ESCcursorRight(FILE *file, int x);
void ESCcursorLeft(FILE *file, int x);
void ESCcursorMoveX(FILE *file, int x);
void ESCcursorMoveY(FILE *file, int y);
#define ESCcursorMoveXY(file, x, y) ESCcursorMoveX(file, x); ESCcursorMoveY(file, y)
void ESCgetCursorPos(FILE *file);
void ESCcursorSave(FILE *file);
void ESCcursorRestore(FILE *file);
void DECboxMode(FILE *file, int set);
#define DECboxON(file) DECboxMode(file, 1)
#define DECboxOFF(file) DECboxMode(file, 0)
void DEClineX(FILE *file, int len);
void DEClineY(FILE *file, int len);
void CHRline(FILE *file, int spX, int spY, int epX, int epY, int newLine, char c);
#define CHRlineXY(file, startX, startY, stopX, stopY, c) CHRline(file, startX, startY, stopX, stopY, 2, c)
#define CHRmoveTo(file, x, y, c) CHRline(file, x, y, 0, 0, 1, c)
#define CHRlineTo(file, x, y) CHRline(file, 0, 0, x, y, 0, 0)
#define CHRclose(file) CHRline(file, 0, 0, 0, 0, 3, 0)
void LINline(FILE *file, int spX, int spY, int epX, int epY, int newLine);
#define LINlineXY(file, startX, startY, stopX, stopY) LINline(file, startX, startY, stopX, stopY, 2)
#define LINmoveTo(file, x, y) LINline(file, x, y, 0, 0, 1)
#define LINlineTo(file, x, y) LINline(file, 0, 0, x, y, 0)
#define LINclose(file) LINline(file, 0, 0, 0, 0, 3)
void LINline2(FILE *file, int spX, int spY, int epX, int epY, int newLine);
#define LINlineXY2(file, startX, startY, stopX, stopY) LINline2(file, startX, startY, stopX, stopY, 2)
#define LINmoveTo2(file, x, y) LINline2(file, x, y, 0, 0, 1)
#define LINlineTo2(file, x, y) LINline2(file,0, 0, x, y, 0)
#define LINclose2(file) LINline2(file, 0, 0, 0, 0, 3)
void DECline(FILE *file, int spX, int spY, int epX, int epY, int newLine);
#define DEClineXY(file, startX, startY, stopX, stopY) DECline(file, startX, startY, stopX, stopY, 2)
#define DECmoveTo(file, x, y) DECline(file, x, y, 0, 0, 1)
#define DEClineTo(file, x, y) DECline(file, 0, 0, x, y, 0)
#define DECclose(file) DECline(file, 0, 0, 0, 0, 3)
void ESCclrLineA(FILE *file, int xS, int xE);
void ESCclrLine(FILE *file);
void ESCclrLineL(FILE *file);
void ESCclrLineR(FILE *file);
void ESCclrScrA(FILE *file, int xS, int yS, int xE, int yE);
void ESCclrScrL(FILE *file);
void ESCclrScrR(FILE *file);
void ESCresFg(FILE *file);
void ESCresBg(FILE *file);
void ESCresUl(FILE *file);
void ESCresFB(FILE *file);
void ESCresFBU(FILE *file);
void ESCsetFgRGB(FILE *file, int r, int g, int b);
void ESCsetBgRGB(FILE *file, int r, int g, int b);
void ESCsetUlRGB(FILE *file, int r, int g, int b);
void ESCsetFBrgb(FILE *file, int fgR, int fgG, int fgB, int bgR, int bgG, int bgB);
void ESCsetFBUrgb(FILE *file, int fgR, int fgG, int fgB, int bgR, int bgG, int bgB, int ulR, int ulG, int ulB);
void ESCsetFg255(FILE *file, int c);
void ESCsetBg255(FILE *file, int c);
void ESCsetUl255(FILE *file, int c);
void ESCsetFB255(FILE *file, int fg, int bg);
void ESCsetFBU255(FILE *file, int fg, int bg, int ul);
void ESCsetFg16(FILE *file, int c);
void ESCsetBg16(FILE *file, int c);
void ESCsetFB16(FILE *file, int fg, int bg);
void ESCtxtBold(FILE *file, int set);
void ESCtxtFaint(FILE *file, int set);
void ESCtxtItalic(FILE *file, int set);
void ESCtxtUnder(FILE *file, int set);
void ESCtxtDblUnder(FILE *file, int set);
void ESCtxtCurlUnder(FILE *file, int set);
void ESCtxtDotUnder(FILE *file, int set);
void ESCtxtDashUnder(FILE *file, int set);
void ESCtxtDashDotUnder(FILE *file, int set);
void ESCtxtDblCurlUnder(FILE *file, int set);
void ESCtxtDblDotUnder(FILE *file, int set);
void ESCtxtDblDashUnder(FILE *file, int set);
void ESCtxtResetBlink(FILE *file);
void ESCtxtBlink(FILE *file, int set);
void ESCtxtFastBlink(FILE *file, int set);
void ESCtxtReverse(FILE *file, int set);
void ESCtxtInvisible(FILE *file, int set);
void ESCtxtStrike(FILE *file, int set);
void ESCtxtResetScript(FILE *file);
void ESCtxtSuperscript(FILE *file, int set);
void ESCtxtSubscript(FILE *file, int set);
void ESCtxtProportional(FILE *file, int set);
void ESCtxtFramed(FILE *file, int set);
void ESCtxtEncircled(FILE *file, int set);
void ESCtxtOverline(FILE *file, int set);
void ESCtxtResetIdeo(FILE *file);
void ESCtxtIdeoRight(FILE *file, int set);
void ESCtxtIdeoDblRight(FILE *file, int set);
void ESCtxtIdeoLeft(FILE *file, int set);
void ESCtxtIdeoDblLeft(FILE *file, int set);
void ESCtxtIdeoStress(FILE *file, int set);
void ESCtxtResetDblTBW(FILE *file);
void ESCtxtDblTop(FILE *file, int set);
void ESCtxtDblBot(FILE *file, int set);
void ESCtxtDblWidth(FILE *file, int set);
void ESCtxtFont(FILE *file, int fnt);
void ESCsetColorStyle(FILE *file, EscColorSTRUCT *pColor, int set);
void ESCsetTxtStyle(FILE *file, EscStyleSTRUCT *pTxtStyle, int set);


void ESCinit(void) {

	// The 24 Greys (+ Black/White) from 256 Palette	
	ESC_Grey[1] = 15; ESC_Grey[26] = 0;
	int p = 1;
	for (int t = 255; t > 231; t--) {
		p++;
		ESC_Grey[p] = t;
	}

	// Default ColorStyle
	// ESC_ActTxtStyle.pColor = &ESC_ActTxtColor;
}

int ESCinitColors(char *strFile, EscColorSTRUCT **userColor){

	// Users Colors - Helper
	char strSearch[STR_SMALL_SIZE];
	char strGroupName[STR_SMALL_SIZE];
	char strColorName[STR_SMALL_SIZE];
	char strHLP[STR_SMALL_SIZE];

	// count of color groups
	int colorsGroupsCount = INIgetInt(strFile, "colors.GroupCount", 0);
	// groups color model
	int colorsModel = INIgetInt(strFile, "colors.ColorModel", 0);
	
	// memory to store count of colors of each group
	int *colorsCount;
	colorsCount = (int*)malloc(colorsGroupsCount * sizeof(int));
	
	int colorsCountSum = 0;

	// read counts of colors of the groups and calc sum
	for (int i = 0; i < colorsGroupsCount; i++){
		sprintf(strSearch, "colors.group%d.Count", i + 1);
		colorsCount[i] = INIgetInt(strFile, strSearch, 0);
		colorsCountSum += colorsCount[i];
	}
	
	// memory to store all colors - index 0 reserved for default/last color
	*userColor = (EscColorSTRUCT*)malloc((colorsCountSum + 1) * sizeof(EscColorSTRUCT));

	colorsCountSum = 1;

	TermClearScreen(LOG_ColorInit,0);

	for (int i = 0; i < colorsGroupsCount; i++){
		sprintf(strSearch, "colors.group%d.Name", i + 1);
		sprintf(strHLP, "Group%d", i + 1);
		INIgetStr(strFile, strSearch, strHLP, strGroupName);
		for (int j = 0; j < colorsCount[i]; j++){

			(*userColor)[colorsCountSum].groupName = INIstrToMem(strGroupName, 0);
			(*userColor)[colorsCountSum].groupID = i + 1;

			sprintf(strSearch, "colors.group%d.%d.Name", i + 1, j + 1);
			sprintf(strHLP, "Color%d", j + 1);
			INIgetStr(strFile, strSearch, strHLP, strColorName);
			(*userColor)[colorsCountSum].colorName = INIstrToMem(strColorName, 0);
			(*userColor)[colorsCountSum].colorID = j + 1;

			sprintf(strSearch, "colors.group%d.%d.ForeGround", i + 1, j + 1);
			(*userColor)[colorsCountSum].fg.Color = INIgetInt(strFile, strSearch, 15);
			sprintf(strSearch, "colors.group%d.%d.BackGround", i + 1, j + 1);
			(*userColor)[colorsCountSum].bg.Color = INIgetInt(strFile, strSearch, 0);
			sprintf(strSearch, "colors.group%d.%d.UnderLine", i + 1, j + 1);
			(*userColor)[colorsCountSum].ul.Color = INIgetInt(strFile, strSearch, 15);
			
			(*userColor)[colorsCountSum].mode = colorsModel;
			
			// printf("%04d. %s_%s:\n",colorsCountSum + 1, strGroupName, strColorName);
			fprintf(LOG_ColorInit, "%04d. %s_%s: ", colorsCountSum , (*userColor)[colorsCountSum].groupName, (*userColor)[colorsCountSum].colorName);

			ESClocateX(LOG_ColorInit, 36);
			ESCsetColorStyle(LOG_ColorInit, &(*userColor)[colorsCountSum], 1);
			//printf("Res - Done\n");
			//return;
			fprintf(LOG_ColorInit, "->   (%03d:%03d)   <-", (*userColor)[colorsCountSum].fg.Color, (*userColor)[colorsCountSum].bg.Color);
			fflush(LOG_ColorInit);
			ESCresFBU(LOG_ColorInit);
			fprintf(LOG_ColorInit, "\n");
			fflush(LOG_ColorInit);

			colorsCountSum++;

		}
		fprintf(LOG_ColorInit, "\n");
		fflush(LOG_ColorInit);		
	}

	/*
	for (size_t i = 0; i < colorsCountSum; i++)
	{
		SetColorStyle(&(*userColor)[i], 1);
		printf("%04d. %s.%s\n", i, (*userColor)[i].groupName, (*userColor)[i].colorName);
	}
	*/

	free(colorsCount);
	return colorsCountSum - 1;
}

int ESCinitTxtStyles(char *strFile, EscStyleSTRUCT **userTxtStyles){

	// Users Colors
	char strSearch[STR_SMALL_SIZE];
	char strGroupName[STR_SMALL_SIZE];
	char strStyleName[STR_SMALL_SIZE];
	char strHLP[STR_SMALL_SIZE];

	//char strFile[] = "desktops.ini";
	int stylesGroupsCount = INIgetInt(strFile, "txtStyles.GroupCount", 0);
	
	int *stylesCount;
	stylesCount = (int*)malloc(stylesGroupsCount * sizeof(int));
	
	int stylesCountSum = 0;

	for (int i = 0; i < stylesGroupsCount; i++){
		sprintf(strSearch, "txtStyles.group%d.Count", i + 1);
		stylesCount[i] = INIgetInt(strFile, strSearch, 0);
		stylesCountSum += stylesCount[i];
	}
	
	*userTxtStyles = (EscStyleSTRUCT*)malloc((stylesCountSum +1) * sizeof(EscStyleSTRUCT));
	
	stylesCountSum = 1;

	TermClearScreen(LOG_StyleInit,0);

	for (int i = 0; i < stylesGroupsCount; i++){
		sprintf(strSearch, "txtStyles.group%d.Name", i + 1);
		sprintf(strHLP, "Font%d", i + 1);
		INIgetStr(strFile, strSearch, strHLP, strGroupName);
		for (int j = 0; j < stylesCount[i]; j++){

			(*userTxtStyles)[stylesCountSum].fontName = INIstrToMem(strGroupName, 0);
			(*userTxtStyles)[stylesCountSum].fontID = i + 1;

			sprintf(strSearch, "txtStyles.group%d.%d.Name", i + 1, j + 1);
			sprintf(strHLP, "Style%d", j + 1);
			INIgetStr(strFile, strSearch, strHLP, strStyleName);
			(*userTxtStyles)[stylesCountSum].styleName = INIstrToMem(strStyleName, 0);
			(*userTxtStyles)[stylesCountSum].styleID = j + 1;

			sprintf(strSearch, "txtStyles.group%d.%d.bold", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].bold = INIgetBool(strFile, strSearch, 0);

			sprintf(strSearch, "txtStyles.group%d.%d.faint", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].faint = INIgetBool(strFile, strSearch, 0);

			sprintf(strSearch, "txtStyles.group%d.%d.italic", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].italic = INIgetBool(strFile, strSearch, 0);

			sprintf(strSearch, "txtStyles.group%d.%d.blink", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].blink = INIgetBool(strFile, strSearch, 0);

			sprintf(strSearch, "txtStyles.group%d.%d.fastblink", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].fast = INIgetBool(strFile, strSearch, 0);

			sprintf(strSearch, "txtStyles.group%d.%d.reverse", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].reverse = INIgetBool(strFile, strSearch, 0);

			sprintf(strSearch, "txtStyles.group%d.%d.invisible", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].invisible = INIgetBool(strFile, strSearch, 0);

			sprintf(strSearch, "txtStyles.group%d.%d.strikethrough", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].strike = INIgetBool(strFile, strSearch, 0);

			sprintf(strSearch, "txtStyles.group%d.%d.superscript", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].superscript = INIgetBool(strFile, strSearch, 0);

			sprintf(strSearch, "txtStyles.group%d.%d.subscript", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].subscript = INIgetBool(strFile, strSearch, 0);

			sprintf(strSearch, "txtStyles.group%d.%d.proportional", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].proportional = INIgetBool(strFile, strSearch, 0);

			sprintf(strSearch, "txtStyles.group%d.%d.framed", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].framed = INIgetBool(strFile, strSearch, 0);

			sprintf(strSearch, "txtStyles.group%d.%d.encircled", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].encircled = INIgetBool(strFile, strSearch, 0);

			sprintf(strSearch, "txtStyles.group%d.%d.overline", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].overline = INIgetBool(strFile, strSearch, 0);

			/*
			sprintf(strSearch, "txtStyles.group%d.%d.ideoright", i + 1, j + 1);
			userTxtStyles[stylesCountSum].ideo_right = INIgetBool(strFile, strSearch, 0);

			sprintf(strSearch, "txtStyles.group%d.%d.ideoleft", i + 1, j + 1);
			userTxtStyles[stylesCountSum].ideo_left = INIgetBool(strFile, strSearch, 0);

			sprintf(strSearch, "txtStyles.group%d.%d.ideodblright", i + 1, j + 1);
			userTxtStyles[stylesCountSum].ideo_dbl_right = INIgetBool(strFile, strSearch, 0);

			sprintf(strSearch, "txtStyles.group%d.%d.ideodblleft", i + 1, j + 1);
			userTxtStyles[stylesCountSum].ideo_dbl_left = INIgetBool(strFile, strSearch, 0);

			sprintf(strSearch, "txtStyles.group%d.%d.ideostress", i + 1, j + 1);
			userTxtStyles[stylesCountSum].ideo_stress = INIgetBool(strFile, strSearch, 0);
			*/

			sprintf(strSearch, "txtStyles.group%d.%d.dblwidth", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].dbl_width = INIgetBool(strFile, strSearch, 0);

			sprintf(strSearch, "txtStyles.group%d.%d.dblheight", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].dbl_height = INIgetBool(strFile, strSearch, 0);

			sprintf(strSearch, "txtStyles.group%d.%d.font", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].font = INIgetInt(strFile, strSearch, 0);

			sprintf(strSearch, "txtStyles.group%d.%d.underline", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].underline = INIgetInt(strFile, strSearch, 0);

		    // *userTxtStyles)[stylesCountSum].pColor = &ESC_ActTxtColor;

			char strTest[] = "->   ( TEST )   <-";
			fprintf(LOG_StyleInit, "%04d. %s_%s: ", stylesCountSum, (*userTxtStyles)[stylesCountSum].fontName, (*userTxtStyles)[stylesCountSum].styleName);

			ESClocateX(LOG_StyleInit, 45);
			ESCsetTxtStyle(LOG_StyleInit, &(*userTxtStyles)[stylesCountSum], 1);
			if ((*userTxtStyles)[stylesCountSum].dbl_height){
				fprintf(LOG_StyleInit, "\n");
				ESCtxtDblTop(LOG_StyleInit, 1);
				fprintf(LOG_StyleInit, "%s\n", strTest);
				ESCtxtDblBot(LOG_StyleInit, 1);
				fprintf(LOG_StyleInit, strTest);
			}
			else if ((*userTxtStyles)[stylesCountSum].dbl_width){
				fprintf(LOG_StyleInit, "\n");
				ESCtxtDblWidth(LOG_StyleInit, 1);
				fprintf(LOG_StyleInit, strTest);				
			}
			else{
				fprintf(LOG_StyleInit, strTest);
			}					
			fflush(LOG_StyleInit);
			ESCsetTxtStyle(LOG_StyleInit, &(*userTxtStyles)[stylesCountSum], 0);
			fprintf(LOG_StyleInit, "\n");
			fflush(LOG_StyleInit);

			stylesCountSum++;

		}
		fprintf(LOG_StyleInit, "\n");
		fflush(LOG_StyleInit);		
	}

	free(stylesCount);
	
	return stylesCountSum - 1;
}

static void ESCfixCursorPosX(){
	if(TERM_CursorPosX > TERM_ScreenWidth){
		TERM_CursorPosX = TERM_ScreenWidth;
	}
	else if(TERM_CursorPosX < 1){
		TERM_CursorPosX = 1;
	}
}
static void ESCfixCursorPosY(){
	if(TERM_CursorPosY > TERM_ScreenHeight){
		TERM_CursorPosY = TERM_ScreenHeight;
	}
	else if(TERM_CursorPosY < 1){
		TERM_CursorPosY = 1;
	}
}
// Cursor Positions
void ESClocate(FILE *file, int x, int y) {
	// printf("%c[%d;%df", 0x01b, y, x);		//('f' instead 'H') is working, at least on WIN, too. (more... see wikipedia 'ANSI-ESC') 
	fprintf(file, "\x1B[%d;%dH", y, x);
	if (file == stdout){
		TERM_CursorPosY = y;
		TERM_CursorPosX = x;
		ESCfixCursorPosX();
		ESCfixCursorPosY();
	}
}
void ESClocateX(FILE *file, int x) {
	fprintf(file, "\x1B[%dG", x);
	if (file == stdout){
		TERM_CursorPosX = x;
		ESCfixCursorPosX();
	}	
}
void ESCcursorUp(FILE *file, int y) {
	fprintf(file, "\x1B[%dA", y);
	if (file == stdout){
		TERM_CursorPosY -= y;
		ESCfixCursorPosY();
	}	
}
void ESCcursorUp1st(FILE *file, int y) {
	fprintf(file, "\x1B[%dF", y);
	if (file == stdout){
		TERM_CursorPosY -= y;
		ESCfixCursorPosY();
		TERM_CursorPosX = 1;
	}	
}
void ESCcursorDown(FILE *file, int y) {
	fprintf(file, "\x1B[%dB", y);
	if (file == stdout){
		TERM_CursorPosY += y;
		ESCfixCursorPosY();
	}
}
void ESCcursorMoveY(FILE *file, int y){
	if (y < 0){
		// Down
		ESCcursorDown(file, y * -1);
	}
	else if (y > 0){
		// Up
		ESCcursorUp(file, y);
	}
}
void ESCursorDown1st(FILE *file, int y) {
	fprintf(file, "\x1B[%dE", y);
	if (file == stdout){
		TERM_CursorPosY += y;
		ESCfixCursorPosY();
		TERM_CursorPosX = 1;
	}
}
void ESCcursorRight(FILE *file, int x) {
	fprintf(file, "\x1B[%dC", x);
	if (file == stdout){
		TERM_CursorPosX += x;
		ESCfixCursorPosX();
	}	
}
void ESCcursorLeft(FILE *file, int x) {
	fprintf(file, "\x1B[%dD", x);
	if (file == stdout){
		TERM_CursorPosX -= x;
		ESCfixCursorPosX();
	}
}
void ESCcursorMoveX(FILE *file, int x){
	if (x < 0){
		// Left
		ESCcursorLeft(file, x * -1);
	}
	else if (x > 0){
		// Right
		ESCcursorRight(file, x);
	}
}
void ESCgetCursorPos(FILE *file){
	TERM_CursorWaitFor = 1;
	fprintf(file, "\x1B[6n");
}
void ESCcursorSave(FILE *file){
  fprintf(file, "\0337");
  if (file == stdout){
	TERM_CursorPosXsave = TERM_CursorPosX;
	TERM_CursorPosYsave = TERM_CursorPosY;
  }  
}
void ESCcursorRestore(FILE *file){
  fprintf(file, "\0338");
	if (file == stdout){
		TERM_CursorPosX = TERM_CursorPosXsave;
		TERM_CursorPosY = TERM_CursorPosYsave;
	}	
}

// DEC BoxDraw
/*
DEC char to BoxDraw:
		printf("a b c d e f g h i j k l m n o p q r s t u v w x y z\n");
		printf("\x1B(0"); // enable BoxDraw
		printf("a b c d e f g h i j k l m n o p q r s t u v w x y z\n");
		printf("\x1B(B"); // disable BoxDraw
		                    1     1     1       1 1 1
                          9 0 6 5 5     2     7 1 3 4 3		
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

void DECboxMode(FILE *file, int set){
	if (set){
		fprintf(file, "\x1B(0"); // enable BoxDraw Mode
	}
	else{
		fprintf(file, "\x1B(B"); // disable BoxDraw Mode
	}
	fflush(file);
}

void DEClineX(FILE *file, int len){

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

	DECboxON(file);

	if (len < 0){
		// Right to left
		for (int i = 0; i > len; i--){
			fprintf(file, "q");
			if (!isLastLine){
				ESCcursorLeft(file, 2);
			}
			else{
				ESCcursorLeft(file, 1);
				isLastLine = 0;
			}			
		}
	}
	else{
		// Left to right
		STRprintChars(file, 'q', len);
	}
	
	DECboxOFF(file);

}
void DEClineY(FILE *file, int len){
	
	DECboxON(file);

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
			fprintf(file, "x");
			if (!isLastLine){
				ESCcursorLeft(file, 1);
			}
			ESCcursorUp(file, 1);
		}
	}
	else{
		// top to bottom
		while (len--){
			fprintf(file, "x");
			if (!isLastLine){
				ESCcursorLeft(file, 1);
			}
			ESCcursorDown(file, 1);
		}
	}
	
	DECboxOFF(file);
		
}

void CHRline(FILE *file, int spX, int spY, int epX, int epY, int newLine, char c) {
    
	static int lastX;
	static int lastY;
	static int polyStartX;
	static int polyStartY;

	static char myChar;
	
	if (c){
		// just set the char
		myChar = c;
	}
	
	if (newLine == 1){
		// just a moveTo for the 1st line
		lastX = spX;
		lastY = spY;
		polyStartX = spX;
		polyStartY = spY;
		return;
	}
	else if (newLine == 3){
		// close poly-ine
		spX = lastX;
		spY = lastY;
		epX = polyStartX;
		epY = polyStartY;
	}
	else if (newLine == 2){
		// full (new) line
		polyStartX = spX;
		polyStartY = spY;
	}
	else{
		// lineTo
		spX = lastX;
		spY = lastY;
	}

	double spXd = spX;
	double spYd = spY;
	double epXd = epX;
	double epYd = epY;
	
	int r = LineInRect(&spXd, &spYd, &epXd, &epYd, 1, 1, TERM_ScreenWidth, TERM_ScreenHeight);

	if (r){
		// We have a line to draw

		// back to int
		spX = (spXd + 0.5);
		spY = (spYd + 0.5);
		epX = (epXd + 0.5);
		epY = (epYd + 0.5);
	}
	else{
		lastX = (epXd + 0.5);
		lastY = (epYd + 0.5);
		return;
	}
	
	int dx = abs(epX - spX);
    int dy = abs(epY - spY);
    int sx = (spX < epX) ? 1 : -1;
    int sy = (spY < epY) ? 1 : -1;

    int diff = dx - dy;

	char nextHorz = 0;

    while (1) {
		ESClocate(file, spX, spY);

		fprintf(file, "%c", myChar);
		
        if (spX == epX && spY == epY) {
			lastX = spX;
			lastY = spY;
			fflush(file);
			break;
        }

        int diff2 = 2 * diff;

        if (diff2 > -dy) {
            diff -= dy;
            spX += sx;
        }

        if (diff2 < dx) {
            diff += dx;
            spY += sy;
        }
    }
}

void LINline(FILE *file, int spX, int spY, int epX, int epY, int newLine) {
    
	static int lastX;
	static int lastY;
	static int polyStartX;
	static int polyStartY;
	
	if (newLine == 1){
		// just a moveTo for the 1st line
		lastX = spX;
		lastY = spY;
		polyStartX = spX;
		polyStartY = spY;
		return;
	}
	else if (newLine == 3){
		// close poly-ine
		spX = lastX;
		spY = lastY;
		epX = polyStartX;
		epY = polyStartY;
	}
	else if (newLine == 2){
		// full (new) line
		polyStartX = spX;
		polyStartY = spY;
	}
	else{
		// lineTo
		spX = lastX;
		spY = lastY;
	}

	double spXd = spX;
	double spYd = spY;
	double epXd = epX;
	double epYd = epY;
	
	int r = LineInRect(&spXd, &spYd, &epXd, &epYd, 1, 1, TERM_ScreenWidth, TERM_ScreenHeight);

	if (r){
		// We have a line to draw

		// back to int
		spX = (spXd + 0.5);
		spY = (spYd + 0.5);
		epX = (epXd + 0.5);
		epY = (epYd + 0.5);
	}
	else{
		lastX = (epXd + 0.5);
		lastY = (epYd + 0.5);
		return;
	}
	
	
	int dx = abs(epX - spX);
    int dy = abs(epY - spY);
    int sx = (spX < epX) ? 1 : -1;
    int sy = (spY < epY) ? 1 : -1;

    int diff = dx - dy;

	char nextHorz = 0;

    while (1) {
		ESClocate(file, spX, spY);

		if (nextHorz){
			// we're on a Y-jump of a more horizontal line....
			fprintf(file, "%c", nextHorz);
			nextHorz = 0;
			DECboxOFF(file);
		}
		else if (dx > dy){
			// more horizontal
			fprintf(file, "-");
		}
		else if (dx < dy){
			// more vertical
	        fprintf(file, "|");
		}
		else{
			// 45°
			if (sx == sy ){
		        fprintf(file, "\\");
			}
			else{
		        fprintf(file, "/");
			}			
		}
		
        if (spX == epX && spY == epY) {
			lastX = spX;
			lastY = spY;
			fflush(file);
			break;
        }

		lastX = spX;
		lastY = spY;

        int diff2 = 2 * diff;

        if (diff2 > -dy) {
            diff -= dy;
            spX += sx;
			if (dx < dy){
				// more vertical - is a jump
				ESClocate(file, lastX, lastY);
				if (sx == sy){
					fprintf(file, "\\");
				}
				else{
					fprintf(file, "/");
				}
			}
        }

        if (diff2 < dx) {
            diff += dx;
            spY += sy;
			if (dx > dy){
				// more horizontal - is a jump
				ESClocate(file, lastX, lastY);
				DECboxON(file);
				if (sy == 1){
					fprintf(file, "s");
					nextHorz = 'o';
				}
				else{
					fprintf(file, "o");
					nextHorz = 's';
				}				
			}
        }
    }
}

void LINline2(FILE *file, int spX, int spY, int epX, int epY, int newLine) {
    
	static int lastX;
	static int lastY;
	static int lastChar;
	static int lastDir;

	static int polyStartX;
	static int polyStartY;
	static int polyStartChar;
	static int polyStartDir;

	static int firstLine = 0;	// Becomes true after 1st line of a poly-line is drawn
								// to signalize, that start-point intersection need to get calculated
	int doInterStart = 0;
	int doInterStop = 0;

	int actChar = 0;
	int actDir = 0;

	if (newLine == 1){
		// just a moveTo for the 1st line
		lastX = spX;
		lastY = spY;
		polyStartX = spX;
		polyStartY = spY;
		polyStartChar = 0;
		polyStartDir = 0;
		lastChar = 0;
		lastDir = 0;
		firstLine = 0;
		return;
	}
	else if (newLine == 3){
		// close poly-ine
		spX = lastX;
		spY = lastY;
		epX = polyStartX;
		epY = polyStartY;
		doInterStop = 1;
	}
	else if (newLine == 2){
		// full (new) line
		polyStartX = spX;
		polyStartY = spY;
		polyStartChar = 0;
		polyStartDir = 0;
		lastChar = 0;
		lastDir = 0;
		firstLine = 0;
	}
	else{
		// lineTo
		spX = lastX;
		spY = lastY;
	}

	double spXd = spX;
	double spYd = spY;
	double epXd = epX;
	double epYd = epY;
	
	int r = LineInRect(&spXd, &spYd, &epXd, &epYd, 1, 1, TERM_ScreenWidth, TERM_ScreenHeight);

	#define SBS 139		// '/' + '\'

	if (r){
		// We have a line to draw

		// back to int
		spX = (spXd + 0.5);
		spY = (spYd + 0.5);
		epX = (epXd + 0.5);
		epY = (epYd + 0.5);
	}
	else{
		lastX = (epXd + 0.5);
		lastY = (epYd + 0.5);
		return;
	}
	
	int dx = abs(epX - spX);
    int dy = abs(epY - spY);
    int sx = (spX < epX) ? 1 : -1;
    int sy = (spY < epY) ? 1 : -1;

    int diff = dx - dy;

	char nextHorz = 0;

	
	actDir = sy;
	
	
	doInterStart = firstLine;

    while (1) {
		ESClocate(file, spX, spY);

		actChar = 0;

		if (nextHorz){
			// we're on a Y-jump of a more horizontal line....
			fprintf(file, "%c", nextHorz);
			nextHorz = 0;
			DECboxOFF(file);
		}
		else if (dx > dy){
			// more horizontal
			fprintf(file, "-");
		}
		else if (dx < dy){
			// more vertical
	        fprintf(file, "|");
		}
		else{
			// 45°
			if (sx == sy ){
		        fprintf(file, "\\");
				actChar = '\\';
			}
			else{
		        fprintf(file, "/");
				actChar = '/';
			}			
		}
		
        if (spX == epX && spY == epY){
			if (doInterStop){
				ESClocate(file, spX, spY);
				actChar += polyStartChar;
				if (polyStartDir != actDir && actChar == SBS){
					fprintf(file, "-");
				}
				else if(actChar == SBS){
					fprintf(file, "|");
				}
			}

			if (!firstLine){
				polyStartChar = actChar;
				polyStartDir = actDir;
			}			
			lastX = spX;
			lastY = spY;
			lastChar = actChar;
			lastDir = actDir;
			fflush(file);
			firstLine = 1;
			break;
        }

		lastX = spX;
		lastY = spY;

        int diff2 = 2 * diff;

        if (diff2 > -dy) {
            diff -= dy;
            spX += sx;
			if (dx < dy){
				// more vertical - is a jump
				ESClocate(file, lastX, lastY);
				if (sx == sy){
					fprintf(file, "\\");
					actChar = '\\';
				}
				else{
					fprintf(file, "/");
					actChar = '/';
				}
			}
        }

        if (diff2 < dx) {
            diff += dx;
            spY += sy;
			if (dx > dy){
				// more horizontal - is a jump
				ESClocate(file, lastX, lastY);
				DECboxON(file);
				if (sy == 1){
					fprintf(file, "s");
					nextHorz = 'o';
				}
				else{
					fprintf(file, "o");
					nextHorz = 's';
				}				
			}
        }

		if (doInterStart){
			ESClocate(file, lastX, lastY);
			lastChar += actChar;

			if (lastDir != actDir && lastChar == SBS){
				fprintf(file, "-");
			}
			else if(lastChar == SBS){
				fprintf(file, "|");
			}

			doInterStart = 0;
		}

		lastChar = actChar;
		lastDir = actDir;
    }
}

void DECline(FILE *file, int spX, int spY, int epX, int epY, int newLine) {
    
	static int lastX;
	static int lastY;
	static int polyStartX;
	static int polyStartY;
	static int polyStartDir;
	static int polyFromDir;
	static int polyToDir;
	
	static int firstLine = 0;	// Becomes true after 1st line of a poly-line is drawn
								// to signalize, that start-point intersection need to get calculated
	int doInterStart = 0;
	int doInterStop = 0;
	
	static int fromDirLast;
	static int toDirLast;
	int fromDir = 0;
	int toDir = 0;
	
	if (newLine == 1){
		// just a moveTo for the 1st line
		lastX = spX;
		lastY = spY;
		polyStartX = spX;
		polyStartY = spY;
		firstLine = 0;
		return;
	}
	else if (newLine == 3){
		// close poly-ine
		spX = lastX;
		spY = lastY;
		epX = polyStartX;
		epY = polyStartY;
		doInterStop = 1;
	}
	else if (newLine == 2){
		// full (new) line
		polyStartX = spX;
		polyStartY = spY;
		firstLine = 0;
	}
	else{
		// lineTo
		spX = lastX;
		spY = lastY;
	}

	double spXd = spX;
	double spYd = spY;
	double epXd = epX;
	double epYd = epY;
	
	int r = LineInRect(&spXd, &spYd, &epXd, &epYd, 1, 1, TERM_ScreenWidth, TERM_ScreenHeight);

	if (r){
		// We have a line to draw

		// back to int
		spX = (spXd + 0.5);
		spY = (spYd + 0.5);
		epX = (epXd + 0.5);
		epY = (epYd + 0.5);
	}
	else{
		lastX = epX;
		lastY = epY;
		firstLine = 0;
		return;
	}
	
	
	int dx = abs(epX - spX);
    int dy = abs(epY - spY);
    int sx = (spX < epX) ? 1 : -1;
    int sy = (spY < epY) ? 1 : -1;

    int diff = dx - dy;

	char nextHorz = 0;

	doInterStart = firstLine;

	DECboxON(file);
    while (1) {
		ESClocate(file, spX, spY);

		if (dx > dy){
			// more horizontal
			if (sx == 1){
				// Left -> Right
				fromDir = 8;		// Left (West)
				toDir = 4;
			}
			else{
				// Left <- Right
				fromDir = 4;		// Right (East)
				toDir = 8;
			}
			fprintf(file, "q");
		}
		else if (dx < dy){
			// more vertical
			if (sy == 1){
				// Top -> Down
				fromDir = 1;		// Top (North)
				toDir = 2;
			}
			else{
				// Down -> Top
				fromDir = 2;		// Down (South)
				toDir = 1;
			}
			fprintf(file, "x");
		}
		else{
			// 45°
			char char2nd = 0;
			if (sx == sy){
				// Backslash - TopLeft - BottomRight
		        if (sy == 1){
					// Top -> Bottom └┐
					fprintf(file, "m");
					char2nd= 'k';			// └┐
					fromDir = 1;		// Top (North)
					toDir = 4;
				}
				else{
					// Bottom -> Top └┐
					fprintf(file, "k");			// ┐
					ESClocate(file, spX - 1, spY);
					char2nd = 'm';			// └
					fromDir = 2;		// Down (South)
					toDir = 8;
				}
			}
			else{
				// Slash - TopRight - BottomLeft
		        if (sy == 1){
					// Top -> Bottom ┌┘
					fprintf(file, "j");			// ┘
					ESClocate(file, spX - 1, spY);
					char2nd = 'l';			// ┌
					fromDir = 1;		// Top (North)
					toDir = 8;
				}
				else{
					// Bottom -> Top ┌┘
					fprintf(file, "l");			// ┌┘
					fromDir = 2;		// Down (South)
					toDir = 4;
					char2nd = 'j';
				}
			}			
			if (!(spX == epX && spY == epY)){
				fprintf(file, "%c", char2nd);
			}
		}

		lastX = spX;
		lastY = spY;
        if (spX == epX && spY == epY) {
			if (doInterStop){
				ESClocate(file, spX, spY);
				if (((fromDir + toDir) == 3 && (polyFromDir + polyToDir) == 12) || ((fromDir + toDir) == 12 && (polyFromDir + polyToDir) == 3)){
					// Vert meets Horz
					fprintf(file, "%c", TuiDecBoxDraw[polyToDir + fromDir]);
				}
				else{
					// no 90°
					fprintf(file, "%c", TuiDecBoxDraw[(fromDir + toDir) | (polyFromDir + polyToDir)]);
				}
			}
			if (!firstLine){
				polyFromDir = fromDir;
				polyToDir = toDir;
			}
			DECboxOFF(file);
			fflush(file);
			firstLine = 1;
			break;
        }

        int diff2 = 2 * diff;

        if (diff2 > -dy) {
            diff -= dy;
            spX += sx;
			if (dx < dy){
				// more vertical - is a jump
				ESClocate(file, lastX, lastY);
				if (sx == sy){
					// Backslash - TopLeft - BottomRight
					if (sy == 1){
						// Top -> Bottom └┐
						fprintf(file, "mk");			// └┐
						fromDir = 1;
						toDir = 4;
					}
					else{
						// Bottom -> Top └┐
						fprintf(file, "k");			// ┐
						ESClocate(file, lastX - 1, lastY);
						fprintf(file, "m");			// └
						fromDir = 2;
						toDir = 8;
					}
				}
				else{
					// Slash - TopRight - BottomLeft
					if (sy == 1){
						// Top -> Bottom ┌┘
						fprintf(file, "j");			// ┘
						ESClocate(file, lastX - 1, lastY);
						fprintf(file, "l");
						fromDir = 1;
						toDir = 8;			// ┌
					}
					else{
						// Bottom -> Top ┌┘
						fprintf(file, "lj");			// ┌┘
						fromDir = 2;
						toDir = 4;
					}
				}
			}
        }

        if (diff2 < dx) {
            diff += dx;
            spY += sy;
			if (dx > dy){
				// more horizontal - is a jump
				ESClocate(file, lastX, lastY);
				if (sx == sy){
					// Backslash - TopLeft - BottomRight
					if (sx == 1){
						// Left -> Right ┐
						//               └ Top -> Down
						fprintf(file, "k");
						ESClocate(file, lastX, lastY + 1);
						fprintf(file, "m");
						fromDir = 8;
						toDir= 2;
					}
					else{
						// Left <- Right ┐
						//               └ Up <- Down
						fprintf(file, "m");
						ESClocate(file, lastX, lastY - 1);
						fprintf(file, "k");
						fromDir = 4;
						toDir = 1;
					}
				}
				else{
					// Slash - TopRight - BottomLeft
					if (sx == 1){
						//             ┌ Left -> Right
						// Down -> Top ┘
						fprintf(file, "j");
						ESClocate(file, lastX, lastY - 1);
						fprintf(file, "l");
						fromDir = 8;
						toDir = 1;
					}
					else{
						//             ┌ Left <- Right
						// Down <- Top ┘
						fprintf(file, "l");
						ESClocate(file, lastX, lastY + 1);
						fprintf(file, "j");
						fromDir = 4;
						toDir = 2;
					}
				}
			}
        }
		if (doInterStart){
			ESClocate(file, lastX, lastY);
			if (((fromDir + toDir) == 3 && (fromDirLast + toDirLast) == 12) || ((fromDir + toDir) == 12 && (fromDirLast + toDirLast) == 3)){
				// Vert meets Horz
				fprintf(file, "%c", TuiDecBoxDraw[fromDirLast + toDir]);
			}
			else{
				// no 90°
				fprintf(file, "%c", TuiDecBoxDraw[(fromDir + toDir) | (fromDirLast + toDirLast)]);
			}
			doInterStart = 0;
		}
		fromDirLast = fromDir;
		toDirLast = toDir;
	}
}

void DECrect(FILE *file, int startX, int startY, int stopX, int stopY){
	
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
		ESClocate(file, startX, startY);
		DECboxON(file);
		fprintf(file, "l");					// top-left edge
		DEClineX(file, stopX - startX - 1);
		DECboxON(file);
		fprintf(file, "k");
		
		ESClocate(file, stopX, startY + 1);
		DEClineY(file, stopY - startY - 1);
		DECboxON(file);
		fprintf(file, "j");					// bottom-right edge
				
		ESClocate(file, stopX - 1, stopY);
		DEClineX(file, startX - stopX + 1);
		DECboxON(file);
		fprintf(file, "m");					// bottom-left edge
				
		ESClocate(file, startX, stopY - 1);
		DEClineY(file, startY - stopY + 1);
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
			ESClocate(file, spX1, spY1);
			DEClineX(file, epX1 - spX1 + 2);
		}
		if (vertRight){
			ESClocate(file, spX2, spY2);
			DEClineY(file, epY2 - spY2 + 2);
		}
		if (horzBot){
			ESClocate(file, spX3, spY3);
			DEClineX(file, epX3 - spX3);
		}
		if (vertLeft){
			ESClocate(file, spX4, spY4);
			DEClineY(file, epY4 - spY4 - 1);
		}

		// draw valid edges
		DECboxON(file);
		if (horzTop && vertLeft && (int)spX1 == startX && (int)spY1 == startY){
			// top-left edge
			ESClocate(file, startX, startY);
			fprintf(file, "l");
		}
		if (horzTop && vertRight && (int)epX1 == stopX && (int)epY1 == startY){
			// top-right edge
			ESClocate(file, stopX, startY);
			fprintf(file, "k");
		}
		if (horzBot && vertRight && (int)spX3 == stopX && (int)spY3 == stopY){
			// bottom-right edge
			ESClocate(file, stopX, stopY);
			fprintf(file, "j");
		}
		if (horzBot && vertLeft && (int)spX4 == startX && (int)spY4 == stopY){
			// bottom left edge
			ESClocate(file, startX, stopY);
			fprintf(file, "m");
		}
		DECboxOFF(file);
	}
}

// Clear Lines
void ESCclrLineA(FILE *file, int xS, int xE){
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

	ESCresFBU(file);
	ESClocateX(file, xS);
	fprintf(file, "%*s", x, "");
}
void ESCclrLine(FILE *file) {
	// ClearLine
	fprintf(file, "\x1B[2K");
}
void ESCclrLineL(FILE *file) {
	// ClearLine from cursor to Left
	fprintf(file, "\x1B[1K");
}
void ESCclrLineR(FILE *file) {
	// ClearLine from cursor to Right
	fprintf(file, "\x1B[K");
	// printf("%s0K", CSI);	// The '0' isn't needed.
}

// Clear Screens
void ESCclrScrA(FILE *file, int xS, int yS, int xE, int yE){
	// Area CLS from Start(xS/Ys) to End(xE/yE)
	
	if (yS == yE){
		// just one Line
		ESClocate(file, xS, yS);
		ESCclrLineA(file, xS, xE);
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
		ESCresFBU(file);

		for (i = 0; i < y; i++){
			ESClocate(file, xS, yS + i);
			fprintf(file, "%*s", x, "");
		}
	}
}
void ESCclrScrL(FILE *file) {
	// Cls from cursor to Upper Left
	fprintf(file, "\x1B[1J");
}
void ESCclrScrR(FILE *file) {
	// Cls from cursor to LowerRight
	fprintf(file, "\x1B[J");
	// printf("%s0J", CSI);	// The '0' isn't needed.
}

// Reset To Default Colors
void ESCresFg(FILE *file) {
	ESC_ActTxtColor.fg.Color = 39; ESC_ActTxtColor.mode = 1; // ESC_ActTxtStyle.pColor = &ESC_ActTxtColor;
	fprintf(file, "\x1B[39m");
}
void ESCresBg(FILE *file) {
	ESC_ActTxtColor.bg.Color = 49; ESC_ActTxtColor.mode = 1; // ESC_ActTxtStyle.pColor = &ESC_ActTxtColor;
	fprintf(file, "\x1B[49m");
}
void ESCresUl(FILE *file) {
	ESC_ActTxtColor.ul.Color = 59; ESC_ActTxtColor.mode = 1; // ESC_ActTxtStyle.pColor = &ESC_ActTxtColor;
	fprintf(file, "\x1B[59m");
}
void ESCresFB(FILE *file) {
	ESCresFg(file);
	ESCresBg(file);
}
void ESCresFBU(FILE *file) {
	ESCresFB(file);
	ESCresUl(file);
}

// Set 24 bit Color
void ESCsetFgRGB(FILE *file, int r, int g, int b) {
	// ESC_ActTxtStyle.pColor->fg.R = r; ESC_ActTxtStyle.pColor->fg.G = g; ESC_ActTxtStyle.pColor->fg.B = b; ESC_ActTxtStyle.pColor->mode = 3;
	ESC_ActTxtColor.fg.R = r; ESC_ActTxtColor.fg.G = g; ESC_ActTxtColor.fg.B = b;  ESC_ActTxtColor.mode = 3;
	fprintf(file, "\x1B[38;2;%d;%d;%dm", r, g, b);
}
void ESCsetBgRGB(FILE *file, int r, int g, int b) {
	// ESC_ActTxtStyle.pColor->bg.R = r; ESC_ActTxtStyle.pColor->bg.G = g; ESC_ActTxtStyle.pColor->bg.B = b; ESC_ActTxtStyle.pColor->mode = 3;
	ESC_ActTxtColor.bg.R = r; ESC_ActTxtColor.bg.G = g; ESC_ActTxtColor.bg.B = b;  ESC_ActTxtColor.mode = 3;
	fprintf(file, "\x1B[48;2;%d;%d;%dm", r, g, b);
}
void ESCsetUlRGB(FILE *file, int r, int g, int b) {
	// ESC_ActTxtStyle.pColor->ul.R = r; ESC_ActTxtStyle.pColor->ul.G = g; ESC_ActTxtStyle.pColor->ul.B = b; ESC_ActTxtStyle.pColor->mode = 3;
	ESC_ActTxtColor.ul.R = r; ESC_ActTxtColor.ul.G = g; ESC_ActTxtColor.ul.B = b; ESC_ActTxtColor.mode = 3;
	fprintf(file, "\x1B[58;2;%d;%d;%dm", r, g, b);
}
void ESCsetFBrgb(FILE *file, int fgR, int fgG, int fgB, int bgR, int bgG, int bgB) {
	ESCsetFgRGB(file, fgR, fgG, fgB);
	ESCsetBgRGB(file, bgR, bgG, bgB);
}
void ESCsetFBUrgb(FILE *file, int fgR, int fgG, int fgB, int bgR, int bgG, int bgB, int ulR, int ulG, int ulB) {
	ESCsetFgRGB(file, fgR, fgG, fgB);
	ESCsetBgRGB(file, bgR, bgG, bgB);
	ESCsetUlRGB(file, ulR, ulG, ulB);
}

// Set 256 Colors
void ESCsetFg255(FILE *file, int c) {
	if (c < 1){
		// don't touch
	}
	else if (c > 255){
		// Reset
		ESCresFg(file);
	}
	else{
		fprintf(file, "\x1B[38;5;%dm", c);
	}	
	ESC_ActTxtColor.fg.Color = c; ESC_ActTxtColor.mode = 2;
}
void ESCsetBg255(FILE *file, int c) {
	if (c < 1){
		// don't touch
	}
	else if (c > 255){
		// Reset
		ESCresBg(file);
	}
	else{
		fprintf(file, "\x1B[48;5;%dm", c);
	}	
	ESC_ActTxtColor.bg.Color = c; ESC_ActTxtColor.mode = 2;
}
void ESCsetUl255(FILE *file, int c) {
	if (c < 1){
		// don't touch
	}
	else if (c > 255){
		// Reset
		ESCresUl(file);
	}
	else{
		fprintf(file, "\x1B[58;5;%dm", c);
	}	
	ESC_ActTxtColor.ul.Color = c; ESC_ActTxtColor.mode = 2;
}
void ESCsetFB255(FILE *file, int fg, int bg) {
	ESCsetFg255(file, fg);
	ESCsetBg255(file, bg);
}
void ESCsetFBU255(FILE *file, int fg, int bg, int ul) {
	ESCsetFg255(file, fg);
	ESCsetBg255(file, bg);
	ESCsetUl255(file, ul);
}

//Set 16 Colors
void ESCsetFg16(FILE *file, int c){
	// ForeGround	
	if (!((c > 29 && c < 38) || (c > 89 && c < 98))){
		ESCresFg(file);
	}
	else{
		fprintf(file, "\x1B[%dm", c);
	}	
	ESC_ActTxtColor.fg.Color = c; ESC_ActTxtColor.mode = 0;
}
void ESCsetBg16(FILE *file, int c) {	
	// BackGround
	if (!((c > 39 && c < 48) || (c > 99 && c < 108))){
		ESCresBg(file);
	}
	else{
		fprintf(file, "\x1B[%dm", c);
	}
	ESC_ActTxtColor.bg.Color = c; ESC_ActTxtColor.mode = 0;
}
void ESCsetFB16(FILE *file, int fg, int bg) {
	ESCsetFg16(file, fg);
	ESCsetBg16(file, bg);
}

// TXT Styles - single commands
void ESCtxtBold(FILE *file, int set) {
	ESC_ActTxtStyle.bold = set;
	ESC_ActTxtStyle.faint = 0;
	if (set) {
		// Set
		fprintf(file, "\x1B[1m");
	}
	else {
		// Reset
		fprintf(file, "\x1B[22m");
	}
}
void ESCtxtFaint(FILE *file, int set) {
	ESC_ActTxtStyle.faint = set;
	ESC_ActTxtStyle.bold = 0;
	if (set) {
		// Set
		fprintf(file, "\x1B[2m");
	}
	else {
		// Reset
		fprintf(file, "\x1B[22m");
	}
}
void ESCtxtItalic(FILE *file, int set) {
	ESC_ActTxtStyle.italic = set;
	if (set) {
		// Set
		fprintf(file, "\x1B[3m");
	}
	else {
		// Reset
		fprintf(file, "\x1B[23m");
	}
}
void ESCtxtUnder(FILE *file, int set) {
	ESC_ActTxtStyle.underline = set;
	if (set) {
		// Set
		fprintf(file, "\x1B[4m");
	}
	else {
		// Reset
		fprintf(file, "\x1B[%sm", RUL);
	}
}
void ESCtxtDblUnder(FILE *file, int set) {
	if (set) {
		// Set
		ESC_ActTxtStyle.underline = 2;
		fprintf(file, "\x1B[21m");
	}
	else {
		// Reset
		ESCtxtUnder(file, 0);
	}
}
void ESCtxtCurlUnder(FILE *file, int set) {
	if (set) {
		// Set
		ESC_ActTxtStyle.underline = 3;
		fprintf(file, "\x1B[4:3m");
	}
	else {
		// Reset
		ESCtxtUnder(file, 0);
	}
}
void ESCtxtDotUnder(FILE *file, int set) {
	if (set) {
		// Set
		ESC_ActTxtStyle.underline = 4;
		fprintf(file, "\x1B[4:4m");
	}
	else {
		// Reset
		ESCtxtUnder(file, 0);
	}
}
void ESCtxtDashUnder(FILE *file, int set) {
	if (set) {
		// Set
		ESC_ActTxtStyle.underline = 5;
		fprintf(file, "\x1B[4:5m");
	}
	else {
		// Reset
		ESCtxtUnder(file, 0);
	}
}
void ESCtxtDashDotUnder(FILE *file, int set) {
	// Undocumented DashDot - just a prognosis
	if (set) {
		// Set
		ESC_ActTxtStyle.underline = 6;
		fprintf(file, "\x1B[4:6m");
	}
	else {
		// Reset
		ESCtxtUnder(file, 0);
	}
}
void ESCtxtDblCurlUnder(FILE *file, int set) {
	// Undocumented DoubleCurl - just a prognosis
	if (set) {
		// Set
		ESC_ActTxtStyle.underline = 7;
		fprintf(file, "\x1B[4:7m");
	}
	else {
		// Reset
		ESCtxtUnder(file, 0);
	}
}
void ESCtxtDblDotUnder(FILE *file, int set) {
	// Undocumented DoubleDotted - just a prognosis
	if (set) {
		// Set
		ESC_ActTxtStyle.underline = 8;
		fprintf(file, "\x1B[4:8m");
	}
	else {
		// Reset
		ESCtxtUnder(file, 0);
	}
}
void ESCtxtDblDashUnder(FILE *file, int set) {
	// Undocumented DoubleDashed - just a prognosis
	if (set) {
		// Set
		ESC_ActTxtStyle.underline = 9;
		fprintf(file, "\x1B[4:9m");
	}
	else {
		// Reset
		ESCtxtUnder(file, 0);
	}
}

void ESCtxtResetBlink(FILE *file){
	fprintf(file, "\x1B[25m");
}
void ESCtxtBlink(FILE *file, int set) {
	ESC_ActTxtStyle.blink = set;
	ESC_ActTxtStyle.fast = 0;
	if (set) {
		// Set
		fprintf(file, "\x1B[5m");
	}
	else {
		// Reset
		ESCtxtResetBlink(file);
	}
}
void ESCtxtFastBlink(FILE *file, int set) {
	ESC_ActTxtStyle.fast = set;
	ESC_ActTxtStyle.blink = 0;
	if (set) {
		// Set
		fprintf(file, "\x1B[6m");
	}
	else {
		// Reset
		ESCtxtResetBlink(file);
	}
}
void ESCtxtReverse(FILE *file, int set) {
	ESC_ActTxtStyle.reverse = set;
	if (set) {
		// Set
		fprintf(file, "\x1B[7m");
	}
	else {
		// Reset
		fprintf(file, "\x1B[27m");
	}
}
void ESCtxtInvisible(FILE *file, int set) {
	ESC_ActTxtStyle.invisible = set;
	if (set) {
		// Set
		fprintf(file, "\x1B[8m");
	}
	else {
		// Reset
		fprintf(file, "\x1B[28m");
	}
}
void ESCtxtStrike(FILE *file, int set) {
	ESC_ActTxtStyle.strike = set;
	if (set) {
		// Set
		fprintf(file, "\x1B[9m");
	}
	else {
		// Reset
		fprintf(file, "\x1B[29m");
	}
}
void ESCtxtResetScript(FILE *file){
	fprintf(file, "\x1B[75m");
}
void ESCtxtSuperscript(FILE *file, int set) {
	ESC_ActTxtStyle.superscript = set;
	ESC_ActTxtStyle.subscript = 0;
	if (set) {
		// Set
		fprintf(file, "\x1B[73m");
	}
	else {
		// Reset
		ESCtxtResetScript(file);
	}
}
void ESCtxtSubscript(FILE *file, int set) {
	ESC_ActTxtStyle.subscript = set;
	ESC_ActTxtStyle.superscript = 0;
	if (set) {
		// Set
		fprintf(file, "\x1B[74m");
	}
	else {
		// Reset
		ESCtxtResetScript(file);
	}
}
void ESCtxtProportional(FILE *file, int set){
	ESC_ActTxtStyle.proportional = set;
	if (set) {
		// Set
		fprintf(file, "\x1B[26m");
	}
	else {
		// Reset
		fprintf(file, "\x1B[50m");
	}
}
void ESCtxtFramed(FILE *file, int set) {
	ESC_ActTxtStyle.framed = set;
	ESC_ActTxtStyle.encircled = 0;
	if (set) {
		// Set
		fprintf(file, "\x1B[51m");
	}
	else {
		// Reset
		fprintf(file, "\x1B[54m");
	}
}
void ESCtxtEncircled(FILE *file, int set) {
	ESC_ActTxtStyle.encircled = set;
	ESC_ActTxtStyle.framed = 0;
	if (set) {
		// Set
		fprintf(file, "\x1B[52m");
	}
	else {
		// Reset
		fprintf(file, "\x1B[54m");
	}
}
void ESCtxtOverline(FILE *file, int set) {
	ESC_ActTxtStyle.overline = set;
	if (set) {
		// Set
		fprintf(file, "\x1B[53m");
	}
	else {
		// Reset
		fprintf(file, "\x1B[55m");
	}
}

void ESCtxtResetIdeo(FILE *file) {
	ESC_ActTxtStyle.ideo_right = 0; ESC_ActTxtStyle.ideo_dbl_right = 0;
	ESC_ActTxtStyle.ideo_left = 0; ESC_ActTxtStyle.ideo_dbl_left = 0;
	ESC_ActTxtStyle.ideo_stress = 0;
	fprintf(file, "\x1B[65m");
}
void ESCtxtIdeoRight(FILE *file, int set) {
	if (set){
		ESC_ActTxtStyle.ideo_right = 1;
		fprintf(file, "\x1B[60m");
	}
	else{
		ESCtxtResetIdeo(file);
	}
}
void ESCtxtIdeoDblRight(FILE *file, int set) {
	if (set){
		ESC_ActTxtStyle.ideo_dbl_right = 1;
		fprintf(file, "\x1B[61m");
	}
	else{
		ESCtxtResetIdeo(file);
	}
}
void ESCtxtIdeoLeft(FILE *file, int set) {
	if (set){
		ESC_ActTxtStyle.ideo_left = 1;
		fprintf(file, "\x1B[62m");
	}
	else{
		ESCtxtResetIdeo(file);
	}
}
void ESCtxtIdeoDblLeft(FILE *file, int set) {
	if (set){
		ESC_ActTxtStyle.ideo_dbl_left = 1;
		fprintf(file, "\x1B[63m");
	}
	else{
		ESCtxtResetIdeo(file);
	}
}
void ESCtxtIdeoStress(FILE *file, int set) {
	if (set){
		ESC_ActTxtStyle.ideo_stress = 1;
		fprintf(file, "\x1B[64m");
	}
	else{
		ESCtxtResetIdeo(file);
	}
}

void ESCtxtResetDblTBW(FILE *file){
	ESC_ActTxtStyle.dbl_width = 0;
	ESC_ActTxtStyle.dbl_height = 0;
	fprintf(file, "\x1B#5");
}
void ESCtxtDblTop(FILE *file, int set) {
	// DoubleHeight + DoubleWidth Top-Part
	ESC_ActTxtStyle.dbl_height = set;
	if (set) {
		// Set
		fprintf(file, "\x1B#3");
	}
	else {
		// Reset
		ESCtxtResetDblTBW(file);
	}
}
void ESCtxtDblBot(FILE *file, int set) {
	// DoubleHeight + DoubleWidth Bottom-Part
	ESC_ActTxtStyle.dbl_height = set;
	if (set) {
		// Set
		fprintf(file, "\x1B#4");
	}
	else {
		// Reset
		ESCtxtResetDblTBW(file);
	}
}
void ESCtxtDblWidth(FILE *file, int set) {
	// DoubleWidth 
	ESC_ActTxtStyle.dbl_width = set;
	if (set) {
		// Set
		fprintf(file, "\x1B#6");
	}
	else {
		// Reset
		ESCtxtResetDblTBW(file);
	}
}

void ESCtxtFont(FILE *file, int fnt) {

	// 10 = Default Font
	// 11 - 19 User Fonts @INVESTIGATE@
	// 20 = Fraktur

	if (fnt > 10){
		fnt = 10;
	}
	if (fnt < 0){
		fnt = 0;
	}
	
	ESC_ActTxtStyle.font = fnt;
	fprintf(file, "\x1B[%dm", fnt + 10);
}

void ESCsetColorStyle(FILE *file, EscColorSTRUCT *pColor, int set){

	if (set){
		switch (pColor->mode) {
		case 1:		// 16 (has no UnderLineColor)
			if (pColor->fg.Color != ESC_ActTxtColor.fg.Color) {
				if (!((pColor->fg.Color > 29 && pColor->fg.Color < 38) || (pColor->fg.Color > 89 && pColor->fg.Color < 98))){
					pColor->fg.Color = 39;
					ESCresFg(file);
				}
				else{
					ESCsetFg16(file, pColor->fg.Color);
				}
			}
			if (pColor->bg.Color != ESC_ActTxtColor.bg.Color) {
				if (!((pColor->bg.Color > 39 && pColor->bg.Color < 48) || (pColor->bg.Color > 99 && pColor->bg.Color < 108))){
					pColor->bg.Color = 49;
					ESCresBg(file);
				}
				else{
					ESCsetBg16(file, pColor->bg.Color);
				}
			}
			break;
		
		case 2:		// 255
			if (pColor->fg.Color != ESC_ActTxtColor.fg.Color) {
				ESCsetFg255(file, pColor->fg.Color);
			}
			if (pColor->bg.Color != ESC_ActTxtColor.bg.Color) {
				ESCsetBg255(file, pColor->bg.Color);
			}
			if (pColor->ul.Color != ESC_ActTxtColor.ul.Color) {
				ESCsetUl255(file, pColor->ul.Color);
			}
			break;
		
		case 3:		// RGB
			if (pColor->fg.Color != ESC_ActTxtColor.fg.Color) {
				ESCsetFgRGB(file, pColor->fg.R, pColor->fg.G, pColor->fg.B);
			}
			if (pColor->bg.Color != ESC_ActTxtColor.bg.Color) {
				ESCsetBgRGB(file, pColor->bg.R, pColor->bg.G, pColor->bg.B);
			}
			if (pColor->ul.Color != ESC_ActTxtColor.ul.Color) {
				ESCsetUlRGB(file, pColor->ul.R, pColor->ul.G, pColor->ul.B);
			}
			break;
		}
	}
	else{
		//memset(&ESC_ActTxtColor, 0, sizeof(ESC_ActTxtColor));
		ESCresFBU(file);
	}
}

// TXT Style - combined from Structure
void ESCsetTxtStyle(FILE *file, EscStyleSTRUCT *pTxtStyle, int set) {

	// set == False -> Reset all to default

	if (set) {
		if (pTxtStyle->bold != ESC_ActTxtStyle.bold) {
			ESCtxtBold(file, pTxtStyle->bold);
		}
		if (pTxtStyle->faint != ESC_ActTxtStyle.faint) {
			ESCtxtFaint(file, pTxtStyle->faint);
		}
		if (pTxtStyle->italic != ESC_ActTxtStyle.italic) {
			ESCtxtItalic(file, pTxtStyle->italic);
		}
		if (pTxtStyle->underline != ESC_ActTxtStyle.underline) {
			ESC_ActTxtStyle.underline = pTxtStyle->underline;
			switch (pTxtStyle->underline) {
			case 0: // None
			case 1: // Single
				ESCtxtUnder(file, pTxtStyle->underline);
				break;
			case 2: // Double
				ESCtxtDblUnder(file, 1);
				break;
			case 3: // Curly
				ESCtxtCurlUnder(file, 1);
				break;
			case 4: // Dot
				ESCtxtUnder(file, 1);
				break;
			case 5: // Dash
				ESCtxtDashUnder(file, 1);
				break;
			case 6: // DashDot
				ESCtxtDashDotUnder(file, 1);
				break;
			case 7: // Double Curly
				ESCtxtDblCurlUnder(file, 1);
				break;
			case 8: // Double Dot
				ESCtxtDblDotUnder(file, 1);
				break;
			case 9: // Double Dash
				ESCtxtDblDashUnder(file, 1);
				break;
			default:
				// This would be an error !!
				pTxtStyle->underline = 0;
				ESCtxtUnder(file, 0);
				break;
			}
		}

		if (pTxtStyle->blink != ESC_ActTxtStyle.blink) {
			if (!pTxtStyle->fast){
				ESCtxtBlink(file, pTxtStyle->blink);
			}			
		}
		if (pTxtStyle->fast != ESC_ActTxtStyle.fast) {
			ESCtxtFastBlink(file, pTxtStyle->fast);
		}

		if (pTxtStyle->reverse != ESC_ActTxtStyle.reverse) {
			ESCtxtReverse(file, pTxtStyle->reverse);
		}
		
		if (pTxtStyle->invisible != ESC_ActTxtStyle.invisible) {
			ESCtxtInvisible(file, pTxtStyle->invisible);
		}

		if (pTxtStyle->strike != ESC_ActTxtStyle.strike) {
			ESCtxtStrike(file, pTxtStyle->strike);
		}

		if (pTxtStyle->superscript != ESC_ActTxtStyle.superscript) {
			if (!pTxtStyle->subscript){
				ESCtxtSuperscript(file, pTxtStyle->superscript);
			}			
		}
		if (pTxtStyle->subscript != ESC_ActTxtStyle.subscript) {
			ESCtxtSubscript(file, pTxtStyle->subscript);
		}

		if (pTxtStyle->proportional != ESC_ActTxtStyle.proportional) {
			ESCtxtProportional(file, pTxtStyle->proportional);
		}

		if (pTxtStyle->framed != ESC_ActTxtStyle.framed) {
			if (!pTxtStyle->encircled){
				ESCtxtFramed(file, pTxtStyle->framed);
			}			
		}
		if (pTxtStyle->encircled != ESC_ActTxtStyle.encircled) {
			ESCtxtEncircled(file, pTxtStyle->encircled);
		}

		if (pTxtStyle->overline != ESC_ActTxtStyle.overline) {
			ESCtxtOverline(file, pTxtStyle->overline);
		}

		/*
		if ((pTxtStyle->ideo_right != ESC_ActTxtStyle.ideo_right) ||
			(pTxtStyle->ideo_dbl_right != ESC_ActTxtStyle.ideo_dbl_right) ||
			(pTxtStyle->ideo_left != ESC_ActTxtStyle.ideo_left) ||
			(pTxtStyle->ideo_dbl_left != ESC_ActTxtStyle.ideo_dbl_left) ||
			(pTxtStyle->ideo_stress != ESC_ActTxtStyle.ideo_stress)) {

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

		if (pTxtStyle->font != ESC_ActTxtStyle.font) {
			if (pTxtStyle->font > 10) {
				pTxtStyle->font = 10;
			}
			ESCtxtFont(file, pTxtStyle->font);
		}

		// SetColorStyle(pTxtStyle->pColor, 1);
		
	}
	else {
		// Reset colors, fonts and styles to their defaults
		memset(&ESC_ActTxtStyle, 0, sizeof(ESC_ActTxtStyle));
		//memset(&ESC_ActTxtColor, 0, sizeof(ESC_ActTxtColor));
		//printf("\x1B[10;0;39;49;59m");
		fprintf(file, "\x1B[10;0m");
		ESCresFBU(file);
	}
}

#endif
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
