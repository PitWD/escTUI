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

#include "Terminal.h"
#include "AnsiESC.h"

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

(00)  = n/a					(Top)
(01)  = n/a					(Right)
(02)  = n/a					(Bottom)
(03) └ = Top-Right
(04)  = n/a					(Left)
(05) │ = Top-Bottom
(06) ┌ = Bottom-Right
(07) ├ = Top-Right-Bottom
(08)  = n/a
(09) ┘ = Top-Left
(10) ─ = Right-Left
(11) ┴ = Top-Left-Right
(12) ┐ = Bottom-Left
(13) ┤ = Top-Left-Bottom
(14) ┬ = Bottom-Left-Right
(15) ┼ = Top-Right-Bottom-Left
*/
static char TuiDecBoxDraw[16][2] = {" ", " ", " ", "m", " ", "x",
									"l", "t", " ", "j", "q", "v",
									"k", "u", "w", "n" };



typedef struct{
	char *caption;
	int txtColor;
	int txtStyle;
	int timeColor;
	int timeStyle;
	uint8_t printRunTime :1;
	uint8_t printRealTime :1;
}TuiHeaderSTRUCT;
// global access on headers
TuiHeaderSTRUCT *userHeaders = NULL;

struct TuiMenuDefSTRUCT;
struct TuiMenuPosSTRUCT;

struct TuiMenuDefSTRUCT{
	int txtColor;
	int txtStyle;
	int selectColor;
	int selectStyle;
	int disabledColor;
	int disabledStyle;
	int keyColor;
	int keyStyle;
	int timeColor;
	int timeStyle;
	int posCnt;						// count of sub-positions
	struct TuiMenuPosSTRUCT *pos1st;		
	uint8_t printRunTime :1;
	uint8_t printRealTime :1;
};
// global access on Menu Definitions
struct TuiMenuDefSTRUCT *userTopMenuDefs = NULL;
struct TuiMenuDefSTRUCT *userBotMenuDefs = NULL;
struct TuiMenuDefSTRUCT *userLeftMenuDefs = NULL;
struct TuiMenuDefSTRUCT *userRightMenuDefs = NULL;

struct TuiMenuPosSTRUCT{
	char *caption;
	struct TuiMenuDefSTRUCT *definition;
	int keyCode;
	int enabled;
	int posCnt;						// count of sub-positions
	struct TuiMenuPosSTRUCT *nextPos;		// on same level
	struct TuiMenuPosSTRUCT *prevPos;		// on same level
	struct TuiMenuPosSTRUCT *pos1st;		// first on sub level
	uint8_t selected :1;			// if position is selected
	uint8_t isOption :1;
	uint8_t isCheck :1;
	uint8_t	activated :1;			// if position is a check or option and active
}TuiMenuPosSTRUCT;

typedef struct{
	int header :1;
	int topMenu :1;
	int bottomMenu :1;
	int leftMenu :1;
	int rightMenu :1;
}TuiDesktopDefSTRUCT;

void TUIrenderHeader(int posX, int posY, int width, int headerID, int justRefresh){
	
	// Width to render
	if (!width){
		width = TERM_ScreenWidth;
		posX = 1;
	}
	// Where to render
	if (posX && posY){
		Locate(posX, posY);
	}
	else if (posX){
		LocateX(posX);
	}
	

	if (!justRefresh){
		// Style & Color
		SetColorStyle(&userColors[userHeaders[headerID].txtColor - 1], 1);
		SetTxtStyle(&userStyles[userHeaders[headerID].txtStyle - 1], 1);
		StrPrintCentered(userHeaders[headerID].caption, width);
	}
	else{
		// we just refresh time(s)
		CursorRight(width);
	}

	if (userHeaders[headerID].printRealTime || userHeaders[headerID].printRunTime){
		// Set style & color of times
		SetColorStyle(&userColors[userHeaders[headerID].timeColor - 1], 1);
		SetTxtStyle(&userStyles[userHeaders[headerID].timeStyle - 1], 1);
	}
	
	// Do we print the Realtime (all time right alignment)
	if (userHeaders[headerID].printRealTime){
		// 01.01.2023 00:00:00
		CursorLeft(19);
		printf("%s %s", gStrDate, gStrTime);
		CursorRight(1);
	}
	// Do we print the RunTime
	if (userHeaders[headerID].printRunTime){
		// 00000d 00:00:00
		if (userHeaders[headerID].printRealTime){
			// left alignment, cause RealTime already exist
			CursorLeft(width - 2);
		}
		else{
			// right alignment
			CursorLeft(15);
		}
		printf("%s", gStrRunTime);
	}	
}

