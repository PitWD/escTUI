
//		       Terminal Functions - See detailed description at the EOF !
//				! Using 3rd party MIT/GPL/"FreeSoftware" technologies !
//				  (see related declarations/functions and at the EOF)
// This is a  ! p_o_o_r !  Header by (c) Pit Demmer. (PIT-Licensed 01.04.2022 -           )

const char TerminalVersion [] = "1.00pa";

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ESC_CLRSCR 0
#define ESC_SCREENSIZE 0
#define ESC_SCREENSIZE_LEGACY 0

#define ESC27_EXCHANGE_SIZE 33          // Has to be at least 1 greater than greatest to expect Command/Response !!
#define ESC27_STREAM_IN_SIZE 33         // Has to be >= ESC27_EXCHANGE_SIZE !!
unsigned char streamInESC27[ESC27_STREAM_IN_SIZE];

#ifdef __unix__
    #include <termios.h>
    #include <unistd.h>
    #include <sys/ioctl.h>
#elif __WIN32__ || _MSC_VER
    #include <conio.h>
    #include <windows.h>
#endif

// Screen
int screenWidth = 0;
int screenHeight = 0;
_Bool screenSizeInCursorPos = 0;

// Keyboard
int cursorPosX = 0;
int cursorPosY = 0;
int cursorSelX = 0;                     // 1st point of selection rectangle
int cursorSelY = 0;                     // or same as CursorPosX&Y = No selection
_Bool cursorIsSelecting = 0;            // (Moving with Shift)
clock_t userEscTimeout = 0.005 * CLOCKS_PER_SEC;

// Mouse
int mousePosX = 0;
int mousePosY = 0;
int mouseSelX = 0;                     // 1st point of selection rectangle
int mouseSelY = 0;                     // or same as MousePosX&Y = No selection
int mouseButton = 0;				   // 1 = left, 2 = wheel, 4 = right
clock_t mouseClickTimeout = 0.0125 * CLOCKS_PER_SEC;

//ESC27 Reading
_Bool isWaitingForESC27 = 0;


// Cross compatible getch() function 
    /*
    Inspired by  http://cc.byexamples.com/2007/04/08/non-blocking-user-input-in-loop-without-ncurses/
                 https://gist.github.com/FyodorK/f22af54cfc1dbcd1754c33d8cd239bef
    but much more by the German book "C von A bis Z" (C from A to Z)
    */

/* LINUX / UNIX */
#ifdef __unix__

    static struct termios new_io;
    static struct termios old_io;
    
    // Switch to cbreak mode:
        // Disable ECHO and ICANON

    int cbreak(int fd) {
        
        // Save actual Terminal
        if((tcgetattr(fd, &old_io)) == -1){
            return -1;
        }
        new_io = old_io;
        
        // Change Flags to cbreak-Mode
        new_io.c_lflag = new_io.c_lflag & ~(ECHO|ICANON);
        new_io.c_cc[VMIN] = 1;
        new_io.c_cc[VTIME]= 0;
        
        // Set cbreak-Mode
        if((tcsetattr(fd, TCSAFLUSH, &new_io)) == -1){
            return -1;
        }
        return 1;
    }

    int getch(void) {
        
		static _Bool isInit = 0;
		static int cnt = 0;
        int c = 0;
        
		if (!isInit){
			if(cbreak(STDIN_FILENO) == -1) {
				// fprintf(stderr,"cbreakERR\n");
				return -1;
	        }
			isInit = 1;
	        //ioctl(0, FIONREAD , &c);
		}
		
		// This seems to work - almost... with real Keyboard/Mouse - probably nearly perfect...
		// But - if an answer out of "int WaitForESC27()" is too unexpected it could hang on following "c = getchar()" until next Key-Press...
		// The VS-Code integrated terminal even need a very first keystroke to work then proper... Xterm is working from start perfect...
        ioctl(0, FIONREAD , &c);
		if (c > 0){
			cnt = c;
		}
		if (cnt > 0 || isWaitingForESC27){
			// printf("Chars: %d: ",cnt);
			c = getchar();
			cnt--;
		}
        return c;
    }
