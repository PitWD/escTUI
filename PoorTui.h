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

//#include "Terminal.h"
//#include "esc.h"

#include "AnsiESC.h"

FILE *LOG_TuiCopy = NULL;

#define TUIsetColorStyle(file, color, style) ESCsetColorStyle(file, &userColors[color], 1); ESCsetTxtStyle(file, &userStyles[style], 1)

struct TuiHeadersSTRUCT{
	char *caption;
	int txtColor;
	int txtStyle;
	int timeColor;
	int timeStyle;
	int printRunTime :1;
	int printRealTime :1;
};
// global access on headers 
struct TuiHeadersSTRUCT *userHeaders = NULL;
struct TuiHeadersSTRUCT *userFooters = NULL;
int TUI_HeaderCnt = 0;
int TUI_FooterCnt = 0;

struct TuiMenusSTRUCT{
	int txtColor;
	int txtStyle;
	int selectColor;
	int selectStyle;
	int selectKeyColor;
	int selectKeyStyle;
	int disabledColor;
	int disabledStyle;
	int keyColor;
	int keyStyle;
	int timeColor;
	int timeStyle;
	int posCnt;	
	int pos1ID;						// helper for fixing pos1st while reallocating - shifts
	struct TuiMenuPosSTRUCT *pos1st;		
	int printRunTime :1;
	int printRealTime :1;
};
// global access on Menu Definitions
struct TuiMenusSTRUCT *userTopMenus = NULL;
struct TuiMenusSTRUCT *userBotMenus = NULL;
struct TuiMenusSTRUCT *userLeftMenus = NULL;
struct TuiMenusSTRUCT *userRightMenus = NULL;
struct TuiMenusSTRUCT *userPopUpMenus = NULL;
int TUI_TopMenuCnt = 0;
int TUI_BotMenuCnt = 0;
int TUI_LeftMenuCnt = 0;
int TUI_RightMenuCnt = 0;
int TUI_PopUpMenuCnt = 0;
#define TUI_MENU_TOP 1
#define TUI_MENU_LEFT 2
#define TUI_MENU_RIGHT 3
#define TUI_MENU_BOTTOM 4

struct TuiMenuPosSTRUCT{
	char *caption;
	int keyCode;
	int enabled;
	int posCnt;								// count of sub-positions
	struct TuiMenuPosSTRUCT *nextPos;		// on same level
	struct TuiMenuPosSTRUCT *prevPos;		// on same level
	struct TuiMenuPosSTRUCT *pos1st;		// first on sub level
	struct TuiMenuPosSTRUCT *parentPos;		// parent on upper level
	int nextID;					// helper for reallocating
	int prevID;					//   ""   ""      ""
	int pos1ID;					//   ""   ""      ""
	int parentID;				//   ""   ""      ""
	int selected :1;			// if position is selected
	int isOption :1;			// if position is a check or option...
	int isCheck :1;
	int	activated :1;			
	int isSpacer :1;			// "-" as caption defines a spacer
	int printSmall :1;			// force to print small (preCalcSubMenu)
	int printInverted :1;		// force to print inverted (preCalcSubMenu)
}TuiMenuPosSTRUCT;

struct TuiDesktopsSTRUCT{
	int header;
	int topMenu;
	int bottomMenu;
	int leftMenu;
	int rightMenu;
	int footer;
};
struct TuiDesktopsSTRUCT *userDesktopDefs = NULL;

struct TuiMenuPosSTRUCT *TUIgetSelectedPos(struct TuiMenuPosSTRUCT *menuPos){

	// Get the last selected pos in a menu
	
	struct TuiMenuPosSTRUCT *selectedPos = NULL;

	while (menuPos){
		if (menuPos->selected && menuPos->enabled){
			selectedPos = menuPos;
			if (!menuPos->pos1st){
				return selectedPos;
			}
			else{
				menuPos = menuPos->pos1st;
			}
		}
		else {
			menuPos = menuPos->nextPos;
		}
	}
	return selectedPos;

}

int TUIfindSelectedMenu(struct TuiDesktopsSTRUCT *deskDef, struct TuiMenuPosSTRUCT *menuPos){

	// Find the selected menu on a desktop
	// Return 0 - 4 for none, top, left, right, bottom
	// Return selectedPos in menuPos

	int menuID[4];
	menuID[0] = deskDef->topMenu;
	menuID[1] = deskDef->leftMenu;
	menuID[2] = deskDef->rightMenu;
	menuID[3] = deskDef->bottomMenu;

	struct TuiMenusSTRUCT *menuDef[4];
	menuDef[0] = userTopMenus;
	menuDef[1] = userLeftMenus;
	menuDef[2] = userRightMenus;
	menuDef[3] = userBotMenus;


	for (int i = 0; i < 4; i++){
		if (menuID[i]){
			menuPos = TUIgetSelectedPos(menuDef[menuID[i] - 1]->pos1st);
			if (menuPos){
				return i + 1;
			}
		}		
	}

	menuPos = NULL;
	return 0;

}

void TUIsecureMinMaxXY(int *minX, int *minY, int *maxX, int *maxY){

	// Secure the min/max values for rendering
	if (*minX == 0){
		*minX = 1;
	}
	if (*maxX == 0){
		*maxX = TERM_ScreenWidth;
	}
	if (*minY == 0){
		*minY = 1;
	}
	if (*maxY == 0){
		*maxY = TERM_ScreenHeight;
	}

}

int TUIgetPosLen(struct TuiMenuPosSTRUCT *menuPos){
	if (!menuPos->printSmall){
		return strlen(menuPos->caption);
	}		
	else{
		if (menuPos->isCheck || menuPos->isOption){
			return 7;
		}
		return 3;
	}
}

int TUIgetSubLen(struct TuiMenuPosSTRUCT *menuPos, int all, int fullLen){

	// menuPos is the 1st position of a sub-menu
	// all is a flag to get the length, including all following sub-menus
	// fullLen is the current length of the (sum of) calling menu(s)

	int len = 0;
	int loopLen = 0;
	int smallLen = 3;	// Key + 2 spaces

	struct TuiMenuPosSTRUCT *selectedPos = NULL;

	int isSmall = menuPos->printSmall;

	while (menuPos){
		if (!isSmall){
			len = strlen(menuPos->caption);
			loopLen = (loopLen > len) ? loopLen : len;
		}		
		else if(isSmall && smallLen == 3 && (menuPos->isCheck || menuPos->isOption)){
			smallLen = 7;	// [ ] A
			if (!all){
				break;
			}
		}
		if (all && menuPos->selected && menuPos->enabled){
			selectedPos = menuPos->pos1st;
		}
		menuPos = menuPos->nextPos;
	}
	
	if (isSmall){
		loopLen = smallLen;
	}	
	fullLen += loopLen;
	
	if (selectedPos){
		fullLen = TUIgetSubLen(selectedPos, all, fullLen);
	}

	return fullLen;
}

