/*		   (Simple) Version Of The Game Of Life (Originally by John Horton Conway)
      	It' a test-application to test and develop PoorTUI- Terminal Functions
					       See detailed description at the EOF !

		 	! Could contain 3rd party MIT/GPL/"FreeSoftware" technologies !
		(see detailed comments in related declarations/functions and at the EOF)

	This is a  ! p_o_o_r !  Application by (c) Pit Demmer. (PIT-Licensed 01.04.2022 - x.x.xxxx)
	PIT-licensed" isn't a joke! - Right now I do not know about the license model I wanna use!
								But Sources will be free!
	 	I'm just struggling with professional 3rd parties distributing binaries
	 (or executing a compiling process on customer machines) without contributing to
									"FreeSoftware"
*/

#define LifeVersion "1.00pa"

// #include "Terminal.h"	
// #include "AnsiESC.h"	

#include "PoorTui.h"

void UserSecondChanged(void){
	static int i = 0;
	i++;
	TERM_RunCoreLoop = 0;
	if (i < 15){
		//printf("Second\n");
		TERM_RunCoreLoop = 1;
	}
}

void UserLoop(void){
	static int i = 0;
	i++;
	//printf("Loop: %d\n", i);
}

void UserDblClick(int x, int y, int button){
	TERM_RunCoreLoop = 0;
	printf("DblClick: %d,%d,%d\n",x ,y, button);
}