/* Billy OS */
#elif __WIN32__ || _MSC_VER
    // #include <conio.h>
#endif

/*
// getch() TEST
int main(void) {
    int c;
    printf("Press 'q' to quit!\n");
    // Waiting for q. 
    while(( c = getch() ) != 'q');
    return EXIT_SUCCESS;
}
*/


// Cross compatible  CONSOLE_SCREEN_BUFFER_INFO 
    // Set Terminal Size in Chars
    // to globals: ScreenWidth & ScreenHeight

void GetTerminalSize(void){

    /* LINUX / UNIX */
    #if ESC_SCREENSIZE

        screenWidth = 0;
        screenHeight = 0;
        #ifdef ESC_SCREENSIZE_LEGACY

            // Poll for 'CSIy;xR' - see AnsiESC.h
            screenSizeInCursorPos = 1;

            // Placing Cursor outside Screen places Cursor on real LowerRight
            printf("\x1B[9999,9999H"); 
            // DSR (Devive Status Report - Get Cursor Position) 
            printf("\x1B[6n");

        #else
            
            // Xterm - TerminalSize
            printf("\x1B[18t");

        #endif

    #else

        #ifdef __unix__
        
            struct winsize w;
            ioctl(fileno(stdout), TIOCGWINSZ, &w);
            screenWidth = (int)(w.ws_col);
            screenHeight = (int)(w.ws_row);

        /* Billy OS */
        #elif __WIN32__ || _MSC_VER

            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
            screenWidth = (int)(csbi.srWindow.Right - csbi.srWindow.Left + 1);
            screenHeight = (int)(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);

        /* ESC */
        #else

            /* !!! Not finished nor tested !!!
            Poll for 'CSIy;xR' - is missing */


            
                //Missing
        #endif
    #endif
}


void ClearScreen(){
#if ESC_CLRSCR

    /* ESC */
	printf("\x1B[2J");          // "just" Screen
	// printf("%s3J", CSI);	    // Including Buffer

#else

    /* LINUX / UNIX */
    #ifdef __unix__
        
        system ("clear");
    
    /* Billy OS */
    #elif __WIN32__ || _MSC_VER
        // system ("cls");
        clrscr();                // It's MUCH faster via <conio.h>
    #endif

#endif
}