int TUIsetSubSmall(struct TuiMenuPosSTRUCT *menuPos){

	// menuPos is the 1st position of a sub-menu
	// crawl through all sub-menus and set 1st menuPos->printSmall = 0 to 1

	while (menuPos){

		if (!menuPos->printSmall){
			menuPos->printSmall = -1;
			return 1;
		}
		
		while (menuPos){
			if (menuPos->selected && menuPos->enabled && menuPos->pos1st){
				// next level
				menuPos = menuPos->pos1st;
				break;
			}
			else{
				// next position
				menuPos = menuPos->nextPos;
			}
		}

	}

	return 0;
}

void TUIclearSmallInverted(struct TuiMenuPosSTRUCT *menuPos){
	while (menuPos){
		menuPos->printSmall = 0;
		menuPos->printInverted = 0;
		if (menuPos->selected && menuPos->enabled){
			// SubMenu found
			menuPos = menuPos->pos1st;
			menuPos->printSmall = 0;
			menuPos->printInverted = 0;
		}				
		menuPos = menuPos->nextPos;
	}
}

int TUIprintMenuPos(int posX, int posY, int printSmall, int renderWidth, struct TuiMenuPosSTRUCT *menuPos, struct TuiMenusSTRUCT *menuDef){

	// Where to render
	if (posX && posY){
		ESClocate(LOG_TuiCopy, posX, posY);
	}
	else if (posX){
		ESClocateX(LOG_TuiCopy, posX);
	}
	

	if (!renderWidth){
		renderWidth = TUIgetSubLen(menuPos, 0, 0);
	}
	
	if (printSmall || menuPos->printSmall){
		// render just the keys (+ eventually check/option brackets)
		if (menuPos->selected && menuPos->enabled){
			// enabled & selected
			TUIsetColorStyle(LOG_TuiCopy, menuDef->selectColor, menuDef->selectStyle);
		}
		else if (menuPos->enabled){
			// enabled
			TUIsetColorStyle(LOG_TuiCopy, menuDef->txtColor, menuDef->txtStyle);
		}
		else{
			// disabled
			TUIsetColorStyle(LOG_TuiCopy, menuDef->disabledColor, menuDef->disabledStyle);
		}
		fprintf(LOG_TuiCopy, " ");	// Leading Space
		if (menuPos->isCheck){
			// is a check
			fprintf(LOG_TuiCopy, "[");
			if (menuPos->activated){
				fprintf(LOG_TuiCopy, "x");
			}
			else{
				fprintf(LOG_TuiCopy, " ");
			}
			fprintf(LOG_TuiCopy, "] ");
		}
		else if (menuPos->isOption){
			// is an option
			fprintf(LOG_TuiCopy, "(");
			if (menuPos->activated){
				fprintf(LOG_TuiCopy, "x");
			}
			else{
				fprintf(LOG_TuiCopy, " ");
			}
			fprintf(LOG_TuiCopy, ") ");
		}
		else if (printSmall == 7){
			if (menuPos->isSpacer){
				DEClineX(LOG_TuiCopy, 4);
			}
			else{
				STRprintSpaces(LOG_TuiCopy, 4);
			}
		}
		else{
			// we're fine
		}
		
		if (menuPos->selected && menuPos->enabled){
			TUIsetColorStyle(LOG_TuiCopy, menuDef->selectKeyColor, menuDef->selectKeyStyle);
		}
		else if (menuPos->enabled && !menuPos->isSpacer){
			// Key in keyColor and keyStyle
			TUIsetColorStyle(LOG_TuiCopy, menuDef->keyColor, menuDef->keyStyle);
		}
		else{
			// Color & Style is fine
		}
		if (menuPos->isSpacer){
			DEClineX(LOG_TuiCopy, 1);
		}
		else{
			fprintf(LOG_TuiCopy, "%c", menuPos->caption[menuPos->keyCode]);
		}

		if (menuPos->selected && menuPos->enabled){
			// enabled & selected
			TUIsetColorStyle(LOG_TuiCopy, menuDef->selectColor, menuDef->selectStyle);
		}
		else if (menuPos->enabled){
			// enabled
			TUIsetColorStyle(LOG_TuiCopy, menuDef->txtColor, menuDef->txtStyle);
		}
		else{
			// disabled
			TUIsetColorStyle(LOG_TuiCopy, menuDef->disabledColor, menuDef->disabledStyle);
		}
		fprintf(LOG_TuiCopy, " ");	// Trailing Space
		
	}

	else{
		// render full line
		int len = (int)strlen(menuPos->caption);
		for (int i = 0; i < len; i++){
			if (menuPos->enabled && menuPos->selected){
				// enabled - selected
				if (i && (i == menuPos->keyCode)){
					// is key
					TUIsetColorStyle(LOG_TuiCopy, menuDef->selectKeyColor, menuDef->selectKeyStyle);
				}
				else{
					TUIsetColorStyle(LOG_TuiCopy, menuDef->selectColor, menuDef->selectStyle);
				}
			}
			else if (menuPos->enabled){
				// enabled
				if (i && (i == menuPos->keyCode)){
					// is key
					TUIsetColorStyle(LOG_TuiCopy, menuDef->keyColor, menuDef->keyStyle);
				}
				else{
					TUIsetColorStyle(LOG_TuiCopy, menuDef->txtColor, menuDef->txtStyle);
				}
			}
			else{
				// disabled
				TUIsetColorStyle(LOG_TuiCopy, menuDef->disabledColor, menuDef->disabledStyle);
			}
			if (i == 2 && (menuPos->isCheck || menuPos->isOption)){
				// Set Value of check/option
				if (menuPos->activated){
					fprintf(LOG_TuiCopy, "x");
				}
				else{
					fprintf(LOG_TuiCopy, " ");
				}
			}
			else{
				if (menuPos->isSpacer){
					/* code */
				}
				else{
					fprintf(LOG_TuiCopy, "%c", menuPos->caption[i]);
				}
			}
		}

		if (menuPos->isSpacer){
			fprintf(LOG_TuiCopy, " ");
			DEClineX(LOG_TuiCopy, renderWidth - 2);
			fprintf(LOG_TuiCopy, " ");
		}
		else{
			STRprintSpaces(LOG_TuiCopy, renderWidth - strlen(menuPos->caption));
		}
	}

	return renderWidth;
	
}

