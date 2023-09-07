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
	int posCnt;	
	int renderLen;					// count of sub-positions
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
	int selected :1;			// if position is selected
	int isOption :1;
	int isCheck :1;
	int	activated :1;			// if position is a check or option...
}TuiMenuPosSTRUCT;

struct TuiDesktopDefSTRUCT{
	int header;
	int topMenu;
	int bottomMenu;
	int leftMenu;
	int rightMenu;
};

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

void TUIrenderSubMenu(int posX, int posY, int menuType, int invert, struct TuiMenuDefSTRUCT *menuDef, struct TuiMenuPosSTRUCT *menuPos, struct TuiDesktopDefSTRUCT *deskDef){

	int selectedMenuPos = 0;
	int selectedMenuPosHlp = 0;

	int renderSmall = 0;
	int dontRender = 0;

	int renderHeight = 0;
	int renderWidth = 0;
	int renderTabs = 1;

	int posCnt = 0;

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

	int selectedPos = 0;

	// Get Size, Count, Selected, ... once... ;-)
	while (menuPos){
		int actWidth = strlen(menuPos->caption);
		posCnt++;
		renderWidth = (renderWidth > actWidth) ? renderWidth : actWidth;
		if (!selectedPos && menuPos->enabled && menuPos->selected){
			// find eventually selected pos...
			selectedPos = posCnt;
		}
		menuPos = menuPos->nextPos;
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
					// invisible selected Pos - shift offset
					offsetPosY = selectedPos - (TERM_ScreenHeight - posY);
				}
			}
			else{
				// fits - shift back as much as possible
				posY += TERM_ScreenHeight - (renderHeight + posY);
			}
		}		
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
		if ((TERM_ScreenWidth - posX - renderWidth) < 0){
			// too width - try right to left
			if ((TERM_ScreenWidth - ((renderWidth * 2) - orgWidth) - renderWidth) < 0){
				// we're screwed - maybe single-key menu is working
				renderSmall = 1;
				renderWidth = 0;
				// Get reduced size
				while (menuPos){
					if (!renderWidth){
						// Is at least one key + 2 spaces
						renderWidth = 3;
					}
					if (menuPos->isCheck || menuPos->isOption){
						// containing option or check
						renderWidth = renderTabs * 7 + (renderTabs -1);
						break;
					}
					menuPos = menuPos->nextPos;
				}
				menuPos = menuPos1st;
			}
			else{
				// we change alignment
				posX -= (renderWidth * 2) - orgWidth;
			}
		}
		break;
	default:
		// TopMenu - 1st level		
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
					menuPos = menuPos->nextPos;
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
		break;
	}

	Locate(posX, posY);
	for (size_t i = 0; i < offsetPosY; i++){
		// trash unused positions
		menuPos = menuPos->nextPos;
	}
	while (menuPos){
		//
	}
	
	
	int posInTab = posCnt - ((posCnt / renderTabs) * (renderTabs - 1));
	for (size_t i = 0; i < renderTabs; i++){
		for (size_t j = 0; j < posInTab; j++){

			SetColorStyle(&userColors[menuDef->txtColor - 1], 1);
			SetTxtStyle(&userStyles[menuDef->txtStyle - 1], 1);
			selectedMenuPosHlp++;

			if (renderSmall){
				// render just the keys (+ eventually check/option brackets)
				printf(" ");	// 1st space...
				if (menuPos->selected && menuPos->enabled){
					// enabled & selected
					SetColorStyle(&userColors[menuDef->selectColor - 1], 1);
					SetTxtStyle(&userStyles[menuDef->selectStyle - 1], 1);
					selectedMenuPos = selectedMenuPosHlp;
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
				
				menuPos = menuPos->nextPos;
			}

			else{
				// render full line
				if (menuPos->selected && menuPos->enabled){
					selectedMenuPos = selectedMenuPosHlp;
				}
				for (size_t k = 0; k < strlen(menuPos->caption); k++){
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
					if (k == 2 && (menuPos->isCheck || menuPos->isOption)){
						// Set Value of check/option
						if (menuPos->activated){
							printf("x");
						}
						else{
							printf(" ");
						}
					}
					else{
						printf("%c", menuPos->caption[k]);
					}
				}
				SetColorStyle(&userColors[menuDef->txtColor - 1], 1);
				SetTxtStyle(&userStyles[menuDef->txtStyle - 1], 1);

				StrPrintSpaces(orgWidth - strlen(menuPos->caption));
				CursorDown(1);
				CursorLeft(orgWidth);
				//printf(" ");
				menuPos = menuPos->nextPos;
				
			}
		}
		// Cursor is on pos1 of next line (which does not get printed in actually tab)
		// The "next line" may get printed in the next tab
		CursorUp(posInTab);		// Back to Top

		posInTab = posCnt / renderTabs;
	}
	
	SetColorStyle(&userColors[menuDef->txtColor - 1], 1);
	SetTxtStyle(&userStyles[menuDef->txtStyle - 1], 1);
	
}