int TUIinitHeaders(char *strFile, TuiHeaderSTRUCT **userHeader){

	// User Header - Helper
	char strSearch[STR_SMALL_SIZE];
	char strHeaderCaption[STR_SMALL_SIZE];
	char strHLP[STR_SMALL_SIZE];

	// count of headers
	int headersCount = IniGetInt(strFile, "global.header.HeaderCount", 0);

	// memory to store all header
	*userHeader = (TuiHeaderSTRUCT*)malloc(headersCount * sizeof(TuiHeaderSTRUCT));

	for (size_t i = 0; i < headersCount; i++){
		// Caption
		sprintf(strSearch, "global.header.%d.Caption", i + 1);
		sprintf(strHLP, "Header%d", i + 1);
		IniGetStr(strFile, strSearch, strHLP, strHeaderCaption);
		(*userHeader)[i].caption = IniStrToMem(strHeaderCaption, 0);
		// TextColor 
		sprintf(strSearch, "global.header.%d.TextColor", i + 1);
		(*userHeader)[i].txtColor = IniGetInt(strFile, strSearch, 0);
		// TextStyle 
		sprintf(strSearch, "global.header.%d.TextStyle", i + 1);
		(*userHeader)[i].txtStyle = IniGetInt(strFile, strSearch, 0);
		// TimeColor 
		sprintf(strSearch, "global.header.%d.TimeColor", i + 1);
		(*userHeader)[i].timeColor = IniGetInt(strFile, strSearch, 0);
		// TimeStyle 
		sprintf(strSearch, "global.header.%d.TimeStyle", i + 1);
		(*userHeader)[i].timeStyle = IniGetInt(strFile, strSearch, 0);
		// PrintRunTime 
		sprintf(strSearch, "global.header.%d.PrintRunTime", i + 1);
		(*userHeader)[i].printRunTime = IniGetBool(strFile, strSearch, 0);
		// PrintRealTime 
		sprintf(strSearch, "global.header.%d.PrintRealTime", i + 1);
		(*userHeader)[i].printRealTime = IniGetBool(strFile, strSearch, 0);

		printf("%04d Header:\n", i +1);
		TUIrenderHeader(0,0,0,i,0);
		ResFBU();
		printf("\n");
		fflush(stdout);

	}
	printf("\n");

	return headersCount;
	
}

struct TuiMenuPosSTRUCT *TUIaddMenuPos(char *strFile, char *strPath, struct TuiMenuDefSTRUCT *definition, int positions){

	static struct TuiMenuPosSTRUCT **menuPos = NULL;
	static size_t cnt = 0;

	// Helper
	char strSearch[STR_SMALL_SIZE];
	char strHLP[STR_SMALL_SIZE];
	char strPos1[STR_SMALL_SIZE];

	
	int posReturn = cnt;	// save for return