int TUIpreRenderSub(int posX, int width, struct TuiMenuPosSTRUCT *menuPos, int minX, int maxX){

	int mainLen = TUIgetSubLen(menuPos, 0, 0);
	int subLen = TUIgetSubLen(menuPos, 1, 0) - mainLen;
	int diff = mainLen - width;
	int invertX = posX - diff;

	// [main-part][sub-part]
	// 0 = to the right
	// 1 = to the left
	int sX[2][2];
	int eX[2][2];
	int spaceX[2][2];
	int main_sub[2][2];

	// Start- and Endpoints
	sX[0][0] = posX;
	sX[0][1] = posX - subLen;
	sX[1][0] = invertX;
	sX[1][1] = invertX - subLen;
	mainLen--;
	eX[0][0] = posX + mainLen + subLen;
	eX[0][1] = posX + mainLen;
	eX[1][0] = invertX + mainLen + subLen;
	eX[1][1] = invertX + mainLen;

	// Space left to minX / maxX
	spaceX[0][0] = maxX - eX[0][0];
	spaceX[0][1] = sX[0][1] - minX;
	spaceX[1][0] = maxX - eX[1][0];
	spaceX[1][1] = sX[1][1] - minX;

	// Final validity
	main_sub[0][0] = (spaceX[0][0] >= 0) && (sX[0][0] >= minX);
	main_sub[0][1] = (spaceX[0][1] >= 0) && (eX[0][1] <= maxX);
	main_sub[1][0] = (spaceX[1][0] >= 0) && (sX[1][0] >= minX);
	main_sub[1][1] = (spaceX[1][1] >= 0) && (eX[1][1] <= maxX);

	// Find the valid one with the biggest space
	int id = 0;
	for (int i = 0; i < 2; i++){
		for (int j = 0; j < 2; j++){
			id++;
			if (main_sub[i][j]){
				if (main_sub[0][0]){
					if ((spaceX[0][0] < spaceX[i][j])){
						spaceX[0][0] = spaceX[i][j];
						main_sub[0][0] = id;
					}			
				}
				else{
					spaceX[0][0] = spaceX[i][j];
					main_sub[0][0] = id;
				}
			}
		}
	}
		
	// Set the valid ones
	int mainSet = 0;
	int subSet = 0;
	switch (main_sub[0][0]){
	case 1:
		// main regular - subs to the right
		break;
	case 2:
		// main regular - subs to the left
		subSet = -1;
		break;
	case 3:
		// main inverted - subs to the right
		mainSet = -1;
		break;
	case 4:
		// main inverted - subs to the left
		mainSet = -1;
		subSet = -1;
		break;
	default:
		// too big to render
		if (TUIsetSubSmall(menuPos)){
			TUIpreRenderSub(posX, width, menuPos, minX, maxX);
			return 1;
		}
		else{
			// we can't go smaller - finally too big to render
			return 0;
		}
	}

	// Set the printInverted
	menuPos->printInverted = mainSet;
	while (menuPos){
		if (menuPos->selected && menuPos->enabled){
			// next level
			menuPos = menuPos->pos1st;
			if (menuPos){
				menuPos->printInverted = subSet;
			}		
		}
		else{
			// next position
			menuPos = menuPos->nextPos;
		}
	}

	return 1;	

}

int TUIrenderSub(int posX, int posY, int width, struct TuiMenuPosSTRUCT *menuPos, struct TuiMenusSTRUCT *menuDef, int isMain, int menuType, int minX, int minY, int maxX, int maxY){

	int height = 0;
	int selected = 0;
	// int ignoreY = 0;

	int maxHeight = maxY - minY + 1;
		
	struct TuiMenuPosSTRUCT *selectedPos = NULL;
	struct TuiMenuPosSTRUCT *menuPos1st = menuPos;
	
	while (menuPos){
		height++;
		if (menuPos->selected && menuPos->enabled){
			selected = height;
			selectedPos = menuPos->pos1st;
		}
		menuPos = menuPos->nextPos;		
	}

	menuPos = menuPos1st;

	int stepY = 1;

	if (menuType == TUI_MENU_BOTTOM){
		// Down -> Up (Bottom Menu) - flip posY for Y-calculation
		posY = maxY - posY + minY;
		stepY = -1;
	}

	int shiftY = posY;

	int spaceBelowReg = maxY - (posY + height - 1);			// space below regular rendered menu
	
	// Up -> Down
	if (spaceBelowReg < 0){
		// top alignment doesn't fit
		int spaceAboveInv = (posY - height + 1) - minY;				// space above inverted rendered menu
		if (spaceAboveInv < 0){
			// bottom alignment doesn't fit either - move up
			shiftY = minY;
			int spaceAboveReg = 0;									// space above regular rendered menu		
			spaceBelowReg = maxY - (shiftY + height - 1);
			if (spaceBelowReg < 0 && selected){
				// very top alignment doesn't fit either - take care on position of selection
				int selPosReg = shiftY + selected - (selected > 0);	// selected, absolute position regular
				if (selPosReg > maxY){
					// selection is hidden - lift selection to the bottom
					shiftY -= (selPosReg - maxY);
					// re-use spaceAboveReg and spaceBelowReg
					spaceBelowReg = height - selected;				// cnt of hidden elements below selection
					spaceAboveReg = maxHeight - 1;					// cnt of visible elements above selection
				}
				else{
					// selection is visible
					// re-use spaceAboveReg and spaceBelowReg
					spaceBelowReg = (height - selected) - (maxY - selPosReg);	// cnt of hidden elements below selection
					spaceAboveReg = selPosReg - minY;							// cnt of visible elements above selection
				}
				if (spaceBelowReg > spaceAboveReg){
					// more hidden below - move up
					shiftY -= (spaceAboveReg / 2);
				}
				else{
					// more visible above - move up all invisible
					shiftY -= spaceBelowReg;
				}
			}
		}
		else{
			// bottom alignment fits - move up
			shiftY = posY - height + 1;
		}			
	}
	
	posY = shiftY;

	if (menuType == TUI_MENU_BOTTOM){
		// Down -> Up (Bottom Menu) - flip back posY from Y-calculation
		posY = maxY - posY + minY;
		stepY = -1;
	}

	int subLen = TUIgetSubLen(menuPos, 0, 0);
	int endX = 0;
	int nextPosX = 0;

	if (isMain){
		// 1st sub, (below/above width)
		if (menuPos->printInverted){
			// inverted
			int diff = subLen - width;
			posX -= diff;
		}
		endX = posX + subLen;
		nextPosX = posX;
	}
	else{
		// next subs (left/right of width)
		if (menuPos->printInverted){
			// left of width
			endX = posX + width;
			posX -= subLen;
			nextPosX = posX;
		}
		else{
			// right of width
			nextPosX = posX;
			posX += width;
			endX = posX + subLen;
		}
	}
	
	int newWidth = endX - nextPosX;

	int printSmall = menuPos->printSmall;
	if (printSmall){
		printSmall = subLen;
	}
	
	selectedPos = NULL;

	int lastValidY = 0;

	while (menuPos){
		if (posY >= minY && posY <= maxY){
			ESClocate(LOG_TuiCopy, posX, posY);
			lastValidY = posY;
			if (menuPos->selected && menuPos->enabled){
				// selected
				selectedPos = menuPos->pos1st;
				selected = posY;
			}
			//printf("Pos:Max %d:%d", posY, maxY);
			TUIprintMenuPos(posX, posY, printSmall, subLen, menuPos, menuDef);
		}
		posY += stepY;
		menuPos = menuPos->nextPos;		
	}

	if (selectedPos){
		TUIrenderSub(nextPosX, selected, newWidth, selectedPos, menuDef, 0, menuType, minX, minY, maxX, maxY);
	}

	return lastValidY + stepY;
	
}

