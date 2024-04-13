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

int PreCalcSubMenu(int posX, int posY, int menuType, int menuWidth, int invert, struct TuiMenusSTRUCT *menuDef, struct TuiMenuPosSTRUCT *menuPos, struct TuiDesktopsSTRUCT *deskDef, int minX, int maxX, int orgX, int orgY, int orgWidth){

	//printf("PreCalcSubMenu\n");
	//fflush(stdout);

	struct TuiMenuPosSTRUCT *selectedMenu = NULL;
	int selectedPos = 0;

	int renderSmall = 3;	// Key + 2 spaces
	int dontRender = 0;

	int renderHeight = 0;
	int renderWidth = 0;

	int posCnt = 0;
	
	struct TuiMenuPosSTRUCT *menuPos1st = menuPos;

	if (minX == 0){
		minX = 1;
	}
	if (maxX == 0){
		maxX = TERM_ScreenWidth;
	}

	// Get Size, Count, Selected, ... once... ;-)
	while (menuPos){
		int actWidth = strlen(menuPos->caption);
		posCnt++;
		renderWidth = (renderWidth > actWidth) ? renderWidth : actWidth;
		if (!selectedPos && menuPos->enabled && menuPos->selected){
			// find eventually selected pos...
			selectedPos = posCnt;
			selectedMenu = menuPos;
		}
		if (menuPos->isCheck || menuPos->isOption){
			renderSmall = 7;	// [ ] A
		}
		menuPos = menuPos->nextPos;
	}

	renderHeight = posCnt;
	menuPos = menuPos1st;

	int XfullInverted = posX - renderWidth;
	int XfullRight = posX + menuWidth;
	int XsmallInverted = posX - renderSmall;
	int XsmallRight = XfullRight;

	if (menuPos->printSmall){
		// is already forced to print small
		renderWidth = renderSmall;
		XfullInverted = XsmallInverted;
		XfullRight = XsmallRight;
	}
 
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
		// TopMenu
		XfullInverted += menuWidth + 1;
		XsmallInverted += menuWidth + 1;
		XfullRight -= menuWidth;
		XsmallRight -= XfullRight;
	}

	// Does inverted fit in X as supposed
	if (invert){
		if (XfullInverted < minX){
			// Full Size to the left will not work - try to reduce
			if (XsmallInverted < minX){
				// Damn, this area is awful small... we can't render this inverse
				invert = 0;
			}
			else{
				if (menuPos->printSmall){
					// cause we're already forced to print small - we're done
					dontRender = 1;
				}
				else{
					renderWidth = renderSmall;
				}
			}			
		}
	}

	// Does Right fit in X as supposed
	if (!invert && !dontRender){
		if (XfullRight + renderWidth > maxX){
			// Full Size to the right will not work - try inverse
			if (XfullInverted < minX){
				// Full Size inverted doesn't work - try small to the right
				if (XsmallRight + renderSmall > maxX){
					// Small Size to the right will not work - try small inverse
					if (XsmallInverted < minX){
						// Damn, this area is awful small... we can't render this
						dontRender = 1;
					}
					else{
						// Change to small inverted
						renderWidth = renderSmall;
						invert = 1;
					}
				}
				else{
					// Change to small
					renderWidth = renderSmall;
				}
			}
			else{
				// Change to inverted
				invert = 1;
			}
		}
	}
	
	// Set posX
	if (invert){
		if (renderWidth == renderSmall){
			// small
			posX = XsmallInverted;
		}
		else{
			// full
			posX = XfullInverted;
		}
	}
	else{
		posX = XfullRight;
	}

	// Render 
	if (!dontRender){

		if (selectedPos){
			// there is another subMenu to render...
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
				menuWidth = 0;
			}
			
			// set X
			if (!invert){
				posX -= menuWidth;
			}
			
			if ((renderSmall == renderWidth) && !menuPos->printSmall){
				// Sub became small this time - loop menu to find 1st full-size subMenu
				struct TuiMenuPosSTRUCT *menu1stSub = NULL;
				menuPos = menuDef->pos1st;
				while (menuPos){
					if (menuPos->selected && menuPos->enabled){
						// SubMenu found
						menuPos = menuPos->pos1st;
						if (!menu1stSub){
							// 1st SubMenu found
							menu1stSub = menuPos->parentPos;
							//printf("  1stSub: %s\n", menuPos->caption);
						}						
						if (!menuPos->printSmall){
							// SubMenu is full-size
							menuPos->printSmall = 1;
							// recalculation needed
							selectedMenu = menu1stSub;
							posX = orgX;
							invert = 0;
							menuWidth = 0;
							renderWidth = orgWidth;
							break;
						}					
					}
					menuPos = menuPos->nextPos;
				}
			}
			if (!PreCalcSubMenu(posX, posY, 4, renderWidth + menuWidth, invert, menuDef, selectedMenu->pos1st, deskDef, minX, maxX, orgX, orgY, orgWidth)){
				return 0;
			}
		}
		else if ((renderSmall == renderWidth) && !menuPos->printSmall){
			// very last sub is small - loop menu to set 1st sub as invert & small
			menuPos = menuDef->pos1st;
			while (menuPos){
				if (menuPos->selected && menuPos->enabled){
					// SubMenu found
					menuPos->pos1st->printInverted = 1;
					menuPos->pos1st->printSmall = 1;
					break;					
				}
				menuPos = menuPos->nextPos;
			}
		}
	}
	else{
		return 0;	// Can't Render
	}

	return 1;	// Ready To Render
}