void TUIrenderTopMenu(int posX, int posY, int width, struct TuiMenuDefSTRUCT *menuDef, int justRefresh){

	char strHlp[STR_SMALL_SIZE];
	char strLine[STR_MID_SIZE];

	int renderRealTime = 0;
	int renderRunTime = 0;
	int renderSmall = 0;	// width too small to render full MenuText 
	int renderLen = 0;
	int dontRender = 0;		// even too small to render shortcuts
	int renderWidth = 0;

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

	if (!menuDef->renderLen){
		// 1st call
		// Calculate len of top-level MenuLine
		struct TuiMenuPosSTRUCT *menuPos = menuDef->pos1st;
		while (menuPos != NULL){
			menuDef->renderLen += strlen(menuPos->caption);
			menuPos = menuPos->nextPos;
		}
		menuDef->renderLen += 1;	// trailing Space
	}
	renderLen = menuDef->renderLen;

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
			struct TuiMenuPosSTRUCT *menuPos = menuDef->pos1st;
			while (menuPos){
				selectedMenuPosHlp++;
				if (menuPos->selected && menuPos->enabled){
					/* code */
					SetColorStyle(&userColors[menuDef->selectColor - 1], 1);
					SetTxtStyle(&userStyles[menuDef->selectStyle - 1], 1);
					selectedMenuPos = selectedMenuPosHlp;
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
				menuPos = menuPos->nextPos;
			}
			//printf(" ");
		}
		else{
			// render full line
			struct TuiMenuPosSTRUCT *menuPos = menuDef->pos1st;
			while (menuPos){
				selectedMenuPosHlp++;
				if (menuPos->selected && menuPos->enabled){
					selectedMenuPos = selectedMenuPosHlp;
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
				menuPos = menuPos->nextPos;
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

	if (selectedMenuPos && menuDef->pos1st){
		// render selected Submenu
		struct TuiMenuPosSTRUCT *menuPos = menuDef->pos1st;
		
		// Set cursor one row below start of selected menu
		CursorLeft(renderWidth);
		CursorDown(1);
		int x = 0;
		while (--selectedMenuPos){
			x += strlen(menuPos->caption) + 2;
			menuPos = menuPos->nextPos;			
		}
		CursorRight(x);
		// Render SubMenu
		TUIrenderSubMenu(++x, 3, 0, menuDef, menuPos);
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

	for (int i = 0; i < headersCount; i++){
		// Caption
		sprintf(strSearch, "global.header.%d.Caption", i + 1);
		sprintf(strHLP, "Header%d", i + 1);
		IniGetStr(strFile, strSearch, strHLP, strHeaderCaption);
		userHeader[i]->caption = IniStrToMem(strHeaderCaption, 0);
		// TextColor 
		sprintf(strSearch, "global.header.%d.TextColor", i + 1);
		userHeader[i]->txtColor = IniGetInt(strFile, strSearch, 0);
		// TextStyle 
		sprintf(strSearch, "global.header.%d.TextStyle", i + 1);
		userHeader[i]->txtStyle = IniGetInt(strFile, strSearch, 0);
		// TimeColor 
		sprintf(strSearch, "global.header.%d.TimeColor", i + 1);
		userHeader[i]->timeColor = IniGetInt(strFile, strSearch, 0);
		// TimeStyle 
		sprintf(strSearch, "global.header.%d.TimeStyle", i + 1);
		userHeader[i]->timeStyle = IniGetInt(strFile, strSearch, 0);
		// PrintRunTime 
		sprintf(strSearch, "global.header.%d.PrintRunTime", i + 1);
		userHeader[i]->printRunTime = IniGetBool(strFile, strSearch, 0);
		// PrintRealTime 
		sprintf(strSearch, "global.header.%d.PrintRealTime", i + 1);
		userHeader[i]->printRealTime = IniGetBool(strFile, strSearch, 0);

	}
	printf("\n");

	return headersCount;
	
}

struct TuiMenuPosSTRUCT *TUIaddMenuPos(const char *strFile, char *strPath, struct TuiMenuDefSTRUCT *definition, int positions){

	static struct TuiMenuPosSTRUCT *menuPos = NULL;
	static int *nextPos = NULL;
	static int *prevPos = NULL;
	
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
		menuPos = realloc(menuPos, cnt * sizeof(struct TuiMenuPosSTRUCT));
		nextPos = realloc(nextPos, (cnt + 1) * sizeof(int)); // +1-indexed to get 0 for NULL
		prevPos = realloc(prevPos, (cnt + 1) * sizeof(int)); // +1-indexed to get 0 for NULL

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
		menuPos[pos1].keyCode++;

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

		menuPos[pos1].caption = IniStrToMem(strPos1, 0);
		// already known stuff
		menuPos[pos1].definition = definition;
		// stuff to reset
		menuPos[pos1].nextPos = NULL;
		menuPos[pos1].prevPos = NULL;
		menuPos[pos1].pos1st = NULL;
		menuPos[pos1].selected = 0;
		nextPos[pos1] = 0;
		prevPos[pos1] = 0;
		
		// if not 1st pos, we have to set...
		if (i){
			// save IDs instead of pointers while recursion
			nextPos[lastPos + 1] = pos1;
			prevPos[pos1 + 1] = lastPos;
		}
		lastPos = pos1;
		
		if (menuPos[pos1].posCnt){
			sprintf(strSearch, "%s%d.", strPath, j);
			menuPos[pos1].pos1st = TUIaddMenuPos(strFile, strSearch, definition, menuPos[pos1].posCnt);
		}

	}
	
	for (size_t i = 0; i < cnt; i++){
		// restore valid pointers from IDs
		// ID = 0 // substitute for NULL
		if (nextPos[i + 1])	{
			// ID = 0 // substitute for NULL
			menuPos[i].nextPos = &menuPos[nextPos[i + 1]];
		}		
		menuPos[i].prevPos = &menuPos[prevPos[i + 1]];
	}
	
	return &menuPos[posReturn];
	
}

// "global.TopMenu."
int TUIinitMenuDefs(char *strFile, char *strPath, struct TuiMenuDefSTRUCT **menu){

	// Helper
	char strSearch[STR_SMALL_SIZE];

	sprintf(strSearch, "%s.%s", strPath, "Count");
	int menusCnt = IniGetInt(strFile, strSearch, 0);

	*menu = (struct TuiMenuDefSTRUCT*)malloc(menusCnt * sizeof(struct TuiMenuDefSTRUCT));

	// Menu Definition Values
	for (int i = 0; i < menusCnt; i++){
		sprintf(strSearch, "%s.%d.TextColor", strPath, i + 1);
		menu[i]->txtColor = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.TextStyle", strPath, i + 1);
		menu[i]->txtStyle = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.SelectColor", strPath, i + 1);
		menu[i]->selectColor = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.SelectStyle", strPath, i + 1);
		menu[i]->selectStyle = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.DisabledColor", strPath, i + 1);
		menu[i]->disabledColor = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.DisabledStyle", strPath, i + 1);
		menu[i]->disabledStyle = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.KeyColor", strPath, i + 1);
		menu[i]->keyColor = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.KeyStyle", strPath, i + 1);
		menu[i]->keyStyle = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.TimeColor", strPath, i + 1);
		menu[i]->timeColor = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.TimeStyle", strPath, i + 1);
		menu[i]->timeStyle = IniGetInt(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.PrintRunTime", strPath, i + 1);
		menu[i]->printRunTime = IniGetBool(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.PrintRealTime", strPath, i + 1);
		menu[i]->printRealTime = IniGetBool(strFile, strSearch, 0);
		sprintf(strSearch, "%s.%d.Positions", strPath, i + 1);
		menu[i]->posCnt = IniGetInt(strFile, strSearch, 0);

		menu[i]->renderLen = 0;

		// Add positions
		sprintf(strSearch, "%s.%d.", strPath, i + 1);
		menu[i]->pos1st = TUIaddMenuPos(strFile, strSearch, menu[i], menu[i]->posCnt);		

	}

	return menusCnt;
	
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