int TUIrenderHeaderFooter(int posX, int posY, int width, struct TuiHeadersSTRUCT *headerDef, int justRefresh){
	
	int renderLen = 0;
	int renderWidth = 0;
	int renderRealTime = 0;
	int renderRunTime = 0;
	int renderSmall = 0;
	// int dontRender = 0;

	int maxYdummy = 0;

	TUIsecureMinMaxXY(&posX, &posY, &width, &maxYdummy);

	char strHLP[STR_MID_SIZE];

	//struct TuiHeadersSTRUCT *headerDef = &userHeaders[headerID];

	// Width - respecting posX
	width = width - posX + 1;
	renderWidth = width;
	strcpy(strHLP, headerDef->caption);
	renderLen = strlen(strHLP);
	
	if (headerDef->printRealTime){
		width -= 20;	// 01.01.2023 09:09:21  (+ trailing space)
		renderRealTime = 1;
	}
	if (headerDef->printRunTime){
		width -= 16;	// 00000d 00:00:00 (+ leading space)
		renderRunTime = 1;
	}
	if (renderRealTime && renderRunTime){
		renderLen += 3;	// cause runtime is smaller than realtime and we'll eventually add three spaces later
	}
	
	if (renderLen > width){
		// menu/caption doesn't fit - f*ck
		if (renderRunTime){
			width += 16;	// 00000d 00:00:00 (+ leading space)
			if (renderRealTime && renderRunTime){
				renderLen -= 3;	// runtime removed...
			}
			renderRunTime = 0;
			if (renderLen > width){
				// still too small...
				if (renderRealTime){
					width += 20;	// 01.01.2023 09:09:21  (+ trailing space)
					renderRealTime = 0;
					if (renderLen > width){
						// we're finally f*cked
						renderSmall = 1;
					}
				}
				else{
					// we're finally f*cked
					renderSmall = 1;
				}
				
			}
			
		}
		else if (renderRealTime){
			width += 20;	// 01.01.2023 09:09:21  (+ trailing space)
			renderRealTime = 0;
			if (renderLen > width){
				// we're finally f*cked
				renderSmall = 1;
			}
		}
		if (renderLen > width){
			// too long without time
			renderSmall = 1;
		}
		
		if (renderSmall){
			// cut caption
			if (width >= 3){
				/* code */
				strncpy(strHLP, headerDef->caption, width - 3);
				strHLP[width - 3] = '~';
				strHLP[width - 2] = '\0';
			}
			else{
				// too small to render 
				return 0;
			}
		}
		else{
			// full caption fits
		}
	}
	else{
		// full caption fits
	}
	
	// Where to render
	if (posX && posY){
		ESClocate(LOG_TuiCopy, posX, posY);
	}
	else if (posX){
		ESClocateX(LOG_TuiCopy, posX);
	}
	
	if (!justRefresh){
		// Style & Color
		TUIsetColorStyle(LOG_TuiCopy, headerDef->txtColor, headerDef->txtStyle);
		if (renderRealTime ^ renderRunTime) {
			// one time active - center is left of time...
			STRprintCentered(LOG_TuiCopy, strHLP, width);
			STRprintSpaces(LOG_TuiCopy, renderWidth - width);
		}
		else{
			// center is center
			if (renderRealTime & renderRunTime){
				// add three spaces, cause runtime is shorter than realtime
				strHLP[renderLen - 3] = ' '; strHLP[renderLen - 2] = ' '; strHLP[renderLen - 1] = ' '; strHLP[renderLen] = '\0';
			}
			STRprintCentered(LOG_TuiCopy, strHLP, renderWidth);					
		}
	}
	else{
		// we just refresh time(s)
		ESCcursorRight(LOG_TuiCopy, renderWidth);
	}

	if (renderRealTime || renderRunTime){
		// Set style & color of times
		TUIsetColorStyle(LOG_TuiCopy, headerDef->timeColor, headerDef->timeStyle);
	}
	
	// Do we print the Realtime (all time right alignment)
	if (renderRealTime){
		// 01.01.2023 00:00:00
		ESCcursorLeft(LOG_TuiCopy, 19);
		fprintf(LOG_TuiCopy, "%s %s", gStrDate, gStrTime);
		ESCcursorRight(LOG_TuiCopy, 1);
	}
	// Do we print the RunTime
	if (renderRunTime){
		// 00000d 00:00:00
		if (renderRealTime){
			// left alignment, cause RealTime already exist
			ESCcursorLeft(LOG_TuiCopy, renderWidth - 2);
		}
		else{
			// right alignment
			ESCcursorLeft(LOG_TuiCopy, 15);
		}
		fprintf(LOG_TuiCopy, "%s", gStrRunTime);
	}	

	return 1;

}
#define TUIrenderHeader(posX, posY, width, headerID, justRefresh) TUIrenderHeaderFooter(posX, posY, width, &userHeaders[headerID], justRefresh)
#define TUIrenderFooter(posX, posY, width, footerID, justRefresh) TUIrenderHeaderFooter(posX, posY, width, &userFooters[footerID], justRefresh)

