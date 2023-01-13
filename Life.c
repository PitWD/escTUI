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

#include "PoorTui.h"	




void UserLoop(void){

	printf("EXIT\n");
	TUI_RunCoreLoop = 0;

}

int main() {
	
	/*
	strcpy(symbolMisc[29].str, "☜");	// \u261C	WHITE LEFT POINTING INDEX
	strcpy(symbolMisc[30].str, "☝");	// \u261D	WHITE UP POINTING INDEX
	strcpy(symbolMisc[31].str, "☞");	// \u261E	WHITE RIGHT POINTING INDEX
	strcpy(symbolMisc[32].str, "☟");	// \u261F	WHITE DOWN POINTING INDEX
	*/

	int r = 0;

	printf("\nHello New Project\n\n");

	printf("Initializing Timing... ");
	TimeInitTime();
	printf("OK\n");
	printf("%s - %s\n\n", gStrDate, gStrTime);

	printf("Try To Enable Video Terminal Mode... ");
	if (!TermSetVT(1)){
		printf("ERROR!\n");
		printf("STOP PROJECT\n");
		return 0;
	}
	printf("OK\n\n");

	printf("Try To Clear Screen...\n");
	r = TermClearScreen(0);
	if (!r){
		// Can't happen....
		printf("Clear Screen... ERROR!\n");
		printf("STOP PROJECT\n");
		return 0;
	}
	printf("Clear Screen... OK, CLS-Mode: %d\n\n",r);

	printf("Try To Fetch Terminal Size... ");
	r = TermGetSize(0);
	if (!r){
		printf("ERROR!\n");
		printf("STOP PROJECT\n");
		return 0;
	}
	printf("OK, Size-Mode: %d\n",r);
	#if __WIN32__ || _MSC_VER || __WIN64__
		printf("Billy-OS: Screen Size Changes Get Polled...Sorry.\n");
	#else
		signal(SIGWINCH, TermSignalHandler);
		printf("PosiX-OS: Screen Size Changes Get Signaled.\n");
	#endif
	printf("Width: %d  Height: %d\n\n", TERM_ScreenWidth, TERM_ScreenHeight);

	printf("Synchronize CLS-Mode With Size-Mode...\n");
	r = TermClearScreen(r);
	printf("Synchronized CLS-Mode With Size-Mode... OK, Mode: %d\n",r);
	printf("Width: %d  Height: %d\n\n", TERM_ScreenWidth, TERM_ScreenHeight);

	printf("Enable Trap Mouse Mode... ");
	TermTrapMouse(1);
	printf("(probably) OK\n\n");

	printf("Enable Trap Focus Change... ");
	TermTrapFocus(1);
	printf("(probably) OK\n\n");

	printf("Catch Ctrl-C... ");
	signal(SIGINT, TermSignalHandler);
	printf("OK\n\n");

	#if __WIN32__ || _MSC_VER || __WIN64__
		printf("Billy-OS: Check On Real-Time Changes Get Polled...Sorry.\n\n");
	#else
		signal(SIGALRM, TermSignalHandler);
		ualarm(333333,333333);
		printf("PosiX-OS: Check On Real-Time Changes Get Signaled.\n\n");
	#endif
	
	InitEscSeq();
	InitColors();
	
	// *************************************************************
	TuiCoreLoop(UserLoop);
	// *************************************************************

	TermClearScreen(0);
	printf("Disable Trap Mouse Mode... ");
	TermTrapMouse(0);
	printf("OK\n\n");

	printf("Disable Trap Focus Change... ");
	TermTrapFocus(0);
	printf("OK\n\n");

	printf("Try To Set Back Terminal From Video Mode... ");
	if (!TermSetVT(0)){
		printf("ERROR!\n");
		return -1;
	}
	printf("OK\n\n");

	printf("%s - %s\n", gStrDate, gStrTime);
	printf("Runtime: %s\n\n", gStrRunTime);


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


