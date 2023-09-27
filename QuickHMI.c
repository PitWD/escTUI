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
	if (i < 10){
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
		for (size_t i = 0; i < userColorsCnt; i++){
			SetColorStyle(&userColors[i], 1);
			printf("%04d. %s.%s\n",i, userColors[i].groupName, userColors[i].colorName);
		}
		*/
		
		// get your styles from INI-file
		int userStylesCnt = ESCinitTxtStyles("desktops.ini", &userStyles);
		if (!userStylesCnt){
			TermExit();
			return -1;
		}
		/*
		for (size_t i = 0; i < userStylesCnt; i++){
			SetTxtStyle(&userStyles[i], 1);
			printf("%04d. %s_%s\n", i, userStyles[i].fontName, userStyles[i].styleName );
			SetTxtStyle(&userStyles[i], 0);
		}
		*/
		
		StrTrimWS_R(gStrRunTime);
		StrTrimWS_R(gStrTime);
		int userHeadersCnt = TUIinitHeaders("desktops.ini", &userHeaders);
		//int userHeadersCnt = TUIinitHeaders("desktops.ini");
		for (size_t i = 0; i < userHeadersCnt; i++){
			TUIrenderHeader(0,0,0,i,0);
			printf("\n");
		}
		
		int userTopMenuCnt = TUIinitMenuDefs("desktops.ini", "global.TopMenu", &userTopMenus);
		for (size_t i = 0; i < userTopMenuCnt; i++){
			TUIrenderTopMenu(0, 0, 0, &userTopMenus[i], &userDesktopDefs[0], 0);
		}
		ResFBU();
		printf("\n");
		TermClearScreen(0);
		for (size_t i = 0; i < userTopMenuCnt; i++){

			//userTopMenus[i].pos1st->selected = 1;
			//userTopMenus[i].pos1st->nextPos->selected = 1;
			//userTopMenus[i].pos1st->nextPos->nextPos->selected = 1;
			//userTopMenus[i].pos1st->nextPos->nextPos->nextPos->selected = 1;
			userTopMenus[i].pos1st->nextPos->nextPos->nextPos->nextPos->selected = 1;
			TUIrenderTopMenu(1, 2, 0, &userTopMenus[i], &userDesktopDefs[0], 0);
		}
		
		ResFBU();
		printf("\n");

		printf("\na b c d e f g h i j k l m n opqrs t u v w x y z\n");
		printf("\x1B(0");
		printf("a b c d e f g h i j k l m n opqrs t u v w x y z\n");
		printf("\x1B(B");

		ResFBU();
		printf("\n");

		TermClearScreen(0);

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

		DECmoveTo(50, 5);
		LINlineTo(55, 5);
		LINlineTo(55, 10);
		LINlineTo(50, 10);
		DECclose;

		DECmoveTo(20,5);
		LINlineTo(25,10);
		LINlineTo(20,15);
		LINlineTo(15,10);
		DECclose;
*/
SetFg16(fgBlue);
		LINmoveTo(82-2,5);
		LINlineTo(82-3,19);
		LINlineTo(82-5,20);
		LINlineTo(82-9,9);
		LINlineTo(82-13,21);
		LINlineTo(82-14,19);
		LINlineTo(82-20,18);
		LINlineTo(82-25,9);
		LINlineTo(82-30,3);
		LINlineTo(82-35,4);
		LINlineTo(82-40,12);
		LINlineTo(82-43,21);
		LINlineTo(82-53,6);
		LINlineTo(82-58,11);
		LINlineTo(82-53,16);
		LINlineTo(82-58,21);
		LINclose;
		
SetFg16(fgGreen);
		LINmoveTo(2,5);
		LINlineTo(3,19);
		LINlineTo(5,20);
		LINlineTo(9,9);
		LINlineTo(13,21);
		LINlineTo(14,19);
		LINlineTo(20,18);
		LINlineTo(25,9);
		LINlineTo(30,3);
		LINlineTo(35,4);
		LINlineTo(40,12);
		LINlineTo(43,21);
		LINlineTo(53,6);
		LINlineTo(58,11);
		LINlineTo(53,16);
		LINlineTo(58,21);
		LINclose;

SetFg16(fgRed);
		LINmoveTo(23,5);
		LINlineTo(60,9);
		LINlineTo(5,12);
		LINlineTo(55,18);
		LINlineTo(13,21);
		LINclose;
SetFg16(fgYellow);
/*
		LINmoveTo(82-23,3);
		LINlineTo(82-70,11);
		LINlineTo(82-5,12);
		LINlineTo(82-55,18);
		LINlineTo(82-13,21);
		LINclose;
*/
		DECmoveTo(82-23,3);
		DEClineTo(82-70,11);
		DEClineTo(82-5,12);
		DEClineTo(82-55,18);
		DEClineTo(82-13,21);
		DECclose;

/*
		LINmoveTo(20,5);
		LINlineTo(25,10);
		LINlineTo(20,15);
		LINlineTo(15,10);
		LINclose;

		DECmoveTo(20,5);
		DEClineTo(25,10);
		DEClineTo(20,15);
		DEClineTo(15,10);
		DECclose;
*/

TxtBold(1);
SetFg16(fgWhite);
		LINmoveTo(9,5);
		LINlineTo(14,10);
		LINlineTo(9,15);
		LINlineTo(4,10);
		LINclose;

		DECmoveTo(21,5);
		DEClineTo(26,10);
		DEClineTo(21,15);
		DEClineTo(16,10);
		DECclose;

		DECmoveTo(33,5);
		DEClineTo(38,10);
		DEClineTo(33,15);
		DEClineTo(28,10);
		DECclose;

SetFg16(fgCyan);
		LINmoveTo(33,5);
		LINlineTo(38,10);
		LINlineTo(33,15);
		LINlineTo(28,10);
		LINclose;

		DECmoveTo(50, 5);
		DEClineTo(55, 5);
		DEClineTo(55, 10);
		DEClineTo(50, 10);
		DECclose;

TxtBold(0);
ResFg();
Locate(1,24);
fflush(stdout);
//printf("%d", (10 &  ))
		// Run TUIs event loop - param is your loop if you have to "ever"-loop something...
		// Use a dummy if your app is fully event-driven 
		TermCoreLoop(UserLoop);

		// Set TUI_RunCoreLoop = 0 to reach this point

		// Free color & font strings
		#if __APPLE__
			// NO CLUE WHY freeing is messy
		#else
			IniStrToMem("",1);
			free(userColors);
			free(userStyles);
			free(userHeaders);
			free(userTopMenus);
			free(userBotMenus);
			free(userRightMenus);
			free(userLeftMenus);
		#endif
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