	for (int i = 0; i < positions; i++){
		
		int j = i + 1;	// User to Memory indexing

		// Add mem for new position
		cnt++;
		int pos1 = cnt - 1;
		menuPos = (struct TuiMenuPosSTRUCT**)realloc(menuPos, cnt * sizeof(struct TuiMenuPosSTRUCT*));
		menuPos[pos1] = (struct TuiMenuPosSTRUCT*)malloc(sizeof(struct TuiMenuPosSTRUCT));

		sprintf(strSearch, "%s%d.Text", strPath, j);
		sprintf(strHLP, "%s%d", strPath, j);
		IniGetStr(strFile, strSearch, strHLP, strPos1);
		(*menuPos)[pos1].caption = IniStrToMem(strPos1, 0);

		sprintf(strSearch, "%s%d.Enabled", strPath, j);
		(*menuPos)[pos1].posCnt = IniGetBool(strFile, strSearch, 1);
		sprintf(strSearch, "%s%d.isCheck", strPath, j);
		(*menuPos)[pos1].isCheck = IniGetBool(strFile, strSearch, 0);
		sprintf(strSearch, "%s%d.isOption", strPath, j);
		(*menuPos)[pos1].isOption = IniGetBool(strFile, strSearch, 0);
		sprintf(strSearch, "%s%d.isActivated", strPath, j);
		(*menuPos)[pos1].activated = IniGetBool(strFile, strSearch, 0);
		sprintf(strSearch, "%s%d.Positions", strPath, j);
		(*menuPos)[pos1].posCnt = IniGetInt(strFile, strSearch, 0);
		
		// already known stuff
		(*menuPos)[pos1].definition = definition;
		(*menuPos)[pos1].nextPos = NULL;
		(*menuPos)[pos1].prevPos = NULL;
		(*menuPos)[pos1].pos1st = NULL;
		
		// if not 1st pos, we have to set...
		if (i){
			// actual pos2 is nextPos of prevPos
			(*menuPos)[pos1 - 1].nextPos = menuPos[pos1];
			// prevPos is prevPos...
			(*menuPos)[pos1].prevPos = menuPos[pos1 - 1];
		}
		// clean temporary vals of this position
		(*menuPos)[pos1].activated = 0;
		(*menuPos)[pos1].selected = 0;
		// extract key

		if ((*menuPos)[pos1].posCnt){
			sprintf(strSearch, "%s%d.", strPath, j);
			(*menuPos)[pos1].pos1st = TUIaddMenuPos(strFile, strSearch, definition, (*menuPos)[pos1].posCnt);
		}
	}
	

	return menuPos[posReturn];
		
}

// "global.TopMenu."
int TUIinitMenuDefs(char *strFile, char *strPath, struct TuiMenuDefSTRUCT **menu){

	// Helper
	char strSearch[STR_SMALL_SIZE];

	sprintf(strSearch, "%s%s", strPath, "count");
	int menusCnt = IniGetInt(strFile, strSearch, 0);

	*menu = (struct TuiMenuDefSTRUCT*)malloc(menusCnt * sizeof(struct TuiMenuDefSTRUCT));

	// Menu Definition Values
	for (size_t i = 0; i < menusCnt; i++){
		sprintf(strSearch, "%s%d.TextColor", strPath, i + 1);
		(*menu)[i].txtColor = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s%d.TextStyle", strPath, i + 1);
		(*menu)[i].txtStyle = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s%d.SelectColor", strPath, i + 1);
		(*menu)[i].selectColor = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s%d.SelectStyle", strPath, i + 1);
		(*menu)[i].selectStyle = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s%d.DisabledColor", strPath, i + 1);
		(*menu)[i].disabledColor = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s%d.DisabledStyle", strPath, i + 1);
		(*menu)[i].disabledStyle = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s%d.KeyColor", strPath, i + 1);
		(*menu)[i].keyColor = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s%d.KeyStyle", strPath, i + 1);
		(*menu)[i].keyStyle = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s%d.TimeColor", strPath, i + 1);
		(*menu)[i].timeColor = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s%d.TimeStyle", strPath, i + 1);
		(*menu)[i].timeStyle = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s%d.PrintRunTime", strPath, i + 1);
		(*menu)[i].printRunTime = IniGetBool(strFile, strSearch, 0);
		sprintf(strSearch, "%s%d.PrintRealTime", strPath, i + 1);
		(*menu)[i].printRealTime = IniGetBool(strFile, strSearch, 0);
		sprintf(strSearch, "%s%d.Positions", strPath, i + 1);
		(*menu)[i].posCnt = IniGetInt(strFile, strSearch, 0);
		
		// Add positions
		(*menu)[i].pos1st = TUIaddMenuPos(strFile, strSearch, menu[i], (*menu)[i].posCnt);		

	}
	
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

Files			            Creation Date   Alpha Date      BetaDate        Creator
    <PoorTui.h>		        01.04.2022		                                Pit

BugID		Description									Found by / Date		Fixed by / Date
*/


