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
	int disabledColor;
	int disabledStyle;
	int keyColor;
	int keyStyle;
	int timeColor;
	int timeStyle;
	int posCnt;	
	int renderLen;					// count of sub-positions
	int pos1st;		
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
	int posCnt;						// count of sub-positions
	int nextPos;					// on same level
	int prevPos;					// on same level
	int pos1st;						// first on sub level
	int selected :1;				// if position is selected
	int isOption :1;
	int isCheck :1;
	int	activated :1;				// if position is a check or option...
	int isSpacer :1;
};
struct TuiMenuPosSTRUCT *userMenuPos = NULL;
struct TuiDesktopsSTRUCT{
	int header;
	int topMenu;
	int bottomMenu;
	int leftMenu;
	int rightMenu;
};
struct TuiDesktopsSTRUCT *userDesktopDefs = NULL;

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

void TUIrenderSubMenu(int posX, int posY, int menuType, int invert, struct TuiMenusSTRUCT *menuDef, int menuPosID, struct TuiDesktopsSTRUCT *deskDef){

	// if posX and invert is given... posX must have right alignment! (after having width we move it)

	int selectedMenu = 0;
	int selectedPos = 0;
	
	int renderSmall = 0;
	int dontRender = 0;

	int renderHeight = 0;
	int renderWidth = 0;
	
	int posCnt = 0;

	struct TuiMenuPosSTRUCT *menuPos = &userMenuPos[menuPosID];

	struct TuiMenuPosSTRUCT *menuPos1st = menuPos;

	// set eventually standard positions...
	switch (menuType){
	case 1:
		// LeftMenu
		break;
	case 2:
		// RightMenu
		break;
	case 3:
		// BottomMenu
		break;
	case 4:
		// TopMenu next levels...
	default:
		// TopMenu
		if (posX && posY){
			// Locate(posX, posY);
		}
		else if (posX){
			// LocateX(posX);
			posY = 3;		
		}
		else if (posY){
			posX = 1;
		}
		else{
			posY = 3;
			posX = 1;
		}
		break;
	}

	// Get Size, Count, Selected, ... once... ;-)
	while (menuPos){
		int actWidth = strlen(menuPos->caption);
		posCnt++;
		renderWidth = (renderWidth > actWidth) ? renderWidth : actWidth;
		if (!selectedPos && menuPos->enabled && menuPos->selected){
			// find eventually selected pos...
			selectedPos = posCnt;
			selectedMenu = menuPosID;
		}
		menuPos = &userMenuPos[menuPos->nextPos];
		menuPosID++;
	}
	renderHeight = posCnt;
	menuPos = menuPos1st;

	// copy original positions & dimensions
	int orgHeight = renderHeight;
	int orgWidth = renderWidth;
	int orgPosY = posY; int orgPosX = posX;

	// Helper if menu doesn't fit as supposed
	int offsetPosY = 0;

	// Does it fit in Y as supposed
	switch (menuType){
	case 1:
		// LeftMenu
		break;
	case 2:
		// RightMenu
		break;
	case 3:
		// BottomMenu
		break;
	case 4:
		// TopMenu next levels...
	default:
		// TopMenu
		if ((renderHeight + posY) > TERM_ScreenHeight){
			// too high to print all positions - shift posY below menuLine
			posY = (deskDef->header > 0) + 1;
			if ((renderHeight + posY) > TERM_ScreenHeight){
				// still to high
				if (selectedPos > TERM_ScreenHeight - posY){
					// invisible selectedPos - shift offset
					offsetPosY = selectedPos - (TERM_ScreenHeight - posY);
				}
			}
			else{
				// fits - shift back as much as possible
				posY += TERM_ScreenHeight - (renderHeight + posY);
			}
		}		
	}

	if (invert){
		// correct posX
		posX -= renderWidth;
	}
	
	// Does it fit in X as supposed
	switch (menuType){
	case 1:
		// LeftMenu
		break;
	case 2:
		// RightMenu
		break;
	case 3:
		// BottomMenu
		break;
	case 4:
		// TopMenu next levels...
		break;
	default:
		// TopMenu - 1st level
		if (invert){
			// Print to left
			if (posX < 1){
				// that's bad - we've just the reduced size as option
				renderSmall = 1;
				renderWidth = 0;
				// Get reduced size
				while (menuPos){
					if (!renderWidth){
						// Is at least one key + 2 spaces
						renderWidth = 3;
					}
					if (menuPos->isCheck || menuPos->isOption){
						// containing option or check - ' [ ] A '
						renderWidth = 7;
						renderSmall = 2;
						break;
					}
					menuPos = &userMenuPos[menuPos->nextPos];
				}
				menuPos = menuPos1st;
				posX = orgPosX - renderWidth;
				if (posX < 1){
					// Damn, this area is awful small... we can't render this
					dontRender = 1;
				}
			}
		}
		else{
			// Print to right
			if ((TERM_ScreenWidth - posX - renderWidth) < 0){
				// too width - try to invert direction
				posX -= renderWidth;
				invert = 1;
				if (posX < 1){
					// we're screwed - maybe single-key menu is working
					posX = orgPosX;
					invert = 0;
					renderSmall = 1;
					renderWidth = 0;
					// Get reduced size
					while (menuPos){
						if (!renderWidth){
							// Is at least one key + 2 spaces
							renderWidth = 3;
						}
						if (menuPos->isCheck || menuPos->isOption){
							// containing option or check - ' [ ] A '
							renderWidth = 7;
							renderSmall = 2;
							break;
						}
						menuPos = &userMenuPos[menuPos->nextPos];
					}
					menuPos = menuPos1st;
					if ((TERM_ScreenWidth - posX - renderWidth) < 0){
						// Uhhh, even the short version doesn't fit...
						posX -= renderWidth;
						invert = 1;
						if (posX < 1){
							// Damn, this area is awful small... we can't render this
							dontRender = 1;
						}
					}
				}
				else{
					// we already changed the alignment
				}
			}
		}
		break;
	}

	Locate(posX, posY);
	for (size_t i = 0; i < offsetPosY; i++){
		// trash unused positions
		menuPos = &userMenuPos[menuPos->nextPos];
	}
	while (menuPos){
		
		SetColorStyle(&userColors[menuDef->txtColor - 1], 1);
		SetTxtStyle(&userStyles[menuDef->txtStyle - 1], 1);

		if (renderSmall){
			// render just the keys (+ eventually check/option brackets)
			printf(" ");	// 1st space...
			if (menuPos->selected && menuPos->enabled){
				// enabled & selected
				SetColorStyle(&userColors[menuDef->selectColor - 1], 1);
				SetTxtStyle(&userStyles[menuDef->selectStyle - 1], 1);
			}
			else if (menuPos->enabled){
				// enabled
			}
			else{
				// disabled
				SetColorStyle(&userColors[menuDef->disabledColor - 1], 1);
				SetTxtStyle(&userStyles[menuDef->disabledStyle - 1], 1);
			}
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
			else if (renderSmall == 2){
				StrPrintSpaces(4);
			}
			else{
				// we're fine
			}
			
			if (menuPos->selected && menuPos->enabled){
			}
			else if (menuPos->enabled){
				// Key in keyColor and keyStyle
				SetColorStyle(&userColors[menuDef->keyColor - 1], 1);
				SetTxtStyle(&userStyles[menuDef->keyStyle - 1], 1);
			}
			else{
			}
			printf("%c", menuPos->caption[menuPos->keyCode]);

			SetColorStyle(&userColors[menuDef->txtColor - 1], 1);
			SetTxtStyle(&userStyles[menuDef->txtStyle - 1], 1);					
			printf(" ");
			
			CursorDown(1);
			if (renderSmall == 2){
				// with option/check
				CursorLeft(7);
			}
			else{
				CursorLeft(3);
			}
			
			menuPos = &userMenuPos[menuPos->nextPos];
		}

		else{
			// render full line
			for (size_t i = 0; i < strlen(menuPos->caption); i++){
				if (menuPos->enabled && menuPos->selected){
					// enabled - selected
					SetColorStyle(&userColors[menuDef->selectColor - 1], 1);
					SetTxtStyle(&userStyles[menuDef->selectStyle - 1], 1);
				}
				else if (menuPos->enabled){
					// enabled
					if (i && (i == menuPos->keyCode)){
						// is key
						SetColorStyle(&userColors[menuDef->keyColor - 1], 1);
						SetTxtStyle(&userStyles[menuDef->keyStyle - 1], 1);
					}
					else{
						SetColorStyle(&userColors[menuDef->txtColor - 1], 1);
						SetTxtStyle(&userStyles[menuDef->txtStyle - 1], 1);
					}
				}
				else{
					// disabled
					SetColorStyle(&userColors[menuDef->disabledColor - 1], 1);
					SetTxtStyle(&userStyles[menuDef->disabledStyle - 1], 1);
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
					printf("%c", menuPos->caption[i]);
				}
			}
			SetColorStyle(&userColors[menuDef->txtColor - 1], 1);
			SetTxtStyle(&userStyles[menuDef->txtStyle - 1], 1);

			StrPrintSpaces(orgWidth - strlen(menuPos->caption));
			CursorDown(1);
			CursorLeft(orgWidth);
			//printf(" ");
			menuPos = &userMenuPos[menuPos->nextPos];
			
		}
	}

	if (selectedMenu){
		// there is another subMenu to render...
		
		// set X & Y
		posY = orgPosY + selectedPos - 1 - offsetPosY - (orgPosY - posY);
		if (invert){
			posX--;
		}
		else{
			posX += renderWidth;
		}
		
		TUIrenderSubMenu(posX, posY, 4, invert, menuDef, selectedMenu, deskDef);
		
	}
		
}