int GetESC27 (int c){

	//	c = one (by one) char from the stream

	static _Bool isValid = 0;
	static _Bool isCSI = 0;
	static _Bool isOSC = 0;
	static _Bool isMouse = 0;
	static _Bool isByteMouse = 0;
	static _Bool allowTxt = 0;
	static _Bool waitForEOT = 0;
	static int numCnt = 0;
	static unsigned char *pNumPos[ESC27_STREAM_IN_SIZE];
	static _Bool isNumGroup[ESC27_STREAM_IN_SIZE];
	static unsigned char streamPos = 0;

	// Return value 	-2	= just the Termination Char was unknown.
	//					-1	= Unknown / Illegal
	//					-4	= overflow - too long
	//					-5	= unexpected EOT
	//					 0	= Valid, but waiting for more Bytes to identify
	//					 n	= The pressed key (see related enum's)
	int r = -1;		

	if (c == 127){
		// Back
		r = 106;
		if (isValid){
			// Alt-Back
			r += 51;
		}
		streamInESC27[1] = 0;
		goto SaveGetESC27ErrReturn;
	}
	else if (c == 9){
		// TAB
		streamInESC27[1] = 0;
		r = 113;
		goto SaveGetESC27ErrReturn;
	}
	else if (c == 10 || c == 13){
		// ENTER
		streamInESC27[1] = 0;
		r = 115;
		goto SaveGetESC27ErrReturn;
	}
	else if (c == 27){
		// (Re)entering ESC Mode	
		if (allowTxt){
			// expecting "EndOfText" '\' as next char
			waitForEOT = 1;
			return 0;
		}
		
		if (isValid){
			// ESC came twice... p			
			if (!streamPos){
				// User ESC
				isValid = 0;
				return 108;
			}
			else{
				// looks like UserESC - but is BS (e.g. Overflow Mouse in ByteMode)
				r = 155;
				goto SaveGetESC27ErrReturn;
			}
			
		}
		
		isValid = 1;
		isCSI = 0;
		isOSC = 0;
	 	isMouse = 0;
	 	isByteMouse = 0;
		allowTxt = 0;
		waitForEOT = 0;
		streamPos = 0;
		numCnt = 0;
		streamInESC27[0] = 27;
		streamInESC27[1] = 0;		
		return 0;
	
	}
	
	else if (!isValid){
		// Something not related to ESC 
		return -1;
	}

	streamPos++;
	
	if (streamPos < ESC27_STREAM_IN_SIZE - 1)
	{
		streamInESC27[streamPos] = c;
		streamInESC27[streamPos + 1] = 0;
	}
	else{
		// Overflow
		r = -4;
		goto SaveGetESC27ErrReturn;
	}

	if (allowTxt){
		if (waitForEOT){
			if (isOSC){
				if (c == 92){
					// EndOfText
					switch (streamInESC27[2]){
					case 76:
						// Window title
						r = 110;
						break;
					case 108:
						// Icon label
						r = 111;
					default:
						break;
					}
					goto SaveGetESC27ErrReturn;
				}
				// EOT Error
				r = -5;
				goto SaveGetESC27ErrReturn;
			}			
		}
		return 0;
	}
	
	if (!isMouse){
		// Fixed Length Sequences
		switch (streamPos){
		case 1:
			if (c > 96 && c < 123){
				// Alt + a-z
				r = c - 17;
			}
			if (c > 64 && c < 91){
				// ShiftAlt + A-Z
				r = c + 64;
			}
			else{
				r = 0;
			}
			
		case 2:
			if (streamInESC27[1] == 79){
				if (c > 79 && c < 84){
					// F1 - F4
					r = c - 79;
				}
				else if (c == 49){
					// Messed Up CSI in LX-Terminal for Shift/Ctrl F1-F4
					isCSI = 1;
				}
			}
			else if (streamInESC27[1] == 91){
				if (c > 64 && c < 73 && c != 71){
					// Up / Down / Right / Left / Center / End / UNKNOWN / Pos1
					r = c - 22;
				}
				else if (c == 90){
					// Shift-TAB
					r = 114;
				}
				else if (c == 60){
					// Mouse Trapping Start
					isMouse = 1;
					return 0;
				}
				else if (c == 77){
					// Byte-Mouse Trapping Start
					isByteMouse = 1;
					return 0;
				}
				isCSI = 1;
			}
			else if (streamInESC27[1] == 93){
				if (c == 76 || c == 108){
					// Icon label & Window title
					allowTxt = 1;
					r = 0;
				}			
				isOSC = 1;
			}
			else{
				// Illegal / Unknown char on streamInESC27[1]
				isValid = 0;
			}
			break;

		case 3:
			if (c == 126){
				if (streamInESC27[2] > 49 && streamInESC27[2] < 55){
					// Ins / Del / PgUp / PgDown
					r = streamInESC27[2] + 1;
				}
			}
			break;
		
		case 4:
			if (c == 126){
				if (streamInESC27[3] < 53){
					// F9 / F10 / F11 / F12
					r = streamInESC27[3] - 38;
				}
				else if (streamInESC27[3] < 58){
					// F5 / F6 / F7  F8
					r = streamInESC27[3] - 48;
				}
			}
			break;

		case 5:
			if (isByteMouse){
				// Mouse in ByteMode...
				// ByteMode is dangerous! XY-Positions > 223 may crash the Terminal ! 
				// But ByteMode has just ~40% of the data volume....
				mousePosX = 0; mousePosY = 0;
				if (streamInESC27[4]>32){
					mousePosX = streamInESC27[4] - 32;
				}
				else{
					// Mouse-X Out Of Range
					r = 156;
					goto SaveGetESC27ErrReturn;
				}
				
				if (streamInESC27[5]>32){
					mousePosY = streamInESC27[5] - 32;
				}
				else{
					// Mouse-Y Out Of Range
					r = 156;
					goto SaveGetESC27ErrReturn;
				}
				
				r = streamInESC27[3];
				// Switch off Shift / Alt / Ctrl
				r &= ~((1 << 2) | (1 << 3) | (1 << 4));

				switch (r){
				case 32:
					// LeftDown
					r = 116;
					mouseButton = 1;
					mouseSelX = mousePosX; mouseSelY = mousePosY;
					break;
				case 34:
					// RightDown
					r = 126;
					mouseButton = 4;
					mouseSelX = mousePosX; mouseSelY = mousePosY;
					break;
				case 35:
					// Mouse Up (Wheel / Right / Left)
					r = 117;
					break;
				case 33:
					// WheelDown
					r = 118;
					mouseButton = 2;
					mouseSelX = mousePosX; mouseSelY = mousePosY;
					break;
				case 67:
					// MouseMove - (All Keys Up)
					r = 120;
					break;
				case 64:
					// MouseMove LeftDown
					r = 121;
					break;
				case 66:
					// MouseMove RightDown
					r = 127;
					break;
				case 65:
					// MouseMove WheelDown
					r = 122;
					break;
				case 96:
					// ScrollWheelUp
					r = 123;
					break;
				case 97:
					// ScrollWheelDown
					r = 124;
					break; 
				default:
					// Unknown Mouse
					r = 125;
				}
				isByteMouse = 0;
			}
			else if (c > 79 && c < 84){
				// Shift OR Ctrl + F1 - F4
				r = c - 65;
				if (streamInESC27[4] == 53){
					// Ctrl
					r+= 14;
				}
			}
			else if (c > 64 && c < 73 && c != 71){
				// Shift OR Ctrl OR Alt + Up / Down / Right / Left / Center / End / UNKNOWN / Pos1
				r = c - 9;
				if (streamInESC27[4] == 53){
					// Ctrl
					r+= 8;
				}
				if (streamInESC27[4] == 51){
					// Alt
					r+= 16;
				}
			}		
			break;

		case 6:
			if (c == 126){
				// Shift OR Ctrl + F5 - F12
				r = streamInESC27[3] - 24;
				if (streamInESC27[3] > 52){
					// F5 - F8
					r-= 10;
				}
				if (streamInESC27[5] == 53){
					// Ctrl
					r+= 14;
				}
			}
			break;		
		};
	}
	
	if (isByteMouse){
		return 0;
	}
	
	if (r){
		// Unknown OR valid OR flexible Sequence found
		isValid = 0;
		if (r == -1 && isCSI){
			if (c > 46 && c < 60){
				// All numbers 0-9 and ; and : are valid (values and separators)
				if (!numCnt){
					// starts with separator and 1st Number was 0... (cause missing)
					// OR
					// it's starting with a digit...					
					numCnt = 1;
					isNumGroup[1] = 0;
					pNumPos[1] = &(streamInESC27[streamPos]);
				}
				if (c > 57){
					// colon & semi-colon
					numCnt++;
					pNumPos[numCnt] = &(streamInESC27[streamPos + 1]);
					isNumGroup[numCnt] = 0;
					if (c == 58){
						// previous and next number are a colon separated group
						isNumGroup[numCnt - 1] = 1;
					}
				}
				r = 0;
				isValid = 1;
			}
			else{
				// Any known terminator of a flexible length sequence ?
				switch (c){
				case 82:
					// Actual Cursor Position
					cursorPosY = atoi(pNumPos[1]);
					cursorPosX = atoi(pNumPos[2]);
					if (screenSizeInCursorPos){
						/* code */
						screenWidth = cursorPosX;
						screenHeight = cursorPosY;
						screenSizeInCursorPos = 0;
					}
					r = 107;
					break;				
				case 77:
					// Mouse Move  /  Mouse Down / Wheel Down
				case 109:
					// Mouse Up / Wheel Up
					if  (isMouse){
						mousePosX = atoi(pNumPos[2]);
						mousePosY = atoi(pNumPos[3]);
						r = (atoi(pNumPos[1]));
						// Switch off Shift / Alt / Ctrl
						r &= ~((1 << 2) | (1 << 3) | (1 << 4));
						
						if (c == 77){
							// Mouse Move  /  Mouse Down / Wheel Down
							switch(r){
							case 35:
								// Move
								r = 120;
								break;
							case 32:
								// Move Button Pressed
								r = 121;
								break;
							case 34:
								// Move Right Button Pressed
								r = 127;
								break;
							case 64:
								// Wheel Scroll Up
								r = 123;
								break;
							case 65:
								// Wheel Scroll Down
								r = 124;
								break;
							case 0:
								// Button Down
								mouseButton = 1;
								mouseSelX = mousePosX; mouseSelY = mousePosY;
								r = 116;
								break;
							case 2:
								// Right Button Down
								mouseButton = 4;
								mouseSelX = mousePosX; mouseSelY = mousePosY;
								r = 126;
								break;
							case 1:
								// Wheel Down
								mouseButton = 2;
								mouseSelX = mousePosX; mouseSelY = mousePosY;
								r = 118;
								break;
							case 33:
								// Move Wheel Pressed
								r = 122;
								break;
							default:
								// UMO - Unknown Mouse Object
								r = 125;
								break;
							}
						}
						else{
							// Mouse Up / Wheel Up
							if (r < 3){
								// Mouse Up (Wheel / Right / Left)
								r = 117;
							}
							else{
								// UMO - Unknown Mouse Object
								r = 125;
							}
						}
					}
					break;
				case 116:
					// Terminal Infos
                    switch (atoi(pNumPos[1])){
                    case 8:
                        // ScreenSize
                        screenHeight = atoi(pNumPos[2]);
                        screenWidth = atoi(pNumPos[3]);
                        r = 109;
                        break;
                    
                    default:
                        // Unknown terminal sequence
						r = 112;
                    }
					break;
				default:
					// Unknown or broken sequence
					r = -2;
					break;
				}
			}
		}	
	}
	return r;

	SaveGetESC27ErrReturn: isValid = 0; allowTxt = 0; isMouse = 0;	isByteMouse = 0; return r;

}

