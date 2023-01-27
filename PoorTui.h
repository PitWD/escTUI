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

typedef struct TuiSizeSTRUCT{
	int leftDef;
	int topDef;
	int widthDef;
	int heightDef;
	int left;
	int top;
	int width;
	int height;
}TuiSizeSTRUCT;

typedef struct TuiAreaCaptionSTRUCT{
	char *strCaption;
	EscColorSTRUCT *color;
	EscStyleSTRUCT *style;
	int alignment;
}TuiAreaCaptionSTRUCT;

typedef struct TuiFrameSTRUCT{
	// Frames-Lines
	union Elemets{
		uint8_t top 		:1;
		uint8_t bottom 		:1;
		uint8_t left 		:1;
		uint8_t right 		:1;
		uint8_t topLeft 	:1;
		uint8_t topRight 	:1;
		uint8_t bottomLeft 	:1;
		uint8_t bottomRight :1;
	}elements;
	EscColorSTRUCT *color;
	EscStyleSTRUCT *style;
	EscColorSTRUCT *innerColor;
	EscStyleSTRUCT *innerStyle;
	
	// In overlapping cases
		// 0 = we get ignored
		// 1 = we're transparent
		// 2 = we "bitwise OR"
		// 3 = we "space" the frame with style of inner area
	union DrawSTYLE{
		uint8_t top 	:2;
		uint8_t bottom 	:2;
		uint8_t left 	:2;
		uint8_t right 	:2;
	}drawStyle;
	
}TuiFrameSTRUCT;

typedef struct TuiFlexAreaSTRUCT{
	TuiAreaCaptionSTRUCT caption;
	TuiSizeSTRUCT size;

	TuiFrameSTRUCT frameDef;

};

typedef struct TuiFixedAreaSTRUCT{
	// Which Areas are the dominant ones in the edges of the screen?
	// 0 = the horizontal one
	// 1 = the vertical one
	union Dominant{
		uint8_t TopLeft		:1;
		uint8_t TopRight	:1;
		uint8_t BottomRight	:1;
		uint8_t BottomLeft	:1;
	}dominant;
	
	// does the dominant one overlap the other, or just touch?
	union Overlap{
		uint8_t overlapTopLeft		:1;
		uint8_t overlapTopRight		:1;
		uint8_t overlapBottomRight	:1;
		uint8_t overlapBottomLeft	:1;
	}overlap;
	

	// Heights and Widths (0 disables the areas)
	int heightTop;
	int heightBottom;
	int widthLeft;
	int widthRight;
	// Height of Top and Bottom Areas can grow when terminal-size shrinks below
	// definition size to keep count of chars >= definition size.
	int heightMaxTop;
	int heightMaxBottom;
	int charsMinTop;		// Not part of INI, get's calculated on INI read.
	int charsMinBottom;		// Not part of INI, get's calculated on INI read.

	TuiAreaCaptionSTRUCT captionTop;
	TuiAreaCaptionSTRUCT captionBottom;
	TuiAreaCaptionSTRUCT captionLeft;
	TuiAreaCaptionSTRUCT captionRight;

	TuiFrameSTRUCT frameTop;
	TuiFrameSTRUCT frameTBottom;
	TuiFrameSTRUCT frameLeft;
	TuiFrameSTRUCT frameRight;
	
}TuiFixedAreaSTRUCT;

