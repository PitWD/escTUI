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
#include "AnsiESC.h"

struct TuiHeadersSTRUCT{
	char *caption;
	int txtColor;
	int txtStyle;
	int timeColor;
	int timeStyle;
	uint8_t printRunTime :1;
	uint8_t printRealTime :1;
};
// global access on headers
struct TuiHeadersSTRUCT *userHeaders = NULL;

struct TuiMenusSTRUCT;
struct TuiMenuPosSTRUCT;

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
	int renderLen;					// count of sub-positions
	struct TuiMenuPosSTRUCT *pos1st;		
	uint8_t printRunTime :1;
	uint8_t printRealTime :1;
};
// global access on Menu Definitions
struct TuiMenusSTRUCT *userTopMenus = NULL;
struct TuiMenusSTRUCT *userBotMenus = NULL;
struct TuiMenusSTRUCT *userLeftMenus = NULL;
struct TuiMenusSTRUCT *userRightMenus = NULL;

struct TuiMenuPosSTRUCT{
	char *caption;
	struct TuiMenusSTRUCT *definition;
	int keyCode;
	int enabled;
	int posCnt;								// count of sub-positions
	struct TuiMenuPosSTRUCT *nextPos;		// on same level
	struct TuiMenuPosSTRUCT *prevPos;		// on same level
	struct TuiMenuPosSTRUCT *pos1st;		// first on sub level
	struct TuiMenuPosSTRUCT *parentPos;		// parent on upper level
	int nextID;					// helper for reallocating
	int prevID;					// helper for reallocating
	int pos1ID;					// helper for reallocating
	int parentID;				// calculated but not used actually
	int selected :1;			// if position is selected
	int isOption :1;
	int isCheck :1;
	int	activated :1;			// if position is a check or option...
	int isSpacer :1;
	int printSmall :1;			// force to print small (preCalcSubMenu)
	int printInverted :1;		// force to print inverted (preCalcSubMenu)
	int isSubMenu :1;			// if position is a subMenu
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
			menuPos->printSmall = 1;
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

int TUIprintMenuPos(int posX, int posY, int printSmall, int renderWidth, struct TuiMenuPosSTRUCT *menuPos, struct TuiMenusSTRUCT *menuDef){

	if (!renderWidth){
		renderWidth = TUIgetSubLen(menuPos, 0, 0);
	}
	
	if (printSmall || menuPos->printSmall){
		// render just the keys (+ eventually check/option brackets)
		if (menuPos->selected && menuPos->enabled){
			// enabled & selected
			SetColorStyle(&userColors[menuDef->selectColor], 1);
			SetTxtStyle(&userStyles[menuDef->selectStyle], 1);
		}
		else if (menuPos->enabled){
			// enabled
			SetColorStyle(&userColors[menuDef->txtColor], 1);
			SetTxtStyle(&userStyles[menuDef->txtStyle], 1);
		}
		else{
			// disabled
			SetColorStyle(&userColors[menuDef->disabledColor], 1);
			SetTxtStyle(&userStyles[menuDef->disabledStyle], 1);
		}
		printf(" ");	// Leading Space
		if (menuPos->isCheck){
			// is a check
			printf("[");
			if (menuPos->activated){
				printf("x");
			}
			else{
				printf(" ");
			}
			printf("] ");
		}
		else if (menuPos->isOption){
			// is an option
			printf("(");
			if (menuPos->activated){
				printf("x");
			}
			else{
				printf(" ");
			}
			printf(") ");
		}
		else if (printSmall == 7){
			if (menuPos->isSpacer){
				DEClineX(4);
			}
			else{
				StrPrintSpaces(4);
			}
		}
		else{
			// we're fine
		}
		
		if (menuPos->selected && menuPos->enabled){
			SetColorStyle(&userColors[menuDef->selectKeyColor], 1);
			SetTxtStyle(&userStyles[menuDef->selectKeyStyle], 1);
		}
		else if (menuPos->enabled && !menuPos->isSpacer){
			// Key in keyColor and keyStyle
			SetColorStyle(&userColors[menuDef->keyColor], 1);
			SetTxtStyle(&userStyles[menuDef->keyStyle], 1);
		}
		else{
			// Color & Style is fine
		}
		if (menuPos->isSpacer){
			DEClineX(1);
		}
		else{
			printf("%c", menuPos->caption[menuPos->keyCode]);
		}

		if (menuPos->selected && menuPos->enabled){
			// enabled & selected
			SetColorStyle(&userColors[menuDef->selectColor], 1);
			SetTxtStyle(&userStyles[menuDef->selectStyle], 1);
		}
		else if (menuPos->enabled){
			// enabled
			SetColorStyle(&userColors[menuDef->txtColor], 1);
			SetTxtStyle(&userStyles[menuDef->txtStyle], 1);
		}
		else{
			// disabled
			SetColorStyle(&userColors[menuDef->disabledColor], 1);
			SetTxtStyle(&userStyles[menuDef->disabledStyle], 1);
		}
		printf(" ");	// Trailing Space
		
	}

	else{
		// render full line
		for (size_t i = 0; i < strlen(menuPos->caption); i++){
			if (menuPos->enabled && menuPos->selected){
				// enabled - selected
				if (i && (i == menuPos->keyCode)){
					// is key
					SetTxtStyle(&userStyles[menuDef->selectKeyStyle], 1);
					SetColorStyle(&userColors[menuDef->selectKeyColor], 1);
				}
				else{
					SetTxtStyle(&userStyles[menuDef->selectStyle], 1);
					SetColorStyle(&userColors[menuDef->selectColor], 1);
				}
			}
			else if (menuPos->enabled){
				// enabled
				if (i && (i == menuPos->keyCode)){
					// is key
					SetColorStyle(&userColors[menuDef->keyColor], 1);
					SetTxtStyle(&userStyles[menuDef->keyStyle], 1);
				}
				else{
					SetColorStyle(&userColors[menuDef->txtColor], 1);
					SetTxtStyle(&userStyles[menuDef->txtStyle], 1);
				}
			}
			else{
				// disabled
				SetColorStyle(&userColors[menuDef->disabledColor], 1);
				SetTxtStyle(&userStyles[menuDef->disabledStyle], 1);
			}
			if (i == 2 && (menuPos->isCheck || menuPos->isOption)){
				// Set Value of check/option
				if (menuPos->activated){
					printf("x");
				}
				else{
					printf(" ");
				}
			}
			else{
				if (menuPos->isSpacer){
					/* code */
				}
				else{
					printf("%c", menuPos->caption[i]);
				}
			}
		}

		if (menuPos->isSpacer){
			printf(" ");
			DEClineX(renderWidth - 2);
			printf(" ");
		}
		else{
			StrPrintSpaces(renderWidth - strlen(menuPos->caption));
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
		subSet = 1;
		break;
	case 3:
		// main inverted - subs to the right
		mainSet = 1;
		break;
	case 4:
		// main inverted - subs to the left
		mainSet = 1;
		subSet = 1;
		break;
	default:
		// too big to render
		if (TUIsetSubSmall(menuPos)){
			TUIpreRenderSub(posX, width, menuPos, minX, maxX);
			return 1;
		}
		else{
			// we can't go smaller - too big to render
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

int TUIrenderSub(int posX, int posY, int width, struct TuiMenuPosSTRUCT *menuPos, struct TuiMenusSTRUCT *menuDef, int isMain, int downUp, int minX, int minY, int maxX, int maxY){

	int height = 0;
	int selected = 0;
	int ignoreY = 0;
	int shiftY = posY;

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
	
	if (!downUp){
		// Up -> Down
		if (posY + height - 1 > maxY){
			// top alignment doesn't fit
			shiftY = posY - height + 2;
			if (shiftY < minY){
				// bottom alignment doesn't fit either
				if (posY + selected - 1 > maxY){
					// selected not on screen with top alignment
					if (shiftY + selected + 2 < minY){
						// selected not on screen with bottom alignment either
						// center selected position // int halfHeight = maxHeight / 2 + minY;
						shiftY = (maxHeight / 2 + minY) - selected - 1;
					}
					else{
						// cut on top while looping - shiftY fits already
					}
				}
				else{
					// cut on bottom while looping
					shiftY = posY;
				}
			}
			else{
				// cut on top - shiftY fits already
			}
		}
	}
	else{
		// Down -> Up
		if (posY - height + 1 < minY){		//*
			// bottom alignment doesn't fit
			shiftY = posY + height - 1;		//*
			if (shiftY > maxY){				//*
				// top alignment doesn't fit either
				if (posY - selected + 1 < minY){	//*
					// selected not on screen with bottom alignment
					if (shiftY + selected - 1 > maxY){ //*
						// selected not on screen with bottom alignment either
						// center selected position // int halfHeight = maxHeight / 2 + minY;
						shiftY = (maxHeight / 2 + minY) + selected - 1;
					}
					else{
					}
				}
				else{
					shiftY = posY;
				}
			}
			else{
			}
		}
		stepY = -1;
	}
	
	posY = shiftY;

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
		nextPosX  = posX;
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

	while (menuPos){
		if (posY >= minY && posY <= maxY){
			Locate(posX, posY);
			if (menuPos->selected && menuPos->enabled){
				// selected
				selectedPos = menuPos->pos1st;
				selected = posY;
			}
			TUIprintMenuPos(posX, posY, printSmall, subLen, menuPos, menuDef);
			posY += stepY;
		}
		menuPos = menuPos->nextPos;		
	}

	if (selectedPos){
		TUIrenderSub(nextPosX, selected, newWidth, selectedPos, menuDef, 0, downUp, minX, minY, maxX, maxY);
	}

	return posY;
	
}

void TUIrenderHeader(int posX, int posY, int width, int headerID, int justRefresh){
	
	int renderLen = 0;
	int renderWidth = 0;
	int renderRealTime = 0;
	int renderRunTime = 0;
	int renderSmall = 0;
	int dontRender = 0;

	char strHLP[STR_MID_SIZE];

	if (!posX){
		// Start at 1st Pos
		posX = 1;
	}
	
	if (!width){
		// Width to render
		width = TERM_ScreenWidth;
	}

	// Width - respecting posX
	width = width - posX + 1;
	renderWidth = width;
	strcpy(strHLP, userHeaders[headerID].caption);
	renderLen = strlen(strHLP);
	
	if (userHeaders[headerID].printRealTime){
		width -= 20;	// 01.01.2023 09:09:21  (+ trailing space)
		renderRealTime = 1;
	}
	if (userHeaders[headerID].printRunTime){
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
				strncpy(strHLP, userHeaders[headerID].caption, width - 3);
				strHLP[width - 3] = '~';
				strHLP[width - 2] = '\0';
			}
			else{
				// too small to render 
				dontRender = 1;
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
		Locate(posX, posY);
	}
	else if (posX){
		LocateX(posX);
	}
	
	if (!justRefresh){
		// Style & Color
		SetColorStyle(&userColors[userHeaders[headerID].txtColor - 1], 1);
		SetTxtStyle(&userStyles[userHeaders[headerID].txtStyle - 1], 1);
		if (renderRealTime ^ renderRunTime) {
			// one time active - center is left of time...
			StrPrintCentered(strHLP, width);
			StrPrintSpaces(renderWidth - width);
		}
		else{
			// center is center
			if (renderRealTime && renderRunTime){
				// add three spaces, cause runtime is shorter than realtime
				strHLP[renderLen - 3] = ' '; strHLP[renderLen - 2] = ' '; strHLP[renderLen - 1] = ' '; strHLP[renderLen] = '\0';
			}			
			StrPrintCentered(strHLP, renderWidth);
		}
	}
	else{
		// we just refresh time(s)
		CursorRight(renderWidth);
	}

	if (renderRealTime || renderRunTime){
		// Set style & color of times
		SetColorStyle(&userColors[userHeaders[headerID].timeColor - 1], 1);
		SetTxtStyle(&userStyles[userHeaders[headerID].timeStyle - 1], 1);
	}
	
	// Do we print the Realtime (all time right alignment)
	if (renderRealTime){
		// 01.01.2023 00:00:00
		CursorLeft(19);
		printf("%s %s", gStrDate, gStrTime);
		CursorRight(1);
	}
	// Do we print the RunTime
	if (renderRunTime){
		// 00000d 00:00:00
		if (renderRealTime){
			// left alignment, cause RealTime already exist
			CursorLeft(renderWidth - 2);
		}
		else{
			// right alignment
			CursorLeft(15);
		}
		printf("%s", gStrRunTime);
	}	
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

void TUIrenderHorzMenu(int posX, int posY, int menuType, struct TuiMenusSTRUCT *menuDef, struct TuiDesktopsSTRUCT *deskDef, int justRefresh, int minX, int minY, int maxX, int maxY){

	int renderRealTime = 0;
	int renderRunTime = 0;
	int renderSmall = 0;	// just the key + 2 spaces
	int renderSelect = 0;	// "just the key" + selected key in full width 
	int renderLen = 0;		// full size len of menu positions 
	int dontRender = 0;		// even too small to render shortcuts

	int renderStyle = 0;	// 0 = full; 1 = small, but full selected; 2 = small

	struct TuiMenuPosSTRUCT *selectedMenu = NULL;

	int subXs = 0;  // subMenu X
	int subXw = 0;	// subMenu Width	
	
	if (!minX){
		minX = 1;
	}
	if (!maxX){
		maxX = TERM_ScreenWidth;
	}
	if (!minY){
		minY = (deskDef->header > 0) + (deskDef->topMenu > 0) + 1;
	}
	if (!maxY){
		maxY = TERM_ScreenHeight - (deskDef->bottomMenu > 0) - (deskDef->footer > 0);
	}	

	if (!posX){
		// Start at 1st Pos
		posX = minX;
	}
	if (!posY){
		if (menuType){
			// BottomMenu
			posY = maxY + 1;
		}
		else{
			// TopMenu
			posY = minY - 1;
		}
	}
	
	if (menuType){
		// BottomMenu
		menuType = 3;
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

	if (menuDef->printRealTime){
		renderLen += 20;	// 01.01.2023 09:09:21  (+ trailing space)
		renderSmall += 20;
		renderRealTime = 1;
	}
	if (menuDef->printRunTime){
		renderLen += 16;	// 00000d 00:00:00 (+ leading space))
		renderSmall += 16;
		renderRunTime = 1;
	}

	renderSelect += renderSmall;

	if (renderLen > renderWidth){
		// menu doesn't fit - f*ck
		if (renderRunTime){
			renderLen -= 16;	// 00000d 00:00:00 (+ leading space)
			renderRunTime = 0;
			if (renderLen > renderWidth){
				// still too small...
				if (renderRealTime){
					renderLen -= 20;	// 01.01.2023 09:09:21  (+ trailing space)
					renderRealTime = 0;
					if (renderLen > renderWidth){
						// we're finally f*cked
						renderStyle = 1;
					}
				}
				else{
					// we're finally f*cked
					renderStyle = 1;
				}
			}
		}
		else if (renderRealTime){
			renderLen -= 20;	// 01.01.2023 09:09:21  (+ trailing space)
			renderRealTime = 0;
			if (renderLen > renderWidth){
				// we're finally f*cked
				renderStyle = 1;
			}
		}
		else{
			// too long without time(s)
			renderStyle = 1;
		}

		if (renderStyle && renderSelect > renderSmall){
			// check if small menu, but full selected, fits...
			renderLen = renderSelect;
			renderRealTime = menuDef->printRealTime;
			renderRunTime = menuDef->printRunTime;
			if (renderLen > renderWidth){
				if (renderRunTime){
					renderLen -= 16;	// 00000d 00:00:00 (+ leading space)
					renderRunTime = 0;
					if (renderLen > renderWidth){
						// still too small...
						if (renderRealTime){
							renderLen -= 20;	// 01.01.2023 09:09:21  (+ trailing space)
							renderRealTime = 0;
							if (renderLen > renderWidth){
								// we're finally f*cked
								renderStyle = 2;
							}
						}
						else{
							// we're finally f*cked
							renderStyle = 2;
						}
					}
				}
				else if (renderRealTime){
					renderLen -= 20;	// 01.01.2023 09:09:21  (+ trailing space)
					renderRealTime = 0;
					if (renderLen > renderWidth){
						// we're finally f*cked
						renderStyle = 2;
					}
				}
				else{
					// too long without time(s)
					renderStyle = 2;
				}
			}
		}
		else if(renderStyle){
			// no key selected
			renderStyle = 2;
		}

		if (renderStyle == 2){
			// check if small menu, fits...
			renderLen = renderSmall;
			renderRealTime = menuDef->printRealTime;
			renderRunTime = menuDef->printRunTime;
			if (renderLen > renderWidth){
				if (renderRunTime){
					renderLen -= 16;	// 00000d 00:00:00 (+ leading space)
					renderRunTime = 0;
					if (renderLen > renderWidth){
						// still too small...
						if (renderRealTime){
							renderLen -= 20;	// 01.01.2023 09:09:21  (+ trailing space)
							renderRealTime = 0;
							if (renderLen > renderWidth){
								// we're finally f*cked
								dontRender = 1;
							}
						}
						else{
							// we're finally f*cked
							dontRender = 1;
						}
					}
				}
				else if (renderRealTime){
					renderLen -= 20;	// 01.01.2023 09:09:21  (+ trailing space)
					renderRealTime = 0;
					if (renderLen > renderWidth){
						// we're finally f*cked
						dontRender = 1;
					}
				}
				else{
					// too long without time(s)
					dontRender = 1;
				}
			}
		}
	}
	else{
		// full menu fits
	}

	if (!dontRender){
		// Where to render
		if (posX && posY){
			Locate(posX, posY);
		}
		else if (posX){
			LocateX(posX);
		}
		subXs = posX;

		if (!justRefresh){
			
			struct TuiMenuPosSTRUCT *menuPos = menuDef->pos1st;
			while (menuPos){
				if (renderStyle){
					// we can't fully render this menu
					menuPos->printSmall = 3;
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
					subXw = posX - subXs - 1;
				}
				menuPos = menuPos->nextPos;
			}
			
			// Fill line with right colored spaces
			SetColorStyle(&userColors[menuDef->txtColor], 1);
			SetTxtStyle(&userStyles[menuDef->txtStyle], 1);
			StrPrintSpaces(renderWidth - renderLen);
			
		}
		else{
			// we just refresh time(s)
			CursorRight(renderWidth);
		}

		if (renderRealTime || renderRunTime){
			// Set style & color of times
			SetColorStyle(&userColors[menuDef->timeColor], 1);
			SetTxtStyle(&userStyles[menuDef->timeStyle], 1);
		}
		
		// Do we print the Realtime (all time right alignment)
		if (renderRealTime){
			// 01.01.2023 00:00:00
			CursorLeft(19);
			printf("%s %s", gStrDate, gStrTime);
			CursorRight(1);
		}
		// Do we print the RunTime
		if (renderRunTime){
			// 00000d 00:00:00
			if (renderRealTime){
				// left of RealTime 
				CursorLeft(36);
			}
			else{
				// right alignment
				CursorLeft(15);
			}
			printf("%s", gStrRunTime);
		}	

		if (selectedMenu && selectedMenu->pos1st){
			if (menuType){
				// BottomMenu
				posY--;
			}
			else{
				// TopMenu
				posY++;
			}
			
			subXw++;
			if (TUIpreRenderSub(subXs, subXw, selectedMenu->pos1st, minX, maxX)){
				TUIrenderSub(subXs, posY, subXw, selectedMenu->pos1st, menuDef, 1, menuType, minX, minY, maxX, maxY);
			}
		}
	}
	else{
		ResFBU();
		SetFg16(fgRed);
		TxtBold(1);
		printf("!! Can't Render Horz-Menu !!\n");
		TxtBold(0);
		ResFBU();
	}
}
#define TUIrenderTopMenu(menuDef, deskDef, justRefresh, minX, minY, maxX, maxY) TUIrenderHorzMenu(0, 0, 0, menuDef, deskDef, justRefresh, minX, minY, maxX, maxY)
#define TUIrenderBottomMenu(menuDef, deskDef, justRefresh, minX, minY, maxX, maxY) TUIrenderHorzMenu(0, 0, 3, menuDef, deskDef, justRefresh, minX, minY, maxX, maxY)

int TUIrenderVertMenu(int posX, int posY, int menuType, struct TuiMenusSTRUCT *menuDef, struct TuiDesktopsSTRUCT *deskDef, int minX, int minY, int maxX, int maxY){
	
	if (!minX){
		minX = 1;
	}
	if (!maxX){
		maxX = TERM_ScreenWidth;
	}
	if (!minY){
		minY = (deskDef->header > 0) + (deskDef->topMenu > 0) + 1;
		minY++;	// Leading space-line
	}
	if (!maxY){
		maxY = TERM_ScreenHeight - (deskDef->bottomMenu > 0) - (deskDef->footer > 0);
		maxY--;	// Trailing space-line
	}	

	if (!posX){
		if (menuType){
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
	
	TUIclearSmallInverted(menuDef->pos1st);

	if (TUIpreRenderSub(posX, 3, menuDef->pos1st, minX, maxX)){
		
		// Leading Line
		int len = TUIgetSubLen(menuDef->pos1st, 0, 0);
		int lineX = posX;
		if (menuType){
			// RightMenu - fix X for leading and trailing line
			lineX = posX - len + 3;
		}
		Locate(lineX, posY);
		SetColorStyle(&userColors[menuDef->txtColor], 1);
		SetTxtStyle(&userStyles[menuDef->txtStyle], 1);		
		StrPrintSpaces(len);
		
		posY = TUIrenderSub(posX, posY + 1, 3, menuDef->pos1st, menuDef, 1, 0, minX, minY + 1, maxX, maxY - 1);

		// Trailing Line
		Locate(lineX, posY);
		SetColorStyle(&userColors[menuDef->txtColor], 1);
		SetTxtStyle(&userStyles[menuDef->txtStyle], 1);		
		StrPrintSpaces(len);

		return 1;

	}
	else{
		ResFBU();
		SetFg16(fgRed);
		TxtBold(1);
		printf("!! Can't Render Vert-Menu !!\n");
		TxtBold(0);
		ResFBU();
		return 0;
	}
}
#define TUIrenderLeftMenu(menuDef, deskDef, minX, minY, maxX, maxY) TUIrenderVertMenu(0, 0, 0, menuDef, deskDef, minX, minY, maxX, maxY)
#define TUIrenderRightMenu(menuDef, deskDef, minX, minY, maxX, maxY) TUIrenderVertMenu(0, 0, 1, menuDef, deskDef, minX, minY, maxX, maxY)

int TUIinitHeaders(char *strFile, struct TuiHeadersSTRUCT **userHeader){

	// User Header - Helper
	char strSearch[STR_SMALL_SIZE];
	char strHeaderCaption[STR_SMALL_SIZE];
	char strHLP[STR_SMALL_SIZE];

	// count of headers
	int headersCount = IniGetInt(strFile, "global.header.HeaderCount", 0);

	// memory to store all header
	*userHeader = (struct TuiHeadersSTRUCT*)malloc(headersCount * sizeof(struct TuiHeadersSTRUCT));

	for (int i = 0; i < headersCount; i++){
		int j = i + 1;	// User index in file is 1-based...
		// Caption
		sprintf(strSearch, "global.header.%d.Caption", j);
		sprintf(strHLP, "Header%d", i + 1);
		IniGetStr(strFile, strSearch, strHLP, strHeaderCaption);
		(*userHeader)[i].caption = IniStrToMem(strHeaderCaption, 0);
		// TextColor 
		sprintf(strSearch, "global.header.%d.TextColor", j);
		(*userHeader)[i].txtColor = IniGetInt(strFile, strSearch, 0);
		// TextStyle 
		sprintf(strSearch, "global.header.%d.TextStyle", j);
		(*userHeader)[i].txtStyle = IniGetInt(strFile, strSearch, 0);
		// TimeColor 
		sprintf(strSearch, "global.header.%d.TimeColor", j);
		(*userHeader)[i].timeColor = IniGetInt(strFile, strSearch, 0);
		// TimeStyle 
		sprintf(strSearch, "global.header.%d.TimeStyle", j);
		(*userHeader)[i].timeStyle = IniGetInt(strFile, strSearch, 0);
		// PrintRunTime 
		sprintf(strSearch, "global.header.%d.PrintRunTime", j);
		(*userHeader)[i].printRunTime = IniGetBool(strFile, strSearch, 0);
		// PrintRealTime 
		sprintf(strSearch, "global.header.%d.PrintRealTime", j);
		(*userHeader)[i].printRealTime = IniGetBool(strFile, strSearch, 0);

	}

	printf("\n");

	return headersCount;
	
}

struct TuiMenuPosSTRUCT *TUIaddMenuPos(const char *strFile, char *strPath, struct TuiMenusSTRUCT *definition, int positions, int testMe, int isSub){

	static struct TuiMenuPosSTRUCT *menuPos = NULL;
	//static int *nextPos = NULL;
	//static int *prevPos = NULL;
	//static int *pos1st = NULL;
	
	static int cnt = 0;

	// Helper
	char strSearch[STR_SMALL_SIZE];
	char strHLP[STR_SMALL_SIZE];
	char strPos1[STR_SMALL_SIZE];

	int posReturn = cnt;	// save for return
	int lastPos = 0;

	int j = 0;

	for (int i = 0; i < positions; i++){
		
		j = i + 1;	// User to Memory indexing

		// Add mem for new position
		cnt++;
		int pos1 = cnt - 1;
//printf("pre realloc...\n");
		menuPos = realloc(menuPos, cnt * sizeof(struct TuiMenuPosSTRUCT));
//printf("after realloc...\n");
		sprintf(strSearch, "%s%d.Enabled", strPath, j);
		menuPos[pos1].enabled = IniGetBool(strFile, strSearch, 1);
		sprintf(strSearch, "%s%d.isOption", strPath, j);
		menuPos[pos1].isOption = IniGetBool(strFile, strSearch, 0);
		sprintf(strSearch, "%s%d.isCheck", strPath, j);
		menuPos[pos1].isCheck = IniGetBool(strFile, strSearch, 0);
		sprintf(strSearch, "%s%d.isActivated", strPath, j);
		menuPos[pos1].activated = IniGetBool(strFile, strSearch, 0);
		sprintf(strSearch, "%s%d.Positions", strPath, j);
		menuPos[pos1].posCnt = IniGetInt(strFile, strSearch, 0);

		sprintf(strSearch, "%s%d.Text", strPath, j);
		sprintf(strHLP, "%s%d", strPath, j);
		IniGetStr(strFile, strSearch, strHLP, strPos1);
		
		menuPos[pos1].isSubMenu = isSub;

		// Search key and remove brackets
		menuPos[pos1].keyCode = 0;

		int k;
		k = 0;
		int keyCodeFound = 0;
		for (int l = 0; l < strlen(strPos1); l++){
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
			menuPos[pos1].isSpacer = 1;
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
		
		sprintf(strPos1, "%s%s ", strPos1, strHLP);

//printf("pre strToMem...%s...\n", strPos1);
		menuPos[pos1].caption = IniStrToMem(strPos1, 0);
//printf("%d: %d: %s\n", pos1, &menuPos[pos1], menuPos[pos1].caption);
		// already known stuff
		menuPos[pos1].definition = definition;
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
	for (size_t i = 0; i < cnt; i++){
		// restore valid pointers from IDs
		if (menuPos[i].prevID || (j && (i == j))){
			menuPos[i].prevPos = &menuPos[menuPos[i].prevID];
		}
		if (menuPos[i].nextID){
			menuPos[i].nextPos = &menuPos[menuPos[i].nextID];
			if (!i){
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
//printf("after Restore...\n");
	if (testMe){
		for (int i = 0; i < cnt; i++){
			printf("pos: %d @: %p pos1st: %p parentPos: %p nextPos: %p prevPos: %p\n", i, &menuPos[i], menuPos[i].pos1st, menuPos[i].parentPos, menuPos[i].nextPos, menuPos[i].prevPos);
			//printf("pos: %d @: %s pos1st: %s nextPos: %s prevPos: %s\n\n", i, menuPos[i].caption, menuPos[i].pos1st->caption, menuPos[i].nextPos->caption, menuPos[i].prevPos->caption);
			printf("pos: %d @: %s pos1st: ", i, menuPos[i].caption);
			
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

// "global.TopMenu."
int TUIinitMenuDefs(char *strFile, char *strPath, struct TuiMenusSTRUCT **menu){

	// Helper
	char strSearch[STR_SMALL_SIZE];

	sprintf(strSearch, "%s.%s", strPath, "Count");
	int menusCnt = IniGetInt(strFile, strSearch, 0);

//printf("menusCnt: %d\n", menusCnt);

	*menu = (struct TuiMenusSTRUCT*)malloc(menusCnt * sizeof(struct TuiMenusSTRUCT));

//printf("after malloc...\n");

	// Menu Definition Values
	for (int i = 0; i < menusCnt; i++){
		sprintf(strSearch, "%s.%d.TextColor", strPath, i + 1);
		menu[i]->txtColor = IniGetInt(strFile, strSearch, 0);
//printf("1st\n");
		sprintf(strSearch, "%s.%d.TextStyle", strPath, i + 1);
		(*menu)[i].txtStyle = IniGetInt(strFile, strSearch, 0);
//printf("2nd\n");
		sprintf(strSearch, "%s.%d.SelectColor", strPath, i + 1);
		(*menu)[i].selectColor = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.SelectStyle", strPath, i + 1);
		(*menu)[i].selectStyle = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.SelectKeyColor", strPath, i + 1);
		(*menu)[i].selectKeyColor = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.SelectKeyStyle", strPath, i + 1);
		(*menu)[i].selectKeyStyle = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.DisabledColor", strPath, i + 1);
		(*menu)[i].disabledColor = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.DisabledStyle", strPath, i + 1);
		(*menu)[i].disabledStyle = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.KeyColor", strPath, i + 1);
		(*menu)[i].keyColor = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.KeyStyle", strPath, i + 1);
		(*menu)[i].keyStyle = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.TimeColor", strPath, i + 1);
		(*menu)[i].timeColor = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.TimeStyle", strPath, i + 1);
		(*menu)[i].timeStyle = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.PrintRunTime", strPath, i + 1);
		(*menu)[i].printRunTime = IniGetBool(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.PrintRealTime", strPath, i + 1);
		(*menu)[i].printRealTime = IniGetBool(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.Positions", strPath, i + 1);
		(*menu)[i].posCnt = IniGetInt(strFile, strSearch, 0);

		(*menu)[i].renderLen = 0;
//printf("last\n");
		// Add positions
		sprintf(strSearch, "%s.%d.", strPath, i + 1);
		(*menu)[i].pos1st = TUIaddMenuPos(strFile, strSearch, menu[i], (*menu)[i].posCnt, 1, 0);	
//printf("after menuPos...\n");
//printf("%s : %d\n", (*menu)[i].pos1st->caption, (*menu)[i].posCnt);	
//printf("%s\n", menu[i]->pos1st->caption);	

	}

	return menusCnt;
	
}

int TUIinitDesktops(char *strFile, struct TuiDesktopsSTRUCT **desktop){
	
	// Helper
	char strSearch[STR_SMALL_SIZE];

	int desksCnt = IniGetInt(strFile, "global.desktops.Count", 0);

	*desktop = (struct TuiDesktopsSTRUCT*)malloc(desksCnt * sizeof(struct TuiDesktopsSTRUCT));

	// Menu Definition Values
	for (int i = 0; i < desksCnt; i++){
		int j = i + 1; // User index in file is 1-based...

		// Header
		sprintf(strSearch, "global.desktops.%d.Header", j);
		desktop[i]->header = IniGetInt(strFile, strSearch, 0);
		// TopMenu
		sprintf(strSearch, "global.desktops.%d.TopMenu", j);
		desktop[i]->topMenu = IniGetInt(strFile, strSearch, 0);
		// BottomMenu
		sprintf(strSearch, "global.desktops.%d.BottomMenu", j);
		desktop[i]->bottomMenu = IniGetInt(strFile, strSearch, 0);
		// LeftMenu
		sprintf(strSearch, "global.desktops.%d.LeftMenu", j);
		desktop[i]->leftMenu = IniGetInt(strFile, strSearch, 0);
		// RightMenu
		sprintf(strSearch, "global.desktops.%d.RightMenu", j);
		desktop[i]->rightMenu = IniGetInt(strFile, strSearch, 0);
		// Header
		sprintf(strSearch, "global.desktops.%d.Footer", j);
		desktop[i]->footer = IniGetInt(strFile, strSearch, 0);
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