int main() {
	
	LOG_SysInit = stdout;
	LOG_SysExit = stdout;
	LOG_ColorInit = stderr;
	LOG_StyleInit = stderr;
	LOG_TuiCopy = stdout;

	if (!TermInit()){
		return -1;
	}
	
	ESCinit();

	// *************************************************************

		// If You Wanna Change Defaults (see BlaBlaInit())
			// ualarm(333333,333333);
			// TIME_EventsTime = 10000;
			// TermTrapMouse(0);	// Disable 1st !
			// TermTrapMouse(1);	// Then Set New...
		
		// set Timer event(s) on your own function(s)
		TimeSecChanged = UserSecondChanged;
		
		// set Terminal event(s) on your own function(s)
		TermMouseClicks[TERM_Event_MouseDblClick] = UserDblClick;

		// get your colors from INI-file
		int userColorsCnt = ESCinitColors("desktops.ini", &userColors);
		if (!userColorsCnt){
			TermExit();
			return -1;
		}
		
		/*
		for (int i = 1; i < userColorsCnt +1; i++){
			SetColorStyle(&userColors[i], 1);
			printf("%04d. %s.%s",i, userColors[i].groupName, userColors[i].colorName);
			SetColorStyle(&userColors[i], 0);
			printf("\n");
		}
		*/
		
		// get your styles from INI-file
		int userStylesCnt = ESCinitTxtStyles("desktops.ini", &userStyles);
		if (!userStylesCnt){
			TermExit();
			return -1;
		}
		
		/*
		for (int i = 1; i < userStylesCnt + 1; i++){
			SetTxtStyle(&userStyles[i], 1);
			printf("%04d. %s_%s\n", i, userStyles[i].fontName, userStyles[i].styleName );
			SetTxtStyle(&userStyles[i], 0);
		}
		*/
	
		fflush(stdout);
		TermClearScreen(stdout, 0);
		STRtrimWS_R(gStrRunTime);
		STRtrimWS_R(gStrTime);
		TUI_HeaderCnt = TUIinitHeaders("desktops.ini", &userHeaders);
		for (int i = 0; i < TUI_HeaderCnt; i++){
			TUIrenderHeader(1,i+1,0,i,0);
		}
		TUI_FooterCnt = TUIinitFooters("desktops.ini", &userFooters);
		for (int i = 0; i < TUI_FooterCnt; i++){
			TUIrenderFooter(1,i+3,0,i,0);
			printf("\n");
		}

		int userDesktopCnt = TUIinitDesktops("desktops.ini", &userDesktopDefs);

		TUI_TopMenuCnt = TUIinitMenuDefs("desktops.ini", "TopMenu", &userTopMenus);
		TUI_BotMenuCnt = TUIinitMenuDefs("desktops.ini", "BottomMenu", &userBotMenus);
		TUI_RightMenuCnt = TUIinitMenuDefs("desktops.ini", "RightMenu", &userRightMenus);
		TUI_LeftMenuCnt = TUIinitMenuDefs("desktops.ini", "LeftMenu", &userLeftMenus);

int reduce = (int)INIstrToMem("", 2);

		fflush(stdout);
		TermClearScreen(stdout, 0);

		printf("         1         2         3         4         5         6         7         8\n");
		fflush(stdout);
		printf("12345678901234567890123456789012345678901234567890123456789012345678901234567890\n");
		ESCresFBU(stdout);
		fflush(stdout);
		TermClearScreen(stdout, 0);

		for (int i = 0; i < TUI_TopMenuCnt; i++){ //userTopMenuCnt; i++){

			//userTopMenus[i].pos1st->selected = 1;
			//userTopMenus[i].pos1st->nextPos->selected = 1;
			//userTopMenus[i].pos1st->nextPos->nextPos->selected = 1;
			//userTopMenus[i].pos1st->nextPos->nextPos->nextPos->selected = 1;
//printf("i-lead: %d\n", i);
//fflush(stdout);
			userBotMenus[i].pos1st->nextPos->nextPos->nextPos->nextPos->selected = -1;
			userBotMenus[i].pos1st->nextPos->nextPos->nextPos->nextPos->pos1st->nextPos->nextPos->selected = -1;
			userBotMenus[i].pos1st->nextPos->nextPos->nextPos->nextPos->pos1st->nextPos->nextPos->pos1st->nextPos->nextPos->selected = -1;
//printf("i-trail: %d\n", i);
//fflush(stdout);
/*
			TUIrenderRightMenu(&userTopMenus[i], 1, 0, 0, 10, 0, 0);
			TUIclearSmallInverted(userTopMenus[i].pos1st);
			TUIrenderLeftMenu(&userTopMenus[i], 0, 1, 0, 23, 0, 0);
			TUIclearSmallInverted(userTopMenus[i].pos1st);
			TUIrenderTopMenu(&userTopMenus[i], 0, 0, 0, 0, 0);
			TUIclearSmallInverted(userTopMenus[i].pos1st);
			TUIrenderBottomMenu(&userTopMenus[i], 0, 0, 0, 0, 0);
*/			
			//TUIrenderTopMenu(1, TERM_ScreenHeight, 0, &userTopMenus[i], &userDesktopDefs[0], 0);
			//ResFBU();
			//printf("\n");
			//TermClearScreen(0);
		}
		
	TUIclearSmallInverted(userTopMenus[0].pos1st);
	TUIbuildMenus(&userDesktopDefs[0], 0, 0, 0, 0);


		ESCresFBU(stdout);
		printf("\n");
		TermClearScreen(stdout, 0);

		printf("\na b c d e f g h i j k l m n opqrs t u v w x y z\n");
		printf("\x1B(0");
		printf("a b c d e f g h i j k l m n opqrs t u v w x y z\n");
		printf("\x1B(B");

		ESCresFBU(stdout);
		printf("\n");

		TermClearScreen(stdout, 0);

		printf("         1         2         3         4         5         6         7         8\n");
		printf("12345678901234567890123456789012345678901234567890123456789012345678901234567890\n");
		for (int i = 3; i < 10; i++){
			printf("%d\n", i);
		}
		for (int i = 0; i < 10; i++){
			printf("%d\n", i);
		}
		for (int i = 0; i < 4; i++){
			printf("%d\n", i);
		}
		printf("4");
		fflush(stdout);

/*		
		DECrect(5, 3, 75, 23);

		// more horizontal
		//LINlineXY(6, 4, 74, 22, 0);
		//LINlineXY(74, 22, 6, 4,  0);

		LINlineXY(6, 5, 10, 22);
		LINlineXY(11, 22, 15, 5);

		//45° Left to right / Top to bottom
		//LINlineXY(7, 5, 17, 15,  0);

		//45° Left to right / Bottom to top
		//LINlineXY(7, 15, 17, 5,  0);

		//45° Right to left / Top to bottom
		LINlineXY(37, 5, 27, 15);

		//45° Right to left / bottom to top
		//LINlineXY(37, 15, 27, 5,  0);

		DECrect(10, 6, 20, 11);

		DECrect(3, 16, 80, 25);

		LINlineXY(-5, 13, 200, 13);

		LINlineXY(200, 14, -5, 14);

		// centered verts
		LINlineXY(39, -4, 39, 50);
		LINlineXY(41, 50, 41, -4);

		DECmoveTo(stdout, 50, 5);
		LINlineTo(stdout, 55, 5);
		LINlineTo(stdout, 55, 10);
		LINlineTo(stdout, 50, 10);
		DECclose;

		DECmoveTo(stdout, 20,5);
		LINlineTo(stdout, 25,10);
		LINlineTo(stdout, 20,15);
		LINlineTo(stdout, 15,10);
		DECclose;
*/
ESCsetFg16(stdout, fgBlue);
		LINmoveTo(stdout, 82-2,5);
		LINlineTo(stdout, 82-3,19);
		LINlineTo(stdout, 82-5,20);
		LINlineTo(stdout, 82-9,9);
		LINlineTo(stdout, 82-13,21);
		LINlineTo(stdout, 82-14,19);
		LINlineTo(stdout, 82-20,18);
		LINlineTo(stdout, 82-25,9);
		LINlineTo(stdout, 82-30,3);
		LINlineTo(stdout, 82-35,4);
		LINlineTo(stdout, 82-40,12);
		LINlineTo(stdout, 82-43,21);
		LINlineTo(stdout, 82-53,6);
		LINlineTo(stdout, 82-58,11);
		LINlineTo(stdout, 82-53,16);
		LINlineTo(stdout, 82-58,21);
		LINclose(stdout);
		
ESCsetFg16(stdout, fgGreen);
		LINmoveTo(stdout, 2,5);
		LINlineTo(stdout, 3,19);
		LINlineTo(stdout, 5,20);
		LINlineTo(stdout, 9,9);
		LINlineTo(stdout, 13,21);
		LINlineTo(stdout, 14,19);
		LINlineTo(stdout, 20,18);
		LINlineTo(stdout, 25,9);
		LINlineTo(stdout, 30,3);
		LINlineTo(stdout, 35,4);
		LINlineTo(stdout, 40,12);
		LINlineTo(stdout, 43,21);
		LINlineTo(stdout, 53,6);
		LINlineTo(stdout, 58,11);
		LINlineTo(stdout, 53,16);
		LINlineTo(stdout, 58,21);
		LINclose(stdout);

ESCsetFg16(stdout, fgRed);
		LINmoveTo(stdout, 23,5);
		LINlineTo(stdout, 60,9);
		LINlineTo(stdout, 5,12);
		LINlineTo(stdout, 55,18);
		LINlineTo(stdout, 13,21);
		LINclose(stdout);
ESCsetFg16(stdout, fgYellow);
/*
		LINmoveTo(stdout, 82-23,3);
		LINlineTo(stdout, 82-70,11);
		LINlineTo(stdout, 82-5,12);
		LINlineTo(stdout, 82-55,18);
		LINlineTo(stdout, 82-13,21);
		LINclose;
*/
		DECmoveTo(stdout, 82-23,3);
		DEClineTo(stdout, 82-70,11);
		DEClineTo(stdout, 82-5,12);
		DEClineTo(stdout, 82-55,18);
		DEClineTo(stdout, 82-13,21);
		DECclose(stdout);

/*
		LINmoveTo(stdout, 20,5);
		LINlineTo(stdout, 25,10);
		LINlineTo(stdout, 20,15);
		LINlineTo(stdout, 15,10);
		LINclose;

		DECmoveTo(stdout, 20,5);
		DEClineTo(stdout, 25,10);
		DEClineTo(stdout, 20,15);
		DEClineTo(stdout, 15,10);
		DECclose;
*/

ESCtxtBold(stdout, 1);
ESCsetFg16(stdout, fgWhite);
		CHRmoveTo(stdout, 9,3, '*');
		CHRlineTo(stdout, 14,8);
		CHRlineTo(stdout, 9,13);
		CHRlineTo(stdout, 4,8);
		CHRclose(stdout);

		DECmoveTo(stdout, 21,3);
		DEClineTo(stdout, 26,8);
		DEClineTo(stdout, 21,13);
		DEClineTo(stdout, 16,8);
		DECclose(stdout);

		DECmoveTo(stdout, 33,3);
		DEClineTo(stdout, 38,8);
		DEClineTo(stdout, 33,13);
		DEClineTo(stdout, 28,8);
		DECclose(stdout);

ESCsetFg16(stdout, fgCyan);
		LINmoveTo(stdout, 33,3);
		LINlineTo(stdout, 28,8);
		LINlineTo(stdout, 33,13);
		LINlineTo(stdout, 38,8);
		LINclose(stdout);

		DECmoveTo(stdout, 45,3);
		DEClineTo(stdout, 40,8);
		DEClineTo(stdout, 45,13);
		DEClineTo(stdout, 50,8);
		DECclose(stdout);

		LINmoveTo2(stdout, 57,3);
		LINlineTo2(stdout, 52,8);
		LINlineTo2(stdout, 57,13);
		LINlineTo2(stdout, 62,8);
		LINclose2(stdout);

		LINmoveTo2(stdout, 9,14);
		LINlineTo2(stdout, 14,19);
		LINlineTo2(stdout, 9,24);
		LINlineTo2(stdout, 4,19);
		LINclose2(stdout);


ESCtxtBold(stdout, 0);
ESCresFg(stdout);
ESClocate(stdout, TERM_ScreenWidth,TERM_ScreenHeight);
printf("\nreduce: %d\n", reduce);
fflush(stdout);
//printf("%d", (10 &  ))
		// Run TUIs event loop - param is your loop if you have to "ever"-loop something...
		// Use a dummy if your app is fully event-driven 
		TermCoreLoop(UserLoop);

		// Set TUI_RunCoreLoop = 0 to reach this point

		// Free color & font strings
		//#if __APPLE__
			// NO CLUE WHY freeing is messy
		//#else
			INIstrToMem("",1);
			free(userColors);
			free(userStyles);
			free(userHeaders);
			free(userTopMenus);
			free(userBotMenus);
			free(userRightMenus);
			free(userLeftMenus);
		//#endif
	// *************************************************************


	if (!TermExit()){
		return -1;
	}
	
	return 0;

};

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
	<Life.c>		    	01.04.2022		                                Pit

BugID		Description									Found by / Date		Fixed by / Date
*/