typedef struct TuiFixedAreasSTRUCT_OLD{
	// Area-Captions
	char *strCaptionTop;
	char *strCaptionBottom;
	char *strCaptionLeft;
	char *strCaptionRight;
	EscColorSTRUCT *colorTopCaption;
	EscColorSTRUCT *colorBottomCaption;
	EscColorSTRUCT *colorLeftCaption;
	EscColorSTRUCT *colorRightCaption;
	EscStyleSTRUCT *styleTopCaption;
	EscStyleSTRUCT *styleBottomCaption;
	EscStyleSTRUCT *styleLeftCaption;
	EscStyleSTRUCT *styleRightCaption;
	// 0 = don't, 1 = Left, 2 = Center, 3 = Right
	int locationTopCaption;
	int locationBottomCaption;
	int locationLeftCaption;
	int locationRightCaption;
	
	// Heights and Widths (0 disables the areas)
	int heightTop;
	int heightBottom;
	int widthLeft;
	int widthRight;
	// Height of Top and Bottom Areas can grow when terminal-size shrinks below
	// definition size to keep count of chars >= definition size.
	int heightMaxTop;
	int heightMaxBottom;
	int charsMinTop;		// Not part of INI, get's calculated on INI read.
	int charsMinBottom;		// Not part of INI, get's calculated on INI read.
	
	// Which Areas are the dominant ones in the edges of the screen?
	// 0 = the horizontal one
	// 1 = the vertical one
	int dominantTopLeft;
	int dominantTopRight;
	int dominantBottomRight;
	int dominantBottomLeft;
	// does the dominant one overlap the other, or just touch?
	int overlapTopLeft;
	int overlapTopRight;
	int overlapBottomRight;
	int overlapBottomLeft;
	
	// Frames-Lines
	// 0 = None, 1 = Top, 2 = Bottom, 4 = Left, 8 = Right
	int linesTop;
	int linesBottom;
	int linesLeft;
	int linesRight;
	// Frames-Edges
	// 0 = None, 1 = TopLeft, 2 = TopRight, 4 = BottomRight, 8 BottomLeft
	int edgesTop;
	int edgesBottom;
	int edgesLeft;
	int edgesRight;
	
	// Frames-Colors
	EscColorSTRUCT *colorTop;
	EscColorSTRUCT *colorBottom;
	EscColorSTRUCT *colorLeft;
	EscColorSTRUCT *colorRight;
	// Frames-txtStyle
	EscStyleSTRUCT *styleTop;
	EscStyleSTRUCT *styleBottom;
	EscStyleSTRUCT *styleLeft;
	EscStyleSTRUCT *styleRight;
	// Areas Inner-Color
	EscColorSTRUCT *innerColorTop;
	EscColorSTRUCT *innerColorBottom;
	EscColorSTRUCT *innerColorLeft;
	EscColorSTRUCT *innerColorRight;
	// Areas Inner-Style
	EscStyleSTRUCT *innerStyleTop;
	EscStyleSTRUCT *innerStyleBottom;
	EscStyleSTRUCT *innerStyleLeft;
	EscStyleSTRUCT *innerStyleRight;
	
};