void TUIrenderSubMenu(int posX, int posY, int menuType, int menuWidth, int invert, struct TuiMenusSTRUCT *menuDef, struct TuiMenuPosSTRUCT *menuPos, struct TuiDesktopsSTRUCT *deskDef, int minX, int maxX){

	// posX is always the 1st char of the calling menu
	// menuWidth is the width of the calling menu

	struct TuiMenuPosSTRUCT *selectedMenu = NULL;
	int selectedPos = 0;
	
	int renderSmall = 3;	// Key + 2 spaces
	int dontRender = 0;

	int renderHeight = 0;
	int renderWidth = 0;
	
	int posCnt = 0;

	int ymove = -1;

	struct TuiMenuPosSTRUCT *menuPos1st = menuPos;

	if (minX == 0){
		minX = 1;
	}
	if (maxX == 0){
		maxX = TERM_ScreenWidth;
	}
	
	// Get Size, Count, Selected, ... once... ;-)
	while (menuPos){
		int actWidth = strlen(menuPos->caption);
		posCnt++;
		renderWidth = (renderWidth > actWidth) ? renderWidth : actWidth;
		if (!selectedPos && menuPos->enabled && menuPos->selected){
			// find eventually selected pos...
			selectedPos = posCnt;
			selectedMenu = menuPos;
		}
		if (menuPos->isCheck || menuPos->isOption){
			renderSmall = 7;	// [ ] A
		}
		
		menuPos = menuPos->nextPos;
	}
	renderHeight = posCnt;
	menuPos = menuPos1st;

	int XfullInverted = posX - renderWidth;
	int XfullRight = posX + menuWidth;
	int XsmallInverted = posX - renderSmall;
	int XsmallRight = XfullRight;

	if (menuPos->printSmall){
		// is already forced to print small
		renderWidth = renderSmall;
		XfullInverted = XsmallInverted;
		XfullRight = XsmallRight;
	}

	if (menuPos->printInverted){
		// is already forced to print inverted
		invert = 1;
	}
	
	switch (menuType){
	case 1:
		// LeftMenu
		break;
	case 2:
		// RightMenu
		break;
	case 4:	
		// TopMenu next levels...
	case 5:
		// BottomMenu next levels...
		break;
	case 3:		
		// BottomMenu
	default:
		// TopMenu
		XfullInverted += menuWidth + 1;
		XsmallInverted += menuWidth + 1;
		XfullRight -= menuWidth;
		XsmallRight -= XfullRight;
	}
	
	// save for eventually next levels and recalculation
	int orgPosY = posY;
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
	case 5:
		// BottomMenu next levels...	
	case 3:
		// BottomMenu
		if ((posY - renderHeight) < 1){
			// too high to print all positions - shift posY above menuLine
			posY = TERM_ScreenHeight - 1;
			if ((posY - renderHeight) < 1){
				// still to high
				if (posY - selectedPos < 1){
					// invisible selectedPos - shift offset
					offsetPosY = (posY - selectedPos) * -1;
				}
			}
			else{
				// fits - shift back as much as possible
				posY -= TERM_ScreenHeight - renderHeight;
			}
		}	
		ymove = 1;	
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

	// Does inverted fit in X as supposed
	if (invert){
		if (XfullInverted < minX){
			// Full Size to the left will not work - try to reduce
			if (XsmallInverted < minX){
				// Damn, this area is awful small... we can't render this inverse
				invert = 0;
			}
			else{
				renderWidth = renderSmall;
			}			
		}
	}
	
	// Does Right fit in X as supposed
	if (!invert){
		if (XfullRight + renderWidth > maxX){
			// Full Size to the right will not work - try inverse
			if (XfullInverted < minX){
				// Full Size inverted doesn't work - try small to the right
				if (XsmallRight + renderSmall > maxX){
					// Small Size to the right will not work - try small inverse
					if (XsmallInverted < minX){
						// Damn, this area is awful small... we can't render this
						dontRender = 1;
					}
					else{
						// Change to small inverted
						renderWidth = renderSmall;
						invert = 1;
					}
				}
				else{
					// Change to small
					renderWidth = renderSmall;
				}
			}
			else{
				// Change to inverted
				invert = 1;
			}
		}
	}

	// Set posX
	if (invert){
		if (renderWidth == renderSmall){
			// small
			posX = XsmallInverted;
		}
		else{
			// full
			posX = XfullInverted;
		}
	}
	else{
		posX = XfullRight;
	}
	 
	// Render 
	if (!dontRender){
		Locate(posX, posY);
		for (size_t i = 0; i < offsetPosY; i++){
			// trash unused positions
			menuPos = menuPos->nextPos;
		}
		while (menuPos){
			
			if (renderSmall == renderWidth){
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
				else if (renderSmall == 7){
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
				
				//CursorDown(1);
				CursorMoveY(ymove);
				CursorLeft(renderSmall);
				
				menuPos = menuPos->nextPos;
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
				SetColorStyle(&userColors[menuDef->txtColor], 1);
				SetTxtStyle(&userStyles[menuDef->txtStyle], 1);

				if (menuPos->isSpacer){
					printf(" ");
					DEClineX(renderWidth - 2);
					printf(" ");
				}
				else{
					StrPrintSpaces(renderWidth - strlen(menuPos->caption));
				}
				
				//CursorDown(1);
				CursorMoveY(ymove);
				CursorLeft(renderWidth);
				//printf(" ");
				fflush(stdout);
				menuPos = menuPos->nextPos;
				
			}
		}

		if (selectedPos){
			// there is another subMenu to render...
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
				menuWidth = 0;
			}
			// set X & Y
			posY = orgPosY + selectedPos - 1 - offsetPosY - (orgPosY - posY);
			if (!invert){
				posX -= menuWidth;
			}
			
			TUIrenderSubMenu(posX, posY, 4, renderWidth + menuWidth, invert, menuDef, selectedMenu->pos1st, deskDef, minX, maxX);
		}
	}
	else{
		ResFBU();
		SetFg16(fgRed);
		TxtBold(1);
		printf("!! Can't Render Sub/Vert-Menu !!\n");
		TxtBold(0);
		ResFBU();
	}
			
}