int WaitForESC27(char *pStrExchange, float timeOut){
    // Send pStrExchange[] as initial command
    // Wait max timeOut sec for the answer
    // return       -1  Illegal answer
    //              -2  Unknown Termination
    //              -3  Timeout with char(s) received
    //              -4  Answer too long
	//				-5	Unknown Terminal Sequence
    //              0   Timout without char(s) received
    //              1-n Known ESC sequence
    // saveAnswer   1   pStrExchange returns the received char(s)

    int r = 0;
    int i = 0;
    int cnt = -1;
	clock_t timeExit;

    printf("%s",pStrExchange);

    timeExit = clock() + (timeOut * CLOCKS_PER_SEC);
	
	isWaitingForESC27 = 1;

    while (clock() < timeExit){
        i = getch();
        if (i > 0){
            cnt++;
            if (cnt > ESC27_EXCHANGE_SIZE - 2){
                //Error - Answer too long
				isWaitingForESC27 = 0;
                return -4;
            }
            r = GetESC27(i);
            if (r){
				isWaitingForESC27 = 0;
                return r;
            }
        }
    }
    // TimeOut
	isWaitingForESC27 = 0;
    if (cnt > -1){
        // With char(s) received
        return -3;
    }
    else{
        // Without char(s) reveived
        return 0;
    }
}