int TUIrenderHorzMenu(int posX, int posY, int menuType, struct TuiMenusSTRUCT *menuDef, int minX, int minY, int maxX, int maxY){

	int renderSmall = 0;	// just the key + 2 spaces
	int renderSelect = 0;	// "just the key" + selected key in full width 
	int renderLen = 0;		// full size len of menu positions (gets the final length, too)

	int renderStyle = 0;	// 0 = full; 1 = small, but full selected; 2 = small

	struct TuiMenuPosSTRUCT *selectedMenu = NULL;

	int subXs = 0;  // subMenu X
	int subXw = 0;	// subMenu Width	
	
	TUIsecureMinMaxXY(&minX, &minY, &maxX, &maxY);

	if (!posX){
		// Start at 1st Pos
		posX = minX;
	}
	if (!posY){
		if (menuType == TUI_MENU_BOTTOM){
			// BottomMenu
			posY = maxY;
		}
		else{
			// TopMenu
			posY = minY;
		}
	}
		
	// renderWidth - respecting posX
	int renderWidth = maxX - posX + 1;	

	struct TuiMenuPosSTRUCT *menuPos = menuDef->pos1st;

	// delete small and inverted flags from sub-structure before render
	TUIclearSmallInverted(menuPos);

	// Calculate len of top-level MenuLine options - without time & date
	while (menuPos != NULL){
		renderLen += strlen(menuPos->caption);
		renderSmall += 3;
		if (menuPos->selected && menuPos->enabled){
			selectedMenu = menuPos;	
			renderSelect = strlen(menuPos->caption) - 3;
		}
		menuPos = menuPos->nextPos;
	}

	renderSelect += renderSmall;

	if (renderLen > renderWidth){
		// full menu doesn't fit
		renderStyle = 1;
		renderLen = renderSelect;

		if (renderLen > renderWidth){
			// small, but full selected, doesn't fits
			renderStyle = 2;
			renderLen = renderSmall;

			if (renderLen > renderWidth){
				// impossible to render
				return 0;
			}
		}
	}

	// Where to render
	ESClocate(LOG_TuiCopy, posX, posY);
		
	menuPos = menuDef->pos1st;
	while (menuPos){
		if (renderStyle){
			// we can't fully render this menu
			menuPos->printSmall = -1;
		}
		if (menuPos->selected && menuPos->enabled){
			// selected - start of selected pos
			subXs = posX;
			if (renderStyle == 1){
				// but the selected pos is in full width
				menuPos->printSmall = 0;
			}
		}
		posX += TUIprintMenuPos(posX, posY, 0, TUIgetPosLen(menuPos), menuPos, menuDef);
		if (menuPos->selected && menuPos->enabled){
			// selected - length of selected pos
			subXw = posX - subXs;
		}
		menuPos = menuPos->nextPos;
	}
	
	// Fill line with right colored spaces
	TUIsetColorStyle(LOG_TuiCopy, menuDef->txtColor, menuDef->txtStyle);

	STRprintSpaces(LOG_TuiCopy, renderWidth - renderLen);
		
	if (selectedMenu && selectedMenu->pos1st){
		if (menuType == TUI_MENU_BOTTOM){
			// BottomMenu
			posY--;
			maxY--;
		}
		else{
			// TopMenu
			posY++;
			minY++;
		}
		
		if (TUIpreRenderSub(subXs, subXw, selectedMenu->pos1st, minX, maxX)){
			TUIrenderSub(subXs, posY, subXw, selectedMenu->pos1st, menuDef, 1, menuType, minX, minY, maxX, maxY);
		}
	}

	return renderWidth;

}
#define TUIrenderTopMenu(menuDef, justRefresh, minX, minY, maxX, maxY) TUIrenderHorzMenu(0, 0, TUI_MENU_TOP, menuDef, minX, minY, maxX, maxY)
#define TUIrenderBottomMenu(menuDef, justRefresh, minX, minY, maxX, maxY) TUIrenderHorzMenu(0, 0, TUI_MENU_BOTTOM, menuDef, minX, minY, maxX, maxY)

int TUIrenderVertMenu(int posX, int posY, int menuType, int doLead, int doTrail, struct TuiMenusSTRUCT *menuDef, int minX, int minY, int maxX, int maxY){
	
	// Take care on 0, 0, 0, 0
	TUIsecureMinMaxXY(&minX, &minY, &maxX, &maxY);

	// Take care on 0, 0
	if (!posX){
		if (menuType == TUI_MENU_RIGHT){
			// RightMenu
			posX = maxX - 2;
		}
		else{
			// LeftMenu
			posX = minX;
		}
	}
	if (!posY){
		posY = minY;
	}

	
	// TUIclearSmallInverted(menuDef->pos1st);	// We can't do this here anymore - could be already set

	if (TUIpreRenderSub(posX, 3, menuDef->pos1st, minX, maxX)){
		
		// Leading Line
		int len = 0;
		int lineX = posX;
		if (doLead || doTrail){
			len = TUIgetSubLen(menuDef->pos1st, 0, 0);
			if (menuType == TUI_MENU_RIGHT){
				// RightMenu - fix X for leading and trailing line
				lineX -= len - 3;
			}
		}
		
		if (doLead){
			// Leading Line
			ESClocate(LOG_TuiCopy, lineX, posY);
			TUIsetColorStyle(LOG_TuiCopy, menuDef->txtColor, menuDef->txtStyle);
			STRprintSpaces(LOG_TuiCopy, len);
		}
		
		int startY = posY + (doLead != 0);
		posY = maxY;
		//minY += doLead;
		//maxY -= (doTrail != 0);
		if (maxY - (doTrail != 0) >= minY + (doLead != 0)){
			posY = TUIrenderSub(posX, startY, 3, menuDef->pos1st, menuDef, 1, 0, minX, minY + (doLead != 0), maxX, maxY - (doTrail != 0));
		}
		
		if (doTrail){
			// Trailing Line(s)
			TUIsetColorStyle(LOG_TuiCopy, menuDef->txtColor, menuDef->txtStyle);
			while (posY <= maxY){
				ESClocate(LOG_TuiCopy, lineX, posY);
				STRprintSpaces(LOG_TuiCopy, len);
				posY++;
				if (doTrail > 0){
					break;	// just negative fills to the bottom
				}
			}
		}

		return posY - startY + 1; // Return real height/count of 1st level including leading and trailing line

	}
	
	return 0;
	
}
#define TUIrenderLeftMenu(menuDef, doLead, doTrail, minX, minY, maxX, maxY) TUIrenderVertMenu(0, 0, TUI_MENU_LEFT, doLead, doTrail, menuDef, minX, minY, maxX, maxY)
#define TUIrenderRightMenu(menuDef, doLead, doTrail, minX, minY, maxX, maxY) TUIrenderVertMenu(0, 0, TUI_MENU_RIGHT, doLead, doTrail, menuDef, minX, minY, maxX, maxY)