void TUIrenderSubMenuOld(int posX, int posY, int menuType, int menuWidth, int invert, struct TuiMenusSTRUCT *menuDef, struct TuiMenuPosSTRUCT *menuPos, struct TuiDesktopsSTRUCT *deskDef, int minX, int maxX){

	// posX is always the 1st char of the calling menu
	// menuWidth is the width of the calling menu

	struct TuiMenuPosSTRUCT *selectedMenu = NULL;
	int selectedPos = 0;
	
	int renderSmall = 3;	// Key + 2 spaces
	int dontRender = 0;

	int renderHeight = 0;
	int renderWidth = 0;
	
	int posCnt = 0;

	struct TuiMenuPosSTRUCT *menuPos1st = menuPos;

	if (minX == 0){
		minX = 1;
	}
	if (maxX == 0){
		maxX = TERM_ScreenWidth;
	}
	
	// Get Size, Count, Selected, ... once... ;-)
	while (menuPos){
		int actWidth = strlen(menuPos->caption);
		posCnt++;
		renderWidth = (renderWidth > actWidth) ? renderWidth : actWidth;
		if (!selectedPos && menuPos->enabled && menuPos->selected){
			// find eventually selected pos...
			selectedPos = posCnt;
			selectedMenu = menuPos;
		}
		if (menuPos->isCheck || menuPos->isOption){
			renderSmall = 7;	// [ ] A
		}
		
		menuPos = menuPos->nextPos;
	}
	renderHeight = posCnt;
	menuPos = menuPos1st;

	int XfullInverted = posX - renderWidth;
	int XfullRight = posX + menuWidth;
	int XsmallInverted = posX - renderSmall;
	int XsmallRight = XfullRight;

	if (menuPos->printSmall){
		// is already forced to print small
		renderWidth = renderSmall;
		XfullInverted = XsmallInverted;
		XfullRight = XsmallRight;
	}

	if (menuPos->printInverted){
		// is already forced to print inverted
		invert = 1;
	}
	
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
		// TopMenu
		XfullInverted += menuWidth + 1;
		XsmallInverted += menuWidth + 1;
		XfullRight -= menuWidth;
		XsmallRight -= XfullRight;
	}
	
	// save for eventually next levels and recalculation
	int orgPosY = posY;
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

	// Does inverted fit in X as supposed
	if (invert){
		if (XfullInverted < minX){
			// Full Size to the left will not work - try to reduce
			if (XsmallInverted < minX){
				// Damn, this area is awful small... we can't render this inverse
				invert = 0;
			}
			else{
				renderWidth = renderSmall;
			}			
		}
	}
	
	// Does Right fit in X as supposed
	if (!invert){
		if (XfullRight + renderWidth > maxX){
			// Full Size to the right will not work - try inverse
			if (XfullInverted < minX){
				// Full Size inverted doesn't work - try small to the right
				if (XsmallRight + renderSmall > maxX){
					// Small Size to the right will not work - try small inverse
					if (XsmallInverted < minX){
						// Damn, this area is awful small... we can't render this
						dontRender = 1;
					}
					else{
						// Change to small inverted
						renderWidth = renderSmall;
						invert = 1;
					}
				}
				else{
					// Change to small
					renderWidth = renderSmall;
				}
			}
			else{
				// Change to inverted
				invert = 1;
			}
		}
	}

	// Set posX
	if (invert){
		if (renderWidth == renderSmall){
			// small
			posX = XsmallInverted;
		}
		else{
			// full
			posX = XfullInverted;
		}
	}
	else{
		posX = XfullRight;
	}
	 
	// Render 
	if (!dontRender){
		Locate(posX, posY);
		for (size_t i = 0; i < offsetPosY; i++){
			// trash unused positions
			menuPos = menuPos->nextPos;
		}
		while (menuPos){
			
			if (renderSmall == renderWidth){
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
				else if (renderSmall == 7){
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
				
				CursorDown(1);
				CursorLeft(renderSmall);
				
				menuPos = menuPos->nextPos;
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
				SetColorStyle(&userColors[menuDef->txtColor], 1);
				SetTxtStyle(&userStyles[menuDef->txtStyle], 1);

				if (menuPos->isSpacer){
					printf(" ");
					DEClineX(renderWidth - 2);
					printf(" ");
				}
				else{
					StrPrintSpaces(renderWidth - strlen(menuPos->caption));
				}
				
				CursorDown(1);
				CursorLeft(renderWidth);
				//printf(" ");
				fflush(stdout);
				menuPos = menuPos->nextPos;
				
			}
		}

		if (selectedPos){
			// there is another subMenu to render...
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
				menuWidth = 0;
			}
			// set X & Y
			posY = orgPosY + selectedPos - 1 - offsetPosY - (orgPosY - posY);
			if (!invert){
				posX -= menuWidth;
			}
			
			TUIrenderSubMenu(posX, posY, 4, renderWidth + menuWidth, invert, menuDef, selectedMenu->pos1st, deskDef, minX, maxX);
		}
	}
	else{
		ResFBU();
		SetFg16(fgRed);
		TxtBold(1);
		printf("!! Can't Render Sub/Vert-Menu !!\n");
		TxtBold(0);
		ResFBU();
	}
			
}

