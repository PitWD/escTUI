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

const char LifeVersion[] = "1.00pa";

#include "PoorTui.h"	

unsigned char ModeBin = 1;		// Periods of being Alive/Dead get counted in all modes! 
								// So, deads getting negative... 
unsigned char ModeAge = 0;		// If there is a lifetime, where degeneration starts.
								// (per period we're then lowering, instead of increasing the reproduction vitality)
int AgeingStart = 0;			// '>' Max Periods to start degrading

int LiveMinStart = 2;			// '>' Min Periods/Nodes around to start
int LiveMaxStart = 4;			// '<' Max  "   "   " "   "  "   "  " "
int LiveMinStop = 2;			// '<' Less than n Periods/Nodes to stop
int LiveMaxStop = 4;			// '>' More   "  "  "   "   " "   "   "


struct LifeSTRUCT {
	unsigned char aging;		// if true then age-count / period counts down 
	int age;
	int age_next;
	unsigned char state_changed;
}; struct LifeSTRUCT life[255][255];

char cmdESC27[ESC27_EXCHANGE_SIZE];

void MonitorGetESC27(void){

	int i = 0;
	int r = 0;
	char c = 0;


	ClearScreen();


	TrapMouse(1);

	// ScreenSize
	// printf("%s18t", CSI);

	
	r = WaitForESC27("\x1B[18t",0.2);
	if (r > 0){
		printf("r: %d  :%s\n", r,&streamInESC27[1]);
	}
	r = 0;

	
	int quatsch = 0;

	// Recognize manual ESC
		_Bool isOnESC27 = 0;
		clock_t timeOnUsrEsc;
	// Recognize Click & DblClick / Area-Selection
		_Bool isOnClick = 0;
		clock_t timeOnClick;
		int posXonClick = 0;
		int posYonClick = 0;


	// Loop Minimum
	while (i != 10 && i != 13){
		
			i = getch();

			// Recognize manual ESC
			if (isOnESC27 && i < 1){
				if (clock() > timeOnUsrEsc){
					// UsrESC
					i = 27;
				}
			}
			else if (i == 27){
				isOnESC27 = 1;
				timeOnUsrEsc = clock() + (0.005 * CLOCKS_PER_SEC);	// Timing interacts with Loop-Sleep...
			}
			else{
				isOnESC27 = 0;
			}

			
	// Loop Minimum

	if (!quatsch){
		// printf("\x1B[20t");		// IconLabel
		printf("\x1B[21t\n");		// WinTitle
		// printf("\x1B[18t");		// TerminalSize		
		quatsch = 1;
	}
	
	// printf("loop");

	// Loop Minimum
		if (i > 0){
	// Loop Minimum
			/*
			if(i == 32){
				printf("\n");
			}
			
			else{
			*/	
				TxtItalic(1);
				SetFg16(fgRed);
				printf("%d", i);
				TxtItalic(0);
				SetFg16(0);
				if (i > 31){// && i < 128){
					c = i;
					printf(": %c",c);
				}
				else if (i == 27){
					TxtBold(1);
					printf(": ESC");
					TxtBold(0);
				}
				
				printf("\n");
				
				// Loop Minimum
					r = GetESC27(i);
					switch (r){
					case 117:
						// Mouse UP (Left / Wheel / Right)
						if ((mouseSelX == mousePosX) && (mouseSelY == mousePosY)){
							// it's a (dbl)click
							if (isOnClick && clock() < timeOnClick){
								// dblClick
								isOnClick = 0;
								printf("dblClick\n");
							}
							else{
								// 1st Click
								isOnClick = 1;
								timeOnClick = clock() + (0.0125 * CLOCKS_PER_SEC);
							}							
						}
						else{
							// it's an area
							isOnClick = 0;
							printf("Area\n");
						}
						break;
					default:
						break;
					}
					
				// Loop Minimum
				
				if (r > 0){
					isOnESC27 = 0;
					TxtBold(1);
					printf("  : %s\t%s\n\n\n", KeyID2String[r], &streamInESC27[1]);
					TxtBold(0);
				}
			// }
	// Loop Minimum
		}

		// Recognize Single Click
		if (isOnClick && (clock() > timeOnClick)){
			// click
			isOnClick = 0;
			printf("Click\n");
		}

		usleep(100);
	}
	// Loop Minimum

	// Reset Mouse	
	TrapMouse(0);

	// Reset old Terminal Settings
	#ifdef __unix__
		tcsetattr(STDIN_FILENO, TCSANOW, &old_io);
	#endif
}

int main() {
	
	/*
	strcpy(symbolMisc[29].str, "☜");	// \u261C	WHITE LEFT POINTING INDEX
	strcpy(symbolMisc[30].str, "☝");	// \u261D	WHITE UP POINTING INDEX
	strcpy(symbolMisc[31].str, "☞");	// \u261E	WHITE RIGHT POINTING INDEX
	strcpy(symbolMisc[32].str, "☟");	// \u261F	WHITE DOWN POINTING INDEX
	*/
	
	printf("Hello New Project\n");
	
	InitEscSeq(); InitColors();
	
	printf("After Inits\n");
	
	ClearScreen();
	
	printf("After ClrScr\n");
		
	GetTerminalSize();

	printf("After Size\n");

	Locate(screenWidth / 2 - 10, screenHeight / 2);
	printf("Width: %d  Height: %d\n", screenWidth, screenHeight);

	/*	const BD_lADR = "╭";		// \x256D	LIGHT ARC DOWN AND RIGHT
	const BD_lADL = "╮";		// \x256E	LIGHT ARC DOWN AND LEFT
	const BD_lAUL = "╯";		// \x256F	LIGHT ARC UP AND LEFT
	const BD_lAUR = "╰";		// \x2570	LIGHT ARC UP AND RIGHT
	*/
	
	printf("\u2500");
	printf("%s\n", "╮");
	printf("╰");
	printf("%s\n", "╯");

	ClearScreen();

	// *************************************************************
	MonitorGetESC27();
	return 0;
	// *************************************************************

	int i = 0;
	int r = 0;

	TrapMouse(1);

	// Recognize manual ESC
	_Bool isOnESC27 = 0;
	clock_t timeOnUsrEsc;

	do{
		
		i = getch();

		// Recognize manual ESC
		if (isOnESC27 && i < 1){
			if (clock() > timeOnUsrEsc){
				// UsrESC
				i = 27;
			}
		}
		else if (i == 27){
			isOnESC27 = 1;
			timeOnUsrEsc = clock() + (0.005 * CLOCKS_PER_SEC);	// Timing interacts with Loop-Sleep...
		}
		else{
			isOnESC27 = 0;
		}
			
		if (i > 0){
			r = GetESC27(i);
		}

		usleep(100);

	} while(1);
	
	// Reset Mouse	
	TrapMouse(0);

	// Reset old Terminal Settings
	#ifdef __unix__
		tcsetattr(STDIN_FILENO, TCSANOW, &old_io);
	#endif
	// **************************************************************

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