int EventESC27 (int event){
	
	switch (event){
	// F1 - F12
	case 1:
		// F1
		break;
	case 2:
		// F2
		break;
	case 3:
		// F3
		break;
	case 4:
		// F4
		break;
	case 5:
		// F5
		break;
	case 7:
		// F6
		break;
	case 8:
		// F7
		break;
	case 9:
		// F8
		break;
	case 10:
		// F9
		break;
	case 11:
		// F10
		break;
	case 13:
		// F11
		break;
	case 14:
		// F11
		break;

	// Shift F1 - F12
	case 15:
		// F1
		break;
	case 16:
		// F2
		break;
	case 17:
		// F3
		break;
	case 18:
		// F4
		break;
	case 19:
		// F5
		break;
	case 21:
		// F6
		break;
	case 22:
		// F7
		break;
	case 23:
		// F8
		break;
	case 24:
		// F9
		break;
	case 25:
		// F10
		break;
	case 27:
		// F11
		break;
	case 28:
		// F11
		break;

	// CTRL F1 - F12
	case 29:
		// F1
		break;
	case 30:
		// F2
		break;
	case 31:
		// F3
		break;
	case 32:
		// F4
		break;
	case 33:
		// F5
		break;
	case 35:
		// F6
		break;
	case 36:
		// F7
		break;
	case 37:
		// F8
		break;
	case 38:
		// F9
		break;
	case 39:
		// F10
		break;
	case 41:
		// F11
		break;
	case 42:
		// F11
		break;

	// Navigation
	case 43:
		// Up
		break;
	case 44:
		// Down
		break;
	case 45:
		// Right
		break;
	case 46:
		// Left
		break;
	case 47:
		// Center
		break;
	case 48:
		// End
		break;
	case 50:
		// Pos1
		break;
	case 51:
		// Ins
		break;
	case 52:
		// Del
		break;
	case 54:
		// PgUp
		break;
	case 55:
		// PgDown
		break;

	// Shift Navigation
	case 56:
		// Up
		break;
	case 57:
		// Down
		break;
	case 58:
		// Right
		break;
	case 59:
		// Left
		break;
	case 61:
		// End
		break;
	case 63:
		// Pos1
		break;

	// Ctrl Navigation
	case 64:
		// Up
		break;
	case 65:
		// Down
		break;
	case 66:
		// Right
		break;
	case 67:
		// Left
		break;
	case 68:
		// Center
		break;
	case 69:
		// End
		break;
	case 71:
		// Pos1
		break;

	// ALT Navigation
	case 72:
		// Up
		break;
	case 73:
		// Down
		break;
	case 74:
		// Right
		break;
	case 75:
		// Left
		break;
	case 76:
		// Center
		break;
	case 77:
		// End
		break;
	case 79:
		// Pos1
		break;

	// ALT-A - ALT-Z
	case 80:
		// A
		break;
	case 81:
		// B
		break;
	case 82:
		// C
		break;
	case 83:
		// D
		break;
	case 84:
		// E
		break;
	case 85:
		// F
		break;
	case 86:
		// G
		break;
	case 87:
		// H
		break;
	case 88:
		// I
		break;
	case 89:
		// J
		break;
	case 90:
		// K
		break;
	case 91:
		// L
		break;
	case 92:
		// M
		break;
	case 93:
		// N
		break;
	case 94:
		// O
		break;
	case 95:
		// P
		break;
	case 96:
		// Q
		break;
	case 97:
		// R
		break;
	case 98:
		// S
		break;
	case 99:
		// T
		break;
	case 100:
		// U
		break;
	case 101:
		// V
		break;
	case 102:
		// W
		break;
	case 103:
		// X
		break;
	case 104:
		// Y
		break;
	case 105:
		// Z
		break;

	// ShiftALT-A - ShiftALT-Z
	case 129:
		// A
		break;
	case 130:
		// B
		break;
	case 131:
		// C
		break;
	case 132:
		// D
		break;
	case 133:
		// E
		break;
	case 134:
		// F
		break;
	case 135:
		// G
		break;
	case 136:
		// H
		break;
	case 137:
		// I
		break;
	case 138:
		// J
		break;
	case 139:
		// K
		break;
	case 140:
		// L
		break;
	case 141:
		// M
		break;
	case 142:
		// N
		break;
	case 143:
		// O
		break;
	case 144:
		// P
		break;
	case 145:
		// Q
		break;
	case 146:
		// R
		break;
	case 147:
		// S
		break;
	case 148:
		// T
		break;
	case 149:
		// U
		break;
	case 150:
		// V
		break;
	case 151:
		// W
		break;
	case 152:
		// X
		break;
	case 153:
		// Y
		break;
	case 154:
		// Z
		break;

	// 'Single'-Keys
	case 106:
		// Back
		break;
	case 108:
		// ESC
		break;
	case 113:
		// TAB
		break;
	case 114:
		// Shift-TAB
		break;
	case 115:
		// ENTER
		break;
	
	// Answers
	case 107:
		// Cursor Position
		break;
	case 109:
		// Terminal Size
		break;
	case 110:
		// Terminal Icon Label
		break;
	case 111:
		// Terminal Name
		break;

	// Errors 
	case 112:
		// Terminal Error
		break;
	case 155:
		// Len Err By Too Early Following ESC
		break;
	case 156:
		// Position Overflow In Mouse Byte-Mode 
		break;
	case 125:
		// Unknown Mouse Object
		break;

	// 1st Level (From GetESC27()) Mouse-Events
	case 120:
		// Mouse Move
		break;
	case 117:
		// MouseUp
		switch (mouseButton){
		case 1:
			// Left
			break;
		case 4:
			// Right
			break;
		case 2:
			// Wheel
			break;
		default:
			// MultiKey (never seen in reality)
			break;
		}
		break;
	case 116:
		// Left Mouse Down
		break;
	case 121:
		// Left Down Mouse Move
		break;
	case 126:
		// Right Mouse Down
		break;
	case 127:
		// Right Down Mouse Move
		break;
	case 118:
		// Wheel Mouse Down
		break;
	case 122:
		// Wheel Down Mouse Move
		break;
	case 123:
		// WheelScrollUp
		break;
	case 124:
		// WheelScrollDown
		break;

	// 2nd Level (From Loop()) Mouse-Events
	case 255:
	default:
		break;
	}
}

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
	<Terminal.h>	    	01.04.2022		                                Pit