/*

typedef struct TabStyleSTRUCT {
    unsigned char Left;                                     // unscaled / unmoved start of Tab
    unsigned char Width;
    TxtStyleSTRUCT *pStyleEnabledOdd;
	TxtStyleSTRUCT *pStyleDisabledOdd;          
    TxtStyleSTRUCT *pStyleEnabledEven;
	TxtStyleSTRUCT *pStyleDisabledEven;          
	struct TabStyleSTRUCT *NextTab;
    unsigned char FixedLeft :1;
    unsigned char FixedWidth :1;
    unsigned char AddFrontSpace :1;
    unsigned char AddBackSpace :1;
    unsigned char Align :2;									// 0 = Left  |  1 = Center  |  2 = Right
} TabStyleSTRUCT;

typedef struct {
	// 
	TxtStyleSTRUCT *pTxtEnabled;
	TxtStyleSTRUCT *pTxtDisabled;
	char *pHorzLineChar, *pVertLineChar;				    //      or Null for disabling the couple
	char *pTopLeftChar, *pTopRightChar;
	char *pBottomLeftChar, *pBottomRightChar;
	unsigned char TopLine :1;								// If line get drawn (if not both disabled (see above))
	unsigned char BottomLine :1;							// ""  ""   "    ""
	unsigned char LeftLine :1;								// ""  ""   "    ""
	unsigned char RightLine :1;								// ""  ""   "    ""
} FrameStyleSTRUCT;

typedef struct {
	// 5 (10) Byte
	TxtStyleSTRUCT *pTxtEnabledID;
	TxtStyleSTRUCT *pTxtDisabledID;
	char *pScrollHorzCharID, *pScrollVertCharID, *pScrollBlankCharID;
} ScrollStyleSTRUCT;

struct AreaSTRUCT {
	// at least xx (xx) Byte

	struct AreaSTRUCT *pParentArea;							// (Null = Terminal)
	
															// If PopUp has Width = 0 Then
															//		FullScreen
															// else
															// Centered

	unsigned char TopBot, LeftRight, Height, Width;			// Virtual(vectorial) Position and Dimensions
															// Safest way is to do the virtial screen design 
															// in the smallest to exepect real-size.
															// So, 255 x 255 is far enough for the Definition
															   
	unsigned char MinHeight, MinWidth;						// If reality gets smaller - use scroll-bars

// Frame
	FrameStyleSTRUCT *pFrameStyle;    	        			// or NULL for frameless area
// Scrollbars
	ScrollStyleSTRUCT *pScrollStyle;						// or Null for both all time disabled
// Color & Text Style	
	TxtStyleSTRUCT *pTxtEnabled;
	TxtStyleSTRUCT *pTxtDisabled;

// to calculate visible Area for Child draw
	unsigned int ScrollPosHorz, ScrollPosVert;				// !! must have at least 32 Bit !!

	struct AreaSTRUCT *pNextArea;

// Which edge is Referenz (UpperLeft is standard)
	unsigned char AlignRight :1;
	unsigned char AlignBottom :1;

// Options / Properties (8 Byte)
	unsigned char HotKey :6;								// To make Area the active area
															//	see list in AnsiESC.h
															
	unsigned char HotKeyShow :2;							//  0		= Don't show
															//	1 		= Left
															//	2		= Center
															//	3		= Right

// ScrollBars to Draw:
	unsigned char ScrollX :2;								// 0 = Never | 1 = Automatic | 2 = Ever
	unsigned char ScrollY :2;								// 0 = Never | 1 = Automatic | 2 = Ever

//	How To Scale & Move										// can fix areas on reference edge
	unsigned char MoveX :1;
	unsigned char MoveY :1;
	unsigned char ScaleX :1;
	unsigned char ScaleY :1;

//  PopUp Options (PopUps - of course using Scale & Move Options, too)
    unsigned char IsPopUp :1;
    unsigned char IsMaximized :1;                           
    unsigned char IsCentered :1;

 };

typedef struct {

	// 7 Byte
	unsigned char *pTxt;                                    // (1st) Text to print 
	unsigned char TxtCnt;                                   // Lines to print (0 = All (if more than 255))
	unsigned char *pArea;									// Parent area
	unsigned char *pTxtStyle;
	unsigned char VerticalAlign :4;							//	0 = On Frame
															//	1 = 1st Line
															//	2 = 2nd Line
															//	3 = Pre Last Line
															//	4 = Last Line
															//	5 = Center
															//	6 = Next
															//	7 = Next (with space-line in front)
															
	unsigned char HorizontalAlign :4;						//	0 = On Frame
															//	1 = Left 
															//	2 = Left (with space in front)
															//	3 = Right (on Frame)
															//	4 = Right
															//	5 = Right (with added space)
															//	6 = (Next) Tab
} TextLineSTRUCT; 


*/





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

STRUCTURES		            Creation Date   Alpha Date      BetaDate        Creator
    AreaStruct              03.04.2022                                      Pit
    ScrollStyleSTRUCT       04.04.2022                                      Pit
    TextLineStruct          04.04.2022                                      Pit
    FrameStyleSTRUCT        05.04.2022                                      Pit
    TabStyleSTRUCT          07.04.2022                                      Pit

Files			            Creation Date   Alpha Date      BetaDate        Creator
    <PoorTui.h>		        01.04.2022		                                Pit

BugID		Description									Found by / Date		Fixed by / Date
*/