void TUIbuildMenus(struct TuiDesktopsSTRUCT *deskDef, int minX, int minY, int maxX, int maxY){

	struct TuiMenuPosSTRUCT selectedPos;

	struct TuiMenusSTRUCT *topMenu = NULL;
	struct TuiMenusSTRUCT *bottomMenu = NULL;
	struct TuiMenusSTRUCT *leftMenu = NULL;
	struct TuiMenusSTRUCT *rightMenu = NULL;

	if (deskDef->topMenu){
		topMenu = &userTopMenus[deskDef->topMenu - 1];
		TUIclearSmallInverted(topMenu->pos1st);
	}
	if (deskDef->bottomMenu){
		bottomMenu = &userBotMenus[deskDef->bottomMenu - 1];
		TUIclearSmallInverted(bottomMenu->pos1st);
	}
	if (deskDef->leftMenu){
		leftMenu = &userLeftMenus[deskDef->leftMenu - 1];
		TUIclearSmallInverted(leftMenu->pos1st);
	}
	if (deskDef->rightMenu){
		rightMenu = &userRightMenus[deskDef->rightMenu - 1];
		TUIclearSmallInverted(rightMenu->pos1st);
	}

	// Is there a selected Menu
	int selectedMenu = TUIfindSelectedMenu(deskDef, &selectedPos);

	// Take care on 0, 0, 0, 0
	TUIsecureMinMaxXY(&minX, &minY, &maxX, &maxY);

	int maxWidth = maxX - minX + 1;
	//int maxHeight = maxY - minY + 1;

	// Full width and lens of left and right menu
	int leftLen = 0;
	int rightLen = 0;
	int fullLen = 0;
	if (leftMenu){
		leftLen = TUIgetSubLen(leftMenu->pos1st, 0, 0);
		fullLen += leftLen;
	}
	if (rightMenu){
		rightLen = TUIgetSubLen(rightMenu->pos1st, 0, 0);
		fullLen += rightLen;	
	}

	// TopMenu or Bottom with Left or Right Menu - TopBotMenus space to render is reduced
	// So, we need to know exactly how much space we have/need to render 
	if ((deskDef->topMenu || deskDef->bottomMenu) && (deskDef->leftMenu || deskDef->rightMenu)){
		if (selectedMenu == TUI_MENU_LEFT){
			if (rightLen && (fullLen > maxWidth)){
				// make unselected small
				rightMenu->pos1st->printSmall = -1;
				rightLen = TUIgetSubLen(rightMenu->pos1st, 0, 0);
			}
			TUIpreRenderSub(minX, 3, leftMenu->pos1st, minX, maxX - rightLen);
			leftLen = TUIgetSubLen(leftMenu->pos1st, 0, 0);	// Len could be smaller now!
		}
		else if (selectedMenu == TUI_MENU_RIGHT){
			if (leftLen && (fullLen > maxWidth)){
				// make unselected small
				leftMenu->pos1st->printSmall = -1;
				leftLen = TUIgetSubLen(leftMenu->pos1st, 0, 0);
			}			
			TUIpreRenderSub(maxX - 2, 3, rightMenu->pos1st, minX + leftLen, maxX);
			rightLen = TUIgetSubLen(rightMenu->pos1st, 0, 0);	// Len could be smaller now!
		}
	}

	// Header
	if (deskDef->header){
		TUIrenderHeader(minX, minY, maxWidth, deskDef->header - 1, 0);
		minY++;
	}
	// Footer
	if (deskDef->footer){
		TUIrenderFooter(minX, maxY, maxWidth, deskDef->footer - 1, 0);
		maxY--;
	}

	// Left
	if (leftMenu){
		if (TUIrenderLeftMenu(leftMenu, (topMenu != NULL), -1, minX, minY, maxX - rightLen, maxY)){
			minX += leftLen;
		}
	}
	// Right
	if (rightMenu){
		if (TUIrenderRightMenu(rightMenu, (topMenu != NULL), -1, minX + leftLen, minY, maxX, maxY)){
			maxX -= rightLen;
		}
	}
	// Top
	if (topMenu){
		if (TUIrenderTopMenu(topMenu, 0, minX, minY, maxX, maxY - (bottomMenu != NULL))){
			minY++;
		}
	}
	// Bottom
	if (bottomMenu){
		if (TUIrenderBottomMenu(bottomMenu, 0, minX, minY, maxX, maxY)){
			maxY--;
		}
	}

}

int TUIinitHeadFoots(char *strFile, char *strLocation, struct TuiHeadersSTRUCT **userHeader){

	// User Header - Helper
	char strSearch[STR_SMALL_SIZE];
	char strHeaderCaption[STR_SMALL_SIZE];
	char strHLP[STR_SMALL_SIZE];

	// count of headers
	sprintf(strSearch, "%s.%sCount", strLocation, strLocation);
	int headersCount = INIgetInt(strFile, strSearch, 0);

	// memory to store all header
	*userHeader = (struct TuiHeadersSTRUCT*)malloc(headersCount * sizeof(struct TuiHeadersSTRUCT));
	int j = 0;	
	for (int i = 0; i < headersCount; i++){
		
		j++; // User index in file is 1-based...
		
		// Caption
		sprintf(strSearch, "%s.%d.Caption", strLocation, j);
		sprintf(strHLP, "Header%d", j);
		INIgetStr(strFile, strSearch, strHLP, strHeaderCaption);
		(*userHeader)[i].caption = INIstrToMem(strHeaderCaption, 0);
		// TextColor 
		sprintf(strSearch, "%s.%d.TextColor", strLocation, j);
		(*userHeader)[i].txtColor = INIgetInt(strFile, strSearch, 0);
		// TextStyle 
		sprintf(strSearch, "%s.%d.TextStyle", strLocation, j);
		(*userHeader)[i].txtStyle = INIgetInt(strFile, strSearch, 0);
		// TimeColor 
		sprintf(strSearch, "%s.%d.TimeColor", strLocation, j);
		(*userHeader)[i].timeColor = INIgetInt(strFile, strSearch, 0);
		// TimeStyle 
		sprintf(strSearch, "%s.%d.TimeStyle", strLocation, j);
		(*userHeader)[i].timeStyle = INIgetInt(strFile, strSearch, 0);
		// PrintRunTime 
		sprintf(strSearch, "%s.%d.PrintRunTime", strLocation, j);
		(*userHeader)[i].printRunTime = INIgetBool(strFile, strSearch, 0);
		// PrintRealTime 
		sprintf(strSearch, "%s.%d.PrintRealTime", strLocation, j);
		(*userHeader)[i].printRealTime = INIgetBool(strFile, strSearch, 0);

	}

	return headersCount;
	
}
#define TUIinitHeaders(strFile, userHeader) TUIinitHeadFoots(strFile, "Header", userHeader)
#define TUIinitFooters(strFile, userFooter) TUIinitHeadFoots(strFile, "Footer", userFooter)

struct TuiMenuPosSTRUCT *TUIaddMenuPos(const char *strFile, char *strPath, struct TuiMenusSTRUCT *definition, int positions, int testMe, int isSub){

	static struct TuiMenuPosSTRUCT *menuPos = NULL;
	
	static int cnt = 0;
	static int total = 0;