BugID		Description									Found by / Date		Fixed by / Date

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		Bytestream Structure to identify ESC27 Sequences- see: int GetESC27(int)
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
								-------------------------
ID	shift/ctrl	key				0	1	2	3	4	5	6

1				F1				27	79	(80)
2				F2				27	79	(81)
3				F3				27	79	(82)
4				F4				27	79	(83)

5				F5				27	91	49	(53)126

7				F6				27	91	49	(55)126
8				F7				27	91	49	(56)126
9				F8				27	91	49	(57)126

10				F9				27	91	50	(48)126
11				F10				27	91	50	(49)126

13				F11				27	91	50	(51)126
14				F12				27	91	50	(52)126


15		shift	F1				27	91	49	59	50	(80)
16		shift	F2				27	91	49	59	50	(81)
17		shift	F3				27	91	49	59	50	(82)
18		shift	F4				27	91	49	59	50	(83)

19		shift	F5				27	91	49	(53)59	50	126

21		shift	F6				27	91	49	(55)59	50	126
22		shift	F7				27	91	49	(56)59	50	126
23		shift	F8				27	91	49	(57)59	50	126

24		shift	F9				27	91	50	(48)59	50	126
25		shift	F10				27	91	50	(49)59	50	126

27		shift	F11				27	91	50	(51)59	50	126
28		shift	F12				27	91	50	(52)59	50	126


