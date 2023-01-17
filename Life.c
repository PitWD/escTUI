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

#include "Terminal.h"	
#include "AnsiESC.h"	


void UserSecondChanged(){
	static int i = 0;
	i++;
	TERM_RunCoreLoop = 0;
	if (i < 1){
		printf("Second\n");
		TERM_RunCoreLoop = 1;
	}
}

void UserLoop(){
	static int i = 0;
	i++;
	printf("Loop: %d\n", i);
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

		// 

		// Run TUIs event loop - param is your loop if you have to "ever"-loop something...
		// Use a dummy if your app is fully event-driven 
		TermCoreLoop(UserLoop);

		// Set TUI_RunCoreLoop = 0 to reach this point

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