void TUIrenderTopMenu(int posX, int posY, int width, struct TuiMenusSTRUCT *menuDef, struct TuiDesktopsSTRUCT *deskDef, int justRefresh){

	char strHlp[STR_SMALL_SIZE];
	char strLine[STR_MID_SIZE];

	int renderRealTime = 0;
	int renderRunTime = 0;
	int renderSmall = 0;	// width too small to render full MenuText 
	int renderLen = 0;
	int dontRender = 0;		// even too small to render shortcuts
	int renderWidth = 0;

	struct TuiMenuPosSTRUCT *selectedMenu = NULL;
	int selectedMenuPos = 0;
	int selectedMenuPosHlp = 0;

	if (!posX){
		// Start at 1st Pos
		posX = 1;
	}
	
	if (!width){
		// Width to render
		width = TERM_ScreenWidth;
	}

	// Width - respecting posX
	width = width - posX + 2;
	renderWidth = width;

printf("RenderWidth: %d menuDef->pos1st: %D\n", renderWidth, menuDef->pos1st);

	if (!menuDef->renderLen){
		// 1st call
		// Calculate len of top-level MenuLine
		struct TuiMenuPosSTRUCT *menuPos = &userMenuPos[menuDef->pos1st];
		while (menuPos != NULL){
			menuDef->renderLen += strlen(menuPos->caption);
			menuPos = &userMenuPos[menuPos->nextPos];
		}
		menuDef->renderLen += 1;	// trailing Space
	}
	renderLen = menuDef->renderLen;

printf("RenderLen: %dßn", renderLen);

	if (menuDef->printRealTime){
		width -= 20;	// 01.01.2023 09:09:21  (+ trailing space)
		renderRealTime = 1;
	}
	if (menuDef->printRunTime){
		width -= 16;	// 00000d 00:00:00 (+ leading space)
		renderRunTime = 1;
	}
	
	if (renderLen > width){
		// menu doesn't fit - f*ck
		if (renderRunTime){
			width += 16;	// 00000d 00:00:00 (+ leading space)
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
			// check if small menu fits...
			renderLen = menuDef->posCnt * 3 + 2;
			if (menuDef->printRealTime && renderLen < width){
				// maybe time fits in short-key-menu-style again
				width -= 20;	// 01.01.2023 09:09:21  (+ trailing space)
				renderRealTime = 1;
				if (renderLen > width){
					// no...
					width += 20;	// 01.01.2023 09:09:21  (+ trailing space)
					renderRealTime = 0;
				}
			}
			if (menuDef->printRunTime && renderLen < width){
				// maybe time fits in short-key-menu-style again
				width -= 16;	// 01.01.2023 09:09:21  (+ trailing space)
				renderRunTime = 1;
				if (renderLen > width){
					// no...
					width += 16;	// 01.01.2023 09:09:21  (+ trailing space)
					renderRunTime = 0;
				}
			}
			if (renderLen > width){
				// too small to render even single-key menu...
				dontRender = 1;
			}
		}
		else{
			// full menu fits
		}
	}
	else{
		// full menu fits
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
		SetColorStyle(&userColors[menuDef->txtColor - 1], 1);
		SetTxtStyle(&userStyles[menuDef->txtStyle - 1], 1);
		

		if (renderSmall){
			// render just the keys
			printf(" ");	// 1st space...
			struct TuiMenuPosSTRUCT *menuPos = &userMenuPos[menuDef->pos1st];
			while (menuPos){
				selectedMenuPosHlp++;
				if (menuPos->selected && menuPos->enabled){
					/* code */
					SetColorStyle(&userColors[menuDef->selectColor - 1], 1);
					SetTxtStyle(&userStyles[menuDef->selectStyle - 1], 1);
					selectedMenuPos = selectedMenuPosHlp;
					selectedMenu = menuPos;
				}
				else if (menuPos->enabled){
					SetColorStyle(&userColors[menuDef->keyColor - 1], 1);
					SetTxtStyle(&userStyles[menuDef->keyStyle - 1], 1);
				}
				else{
					SetColorStyle(&userColors[menuDef->disabledColor - 1], 1);
					SetTxtStyle(&userStyles[menuDef->disabledStyle - 1], 1);
				}			
				printf("%c", menuPos->caption[menuPos->keyCode]);
				SetColorStyle(&userColors[menuDef->txtColor - 1], 1);
				SetTxtStyle(&userStyles[menuDef->txtStyle - 1], 1);					
				printf("  ");
				menuPos = &userMenuPos[menuPos->nextPos];
			}
			//printf(" ");
		}
		else{
			// render full line
			struct TuiMenuPosSTRUCT *menuPos = &userMenuPos[menuDef->pos1st];
			while (menuPos){
				selectedMenuPosHlp++;
				if (menuPos->selected && menuPos->enabled){
					selectedMenuPos = selectedMenuPosHlp;
					selectedMenu = menuPos;
				}
				for (size_t i = 0; i < strlen(menuPos->caption); i++){
					// int j = 0;
					if (menuPos->enabled && menuPos->selected){
						/* code */
						SetColorStyle(&userColors[menuDef->selectColor - 1], 1);
						SetTxtStyle(&userStyles[menuDef->selectStyle - 1], 1);
					}
					else if (menuPos->enabled){
						if (i == menuPos->keyCode){
							SetColorStyle(&userColors[menuDef->keyColor - 1], 1);
							SetTxtStyle(&userStyles[menuDef->keyStyle - 1], 1);
						}
						else{
							SetColorStyle(&userColors[menuDef->txtColor - 1], 1);
							SetTxtStyle(&userStyles[menuDef->txtStyle - 1], 1);
						}
					}
					else{
						SetColorStyle(&userColors[menuDef->disabledColor - 1], 1);
						SetTxtStyle(&userStyles[menuDef->disabledStyle - 1], 1);
					}						
					printf("%c", menuPos->caption[i]);
				}
				SetColorStyle(&userColors[menuDef->txtColor - 1], 1);
				SetTxtStyle(&userStyles[menuDef->txtStyle - 1], 1);
				//printf(" ");
				menuPos = &userMenuPos[menuPos->nextPos];
			}
		}

		StrPrintSpaces(renderWidth - renderLen);

	}
	else{
		// we just refresh time(s)
		CursorRight(renderWidth);
	}

	if (renderRealTime || renderRunTime){
		// Set style & color of times
		SetColorStyle(&userColors[menuDef->timeColor - 1], 1);
		SetTxtStyle(&userStyles[menuDef->timeStyle - 1], 1);
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

	// if (selectedMenuPos && menuDef->pos1st){
	if (selectedMenuPos){
		// render selected Submenu
		struct TuiMenuPosSTRUCT *menuPos = &userMenuPos[menuDef->pos1st];
		
		// Set cursor one row below start of selected menu
		CursorLeft(renderWidth);
		CursorDown(1);
		int x = 0;
		while (--selectedMenuPos){
			x += strlen(menuPos->caption) + 2;
			menuPos = &userMenuPos[menuPos->nextPos];			
		}
		CursorRight(x);
		// Render SubMenu
		TUIrenderSubMenu(++x, 3, 0, 0, menuDef, selectedMenu->pos1st, deskDef);
	}
}

int TUIinitHeaders(char *strFile){

	// User Header - Helper
	char strSearch[STR_SMALL_SIZE];
	char strHeaderCaption[STR_SMALL_SIZE];
	char strHLP[STR_SMALL_SIZE];

	// count of headers
	int headersCount = IniGetInt(strFile, "global.header.HeaderCount", 0);

	// memory to store all header
	userHeaders = malloc(headersCount * sizeof(struct TuiHeadersSTRUCT));

	for (int i = 0; i < headersCount; i++){
		int j = i + 1;	// User index in file is 1-based...
		// Caption
		sprintf(strSearch, "global.header.%d.Caption", j);
		sprintf(strHLP, "Header%d", i + 1);
		IniGetStr(strFile, strSearch, strHLP, strHeaderCaption);
		userHeaders[i].caption = IniStrToMem(strHeaderCaption, 0);
		// TextColor 
		sprintf(strSearch, "global.header.%d.TextColor", j);
		userHeaders[i].txtColor = IniGetInt(strFile, strSearch, 0);
		// TextStyle 
		sprintf(strSearch, "global.header.%d.TextStyle", j);
		userHeaders[i].txtStyle = IniGetInt(strFile, strSearch, 0);
		// TimeColor 
		sprintf(strSearch, "global.header.%d.TimeColor", j);
		userHeaders[i].timeColor = IniGetInt(strFile, strSearch, 0);
		// TimeStyle 
		sprintf(strSearch, "global.header.%d.TimeStyle", j);
		userHeaders[i].timeStyle = IniGetInt(strFile, strSearch, 0);
		// PrintRunTime 
		sprintf(strSearch, "global.header.%d.PrintRunTime", j);
		userHeaders[i].printRunTime = IniGetBool(strFile, strSearch, 0);
		// PrintRealTime 
		sprintf(strSearch, "global.header.%d.PrintRealTime", j);
		userHeaders[i].printRealTime = IniGetBool(strFile, strSearch, 0);

	}
	printf("\n");

	return headersCount;
	
}

int TUIaddMenuPos(const char *strFile, char *strPath, struct TuiMenusSTRUCT *definition, int positions){

	//static struct TuiMenuPosSTRUCT *menuPos = NULL;
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

	for (int i = 0; i < positions; i++){
		
		int j = i + 1;	// User to Memory indexing

		// Add mem for new position
		cnt++;
		int pos1 = cnt - 1;
		userMenuPos = realloc(userMenuPos, cnt * sizeof(struct TuiMenuPosSTRUCT));
		// nextPos = realloc(nextPos, (cnt + 1) * sizeof(int)); // +1-indexed to get 0 for NULL
		// prevPos = realloc(prevPos, (cnt + 1) * sizeof(int)); // +1-indexed to get 0 for NULL
		// pos1st = realloc(pos1st, (cnt + 1) * sizeof(int)); // +1-indexed to get 0 for NULL

		sprintf(strSearch, "%s%d.Enabled", strPath, j);
		userMenuPos[pos1].enabled = IniGetBool(strFile, strSearch, 1);
		sprintf(strSearch, "%s%d.isOption", strPath, j);
		userMenuPos[pos1].isOption = IniGetBool(strFile, strSearch, 0);
		sprintf(strSearch, "%s%d.isCheck", strPath, j);
		userMenuPos[pos1].isCheck = IniGetBool(strFile, strSearch, 0);
		sprintf(strSearch, "%s%d.isActivated", strPath, j);
		userMenuPos[pos1].activated = IniGetBool(strFile, strSearch, 0);
		sprintf(strSearch, "%s%d.Positions", strPath, j);
		userMenuPos[pos1].posCnt = IniGetInt(strFile, strSearch, 0);

		sprintf(strSearch, "%s%d.Text", strPath, j);
		sprintf(strHLP, "%s%d", strPath, j);
		IniGetStr(strFile, strSearch, strHLP, strPos1);
		
		// Search key and remove brackets
		userMenuPos[pos1].keyCode = 0;

		int k;
		k = 0;
		int keyCodeFound = 0;
		for (int l = 0; l < strlen(strPos1); l++){
			if (strPos1[l] == '(' && !keyCodeFound){
				keyCodeFound = 1;
				userMenuPos[pos1].keyCode = k;
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
			userMenuPos[pos1].isSpacer = 1;
		}
		else{
			userMenuPos[pos1].isSpacer = 0;
			// increase keyPos (leading space)
			userMenuPos[pos1].keyCode++;
		}
		
		// Build leading part
		if (userMenuPos[pos1].isOption){
			// ( ) / (x)
			if (userMenuPos[pos1].activated){
				strcpy(strPos1, " (x) ");
			}
			else{
				strcpy(strPos1, " ( ) ");
			}
			userMenuPos[pos1].keyCode += 4;
		}
		else if (userMenuPos[pos1].isCheck){
			// [ ] / [x]
			if (userMenuPos[pos1].activated){
				strcpy(strPos1, " [x] ");
			}
			else{
				strcpy(strPos1, " [ ] ");
			}
			userMenuPos[pos1].keyCode += 4;
		}
		else{
			// just leading and trailing spaces
			strcpy(strPos1, " ");
		}
		
		sprintf(strPos1, "%s%s ", strPos1, strHLP);

		userMenuPos[pos1].caption = IniStrToMem(strPos1, 0);
		printf("%d: %s\n", pos1, userMenuPos[pos1].caption);
		// already known stuff
		userMenuPos[pos1].definition = definition;
		// stuff to reset
		userMenuPos[pos1].nextPos = 0;
		userMenuPos[pos1].prevPos = 0;
		userMenuPos[pos1].pos1st = 0;
		userMenuPos[pos1].selected = 0;
		// nextPos[pos1] = 0;
		// prevPos[pos1] = 0;
		// pos1st[pos1] = 0;

		userMenuPos[pos1].prevPos = lastPos;
		userMenuPos[lastPos].nextPos = pos1;

		// if not 1st pos, we have to set...
		if (i){
			// save IDs instead of pointers while recursion (realloc moves pointer)
			// nextPos[lastPos + 1] = pos1;
			// prevPos[pos1 + 1] = lastPos;
		}
		lastPos = pos1;
		
		if (userMenuPos[pos1].posCnt){
			sprintf(strSearch, "%s%d.", strPath, j);
			// pos1st[pos1 + 1] = pos1 + 1;
			userMenuPos[pos1].pos1st = TUIaddMenuPos(strFile, strSearch, definition, userMenuPos[pos1].posCnt);			
		}

	}
	
	/*
	for (size_t i = 0; i < cnt; i++){
		// restore valid pointers from IDs
		// ID = 0 // substitute for NULL
		if (nextPos[i + 1])	{
			// ID = 0 // substitute for NULL
			menuPos[i].nextPos = &menuPos[nextPos[i + 1]];
		}		
		//if (prevPos[i + 1]){
			menuPos[i].prevPos = &menuPos[prevPos[i + 1]];
		//}
		//if (pos1st[i + 1]){
			menuPos[i].pos1st = &menuPos[pos1st[i + 1]];
		//}
	}
	*/

	printf("\n%d\n\n", posReturn);
	return posReturn;
	
}

// "global.TopMenu."
int TUIinitMenuDefs(char *strFile, char *strPath, struct TuiMenusSTRUCT *menu){

	// Helper
	char strSearch[STR_SMALL_SIZE];

	sprintf(strSearch, "%s.%s", strPath, "Count");
	int menusCnt = IniGetInt(strFile, strSearch, 0);

	menu = malloc(menusCnt * sizeof(struct TuiMenusSTRUCT));

	// Menu Definition Values
	for (int i = 0; i < menusCnt; i++){
		sprintf(strSearch, "%s.%d.TextColor", strPath, i + 1);
		menu[i].txtColor = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.TextStyle", strPath, i + 1);
		menu[i].txtStyle = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.SelectColor", strPath, i + 1);
		menu[i].selectColor = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.SelectStyle", strPath, i + 1);
		menu[i].selectStyle = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.DisabledColor", strPath, i + 1);
		menu[i].disabledColor = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.DisabledStyle", strPath, i + 1);
		menu[i].disabledStyle = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.KeyColor", strPath, i + 1);
		menu[i].keyColor = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.KeyStyle", strPath, i + 1);
		menu[i].keyStyle = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.TimeColor", strPath, i + 1);
		menu[i].timeColor = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.TimeStyle", strPath, i + 1);
		menu[i].timeStyle = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.PrintRunTime", strPath, i + 1);
		menu[i].printRunTime = IniGetBool(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.PrintRealTime", strPath, i + 1);
		menu[i].printRealTime = IniGetBool(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.Positions", strPath, i + 1);
		menu[i].posCnt = IniGetInt(strFile, strSearch, 0);

		menu[i].renderLen = 0;

		// Add positions
		sprintf(strSearch, "%s.%d.", strPath, i + 1);
		menu[i].pos1st = TUIaddMenuPos(strFile, strSearch, &menu[i], menu[i].posCnt);		

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