29		ctrl	F1				27	91	49	59	53	(80)
30		ctrl	F2				27	91	49	59	53	(81)
31		ctrl	F3				27	91	49	59	53	(82)
32		ctrl	F4				27	91	49	59	53	(83)

33		ctrl	F5				27	91	49	(53)59	53	126

35		ctrl	F6				27	91	49	(55)59	53	126
36		ctrl	F7				27	91	49	(56)59	53	126
37		ctrl	F8				27	91	49	(57)59	53	126

38		ctrl	F9				27	91	50	(48)59	53	126
39		ctrl	F10				27	91	50	(49)59	53	126

41		ctrl	F11				27	91	50	(51)59	53	126
42		ctrl	F12				27	91	50	(52)59	53	126


43				UP				27	91	(65)
44				DOWN			27	91	(66)
45				Right			27	91	(67)
46				Left			27	91	(68)
47				Center			27	91	(69)
48				END				27	91	(70)

50				Pos1			27	91	(72)

51				Ins				27	91	(50)126
52				Del				27	91	(51)126

54				PgUp			27	91	(53)126
55				PgDown			27	91	(54)126

56		shift	UP				27	91	49	59	50	(65)
57		shift	DOWN			27	91	49	59	50	(66)
58		shift	RIGHT			27	91	49	59	50	(67)
59		shift	LEFT			27	91	49	59	50	(68)

