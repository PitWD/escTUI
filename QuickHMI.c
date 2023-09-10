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
	if (i < 1){
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
		for (size_t i = 0; i < userHeadersCnt; i++){
			TUIrenderHeader(0,0,0,i,0);
			printf("\n");
		}
		
		//printf("Und hier...\n");
		int userTopMenuCnt = TUIinitMenuDefs("desktops.ini", "global.TopMenu", &userTopMenus);

		for (size_t i = 0; i < userTopMenuCnt; i++){
			TUIrenderTopMenu(0, 0, 0, &userTopMenus[i], &userDesktopDefs[0], 0);
		}
		ResFBU();
		printf("\n");

		for (size_t i = 0; i < userTopMenuCnt; i++){

			userTopMenus[i].pos1st->selected = 1;
			TUIrenderTopMenu(0, 0, 0, &userTopMenus[i], &userDesktopDefs[0], 0);
		}
		
		ResFBU();
		printf("\n");


		printf("\na b c d e f g h i j k l m n opqrs t u v w x y z\n");
		printf("\x1B(0");
		printf("a b c d e f g h i j k l m n opqrs t u v w x y z\n");
		printf("\x1B(B");

		// Run TUIs event loop - param is your loop if you have to "ever"-loop something...
		// Use a dummy if your app is fully event-driven 
		TermCoreLoop(UserLoop);

		// Set TUI_RunCoreLoop = 0 to reach this point

		// Free color & font strings
		IniStrToMem("",1);
		#if __APPLE__
			// NO CLUE WHY freeing is messy
		#else
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