void TUIrenderTopMenu(int posX, int posY, int width, struct TuiMenusSTRUCT *menuDef, struct TuiDesktopsSTRUCT *deskDef, int justRefresh){

	char strHlp[STR_SMALL_SIZE];
	char strLine[STR_MID_SIZE];

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

	if (!posX){
		// Start at 1st Pos
		posX = 1;
	}
	if (!width){
		width = TERM_ScreenWidth;
	}

	// renderWidth - respecting posX
	int renderWidth = width - posX + 1;	

	// Calculate len of top-level MenuLine options - without time & date
	struct TuiMenuPosSTRUCT *menuPos = menuDef->pos1st;
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
			
			int butSelected = 0;	// if a selected key fits in full width
			
			// Style & Color
			SetColorStyle(&userColors[menuDef->txtColor], 1);
			SetTxtStyle(&userStyles[menuDef->txtStyle], 1);

			if (renderStyle){
				// render just the keys
				struct TuiMenuPosSTRUCT *menuPos = menuDef->pos1st;
				while (menuPos){
					if (menuPos->selected && menuPos->enabled){
						// selected
						if (renderStyle == 2){
							// we can't fully render this menu
							subXw = 2;
							SetColorStyle(&userColors[menuDef->selectColor], 1);
							SetTxtStyle(&userStyles[menuDef->selectStyle], 1);
							printf(" ");	// 1st space...
						}
						else{
							// we can fully render the selected menu
							butSelected = 1;
							subXw = strlen(menuPos->caption);
							for (size_t i = 0; i < subXw; i++){
								if (i == menuPos->keyCode){
									SetColorStyle(&userColors[menuDef->selectKeyColor], 1);
									SetTxtStyle(&userStyles[menuDef->selectKeyStyle], 1);
								}
								else{
									SetColorStyle(&userColors[menuDef->selectColor], 1);
									SetTxtStyle(&userStyles[menuDef->selectStyle], 1);
								}
								printf("%c", menuPos->caption[i]);
							}
							subXw--;
						}
						SetColorStyle(&userColors[menuDef->selectKeyColor], 1);
						SetTxtStyle(&userStyles[menuDef->selectKeyStyle], 1);
					}
					else if (menuPos->enabled){
						printf(" ");	// 1st space...
						SetColorStyle(&userColors[menuDef->keyColor], 1);
						SetTxtStyle(&userStyles[menuDef->keyStyle], 1);
					}
					else{
						printf(" ");	// 1st space...
						SetColorStyle(&userColors[menuDef->disabledColor], 1);
						SetTxtStyle(&userStyles[menuDef->disabledStyle], 1);
					}

					if (!butSelected){
						printf("%c", menuPos->caption[menuPos->keyCode]);
					}
					
					if (menuPos->selected && menuPos->enabled){
						SetColorStyle(&userColors[menuDef->selectColor], 1);
						SetTxtStyle(&userStyles[menuDef->selectStyle], 1);
					}
					else if (menuPos->enabled){
						SetColorStyle(&userColors[menuDef->txtColor], 1);
						SetTxtStyle(&userStyles[menuDef->txtStyle], 1);					
					}
					else{
						SetColorStyle(&userColors[menuDef->disabledColor], 1);
						SetTxtStyle(&userStyles[menuDef->disabledStyle], 1);
					}			

					if (!butSelected){
						printf(" ");
					}
					butSelected = 0;

					SetColorStyle(&userColors[menuDef->txtColor], 1);
					SetTxtStyle(&userStyles[menuDef->txtStyle], 1);					

					menuPos = menuPos->nextPos;
					if (!subXw){
						subXs += 3;
					}
				}
				//printf(" ");
			}
			else{
				// render full line
				struct TuiMenuPosSTRUCT *menuPos = menuDef->pos1st;
				while (menuPos){
					for (size_t i = 0; i < strlen(menuPos->caption); i++){
						if (menuPos->enabled && menuPos->selected){
							subXw++;
							if (i == menuPos->keyCode){
								SetColorStyle(&userColors[menuDef->selectKeyColor], 1);
								SetTxtStyle(&userStyles[menuDef->selectKeyStyle], 1);
							}
							else{
								SetColorStyle(&userColors[menuDef->selectColor], 1);
								SetTxtStyle(&userStyles[menuDef->selectStyle], 1);
							}
						}
						else if (menuPos->enabled){
							if (i == menuPos->keyCode){
								SetColorStyle(&userColors[menuDef->keyColor], 1);
								SetTxtStyle(&userStyles[menuDef->keyStyle], 1);
							}
							else{
								SetColorStyle(&userColors[menuDef->txtColor], 1);
								SetTxtStyle(&userStyles[menuDef->txtStyle], 1);
							}
						}
						else{
							SetColorStyle(&userColors[menuDef->disabledColor], 1);
							SetTxtStyle(&userStyles[menuDef->disabledStyle], 1);
						}						
						printf("%c", menuPos->caption[i]);
						if (!subXw){
							subXs ++;
						}
					}
					menuPos = menuPos->nextPos;
				}
				subXw--;
			}

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
			// delete small and inverted flags from sub-structure before render
			struct TuiMenuPosSTRUCT *menuPos = selectedMenu->pos1st;
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
			// render selected Submenu
			if (PreCalcSubMenu(subXs, 3, 0, subXw, 0, menuDef, selectedMenu->pos1st, deskDef, 0, 0, subXs, 3, subXw)){
				//printf("pre-Render\n");
				//fflush(stdout);
				TUIrenderSubMenu(subXs, 3, 0, subXw, 0, menuDef, selectedMenu->pos1st, deskDef, 0, 0);
			}
		}
	}
	else{
		ResFBU();
		SetFg16(fgRed);
		TxtBold(1);
		printf("!! Can't Render Top-Menu !!\n");
		TxtBold(0);
		ResFBU();
	}
}

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


	for (int i = 0; i < positions; i++){
		
		int j = i + 1;	// User to Memory indexing

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
	int j = 0;
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
		for (size_t i = 0; i < cnt; i++){
			printf("pos: %d @: %d pos1st: %d parentPos: %d nextPos: %d prevPos: %d\n", i, &menuPos[i], menuPos[i].pos1st, menuPos[i].parentPos, menuPos[i].nextPos, menuPos[i].prevPos);
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