61		shift	END				27	91	49	59	50	(70)
63		shift	Pos1			27	91	49	59	50	(72)
				
64		ctrl	UP				27	91	49	59	53	(65)
65		ctrl	DOWN			27	91	49	59	53	(66)
66		ctrl	RIGHT			27	91	49	59	53	(67)
67		ctrl	LEFT			27	91	49	59	53	(68)
68		ctrl	Center			27	91	49	59	53	(69)
69		ctrl	END				27	91	49	59	53	(70)

71		ctrl	Pos1			27	91	49	59	53	(72)		

72		alt		UP				27	91	49	59	50	(65)
73		alt		DOWN			27	91	49	59	50	(66)
74		alt		RIGHT			27	91	49	59	50	(67)
75		alt		LEFT			27	91	49	59	50	(68)
76		alt		Center			27	91	49	59	50	(69)
77		alt		END				27	91	49	59	50	(70)

79		alt		Pos1			27	91	49	59	50	(72)
		
80-105	alt		A-Z				27	97-122

106				Back			127
		
107				Cursor			27	91	Yrows	59	Xcolumns	82

108				UsrESC			27	(with an timeout - function in progress...)

109				TermSize		27	91	Yrows	59	Xcolumns	116
110				TermIconLabel	27	93	76	ABCtxt	27	92
111				TermWinName		27	93	108	ABCtxt	27	92
112				TermERR			27	91	unknown

113				TAB				9
114		shift	TAB				27	91	90

115				ENTER			10(13)

116				MouseDown		27	91	60	(id-0)	59	Xcolumn	59	Yrow	77
117				MouseUp			27	91	60	(id-0)	59	Xcolumn	59	Yrow	109
118				WheelDown		27	91	60	(id-1)	59	Xcolumn	59	Yrow	77
119				WheelUp			27	91	60	(id-1)	59	Xcolumn	59	Yrow	109
120				MouseMove		27	91	60	(id-35)	59	Xcolumn	59	Yrow	77
121				MouseDownMove	27	91	60	(id-32)	59	Xcolumn	59	Yrow	77
122				WheelDownMove	27	91	60	(id-33)	59	Xcolumn	59	Yrow	77
123				ScrollUp		27	91	60	(id-64)	59	Xcolumn	59	Yrow	77
124				ScrollDown		27	91	60	(id-65)	59	Xcolumn	59	Yrow	77
125				UMO				27	91	60	Unknown Mouse Object	77 / 109

Mouse in ByteMode (e.g. LX-Terminal)
116				MouseDown		27	91	77	32/34	Xcolumn		Yrow
117				MouseUp			27	91	77	35		Xcolumn		Yrow
118				WheelDown		27	91	77	33		Xcolumn		Yrow
119		NA 		Cause is same with MouseUp
120				MouseMove		27	91	77	67		Xcolumn		Yrow
121				MouseDownMove	27	91	77	64/66	Xcolumn		Yrow
122				WheelDownMove	27	91	77	65		Xcolumn		Yrow
123				ScrollUp		27	91	77	96		Xcolumn		Yrow
124				ScrollDown		27	91	77	97		Xcolumn		Yrow
125				UMO				27	91	77	UMO		Xcolumn		Yrow


Added Right Mouse
126				RMouseDown		27	91	77	34		Xcolumn		Yrow
127				RMouseDownMove	27	91	77	66		Xcolumn		Yrow
128				RMouseUp		27	91	77	66		Xcolumn		Yrow

Added ShiftAlt A-Z
129-155	ShiftAlt	a-z			27	65-90

++++++++Bytestream Structure to identify ESC27 sequences - see: int GetESC27(int)+++++++

*/