	// Helper
	char strSearch[STR_SMALL_SIZE];
	char strHLP[STR_SMALL_SIZE];
	char strPos1[STR_SMALL_SIZE];

	char strMIDhlp[STR_MID_SIZE];

	int posReturn = cnt;	// save for return
	int lastPos = 0;

	int j = 0;

	for (int i = 0; i < positions; i++){
		
		j++; // User index in file is 1-based...

		// Add mem for new position
		cnt++;
		int pos1 = cnt - 1;
//printf("pre realloc...\n");
		menuPos = realloc(menuPos, cnt * sizeof(struct TuiMenuPosSTRUCT));
		/*
		// Working !! - For the case of realloc is doing a mess (like in IniStrToMem)
		if (pos1){
			menuHLP = malloc(cnt * sizeof(struct TuiMenuPosSTRUCT));
			memcpy(menuHLP, menuPos, cnt * sizeof(struct TuiMenuPosSTRUCT));
			free(menuPos);
			menuPos = malloc(cnt * sizeof(struct TuiMenuPosSTRUCT));
			memcpy(menuPos, menuHLP, cnt * sizeof(struct TuiMenuPosSTRUCT));
			free(menuHLP);
		}
		else{
			menuPos = malloc(cnt * sizeof(struct TuiMenuPosSTRUCT));
		}
		*/
		
//printf("after realloc...\n");
		sprintf(strSearch, "%s%d.Enabled", strPath, j);
		menuPos[pos1].enabled = INIgetBool(strFile, strSearch, 1);
		sprintf(strSearch, "%s%d.isOption", strPath, j);
		menuPos[pos1].isOption = INIgetBool(strFile, strSearch, 0);
		sprintf(strSearch, "%s%d.isCheck", strPath, j);
		menuPos[pos1].isCheck = INIgetBool(strFile, strSearch, 0);
		sprintf(strSearch, "%s%d.isActivated", strPath, j);
		menuPos[pos1].activated = INIgetBool(strFile, strSearch, 0);
		sprintf(strSearch, "%s%d.Positions", strPath, j);
		menuPos[pos1].posCnt = INIgetInt(strFile, strSearch, 0);

		sprintf(strSearch, "%s%d.Text", strPath, j);
		sprintf(strHLP, "%s%d", strPath, j);
		INIgetStr(strFile, strSearch, strHLP, strPos1);
		
		// Search key and remove brackets
		menuPos[pos1].keyCode = 0;

		int k = 0;
		int keyCodeFound = 0;
		int len = (int)strlen(strPos1);
		for (int l = 0; l < len; l++){
			if (strPos1[l] == '(' && !keyCodeFound){
				keyCodeFound = 1;
				menuPos[pos1].keyCode = k;
				l++;
				strHLP[k++] = strPos1[l++];
				if (strPos1[l] == ')'){
					// Key is encapsulated
					l++;
				}
			}
			strHLP[k++] = strPos1[l];
		}
		strHLP[k] = '\0';
		if (strHLP[0] == '-'){
			// indicates a spacer
			menuPos[pos1].isSpacer = -1;
		}
		else{
			menuPos[pos1].isSpacer = 0;
			// increase keyPos (leading space)
			menuPos[pos1].keyCode++;
		}
		
		// Build leading part
		if (menuPos[pos1].isOption){
			// ( ) / (x)
			if (menuPos[pos1].activated){
				strcpy(strPos1, " (x) ");
			}
			else{
				strcpy(strPos1, " ( ) ");
			}
			menuPos[pos1].keyCode += 4;
		}
		else if (menuPos[pos1].isCheck){
			// [ ] / [x]
			if (menuPos[pos1].activated){
				strcpy(strPos1, " [x] ");
			}
			else{
				strcpy(strPos1, " [ ] ");
			}
			menuPos[pos1].keyCode += 4;
		}
		else{
			// just leading and trailing spaces
			strcpy(strPos1, " ");
		}
		
		sprintf(strMIDhlp, "%s%s ", strPos1, strHLP);

//printf("pre strToMem...%s...\n", strMIDhlp);
//fflush(stdout);

		menuPos[pos1].caption = INIstrToMem(strMIDhlp, 0);
//printf("%d: %d: %s\n", pos1, &menuPos[pos1], menuPos[pos1].caption);

		// stuff to reset
		menuPos[pos1].nextPos = NULL;
		menuPos[pos1].prevPos = NULL;
		menuPos[pos1].pos1st = NULL;
		menuPos[pos1].parentPos = NULL;

		menuPos[pos1].nextID = 0;
		menuPos[pos1].prevID = 0;
		menuPos[pos1].pos1ID = 0;

		menuPos[pos1].selected = 0;

		menuPos[pos1].printSmall = 0;
		menuPos[pos1].printInverted = 0;
		
		// if not 1st pos, we have to set...
		if (i){
			// save IDs instead of pointers while recursion (realloc moves pointer)
			menuPos[lastPos].nextID = pos1;
		}

		
		menuPos[pos1].prevID = lastPos;
		lastPos = pos1;
		
		if (menuPos[pos1].posCnt){
			// SubMenu exist
			sprintf(strSearch, "%s%d.", strPath, j);
			//pos1st[pos1 + 1] = pos1 + 1;
			menuPos[pos1].pos1ID = cnt;
			menuPos[cnt].pos1ID = pos1;

			TUIaddMenuPos(strFile, strSearch, definition, menuPos[pos1].posCnt, 0, 1);
		}
		else{
			// No SubMenu
			menuPos[pos1].pos1ID = 0;
		}
	}

//printf("pre Restore...\n");
	j = 0;
	for (int i = 0; i < cnt; i++){
		// restore valid pointers from IDs
		if (menuPos[i].prevID || (j && (i == j))){
			menuPos[i].prevPos = &menuPos[menuPos[i].prevID];
		}
		if (menuPos[i].nextID){
			menuPos[i].nextPos = &menuPos[menuPos[i].nextID];
			if (!i){
			//if (!menuPos[i].prevPos){ // Works, too !!
				// e.g. File  Edit  Settings
				// keep ID of "Edit" to get/set missing prevPos from "Edit"
				// see "|| (j && (i == j))" in 1st IF
				j = menuPos[i].nextID;
			}
		}
		if (menuPos[i].pos1ID){
			menuPos[i].pos1st = &menuPos[menuPos[i].pos1ID];
			menuPos[menuPos[i].pos1ID].parentPos = &menuPos[i];
			menuPos[menuPos[i].pos1ID].parentID = i;
		}
	}
	definition->pos1ID = posReturn;
	// Restore pos1st pointer of all menus
	for (int i = 0; i < TUI_TopMenuCnt; i++){
		userTopMenus[i].pos1st = &menuPos[userTopMenus[i].pos1ID];
	}
	for (int i = 0; i < TUI_BotMenuCnt; i++){
		userBotMenus[i].pos1st = &menuPos[userBotMenus[i].pos1ID];
	}
	for (int i = 0; i < TUI_LeftMenuCnt; i++){
		userLeftMenus[i].pos1st = &menuPos[userLeftMenus[i].pos1ID];
	}
	for (int i = 0; i < TUI_RightMenuCnt; i++){
		userRightMenus[i].pos1st = &menuPos[userRightMenus[i].pos1ID];
	}
	for (int i = 0; i < TUI_PopUpMenuCnt; i++){
		userPopUpMenus[i].pos1st = &menuPos[userPopUpMenus[i].pos1ID];
	}

//printf("after Restore...\n");
	if (testMe){
		for (int i = 0; i < cnt; i++){
			//printf("pos: %d @: %p pos1st: %p parentPos: %p nextPos: %p prevPos: %p\n", i, &menuPos[i], menuPos[i].pos1st, menuPos[i].parentPos, menuPos[i].nextPos, menuPos[i].prevPos);
			//printf("pos: %d @: %s pos1st: %s nextPos: %s prevPos: %s\n\n", i, menuPos[i].caption, menuPos[i].pos1st->caption, menuPos[i].nextPos->caption, menuPos[i].prevPos->caption);
			//printf("pos: %d @: %s pos1st: ", i, menuPos[i].caption);
			
			if (menuPos[i].pos1st){
				printf("%s parentPos: ", menuPos[i].pos1st->caption);
			}
			else{
				printf("NULL parentPos: ");
			}
			if (menuPos[i].parentPos){
				printf("%s nextPos: ", menuPos[i].parentPos->caption);
			}
			else{
				printf("NULL nextPos: ");
			}

			if (menuPos[i].nextPos){
				printf("%s prevPos: ", menuPos[i].nextPos->caption);
			}
			else{
				printf("NULL prevPos: ");
			}
			if (menuPos[i].prevPos){
				printf("%s\n\n", menuPos[i].prevPos->caption);
			}
			else{
				printf("NULL\n\n");
			}
		}
		// return NULL;
	}

// printf("\n%d: %d\n\n", posReturn, &menuPos[posReturn]);	
	return &menuPos[posReturn];
	
}

// "TopMenu."
int TUIinitMenuDefs(char *strFile, char *strPath, struct TuiMenusSTRUCT **menu){

	// Helper
	char strSearch[STR_SMALL_SIZE];

	sprintf(strSearch, "%s.%s", strPath, "Count");
	int menusCnt = INIgetInt(strFile, strSearch, 0);

//printf("menusCnt: %d\n", menusCnt);
//fflush(stdout);

	*menu = (struct TuiMenusSTRUCT*)malloc(menusCnt * sizeof(struct TuiMenusSTRUCT));

//printf("after malloc...\n");
//fflush(stdout);

	int j = 0;
	// Menu Definition Values
	for (int i = 0; i < menusCnt; i++){
		
		j++; // User index in file is 1-based...
		
		sprintf(strSearch, "%s.%d.TextColor", strPath, j);
		(*menu)[i].txtColor = INIgetInt(strFile, strSearch, 0);
//printf("1st\n");
		sprintf(strSearch, "%s.%d.TextStyle", strPath, j);
		(*menu)[i].txtStyle = INIgetInt(strFile, strSearch, 0);
//printf("2nd\n");
		sprintf(strSearch, "%s.%d.SelectColor", strPath, j);
		(*menu)[i].selectColor = INIgetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.SelectStyle", strPath, j);
		(*menu)[i].selectStyle = INIgetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.SelectKeyColor", strPath, j);
		(*menu)[i].selectKeyColor = INIgetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.SelectKeyStyle", strPath, j);
		(*menu)[i].selectKeyStyle = INIgetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.DisabledColor", strPath, j);
		(*menu)[i].disabledColor = INIgetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.DisabledStyle", strPath, j);
		(*menu)[i].disabledStyle = INIgetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.KeyColor", strPath, j);
		(*menu)[i].keyColor = INIgetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.KeyStyle", strPath, j);
		(*menu)[i].keyStyle = INIgetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.TimeColor", strPath, j);
		(*menu)[i].timeColor = INIgetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.TimeStyle", strPath, j);
		(*menu)[i].timeStyle = INIgetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.PrintRunTime", strPath, j);
		(*menu)[i].printRunTime = INIgetBool(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.PrintRealTime", strPath, j);
		(*menu)[i].printRealTime = INIgetBool(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.Positions", strPath, j);
		(*menu)[i].posCnt = INIgetInt(strFile, strSearch, 0);

//printf("last\n");
//fflush(stdout);
		// Add positions
		sprintf(strSearch, "%s.%d.", strPath, j);
		(*menu)[i].pos1st = TUIaddMenuPos(strFile, strSearch, menu[i], (*menu)[i].posCnt, 0, 0);	
//printf("after menuPos...\n");
//printf("%s : %d\n", (*menu)[i].pos1st->caption, (*menu)[i].posCnt);	
//printf("%s\n", menu[i]->pos1st->caption);	
//fflush(stdout);
	}

	return menusCnt;
	
}

int TUIinitDesktops(char *strFile, struct TuiDesktopsSTRUCT **desktop){
	
	// Helper
	char strSearch[STR_SMALL_SIZE];

	int desksCnt = INIgetInt(strFile, "desktops.Count", 0);


	*desktop = (struct TuiDesktopsSTRUCT*)malloc(desksCnt * sizeof(struct TuiDesktopsSTRUCT));
 
	int j = 0;
 	for (int i = 0; i < desksCnt; i++){
		
		j++; // User index in file is 1-based...

		// Header
		sprintf(strSearch, "desktops.%d.Header", j);
		(*desktop[i]).header = INIgetInt(strFile, strSearch, 0);
		// TopMenu
		sprintf(strSearch, "desktops.%d.TopMenu", j);
		(*desktop[i]).topMenu = INIgetInt(strFile, strSearch, 0);
		// BottomMenu
		sprintf(strSearch, "desktops.%d.BottomMenu", j);
		(*desktop[i]).bottomMenu = INIgetInt(strFile, strSearch, 0);
		// LeftMenu
		sprintf(strSearch, "desktops.%d.LeftMenu", j);
		(*desktop[i]).leftMenu = INIgetInt(strFile, strSearch, 0);
		// RightMenu
		sprintf(strSearch, "desktops.%d.RightMenu", j);
		(*desktop[i]).rightMenu = INIgetInt(strFile, strSearch, 0);
		// Footer
		sprintf(strSearch, "desktops.%d.Footer", j);
		(*desktop[i]).footer = INIgetInt(strFile, strSearch, 0);
	}

	return desksCnt;

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


