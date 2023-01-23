#pragma once

//		        ! UTF-8 Symbols - See detailed description at the EOF !
//				! Using 3rd party MIT/GPL/"FreeSoftware" technologies !
//				  (see related declarations/functions and at the EOF)
// This is a  ! p_o_o_r !  Header by (c) Pit Demmer. (PIT-Licensed 26.01.2022 -           )


#include<stdio.h>

struct String3Utf8STRUCT {
	char str[3];
};
struct String4Utf8STRUCT {
	char str[4];
};
struct String6Utf8STRUCT {
	char str[6];
};

struct String6Utf8STRUCT symbolModify[6];
struct String4Utf8STRUCT symbolSkin[103];

struct String3Utf8STRUCT symbolBoxDraw[129];
struct String3Utf8STRUCT symbolGeometric[97];
struct String3Utf8STRUCT symbolArrows[113];
struct String3Utf8STRUCT symbolPowerLine[8];
struct String3Utf8STRUCT symbolPowerLineXtra[32];

struct String3Utf8STRUCT symbolPunctuation[99];
struct String3Utf8STRUCT symbolCurrency[33];
struct String3Utf8STRUCT symbolLetterlike[81];
struct String3Utf8STRUCT symbolMath[257];
struct String3Utf8STRUCT symbolBlock[33];
struct String3Utf8STRUCT symbolMisc[257];

struct String3Utf8STRUCT symbolDingbats[192];
struct String4Utf8STRUCT symbolEmoji[1089];
struct String4Utf8STRUCT symbolSmiley[83];

void InitUtf8(void) {

	/*
		All Symbols
		extracted from: https://www.w3schools.com
		(Thanks a lot guys!)
	*/

	// Skin Tone Modifier
	strcpy(symbolModify[0].str, "\u261D");// No SKin Tone
	strcpy(symbolModify[1].str, symbolModify[0].str);
	strcat(symbolModify[1].str, "\u01F3");
	strcpy(symbolModify[2].str, symbolModify[1].str);
	strcpy(symbolModify[3].str, symbolModify[1].str);
	strcpy(symbolModify[4].str, symbolModify[1].str);
	strcpy(symbolModify[5].str, symbolModify[1].str);
	strcat(symbolModify[1].str, "\xFB");	// Light
	strcat(symbolModify[2].str, "\xFC");	// Medium Light
	strcat(symbolModify[3].str, "\xFD");	// Medium
	strcat(symbolModify[4].str, "\xFE");	// Medium Dark
	strcat(symbolModify[5].str, "\xFF");	// Dark

	// emojis with skin tone capabilities
	strcpy(symbolSkin[1].str, "☝");		// \u261D	
	strcpy(symbolSkin[2].str, "⛹");		// \u26F9	
	strcpy(symbolSkin[3].str, "✊");		// \u270A	
	strcpy(symbolSkin[4].str, "✋");		// \u270B	
	strcpy(symbolSkin[5].str, "✌");		// \u270C	
	strcpy(symbolSkin[6].str, "✍");		// \u270D	
	strcpy(symbolSkin[7].str, "🎅");	// \u1F385	
	strcpy(symbolSkin[8].str, "🏂");	// \u1F3C2	
	strcpy(symbolSkin[9].str, "🏃");	// \u1F3C3	
	strcpy(symbolSkin[10].str, "🏄");	// \u1F3C4	
	strcpy(symbolSkin[11].str, "🏇");	// \u1F3C7	
	strcpy(symbolSkin[12].str, "🏊");	// \u1F3CA	
	strcpy(symbolSkin[13].str, "🏋");	// \u1F3CB	
	strcpy(symbolSkin[14].str, "🏌");	// \u1F3CC	
	strcpy(symbolSkin[15].str, "👂");	// \u1F442	
	strcpy(symbolSkin[16].str, "👃");	// \u1F443	
	strcpy(symbolSkin[17].str, "👆");	// \u1F446	
	strcpy(symbolSkin[18].str, "👇");	// \u1F447	
	strcpy(symbolSkin[19].str, "👈");	// \u1F448	
	strcpy(symbolSkin[20].str, "👉");	// \u1F449	
	strcpy(symbolSkin[21].str, "👊");	// \u1F44A	
	strcpy(symbolSkin[22].str, "👋");	// \u1F44B	
	strcpy(symbolSkin[23].str, "👌");	// \u1F44C	
	strcpy(symbolSkin[24].str, "👍");	// \u1F44D	
	strcpy(symbolSkin[25].str, "👎");	// \u1F44E	
	strcpy(symbolSkin[26].str, "👏");	// \u1F44F	
	strcpy(symbolSkin[27].str, "👐");	// \u1F450	
	strcpy(symbolSkin[28].str, "👦");	// \u1F466	
	strcpy(symbolSkin[29].str, "👧");	// \u1F467	
	strcpy(symbolSkin[30].str, "👨");	// \u1F468	
	strcpy(symbolSkin[31].str, "👩");	// \u1F469	
	strcpy(symbolSkin[32].str, "👮");	// \u1F46E	
	strcpy(symbolSkin[33].str, "👰");	// \u1F470	
	strcpy(symbolSkin[34].str, "👱");	// \u1F471	
	strcpy(symbolSkin[35].str, "👲");	// \u1F472	
	strcpy(symbolSkin[36].str, "👳");	// \u1F473	
	strcpy(symbolSkin[37].str, "👴");	// \u1F474	
	strcpy(symbolSkin[38].str, "👵");	// \u1F475	
	strcpy(symbolSkin[39].str, "👶");	// \u1F476	
	strcpy(symbolSkin[40].str, "👷");	// \u1F477	
	strcpy(symbolSkin[41].str, "👸");	// \u1F478	
	strcpy(symbolSkin[42].str, "👼");	// \u1F47C	
	strcpy(symbolSkin[43].str, "💁");	// \u1F481	
	strcpy(symbolSkin[44].str, "💂");	// \u1F482	
	strcpy(symbolSkin[45].str, "💃");	// \u1F483	
	strcpy(symbolSkin[46].str, "💅");	// \u1F485	
	strcpy(symbolSkin[47].str, "💆");	// \u1F486	
	strcpy(symbolSkin[48].str, "💇");	// \u1F487	
	strcpy(symbolSkin[49].str, "💪");	// \u1F4AA	
	strcpy(symbolSkin[50].str, "🕴");	// \u1F574	
	strcpy(symbolSkin[51].str, "🕵");	// \u1F575	
	strcpy(symbolSkin[52].str, "🕺");	// \u1F57A	
	strcpy(symbolSkin[53].str, "🖐");	// \u1F590	
	strcpy(symbolSkin[54].str, "🖕");	// \u1F595	
	strcpy(symbolSkin[55].str, "🖖");	// \u1F596	
	strcpy(symbolSkin[56].str, "🙅");	// \u1F645	
	strcpy(symbolSkin[57].str, "🙆");	// \u1F646	
	strcpy(symbolSkin[58].str, "🙇");	// \u1F647	
	strcpy(symbolSkin[59].str, "🙋");	// \u1F64B	
	strcpy(symbolSkin[60].str, "🙌");	// \u1F64C	
	strcpy(symbolSkin[61].str, "🙍");	// \u1F64D	
	strcpy(symbolSkin[62].str, "🙎");	// \u1F64E	
	strcpy(symbolSkin[63].str, "🙏");	// \u1F64F	
	strcpy(symbolSkin[64].str, "🚣");	// \u1F6A3	
	strcpy(symbolSkin[65].str, "🚴");	// \u1F6B4	
	strcpy(symbolSkin[66].str, "🚵");	// \u1F6B5	
	strcpy(symbolSkin[67].str, "🚶");	// \u1F6B6	
	strcpy(symbolSkin[68].str, "🛀");	// \u1F6C0	
	strcpy(symbolSkin[69].str, "🛌");	// \u1F6CC	
	strcpy(symbolSkin[70].str, "🤘");	// \u1F918	
	strcpy(symbolSkin[71].str, "🤙");	// \u1F919	
	strcpy(symbolSkin[72].str, "🤚");	// \u1F91A	
	strcpy(symbolSkin[73].str, "🤛");	// \u1F91B	
	strcpy(symbolSkin[74].str, "🤜");	// \u1F91C	
	strcpy(symbolSkin[75].str, "🤝");	// \u1F91D	
	strcpy(symbolSkin[76].str, "🤞");	// \u1F91E	
	strcpy(symbolSkin[77].str, "🤟");	// \u1F91F	
	strcpy(symbolSkin[78].str, "🤦");	// \u1F926	
	strcpy(symbolSkin[79].str, "🤰");	// \u1F930	
	strcpy(symbolSkin[80].str, "🤱");	// \u1F931	
	strcpy(symbolSkin[81].str, "🤲");	// \u1F932	
	strcpy(symbolSkin[82].str, "🤳");	// \u1F933	
	strcpy(symbolSkin[83].str, "🤴");	// \u1F934	
	strcpy(symbolSkin[84].str, "🤵");	// \u1F935	
	strcpy(symbolSkin[85].str, "🤶");	// \u1F936	
	strcpy(symbolSkin[86].str, "🤷");	// \u1F937	
	strcpy(symbolSkin[87].str, "🤸");	// \u1F938	
	strcpy(symbolSkin[88].str, "🤹");	// \u1F939	
	strcpy(symbolSkin[89].str, "🤽");	// \u1F93D	
	strcpy(symbolSkin[90].str, "🤾");	// \u1F93E	
	strcpy(symbolSkin[91].str, "🧑");	// \u1F9D1	
	strcpy(symbolSkin[92].str, "🧒");	// \u1F9D2	
	strcpy(symbolSkin[93].str, "🧓");	// \u1F9D3	
	strcpy(symbolSkin[94].str, "🧔");	// \u1F9D4	
	strcpy(symbolSkin[95].str, "🧕");	// \u1F9D5	
	strcpy(symbolSkin[96].str, "🧖");	// \u1F9D6	
	strcpy(symbolSkin[97].str, "🧗");	// \u1F9D7	
	strcpy(symbolSkin[98].str, "🧘");	// \u1F9D8	
	strcpy(symbolSkin[99].str, "🧙");	// \u1F9D9	
	strcpy(symbolSkin[100].str, "🧚");	// \u1F9DA	
	strcpy(symbolSkin[101].str, "🧜");	// \u1F9DC	
	strcpy(symbolSkin[102].str, "🧝");	// \u1F9DD	

	
	// Box Drawing Elements
	strcpy(symbolBoxDraw[1].str, "─");		// \u2500	BOX DRAWINGS LIGHT HORIZONTAL
	strcpy(symbolBoxDraw[2].str, "━");		// \u2501	BOX DRAWINGS HEAVY HORIZONTAL
	strcpy(symbolBoxDraw[3].str, "│");		// \u2502	BOX DRAWINGS LIGHT VERTICAL
	strcpy(symbolBoxDraw[4].str, "┃");		// \u2503	BOX DRAWINGS HEAVY VERTICAL
	strcpy(symbolBoxDraw[5].str, "┄");		// \u2504	BOX DRAWINGS LIGHT TRIPLE DASH HORIZONTAL
	strcpy(symbolBoxDraw[6].str, "┅");		// \u2505	BOX DRAWINGS HEAVY TRIPLE DASH HORIZONTAL
	strcpy(symbolBoxDraw[7].str, "┆");		// \u2506	BOX DRAWINGS LIGHT TRIPLE DASH VERTICAL
	strcpy(symbolBoxDraw[8].str, "┇");		// \u2507	BOX DRAWINGS HEAVY TRIPLE DASH VERTICAL
	strcpy(symbolBoxDraw[9].str, "┈");		// \u2508	BOX DRAWINGS LIGHT QUADRUPLE DASH HORIZONTAL
	strcpy(symbolBoxDraw[10].str, "┉");		// \u2509	BOX DRAWINGS HEAVY QUADRUPLE DASH HORIZONTAL
	strcpy(symbolBoxDraw[11].str, "┊");		// \u250A	BOX DRAWINGS LIGHT QUADRUPLE DASH VERTICAL
	strcpy(symbolBoxDraw[12].str, "┋");		// \u250B	BOX DRAWINGS HEAVY QUADRUPLE DASH VERTICAL
	strcpy(symbolBoxDraw[13].str, "┌");		// \u250C	BOX DRAWINGS LIGHT DOWN AND RIGHT
	strcpy(symbolBoxDraw[14].str, "┍");		// \u250D	BOX DRAWINGS DOWN LIGHT AND RIGHT HEAVY
	strcpy(symbolBoxDraw[15].str, "┎");		// \u250E	BOX DRAWINGS DOWN HEAVY AND RIGHT LIGHT
	strcpy(symbolBoxDraw[16].str, "┏");		// \u250F	BOX DRAWINGS HEAVY DOWN AND RIGHT
	strcpy(symbolBoxDraw[17].str, "┐");		// \u2510	BOX DRAWINGS LIGHT DOWN AND LEFT
	strcpy(symbolBoxDraw[18].str, "┑");		// \u2511	BOX DRAWINGS DOWN LIGHT AND LEFT HEAVY
	strcpy(symbolBoxDraw[19].str, "┒");		// \u2512	BOX DRAWINGS DOWN HEAVY AND LEFT LIGHT
	strcpy(symbolBoxDraw[20].str, "┓");		// \u2513	BOX DRAWINGS HEAVY DOWN AND LEFT
	strcpy(symbolBoxDraw[21].str, "└");		// \u2514	BOX DRAWINGS LIGHT UP AND RIGHT
	strcpy(symbolBoxDraw[22].str, "┕");		// \u2515	BOX DRAWINGS UP LIGHT AND RIGHT HEAVY
	strcpy(symbolBoxDraw[23].str, "┖");		// \u2516	BOX DRAWINGS UP HEAVY AND RIGHT LIGHT
	strcpy(symbolBoxDraw[24].str, "┗");		// \u2517	BOX DRAWINGS HEAVY UP AND RIGHT
	strcpy(symbolBoxDraw[25].str, "┘");		// \u2518	BOX DRAWINGS LIGHT UP AND LEFT
	strcpy(symbolBoxDraw[26].str, "┙");		// \u2519	BOX DRAWINGS UP LIGHT AND LEFT HEAVY
	strcpy(symbolBoxDraw[27].str, "┚");		// \u251A	BOX DRAWINGS UP HEAVY AND LEFT LIGHT
	strcpy(symbolBoxDraw[28].str, "┛");		// \u251B	BOX DRAWINGS HEAVY UP AND LEFT
	strcpy(symbolBoxDraw[29].str, "├");		// \u251C	BOX DRAWINGS LIGHT VERTICAL AND RIGHT
	strcpy(symbolBoxDraw[30].str, "┝");		// \u251D	BOX DRAWINGS VERTICAL LIGHT AND RIGHT HEAVY
	strcpy(symbolBoxDraw[31].str, "┞");		// \u251E	BOX DRAWINGS UP HEAVY AND RIGHT DOWN LIGHT
	strcpy(symbolBoxDraw[32].str, "┟");		// \u251F	BOX DRAWINGS DOWN HEAVY AND RIGHT UP LIGHT
	strcpy(symbolBoxDraw[33].str, "┠");		// \u2520	BOX DRAWINGS VERTICAL HEAVY AND RIGHT LIGHT
	strcpy(symbolBoxDraw[34].str, "┡");		// \u2521	BOX DRAWINGS DOWN LIGHT AND RIGHT UP HEAVY
	strcpy(symbolBoxDraw[35].str, "┢");		// \u2522	BOX DRAWINGS UP LIGHT AND RIGHT DOWN HEAVY
	strcpy(symbolBoxDraw[36].str, "┣");		// \u2523	BOX DRAWINGS HEAVY VERTICAL AND RIGHT
	strcpy(symbolBoxDraw[37].str, "┤");		// \u2524	BOX DRAWINGS LIGHT VERTICAL AND LEFT
	strcpy(symbolBoxDraw[38].str, "┥");		// \u2525	BOX DRAWINGS VERTICAL LIGHT AND LEFT HEAVY
	strcpy(symbolBoxDraw[39].str, "┦");		// \u2526	BOX DRAWINGS UP HEAVY AND LEFT DOWN LIGHT
	strcpy(symbolBoxDraw[40].str, "┧");		// \u2527	BOX DRAWINGS DOWN HEAVY AND LEFT UP LIGHT
	strcpy(symbolBoxDraw[41].str, "┨");		// \u2528	BOX DRAWINGS VERTICAL HEAVY AND LEFT LIGHT
	strcpy(symbolBoxDraw[42].str, "┩");		// \u2529	BOX DRAWINGS DOWN LIGHT AND LEFT UP HEAVY
	strcpy(symbolBoxDraw[43].str, "┪");		// \u252A	BOX DRAWINGS UP LIGHT AND LEFT DOWN HEAVY
	strcpy(symbolBoxDraw[44].str, "┫");		// \u252B	BOX DRAWINGS HEAVY VERTICAL AND LEFT
	strcpy(symbolBoxDraw[45].str, "┬");		// \u252C	BOX DRAWINGS LIGHT DOWN AND HORIZONTAL
	strcpy(symbolBoxDraw[46].str, "┭");		// \u252D	BOX DRAWINGS LEFT HEAVY AND RIGHT DOWN LIGHT
	strcpy(symbolBoxDraw[47].str, "┮");		// \u252E	BOX DRAWINGS RIGHT HEAVY AND LEFT DOWN LIGHT
	strcpy(symbolBoxDraw[48].str, "┯");		// \u252F	BOX DRAWINGS DOWN LIGHT AND HORIZONTAL HEAVY
	strcpy(symbolBoxDraw[49].str, "┰");		// \u2530	BOX DRAWINGS DOWN HEAVY AND HORIZONTAL LIGHT
	strcpy(symbolBoxDraw[50].str, "┱");		// \u2531	BOX DRAWINGS RIGHT LIGHT AND LEFT DOWN HEAVY
	strcpy(symbolBoxDraw[51].str, "┲");		// \u2532	BOX DRAWINGS LEFT LIGHT AND RIGHT DOWN HEAVY
	strcpy(symbolBoxDraw[52].str, "┳");		// \u2533	BOX DRAWINGS HEAVY DOWN AND HORIZONTAL
	strcpy(symbolBoxDraw[53].str, "┴");		// \u2534	BOX DRAWINGS LIGHT UP AND HORIZONTAL
	strcpy(symbolBoxDraw[54].str, "┵");		// \u2535	BOX DRAWINGS LEFT HEAVY AND RIGHT UP LIGHT
	strcpy(symbolBoxDraw[55].str, "┶");		// \u2536	BOX DRAWINGS RIGHT HEAVY AND LEFT UP LIGHT
	strcpy(symbolBoxDraw[56].str, "┷");		// \u2537	BOX DRAWINGS UP LIGHT AND HORIZONTAL HEAVY
	strcpy(symbolBoxDraw[57].str, "┸");		// \u2538	BOX DRAWINGS UP HEAVY AND HORIZONTAL LIGHT
	strcpy(symbolBoxDraw[58].str, "┹");		// \u2539	BOX DRAWINGS RIGHT LIGHT AND LEFT UP HEAVY
	strcpy(symbolBoxDraw[59].str, "┺");		// \u253A	BOX DRAWINGS LEFT LIGHT AND RIGHT UP HEAVY
	strcpy(symbolBoxDraw[60].str, "┻");		// \u253B	BOX DRAWINGS HEAVY UP AND HORIZONTAL
	strcpy(symbolBoxDraw[61].str, "┼");		// \u253C	BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL
	strcpy(symbolBoxDraw[62].str, "┽");		// \u253D	BOX DRAWINGS LEFT HEAVY AND RIGHT VERTICAL LIGHT
	strcpy(symbolBoxDraw[63].str, "┾");	// \u253E	BOX DRAWINGS RIGHT HEAVY AND LEFT VERTICAL LIGHT
	strcpy(symbolBoxDraw[64].str, "┿");	// \u253F	BOX DRAWINGS VERTICAL LIGHT AND HORIZONTAL HEAVY
	strcpy(symbolBoxDraw[65].str, "╀");	// \u2540	BOX DRAWINGS UP HEAVY AND DOWN HORIZONTAL LIGHT
	strcpy(symbolBoxDraw[66].str, "╁");	// \u2541	BOX DRAWINGS DOWN HEAVY AND UP HORIZONTAL LIGHT
	strcpy(symbolBoxDraw[67].str, "╂");	// \u2542	BOX DRAWINGS VERTICAL HEAVY AND HORIZONTAL LIGHT
	strcpy(symbolBoxDraw[68].str, "╃");	// \u2543	BOX DRAWINGS LEFT UP HEAVY AND RIGHT DOWN LIGHT
	strcpy(symbolBoxDraw[69].str, "╄");	// \u2544	BOX DRAWINGS RIGHT UP HEAVY AND LEFT DOWN LIGHT
	strcpy(symbolBoxDraw[70].str, "╅");	// \u2545	BOX DRAWINGS LEFT DOWN HEAVY AND RIGHT UP LIGHT
	strcpy(symbolBoxDraw[71].str, "╆");	// \u2546	BOX DRAWINGS RIGHT DOWN HEAVY AND LEFT UP LIGHT
	strcpy(symbolBoxDraw[72].str, "╇");	// \u2547	BOX DRAWINGS DOWN LIGHT AND UP HORIZONTAL HEAVY
	strcpy(symbolBoxDraw[73].str, "╈");	// \u2548	BOX DRAWINGS UP LIGHT AND DOWN HORIZONTAL HEAVY
	strcpy(symbolBoxDraw[74].str, "╉");	// \u2549	BOX DRAWINGS RIGHT LIGHT AND LEFT VERTICAL HEAVY
	strcpy(symbolBoxDraw[75].str, "╊");	// \u254A	BOX DRAWINGS LEFT LIGHT AND RIGHT VERTICAL HEAVY
	strcpy(symbolBoxDraw[76].str, "╋");	// \u254B	BOX DRAWINGS HEAVY VERTICAL AND HORIZONTAL
	strcpy(symbolBoxDraw[77].str, "╌");	// \u254C	BOX DRAWINGS LIGHT DOUBLE DASH HORIZONTAL
	strcpy(symbolBoxDraw[78].str, "╍");	// \u254D	BOX DRAWINGS HEAVY DOUBLE DASH HORIZONTAL
	strcpy(symbolBoxDraw[79].str, "╎");	// \u254E	BOX DRAWINGS LIGHT DOUBLE DASH VERTICAL
	strcpy(symbolBoxDraw[80].str, "╏");	// \u254F	BOX DRAWINGS HEAVY DOUBLE DASH VERTICAL
	strcpy(symbolBoxDraw[81].str, "═");	// \u2550	BOX DRAWINGS DOUBLE HORIZONTAL
	strcpy(symbolBoxDraw[82].str, "║");	// \u2551	BOX DRAWINGS DOUBLE VERTICAL
	strcpy(symbolBoxDraw[83].str, "╒");	// \u2552	BOX DRAWINGS DOWN SINGLE AND RIGHT DOUBLE
	strcpy(symbolBoxDraw[84].str, "╓");	// \u2553	BOX DRAWINGS DOWN DOUBLE AND RIGHT SINGLE
	strcpy(symbolBoxDraw[85].str, "╔");	// \u2554	BOX DRAWINGS DOUBLE DOWN AND RIGHT
	strcpy(symbolBoxDraw[86].str, "╕");	// \u2555	BOX DRAWINGS DOWN SINGLE AND LEFT DOUBLE
	strcpy(symbolBoxDraw[87].str, "╖");	// \u2556	BOX DRAWINGS DOWN DOUBLE AND LEFT SINGLE
	strcpy(symbolBoxDraw[88].str, "╗");	// \u2557	BOX DRAWINGS DOUBLE DOWN AND LEFT
	strcpy(symbolBoxDraw[89].str, "╘");	// \u2558	BOX DRAWINGS UP SINGLE AND RIGHT DOUBLE
	strcpy(symbolBoxDraw[90].str, "╙");	// \u2559	BOX DRAWINGS UP DOUBLE AND RIGHT SINGLE
	strcpy(symbolBoxDraw[91].str, "╚");	// \u255A	BOX DRAWINGS DOUBLE UP AND RIGHT
	strcpy(symbolBoxDraw[92].str, "╛");	// \u255B	BOX DRAWINGS UP SINGLE AND LEFT DOUBLE
	strcpy(symbolBoxDraw[93].str, "╜");	// \u255C	BOX DRAWINGS UP DOUBLE AND LEFT SINGLE
	strcpy(symbolBoxDraw[94].str, "╝");	// \u255D	BOX DRAWINGS DOUBLE UP AND LEFT
	strcpy(symbolBoxDraw[95].str, "╞");	// \u255E	BOX DRAWINGS VERTICAL SINGLE AND RIGHT DOUBLE
	strcpy(symbolBoxDraw[96].str, "╟");	// \u255F	BOX DRAWINGS VERTICAL DOUBLE AND RIGHT SINGLE
	strcpy(symbolBoxDraw[97].str, "╠");	// \u2560	BOX DRAWINGS DOUBLE VERTICAL AND RIGHT
	strcpy(symbolBoxDraw[98].str, "╡");	// \u2561	BOX DRAWINGS VERTICAL SINGLE AND LEFT DOUBLE
	strcpy(symbolBoxDraw[99].str, "╢");	// \u2562	BOX DRAWINGS VERTICAL DOUBLE AND LEFT SINGLE
	strcpy(symbolBoxDraw[100].str, "╣");	// \u2563	BOX DRAWINGS DOUBLE VERTICAL AND LEFT
	strcpy(symbolBoxDraw[101].str, "╤");	// \u2564	BOX DRAWINGS DOWN SINGLE AND HORIZONTAL DOUBLE
	strcpy(symbolBoxDraw[102].str, "╥");	// \u2565	BOX DRAWINGS DOWN DOUBLE AND HORIZONTAL SINGLE
	strcpy(symbolBoxDraw[103].str, "╦");	// \u2566	BOX DRAWINGS DOUBLE DOWN AND HORIZONTAL
	strcpy(symbolBoxDraw[104].str, "╧");	// \u2567	BOX DRAWINGS UP SINGLE AND HORIZONTAL DOUBLE
	strcpy(symbolBoxDraw[105].str, "╨");	// \u2568	BOX DRAWINGS UP DOUBLE AND HORIZONTAL SINGLE
	strcpy(symbolBoxDraw[106].str, "╩");	// \u2569	BOX DRAWINGS DOUBLE UP AND HORIZONTAL
	strcpy(symbolBoxDraw[107].str, "╪");	// \u256A	BOX DRAWINGS VERTICAL SINGLE AND HORIZONTAL DOUBLE
	strcpy(symbolBoxDraw[108].str, "╫");	// \u256B	BOX DRAWINGS VERTICAL DOUBLE AND HORIZONTAL SINGLE
	strcpy(symbolBoxDraw[109].str, "╬");	// \u256C	BOX DRAWINGS DOUBLE VERTICAL AND HORIZONTAL
	strcpy(symbolBoxDraw[110].str, "╭");	// \u256D	BOX DRAWINGS LIGHT ARC DOWN AND RIGHT
	strcpy(symbolBoxDraw[111].str, "╮");	// \u256E	BOX DRAWINGS LIGHT ARC DOWN AND LEFT
	strcpy(symbolBoxDraw[112].str, "╯");	// \u256F	BOX DRAWINGS LIGHT ARC UP AND LEFT
	strcpy(symbolBoxDraw[113].str, "╰");	// \u2570	BOX DRAWINGS LIGHT ARC UP AND RIGHT
	strcpy(symbolBoxDraw[114].str, "╱");	// \u2571	BOX DRAWINGS LIGHT DIAGONAL UPPER RIGHT TO LOWER LEFT
	strcpy(symbolBoxDraw[115].str, "╲");	// \u2572	BOX DRAWINGS LIGHT DIAGONAL UPPER LEFT TO LOWER RIGHT
	strcpy(symbolBoxDraw[116].str, "╳");	// \u2573	BOX DRAWINGS LIGHT DIAGONAL CROSS
	strcpy(symbolBoxDraw[117].str, "╴");	// \u2574	BOX DRAWINGS LIGHT LEFT
	strcpy(symbolBoxDraw[118].str, "╵");	// \u2575	BOX DRAWINGS LIGHT UP
	strcpy(symbolBoxDraw[119].str, "╶");	// \u2576	BOX DRAWINGS LIGHT RIGHT
	strcpy(symbolBoxDraw[120].str, "╷");	// \u2577	BOX DRAWINGS LIGHT DOWN
	strcpy(symbolBoxDraw[121].str, "╸");	// \u2578	BOX DRAWINGS HEAVY LEFT
	strcpy(symbolBoxDraw[122].str, "╹");	// \u2579	BOX DRAWINGS HEAVY UP
	strcpy(symbolBoxDraw[123].str, "╺");	// \u257A	BOX DRAWINGS HEAVY RIGHT
	strcpy(symbolBoxDraw[124].str, "╻");	// \u257B	BOX DRAWINGS HEAVY DOWN
	strcpy(symbolBoxDraw[125].str, "╼");	// \u257C	BOX DRAWINGS LIGHT LEFT AND HEAVY RIGHT
	strcpy(symbolBoxDraw[126].str, "╽");	// \u257D	BOX DRAWINGS LIGHT UP AND HEAVY DOWN
	strcpy(symbolBoxDraw[127].str, "╾");	// \u257E	BOX DRAWINGS HEAVY LEFT AND LIGHT RIGHT
	strcpy(symbolBoxDraw[128].str, "╿");	// \u257F	BOX DRAWINGS HEAVY UP AND LIGHT DOWN

	strcpy(symbolGeometric[1].str, "■");		// \u25A0	BLACK SQUARE
	strcpy(symbolGeometric[2].str, "□");	// \u25A1	WHITE SQUARE
	strcpy(symbolGeometric[3].str, "▢");	// \u25A2	WHITE SQUARE WITH ROUNDED CORNERS
	strcpy(symbolGeometric[4].str, "▣");	// \u25A3	WHITE SQUARE CONTAINING BLACK SMALL SQUARE
	strcpy(symbolGeometric[5].str, "▤");	// \u25A4	SQUARE WITH HORIZONTAL FILL
	strcpy(symbolGeometric[6].str, "▥");	// \u25A5	SQUARE WITH VERTICAL FILL
	strcpy(symbolGeometric[7].str, "▦");	// \u25A6	SQUARE WITH ORTHOGONAL CROSSHATCH FILL
	strcpy(symbolGeometric[8].str, "▧");	// \u25A7	SQUARE WITH UPPER LEFT TO LOWER RIGHT FILL
	strcpy(symbolGeometric[9].str, "▨");	// \u25A8	SQUARE WITH UPPER RIGHT TO LOWER LEFT FILL
	strcpy(symbolGeometric[10].str, "▩");	// \u25A9	SQUARE WITH DIAGONAL CROSSHATCH FILL
	strcpy(symbolGeometric[11].str, "▪");	// \u25AA	BLACK SMALL SQUARE
	strcpy(symbolGeometric[12].str, "▫");	// \u25AB	WHITE SMALL SQUARE
	strcpy(symbolGeometric[13].str, "▬");	// \u25AC	BLACK RECTANGLE
	strcpy(symbolGeometric[14].str, "▭");	// \u25AD	WHITE RECTANGLE
	strcpy(symbolGeometric[15].str, "▮");	// \u25AE	BLACK VERTICAL RECTANGLE
	strcpy(symbolGeometric[16].str, "▯");	// \u25AF	WHITE VERTICAL RECTANGLE
	strcpy(symbolGeometric[17].str, "▰");	// \u25B0	BLACK PARALLELOGRAM
	strcpy(symbolGeometric[18].str, "▱");	// \u25B1	WHITE PARALLELOGRAM
	strcpy(symbolGeometric[19].str, "▲");	// \u25B2	BLACK UP-POINTING TRIANGLE
	strcpy(symbolGeometric[20].str, "△");	// \u25B3	WHITE UP-POINTING TRIANGLE
	strcpy(symbolGeometric[21].str, "▴");	// \u25B4	BLACK UP-POINTING SMALL TRIANGLE
	strcpy(symbolGeometric[22].str, "▵");	// \u25B5	WHITE UP-POINTING SMALL TRIANGLE
	strcpy(symbolGeometric[23].str, "▶");	// \u25B6	BLACK RIGHT-POINTING TRIANGLE
	strcpy(symbolGeometric[24].str, "▷");	// \u25B7	WHITE RIGHT-POINTING TRIANGLE
	strcpy(symbolGeometric[25].str, "▸");	// \u25B8	BLACK RIGHT-POINTING SMALL TRIANGLE
	strcpy(symbolGeometric[26].str, "▹");	// \u25B9	WHITE RIGHT-POINTING SMALL TRIANGLE
	strcpy(symbolGeometric[27].str, "►");	// \u25BA	BLACK RIGHT-POINTING POINTER
	strcpy(symbolGeometric[28].str, "▻");	// \u25BB	WHITE RIGHT-POINTING POINTER
	strcpy(symbolGeometric[29].str, "▼");	// \u25BC	BLACK DOWN-POINTING TRIANGLE
	strcpy(symbolGeometric[30].str, "▽");	// \u25BD	WHITE DOWN-POINTING TRIANGLE
	strcpy(symbolGeometric[31].str, "▾");	// \u25BE	BLACK DOWN-POINTING SMALL TRIANGLE
	strcpy(symbolGeometric[32].str, "▿");	// \u25BF	WHITE DOWN-POINTING SMALL TRIANGLE
	strcpy(symbolGeometric[33].str, "◀");	// \u25C0	BLACK LEFT-POINTING TRIANGLE
	strcpy(symbolGeometric[34].str, "◁");	// \u25C1	WHITE LEFT-POINTING TRIANGLE
	strcpy(symbolGeometric[35].str, "◂");	// \u25C2	BLACK LEFT-POINTING SMALL TRIANGLE
	strcpy(symbolGeometric[36].str, "◃");	// \u25C3	WHITE LEFT-POINTING SMALL TRIANGLE
	strcpy(symbolGeometric[37].str, "◄");	// \u25C4	BLACK LEFT-POINTING POINTER
	strcpy(symbolGeometric[38].str, "◅");	// \u25C5	WHITE LEFT-POINTING POINTER
	strcpy(symbolGeometric[39].str, "◆");	// \u25C6	BLACK DIAMOND
	strcpy(symbolGeometric[40].str, "◇");	// \u25C7	WHITE DIAMOND
	strcpy(symbolGeometric[41].str, "◈");	// \u25C8	WHITE DIAMOND CONTAINING BLACK SMALL DIAMOND
	strcpy(symbolGeometric[42].str, "◉");	// \u25C9	FISHEYE
	strcpy(symbolGeometric[43].str, "◊");	// \u25CA	LOZENGE
	strcpy(symbolGeometric[44].str, "○");	// \u25CB	WHITE CIRCLE
	strcpy(symbolGeometric[45].str, "◌");	// \u25CC	DOTTED CIRCLE
	strcpy(symbolGeometric[46].str, "◍");	// \u25CD	CIRCLE WITH VERTICAL FILL
	strcpy(symbolGeometric[47].str, "◎");	// \u25CE	BULLSEYE
	strcpy(symbolGeometric[48].str, "●");	// \u25CF	BLACK CIRCLE
	strcpy(symbolGeometric[49].str, "◐");	// \u25D0	CIRCLE WITH LEFT HALF BLACK
	strcpy(symbolGeometric[50].str, "◑");	// \u25D1	CIRCLE WITH RIGHT HALF BLACK
	strcpy(symbolGeometric[51].str, "◒");	// \u25D2	CIRCLE WITH LOWER HALF BLACK
	strcpy(symbolGeometric[52].str, "◓");	// \u25D3	CIRCLE WITH UPPER HALF BLACK
	strcpy(symbolGeometric[53].str, "◔");	// \u25D4	CIRCLE WITH UPPER RIGHT QUADRANT BLACK
	strcpy(symbolGeometric[54].str, "◕");	// \u25D5	CIRCLE WITH ALL BUT UPPER LEFT QUADRANT BLACK
	strcpy(symbolGeometric[55].str, "◖");	// \u25D6	LEFT HALF BLACK CIRCLE
	strcpy(symbolGeometric[56].str, "◗");	// \u25D7	RIGHT HALF BLACK CIRCLE
	strcpy(symbolGeometric[57].str, "◘");	// \u25D8	INVERSE BULLET
	strcpy(symbolGeometric[58].str, "◙");	// \u25D9	INVERSE WHITE CIRCLE
	strcpy(symbolGeometric[59].str, "◚");	// \u25DA	UPPER HALF INVERSE WHITE CIRCLE
	strcpy(symbolGeometric[60].str, "◛");	// \u25DB	LOWER HALF INVERSE WHITE CIRCLE
	strcpy(symbolGeometric[61].str, "◜");	// \u25DC	UPPER LEFT QUADRANT CIRCULAR ARC
	strcpy(symbolGeometric[62].str, "◝");	// \u25DD	UPPER RIGHT QUADRANT CIRCULAR ARC
	strcpy(symbolGeometric[63].str, "◞");	// \u25DE	LOWER RIGHT QUADRANT CIRCULAR ARC
	strcpy(symbolGeometric[64].str, "◟");	// \u25DF	LOWER LEFT QUADRANT CIRCULAR ARC
	strcpy(symbolGeometric[65].str, "◠");	// \u25	UPPER HALF CIRCLE
	strcpy(symbolGeometric[66].str, "◡");	// \u250	LOWER HALF CIRCLE
	strcpy(symbolGeometric[67].str, "◢");	// \u2500	BLACK LOWER RIGHT TRIANGLE
	strcpy(symbolGeometric[68].str, "◣");	// \u25000	BLACK LOWER LEFT TRIANGLE
	strcpy(symbolGeometric[69].str, "◤");	// \u250000	BLACK UPPER LEFT TRIANGLE
	strcpy(symbolGeometric[70].str, "◥");	// \u2500000	BLACK UPPER RIGHT TRIANGLE
	strcpy(symbolGeometric[71].str, "◦");	// \u25000000	WHITE BULLET
	strcpy(symbolGeometric[72].str, "◧");	// \u250000000	SQUARE WITH LEFT HALF BLACK
	strcpy(symbolGeometric[73].str, "◨");	// \u2500000000	SQUARE WITH RIGHT HALF BLACK
	strcpy(symbolGeometric[74].str, "◩");	// \u25000000000	SQUARE WITH UPPER LEFT DIAGONAL HALF BLACK
	strcpy(symbolGeometric[75].str, "◪");	// \u25EA	SQUARE WITH LOWER RIGHT DIAGONAL HALF BLACK
	strcpy(symbolGeometric[76].str, "◫");	// \u25EB	WHITE SQUARE WITH VERTICAL BISECTING LINE
	strcpy(symbolGeometric[77].str, "◬");	// \u25EC	WHITE UP-POINTING TRIANGLE WITH DOT
	strcpy(symbolGeometric[78].str, "◭");	// \u25ED	UP-POINTING TRIANGLE WITH LEFT HALF BLACK
	strcpy(symbolGeometric[79].str, "◮");	// \u25EE	UP-POINTING TRIANGLE WITH RIGHT HALF BLACK
	strcpy(symbolGeometric[80].str, "◯");	// \u25EF	LARGE CIRCLE
	strcpy(symbolGeometric[81].str, "◰");	// \u25F0	WHITE SQUARE WITH UPPER LEFT QUADRANT
	strcpy(symbolGeometric[82].str, "◱");	// \u25F1	WHITE SQUARE WITH LOWER LEFT QUADRANT
	strcpy(symbolGeometric[83].str, "◲");	// \u25F2	WHITE SQUARE WITH LOWER RIGHT QUADRANT
	strcpy(symbolGeometric[84].str, "◳");	// \u25F3	WHITE SQUARE WITH UPPER RIGHT QUADRANT
	strcpy(symbolGeometric[85].str, "◴");	// \u25F4	WHITE CIRCLE WITH UPPER LEFT QUADRANT
	strcpy(symbolGeometric[86].str, "◵");	// \u25F5	WHITE CIRCLE WITH LOWER LEFT QUADRANT
	strcpy(symbolGeometric[87].str, "◶");	// \u25F6	WHITE CIRCLE WITH LOWER RIGHT QUADRANT
	strcpy(symbolGeometric[88].str, "◷");	// \u25F7	WHITE CIRCLE WITH UPPER RIGHT QUADRANT
	strcpy(symbolGeometric[89].str, "◸");	// \u25F8	UPPER LEFT TRIANGLE
	strcpy(symbolGeometric[90].str, "◹");	// \u25F9	UPPER RIGHT TRIANGLE
	strcpy(symbolGeometric[91].str, "◺");	// \u25FA	LOWER LEFT TRIANGLE
	strcpy(symbolGeometric[92].str, "◻");	// \u25FB	WHITE MEDIUM SQUARE
	strcpy(symbolGeometric[93].str, "◼");	// \u25FC	BLACK MEDIUM SQUARE
	strcpy(symbolGeometric[94].str, "◽");	// \u25FD	WHITE MEDIUM SMALL SQUARE
	strcpy(symbolGeometric[95].str, "◾");	// \u25FE	BLACK MEDIUM SMALL SQUARE
	strcpy(symbolGeometric[96].str, "◿");	// \u25FF	LOWER RIGHT TRIANGLE


	strcpy(symbolPunctuation[1].str, " ");		// \u2000	EN QUAD
	strcpy(symbolPunctuation[2].str, " ");		// \u2001	EM QUAD
	strcpy(symbolPunctuation[3].str, " ");		// \u2002	EN SPACE
	strcpy(symbolPunctuation[4].str, " ");		// \u2003	EM SPACE
	strcpy(symbolPunctuation[5].str, " ");		// \u2004	THREE-PER-EM SPACE
	strcpy(symbolPunctuation[6].str, " ");		// \u2005	FOUR-PER-EM SPACE
	strcpy(symbolPunctuation[7].str, " ");		// \u2006	SIX-PER-EM SPACE
	strcpy(symbolPunctuation[8].str, " ");		// \u2007	FIGURE SPACE
	strcpy(symbolPunctuation[9].str, " ");		// \u2008	PUNCTUATION SPACE
	strcpy(symbolPunctuation[10].str, " ");		// \u2009	THIN SPACE
	strcpy(symbolPunctuation[11].str, " ");		// \u200A	HAIR SPACE
	strcpy(symbolPunctuation[12].str, "​");		// \u200B	ZERO WIDTH SPACE
	strcpy(symbolPunctuation[13].str, "‌");		// \u200C	ZERO WIDTH NON-JOINER
	strcpy(symbolPunctuation[14].str, "‍");		// \u200D	ZERO WIDTH JOINER
	strcpy(symbolPunctuation[15].str, "‐");	// \u2010	HYPHEN
	strcpy(symbolPunctuation[16].str, "‑");	// \u2011	NON-BREAKING HYPHEN
	strcpy(symbolPunctuation[17].str, "‒");	// \u2012	FIGURE DASH
	strcpy(symbolPunctuation[18].str, "–");	// \u2013	EN DASH
	strcpy(symbolPunctuation[19].str, "—");	// \u2014	EM DASH
	strcpy(symbolPunctuation[20].str, "―");	// \u2015	HORIZONTAL BAR
	strcpy(symbolPunctuation[21].str, "‖");		// \u2016	DOUBLE VERTICAL LINE
	strcpy(symbolPunctuation[22].str, "‗");	// \u2017	DOUBLE LOW LINE
	strcpy(symbolPunctuation[23].str, "‘");	// \u2018	LEFT SINGLE QUOTATION MARK
	strcpy(symbolPunctuation[24].str, "’");	// \u2019	RIGHT SINGLE QUOTATION MARK
	strcpy(symbolPunctuation[25].str, "‚");	// \u201A	SINGLE LOW-9 QUOTATION MARK
	strcpy(symbolPunctuation[26].str, "‛");	// \u201B	SINGLE HIGH-REVERSED-9 QUOTATION MARK
	strcpy(symbolPunctuation[27].str, "“");	// \u201C	LEFT DOUBLE QUOTATION MARK
	strcpy(symbolPunctuation[28].str, "”");	// \u201D	RIGHT DOUBLE QUOTATION MARK
	strcpy(symbolPunctuation[29].str, "„");	// \u201E	DOUBLE LOW-9 QUOTATION MARK
	strcpy(symbolPunctuation[30].str, "‟");	// \u201F	DOUBLE HIGH-REVERSED-9 QUOTATION MARK
	strcpy(symbolPunctuation[31].str, "†");	// \u2020	DAGGER
	strcpy(symbolPunctuation[32].str, "‡");	// \u2021	DOUBLE DAGGER
	strcpy(symbolPunctuation[33].str, "•");	// \u2022	BULLET
	strcpy(symbolPunctuation[34].str, "‣");		// \u2023	TRIANGULAR BULLET
	strcpy(symbolPunctuation[35].str, "․");	// \u2024	ONE DOT LEADER
	strcpy(symbolPunctuation[36].str, "‥");		// \u2025	TWO DOT LEADER
	strcpy(symbolPunctuation[37].str, "…");	// \u2026	HORIZONTAL ELLIPSIS
	strcpy(symbolPunctuation[38].str, "‧");		// \u2027	HYPHENATION POINT
	strcpy(symbolPunctuation[39].str, " ");		// \u202F	NARROW NON-BREAK SPACE
	strcpy(symbolPunctuation[40].str, "‰");	// \u2030	PER MILLE SIGN
	strcpy(symbolPunctuation[41].str, "‱");	// \u2031	PER TEN THOUSAND SIGN
	strcpy(symbolPunctuation[42].str, "′");	// \u2032	PRIME
	strcpy(symbolPunctuation[43].str, "″");	// \u2033	DOUBLE PRIME
	strcpy(symbolPunctuation[44].str, "‴");	// \u2034	TRIPLE PRIME
	strcpy(symbolPunctuation[45].str, "‵");		// \u2035	REVERSED PRIME
	strcpy(symbolPunctuation[46].str, "‶");		// \u2036	REVERSED DOUBLE PRIME
	strcpy(symbolPunctuation[47].str, "‷");		// \u2037	REVERSED TRIPLE PRIME
	strcpy(symbolPunctuation[48].str, "‸");		// \u2038	CARET
	strcpy(symbolPunctuation[49].str, "‹");	// \u2039	SINGLE LEFT-POINTING ANGLE QUOTATION MARK
	strcpy(symbolPunctuation[50].str, "›");	// \u203A	SINGLE RIGHT-POINTING ANGLE QUOTATION MARK
	strcpy(symbolPunctuation[51].str, "※");	// \u203B	REFERENCE MARK
	strcpy(symbolPunctuation[52].str, "‼");	// \u203C	DOUBLE EXCLAMATION MARK
	strcpy(symbolPunctuation[53].str, "‽");	// \u203D	INTERROBANG
	strcpy(symbolPunctuation[54].str, "‾");	// \u203E	OVERLINE
	strcpy(symbolPunctuation[55].str, "‿");	// \u203F	UNDERTIE
	strcpy(symbolPunctuation[56].str, "⁀");	// \u2040	CHARACTER TIE
	strcpy(symbolPunctuation[57].str, "⁁");		// \u2041	CARET INSERTION POINT
	strcpy(symbolPunctuation[58].str, "⁂");	// \u2042	ASTERISM
	strcpy(symbolPunctuation[59].str, "⁃");		// \u2043	HYPHEN BULLET
	strcpy(symbolPunctuation[60].str, "⁄");	// \u2044	FRACTION SLASH
	strcpy(symbolPunctuation[61].str, "⁅");		// \u2045	LEFT SQUARE BRACKET WITH QUILL
	strcpy(symbolPunctuation[62].str, "⁆");		// \u2046	RIGHT SQUARE BRACKET WITH QUILL
	strcpy(symbolPunctuation[63].str, "⁇");	// \u2047	DOUBLE QUESTION MARK
	strcpy(symbolPunctuation[64].str, "⁈");	// \u2048	QUESTION EXCLAMATION MARK
	strcpy(symbolPunctuation[65].str, "⁉");	// \u2049	EXCLAMATION QUESTION MARK
	strcpy(symbolPunctuation[66].str, "⁊");		// \u204A	TIRONIAN SIGN ET
	strcpy(symbolPunctuation[67].str, "⁋");		// \u204B	REVERSED PILCROW SIGN
	strcpy(symbolPunctuation[68].str, "⁌");		// \u204C	BLACK LEFTWARDS BULLET
	strcpy(symbolPunctuation[69].str, "⁍");		// \u204D	BLACK RIGHTWARDS BULLET
	strcpy(symbolPunctuation[70].str, "⁎");		// \u204E	LOW ASTERISK
	strcpy(symbolPunctuation[71].str, "⁏");		// \u204F	REVERSED SEMICOLON
	strcpy(symbolPunctuation[72].str, "⁐");	// \u2050	CLOSE UP
	strcpy(symbolPunctuation[73].str, "⁑");		// \u2051	TWO ASTERISKS ALIGNED VERTICALLY
	strcpy(symbolPunctuation[74].str, "⁒");		// \u2052	COMMERCIAL MINUS SIGN
	strcpy(symbolPunctuation[75].str, "⁓");	// \u2053	SWUNG DASH
	strcpy(symbolPunctuation[76].str, "⁔");	// \u2054	INVERTED UNDERTIE
	strcpy(symbolPunctuation[77].str, "⁕");		// \u2055	FLOWER PUNCTUATION MARK
	strcpy(symbolPunctuation[78].str, "⁖");		// \u2056	THREE DOT PUNCTUATION
	strcpy(symbolPunctuation[79].str, "⁗");		// \u2057	QUADRUPLE PRIME
	strcpy(symbolPunctuation[80].str, "⁘");	// \u2058	FOUR DOT PUNCTUATION
	strcpy(symbolPunctuation[81].str, "⁙");	// \u2059	FIVE DOT PUNCTUATION
	strcpy(symbolPunctuation[82].str, "⁚");		// \u205A	TWO DOT PUNCTUATION
	strcpy(symbolPunctuation[83].str, "⁛");	// \u205B	FOUR DOT MARK
	strcpy(symbolPunctuation[84].str, "⁜");	// \u205C	DOTTED CROSS
	strcpy(symbolPunctuation[85].str, "⁝");		// \u205D	TRICOLON
	strcpy(symbolPunctuation[86].str, "⁞");	// \u205E	VERTICAL FOUR DOTS
	strcpy(symbolPunctuation[87].str, " ");		// \u205F	MEDIUM MATHEMATICAL SPACE
	strcpy(symbolPunctuation[88].str, "⁠");		// \u2060	WORD JOINER
	strcpy(symbolPunctuation[89].str, "⁡");		// \u2061	FUNCTION APPLICATION
	strcpy(symbolPunctuation[90].str, "⁢");		// \u2062	INVISIBLE TIMES
	strcpy(symbolPunctuation[91].str, "⁣");		// \u2063	INVISIBLE SEPARATOR
	strcpy(symbolPunctuation[92].str, "⁤");		// \u2064	INVISIBLE PLUS
	strcpy(symbolPunctuation[93].str, "⁪");		// \u206A	INHIBIT SYMMETRIC SWAPPING
	strcpy(symbolPunctuation[94].str, "⁫");		// \u206B	ACTIVATE SYMMETRIC SWAPPING
	strcpy(symbolPunctuation[95].str, "⁬");		// \u206C	INHIBIT ARABIC FORM SHAPING
	strcpy(symbolPunctuation[96].str, "⁭");		// \u206D	ACTIVATE ARABIC FORM SHAPING
	strcpy(symbolPunctuation[97].str, "⁮");		// \u206E	NATIONAL DIGIT SHAPES
	strcpy(symbolPunctuation[98].str, "⁯");		// \u206F	NOMINAL DIGIT SHAPES

	strcpy(symbolCurrency[1].str, "₠");		// \u20A0	EURO-CURRENCY SIGN
	strcpy(symbolCurrency[2].str, "₡");		// \u20A1	COLON SIGN
	strcpy(symbolCurrency[3].str, "₢");		// \u20A2	CRUZEIRO SIGN
	strcpy(symbolCurrency[4].str, "₣");		// \u20A3	FRENCH FRANC SIGN
	strcpy(symbolCurrency[5].str, "₤");		// \u20A4	LIRA SIGN
	strcpy(symbolCurrency[6].str, "₥");		// \u20A5	MILL SIGN
	strcpy(symbolCurrency[7].str, "₦");		// \u20A6	NAIRA SIGN
	strcpy(symbolCurrency[8].str, "₧");		// \u20A7	PESETA SIGN
	strcpy(symbolCurrency[9].str, "₨");		// \u20A8	RUPEE SIGN
	strcpy(symbolCurrency[10].str, "₩");	// \u20A9	WON SIGN
	strcpy(symbolCurrency[11].str, "₪");	// \u20AA	NEW SHEQEL SIGN
	strcpy(symbolCurrency[12].str, "₫");	// \u20AB	DONG SIGN
	strcpy(symbolCurrency[13].str, "€");	// \u20AC	EURO SIGN
	strcpy(symbolCurrency[14].str, "₭");	// \u20AD	KIP SIGN
	strcpy(symbolCurrency[15].str, "₮");	// \u20AE	TUGRIK SIGN
	strcpy(symbolCurrency[16].str, "₯");	// \u20AF	DRACHMA SIGN
	strcpy(symbolCurrency[17].str, "₰");	// \u20B0	GERMAN PENNY SYMBOL
	strcpy(symbolCurrency[18].str, "₱");	// \u20B1	PESO SIGN
	strcpy(symbolCurrency[19].str, "₲");	// \u20B2	GUARANI SIGN
	strcpy(symbolCurrency[20].str, "₳");	// \u20B3	AUSTRAL SIGN
	strcpy(symbolCurrency[21].str, "₴");	// \u20B4	HRYVNIA SIGN
	strcpy(symbolCurrency[22].str, "₵");	// \u20B5	CEDI SIGN
	strcpy(symbolCurrency[23].str, "₶");	// \u20B6	LIVRE TOURNOIS SIGN
	strcpy(symbolCurrency[24].str, "₷");	// \u20B7	SPESMILO SIGN
	strcpy(symbolCurrency[25].str, "₸");	// \u20B8	TENGE SIGN
	strcpy(symbolCurrency[26].str, "₹");	// \u20B9	INDIAN RUPEE SIGN
	strcpy(symbolCurrency[27].str, "₺");	// \u20BA	TURKISH LIRA SIGN
	strcpy(symbolCurrency[28].str, "₻");	// \u20BB	NORDIC MARK SIGN
	strcpy(symbolCurrency[29].str, "₼");	// \u20BC	MANAT SIGN
	strcpy(symbolCurrency[30].str, "₽");	// \u20BD	RUBLE SIGN
	strcpy(symbolCurrency[31].str, "₾");	// \u20BE	LARI SIGN
	strcpy(symbolCurrency[32].str, "₿");	// \u20BF	BITCOIN SIGN

	strcpy(symbolLetterlike[1].str, "℀");		// \u2100	ACCOUNT OF
	strcpy(symbolLetterlike[2].str, "℁");		// \u2101	ADDRESSED TO THE SUBJECT
	strcpy(symbolLetterlike[3].str, "ℂ");		// \u2102	DOUBLE-STRUCK CAPITAL C
	strcpy(symbolLetterlike[4].str, "℃");		// \u2103	DEGREE CELSIUS
	strcpy(symbolLetterlike[5].str, "℄");		// \u2104	CENTRE LINE SYMBOL
	strcpy(symbolLetterlike[6].str, "℅");		// \u2105	CARE OF
	strcpy(symbolLetterlike[7].str, "℆");		// \u2106	CADA UNA
	strcpy(symbolLetterlike[8].str, "ℇ");		// \u2107	EULER CONSTANT
	strcpy(symbolLetterlike[9].str, "℈");		// \u2108	SCRUPLE
	strcpy(symbolLetterlike[10].str, "℉");		// \u2109	DEGREE FAHRENHEIT
	strcpy(symbolLetterlike[11].str, "ℊ");		// \u210A	SCRIPT SMALL G
	strcpy(symbolLetterlike[12].str, "ℋ");		// \u210B	SCRIPT CAPITAL H
	strcpy(symbolLetterlike[13].str, "ℌ");		// \u210C	BLACK-LETTER CAPITAL H
	strcpy(symbolLetterlike[14].str, "ℍ");		// \u210D	DOUBLE-STRUCK CAPITAL H
	strcpy(symbolLetterlike[15].str, "ℎ");		// \u210E	PLANCK CONSTANT
	strcpy(symbolLetterlike[16].str, "ℏ");		// \u210F	PLANCK CONSTANT OVER TWO PI
	strcpy(symbolLetterlike[17].str, "ℐ");		// \u2110	SCRIPT CAPITAL I
	strcpy(symbolLetterlike[18].str, "ℑ");		// \u2111	BLACK-LETTER CAPITAL I
	strcpy(symbolLetterlike[19].str, "ℒ");		// \u2112	SCRIPT CAPITAL L
	strcpy(symbolLetterlike[20].str, "ℓ");	// \u2113	SCRIPT SMALL L
	strcpy(symbolLetterlike[21].str, "℔");		// \u2114	L B BAR SYMBOL
	strcpy(symbolLetterlike[22].str, "ℕ");		// \u2115	DOUBLE-STRUCK CAPITAL N
	strcpy(symbolLetterlike[23].str, "№");	// \u2116	NUMERO SIGN
	strcpy(symbolLetterlike[24].str, "℗");	// \u2117	SOUND RECORDING COPYRIGHT
	strcpy(symbolLetterlike[25].str, "℘");		// \u2118	SCRIPT CAPITAL P
	strcpy(symbolLetterlike[26].str, "ℙ");		// \u2119	DOUBLE-STRUCK CAPITAL P
	strcpy(symbolLetterlike[27].str, "ℚ");		// \u211A	DOUBLE-STRUCK CAPITAL Q
	strcpy(symbolLetterlike[28].str, "ℛ");		// \u211B	SCRIPT CAPITAL R
	strcpy(symbolLetterlike[29].str, "ℜ");		// \u211C	BLACK-LETTER CAPITAL R
	strcpy(symbolLetterlike[30].str, "ℝ");		// \u211D	DOUBLE-STRUCK CAPITAL R
	strcpy(symbolLetterlike[31].str, "℞");	// \u211E	PRESCRIPTION TAKE
	strcpy(symbolLetterlike[32].str, "℟");		// \u211F	RESPONSE
	strcpy(symbolLetterlike[33].str, "℠");		// \u2120	SERVICE MARK
	strcpy(symbolLetterlike[34].str, "℡");	// \u2121	TELEPHONE SIGN
	strcpy(symbolLetterlike[35].str, "™");	// \u2122	TRADE MARK SIGN
	strcpy(symbolLetterlike[36].str, "℣");		// \u2123	VERSICLE
	strcpy(symbolLetterlike[37].str, "ℤ");		// \u2124	DOUBLE-STRUCK CAPITAL Z
	strcpy(symbolLetterlike[38].str, "℥");		// \u2125	OUNCE SIGN
	strcpy(symbolLetterlike[39].str, "Ω");	// \u2126	OHM SIGN
	strcpy(symbolLetterlike[40].str, "℧");		// \u2127	INVERTED OHM SIGN
	strcpy(symbolLetterlike[41].str, "ℨ");		// \u2128	BLACK-LETTER CAPITAL Z
	strcpy(symbolLetterlike[42].str, "℩");		// \u2129	TURNED GREEK SMALL LETTER IOTA
	strcpy(symbolLetterlike[43].str, "K");		// \u212A	KELVIN SIGN
	strcpy(symbolLetterlike[44].str, "Å");	// \u212B	ANGSTROM SIGN
	strcpy(symbolLetterlike[45].str, "ℬ");		// \u212C	SCRIPT CAPITAL B
	strcpy(symbolLetterlike[46].str, "ℭ");		// \u212D	BLACK-LETTER CAPITAL C
	strcpy(symbolLetterlike[47].str, "℮");	// \u212E	ESTIMATED SYMBOL
	strcpy(symbolLetterlike[48].str, "ℯ");		// \u212F	SCRIPT SMALL E
	strcpy(symbolLetterlike[49].str, "ℰ");		// \u2130	SCRIPT CAPITAL E
	strcpy(symbolLetterlike[50].str, "ℱ");		// \u2131	SCRIPT CAPITAL F
	strcpy(symbolLetterlike[51].str, "Ⅎ");		// \u2132	TURNED CAPITAL F
	strcpy(symbolLetterlike[52].str, "ℳ");	// \u2133	SCRIPT CAPITAL M
	strcpy(symbolLetterlike[53].str, "ℴ");		// \u2134	SCRIPT SMALL O
	strcpy(symbolLetterlike[54].str, "ℵ");		// \u2135	ALEF SYMBOL
	strcpy(symbolLetterlike[55].str, "ℶ");		// \u2136	BET SYMBOL
	strcpy(symbolLetterlike[56].str, "ℷ");		// \u2137	GIMEL SYMBOL
	strcpy(symbolLetterlike[57].str, "ℸ");		// \u2138	DALET SYMBOL
	strcpy(symbolLetterlike[58].str, "ℹ");	// \u2139	INFORMATION SOURCE
	strcpy(symbolLetterlike[59].str, "℺");	// \u213A	ROTATED CAPITAL Q
	strcpy(symbolLetterlike[60].str, "℻");	// \u213B	FACSIMILE SIGN
	strcpy(symbolLetterlike[61].str, "ℼ");	// \u213C	DOUBLE-STRUCK SMALL PI
	strcpy(symbolLetterlike[62].str, "ℽ");		// \u213D	DOUBLE-STRUCK SMALL GAMMA
	strcpy(symbolLetterlike[63].str, "ℾ");		// \u213E	DOUBLE-STRUCK CAPITAL GAMMA
	strcpy(symbolLetterlike[64].str, "ℿ");	// \u213F	DOUBLE-STRUCK CAPITAL PI
	strcpy(symbolLetterlike[65].str, "⅀");	// \u2140	DOUBLE-STRUCK N-ARY SUMMATION
	strcpy(symbolLetterlike[66].str, "⅁");	// \u2141	TURNED SANS-SERIF CAPITAL G
	strcpy(symbolLetterlike[67].str, "⅂");		// \u2142	TURNED SANS-SERIF CAPITAL L
	strcpy(symbolLetterlike[68].str, "⅃");		// \u2143	REVERSED SANS-SERIF CAPITAL L
	strcpy(symbolLetterlike[69].str, "⅄");		// \u2144	TURNED SANS-SERIF CAPITAL Y
	strcpy(symbolLetterlike[70].str, "ⅅ");	// \u2145	DOUBLE-STRUCK ITALIC CAPITAL D
	strcpy(symbolLetterlike[71].str, "ⅆ");	// \u2146	DOUBLE-STRUCK ITALIC SMALL D
	strcpy(symbolLetterlike[72].str, "ⅇ");	// \u2147	DOUBLE-STRUCK ITALIC SMALL E
	strcpy(symbolLetterlike[73].str, "ⅈ");		// \u2148	DOUBLE-STRUCK ITALIC SMALL I
	strcpy(symbolLetterlike[74].str, "ⅉ");		// \u2149	DOUBLE-STRUCK ITALIC SMALL J
	strcpy(symbolLetterlike[75].str, "⅊");	// \u214A	PROPERTY LINE
	strcpy(symbolLetterlike[76].str, "⅋");	// \u214B	TURNED AMPERSAND
	strcpy(symbolLetterlike[77].str, "⅌");		// \u214C	PER SIGN
	strcpy(symbolLetterlike[78].str, "⅍");	// \u214D	AKTIESELSKAB
	strcpy(symbolLetterlike[79].str, "ⅎ");	// \u214E	TURNED SMALL F
	strcpy(symbolLetterlike[80].str, "⅏");	// \u214F	SYMBOL FOR SAMARITAN SOURCE

	strcpy(symbolArrows[1].str, "←");		// \u2190	LEFTWARDS ARROW
	strcpy(symbolArrows[2].str, "↑");		// \u2191	UPWARDS ARROW
	strcpy(symbolArrows[3].str, "→");		// \u2192	RIGHTWARDS ARROW
	strcpy(symbolArrows[4].str, "↓");		// \u2193	DOWNWARDS ARROW
	strcpy(symbolArrows[5].str, "↔");		// \u2194	LEFT RIGHT ARROW
	strcpy(symbolArrows[6].str, "↕");		// \u2195	UP DOWN ARROW
	strcpy(symbolArrows[7].str, "↖");		// \u2196	NORTH WEST ARROW
	strcpy(symbolArrows[8].str, "↗");		// \u2197	NORTH EAST ARROW
	strcpy(symbolArrows[9].str, "↘");		// \u2198	SOUTH EAST ARROW
	strcpy(symbolArrows[10].str, "↙");		// \u2199	SOUTH WEST ARROW
	strcpy(symbolArrows[11].str, "↚");		// \u219A	LEFTWARDS ARROW WITH STROKE
	strcpy(symbolArrows[12].str, "↛");		// \u219B	RIGHTWARDS ARROW WITH STROKE
	strcpy(symbolArrows[13].str, "↜");		// \u219C	LEFTWARDS WAVE ARROW
	strcpy(symbolArrows[14].str, "↝");		// \u219D	RIGHTWARDS WAVE ARROW
	strcpy(symbolArrows[15].str, "↞");		// \u219E	LEFTWARDS TWO HEADED ARROW
	strcpy(symbolArrows[16].str, "↟");		// \u219F	UPWARDS TWO HEADED ARROW
	strcpy(symbolArrows[17].str, "↠");		// \u21A0	RIGHTWARDS TWO HEADED ARROW
	strcpy(symbolArrows[18].str, "↡");		// \u21A1	DOWNWARDS TWO HEADED ARROW
	strcpy(symbolArrows[19].str, "↢");		// \u21A2	LEFTWARDS ARROW WITH TAIL
	strcpy(symbolArrows[20].str, "↣");		// \u21A3	RIGHTWARDS ARROW WITH TAIL
	strcpy(symbolArrows[21].str, "↤");		// \u21A4	LEFTWARDS ARROW FROM BAR
	strcpy(symbolArrows[22].str, "↥");		// \u21A5	UPWARDS ARROW FROM BAR
	strcpy(symbolArrows[23].str, "↦");		// \u21A6	RIGHTWARDS ARROW FROM BAR
	strcpy(symbolArrows[24].str, "↧");		// \u21A7	DOWNWARDS ARROW FROM BAR
	strcpy(symbolArrows[25].str, "↨");	// \u21A8	UP DOWN ARROW WITH BASE
	strcpy(symbolArrows[26].str, "↩");		// \u21A9	LEFTWARDS ARROW WITH HOOK
	strcpy(symbolArrows[27].str, "↪");		// \u21AA	RIGHTWARDS ARROW WITH HOOK
	strcpy(symbolArrows[28].str, "↫");		// \u21AB	LEFTWARDS ARROW WITH LOOP
	strcpy(symbolArrows[29].str, "↬");		// \u21AC	RIGHTWARDS ARROW WITH LOOP
	strcpy(symbolArrows[30].str, "↭");		// \u21AD	LEFT RIGHT WAVE ARROW
	strcpy(symbolArrows[31].str, "↮");		// \u21AE	LEFT RIGHT ARROW WITH STROKE
	strcpy(symbolArrows[32].str, "↯");		// \u21AF	DOWNWARDS ZIGZAG ARROW
	strcpy(symbolArrows[33].str, "↰");		// \u21B0	UPWARDS ARROW WITH TIP LEFTWARDS
	strcpy(symbolArrows[34].str, "↱");		// \u21B1	UPWARDS ARROW WITH TIP RIGHTWARDS
	strcpy(symbolArrows[35].str, "↲");	// \u21B2	DOWNWARDS ARROW WITH TIP LEFTWARDS
	strcpy(symbolArrows[36].str, "↳");		// \u21B3	DOWNWARDS ARROW WITH TIP RIGHTWARDS
	strcpy(symbolArrows[37].str, "↴");		// \u21B4	RIGHTWARDS ARROW WITH CORNER DOWNWARDS
	strcpy(symbolArrows[38].str, "↵");		// \u21B5	DOWNWARDS ARROW WITH CORNER LEFTWARDS
	strcpy(symbolArrows[39].str, "↶");		// \u21B6	ANTICLOCKWISE TOP SEMICIRCLE ARROW
	strcpy(symbolArrows[40].str, "↷");		// \u21B7	CLOCKWISE TOP SEMICIRCLE ARROW
	strcpy(symbolArrows[41].str, "↸");		// \u21B8	NORTH WEST ARROW TO LONG BAR
	strcpy(symbolArrows[42].str, "↹");		// \u21B9	LEFTWARDS ARROW TO BAR OVER RIGHTWARDS ARROW TO BAR
	strcpy(symbolArrows[43].str, "↺");		// \u21BA	ANTICLOCKWISE OPEN CIRCLE ARROW
	strcpy(symbolArrows[44].str, "↻");		// \u21BB	CLOCKWISE OPEN CIRCLE ARROW
	strcpy(symbolArrows[45].str, "↼");		// \u21BC	LEFTWARDS HARPOON WITH BARB UPWARDS
	strcpy(symbolArrows[46].str, "↽");		// \u21BD	LEFTWARDS HARPOON WITH BARB DOWNWARDS
	strcpy(symbolArrows[47].str, "↾");		// \u21BE	UPWARDS HARPOON WITH BARB RIGHTWARDS
	strcpy(symbolArrows[48].str, "↿");		// \u21BF	UPWARDS HARPOON WITH BARB LEFTWARDS
	strcpy(symbolArrows[49].str, "⇀");		// \u21C0	RIGHTWARDS HARPOON WITH BARB UPWARDS
	strcpy(symbolArrows[50].str, "⇁");		// \u21C1	RIGHTWARDS HARPOON WITH BARB DOWNWARDS
	strcpy(symbolArrows[51].str, "⇂");		// \u21C2	DOWNWARDS HARPOON WITH BARB RIGHTWARDS
	strcpy(symbolArrows[52].str, "⇃");		// \u21C3	DOWNWARDS HARPOON WITH BARB LEFTWARDS
	strcpy(symbolArrows[53].str, "⇄");		// \u21C4	RIGHTWARDS ARROW OVER LEFTWARDS ARROW
	strcpy(symbolArrows[54].str, "⇅");		// \u21C5	UPWARDS ARROW LEFTWARDS OF DOWNWARDS ARROW
	strcpy(symbolArrows[55].str, "⇆");		// \u21C6	LEFTWARDS ARROW OVER RIGHTWARDS ARROW
	strcpy(symbolArrows[56].str, "⇇");		// \u21C7	LEFTWARDS PAIRED ARROWS
	strcpy(symbolArrows[57].str, "⇈");		// \u21C8	UPWARDS PAIRED ARROWS
	strcpy(symbolArrows[58].str, "⇉");		// \u21C9	RIGHTWARDS PAIRED ARROWS
	strcpy(symbolArrows[59].str, "⇊");		// \u21CA	DOWNWARDS PAIRED ARROWS
	strcpy(symbolArrows[60].str, "⇋");		// \u21CB	LEFTWARDS HARPOON OVER RIGHTWARDS HARPOON
	strcpy(symbolArrows[61].str, "⇌");		// \u21CC	RIGHTWARDS HARPOON OVER LEFTWARDS HARPOON
	strcpy(symbolArrows[62].str, "⇍");		// \u21CD	LEFTWARDS DOUBLE ARROW WITH STROKE
	strcpy(symbolArrows[63].str, "⇎");		// \u21CE	LEFT RIGHT DOUBLE ARROW WITH STROKE
	strcpy(symbolArrows[64].str, "⇏");		// \u21CF	RIGHTWARDS DOUBLE ARROW WITH STROKE
	strcpy(symbolArrows[65].str, "⇐");		// \u21D0	LEFTWARDS DOUBLE ARROW
	strcpy(symbolArrows[66].str, "⇑");		// \u21D1	UPWARDS DOUBLE ARROW
	strcpy(symbolArrows[67].str, "⇒");	// \u21D2	RIGHTWARDS DOUBLE ARROW
	strcpy(symbolArrows[68].str, "⇓");		// \u21D3	DOWNWARDS DOUBLE ARROW
	strcpy(symbolArrows[69].str, "⇔");	// \u21D4	LEFT RIGHT DOUBLE ARROW
	strcpy(symbolArrows[70].str, "⇕");		// \u21D5	UP DOWN DOUBLE ARROW
	strcpy(symbolArrows[71].str, "⇖");		// \u21D6	NORTH WEST DOUBLE ARROW
	strcpy(symbolArrows[72].str, "⇗");		// \u21D7	NORTH EAST DOUBLE ARROW
	strcpy(symbolArrows[73].str, "⇘");		// \u21D8	SOUTH EAST DOUBLE ARROW
	strcpy(symbolArrows[74].str, "⇙");		// \u21D9	SOUTH WEST DOUBLE ARROW
	strcpy(symbolArrows[75].str, "⇚");		// \u21DA	LEFTWARDS TRIPLE ARROW
	strcpy(symbolArrows[76].str, "⇛");		// \u21DB	RIGHTWARDS TRIPLE ARROW
	strcpy(symbolArrows[77].str, "⇜");		// \u21DC	LEFTWARDS SQUIGGLE ARROW
	strcpy(symbolArrows[78].str, "⇝");		// \u21DD	RIGHTWARDS SQUIGGLE ARROW
	strcpy(symbolArrows[79].str, "⇞");		// \u21DE	UPWARDS ARROW WITH DOUBLE STROKE
	strcpy(symbolArrows[80].str, "⇟");		// \u21DF	DOWNWARDS ARROW WITH DOUBLE STROKE
	strcpy(symbolArrows[81].str, "⇠");		// \u21	LEFTWARDS DASHED ARROW
	strcpy(symbolArrows[82].str, "⇡");	// \u210	UPWARDS DASHED ARROW
	strcpy(symbolArrows[83].str, "⇢");		// \u2100	RIGHTWARDS DASHED ARROW
	strcpy(symbolArrows[84].str, "⇣");	// \u21000	DOWNWARDS DASHED ARROW
	strcpy(symbolArrows[85].str, "⇤");		// \u210000	LEFTWARDS ARROW TO BAR
	strcpy(symbolArrows[86].str, "⇥");		// \u2100000	RIGHTWARDS ARROW TO BAR
	strcpy(symbolArrows[87].str, "⇦");		// \u21000000	LEFTWARDS WHITE ARROW
	strcpy(symbolArrows[88].str, "⇧");		// \u210000000	UPWARDS WHITE ARROW
	strcpy(symbolArrows[89].str, "⇨");		// \u2100000000	RIGHTWARDS WHITE ARROW
	strcpy(symbolArrows[90].str, "⇩");		// \u21000000000	DOWNWARDS WHITE ARROW
	strcpy(symbolArrows[91].str, "⇪");		// \u21EA	UPWARDS WHITE ARROW FROM BAR
	strcpy(symbolArrows[92].str, "⇫");	// \u21EB	UPWARDS WHITE ARROW ON PEDESTAL
	strcpy(symbolArrows[93].str, "⇬");	// \u21EC	UPWARDS WHITE ARROW ON PEDESTAL WITH HORIZONTAL BAR
	strcpy(symbolArrows[94].str, "⇭");	// \u21ED	UPWARDS WHITE ARROW ON PEDESTAL WITH VERTICAL BAR
	strcpy(symbolArrows[95].str, "⇮");	// \u21EE	UPWARDS WHITE DOUBLE ARROW
	strcpy(symbolArrows[96].str, "⇯");	// \u21EF	UPWARDS WHITE DOUBLE ARROW ON PEDESTAL
	strcpy(symbolArrows[97].str, "⇰");	// \u21F0	RIGHTWARDS WHITE ARROW FROM WALL
	strcpy(symbolArrows[98].str, "⇱");	// \u21F1	NORTH WEST ARROW TO CORNER
	strcpy(symbolArrows[99].str, "⇲");	// \u21F2	SOUTH EAST ARROW TO CORNER
	strcpy(symbolArrows[100].str, "⇳");	// \u21F3	UP DOWN WHITE ARROW
	strcpy(symbolArrows[101].str, "⇴");	// \u21F4	RIGHT ARROW WITH SMALL CIRCLE
	strcpy(symbolArrows[102].str, "⇵");	// \u21F5	DOWNWARDS ARROW LEFTWARDS OF UPWARDS ARROW
	strcpy(symbolArrows[103].str, "⇶");	// \u21F6	THREE RIGHTWARDS ARROWS
	strcpy(symbolArrows[104].str, "⇷");	// \u21F7	LEFTWARDS ARROW WITH VERTICAL STROKE
	strcpy(symbolArrows[105].str, "⇸");	// \u21F8	RIGHTWARDS ARROW WITH VERTICAL STROKE
	strcpy(symbolArrows[106].str, "⇹");	// \u21F9	LEFT RIGHT ARROW WITH VERTICAL STROKE
	strcpy(symbolArrows[107].str, "⇺");	// \u21FA	LEFTWARDS ARROW WITH DOUBLE VERTICAL STROKE
	strcpy(symbolArrows[108].str, "⇻");	// \u21FB	RIGHTWARDS ARROW WITH DOUBLE VERTICAL STROKE
	strcpy(symbolArrows[109].str, "⇼");	// \u21FC	LEFT RIGHT ARROW WITH DOUBLE VERTICAL STROKE
	strcpy(symbolArrows[110].str, "⇽");	// \u21FD	LEFTWARDS OPEN-HEADED ARROW
	strcpy(symbolArrows[111].str, "⇾");	// \u21FE	RIGHTWARDS OPEN-HEADED ARROW
	strcpy(symbolArrows[112].str, "⇿");	// \u21FF	LEFT RIGHT OPEN-HEADED ARROW

	strcpy(symbolMath[1].str, "∀");		// \u2200	FOR ALL
	strcpy(symbolMath[2].str, "∁");		// \u2201	COMPLEMENT
	strcpy(symbolMath[3].str, "∂");		// \u2202	PARTIAL DIFFERENTIAL
	strcpy(symbolMath[4].str, "∃");		// \u2203	THERE EXISTS
	strcpy(symbolMath[5].str, "∄");		// \u2204	THERE DOES NOT EXIST
	strcpy(symbolMath[6].str, "∅");		// \u2205	EMPTY SET
	strcpy(symbolMath[7].str, "∆");		// \u2206	INCREMENT
	strcpy(symbolMath[8].str, "∇");		// \u2207	NABLA
	strcpy(symbolMath[9].str, "∈");		// \u2208	ELEMENT OF
	strcpy(symbolMath[10].str, "∉");		// \u2209	NOT AN ELEMENT OF
	strcpy(symbolMath[11].str, "∊");		// \u220A	SMALL ELEMENT OF
	strcpy(symbolMath[12].str, "∋");	// \u220B	CONTAINS AS MEMBER
	strcpy(symbolMath[13].str, "∌");		// \u220C	DOES NOT CONTAIN AS MEMBER
	strcpy(symbolMath[14].str, "∍");		// \u220D	SMALL CONTAINS AS MEMBER
	strcpy(symbolMath[15].str, "∎");		// \u220E	END OF PROOF
	strcpy(symbolMath[16].str, "∏");	// \u220F	N-ARY PRODUCT
	strcpy(symbolMath[17].str, "∐");		// \u2210	N-ARY COPRODUCT
	strcpy(symbolMath[18].str, "∑");	// \u2211	N-ARY SUMMATION
	strcpy(symbolMath[19].str, "−");	// \u2212	MINUS SIGN
	strcpy(symbolMath[20].str, "∓");		// \u2213	MINUS-OR-PLUS SIGN
	strcpy(symbolMath[21].str, "∔");		// \u2214	DOT PLUS
	strcpy(symbolMath[22].str, "∕");	// \u2215	DIVISION SLASH
	strcpy(symbolMath[23].str, "∖");		// \u2216	SET MINUS
	strcpy(symbolMath[24].str, "∗");		// \u2217	ASTERISK OPERATOR
	strcpy(symbolMath[25].str, "∘");		// \u2218	RING OPERATOR
	strcpy(symbolMath[26].str, "∙");	// \u2219	BULLET OPERATOR
	strcpy(symbolMath[27].str, "√");	// \u221A	SQUARE ROOT
	strcpy(symbolMath[28].str, "∛");		// \u221B	CUBE ROOT
	strcpy(symbolMath[29].str, "∜");		// \u221C	FOURTH ROOT
	strcpy(symbolMath[30].str, "∝");	// \u221D	PROPORTIONAL TO
	strcpy(symbolMath[31].str, "∞");	// \u221E	INFINITY
	strcpy(symbolMath[32].str, "∟");	// \u221F	RIGHT ANGLE
	strcpy(symbolMath[33].str, "∠");	// \u2220	ANGLE
	strcpy(symbolMath[34].str, "∡");		// \u2221	MEASURED ANGLE
	strcpy(symbolMath[35].str, "∢");		// \u2222	SPHERICAL ANGLE
	strcpy(symbolMath[36].str, "∣");		// \u2223	DIVIDES
	strcpy(symbolMath[37].str, "∤");		// \u2224	DOES NOT DIVIDE
	strcpy(symbolMath[38].str, "∥");	// \u2225	PARALLEL TO
	strcpy(symbolMath[39].str, "∦");		// \u2226	NOT PARALLEL TO
	strcpy(symbolMath[40].str, "∧");	// \u2227	LOGICAL AND
	strcpy(symbolMath[41].str, "∨");	// \u2228	LOGICAL OR
	strcpy(symbolMath[42].str, "∩");	// \u2229	INTERSECTION
	strcpy(symbolMath[43].str, "∪");	// \u222A	UNION
	strcpy(symbolMath[44].str, "∫");	// \u222B	INTEGRAL
	strcpy(symbolMath[45].str, "∬");	// \u222C	DOUBLE INTEGRAL
	strcpy(symbolMath[46].str, "∭");		// \u222D	TRIPLE INTEGRAL
	strcpy(symbolMath[47].str, "∮");	// \u222E	CONTOUR INTEGRAL
	strcpy(symbolMath[48].str, "∯");		// \u222F	SURFACE INTEGRAL
	strcpy(symbolMath[49].str, "∰");		// \u2230	VOLUME INTEGRAL
	strcpy(symbolMath[50].str, "∱");		// \u2231	CLOCKWISE INTEGRAL
	strcpy(symbolMath[51].str, "∲");		// \u2232	CLOCKWISE CONTOUR INTEGRAL
	strcpy(symbolMath[52].str, "∳");		// \u2233	ANTICLOCKWISE CONTOUR INTEGRAL
	strcpy(symbolMath[53].str, "∴");	// \u2234	THEREFORE
	strcpy(symbolMath[54].str, "∵");	// \u2235	BECAUSE
	strcpy(symbolMath[55].str, "∶");	// \u2236	RATIO
	strcpy(symbolMath[56].str, "∷");		// \u2237	PROPORTION
	strcpy(symbolMath[57].str, "∸");		// \u2238	DOT MINUS
	strcpy(symbolMath[58].str, "∹");		// \u2239	EXCESS
	strcpy(symbolMath[59].str, "∺");		// \u223A	GEOMETRIC PROPORTION
	strcpy(symbolMath[60].str, "∻");		// \u223B	HOMOTHETIC
	strcpy(symbolMath[61].str, "∼");		// \u223C	TILDE OPERATOR
	strcpy(symbolMath[62].str, "∽");	// \u223D	REVERSED TILDE
	strcpy(symbolMath[63].str, "∾");		// \u223E	INVERTED LAZY S
	strcpy(symbolMath[64].str, "∿");		// \u223F	SINE WAVE
	strcpy(symbolMath[65].str, "≀");		// \u2240	WREATH PRODUCT
	strcpy(symbolMath[66].str, "≁");		// \u2241	NOT TILDE
	strcpy(symbolMath[67].str, "≂");		// \u2242	MINUS TILDE
	strcpy(symbolMath[68].str, "≃");		// \u2243	ASYMPTOTICALLY EQUAL TO
	strcpy(symbolMath[69].str, "≄");		// \u2244	NOT ASYMPTOTICALLY EQUAL TO
	strcpy(symbolMath[70].str, "≅");		// \u2245	APPROXIMATELY EQUAL TO
	strcpy(symbolMath[71].str, "≆");		// \u2246	APPROXIMATELY BUT NOT ACTUALLY EQUAL TO
	strcpy(symbolMath[72].str, "≇");		// \u2247	NEITHER APPROXIMATELY NOR ACTUALLY EQUAL TO
	strcpy(symbolMath[73].str, "≈");	// \u2248	ALMOST EQUAL TO
	strcpy(symbolMath[74].str, "≉");		// \u2249	NOT ALMOST EQUAL TO
	strcpy(symbolMath[75].str, "≊");		// \u224A	ALMOST EQUAL OR EQUAL TO
	strcpy(symbolMath[76].str, "≋");		// \u224B	TRIPLE TILDE
	strcpy(symbolMath[77].str, "≌");		// \u224C	ALL EQUAL TO
	strcpy(symbolMath[78].str, "≍");		// \u224D	EQUIVALENT TO
	strcpy(symbolMath[79].str, "≎");		// \u224E	GEOMETRICALLY EQUIVALENT TO
	strcpy(symbolMath[80].str, "≏");		// \u224F	DIFFERENCE BETWEEN
	strcpy(symbolMath[81].str, "≐");		// \u2250	APPROACHES THE LIMIT
	strcpy(symbolMath[82].str, "≑");		// \u2251	GEOMETRICALLY EQUAL TO
	strcpy(symbolMath[83].str, "≒");	// \u2252	APPROXIMATELY EQUAL TO OR THE IMAGE OF
	strcpy(symbolMath[84].str, "≓");		// \u2253	IMAGE OF OR APPROXIMATELY EQUAL TO
	strcpy(symbolMath[85].str, "≔");		// \u2254	COLON EQUALS
	strcpy(symbolMath[86].str, "≕");		// \u2255	EQUALS COLON
	strcpy(symbolMath[87].str, "≖");		// \u2256	RING IN EQUAL TO
	strcpy(symbolMath[88].str, "≗");		// \u2257	RING EQUAL TO
	strcpy(symbolMath[89].str, "≘");		// \u2258	CORRESPONDS TO
	strcpy(symbolMath[90].str, "≙");		// \u2259	ESTIMATES
	strcpy(symbolMath[91].str, "≚");		// \u225A	EQUIANGULAR TO
	strcpy(symbolMath[92].str, "≛");		// \u225B	STAR EQUALS
	strcpy(symbolMath[93].str, "≜");		// \u225C	DELTA EQUAL TO
	strcpy(symbolMath[94].str, "≝");		// \u225D	EQUAL TO BY DEFINITION
	strcpy(symbolMath[95].str, "≞");		// \u225E	MEASURED BY
	strcpy(symbolMath[96].str, "≟");		// \u225F	QUESTIONED EQUAL TO
	strcpy(symbolMath[97].str, "≠");	// \u2260	NOT EQUAL TO
	strcpy(symbolMath[98].str, "≡");	// \u2261	IDENTICAL TO
	strcpy(symbolMath[99].str, "≢");	// \u2262	NOT IDENTICAL TO
	strcpy(symbolMath[100].str, "≣");	// \u2263	STRICTLY EQUIVALENT TO
	strcpy(symbolMath[101].str, "≤");	// \u2264	LESS-THAN OR EQUAL TO
	strcpy(symbolMath[102].str, "≥");	// \u2265	GREATER-THAN OR EQUAL TO
	strcpy(symbolMath[103].str, "≦");	// \u2266	LESS-THAN OVER EQUAL TO
	strcpy(symbolMath[104].str, "≧");	// \u2267	GREATER-THAN OVER EQUAL TO
	strcpy(symbolMath[105].str, "≨");	// \u2268	LESS-THAN BUT NOT EQUAL TO
	strcpy(symbolMath[106].str, "≩");	// \u2269	GREATER-THAN BUT NOT EQUAL TO
	strcpy(symbolMath[107].str, "≪");	// \u226A	MUCH LESS-THAN
	strcpy(symbolMath[108].str, "≫");	// \u226B	MUCH GREATER-THAN
	strcpy(symbolMath[109].str, "≬");	// \u226C	BETWEEN
	strcpy(symbolMath[110].str, "≭");	// \u226D	NOT EQUIVALENT TO
	strcpy(symbolMath[111].str, "≮");	// \u226E	NOT LESS-THAN
	strcpy(symbolMath[112].str, "≯");	// \u226F	NOT GREATER-THAN
	strcpy(symbolMath[113].str, "≰");	// \u2270	NEITHER LESS-THAN NOR EQUAL TO
	strcpy(symbolMath[114].str, "≱");	// \u2271	NEITHER GREATER-THAN NOR EQUAL TO
	strcpy(symbolMath[115].str, "≲");	// \u2272	LESS-THAN OR EQUIVALENT TO
	strcpy(symbolMath[116].str, "≳");	// \u2273	GREATER-THAN OR EQUIVALENT TO
	strcpy(symbolMath[117].str, "≴");	// \u2274	NEITHER LESS-THAN NOR EQUIVALENT TO
	strcpy(symbolMath[118].str, "≵");	// \u2275	NEITHER GREATER-THAN NOR EQUIVALENT TO
	strcpy(symbolMath[119].str, "≶");	// \u2276	LESS-THAN OR GREATER-THAN
	strcpy(symbolMath[120].str, "≷");	// \u2277	GREATER-THAN OR LESS-THAN
	strcpy(symbolMath[121].str, "≸");	// \u2278	NEITHER LESS-THAN NOR GREATER-THAN
	strcpy(symbolMath[122].str, "≹");	// \u2279	NEITHER GREATER-THAN NOR LESS-THAN
	strcpy(symbolMath[123].str, "≺");	// \u227A	PRECEDES
	strcpy(symbolMath[124].str, "≻");	// \u227B	SUCCEEDS
	strcpy(symbolMath[125].str, "≼");	// \u227C	PRECEDES OR EQUAL TO
	strcpy(symbolMath[126].str, "≽");	// \u227D	SUCCEEDS OR EQUAL TO
	strcpy(symbolMath[127].str, "≾");	// \u227E	PRECEDES OR EQUIVALENT TO
	strcpy(symbolMath[128].str, "≿");	// \u227F	SUCCEEDS OR EQUIVALENT TO
	strcpy(symbolMath[129].str, "⊀");	// \u2280	DOES NOT PRECEDE
	strcpy(symbolMath[130].str, "⊁");	// \u2281	DOES NOT SUCCEED
	strcpy(symbolMath[131].str, "⊂");	// \u2282	SUBSET OF
	strcpy(symbolMath[132].str, "⊃");	// \u2283	SUPERSET OF
	strcpy(symbolMath[133].str, "⊄");	// \u2284	NOT A SUBSET OF
	strcpy(symbolMath[134].str, "⊅");	// \u2285	NOT A SUPERSET OF
	strcpy(symbolMath[135].str, "⊆");	// \u2286	SUBSET OF OR EQUAL TO
	strcpy(symbolMath[136].str, "⊇");	// \u2287	SUPERSET OF OR EQUAL TO
	strcpy(symbolMath[137].str, "⊈");	// \u2288	NEITHER A SUBSET OF NOR EQUAL TO
	strcpy(symbolMath[138].str, "⊉");	// \u2289	NEITHER A SUPERSET OF NOR EQUAL TO
	strcpy(symbolMath[139].str, "⊊");	// \u228A	SUBSET OF WITH NOT EQUAL TO
	strcpy(symbolMath[140].str, "⊋");	// \u228B	SUPERSET OF WITH NOT EQUAL TO
	strcpy(symbolMath[141].str, "⊌");	// \u228C	MULTISET
	strcpy(symbolMath[142].str, "⊍");	// \u228D	MULTISET MULTIPLICATION
	strcpy(symbolMath[143].str, "⊎");	// \u228E	MULTISET UNION
	strcpy(symbolMath[144].str, "⊏");	// \u228F	SQUARE IMAGE OF
	strcpy(symbolMath[145].str, "⊐");	// \u2290	SQUARE ORIGINAL OF
	strcpy(symbolMath[146].str, "⊑");	// \u2291	SQUARE IMAGE OF OR EQUAL TO
	strcpy(symbolMath[147].str, "⊒");	// \u2292	SQUARE ORIGINAL OF OR EQUAL TO
	strcpy(symbolMath[148].str, "⊓");	// \u2293	SQUARE CAP
	strcpy(symbolMath[149].str, "⊔");	// \u2294	SQUARE CUP
	strcpy(symbolMath[150].str, "⊕");	// \u2295	CIRCLED PLUS
	strcpy(symbolMath[151].str, "⊖");	// \u2296	CIRCLED MINUS
	strcpy(symbolMath[152].str, "⊗");	// \u2297	CIRCLED TIMES
	strcpy(symbolMath[153].str, "⊘");	// \u2298	CIRCLED DIVISION SLASH
	strcpy(symbolMath[154].str, "⊙");	// \u2299	CIRCLED DOT OPERATOR
	strcpy(symbolMath[155].str, "⊚");	// \u229A	CIRCLED RING OPERATOR
	strcpy(symbolMath[156].str, "⊛");	// \u229B	CIRCLED ASTERISK OPERATOR
	strcpy(symbolMath[157].str, "⊜");	// \u229C	CIRCLED EQUALS
	strcpy(symbolMath[158].str, "⊝");	// \u229D	CIRCLED DASH
	strcpy(symbolMath[159].str, "⊞");	// \u229E	SQUARED PLUS
	strcpy(symbolMath[160].str, "⊟");	// \u229F	SQUARED MINUS
	strcpy(symbolMath[161].str, "⊠");	// \u22A0	SQUARED TIMES
	strcpy(symbolMath[162].str, "⊡");	// \u22A1	SQUARED DOT OPERATOR
	strcpy(symbolMath[163].str, "⊢");	// \u22A2	RIGHT TACK
	strcpy(symbolMath[164].str, "⊣");	// \u22A3	LEFT TACK
	strcpy(symbolMath[165].str, "⊤");	// \u22A4	DOWN TACK
	strcpy(symbolMath[166].str, "⊥");	// \u22A5	UP TACK
	strcpy(symbolMath[167].str, "⊦");	// \u22A6	ASSERTION
	strcpy(symbolMath[168].str, "⊧");	// \u22A7	MODELS
	strcpy(symbolMath[169].str, "⊨");	// \u22A8	TRUE
	strcpy(symbolMath[170].str, "⊩");	// \u22A9	FORCES
	strcpy(symbolMath[171].str, "⊪");	// \u22AA	TRIPLE VERTICAL BAR RIGHT TURNSTILE
	strcpy(symbolMath[172].str, "⊫");	// \u22AB	DOUBLE VERTICAL BAR DOUBLE RIGHT TURNSTILE
	strcpy(symbolMath[173].str, "⊬");	// \u22AC	DOES NOT PROVE
	strcpy(symbolMath[174].str, "⊭");	// \u22AD	NOT TRUE
	strcpy(symbolMath[175].str, "⊮");	// \u22AE	DOES NOT FORCE
	strcpy(symbolMath[176].str, "⊯");	// \u22AF	NEGATED DOUBLE VERTICAL BAR DOUBLE RIGHT TURNSTILE
	strcpy(symbolMath[177].str, "⊰");	// \u22B0	PRECEDES UNDER RELATION
	strcpy(symbolMath[178].str, "⊱");	// \u22B1	SUCCEEDS UNDER RELATION
	strcpy(symbolMath[179].str, "⊲");	// \u22B2	NORMAL SUBGROUP OF
	strcpy(symbolMath[180].str, "⊳");	// \u22B3	CONTAINS AS NORMAL SUBGROUP
	strcpy(symbolMath[181].str, "⊴");	// \u22B4	NORMAL SUBGROUP OF OR EQUAL TO
	strcpy(symbolMath[182].str, "⊵");	// \u22B5	CONTAINS AS NORMAL SUBGROUP OR EQUAL TO
	strcpy(symbolMath[183].str, "⊶");	// \u22B6	ORIGINAL OF
	strcpy(symbolMath[184].str, "⊷");	// \u22B7	IMAGE OF
	strcpy(symbolMath[185].str, "⊸");	// \u22B8	MULTIMAP
	strcpy(symbolMath[186].str, "⊹");	// \u22B9	HERMITIAN CONJUGATE MATRIX
	strcpy(symbolMath[187].str, "⊺");	// \u22BA	INTERCALATE
	strcpy(symbolMath[188].str, "⊻");	// \u22BB	XOR
	strcpy(symbolMath[189].str, "⊼");	// \u22BC	NAND
	strcpy(symbolMath[190].str, "⊽");	// \u22BD	NOR
	strcpy(symbolMath[191].str, "⊾");	// \u22BE	RIGHT ANGLE WITH ARC
	strcpy(symbolMath[192].str, "⊿");	// \u22BF	RIGHT TRIANGLE
	strcpy(symbolMath[193].str, "⋀");	// \u22C0	N-ARY LOGICAL AND
	strcpy(symbolMath[194].str, "⋁");	// \u22C1	N-ARY LOGICAL OR
	strcpy(symbolMath[195].str, "⋂");	// \u22C2	N-ARY INTERSECTION
	strcpy(symbolMath[196].str, "⋃");	// \u22C3	N-ARY UNION
	strcpy(symbolMath[197].str, "⋄");	// \u22C4	DIAMOND OPERATOR
	strcpy(symbolMath[198].str, "⋅");	// \u22C5	DOT OPERATOR
	strcpy(symbolMath[199].str, "⋆");	// \u22C6	STAR OPERATOR
	strcpy(symbolMath[200].str, "⋇");	// \u22C7	DIVISION TIMES
	strcpy(symbolMath[201].str, "⋈");	// \u22C8	BOWTIE
	strcpy(symbolMath[202].str, "⋉");	// \u22C9	LEFT NORMAL FACTOR SEMIDIRECT PRODUCT
	strcpy(symbolMath[203].str, "⋊");	// \u22CA	RIGHT NORMAL FACTOR SEMIDIRECT PRODUCT
	strcpy(symbolMath[204].str, "⋋");	// \u22CB	LEFT SEMIDIRECT PRODUCT
	strcpy(symbolMath[205].str, "⋌");	// \u22CC	RIGHT SEMIDIRECT PRODUCT
	strcpy(symbolMath[206].str, "⋍");	// \u22CD	REVERSED TILDE EQUALS
	strcpy(symbolMath[207].str, "⋎");	// \u22CE	CURLY LOGICAL OR
	strcpy(symbolMath[208].str, "⋏");	// \u22CF	CURLY LOGICAL AND
	strcpy(symbolMath[209].str, "⋐");	// \u22D0	DOUBLE SUBSET
	strcpy(symbolMath[210].str, "⋑");	// \u22D1	DOUBLE SUPERSET
	strcpy(symbolMath[211].str, "⋒");	// \u22D2	DOUBLE INTERSECTION
	strcpy(symbolMath[212].str, "⋓");	// \u22D3	DOUBLE UNION
	strcpy(symbolMath[213].str, "⋔");	// \u22D4	PITCHFORK
	strcpy(symbolMath[214].str, "⋕");	// \u22D5	EQUAL AND PARALLEL TO
	strcpy(symbolMath[215].str, "⋖");	// \u22D6	LESS-THAN WITH DOT
	strcpy(symbolMath[216].str, "⋗");	// \u22D7	GREATER-THAN WITH DOT
	strcpy(symbolMath[217].str, "⋘");	// \u22D8	VERY MUCH LESS-THAN
	strcpy(symbolMath[218].str, "⋙");	// \u22D9	VERY MUCH GREATER-THAN
	strcpy(symbolMath[219].str, "⋚");	// \u22DA	LESS-THAN EQUAL TO OR GREATER-THAN
	strcpy(symbolMath[220].str, "⋛");	// \u22DB	GREATER-THAN EQUAL TO OR LESS-THAN
	strcpy(symbolMath[221].str, "⋜");	// \u22DC	EQUAL TO OR LESS-THAN
	strcpy(symbolMath[222].str, "⋝");	// \u22DD	EQUAL TO OR GREATER-THAN
	strcpy(symbolMath[223].str, "⋞");	// \u22DE	EQUAL TO OR PRECEDES
	strcpy(symbolMath[224].str, "⋟");	// \u22DF	EQUAL TO OR SUCCEEDS
	strcpy(symbolMath[225].str, "⋠");	// \u22	DOES NOT PRECEDE OR EQUAL
	strcpy(symbolMath[226].str, "⋡");	// \u220	DOES NOT SUCCEED OR EQUAL
	strcpy(symbolMath[227].str, "⋢");	// \u2200	NOT SQUARE IMAGE OF OR EQUAL TO
	strcpy(symbolMath[228].str, "⋣");	// \u22000	NOT SQUARE ORIGINAL OF OR EQUAL TO
	strcpy(symbolMath[229].str, "⋤");	// \u220000	SQUARE IMAGE OF OR NOT EQUAL TO
	strcpy(symbolMath[230].str, "⋥");	// \u2200000	SQUARE ORIGINAL OF OR NOT EQUAL TO
	strcpy(symbolMath[231].str, "⋦");	// \u22000000	LESS-THAN BUT NOT EQUIVALENT TO
	strcpy(symbolMath[232].str, "⋧");	// \u220000000	GREATER-THAN BUT NOT EQUIVALENT TO
	strcpy(symbolMath[233].str, "⋨");	// \u2200000000	PRECEDES BUT NOT EQUIVALENT TO
	strcpy(symbolMath[234].str, "⋩");	// \u22000000000	SUCCEEDS BUT NOT EQUIVALENT TO
	strcpy(symbolMath[235].str, "⋪");	// \u22EA	NOT NORMAL SUBGROUP OF
	strcpy(symbolMath[236].str, "⋫");	// \u22EB	DOES NOT CONTAIN AS NORMAL SUBGROUP
	strcpy(symbolMath[237].str, "⋬");	// \u22EC	NOT NORMAL SUBGROUP OF OR EQUAL TO
	strcpy(symbolMath[238].str, "⋭");	// \u22ED	DOES NOT CONTAIN AS NORMAL SUBGROUP OR EQUAL
	strcpy(symbolMath[239].str, "⋮");	// \u22EE	VERTICAL ELLIPSIS
	strcpy(symbolMath[240].str, "⋯");	// \u22EF	MIDLINE HORIZONTAL ELLIPSIS
	strcpy(symbolMath[241].str, "⋰");	// \u22F0	UP RIGHT DIAGONAL ELLIPSIS
	strcpy(symbolMath[242].str, "⋱");	// \u22F1	DOWN RIGHT DIAGONAL ELLIPSIS
	strcpy(symbolMath[243].str, "⋲");	// \u22F2	ELEMENT OF WITH LONG HORIZONTAL STROKE
	strcpy(symbolMath[244].str, "⋳");	// \u22F3	ELEMENT OF WITH VERTICAL BAR AT END OF HORIZONTAL STROKE
	strcpy(symbolMath[245].str, "⋴");	// \u22F4	SMALL ELEMENT OF WITH VERTICAL BAR AT END OF HORIZONTAL STROKE
	strcpy(symbolMath[246].str, "⋵");	// \u22F5	ELEMENT OF WITH DOT ABOVE
	strcpy(symbolMath[247].str, "⋶");	// \u22F6	ELEMENT OF WITH OVERBAR
	strcpy(symbolMath[248].str, "⋷");	// \u22F7	SMALL ELEMENT OF WITH OVERBAR
	strcpy(symbolMath[249].str, "⋸");	// \u22F8	ELEMENT OF WITH UNDERBAR
	strcpy(symbolMath[250].str, "⋹");	// \u22F9	ELEMENT OF WITH TWO HORIZONTAL STROKES
	strcpy(symbolMath[251].str, "⋺");	// \u22FA	CONTAINS WITH LONG HORIZONTAL STROKE
	strcpy(symbolMath[252].str, "⋻");	// \u22FB	CONTAINS WITH VERTICAL BAR AT END OF HORIZONTAL STROKE
	strcpy(symbolMath[253].str, "⋼");	// \u22FC	SMALL CONTAINS WITH VERTICAL BAR AT END OF HORIZONTAL STROKE
	strcpy(symbolMath[254].str, "⋽");	// \u22FD	CONTAINS WITH OVERBAR
	strcpy(symbolMath[255].str, "⋾");	// \u22FE	SMALL CONTAINS WITH OVERBAR
	strcpy(symbolMath[256].str, "⋿");	// \u22FF	Z NOTATION BAG MEMBERSHIP

	strcpy(symbolBlock[1].str, "▀");		// \u2580	UPPER HALF BLOCK
	strcpy(symbolBlock[2].str, "▁");	// \u2581	LOWER ONE EIGHTH BLOCK
	strcpy(symbolBlock[3].str, "▂");	// \u2582	LOWER ONE QUARTER BLOCK
	strcpy(symbolBlock[4].str, "▃");	// \u2583	LOWER THREE EIGHTHS BLOCK
	strcpy(symbolBlock[5].str, "▄");	// \u2584	LOWER HALF BLOCK
	strcpy(symbolBlock[6].str, "▅");	// \u2585	LOWER FIVE EIGHTHS BLOCK
	strcpy(symbolBlock[7].str, "▆");	// \u2586	LOWER THREE QUARTERS BLOCK
	strcpy(symbolBlock[8].str, "▇");	// \u2587	LOWER SEVEN EIGHTHS BLOCK
	strcpy(symbolBlock[9].str, "█");	// \u2588	FULL BLOCK
	strcpy(symbolBlock[10].str, "▉");	// \u2589	LEFT SEVEN EIGHTHS BLOCK
	strcpy(symbolBlock[11].str, "▊");	// \u258A	LEFT THREE QUARTERS BLOCK
	strcpy(symbolBlock[12].str, "▋");	// \u258B	LEFT FIVE EIGHTHS BLOCK
	strcpy(symbolBlock[13].str, "▌");	// \u258C	LEFT HALF BLOCK
	strcpy(symbolBlock[14].str, "▍");	// \u258D	LEFT THREE EIGHTHS BLOCK
	strcpy(symbolBlock[15].str, "▎");	// \u258E	LEFT ONE QUARTER BLOCK
	strcpy(symbolBlock[16].str, "▏");	// \u258F	LEFT ONE EIGHTH BLOCK
	strcpy(symbolBlock[17].str, "▐");	// \u2590	RIGHT HALF BLOCK
	strcpy(symbolBlock[18].str, "░");	// \u2591	LIGHT SHADE
	strcpy(symbolBlock[19].str, "▒");	// \u2592	MEDIUM SHADE
	strcpy(symbolBlock[20].str, "▓");	// \u2593	DARK SHADE
	strcpy(symbolBlock[21].str, "▔");	// \u2594	UPPER ONE EIGHTH BLOCK
	strcpy(symbolBlock[22].str, "▕");	// \u2595	RIGHT ONE EIGHTH BLOCK
	strcpy(symbolBlock[23].str, "▖");	// \u2596	QUADRANT LOWER LEFT
	strcpy(symbolBlock[24].str, "▗");	// \u2597	QUADRANT LOWER RIGHT
	strcpy(symbolBlock[25].str, "▘");	// \u2598	QUADRANT UPPER LEFT
	strcpy(symbolBlock[26].str, "▙");	// \u2599	QUADRANT UPPER LEFT AND LOWER LEFT AND LOWER RIGHT
	strcpy(symbolBlock[27].str, "▚");	// \u259A	QUADRANT UPPER LEFT AND LOWER RIGHT
	strcpy(symbolBlock[28].str, "▛");	// \u259B	QUADRANT UPPER LEFT AND UPPER RIGHT AND LOWER LEFT
	strcpy(symbolBlock[29].str, "▜");	// \u259C	QUADRANT UPPER LEFT AND UPPER RIGHT AND LOWER RIGHT
	strcpy(symbolBlock[30].str, "▝");	// \u259D	QUADRANT UPPER RIGHT
	strcpy(symbolBlock[31].str, "▞");	// \u259E	QUADRANT UPPER RIGHT AND LOWER LEFT
	strcpy(symbolBlock[32].str, "▟");	// \u259F	QUADRANT UPPER RIGHT AND LOWER LEFT AND LOWER RIGHT

	strcpy(symbolMisc[1].str, "☀");		// \u2600	BLACK SUN WITH RAYS
	strcpy(symbolMisc[2].str, "☁");	// \u2601	CLOUD
	strcpy(symbolMisc[3].str, "☂");	// \u2602	UMBRELLA
	strcpy(symbolMisc[4].str, "☃");	// \u2603	SNOWMAN
	strcpy(symbolMisc[5].str, "☄");	// \u2604	COMET
	strcpy(symbolMisc[6].str, "★");	// \u2605	BLACK STAR
	strcpy(symbolMisc[7].str, "☆");	// \u2606	WHITE STAR
	strcpy(symbolMisc[8].str, "☇");	// \u2607	LIGHTNING
	strcpy(symbolMisc[9].str, "☈");	// \u2608	THUNDERSTORM
	strcpy(symbolMisc[10].str, "☉");	// \u2609	SUN
	strcpy(symbolMisc[11].str, "☊");	// \u260A	ASCENDING NODE
	strcpy(symbolMisc[12].str, "☋");	// \u260B	DESCENDING NODE
	strcpy(symbolMisc[13].str, "☌");	// \u260C	CONJUNCTION
	strcpy(symbolMisc[14].str, "☍");	// \u260D	OPPOSITION
	strcpy(symbolMisc[15].str, "☎");	// \u260E	BLACK TELEPHONE
	strcpy(symbolMisc[16].str, "☏");	// \u260F	WHITE TELEPHONE
	strcpy(symbolMisc[17].str, "☐");	// \u2610	BALLOT BOX
	strcpy(symbolMisc[18].str, "☑");	// \u2611	BALLOT BOX WITH CHECK
	strcpy(symbolMisc[19].str, "☒");	// \u2612	BALLOT BOX WITH X
	strcpy(symbolMisc[20].str, "☓");	// \u2613	SALTIRE
	strcpy(symbolMisc[21].str, "☔");	// \u2614	UMBRELLA WITH RAIN DROPS
	strcpy(symbolMisc[22].str, "☕");	// \u2615	HOT BEVERAGE
	strcpy(symbolMisc[23].str, "☖");	// \u2616	WHITE SHOGI PIECE
	strcpy(symbolMisc[24].str, "☗");	// \u2617	BLACK SHOGI PIECE
	strcpy(symbolMisc[25].str, "☘");	// \u2618	SHAMROCK
	strcpy(symbolMisc[26].str, "☙");	// \u2619	REVERSED ROTATED FLORAL HEART BULLET
	strcpy(symbolMisc[27].str, "☚");	// \u261A	BLACK LEFT POINTING INDEX
	strcpy(symbolMisc[28].str, "☛");	// \u261B	BLACK RIGHT POINTING INDEX
	strcpy(symbolMisc[29].str, "☜");	// \u261C	WHITE LEFT POINTING INDEX
	strcpy(symbolMisc[30].str, "☝");	// \u261D	WHITE UP POINTING INDEX
	strcpy(symbolMisc[31].str, "☞");	// \u261E	WHITE RIGHT POINTING INDEX
	strcpy(symbolMisc[32].str, "☟");	// \u261F	WHITE DOWN POINTING INDEX
	strcpy(symbolMisc[33].str, "☠");	// \u2620	SKULL AND CROSSBONES
	strcpy(symbolMisc[34].str, "☡");	// \u2621	CAUTION SIGN
	strcpy(symbolMisc[35].str, "☢");	// \u2622	RADIOACTIVE SIGN
	strcpy(symbolMisc[36].str, "☣");	// \u2623	BIOHAZARD SIGN
	strcpy(symbolMisc[37].str, "☤");	// \u2624	CADUCEUS
	strcpy(symbolMisc[38].str, "☥");	// \u2625	ANKH
	strcpy(symbolMisc[39].str, "☦");	// \u2626	ORTHODOX CROSS
	strcpy(symbolMisc[40].str, "☧");	// \u2627	CHI RHO
	strcpy(symbolMisc[41].str, "☨");	// \u2628	CROSS OF LORRAINE
	strcpy(symbolMisc[42].str, "☩");	// \u2629	CROSS OF JERUSALEM
	strcpy(symbolMisc[43].str, "☪");	// \u262A	STAR AND CRESCENT
	strcpy(symbolMisc[44].str, "☫");	// \u262B	FARSI SYMBOL
	strcpy(symbolMisc[45].str, "☬");	// \u262C	KHANDA
	strcpy(symbolMisc[46].str, "☭");	// \u262D	HAMMER AND SICKLE
	strcpy(symbolMisc[47].str, "☮");	// \u262E	PEACE SYMBOL
	strcpy(symbolMisc[48].str, "☯");	// \u262F	YIN YANG
	strcpy(symbolMisc[49].str, "☰");	// \u2630	TRIGRAM FOR HEAVEN
	strcpy(symbolMisc[50].str, "☱");	// \u2631	TRIGRAM FOR LAKE
	strcpy(symbolMisc[51].str, "☲");	// \u2632	TRIGRAM FOR FIRE
	strcpy(symbolMisc[52].str, "☳");	// \u2633	TRIGRAM FOR THUNDER
	strcpy(symbolMisc[53].str, "☴");	// \u2634	TRIGRAM FOR WIND
	strcpy(symbolMisc[54].str, "☵");	// \u2635	TRIGRAM FOR WATER
	strcpy(symbolMisc[55].str, "☶");	// \u2636	TRIGRAM FOR MOUNTAIN
	strcpy(symbolMisc[56].str, "☷");	// \u2637	TRIGRAM FOR EARTH
	strcpy(symbolMisc[57].str, "☸");	// \u2638	WHEEL OF DHARMA
	strcpy(symbolMisc[58].str, "☹");	// \u2639	WHITE FROWNING FACE
	strcpy(symbolMisc[59].str, "☺");	// \u263A	WHITE SMILING FACE
	strcpy(symbolMisc[60].str, "☻");	// \u263B	BLACK SMILING FACE
	strcpy(symbolMisc[61].str, "☼");	// \u263C	WHITE SUN WITH RAYS
	strcpy(symbolMisc[62].str, "☽");	// \u263D	FIRST QUARTER MOON
	strcpy(symbolMisc[63].str, "☾");	// \u263E	LAST QUARTER MOON
	strcpy(symbolMisc[64].str, "☿");	// \u263F	MERCURY
	strcpy(symbolMisc[65].str, "♀");	// \u2640	FEMALE SIGN
	strcpy(symbolMisc[66].str, "♁");	// \u2641	EARTH
	strcpy(symbolMisc[67].str, "♂");	// \u2642	MALE SIGN
	strcpy(symbolMisc[68].str, "♃");	// \u2643	JUPITER
	strcpy(symbolMisc[69].str, "♄");	// \u2644	SATURN
	strcpy(symbolMisc[70].str, "♅");	// \u2645	URANUS
	strcpy(symbolMisc[71].str, "♆");	// \u2646	NEPTUNE
	strcpy(symbolMisc[72].str, "♇");	// \u2647	PLUTO
	strcpy(symbolMisc[73].str, "♈");	// \u2648	ARIES
	strcpy(symbolMisc[74].str, "♉");	// \u2649	TAURUS
	strcpy(symbolMisc[75].str, "♊");	// \u264A	GEMINI
	strcpy(symbolMisc[76].str, "♋");	// \u264B	CANCER
	strcpy(symbolMisc[77].str, "♌");	// \u264C	LEO
	strcpy(symbolMisc[78].str, "♍");	// \u264D	VIRGO
	strcpy(symbolMisc[79].str, "♎");	// \u264E	LIBRA
	strcpy(symbolMisc[80].str, "♏");	// \u264F	SCORPIUS
	strcpy(symbolMisc[81].str, "♐");	// \u2650	SAGITTARIUS
	strcpy(symbolMisc[82].str, "♑");	// \u2651	CAPRICORN
	strcpy(symbolMisc[83].str, "♒");	// \u2652	AQUARIUS
	strcpy(symbolMisc[84].str, "♓");	// \u2653	PISCES
	strcpy(symbolMisc[85].str, "♔");	// \u2654	WHITE CHESS KING
	strcpy(symbolMisc[86].str, "♕");	// \u2655	WHITE CHESS QUEEN
	strcpy(symbolMisc[87].str, "♖");	// \u2656	WHITE CHESS ROOK
	strcpy(symbolMisc[88].str, "♗");	// \u2657	WHITE CHESS BISHOP
	strcpy(symbolMisc[89].str, "♘");	// \u2658	WHITE CHESS KNIGHT
	strcpy(symbolMisc[90].str, "♙");	// \u2659	WHITE CHESS PAWN
	strcpy(symbolMisc[91].str, "♚");	// \u265A	BLACK CHESS KING
	strcpy(symbolMisc[92].str, "♛");	// \u265B	BLACK CHESS QUEEN
	strcpy(symbolMisc[93].str, "♜");	// \u265C	BLACK CHESS ROOK
	strcpy(symbolMisc[94].str, "♝");	// \u265D	BLACK CHESS BISHOP
	strcpy(symbolMisc[95].str, "♞");	// \u265E	BLACK CHESS KNIGHT
	strcpy(symbolMisc[96].str, "♟");	// \u265F	BLACK CHESS PAWN
	strcpy(symbolMisc[97].str, "♠");	// \u2660	BLACK SPADE SUIT
	strcpy(symbolMisc[98].str, "♡");	// \u2661	WHITE HEART SUIT
	strcpy(symbolMisc[99].str, "♢");	// \u2662	WHITE DIAMOND SUIT
	strcpy(symbolMisc[100].str, "♣");	// \u2663	BLACK CLUB SUIT
	strcpy(symbolMisc[101].str, "♤");	// \u2664	WHITE SPADE SUIT
	strcpy(symbolMisc[102].str, "♥");	// \u2665	BLACK HEART SUIT
	strcpy(symbolMisc[103].str, "♦");	// \u2666	BLACK DIAMOND SUIT
	strcpy(symbolMisc[104].str, "♧");	// \u2667	WHITE CLUB SUIT
	strcpy(symbolMisc[105].str, "♨");	// \u2668	HOT SPRINGS
	strcpy(symbolMisc[106].str, "♩");	// \u2669	QUARTER NOTE
	strcpy(symbolMisc[107].str, "♪");	// \u266A	EIGHTH NOTE
	strcpy(symbolMisc[108].str, "♫");	// \u266B	BEAMED EIGHTH NOTES
	strcpy(symbolMisc[109].str, "♬");	// \u266C	BEAMED SIXTEENTH NOTES
	strcpy(symbolMisc[110].str, "♭");	// \u266D	MUSIC FLAT SIGN
	strcpy(symbolMisc[111].str, "♮");	// \u266E	MUSIC NATURAL SIGN
	strcpy(symbolMisc[112].str, "♯");	// \u266F	MUSIC SHARP SIGN
	strcpy(symbolMisc[113].str, "♰");	// \u2670	WEST SYRIAC CROSS
	strcpy(symbolMisc[114].str, "♱");	// \u2671	EAST SYRIAC CROSS
	strcpy(symbolMisc[115].str, "♲");	// \u2672	UNIVERSAL RECYCLING SYMBOL
	strcpy(symbolMisc[116].str, "♳");	// \u2673	RECYCLING SYMBOL FOR TYPE-1 PLASTICS
	strcpy(symbolMisc[117].str, "♴");	// \u2674	RECYCLING SYMBOL FOR TYPE-2 PLASTICS
	strcpy(symbolMisc[118].str, "♵");	// \u2675	RECYCLING SYMBOL FOR TYPE-3 PLASTICS
	strcpy(symbolMisc[119].str, "♶");	// \u2676	RECYCLING SYMBOL FOR TYPE-4 PLASTICS
	strcpy(symbolMisc[120].str, "♷");	// \u2677	RECYCLING SYMBOL FOR TYPE-5 PLASTICS
	strcpy(symbolMisc[121].str, "♸");	// \u2678	RECYCLING SYMBOL FOR TYPE-6 PLASTICS
	strcpy(symbolMisc[122].str, "♹");	// \u2679	RECYCLING SYMBOL FOR TYPE-7 PLASTICS
	strcpy(symbolMisc[123].str, "♺");	// \u267A	RECYCLING SYMBOL FOR GENERIC MATERIALS
	strcpy(symbolMisc[124].str, "♻");	// \u267B	BLACK UNIVERSAL RECYCLING SYMBOL
	strcpy(symbolMisc[125].str, "♼");	// \u267C	RECYCLED PAPER SYMBOL
	strcpy(symbolMisc[126].str, "♽");	// \u267D	PARTIALLY-RECYCLED PAPER SYMBOL
	strcpy(symbolMisc[127].str, "♾");	// \u267E	PERMANENT PAPER SIGN
	strcpy(symbolMisc[128].str, "♿");	// \u267F	WHEELCHAIR SYMBOL
	strcpy(symbolMisc[129].str, "⚀");	// \u2680	DIE FACE-1
	strcpy(symbolMisc[130].str, "⚁");	// \u2681	DIE FACE-2
	strcpy(symbolMisc[131].str, "⚂");	// \u2682	DIE FACE-3
	strcpy(symbolMisc[132].str, "⚃");	// \u2683	DIE FACE-4
	strcpy(symbolMisc[133].str, "⚄");	// \u2684	DIE FACE-5
	strcpy(symbolMisc[134].str, "⚅");	// \u2685	DIE FACE-6
	strcpy(symbolMisc[135].str, "⚆");	// \u2686	WHITE CIRCLE WITH DOT RIGHT
	strcpy(symbolMisc[136].str, "⚇");	// \u2687	WHITE CIRCLE WITH TWO DOTS
	strcpy(symbolMisc[137].str, "⚈");	// \u2688	BLACK CIRCLE WITH WHITE DOT RIGHT
	strcpy(symbolMisc[138].str, "⚉");	// \u2689	BLACK CIRCLE WITH TWO WHITE DOTS
	strcpy(symbolMisc[139].str, "⚊");	// \u268A	MONOGRAM FOR YANG
	strcpy(symbolMisc[140].str, "⚋");	// \u268B	MONOGRAM FOR YIN
	strcpy(symbolMisc[141].str, "⚌");	// \u268C	DIGRAM FOR GREATER YANG
	strcpy(symbolMisc[142].str, "⚍");	// \u268D	DIGRAM FOR LESSER YIN
	strcpy(symbolMisc[143].str, "⚎");	// \u268E	DIGRAM FOR LESSER YANG
	strcpy(symbolMisc[144].str, "⚏");	// \u268F	DIGRAM FOR GREATER YIN
	strcpy(symbolMisc[145].str, "⚐");	// \u2690	WHITE FLAG
	strcpy(symbolMisc[146].str, "⚑");	// \u2691	BLACK FLAG
	strcpy(symbolMisc[147].str, "⚒");	// \u2692	HAMMER AND PICK
	strcpy(symbolMisc[148].str, "⚓");	// \u2693	ANCHOR
	strcpy(symbolMisc[149].str, "⚔");	// \u2694	CROSSED SWORDS
	strcpy(symbolMisc[150].str, "⚕");	// \u2695	STAFF OF AESCULAPIUS
	strcpy(symbolMisc[151].str, "⚖");	// \u2696	SCALES
	strcpy(symbolMisc[152].str, "⚗");	// \u2697	ALEMBIC
	strcpy(symbolMisc[153].str, "⚘");	// \u2698	FLOWER
	strcpy(symbolMisc[154].str, "⚙");	// \u2699	GEAR
	strcpy(symbolMisc[155].str, "⚚");	// \u269A	STAFF OF HERMES
	strcpy(symbolMisc[156].str, "⚛");	// \u269B	ATOM SYMBOL
	strcpy(symbolMisc[157].str, "⚜");	// \u269C	FLEUR-DE-LIS
	strcpy(symbolMisc[158].str, "⚝");	// \u269D	OUTLINED WHITE STAR
	strcpy(symbolMisc[159].str, "⚞");	// \u269E	THREE LINES CONVERGING RIGHT
	strcpy(symbolMisc[160].str, "⚟");	// \u269F	THREE LINES CONVERGING LEFT
	strcpy(symbolMisc[161].str, "⚠");	// \u26A0	WARNING SIGN
	strcpy(symbolMisc[162].str, "⚡");	// \u26A1	HIGH VOLTAGE SIGN
	strcpy(symbolMisc[163].str, "⚢");	// \u26A2	DOUBLED FEMALE SIGN
	strcpy(symbolMisc[164].str, "⚣");	// \u26A3	DOUBLED MALE SIGN
	strcpy(symbolMisc[165].str, "⚤");	// \u26A4	INTERLOCKED FEMALE AND MALE SIGN
	strcpy(symbolMisc[166].str, "⚥");	// \u26A5	MALE AND FEMALE SIGN
	strcpy(symbolMisc[167].str, "⚦");	// \u26A6	MALE WITH STROKE SIGN
	strcpy(symbolMisc[168].str, "⚧");	// \u26A7	MALE WITH STROKE AND MALE AND FEMALE SIGN
	strcpy(symbolMisc[169].str, "⚨");	// \u26A8	VERTICAL MALE WITH STROKE SIGN
	strcpy(symbolMisc[170].str, "⚩");	// \u26A9	HORIZONTAL MALE WITH STROKE SIGN
	strcpy(symbolMisc[171].str, "⚪");	// \u26AA	MEDIUM WHITE CIRCLE
	strcpy(symbolMisc[172].str, "⚫");	// \u26AB	MEDIUM BLACK CIRCLE
	strcpy(symbolMisc[173].str, "⚬");	// \u26AC	MEDIUM SMALL WHITE CIRCLE
	strcpy(symbolMisc[174].str, "⚭");	// \u26AD	MARRIAGE SYMBOL
	strcpy(symbolMisc[175].str, "⚮");	// \u26AE	DIVORCE SYMBOL
	strcpy(symbolMisc[176].str, "⚯");	// \u26AF	UNMARRIED PARTNERSHIP SYMBOL
	strcpy(symbolMisc[177].str, "⚰");	// \u26B0	COFFIN
	strcpy(symbolMisc[178].str, "⚱");	// \u26B1	FUNERAL URN
	strcpy(symbolMisc[179].str, "⚲");	// \u26B2	NEUTER
	strcpy(symbolMisc[180].str, "⚳");	// \u26B3	CERES
	strcpy(symbolMisc[181].str, "⚴");	// \u26B4	PALLAS
	strcpy(symbolMisc[182].str, "⚵");	// \u26B5	JUNO
	strcpy(symbolMisc[183].str, "⚶");	// \u26B6	VESTA
	strcpy(symbolMisc[184].str, "⚷");	// \u26B7	CHIRON
	strcpy(symbolMisc[185].str, "⚸");	// \u26B8	BLACK MOON LILITH
	strcpy(symbolMisc[186].str, "⚹");	// \u26B9	SEXTILE
	strcpy(symbolMisc[187].str, "⚺");	// \u26BA	SEMISEXTILE
	strcpy(symbolMisc[188].str, "⚻");	// \u26BB	QUINCUNX
	strcpy(symbolMisc[189].str, "⚼");	// \u26BC	SESQUIQUADRATE
	strcpy(symbolMisc[190].str, "⚽");	// \u26BD	SOCCER BALL
	strcpy(symbolMisc[191].str, "⚾");	// \u26BE	BASEBALL
	strcpy(symbolMisc[192].str, "⚿");	// \u26BF	SQUARED KEY
	strcpy(symbolMisc[193].str, "⛀");	// \u26C0	WHITE DRAUGHTS MAN
	strcpy(symbolMisc[194].str, "⛁");	// \u26C1	WHITE DRAUGHTS KING
	strcpy(symbolMisc[195].str, "⛂");	// \u26C2	BLACK DRAUGHTS MAN
	strcpy(symbolMisc[196].str, "⛃");	// \u26C3	BLACK DRAUGHTS KING
	strcpy(symbolMisc[197].str, "⛄");	// \u26C4	SNOWMAN WITHOUT SNOW
	strcpy(symbolMisc[198].str, "⛅");	// \u26C5	SUN BEHIND CLOUD
	strcpy(symbolMisc[199].str, "⛆");	// \u26C6	RAIN
	strcpy(symbolMisc[200].str, "⛇");	// \u26C7	BLACK SNOWMAN
	strcpy(symbolMisc[201].str, "⛈");	// \u26C8	THUNDER CLOUD AND RAIN
	strcpy(symbolMisc[202].str, "⛉");	// \u26C9	TURNED WHITE SHOGI PIECE
	strcpy(symbolMisc[203].str, "⛊");	// \u26CA	TURNED BLACK SHOGI PIECE
	strcpy(symbolMisc[204].str, "⛋");	// \u26CB	WHITE DIAMOND IN SQUARE
	strcpy(symbolMisc[205].str, "⛌");	// \u26CC	CROSSING LANES
	strcpy(symbolMisc[206].str, "⛍");	// \u26CD	DISABLED CAR
	strcpy(symbolMisc[207].str, "⛎");	// \u26CE	OPHIUCHUS
	strcpy(symbolMisc[208].str, "⛏");	// \u26CF	PICK
	strcpy(symbolMisc[209].str, "⛐");	// \u26D0	CAR SLIDING
	strcpy(symbolMisc[210].str, "⛑");	// \u26D1	HELMET WITH WHITE CROSS
	strcpy(symbolMisc[211].str, "⛒");	// \u26D2	CIRCLED CROSSING LANES
	strcpy(symbolMisc[212].str, "⛓");	// \u26D3	CHAINS
	strcpy(symbolMisc[213].str, "⛔");	// \u26D4	NO ENTRY
	strcpy(symbolMisc[214].str, "⛕");	// \u26D5	ALTERNATE ONE-WAY LEFT WAY TRAFFIC
	strcpy(symbolMisc[215].str, "⛖");	// \u26D6	BLACK TWO-WAY LEFT WAY TRAFFIC
	strcpy(symbolMisc[216].str, "⛗");	// \u26D7	WHITE TWO-WAY LEFT WAY TRAFFIC
	strcpy(symbolMisc[217].str, "⛘");	// \u26D8	BLACK LEFT LANE MERGE
	strcpy(symbolMisc[218].str, "⛙");	// \u26D9	WHITE LEFT LANE MERGE
	strcpy(symbolMisc[219].str, "⛚");	// \u26DA	DRIVE SLOW SIGN
	strcpy(symbolMisc[220].str, "⛛");	// \u26DB	HEAVY WHITE DOWN-POINTING TRIANGLE
	strcpy(symbolMisc[221].str, "⛜");	// \u26DC	LEFT CLOSED ENTRY
	strcpy(symbolMisc[222].str, "⛝");	// \u26DD	SQUARED SALTIRE
	strcpy(symbolMisc[223].str, "⛞");	// \u26DE	FALLING DIAGONAL IN WHITE CIRCLE IN BLACK SQUARE
	strcpy(symbolMisc[224].str, "⛟");	// \u26DF	BLACK TRUCK
	strcpy(symbolMisc[225].str, "⛠");	// \u26	RESTRICTED LEFT ENTRY-1
	strcpy(symbolMisc[226].str, "⛡");	// \u260	RESTRICTED LEFT ENTRY-2
	strcpy(symbolMisc[227].str, "⛢");	// \u2600	ASTRONOMICAL SYMBOL FOR URANUS
	strcpy(symbolMisc[228].str, "⛣");	// \u26000	HEAVY CIRCLE WITH STROKE AND TWO DOTS ABOVE
	strcpy(symbolMisc[229].str, "⛤");	// \u260000	PENTAGRAM
	strcpy(symbolMisc[230].str, "⛥");	// \u2600000	RIGHT-HANDED INTERLACED PENTAGRAM
	strcpy(symbolMisc[231].str, "⛦");	// \u26000000	LEFT-HANDED INTERLACED PENTAGRAM
	strcpy(symbolMisc[232].str, "⛧");	// \u260000000	INVERTED PENTAGRAM
	strcpy(symbolMisc[233].str, "⛨");	// \u2600000000	BLACK CROSS ON SHIELD
	strcpy(symbolMisc[234].str, "⛩");	// \u26000000000	SHINTO SHRINE
	strcpy(symbolMisc[235].str, "⛪");	// \u26EA	CHURCH
	strcpy(symbolMisc[236].str, "⛫");	// \u26EB	CASTLE
	strcpy(symbolMisc[237].str, "⛬");	// \u26EC	HISTORIC SITE
	strcpy(symbolMisc[238].str, "⛭");	// \u26ED	GEAR WITHOUT HUB
	strcpy(symbolMisc[239].str, "⛮");	// \u26EE	GEAR WITH HANDLES
	strcpy(symbolMisc[240].str, "⛯");	// \u26EF	MAP SYMBOL FOR LIGHTHOUSE
	strcpy(symbolMisc[241].str, "⛰");	// \u26F0	MOUNTAIN
	strcpy(symbolMisc[242].str, "⛱");	// \u26F1	UMBRELLA ON GROUND
	strcpy(symbolMisc[243].str, "⛲");	// \u26F2	FOUNTAIN
	strcpy(symbolMisc[244].str, "⛳");	// \u26F3	FLAG IN HOLE
	strcpy(symbolMisc[245].str, "⛴");	// \u26F4	FERRY
	strcpy(symbolMisc[246].str, "⛵");	// \u26F5	SAILBOAT
	strcpy(symbolMisc[247].str, "⛶");	// \u26F6	SQUARE FOUR CORNERS
	strcpy(symbolMisc[248].str, "⛷");	// \u26F7	SKIER
	strcpy(symbolMisc[249].str, "⛸");	// \u26F8	ICE SKATE
	strcpy(symbolMisc[250].str, "⛹");	// \u26F9	PERSON WITH BALL
	strcpy(symbolMisc[251].str, "⛺");	// \u26FA	TENT
	strcpy(symbolMisc[252].str, "⛻");	// \u26FB	JAPANESE BANK SYMBOL
	strcpy(symbolMisc[253].str, "⛼");	// \u26FC	HEADSTONE GRAVEYARD SYMBOL
	strcpy(symbolMisc[254].str, "⛽");	// \u26FD	FUEL PUMP
	strcpy(symbolMisc[255].str, "⛾");	// \u26FE	CUP ON BLACK SQUARE
	strcpy(symbolMisc[256].str, "⛿");	// \u26FF	WHITE FLAG WITH HORIZONTAL MIDDLE BLACK STRIPE

	strcpy(symbolDingbats[1].str, "✁");		// \u2701	UPPER BLADE SCISSORS
	strcpy(symbolDingbats[2].str, "✂");	// \u2702	BLACK SCISSORS
	strcpy(symbolDingbats[3].str, "✃");	// \u2703	LOWER BLADE SCISSORS
	strcpy(symbolDingbats[4].str, "✄");	// \u2704	WHITE SCISSORS
	strcpy(symbolDingbats[5].str, "✅");	// \u2705	WHITE HEAVY CHECK MARK
	strcpy(symbolDingbats[6].str, "✆");	// \u2706	TELEPHONE LOCATION SIGN
	strcpy(symbolDingbats[7].str, "✇");	// \u2707	TAPE DRIVE
	strcpy(symbolDingbats[8].str, "✈");	// \u2708	AIRPLANE
	strcpy(symbolDingbats[9].str, "✉");	// \u2709	ENVELOPE
	strcpy(symbolDingbats[10].str, "✊");	// \u270A	RAISED FIST
	strcpy(symbolDingbats[11].str, "✋");	// \u270B	RAISED HAND
	strcpy(symbolDingbats[12].str, "✌");	// \u270C	VICTORY HAND
	strcpy(symbolDingbats[13].str, "✍");	// \u270D	WRITING HAND
	strcpy(symbolDingbats[14].str, "✎");	// \u270E	LOWER RIGHT PENCIL
	strcpy(symbolDingbats[15].str, "✏");	// \u270F	PENCIL
	strcpy(symbolDingbats[16].str, "✐");	// \u2710	UPPER RIGHT PENCIL
	strcpy(symbolDingbats[17].str, "✑");	// \u2711	WHITE NIB
	strcpy(symbolDingbats[18].str, "✒");	// \u2712	BLACK NIB
	strcpy(symbolDingbats[19].str, "✓");	// \u2713	CHECK MARK
	strcpy(symbolDingbats[20].str, "✔");	// \u2714	HEAVY CHECK MARK
	strcpy(symbolDingbats[21].str, "✕");	// \u2715	MULTIPLICATION X
	strcpy(symbolDingbats[22].str, "✖");	// \u2716	HEAVY MULTIPLICATION X
	strcpy(symbolDingbats[23].str, "✗");	// \u2717	BALLOT X
	strcpy(symbolDingbats[24].str, "✘");	// \u2718	HEAVY BALLOT X
	strcpy(symbolDingbats[25].str, "✙");	// \u2719	OUTLINED GREEK CROSS
	strcpy(symbolDingbats[26].str, "✚");	// \u271A	HEAVY GREEK CROSS
	strcpy(symbolDingbats[27].str, "✛");	// \u271B	OPEN CENTRE CROSS
	strcpy(symbolDingbats[28].str, "✜");	// \u271C	HEAVY OPEN CENTRE CROSS
	strcpy(symbolDingbats[29].str, "✝");	// \u271D	LATIN CROSS
	strcpy(symbolDingbats[30].str, "✞");	// \u271E	SHADOWED WHITE LATIN CROSS
	strcpy(symbolDingbats[31].str, "✟");	// \u271F	OUTLINED LATIN CROSS
	strcpy(symbolDingbats[32].str, "✠");	// \u2720	MALTESE CROSS
	strcpy(symbolDingbats[33].str, "✡");	// \u2721	STAR OF DAVID
	strcpy(symbolDingbats[34].str, "✢");	// \u2722	FOUR TEARDROP-SPOKED ASTERISK
	strcpy(symbolDingbats[35].str, "✣");	// \u2723	FOUR BALLOON-SPOKED ASTERISK
	strcpy(symbolDingbats[36].str, "✤");	// \u2724	HEAVY FOUR BALLOON-SPOKED ASTERISK
	strcpy(symbolDingbats[37].str, "✥");	// \u2725	FOUR CLUB-SPOKED ASTERISK
	strcpy(symbolDingbats[38].str, "✦");	// \u2726	BLACK FOUR POINTED STAR
	strcpy(symbolDingbats[39].str, "✧");	// \u2727	WHITE FOUR POINTED STAR
	strcpy(symbolDingbats[40].str, "✨");	// \u2728	SPARKLES
	strcpy(symbolDingbats[41].str, "✩");	// \u2729	STRESS OUTLINED WHITE STAR
	strcpy(symbolDingbats[42].str, "✪");	// \u272A	CIRCLED WHITE STAR
	strcpy(symbolDingbats[43].str, "✫");	// \u272B	OPEN CENTRE BLACK STAR
	strcpy(symbolDingbats[44].str, "✬");	// \u272C	BLACK CENTRE WHITE STAR
	strcpy(symbolDingbats[45].str, "✭");	// \u272D	OUTLINED BLACK STAR
	strcpy(symbolDingbats[46].str, "✮");	// \u272E	HEAVY OUTLINED BLACK STAR
	strcpy(symbolDingbats[47].str, "✯");	// \u272F	PINWHEEL STAR
	strcpy(symbolDingbats[48].str, "✰");	// \u2730	SHADOWED WHITE STAR
	strcpy(symbolDingbats[49].str, "✱");	// \u2731	HEAVY ASTERISK
	strcpy(symbolDingbats[50].str, "✲");	// \u2732	OPEN CENTRE ASTERISK
	strcpy(symbolDingbats[51].str, "✳");	// \u2733	EIGHT SPOKED ASTERISK
	strcpy(symbolDingbats[52].str, "✴");	// \u2734	EIGHT POINTED BLACK STAR
	strcpy(symbolDingbats[53].str, "✵");	// \u2735	EIGHT POINTED PINWHEEL STAR
	strcpy(symbolDingbats[54].str, "✶");	// \u2736	SIX POINTED BLACK STAR
	strcpy(symbolDingbats[55].str, "✷");	// \u2737	EIGHT POINTED RECTILINEAR BLACK STAR
	strcpy(symbolDingbats[56].str, "✸");	// \u2738	HEAVY EIGHT POINTED RECTILINEAR BLACK STAR
	strcpy(symbolDingbats[57].str, "✹");	// \u2739	TWELVE POINTED BLACK STAR
	strcpy(symbolDingbats[58].str, "✺");	// \u273A	SIXTEEN POINTED ASTERISK
	strcpy(symbolDingbats[59].str, "✻");	// \u273B	TEARDROP-SPOKED ASTERISK
	strcpy(symbolDingbats[60].str, "✼");	// \u273C	OPEN CENTRE TEARDROP-SPOKED ASTERISK
	strcpy(symbolDingbats[61].str, "✽");	// \u273D	HEAVY TEARDROP-SPOKED ASTERISK
	strcpy(symbolDingbats[62].str, "✾");	// \u273E	SIX PETALLED BLACK AND WHITE FLORETTE
	strcpy(symbolDingbats[63].str, "✿");	// \u273F	BLACK FLORETTE
	strcpy(symbolDingbats[64].str, "❀");	// \u2740	WHITE FLORETTE
	strcpy(symbolDingbats[65].str, "❁");	// \u2741	EIGHT PETALLED OUTLINED BLACK FLORETTE
	strcpy(symbolDingbats[66].str, "❂");	// \u2742	CIRCLED OPEN CENTRE EIGHT POINTED STAR
	strcpy(symbolDingbats[67].str, "❃");	// \u2743	HEAVY TEARDROP-SPOKED PINWHEEL ASTERISK
	strcpy(symbolDingbats[68].str, "❄");	// \u2744	SNOWFLAKE
	strcpy(symbolDingbats[69].str, "❅");	// \u2745	TIGHT TRIFOLIATE SNOWFLAKE
	strcpy(symbolDingbats[70].str, "❆");	// \u2746	HEAVY CHEVRON SNOWFLAKE
	strcpy(symbolDingbats[71].str, "❇");	// \u2747	SPARKLE
	strcpy(symbolDingbats[72].str, "❈");	// \u2748	HEAVY SPARKLE
	strcpy(symbolDingbats[73].str, "❉");	// \u2749	BALLOON-SPOKED ASTERISK
	strcpy(symbolDingbats[74].str, "❊");	// \u274A	EIGHT TEARDROP-SPOKED PROPELLER ASTERISK
	strcpy(symbolDingbats[75].str, "❋");	// \u274B	HEAVY EIGHT TEARDROP-SPOKED PROPELLER ASTERISK
	strcpy(symbolDingbats[76].str, "❌");	// \u274C	CROSS MARK
	strcpy(symbolDingbats[77].str, "❍");	// \u274D	SHADOWED WHITE CIRCLE
	strcpy(symbolDingbats[78].str, "❎");	// \u274E	NEGATIVE SQUARED CROSS MARK
	strcpy(symbolDingbats[79].str, "❏");	// \u274F	LOWER RIGHT DROP-SHADOWED WHITE SQUARE
	strcpy(symbolDingbats[80].str, "❐");	// \u2750	UPPER RIGHT DROP-SHADOWED WHITE SQUARE
	strcpy(symbolDingbats[81].str, "❑");	// \u2751	LOWER RIGHT SHADOWED WHITE SQUARE
	strcpy(symbolDingbats[82].str, "❒");	// \u2752	UPPER RIGHT SHADOWED WHITE SQUARE
	strcpy(symbolDingbats[83].str, "❓");	// \u2753	BLACK QUESTION MARK ORNAMENT
	strcpy(symbolDingbats[84].str, "❔");	// \u2754	WHITE QUESTION MARK ORNAMENT
	strcpy(symbolDingbats[85].str, "❕");	// \u2755	WHITE EXCLAMATION MARK ORNAMENT
	strcpy(symbolDingbats[86].str, "❖");	// \u2756	BLACK DIAMOND MINUS WHITE X
	strcpy(symbolDingbats[87].str, "❗");	// \u2757	HEAVY EXCLAMATION MARK SYMBOL
	strcpy(symbolDingbats[88].str, "❘");	// \u2758	LIGHT VERTICAL BAR
	strcpy(symbolDingbats[89].str, "❙");	// \u2759	MEDIUM VERTICAL BAR
	strcpy(symbolDingbats[90].str, "❚");	// \u275A	HEAVY VERTICAL BAR
	strcpy(symbolDingbats[91].str, "❛");	// \u275B	HEAVY SINGLE TURNED COMMA QUOTATION MARK ORNAMENT
	strcpy(symbolDingbats[92].str, "❜");	// \u275C	HEAVY SINGLE COMMA QUOTATION MARK ORNAMENT
	strcpy(symbolDingbats[93].str, "❝");	// \u275D	HEAVY DOUBLE TURNED COMMA QUOTATION MARK ORNAMENT
	strcpy(symbolDingbats[94].str, "❞");	// \u275E	HEAVY DOUBLE COMMA QUOTATION MARK ORNAMENT
	strcpy(symbolDingbats[95].str, "❟");	// \u275F	HEAVY LOW SINGLE COMMA QUOTATION MARK ORNAMENT
	strcpy(symbolDingbats[96].str, "❠");	// \u2760	HEAVY LOW DOUBLE COMMA QUOTATION MARK ORNAMENT
	strcpy(symbolDingbats[97].str, "❡");	// \u2761	CURVED STEM PARAGRAPH SIGN ORNAMENT
	strcpy(symbolDingbats[98].str, "❢");	// \u2762	HEAVY EXCLAMATION MARK ORNAMENT
	strcpy(symbolDingbats[99].str, "❣");	// \u2763	HEAVY HEART EXCLAMATION MARK ORNAMENT
	strcpy(symbolDingbats[100].str, "❤");	// \u2764	HEAVY BLACK HEART
	strcpy(symbolDingbats[101].str, "❥");	// \u2765	ROTATED HEAVY BLACK HEART BULLET
	strcpy(symbolDingbats[102].str, "❦");	// \u2766	FLORAL HEART
	strcpy(symbolDingbats[103].str, "❧");	// \u2767	ROTATED FLORAL HEART BULLET
	strcpy(symbolDingbats[104].str, "❨");	// \u2768	MEDIUM LEFT PARENTHESIS ORNAMENT
	strcpy(symbolDingbats[105].str, "❩");	// \u2769	MEDIUM RIGHT PARENTHESIS ORNAMENT
	strcpy(symbolDingbats[106].str, "❪");	// \u276A	MEDIUM FLATTENED LEFT PARENTHESIS ORNAMENT
	strcpy(symbolDingbats[107].str, "❫");	// \u276B	MEDIUM FLATTENED RIGHT PARENTHESIS ORNAMENT
	strcpy(symbolDingbats[108].str, "❬");	// \u276C	MEDIUM LEFT-POINTING ANGLE BRACKET ORNAMENT
	strcpy(symbolDingbats[109].str, "❭");	// \u276D	MEDIUM RIGHT-POINTING ANGLE BRACKET ORNAMENT
	strcpy(symbolDingbats[110].str, "❮");	// \u276E	HEAVY LEFT-POINTING ANGLE QUOTATION MARK ORNAMENT
	strcpy(symbolDingbats[111].str, "❯");	// \u276F	HEAVY RIGHT-POINTING ANGLE QUOTATION MARK ORNAMENT
	strcpy(symbolDingbats[112].str, "❰");	// \u2770	HEAVY LEFT-POINTING ANGLE BRACKET ORNAMENT
	strcpy(symbolDingbats[113].str, "❱");	// \u2771	HEAVY RIGHT-POINTING ANGLE BRACKET ORNAMENT
	strcpy(symbolDingbats[114].str, "❲");	// \u2772	LIGHT LEFT TORTOISE SHELL BRACKET ORNAMENT
	strcpy(symbolDingbats[115].str, "❳");	// \u2773	LIGHT RIGHT TORTOISE SHELL BRACKET ORNAMENT
	strcpy(symbolDingbats[116].str, "❴");	// \u2774	MEDIUM LEFT CURLY BRACKET ORNAMENT
	strcpy(symbolDingbats[117].str, "❵");	// \u2775	MEDIUM RIGHT CURLY BRACKET ORNAMENT
	strcpy(symbolDingbats[118].str, "❶");	// \u2776	DINGBAT NEGATIVE CIRCLED DIGIT ONE
	strcpy(symbolDingbats[119].str, "❷");	// \u2777	DINGBAT NEGATIVE CIRCLED DIGIT TWO
	strcpy(symbolDingbats[120].str, "❸");	// \u2778	DINGBAT NEGATIVE CIRCLED DIGIT THREE
	strcpy(symbolDingbats[121].str, "❹");	// \u2779	DINGBAT NEGATIVE CIRCLED DIGIT FOUR
	strcpy(symbolDingbats[122].str, "❺");	// \u277A	DINGBAT NEGATIVE CIRCLED DIGIT FIVE
	strcpy(symbolDingbats[123].str, "❻");	// \u277B	DINGBAT NEGATIVE CIRCLED DIGIT SIX
	strcpy(symbolDingbats[124].str, "❼");	// \u277C	DINGBAT NEGATIVE CIRCLED DIGIT SEVEN
	strcpy(symbolDingbats[125].str, "❽");	// \u277D	DINGBAT NEGATIVE CIRCLED DIGIT EIGHT
	strcpy(symbolDingbats[126].str, "❾");	// \u277E	DINGBAT NEGATIVE CIRCLED DIGIT NINE
	strcpy(symbolDingbats[127].str, "❿");	// \u277F	DINGBAT NEGATIVE CIRCLED NUMBER TEN
	strcpy(symbolDingbats[128].str, "➀");	// \u2780	DINGBAT CIRCLED SANS-SERIF DIGIT ONE
	strcpy(symbolDingbats[129].str, "➁");	// \u2781	DINGBAT CIRCLED SANS-SERIF DIGIT TWO
	strcpy(symbolDingbats[130].str, "➂");	// \u2782	DINGBAT CIRCLED SANS-SERIF DIGIT THREE
	strcpy(symbolDingbats[131].str, "➃");	// \u2783	DINGBAT CIRCLED SANS-SERIF DIGIT FOUR
	strcpy(symbolDingbats[132].str, "➄");	// \u2784	DINGBAT CIRCLED SANS-SERIF DIGIT FIVE
	strcpy(symbolDingbats[133].str, "➅");	// \u2785	DINGBAT CIRCLED SANS-SERIF DIGIT SIX
	strcpy(symbolDingbats[134].str, "➆");	// \u2786	DINGBAT CIRCLED SANS-SERIF DIGIT SEVEN
	strcpy(symbolDingbats[135].str, "➇");	// \u2787	DINGBAT CIRCLED SANS-SERIF DIGIT EIGHT
	strcpy(symbolDingbats[136].str, "➈");	// \u2788	DINGBAT CIRCLED SANS-SERIF DIGIT NINE
	strcpy(symbolDingbats[137].str, "➉");	// \u2789	DINGBAT CIRCLED SANS-SERIF NUMBER TEN
	strcpy(symbolDingbats[138].str, "➊");	// \u278A	DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT ONE
	strcpy(symbolDingbats[139].str, "➋");	// \u278B	DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT TWO
	strcpy(symbolDingbats[140].str, "➌");	// \u278C	DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT THREE
	strcpy(symbolDingbats[141].str, "➍");	// \u278D	DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT FOUR
	strcpy(symbolDingbats[142].str, "➎");	// \u278E	DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT FIVE
	strcpy(symbolDingbats[143].str, "➏");	// \u278F	DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT SIX
	strcpy(symbolDingbats[144].str, "➐");	// \u2790	DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT SEVEN
	strcpy(symbolDingbats[145].str, "➑");	// \u2791	DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT EIGHT
	strcpy(symbolDingbats[146].str, "➒");	// \u2792	DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT NINE
	strcpy(symbolDingbats[147].str, "➓");	// \u2793	DINGBAT NEGATIVE CIRCLED SANS-SERIF NUMBER TEN
	strcpy(symbolDingbats[148].str, "➔");	// \u2794	HEAVY WIDE-HEADED RIGHTWARDS ARROW
	strcpy(symbolDingbats[149].str, "➕");	// \u2795	HEAVY PLUS SIGN
	strcpy(symbolDingbats[150].str, "➖");	// \u2796	HEAVY MINUS SIGN
	strcpy(symbolDingbats[151].str, "➗");	// \u2797	HEAVY DIVISION SIGN
	strcpy(symbolDingbats[152].str, "➘");	// \u2798	HEAVY SOUTH EAST ARROW
	strcpy(symbolDingbats[153].str, "➙");	// \u2799	HEAVY RIGHTWARDS ARROW
	strcpy(symbolDingbats[154].str, "➚");	// \u279A	HEAVY NORTH EAST ARROW
	strcpy(symbolDingbats[155].str, "➛");	// \u279B	DRAFTING POINT RIGHTWARDS ARROW
	strcpy(symbolDingbats[156].str, "➜");	// \u279C	HEAVY ROUND-TIPPED RIGHTWARDS ARROW
	strcpy(symbolDingbats[157].str, "➝");	// \u279D	TRIANGLE-HEADED RIGHTWARDS ARROW
	strcpy(symbolDingbats[158].str, "➞");	// \u279E	HEAVY TRIANGLE-HEADED RIGHTWARDS ARROW
	strcpy(symbolDingbats[159].str, "➟");	// \u279F	DASHED TRIANGLE-HEADED RIGHTWARDS ARROW
	strcpy(symbolDingbats[160].str, "➠");	// \u27A0	HEAVY DASHED TRIANGLE-HEADED RIGHTWARDS ARROW
	strcpy(symbolDingbats[161].str, "➡");	// \u27A1	BLACK RIGHTWARDS ARROW
	strcpy(symbolDingbats[162].str, "➢");	// \u27A2	THREE-D TOP-LIGHTED RIGHTWARDS ARROWHEAD
	strcpy(symbolDingbats[163].str, "➣");	// \u27A3	THREE-D BOTTOM-LIGHTED RIGHTWARDS ARROWHEAD
	strcpy(symbolDingbats[164].str, "➤");	// \u27A4	BLACK RIGHTWARDS ARROWHEAD
	strcpy(symbolDingbats[165].str, "➥");	// \u27A5	HEAVY BLACK CURVED DOWNWARDS AND RIGHTWARDS ARROW
	strcpy(symbolDingbats[166].str, "➦");	// \u27A6	HEAVY BLACK CURVED UPWARDS AND RIGHTWARDS ARROW
	strcpy(symbolDingbats[167].str, "➧");	// \u27A7	SQUAT BLACK RIGHTWARDS ARROW
	strcpy(symbolDingbats[168].str, "➨");	// \u27A8	HEAVY CONCAVE-POINTED BLACK RIGHTWARDS ARROW
	strcpy(symbolDingbats[169].str, "➩");	// \u27A9	RIGHT-SHADED WHITE RIGHTWARDS ARROW
	strcpy(symbolDingbats[170].str, "➪");	// \u27AA	LEFT-SHADED WHITE RIGHTWARDS ARROW
	strcpy(symbolDingbats[171].str, "➫");	// \u27AB	BACK-TILTED SHADOWED WHITE RIGHTWARDS ARROW
	strcpy(symbolDingbats[172].str, "➬");	// \u27AC	FRONT-TILTED SHADOWED WHITE RIGHTWARDS ARROW
	strcpy(symbolDingbats[173].str, "➭");	// \u27AD	HEAVY LOWER RIGHT-SHADOWED WHITE RIGHTWARDS ARROW
	strcpy(symbolDingbats[174].str, "➮");	// \u27AE	HEAVY UPPER RIGHT-SHADOWED WHITE RIGHTWARDS ARROW
	strcpy(symbolDingbats[175].str, "➯");	// \u27AF	NOTCHED LOWER RIGHT-SHADOWED WHITE RIGHTWARDS ARROW
	strcpy(symbolDingbats[176].str, "➰");	// \u27B0	CURLY LOOP
	strcpy(symbolDingbats[177].str, "➱");	// \u27B1	NOTCHED UPPER RIGHT-SHADOWED WHITE RIGHTWARDS ARROW
	strcpy(symbolDingbats[178].str, "➲");	// \u27B2	CIRCLED HEAVY WHITE RIGHTWARDS ARROW
	strcpy(symbolDingbats[179].str, "➳");	// \u27B3	WHITE-FEATHERED RIGHTWARDS ARROW
	strcpy(symbolDingbats[180].str, "➴");	// \u27B4	BLACK-FEATHERED SOUTH EAST ARROW
	strcpy(symbolDingbats[181].str, "➵");	// \u27B5	BLACK-FEATHERED RIGHTWARDS ARROW
	strcpy(symbolDingbats[182].str, "➶");	// \u27B6	BLACK-FEATHERED NORTH EAST ARROW
	strcpy(symbolDingbats[183].str, "➷");	// \u27B7	HEAVY BLACK-FEATHERED SOUTH EAST ARROW
	strcpy(symbolDingbats[184].str, "➸");	// \u27B8	HEAVY BLACK-FEATHERED RIGHTWARDS ARROW
	strcpy(symbolDingbats[185].str, "➹");	// \u27B9	HEAVY BLACK-FEATHERED NORTH EAST ARROW
	strcpy(symbolDingbats[186].str, "➺");	// \u27BA	TEARDROP-BARBED RIGHTWARDS ARROW
	strcpy(symbolDingbats[187].str, "➻");	// \u27BB	HEAVY TEARDROP-SHANKED RIGHTWARDS ARROW
	strcpy(symbolDingbats[188].str, "➼");	// \u27BC	WEDGE-TAILED RIGHTWARDS ARROW
	strcpy(symbolDingbats[189].str, "➽");	// \u27BD	HEAVY WEDGE-TAILED RIGHTWARDS ARROW
	strcpy(symbolDingbats[190].str, "➾");	// \u27BE	OPEN-OUTLINED RIGHTWARDS ARROW
	strcpy(symbolDingbats[191].str, "➿");	// \u27BF	DOUBLE CURLY LOOP

	strcpy(symbolEmoji[1].str, "⌚");		// \u231A	
	strcpy(symbolEmoji[2].str, "⌛");	// \u231B	
	strcpy(symbolEmoji[3].str, "⏩");	// \u23000000000	
	strcpy(symbolEmoji[4].str, "⏪");	// \u23EA	
	strcpy(symbolEmoji[5].str, "⏫");	// \u23EB	
	strcpy(symbolEmoji[6].str, "⏬");	// \u23EC	
	strcpy(symbolEmoji[7].str, "⏭");	// \u23ED	
	strcpy(symbolEmoji[8].str, "⏮");	// \u23EE	
	strcpy(symbolEmoji[9].str, "⏯");	// \u23EF	
	strcpy(symbolEmoji[10].str, "⏰");	// \u23F0	
	strcpy(symbolEmoji[11].str, "⏱");	// \u23F1	
	strcpy(symbolEmoji[12].str, "⏲");	// \u23F2	
	strcpy(symbolEmoji[13].str, "⏳");	// \u23F3	
	strcpy(symbolEmoji[14].str, "⏸");	// \u23F8	
	strcpy(symbolEmoji[15].str, "⏹");	// \u23F9	
	strcpy(symbolEmoji[16].str, "⏺");	// \u23FA	
	strcpy(symbolEmoji[17].str, "Ⓜ");	// \u24C2	
	strcpy(symbolEmoji[18].str, "☔");	// \u2614	
	strcpy(symbolEmoji[19].str, "☕");	// \u2615	
	strcpy(symbolEmoji[20].str, "☝");	// \u261D	
	strcpy(symbolEmoji[21].str, "♈");	// \u2648	
	strcpy(symbolEmoji[22].str, "♉");	// \u2649	
	strcpy(symbolEmoji[23].str, "♊");	// \u264A	
	strcpy(symbolEmoji[24].str, "♋");	// \u264B	
	strcpy(symbolEmoji[25].str, "♌");	// \u264C	
	strcpy(symbolEmoji[26].str, "♍");	// \u264D	
	strcpy(symbolEmoji[27].str, "♎");	// \u264E	
	strcpy(symbolEmoji[28].str, "♏");	// \u264F	
	strcpy(symbolEmoji[29].str, "♐");	// \u2650	
	strcpy(symbolEmoji[30].str, "♑");	// \u2651	
	strcpy(symbolEmoji[31].str, "♒");	// \u2652	
	strcpy(symbolEmoji[32].str, "♓");	// \u2653	
	strcpy(symbolEmoji[33].str, "♟");	// \u265F	
	strcpy(symbolEmoji[34].str, "♿");	// \u267F	
	strcpy(symbolEmoji[35].str, "⚓");	// \u2693	
	strcpy(symbolEmoji[36].str, "⚡");	// \u26A1	
	strcpy(symbolEmoji[37].str, "⚪");	// \u26AA	
	strcpy(symbolEmoji[38].str, "⚫");	// \u26AB	
	strcpy(symbolEmoji[39].str, "⚽");	// \u26BD	
	strcpy(symbolEmoji[40].str, "⚾");	// \u26BE	
	strcpy(symbolEmoji[41].str, "⛄");	// \u26C4	
	strcpy(symbolEmoji[42].str, "⛅");	// \u26C5	
	strcpy(symbolEmoji[43].str, "⛎");	// \u26CE	
	strcpy(symbolEmoji[44].str, "⛏");	// \u26CF	
	strcpy(symbolEmoji[45].str, "⛑");	// \u26D1	
	strcpy(symbolEmoji[46].str, "⛓");	// \u26D3	
	strcpy(symbolEmoji[47].str, "⛔");	// \u26D4	
	strcpy(symbolEmoji[48].str, "⛩");	// \u26000000000	
	strcpy(symbolEmoji[49].str, "⛪");	// \u26EA	
	strcpy(symbolEmoji[50].str, "⛰");	// \u26F0	
	strcpy(symbolEmoji[51].str, "⛱");	// \u26F1	
	strcpy(symbolEmoji[52].str, "⛲");	// \u26F2	
	strcpy(symbolEmoji[53].str, "⛳");	// \u26F3	
	strcpy(symbolEmoji[54].str, "⛴");	// \u26F4	
	strcpy(symbolEmoji[55].str, "⛵");	// \u26F5	
	strcpy(symbolEmoji[56].str, "⛷");	// \u26F7	
	strcpy(symbolEmoji[57].str, "⛸");	// \u26F8	
	strcpy(symbolEmoji[58].str, "⛹");	// \u26F9	
	strcpy(symbolEmoji[59].str, "⛺");	// \u26FA	
	strcpy(symbolEmoji[60].str, "⛽");	// \u26FD	
	strcpy(symbolEmoji[61].str, "✂");	// \u2702	
	strcpy(symbolEmoji[62].str, "✅");	// \u2705	
	strcpy(symbolEmoji[63].str, "✈");	// \u2708	
	strcpy(symbolEmoji[64].str, "✉");	// \u2709	
	strcpy(symbolEmoji[65].str, "✊");	// \u270A	
	strcpy(symbolEmoji[66].str, "✋");	// \u270B	
	strcpy(symbolEmoji[67].str, "✌");	// \u270C	
	strcpy(symbolEmoji[68].str, "✍");	// \u270D	
	strcpy(symbolEmoji[69].str, "✏");	// \u270F	
	strcpy(symbolEmoji[70].str, "✒");	// \u2712	
	strcpy(symbolEmoji[71].str, "✔");	// \u2714	
	strcpy(symbolEmoji[72].str, "✖");	// \u2716	
	strcpy(symbolEmoji[73].str, "✝");	// \u271D	
	strcpy(symbolEmoji[74].str, "✡");	// \u2721	
	strcpy(symbolEmoji[75].str, "✨");	// \u2728	
	strcpy(symbolEmoji[76].str, "✳");	// \u2733	
	strcpy(symbolEmoji[77].str, "✴");	// \u2734	
	strcpy(symbolEmoji[78].str, "❄");	// \u2744	
	strcpy(symbolEmoji[79].str, "❇");	// \u2747	
	strcpy(symbolEmoji[80].str, "❌");	// \u274C	
	strcpy(symbolEmoji[81].str, "❎");	// \u274E	
	strcpy(symbolEmoji[82].str, "❓");	// \u2753	
	strcpy(symbolEmoji[83].str, "❔");	// \u2754	
	strcpy(symbolEmoji[84].str, "❕");	// \u2755	
	strcpy(symbolEmoji[85].str, "❗");	// \u2757	
	strcpy(symbolEmoji[86].str, "❣");	// \u2763	
	strcpy(symbolEmoji[87].str, "❤");	// \u2764	
	strcpy(symbolEmoji[88].str, "➕");	// \u2795	
	strcpy(symbolEmoji[89].str, "➖");	// \u2796	
	strcpy(symbolEmoji[90].str, "➗");	// \u2797	
	strcpy(symbolEmoji[91].str, "➡");	// \u27A1	
	strcpy(symbolEmoji[92].str, "➰");	// \u27B0	
	strcpy(symbolEmoji[93].str, "➿");	// \u27BF	
	strcpy(symbolEmoji[94].str, "⤴");	// \u2934	
	strcpy(symbolEmoji[95].str, "⤵");	// \u2935	
	strcpy(symbolEmoji[96].str, "⬅");	// \u2B05	
	strcpy(symbolEmoji[97].str, "⬆");	// \u2B06	
	strcpy(symbolEmoji[98].str, "⬇");	// \u2B07	
	strcpy(symbolEmoji[99].str, "⬛");	// \u2B1B	
	strcpy(symbolEmoji[100].str, "⬜");	// \u2B1C	
	strcpy(symbolEmoji[101].str, "⭐");	// \u2B50	
	strcpy(symbolEmoji[102].str, "⭕");	// \u2B55	
	strcpy(symbolEmoji[103].str, "〰");	// \u3030	
	strcpy(symbolEmoji[104].str, "〽");	// \u303D	
	strcpy(symbolEmoji[105].str, "㊗");	// \u3297	
	strcpy(symbolEmoji[106].str, "㊙");	// \u3299	
	strcpy(symbolEmoji[107].str, "🀄");	// \u1F004	
	strcpy(symbolEmoji[108].str, "🃏");	// \u1F0CF	
	strcpy(symbolEmoji[109].str, "🅰");	// \u1F170	
	strcpy(symbolEmoji[110].str, "🅱");	// \u1F171	
	strcpy(symbolEmoji[111].str, "🅾");	// \u1F17E	
	strcpy(symbolEmoji[112].str, "🅿");	// \u1F17F	
	strcpy(symbolEmoji[113].str, "🆎");	// \u1F18E	
	strcpy(symbolEmoji[114].str, "🆑");	// \u1F191	
	strcpy(symbolEmoji[115].str, "🆒");	// \u1F192	
	strcpy(symbolEmoji[116].str, "🆓");	// \u1F193	
	strcpy(symbolEmoji[117].str, "🆔");	// \u1F194	
	strcpy(symbolEmoji[118].str, "🆕");	// \u1F195	
	strcpy(symbolEmoji[119].str, "🆖");	// \u1F196	
	strcpy(symbolEmoji[120].str, "🆗");	// \u1F197	
	strcpy(symbolEmoji[121].str, "🆘");	// \u1F198	
	strcpy(symbolEmoji[122].str, "🆙");	// \u1F199	
	strcpy(symbolEmoji[123].str, "🆚");	// \u1F19A	
	strcpy(symbolEmoji[124].str, "🈁");	// \u1F201	
	strcpy(symbolEmoji[125].str, "🈂");	// \u1F202	
	strcpy(symbolEmoji[126].str, "🈚");	// \u1F21A	
	strcpy(symbolEmoji[127].str, "🈯");	// \u1F22F	
	strcpy(symbolEmoji[128].str, "🈲");	// \u1F232	
	strcpy(symbolEmoji[129].str, "🈳");	// \u1F233	
	strcpy(symbolEmoji[130].str, "🈴");	// \u1F234	
	strcpy(symbolEmoji[131].str, "🈵");	// \u1F235	
	strcpy(symbolEmoji[132].str, "🈶");	// \u1F236	
	strcpy(symbolEmoji[133].str, "🈷");	// \u1F237	
	strcpy(symbolEmoji[134].str, "🈸");	// \u1F238	
	strcpy(symbolEmoji[135].str, "🈹");	// \u1F239	
	strcpy(symbolEmoji[136].str, "🈺");	// \u1F23A	
	strcpy(symbolEmoji[137].str, "🉐");	// \u1F250	
	strcpy(symbolEmoji[138].str, "🉑");	// \u1F251	
	strcpy(symbolEmoji[139].str, "🌀");	// \u1F300	
	strcpy(symbolEmoji[140].str, "🌁");	// \u1F301	
	strcpy(symbolEmoji[141].str, "🌂");	// \u1F302	
	strcpy(symbolEmoji[142].str, "🌃");	// \u1F303	
	strcpy(symbolEmoji[143].str, "🌄");	// \u1F304	
	strcpy(symbolEmoji[144].str, "🌅");	// \u1F305	
	strcpy(symbolEmoji[145].str, "🌆");	// \u1F306	
	strcpy(symbolEmoji[146].str, "🌇");	// \u1F307	
	strcpy(symbolEmoji[147].str, "🌈");	// \u1F308	
	strcpy(symbolEmoji[148].str, "🌉");	// \u1F309	
	strcpy(symbolEmoji[149].str, "🌊");	// \u1F30A	
	strcpy(symbolEmoji[150].str, "🌋");	// \u1F30B	
	strcpy(symbolEmoji[151].str, "🌌");	// \u1F30C	
	strcpy(symbolEmoji[152].str, "🌍");	// \u1F30D	
	strcpy(symbolEmoji[153].str, "🌎");	// \u1F30E	
	strcpy(symbolEmoji[154].str, "🌏");	// \u1F30F	
	strcpy(symbolEmoji[155].str, "🌐");	// \u1F310	
	strcpy(symbolEmoji[156].str, "🌑");	// \u1F311	
	strcpy(symbolEmoji[157].str, "🌒");	// \u1F312	
	strcpy(symbolEmoji[158].str, "🌓");	// \u1F313	
	strcpy(symbolEmoji[159].str, "🌔");	// \u1F314	
	strcpy(symbolEmoji[160].str, "🌕");	// \u1F315	
	strcpy(symbolEmoji[161].str, "🌖");	// \u1F316	
	strcpy(symbolEmoji[162].str, "🌗");	// \u1F317	
	strcpy(symbolEmoji[163].str, "🌘");	// \u1F318	
	strcpy(symbolEmoji[164].str, "🌙");	// \u1F319	
	strcpy(symbolEmoji[165].str, "🌚");	// \u1F31A	
	strcpy(symbolEmoji[166].str, "🌛");	// \u1F31B	
	strcpy(symbolEmoji[167].str, "🌜");	// \u1F31C	
	strcpy(symbolEmoji[168].str, "🌝");	// \u1F31D	
	strcpy(symbolEmoji[169].str, "🌞");	// \u1F31E	
	strcpy(symbolEmoji[170].str, "🌟");	// \u1F31F	
	strcpy(symbolEmoji[171].str, "🌠");	// \u1F320	
	strcpy(symbolEmoji[172].str, "🌡");	// \u1F321	
	strcpy(symbolEmoji[173].str, "🌤");	// \u1F324	
	strcpy(symbolEmoji[174].str, "🌥");	// \u1F325	
	strcpy(symbolEmoji[175].str, "🌦");	// \u1F326	
	strcpy(symbolEmoji[176].str, "🌧");	// \u1F327	
	strcpy(symbolEmoji[177].str, "🌨");	// \u1F328	
	strcpy(symbolEmoji[178].str, "🌩");	// \u1F329	
	strcpy(symbolEmoji[179].str, "🌪");	// \u1F32A	
	strcpy(symbolEmoji[180].str, "🌫");	// \u1F32B	
	strcpy(symbolEmoji[181].str, "🌬");	// \u1F32C	
	strcpy(symbolEmoji[182].str, "🌭");	// \u1F32D	
	strcpy(symbolEmoji[183].str, "🌮");	// \u1F32E	
	strcpy(symbolEmoji[184].str, "🌯");	// \u1F32F	
	strcpy(symbolEmoji[185].str, "🌰");	// \u1F330	
	strcpy(symbolEmoji[186].str, "🌱");	// \u1F331	
	strcpy(symbolEmoji[187].str, "🌲");	// \u1F332	
	strcpy(symbolEmoji[188].str, "🌳");	// \u1F333	
	strcpy(symbolEmoji[189].str, "🌴");	// \u1F334	
	strcpy(symbolEmoji[190].str, "🌵");	// \u1F335	
	strcpy(symbolEmoji[191].str, "🌶");	// \u1F336	
	strcpy(symbolEmoji[192].str, "🌷");	// \u1F337	
	strcpy(symbolEmoji[193].str, "🌸");	// \u1F338	
	strcpy(symbolEmoji[194].str, "🌹");	// \u1F339	
	strcpy(symbolEmoji[195].str, "🌺");	// \u1F33A	
	strcpy(symbolEmoji[196].str, "🌻");	// \u1F33B	
	strcpy(symbolEmoji[197].str, "🌼");	// \u1F33C	
	strcpy(symbolEmoji[198].str, "🌽");	// \u1F33D	
	strcpy(symbolEmoji[199].str, "🌾");	// \u1F33E	
	strcpy(symbolEmoji[200].str, "🌿");	// \u1F33F	
	strcpy(symbolEmoji[201].str, "🍀");	// \u1F340	
	strcpy(symbolEmoji[202].str, "🍁");	// \u1F341	
	strcpy(symbolEmoji[203].str, "🍂");	// \u1F342	
	strcpy(symbolEmoji[204].str, "🍃");	// \u1F343	
	strcpy(symbolEmoji[205].str, "🍄");	// \u1F344	
	strcpy(symbolEmoji[206].str, "🍅");	// \u1F345	
	strcpy(symbolEmoji[207].str, "🍆");	// \u1F346	
	strcpy(symbolEmoji[208].str, "🍇");	// \u1F347	
	strcpy(symbolEmoji[209].str, "🍈");	// \u1F348	
	strcpy(symbolEmoji[210].str, "🍉");	// \u1F349	
	strcpy(symbolEmoji[211].str, "🍊");	// \u1F34A	
	strcpy(symbolEmoji[212].str, "🍋");	// \u1F34B	
	strcpy(symbolEmoji[213].str, "🍌");	// \u1F34C	
	strcpy(symbolEmoji[214].str, "🍍");	// \u1F34D	
	strcpy(symbolEmoji[215].str, "🍎");	// \u1F34E	
	strcpy(symbolEmoji[216].str, "🍏");	// \u1F34F	
	strcpy(symbolEmoji[217].str, "🍐");	// \u1F350	
	strcpy(symbolEmoji[218].str, "🍑");	// \u1F351	
	strcpy(symbolEmoji[219].str, "🍒");	// \u1F352	
	strcpy(symbolEmoji[220].str, "🍓");	// \u1F353	
	strcpy(symbolEmoji[221].str, "🍔");	// \u1F354	
	strcpy(symbolEmoji[222].str, "🍕");	// \u1F355	
	strcpy(symbolEmoji[223].str, "🍖");	// \u1F356	
	strcpy(symbolEmoji[224].str, "🍗");	// \u1F357	
	strcpy(symbolEmoji[225].str, "🍘");	// \u1F358	
	strcpy(symbolEmoji[226].str, "🍙");	// \u1F359	
	strcpy(symbolEmoji[227].str, "🍚");	// \u1F35A	
	strcpy(symbolEmoji[228].str, "🍛");	// \u1F35B	
	strcpy(symbolEmoji[229].str, "🍜");	// \u1F35C	
	strcpy(symbolEmoji[230].str, "🍝");	// \u1F35D	
	strcpy(symbolEmoji[231].str, "🍞");	// \u1F35E	
	strcpy(symbolEmoji[232].str, "🍟");	// \u1F35F	
	strcpy(symbolEmoji[233].str, "🍠");	// \u1F360	
	strcpy(symbolEmoji[234].str, "🍡");	// \u1F361	
	strcpy(symbolEmoji[235].str, "🍢");	// \u1F362	
	strcpy(symbolEmoji[236].str, "🍣");	// \u1F363	
	strcpy(symbolEmoji[237].str, "🍤");	// \u1F364	
	strcpy(symbolEmoji[238].str, "🍥");	// \u1F365	
	strcpy(symbolEmoji[239].str, "🍦");	// \u1F366	
	strcpy(symbolEmoji[240].str, "🍧");	// \u1F367	
	strcpy(symbolEmoji[241].str, "🍨");	// \u1F368	
	strcpy(symbolEmoji[242].str, "🍩");	// \u1F369	
	strcpy(symbolEmoji[243].str, "🍪");	// \u1F36A	
	strcpy(symbolEmoji[244].str, "🍫");	// \u1F36B	
	strcpy(symbolEmoji[245].str, "🍬");	// \u1F36C	
	strcpy(symbolEmoji[246].str, "🍭");	// \u1F36D	
	strcpy(symbolEmoji[247].str, "🍮");	// \u1F36E	
	strcpy(symbolEmoji[248].str, "🍯");	// \u1F36F	
	strcpy(symbolEmoji[249].str, "🍰");	// \u1F370	
	strcpy(symbolEmoji[250].str, "🍱");	// \u1F371	
	strcpy(symbolEmoji[251].str, "🍲");	// \u1F372	
	strcpy(symbolEmoji[252].str, "🍳");	// \u1F373	
	strcpy(symbolEmoji[253].str, "🍴");	// \u1F374	
	strcpy(symbolEmoji[254].str, "🍵");	// \u1F375	
	strcpy(symbolEmoji[255].str, "🍶");	// \u1F376	
	strcpy(symbolEmoji[256].str, "🍷");	// \u1F377	
	strcpy(symbolEmoji[257].str, "🍸");	// \u1F378	
	strcpy(symbolEmoji[258].str, "🍹");	// \u1F379	
	strcpy(symbolEmoji[259].str, "🍺");	// \u1F37A	
	strcpy(symbolEmoji[260].str, "🍻");	// \u1F37B	
	strcpy(symbolEmoji[261].str, "🍼");	// \u1F37C	
	strcpy(symbolEmoji[262].str, "🍽");	// \u1F37D	
	strcpy(symbolEmoji[263].str, "🍾");	// \u1F37E	
	strcpy(symbolEmoji[264].str, "🍿");	// \u1F37F	
	strcpy(symbolEmoji[265].str, "🎀");	// \u1F380	
	strcpy(symbolEmoji[266].str, "🎁");	// \u1F381	
	strcpy(symbolEmoji[267].str, "🎂");	// \u1F382	
	strcpy(symbolEmoji[268].str, "🎃");	// \u1F383	
	strcpy(symbolEmoji[269].str, "🎄");	// \u1F384	
	strcpy(symbolEmoji[270].str, "🎅");	// \u1F385	
	strcpy(symbolEmoji[271].str, "🎆");	// \u1F386	
	strcpy(symbolEmoji[272].str, "🎇");	// \u1F387	
	strcpy(symbolEmoji[273].str, "🎈");	// \u1F388	
	strcpy(symbolEmoji[274].str, "🎉");	// \u1F389	
	strcpy(symbolEmoji[275].str, "🎊");	// \u1F38A	
	strcpy(symbolEmoji[276].str, "🎋");	// \u1F38B	
	strcpy(symbolEmoji[277].str, "🎌");	// \u1F38C	
	strcpy(symbolEmoji[278].str, "🎍");	// \u1F38D	
	strcpy(symbolEmoji[279].str, "🎎");	// \u1F38E	
	strcpy(symbolEmoji[280].str, "🎏");	// \u1F38F	
	strcpy(symbolEmoji[281].str, "🎐");	// \u1F390	
	strcpy(symbolEmoji[282].str, "🎑");	// \u1F391	
	strcpy(symbolEmoji[283].str, "🎒");	// \u1F392	
	strcpy(symbolEmoji[284].str, "🎓");	// \u1F393	
	strcpy(symbolEmoji[285].str, "🎖");	// \u1F396	
	strcpy(symbolEmoji[286].str, "🎗");	// \u1F397	
	strcpy(symbolEmoji[287].str, "🎙");	// \u1F399	
	strcpy(symbolEmoji[288].str, "🎚");	// \u1F39A	
	strcpy(symbolEmoji[289].str, "🎛");	// \u1F39B	
	strcpy(symbolEmoji[290].str, "🎞");	// \u1F39E	
	strcpy(symbolEmoji[291].str, "🎟");	// \u1F39F	
	strcpy(symbolEmoji[292].str, "🎠");	// \u1F3A0	
	strcpy(symbolEmoji[293].str, "🎡");	// \u1F3A1	
	strcpy(symbolEmoji[294].str, "🎢");	// \u1F3A2	
	strcpy(symbolEmoji[295].str, "🎣");	// \u1F3A3	
	strcpy(symbolEmoji[296].str, "🎤");	// \u1F3A4	
	strcpy(symbolEmoji[297].str, "🎥");	// \u1F3A5	
	strcpy(symbolEmoji[298].str, "🎦");	// \u1F3A6	
	strcpy(symbolEmoji[299].str, "🎧");	// \u1F3A7	
	strcpy(symbolEmoji[300].str, "🎨");	// \u1F3A8	
	strcpy(symbolEmoji[301].str, "🎩");	// \u1F3A9	
	strcpy(symbolEmoji[302].str, "🎪");	// \u1F3AA	
	strcpy(symbolEmoji[303].str, "🎫");	// \u1F3AB	
	strcpy(symbolEmoji[304].str, "🎬");	// \u1F3AC	
	strcpy(symbolEmoji[305].str, "🎭");	// \u1F3AD	
	strcpy(symbolEmoji[306].str, "🎮");	// \u1F3AE	
	strcpy(symbolEmoji[307].str, "🎯");	// \u1F3AF	
	strcpy(symbolEmoji[308].str, "🎰");	// \u1F3B0	
	strcpy(symbolEmoji[309].str, "🎱");	// \u1F3B1	
	strcpy(symbolEmoji[310].str, "🎲");	// \u1F3B2	
	strcpy(symbolEmoji[311].str, "🎳");	// \u1F3B3	
	strcpy(symbolEmoji[312].str, "🎴");	// \u1F3B4	
	strcpy(symbolEmoji[313].str, "🎵");	// \u1F3B5	
	strcpy(symbolEmoji[314].str, "🎶");	// \u1F3B6	
	strcpy(symbolEmoji[315].str, "🎷");	// \u1F3B7	
	strcpy(symbolEmoji[316].str, "🎸");	// \u1F3B8	
	strcpy(symbolEmoji[317].str, "🎹");	// \u1F3B9	
	strcpy(symbolEmoji[318].str, "🎺");	// \u1F3BA	
	strcpy(symbolEmoji[319].str, "🎻");	// \u1F3BB	
	strcpy(symbolEmoji[320].str, "🎼");	// \u1F3BC	
	strcpy(symbolEmoji[321].str, "🎽");	// \u1F3BD	
	strcpy(symbolEmoji[322].str, "🎾");	// \u1F3BE	
	strcpy(symbolEmoji[323].str, "🎿");	// \u1F3BF	
	strcpy(symbolEmoji[324].str, "🏀");	// \u1F3C0	
	strcpy(symbolEmoji[325].str, "🏁");	// \u1F3C1	
	strcpy(symbolEmoji[326].str, "🏂");	// \u1F3C2	
	strcpy(symbolEmoji[327].str, "🏃");	// \u1F3C3	
	strcpy(symbolEmoji[328].str, "🏄");	// \u1F3C4	
	strcpy(symbolEmoji[329].str, "🏅");	// \u1F3C5	
	strcpy(symbolEmoji[330].str, "🏆");	// \u1F3C6	
	strcpy(symbolEmoji[331].str, "🏇");	// \u1F3C7	
	strcpy(symbolEmoji[332].str, "🏈");	// \u1F3C8	
	strcpy(symbolEmoji[333].str, "🏉");	// \u1F3C9	
	strcpy(symbolEmoji[334].str, "🏊");	// \u1F3CA	
	strcpy(symbolEmoji[335].str, "🏋");	// \u1F3CB	
	strcpy(symbolEmoji[336].str, "🏌");	// \u1F3CC	
	strcpy(symbolEmoji[337].str, "🏍");	// \u1F3CD	
	strcpy(symbolEmoji[338].str, "🏎");	// \u1F3CE	
	strcpy(symbolEmoji[339].str, "🏏");	// \u1F3CF	
	strcpy(symbolEmoji[340].str, "🏐");	// \u1F3D0	
	strcpy(symbolEmoji[341].str, "🏑");	// \u1F3D1	
	strcpy(symbolEmoji[342].str, "🏒");	// \u1F3D2	
	strcpy(symbolEmoji[343].str, "🏓");	// \u1F3D3	
	strcpy(symbolEmoji[344].str, "🏔");	// \u1F3D4	
	strcpy(symbolEmoji[345].str, "🏕");	// \u1F3D5	
	strcpy(symbolEmoji[346].str, "🏖");	// \u1F3D6	
	strcpy(symbolEmoji[347].str, "🏗");	// \u1F3D7	
	strcpy(symbolEmoji[348].str, "🏘");	// \u1F3D8	
	strcpy(symbolEmoji[349].str, "🏙");	// \u1F3D9	
	strcpy(symbolEmoji[350].str, "🏚");	// \u1F3DA	
	strcpy(symbolEmoji[351].str, "🏛");	// \u1F3DB	
	strcpy(symbolEmoji[352].str, "🏜");	// \u1F3DC	
	strcpy(symbolEmoji[353].str, "🏝");	// \u1F3DD	
	strcpy(symbolEmoji[354].str, "🏞");	// \u1F3DE	
	strcpy(symbolEmoji[355].str, "🏟");	// \u1F3DF	
	strcpy(symbolEmoji[356].str, "🏠");	// \u1F3E0	
	strcpy(symbolEmoji[357].str, "🏡");	// \u1F3E1	
	strcpy(symbolEmoji[358].str, "🏢");	// \u1F3E2	
	strcpy(symbolEmoji[359].str, "🏣");	// \u1F3E3	
	strcpy(symbolEmoji[360].str, "🏤");	// \u1F3E4	
	strcpy(symbolEmoji[361].str, "🏥");	// \u1F3E5	
	strcpy(symbolEmoji[362].str, "🏦");	// \u1F3E6	
	strcpy(symbolEmoji[363].str, "🏧");	// \u1F3E7	
	strcpy(symbolEmoji[364].str, "🏨");	// \u1F3E8	
	strcpy(symbolEmoji[365].str, "🏩");	// \u1F3E9	
	strcpy(symbolEmoji[366].str, "🏪");	// \u1F3EA	
	strcpy(symbolEmoji[367].str, "🏫");	// \u1F3EB	
	strcpy(symbolEmoji[368].str, "🏬");	// \u1F3EC	
	strcpy(symbolEmoji[369].str, "🏭");	// \u1F3ED	
	strcpy(symbolEmoji[370].str, "🏮");	// \u1F3EE	
	strcpy(symbolEmoji[371].str, "🏯");	// \u1F3EF	
	strcpy(symbolEmoji[372].str, "🏰");	// \u1F3F0	
	strcpy(symbolEmoji[373].str, "🏳");	// \u1F3F3	
	strcpy(symbolEmoji[374].str, "🏴");	// \u1F3F4	
	strcpy(symbolEmoji[375].str, "🏵");	// \u1F3F5	
	strcpy(symbolEmoji[376].str, "🏷");	// \u1F3F7	
	strcpy(symbolEmoji[377].str, "🏸");	// \u1F3F8	
	strcpy(symbolEmoji[378].str, "🏹");	// \u1F3F9	
	strcpy(symbolEmoji[379].str, "🏺");	// \u1F3FA	
	strcpy(symbolEmoji[380].str, "🏻");	// \u1F3FB	
	strcpy(symbolEmoji[381].str, "🏼");	// \u1F3FC	
	strcpy(symbolEmoji[382].str, "🏽");	// \u1F3FD	
	strcpy(symbolEmoji[383].str, "🏾");	// \u1F3FE	
	strcpy(symbolEmoji[384].str, "🏿");	// \u1F3FF	
	strcpy(symbolEmoji[385].str, "🐀");	// \u1F400	
	strcpy(symbolEmoji[386].str, "🐁");	// \u1F401	
	strcpy(symbolEmoji[387].str, "🐂");	// \u1F402	
	strcpy(symbolEmoji[388].str, "🐃");	// \u1F403	
	strcpy(symbolEmoji[389].str, "🐄");	// \u1F404	
	strcpy(symbolEmoji[390].str, "🐅");	// \u1F405	
	strcpy(symbolEmoji[391].str, "🐆");	// \u1F406	
	strcpy(symbolEmoji[392].str, "🐇");	// \u1F407	
	strcpy(symbolEmoji[393].str, "🐈");	// \u1F408	
	strcpy(symbolEmoji[394].str, "🐉");	// \u1F409	
	strcpy(symbolEmoji[395].str, "🐊");	// \u1F40A	
	strcpy(symbolEmoji[396].str, "🐋");	// \u1F40B	
	strcpy(symbolEmoji[397].str, "🐌");	// \u1F40C	
	strcpy(symbolEmoji[398].str, "🐍");	// \u1F40D	
	strcpy(symbolEmoji[399].str, "🐎");	// \u1F40E	
	strcpy(symbolEmoji[400].str, "🐏");	// \u1F40F	
	strcpy(symbolEmoji[401].str, "🐐");	// \u1F410	
	strcpy(symbolEmoji[402].str, "🐑");	// \u1F411	
	strcpy(symbolEmoji[403].str, "🐒");	// \u1F412	
	strcpy(symbolEmoji[404].str, "🐓");	// \u1F413	
	strcpy(symbolEmoji[405].str, "🐔");	// \u1F414	
	strcpy(symbolEmoji[406].str, "🐕");	// \u1F415	
	strcpy(symbolEmoji[407].str, "🐖");	// \u1F416	
	strcpy(symbolEmoji[408].str, "🐗");	// \u1F417	
	strcpy(symbolEmoji[409].str, "🐘");	// \u1F418	
	strcpy(symbolEmoji[410].str, "🐙");	// \u1F419	
	strcpy(symbolEmoji[411].str, "🐚");	// \u1F41A	
	strcpy(symbolEmoji[412].str, "🐛");	// \u1F41B	
	strcpy(symbolEmoji[413].str, "🐜");	// \u1F41C	
	strcpy(symbolEmoji[414].str, "🐝");	// \u1F41D	
	strcpy(symbolEmoji[415].str, "🐞");	// \u1F41E	
	strcpy(symbolEmoji[416].str, "🐟");	// \u1F41F	
	strcpy(symbolEmoji[417].str, "🐠");	// \u1F420	
	strcpy(symbolEmoji[418].str, "🐡");	// \u1F421	
	strcpy(symbolEmoji[419].str, "🐢");	// \u1F422	
	strcpy(symbolEmoji[420].str, "🐣");	// \u1F423	
	strcpy(symbolEmoji[421].str, "🐤");	// \u1F424	
	strcpy(symbolEmoji[422].str, "🐥");	// \u1F425	
	strcpy(symbolEmoji[423].str, "🐦");	// \u1F426	
	strcpy(symbolEmoji[424].str, "🐧");	// \u1F427	
	strcpy(symbolEmoji[425].str, "🐨");	// \u1F428	
	strcpy(symbolEmoji[426].str, "🐩");	// \u1F429	
	strcpy(symbolEmoji[427].str, "🐪");	// \u1F42A	
	strcpy(symbolEmoji[428].str, "🐫");	// \u1F42B	
	strcpy(symbolEmoji[429].str, "🐬");	// \u1F42C	
	strcpy(symbolEmoji[430].str, "🐭");	// \u1F42D	
	strcpy(symbolEmoji[431].str, "🐮");	// \u1F42E	
	strcpy(symbolEmoji[432].str, "🐯");	// \u1F42F	
	strcpy(symbolEmoji[433].str, "🐰");	// \u1F430	
	strcpy(symbolEmoji[434].str, "🐱");	// \u1F431	
	strcpy(symbolEmoji[435].str, "🐲");	// \u1F432	
	strcpy(symbolEmoji[436].str, "🐳");	// \u1F433	
	strcpy(symbolEmoji[437].str, "🐴");	// \u1F434	
	strcpy(symbolEmoji[438].str, "🐵");	// \u1F435	
	strcpy(symbolEmoji[439].str, "🐶");	// \u1F436	
	strcpy(symbolEmoji[440].str, "🐷");	// \u1F437	
	strcpy(symbolEmoji[441].str, "🐸");	// \u1F438	
	strcpy(symbolEmoji[442].str, "🐹");	// \u1F439	
	strcpy(symbolEmoji[443].str, "🐺");	// \u1F43A	
	strcpy(symbolEmoji[444].str, "🐻");	// \u1F43B	
	strcpy(symbolEmoji[445].str, "🐼");	// \u1F43C	
	strcpy(symbolEmoji[446].str, "🐽");	// \u1F43D	
	strcpy(symbolEmoji[447].str, "🐾");	// \u1F43E	
	strcpy(symbolEmoji[448].str, "🐿");	// \u1F43F	
	strcpy(symbolEmoji[449].str, "👀");	// \u1F440	
	strcpy(symbolEmoji[450].str, "👁");	// \u1F441	
	strcpy(symbolEmoji[451].str, "👂");	// \u1F442	
	strcpy(symbolEmoji[452].str, "👃");	// \u1F443	
	strcpy(symbolEmoji[453].str, "👄");	// \u1F444	
	strcpy(symbolEmoji[454].str, "👅");	// \u1F445	
	strcpy(symbolEmoji[455].str, "👆");	// \u1F446	
	strcpy(symbolEmoji[456].str, "👇");	// \u1F447	
	strcpy(symbolEmoji[457].str, "👈");	// \u1F448	
	strcpy(symbolEmoji[458].str, "👉");	// \u1F449	
	strcpy(symbolEmoji[459].str, "👊");	// \u1F44A	
	strcpy(symbolEmoji[460].str, "👋");	// \u1F44B	
	strcpy(symbolEmoji[461].str, "👌");	// \u1F44C	
	strcpy(symbolEmoji[462].str, "👍");	// \u1F44D	
	strcpy(symbolEmoji[463].str, "👎");	// \u1F44E	
	strcpy(symbolEmoji[464].str, "👏");	// \u1F44F	
	strcpy(symbolEmoji[465].str, "👐");	// \u1F450	
	strcpy(symbolEmoji[466].str, "👑");	// \u1F451	
	strcpy(symbolEmoji[467].str, "👒");	// \u1F452	
	strcpy(symbolEmoji[468].str, "👓");	// \u1F453	
	strcpy(symbolEmoji[469].str, "👔");	// \u1F454	
	strcpy(symbolEmoji[470].str, "👕");	// \u1F455	
	strcpy(symbolEmoji[471].str, "👖");	// \u1F456	
	strcpy(symbolEmoji[472].str, "👗");	// \u1F457	
	strcpy(symbolEmoji[473].str, "👘");	// \u1F458	
	strcpy(symbolEmoji[474].str, "👙");	// \u1F459	
	strcpy(symbolEmoji[475].str, "👚");	// \u1F45A	
	strcpy(symbolEmoji[476].str, "👛");	// \u1F45B	
	strcpy(symbolEmoji[477].str, "👜");	// \u1F45C	
	strcpy(symbolEmoji[478].str, "👝");	// \u1F45D	
	strcpy(symbolEmoji[479].str, "👞");	// \u1F45E	
	strcpy(symbolEmoji[480].str, "👟");	// \u1F45F	
	strcpy(symbolEmoji[481].str, "👠");	// \u1F460	
	strcpy(symbolEmoji[482].str, "👡");	// \u1F461	
	strcpy(symbolEmoji[483].str, "👢");	// \u1F462	
	strcpy(symbolEmoji[484].str, "👣");	// \u1F463	
	strcpy(symbolEmoji[485].str, "👤");	// \u1F464	
	strcpy(symbolEmoji[486].str, "👥");	// \u1F465	
	strcpy(symbolEmoji[487].str, "👦");	// \u1F466	
	strcpy(symbolEmoji[488].str, "👧");	// \u1F467	
	strcpy(symbolEmoji[489].str, "👨");	// \u1F468	
	strcpy(symbolEmoji[490].str, "👩");	// \u1F469	
	strcpy(symbolEmoji[491].str, "👪");	// \u1F46A	
	strcpy(symbolEmoji[492].str, "👫");	// \u1F46B	
	strcpy(symbolEmoji[493].str, "👬");	// \u1F46C	
	strcpy(symbolEmoji[494].str, "👭");	// \u1F46D	
	strcpy(symbolEmoji[495].str, "👮");	// \u1F46E	
	strcpy(symbolEmoji[496].str, "👯");	// \u1F46F	
	strcpy(symbolEmoji[497].str, "👰");	// \u1F470	
	strcpy(symbolEmoji[498].str, "👱");	// \u1F471	
	strcpy(symbolEmoji[499].str, "👲");	// \u1F472	
	strcpy(symbolEmoji[500].str, "👳");	// \u1F473	
	strcpy(symbolEmoji[501].str, "👴");	// \u1F474	
	strcpy(symbolEmoji[502].str, "👵");	// \u1F475	
	strcpy(symbolEmoji[503].str, "👶");	// \u1F476	
	strcpy(symbolEmoji[504].str, "👷");	// \u1F477	
	strcpy(symbolEmoji[505].str, "👸");	// \u1F478	
	strcpy(symbolEmoji[506].str, "👹");	// \u1F479	
	strcpy(symbolEmoji[507].str, "👺");	// \u1F47A	
	strcpy(symbolEmoji[508].str, "👻");	// \u1F47B	
	strcpy(symbolEmoji[509].str, "👼");	// \u1F47C	
	strcpy(symbolEmoji[510].str, "👽");	// \u1F47D	
	strcpy(symbolEmoji[511].str, "👾");	// \u1F47E	
	strcpy(symbolEmoji[512].str, "👿");	// \u1F47F	
	strcpy(symbolEmoji[513].str, "💀");	// \u1F480	
	strcpy(symbolEmoji[514].str, "💁");	// \u1F481	
	strcpy(symbolEmoji[515].str, "💂");	// \u1F482	
	strcpy(symbolEmoji[516].str, "💃");	// \u1F483	
	strcpy(symbolEmoji[517].str, "💄");	// \u1F484	
	strcpy(symbolEmoji[518].str, "💅");	// \u1F485	
	strcpy(symbolEmoji[519].str, "💆");	// \u1F486	
	strcpy(symbolEmoji[520].str, "💇");	// \u1F487	
	strcpy(symbolEmoji[521].str, "💈");	// \u1F488	
	strcpy(symbolEmoji[522].str, "💉");	// \u1F489	
	strcpy(symbolEmoji[523].str, "💊");	// \u1F48A	
	strcpy(symbolEmoji[524].str, "💋");	// \u1F48B	
	strcpy(symbolEmoji[525].str, "💌");	// \u1F48C	
	strcpy(symbolEmoji[526].str, "💍");	// \u1F48D	
	strcpy(symbolEmoji[527].str, "💎");	// \u1F48E	
	strcpy(symbolEmoji[528].str, "💏");	// \u1F48F	
	strcpy(symbolEmoji[529].str, "💐");	// \u1F490	
	strcpy(symbolEmoji[530].str, "💑");	// \u1F491	
	strcpy(symbolEmoji[531].str, "💒");	// \u1F492	
	strcpy(symbolEmoji[532].str, "💓");	// \u1F493	
	strcpy(symbolEmoji[533].str, "💔");	// \u1F494	
	strcpy(symbolEmoji[534].str, "💕");	// \u1F495	
	strcpy(symbolEmoji[535].str, "💖");	// \u1F496	
	strcpy(symbolEmoji[536].str, "💗");	// \u1F497	
	strcpy(symbolEmoji[537].str, "💘");	// \u1F498	
	strcpy(symbolEmoji[538].str, "💙");	// \u1F499	
	strcpy(symbolEmoji[539].str, "💚");	// \u1F49A	
	strcpy(symbolEmoji[540].str, "💛");	// \u1F49B	
	strcpy(symbolEmoji[541].str, "💜");	// \u1F49C	
	strcpy(symbolEmoji[542].str, "💝");	// \u1F49D	
	strcpy(symbolEmoji[543].str, "💞");	// \u1F49E	
	strcpy(symbolEmoji[544].str, "💟");	// \u1F49F	
	strcpy(symbolEmoji[545].str, "💠");	// \u1F4A0	
	strcpy(symbolEmoji[546].str, "💡");	// \u1F4A1	
	strcpy(symbolEmoji[547].str, "💢");	// \u1F4A2	
	strcpy(symbolEmoji[548].str, "💣");	// \u1F4A3	
	strcpy(symbolEmoji[549].str, "💤");	// \u1F4A4	
	strcpy(symbolEmoji[550].str, "💥");	// \u1F4A5	
	strcpy(symbolEmoji[551].str, "💦");	// \u1F4A6	
	strcpy(symbolEmoji[552].str, "💧");	// \u1F4A7	
	strcpy(symbolEmoji[553].str, "💨");	// \u1F4A8	
	strcpy(symbolEmoji[554].str, "💩");	// \u1F4A9	
	strcpy(symbolEmoji[555].str, "💪");	// \u1F4AA	
	strcpy(symbolEmoji[556].str, "💫");	// \u1F4AB	
	strcpy(symbolEmoji[557].str, "💬");	// \u1F4AC	
	strcpy(symbolEmoji[558].str, "💭");	// \u1F4AD	
	strcpy(symbolEmoji[559].str, "💮");	// \u1F4AE	
	strcpy(symbolEmoji[560].str, "💯");	// \u1F4AF	
	strcpy(symbolEmoji[561].str, "💰");	// \u1F4B0	
	strcpy(symbolEmoji[562].str, "💱");	// \u1F4B1	
	strcpy(symbolEmoji[563].str, "💲");	// \u1F4B2	
	strcpy(symbolEmoji[564].str, "💳");	// \u1F4B3	
	strcpy(symbolEmoji[565].str, "💴");	// \u1F4B4	
	strcpy(symbolEmoji[566].str, "💵");	// \u1F4B5	
	strcpy(symbolEmoji[567].str, "💶");	// \u1F4B6	
	strcpy(symbolEmoji[568].str, "💷");	// \u1F4B7	
	strcpy(symbolEmoji[569].str, "💸");	// \u1F4B8	
	strcpy(symbolEmoji[570].str, "💹");	// \u1F4B9	
	strcpy(symbolEmoji[571].str, "💺");	// \u1F4BA	
	strcpy(symbolEmoji[572].str, "💻");	// \u1F4BB	
	strcpy(symbolEmoji[573].str, "💼");	// \u1F4BC	
	strcpy(symbolEmoji[574].str, "💽");	// \u1F4BD	
	strcpy(symbolEmoji[575].str, "💾");	// \u1F4BE	
	strcpy(symbolEmoji[576].str, "💿");	// \u1F4BF	
	strcpy(symbolEmoji[577].str, "📀");	// \u1F4C0	
	strcpy(symbolEmoji[578].str, "📁");	// \u1F4C1	
	strcpy(symbolEmoji[579].str, "📂");	// \u1F4C2	
	strcpy(symbolEmoji[580].str, "📃");	// \u1F4C3	
	strcpy(symbolEmoji[581].str, "📄");	// \u1F4C4	
	strcpy(symbolEmoji[582].str, "📅");	// \u1F4C5	
	strcpy(symbolEmoji[583].str, "📆");	// \u1F4C6	
	strcpy(symbolEmoji[584].str, "📇");	// \u1F4C7	
	strcpy(symbolEmoji[585].str, "📈");	// \u1F4C8	
	strcpy(symbolEmoji[586].str, "📉");	// \u1F4C9	
	strcpy(symbolEmoji[587].str, "📊");	// \u1F4CA	
	strcpy(symbolEmoji[588].str, "📋");	// \u1F4CB	
	strcpy(symbolEmoji[589].str, "📌");	// \u1F4CC	
	strcpy(symbolEmoji[590].str, "📍");	// \u1F4CD	
	strcpy(symbolEmoji[591].str, "📎");	// \u1F4CE	
	strcpy(symbolEmoji[592].str, "📏");	// \u1F4CF	
	strcpy(symbolEmoji[593].str, "📐");	// \u1F4D0	
	strcpy(symbolEmoji[594].str, "📑");	// \u1F4D1	
	strcpy(symbolEmoji[595].str, "📒");	// \u1F4D2	
	strcpy(symbolEmoji[596].str, "📓");	// \u1F4D3	
	strcpy(symbolEmoji[597].str, "📔");	// \u1F4D4	
	strcpy(symbolEmoji[598].str, "📕");	// \u1F4D5	
	strcpy(symbolEmoji[599].str, "📖");	// \u1F4D6	
	strcpy(symbolEmoji[600].str, "📗");	// \u1F4D7	
	strcpy(symbolEmoji[601].str, "📘");	// \u1F4D8	
	strcpy(symbolEmoji[602].str, "📙");	// \u1F4D9	
	strcpy(symbolEmoji[603].str, "📚");	// \u1F4DA	
	strcpy(symbolEmoji[604].str, "📛");	// \u1F4DB	
	strcpy(symbolEmoji[605].str, "📜");	// \u1F4DC	
	strcpy(symbolEmoji[606].str, "📝");	// \u1F4DD	
	strcpy(symbolEmoji[607].str, "📞");	// \u1F4DE	
	strcpy(symbolEmoji[608].str, "📟");	// \u1F4DF	
	strcpy(symbolEmoji[609].str, "📠");	// \u1F4E0	
	strcpy(symbolEmoji[610].str, "📡");	// \u1F4E1	
	strcpy(symbolEmoji[611].str, "📢");	// \u1F4E2	
	strcpy(symbolEmoji[612].str, "📣");	// \u1F4E3	
	strcpy(symbolEmoji[613].str, "📤");	// \u1F4E4	
	strcpy(symbolEmoji[614].str, "📥");	// \u1F4E5	
	strcpy(symbolEmoji[615].str, "📦");	// \u1F4E6	
	strcpy(symbolEmoji[616].str, "📧");	// \u1F4E7	
	strcpy(symbolEmoji[617].str, "📨");	// \u1F4E8	
	strcpy(symbolEmoji[618].str, "📩");	// \u1F4E9	
	strcpy(symbolEmoji[619].str, "📪");	// \u1F4EA	
	strcpy(symbolEmoji[620].str, "📫");	// \u1F4EB	
	strcpy(symbolEmoji[621].str, "📬");	// \u1F4EC	
	strcpy(symbolEmoji[622].str, "📭");	// \u1F4ED	
	strcpy(symbolEmoji[623].str, "📮");	// \u1F4EE	
	strcpy(symbolEmoji[624].str, "📯");	// \u1F4EF	
	strcpy(symbolEmoji[625].str, "📰");	// \u1F4F0	
	strcpy(symbolEmoji[626].str, "📱");	// \u1F4F1	
	strcpy(symbolEmoji[627].str, "📲");	// \u1F4F2	
	strcpy(symbolEmoji[628].str, "📳");	// \u1F4F3	
	strcpy(symbolEmoji[629].str, "📴");	// \u1F4F4	
	strcpy(symbolEmoji[630].str, "📵");	// \u1F4F5	
	strcpy(symbolEmoji[631].str, "📶");	// \u1F4F6	
	strcpy(symbolEmoji[632].str, "📷");	// \u1F4F7	
	strcpy(symbolEmoji[633].str, "📸");	// \u1F4F8	
	strcpy(symbolEmoji[634].str, "📹");	// \u1F4F9	
	strcpy(symbolEmoji[635].str, "📺");	// \u1F4FA	
	strcpy(symbolEmoji[636].str, "📻");	// \u1F4FB	
	strcpy(symbolEmoji[637].str, "📼");	// \u1F4FC	
	strcpy(symbolEmoji[638].str, "📽");	// \u1F4FD	
	strcpy(symbolEmoji[639].str, "📿");	// \u1F4FF	
	strcpy(symbolEmoji[640].str, "🔀");	// \u1F500	
	strcpy(symbolEmoji[641].str, "🔁");	// \u1F501	
	strcpy(symbolEmoji[642].str, "🔂");	// \u1F502	
	strcpy(symbolEmoji[643].str, "🔃");	// \u1F503	
	strcpy(symbolEmoji[644].str, "🔄");	// \u1F504	
	strcpy(symbolEmoji[645].str, "🔅");	// \u1F505	
	strcpy(symbolEmoji[646].str, "🔆");	// \u1F506	
	strcpy(symbolEmoji[647].str, "🔇");	// \u1F507	
	strcpy(symbolEmoji[648].str, "🔈");	// \u1F508	
	strcpy(symbolEmoji[649].str, "🔉");	// \u1F509	
	strcpy(symbolEmoji[650].str, "🔊");	// \u1F50A	
	strcpy(symbolEmoji[651].str, "🔋");	// \u1F50B	
	strcpy(symbolEmoji[652].str, "🔌");	// \u1F50C	
	strcpy(symbolEmoji[653].str, "🔍");	// \u1F50D	
	strcpy(symbolEmoji[654].str, "🔎");	// \u1F50E	
	strcpy(symbolEmoji[655].str, "🔏");	// \u1F50F	
	strcpy(symbolEmoji[656].str, "🔐");	// \u1F510	
	strcpy(symbolEmoji[657].str, "🔑");	// \u1F511	
	strcpy(symbolEmoji[658].str, "🔒");	// \u1F512	
	strcpy(symbolEmoji[659].str, "🔓");	// \u1F513	
	strcpy(symbolEmoji[660].str, "🔔");	// \u1F514	
	strcpy(symbolEmoji[661].str, "🔕");	// \u1F515	
	strcpy(symbolEmoji[662].str, "🔖");	// \u1F516	
	strcpy(symbolEmoji[663].str, "🔗");	// \u1F517	
	strcpy(symbolEmoji[664].str, "🔘");	// \u1F518	
	strcpy(symbolEmoji[665].str, "🔙");	// \u1F519	
	strcpy(symbolEmoji[666].str, "🔚");	// \u1F51A	
	strcpy(symbolEmoji[667].str, "🔛");	// \u1F51B	
	strcpy(symbolEmoji[668].str, "🔜");	// \u1F51C	
	strcpy(symbolEmoji[669].str, "🔝");	// \u1F51D	
	strcpy(symbolEmoji[670].str, "🔞");	// \u1F51E	
	strcpy(symbolEmoji[671].str, "🔟");	// \u1F51F	
	strcpy(symbolEmoji[672].str, "🔠");	// \u1F520	
	strcpy(symbolEmoji[673].str, "🔡");	// \u1F521	
	strcpy(symbolEmoji[674].str, "🔢");	// \u1F522	
	strcpy(symbolEmoji[675].str, "🔣");	// \u1F523	
	strcpy(symbolEmoji[676].str, "🔤");	// \u1F524	
	strcpy(symbolEmoji[677].str, "🔥");	// \u1F525	
	strcpy(symbolEmoji[678].str, "🔦");	// \u1F526	
	strcpy(symbolEmoji[679].str, "🔧");	// \u1F527	
	strcpy(symbolEmoji[680].str, "🔨");	// \u1F528	
	strcpy(symbolEmoji[681].str, "🔩");	// \u1F529	
	strcpy(symbolEmoji[682].str, "🔪");	// \u1F52A	
	strcpy(symbolEmoji[683].str, "🔫");	// \u1F52B	
	strcpy(symbolEmoji[684].str, "🔬");	// \u1F52C	
	strcpy(symbolEmoji[685].str, "🔭");	// \u1F52D	
	strcpy(symbolEmoji[686].str, "🔮");	// \u1F52E	
	strcpy(symbolEmoji[687].str, "🔯");	// \u1F52F	
	strcpy(symbolEmoji[688].str, "🔰");	// \u1F530	
	strcpy(symbolEmoji[689].str, "🔱");	// \u1F531	
	strcpy(symbolEmoji[690].str, "🔲");	// \u1F532	
	strcpy(symbolEmoji[691].str, "🔳");	// \u1F533	
	strcpy(symbolEmoji[692].str, "🔴");	// \u1F534	
	strcpy(symbolEmoji[693].str, "🔵");	// \u1F535	
	strcpy(symbolEmoji[694].str, "🔶");	// \u1F536	
	strcpy(symbolEmoji[695].str, "🔷");	// \u1F537	
	strcpy(symbolEmoji[696].str, "🔸");	// \u1F538	
	strcpy(symbolEmoji[697].str, "🔹");	// \u1F539	
	strcpy(symbolEmoji[698].str, "🔺");	// \u1F53A	
	strcpy(symbolEmoji[699].str, "🔻");	// \u1F53B	
	strcpy(symbolEmoji[700].str, "🔼");	// \u1F53C	
	strcpy(symbolEmoji[701].str, "🔽");	// \u1F53D	
	strcpy(symbolEmoji[702].str, "🕉");	// \u1F549	
	strcpy(symbolEmoji[703].str, "🕊");	// \u1F54A	
	strcpy(symbolEmoji[704].str, "🕋");	// \u1F54B	
	strcpy(symbolEmoji[705].str, "🕌");	// \u1F54C	
	strcpy(symbolEmoji[706].str, "🕍");	// \u1F54D	
	strcpy(symbolEmoji[707].str, "🕎");	// \u1F54E	
	strcpy(symbolEmoji[708].str, "🕐");	// \u1F550	
	strcpy(symbolEmoji[709].str, "🕑");	// \u1F551	
	strcpy(symbolEmoji[710].str, "🕒");	// \u1F552	
	strcpy(symbolEmoji[711].str, "🕓");	// \u1F553	
	strcpy(symbolEmoji[712].str, "🕔");	// \u1F554	
	strcpy(symbolEmoji[713].str, "🕕");	// \u1F555	
	strcpy(symbolEmoji[714].str, "🕖");	// \u1F556	
	strcpy(symbolEmoji[715].str, "🕗");	// \u1F557	
	strcpy(symbolEmoji[716].str, "🕘");	// \u1F558	
	strcpy(symbolEmoji[717].str, "🕙");	// \u1F559	
	strcpy(symbolEmoji[718].str, "🕚");	// \u1F55A	
	strcpy(symbolEmoji[719].str, "🕛");	// \u1F55B	
	strcpy(symbolEmoji[720].str, "🕜");	// \u1F55C	
	strcpy(symbolEmoji[721].str, "🕝");	// \u1F55D	
	strcpy(symbolEmoji[722].str, "🕞");	// \u1F55E	
	strcpy(symbolEmoji[723].str, "🕟");	// \u1F55F	
	strcpy(symbolEmoji[724].str, "🕠");	// \u1F560	
	strcpy(symbolEmoji[725].str, "🕡");	// \u1F561	
	strcpy(symbolEmoji[726].str, "🕢");	// \u1F562	
	strcpy(symbolEmoji[727].str, "🕣");	// \u1F563	
	strcpy(symbolEmoji[728].str, "🕤");	// \u1F564	
	strcpy(symbolEmoji[729].str, "🕥");	// \u1F565	
	strcpy(symbolEmoji[730].str, "🕦");	// \u1F566	
	strcpy(symbolEmoji[731].str, "🕧");	// \u1F567	
	strcpy(symbolEmoji[732].str, "🕯");	// \u1F56F	
	strcpy(symbolEmoji[733].str, "🕰");	// \u1F570	
	strcpy(symbolEmoji[734].str, "🕳");	// \u1F573	
	strcpy(symbolEmoji[735].str, "🕴");	// \u1F574	
	strcpy(symbolEmoji[736].str, "🕵");	// \u1F575	
	strcpy(symbolEmoji[737].str, "🕶");	// \u1F576	
	strcpy(symbolEmoji[738].str, "🕷");	// \u1F577	
	strcpy(symbolEmoji[739].str, "🕸");	// \u1F578	
	strcpy(symbolEmoji[740].str, "🕹");	// \u1F579	
	strcpy(symbolEmoji[741].str, "🕺");	// \u1F57A	
	strcpy(symbolEmoji[742].str, "🖇");	// \u1F587	
	strcpy(symbolEmoji[743].str, "🖊");	// \u1F58A	
	strcpy(symbolEmoji[744].str, "🖋");	// \u1F58B	
	strcpy(symbolEmoji[745].str, "🖌");	// \u1F58C	
	strcpy(symbolEmoji[746].str, "🖍");	// \u1F58D	
	strcpy(symbolEmoji[747].str, "🖐");	// \u1F590	
	strcpy(symbolEmoji[748].str, "🖕");	// \u1F595	
	strcpy(symbolEmoji[749].str, "🖖");	// \u1F596	
	strcpy(symbolEmoji[750].str, "🖤");	// \u1F5A4	
	strcpy(symbolEmoji[751].str, "🖥");	// \u1F5A5	
	strcpy(symbolEmoji[752].str, "🖨");	// \u1F5A8	
	strcpy(symbolEmoji[753].str, "🖱");	// \u1F5B1	
	strcpy(symbolEmoji[754].str, "🖲");	// \u1F5B2	
	strcpy(symbolEmoji[755].str, "🖼");	// \u1F5BC	
	strcpy(symbolEmoji[756].str, "🗂");	// \u1F5C2	
	strcpy(symbolEmoji[757].str, "🗃");	// \u1F5C3	
	strcpy(symbolEmoji[758].str, "🗄");	// \u1F5C4	
	strcpy(symbolEmoji[759].str, "🗑");	// \u1F5D1	
	strcpy(symbolEmoji[760].str, "🗒");	// \u1F5D2	
	strcpy(symbolEmoji[761].str, "🗓");	// \u1F5D3	
	strcpy(symbolEmoji[762].str, "🗜");	// \u1F5DC	
	strcpy(symbolEmoji[763].str, "🗝");	// \u1F5DD	
	strcpy(symbolEmoji[764].str, "🗞");	// \u1F5DE	
	strcpy(symbolEmoji[765].str, "🗡");	// \u1F5E1	
	strcpy(symbolEmoji[766].str, "🗣");	// \u1F5E3	
	strcpy(symbolEmoji[767].str, "🗨");	// \u1F5E8	
	strcpy(symbolEmoji[768].str, "🗯");	// \u1F5EF	
	strcpy(symbolEmoji[769].str, "🗳");	// \u1F5F3	
	strcpy(symbolEmoji[770].str, "🗺");	// \u1F5FA	
	strcpy(symbolEmoji[771].str, "🗻");	// \u1F5FB	
	strcpy(symbolEmoji[772].str, "🗼");	// \u1F5FC	
	strcpy(symbolEmoji[773].str, "🗽");	// \u1F5FD	
	strcpy(symbolEmoji[774].str, "🗾");	// \u1F5FE	
	strcpy(symbolEmoji[775].str, "🗿");	// \u1F5FF	
	strcpy(symbolEmoji[776].str, "😀");	// \u1F600	
	strcpy(symbolEmoji[777].str, "😁");	// \u1F601	
	strcpy(symbolEmoji[778].str, "😂");	// \u1F602	
	strcpy(symbolEmoji[779].str, "😃");	// \u1F603	
	strcpy(symbolEmoji[780].str, "😄");	// \u1F604	
	strcpy(symbolEmoji[781].str, "😅");	// \u1F605	
	strcpy(symbolEmoji[782].str, "😆");	// \u1F606	
	strcpy(symbolEmoji[783].str, "😇");	// \u1F607	
	strcpy(symbolEmoji[784].str, "😈");	// \u1F608	
	strcpy(symbolEmoji[785].str, "😉");	// \u1F609	
	strcpy(symbolEmoji[786].str, "😊");	// \u1F60A	
	strcpy(symbolEmoji[787].str, "😋");	// \u1F60B	
	strcpy(symbolEmoji[788].str, "😌");	// \u1F60C	
	strcpy(symbolEmoji[789].str, "😍");	// \u1F60D	
	strcpy(symbolEmoji[790].str, "😎");	// \u1F60E	
	strcpy(symbolEmoji[791].str, "😏");	// \u1F60F	
	strcpy(symbolEmoji[792].str, "😐");	// \u1F610	
	strcpy(symbolEmoji[793].str, "😑");	// \u1F611	
	strcpy(symbolEmoji[794].str, "😒");	// \u1F612	
	strcpy(symbolEmoji[795].str, "😓");	// \u1F613	
	strcpy(symbolEmoji[796].str, "😔");	// \u1F614	
	strcpy(symbolEmoji[797].str, "😕");	// \u1F615	
	strcpy(symbolEmoji[798].str, "😖");	// \u1F616	
	strcpy(symbolEmoji[799].str, "😗");	// \u1F617	
	strcpy(symbolEmoji[800].str, "😘");	// \u1F618	
	strcpy(symbolEmoji[801].str, "😙");	// \u1F619	
	strcpy(symbolEmoji[802].str, "😚");	// \u1F61A	
	strcpy(symbolEmoji[803].str, "😛");	// \u1F61B	
	strcpy(symbolEmoji[804].str, "😜");	// \u1F61C	
	strcpy(symbolEmoji[805].str, "😝");	// \u1F61D	
	strcpy(symbolEmoji[806].str, "😞");	// \u1F61E	
	strcpy(symbolEmoji[807].str, "😟");	// \u1F61F	
	strcpy(symbolEmoji[808].str, "😠");	// \u1F620	
	strcpy(symbolEmoji[809].str, "😡");	// \u1F621	
	strcpy(symbolEmoji[810].str, "😢");	// \u1F622	
	strcpy(symbolEmoji[811].str, "😣");	// \u1F623	
	strcpy(symbolEmoji[812].str, "😤");	// \u1F624	
	strcpy(symbolEmoji[813].str, "😥");	// \u1F625	
	strcpy(symbolEmoji[814].str, "😦");	// \u1F626	
	strcpy(symbolEmoji[815].str, "😧");	// \u1F627	
	strcpy(symbolEmoji[816].str, "😨");	// \u1F628	
	strcpy(symbolEmoji[817].str, "😩");	// \u1F629	
	strcpy(symbolEmoji[818].str, "😪");	// \u1F62A	
	strcpy(symbolEmoji[819].str, "😫");	// \u1F62B	
	strcpy(symbolEmoji[820].str, "😬");	// \u1F62C	
	strcpy(symbolEmoji[821].str, "😭");	// \u1F62D	
	strcpy(symbolEmoji[822].str, "😮");	// \u1F62E	
	strcpy(symbolEmoji[823].str, "😯");	// \u1F62F	
	strcpy(symbolEmoji[824].str, "😰");	// \u1F630	
	strcpy(symbolEmoji[825].str, "😱");	// \u1F631	
	strcpy(symbolEmoji[826].str, "😲");	// \u1F632	
	strcpy(symbolEmoji[827].str, "😳");	// \u1F633	
	strcpy(symbolEmoji[828].str, "😴");	// \u1F634	
	strcpy(symbolEmoji[829].str, "😵");	// \u1F635	
	strcpy(symbolEmoji[830].str, "😶");	// \u1F636	
	strcpy(symbolEmoji[831].str, "😷");	// \u1F637	
	strcpy(symbolEmoji[832].str, "😸");	// \u1F638	
	strcpy(symbolEmoji[833].str, "😹");	// \u1F639	
	strcpy(symbolEmoji[834].str, "😺");	// \u1F63A	
	strcpy(symbolEmoji[835].str, "😻");	// \u1F63B	
	strcpy(symbolEmoji[836].str, "😼");	// \u1F63C	
	strcpy(symbolEmoji[837].str, "😽");	// \u1F63D	
	strcpy(symbolEmoji[838].str, "😾");	// \u1F63E	
	strcpy(symbolEmoji[839].str, "😿");	// \u1F63F	
	strcpy(symbolEmoji[840].str, "🙀");	// \u1F640	
	strcpy(symbolEmoji[841].str, "🙁");	// \u1F641	
	strcpy(symbolEmoji[842].str, "🙂");	// \u1F642	
	strcpy(symbolEmoji[843].str, "🙃");	// \u1F643	
	strcpy(symbolEmoji[844].str, "🙄");	// \u1F644	
	strcpy(symbolEmoji[845].str, "🙅");	// \u1F645	
	strcpy(symbolEmoji[846].str, "🙆");	// \u1F646	
	strcpy(symbolEmoji[847].str, "🙇");	// \u1F647	
	strcpy(symbolEmoji[848].str, "🙈");	// \u1F648	
	strcpy(symbolEmoji[849].str, "🙉");	// \u1F649	
	strcpy(symbolEmoji[850].str, "🙊");	// \u1F64A	
	strcpy(symbolEmoji[851].str, "🙋");	// \u1F64B	
	strcpy(symbolEmoji[852].str, "🙌");	// \u1F64C	
	strcpy(symbolEmoji[853].str, "🙍");	// \u1F64D	
	strcpy(symbolEmoji[854].str, "🙎");	// \u1F64E	
	strcpy(symbolEmoji[855].str, "🙏");	// \u1F64F	
	strcpy(symbolEmoji[856].str, "🚀");	// \u1F680	
	strcpy(symbolEmoji[857].str, "🚁");	// \u1F681	
	strcpy(symbolEmoji[858].str, "🚂");	// \u1F682	
	strcpy(symbolEmoji[859].str, "🚃");	// \u1F683	
	strcpy(symbolEmoji[860].str, "🚄");	// \u1F684	
	strcpy(symbolEmoji[861].str, "🚅");	// \u1F685	
	strcpy(symbolEmoji[862].str, "🚆");	// \u1F686	
	strcpy(symbolEmoji[863].str, "🚇");	// \u1F687	
	strcpy(symbolEmoji[864].str, "🚈");	// \u1F688	
	strcpy(symbolEmoji[865].str, "🚉");	// \u1F689	
	strcpy(symbolEmoji[866].str, "🚊");	// \u1F68A	
	strcpy(symbolEmoji[867].str, "🚋");	// \u1F68B	
	strcpy(symbolEmoji[868].str, "🚌");	// \u1F68C	
	strcpy(symbolEmoji[869].str, "🚍");	// \u1F68D	
	strcpy(symbolEmoji[870].str, "🚎");	// \u1F68E	
	strcpy(symbolEmoji[871].str, "🚏");	// \u1F68F	
	strcpy(symbolEmoji[872].str, "🚐");	// \u1F690	
	strcpy(symbolEmoji[873].str, "🚑");	// \u1F691	
	strcpy(symbolEmoji[874].str, "🚒");	// \u1F692	
	strcpy(symbolEmoji[875].str, "🚓");	// \u1F693	
	strcpy(symbolEmoji[876].str, "🚔");	// \u1F694	
	strcpy(symbolEmoji[877].str, "🚕");	// \u1F695	
	strcpy(symbolEmoji[878].str, "🚖");	// \u1F696	
	strcpy(symbolEmoji[879].str, "🚗");	// \u1F697	
	strcpy(symbolEmoji[880].str, "🚘");	// \u1F698	
	strcpy(symbolEmoji[881].str, "🚙");	// \u1F699	
	strcpy(symbolEmoji[882].str, "🚚");	// \u1F69A	
	strcpy(symbolEmoji[883].str, "🚛");	// \u1F69B	
	strcpy(symbolEmoji[884].str, "🚜");	// \u1F69C	
	strcpy(symbolEmoji[885].str, "🚝");	// \u1F69D	
	strcpy(symbolEmoji[886].str, "🚞");	// \u1F69E	
	strcpy(symbolEmoji[887].str, "🚟");	// \u1F69F	
	strcpy(symbolEmoji[888].str, "🚠");	// \u1F6A0	
	strcpy(symbolEmoji[889].str, "🚡");	// \u1F6A1	
	strcpy(symbolEmoji[890].str, "🚢");	// \u1F6A2	
	strcpy(symbolEmoji[891].str, "🚣");	// \u1F6A3	
	strcpy(symbolEmoji[892].str, "🚤");	// \u1F6A4	
	strcpy(symbolEmoji[893].str, "🚥");	// \u1F6A5	
	strcpy(symbolEmoji[894].str, "🚦");	// \u1F6A6	
	strcpy(symbolEmoji[895].str, "🚧");	// \u1F6A7	
	strcpy(symbolEmoji[896].str, "🚨");	// \u1F6A8	
	strcpy(symbolEmoji[897].str, "🚩");	// \u1F6A9	
	strcpy(symbolEmoji[898].str, "🚪");	// \u1F6AA	
	strcpy(symbolEmoji[899].str, "🚫");	// \u1F6AB	
	strcpy(symbolEmoji[900].str, "🚬");	// \u1F6AC	
	strcpy(symbolEmoji[901].str, "🚭");	// \u1F6AD	
	strcpy(symbolEmoji[902].str, "🚮");	// \u1F6AE	
	strcpy(symbolEmoji[903].str, "🚯");	// \u1F6AF	
	strcpy(symbolEmoji[904].str, "🚰");	// \u1F6B0	
	strcpy(symbolEmoji[905].str, "🚱");	// \u1F6B1	
	strcpy(symbolEmoji[906].str, "🚲");	// \u1F6B2	
	strcpy(symbolEmoji[907].str, "🚳");	// \u1F6B3	
	strcpy(symbolEmoji[908].str, "🚴");	// \u1F6B4	
	strcpy(symbolEmoji[909].str, "🚵");	// \u1F6B5	
	strcpy(symbolEmoji[910].str, "🚶");	// \u1F6B6	
	strcpy(symbolEmoji[911].str, "🚷");	// \u1F6B7	
	strcpy(symbolEmoji[912].str, "🚸");	// \u1F6B8	
	strcpy(symbolEmoji[913].str, "🚹");	// \u1F6B9	
	strcpy(symbolEmoji[914].str, "🚺");	// \u1F6BA	
	strcpy(symbolEmoji[915].str, "🚻");	// \u1F6BB	
	strcpy(symbolEmoji[916].str, "🚼");	// \u1F6BC	
	strcpy(symbolEmoji[917].str, "🚽");	// \u1F6BD	
	strcpy(symbolEmoji[918].str, "🚾");	// \u1F6BE	
	strcpy(symbolEmoji[919].str, "🚿");	// \u1F6BF	
	strcpy(symbolEmoji[920].str, "🛀");	// \u1F6C0	
	strcpy(symbolEmoji[921].str, "🛁");	// \u1F6C1	
	strcpy(symbolEmoji[922].str, "🛂");	// \u1F6C2	
	strcpy(symbolEmoji[923].str, "🛃");	// \u1F6C3	
	strcpy(symbolEmoji[924].str, "🛄");	// \u1F6C4	
	strcpy(symbolEmoji[925].str, "🛅");	// \u1F6C5	
	strcpy(symbolEmoji[926].str, "🛋");	// \u1F6CB	
	strcpy(symbolEmoji[927].str, "🛌");	// \u1F6CC	
	strcpy(symbolEmoji[928].str, "🛍");	// \u1F6CD	
	strcpy(symbolEmoji[929].str, "🛎");	// \u1F6CE	
	strcpy(symbolEmoji[930].str, "🛏");	// \u1F6CF	
	strcpy(symbolEmoji[931].str, "🛐");	// \u1F6D0	
	strcpy(symbolEmoji[932].str, "🛑");	// \u1F6D1	
	strcpy(symbolEmoji[933].str, "🛒");	// \u1F6D2	
	strcpy(symbolEmoji[934].str, "🛠");	// \u1F6E0	
	strcpy(symbolEmoji[935].str, "🛡");	// \u1F6E1	
	strcpy(symbolEmoji[936].str, "🛢");	// \u1F6E2	
	strcpy(symbolEmoji[937].str, "🛣");	// \u1F6E3	
	strcpy(symbolEmoji[938].str, "🛤");	// \u1F6E4	
	strcpy(symbolEmoji[939].str, "🛥");	// \u1F6E5	
	strcpy(symbolEmoji[940].str, "🛩");	// \u1F6E9	
	strcpy(symbolEmoji[941].str, "🛫");	// \u1F6EB	
	strcpy(symbolEmoji[942].str, "🛬");	// \u1F6EC	
	strcpy(symbolEmoji[943].str, "🛰");	// \u1F6F0	
	strcpy(symbolEmoji[944].str, "🛳");	// \u1F6F3	
	strcpy(symbolEmoji[945].str, "🛴");	// \u1F6F4	
	strcpy(symbolEmoji[946].str, "🛵");	// \u1F6F5	
	strcpy(symbolEmoji[947].str, "🛶");	// \u1F6F6	
	strcpy(symbolEmoji[948].str, "🛷");	// \u1F6F7	
	strcpy(symbolEmoji[949].str, "🛸");	// \u1F6F8	
	strcpy(symbolEmoji[950].str, "🛹");	// \u1F6F9	
	strcpy(symbolEmoji[951].str, "🛺");	// \u1F6FA	
	strcpy(symbolEmoji[952].str, "🤐");	// \u1F910	
	strcpy(symbolEmoji[953].str, "🤑");	// \u1F911	
	strcpy(symbolEmoji[954].str, "🤒");	// \u1F912	
	strcpy(symbolEmoji[955].str, "🤓");	// \u1F913	
	strcpy(symbolEmoji[956].str, "🤔");	// \u1F914	
	strcpy(symbolEmoji[957].str, "🤕");	// \u1F915	
	strcpy(symbolEmoji[958].str, "🤖");	// \u1F916	
	strcpy(symbolEmoji[959].str, "🤗");	// \u1F917	
	strcpy(symbolEmoji[960].str, "🤘");	// \u1F918	
	strcpy(symbolEmoji[961].str, "🤙");	// \u1F919	
	strcpy(symbolEmoji[962].str, "🤚");	// \u1F91A	
	strcpy(symbolEmoji[963].str, "🤛");	// \u1F91B	
	strcpy(symbolEmoji[964].str, "🤜");	// \u1F91C	
	strcpy(symbolEmoji[965].str, "🤝");	// \u1F91D	
	strcpy(symbolEmoji[966].str, "🤞");	// \u1F91E	
	strcpy(symbolEmoji[967].str, "🤟");	// \u1F91F	
	strcpy(symbolEmoji[968].str, "🤠");	// \u1F920	
	strcpy(symbolEmoji[969].str, "🤡");	// \u1F921	
	strcpy(symbolEmoji[970].str, "🤢");	// \u1F922	
	strcpy(symbolEmoji[971].str, "🤣");	// \u1F923	
	strcpy(symbolEmoji[972].str, "🤤");	// \u1F924	
	strcpy(symbolEmoji[973].str, "🤥");	// \u1F925	
	strcpy(symbolEmoji[974].str, "🤦");	// \u1F926	
	strcpy(symbolEmoji[975].str, "🤧");	// \u1F927	
	strcpy(symbolEmoji[976].str, "🤨");	// \u1F928	
	strcpy(symbolEmoji[977].str, "🤩");	// \u1F929	
	strcpy(symbolEmoji[978].str, "🤪");	// \u1F92A	
	strcpy(symbolEmoji[979].str, "🤫");	// \u1F92B	
	strcpy(symbolEmoji[980].str, "🤬");	// \u1F92C	
	strcpy(symbolEmoji[981].str, "🤭");	// \u1F92D	
	strcpy(symbolEmoji[982].str, "🤮");	// \u1F92E	
	strcpy(symbolEmoji[983].str, "🤯");	// \u1F92F	
	strcpy(symbolEmoji[984].str, "🤰");	// \u1F930	
	strcpy(symbolEmoji[985].str, "🤱");	// \u1F931	
	strcpy(symbolEmoji[986].str, "🤲");	// \u1F932	
	strcpy(symbolEmoji[987].str, "🤳");	// \u1F933	
	strcpy(symbolEmoji[988].str, "🤴");	// \u1F934	
	strcpy(symbolEmoji[989].str, "🤵");	// \u1F935	
	strcpy(symbolEmoji[990].str, "🤶");	// \u1F936	
	strcpy(symbolEmoji[991].str, "🤷");	// \u1F937	
	strcpy(symbolEmoji[992].str, "🤸");	// \u1F938	
	strcpy(symbolEmoji[993].str, "🤹");	// \u1F939	
	strcpy(symbolEmoji[994].str, "🤺");	// \u1F93A	
	strcpy(symbolEmoji[995].str, "🤼");	// \u1F93C	
	strcpy(symbolEmoji[996].str, "🤽");	// \u1F93D	
	strcpy(symbolEmoji[997].str, "🤾");	// \u1F93E	
	strcpy(symbolEmoji[998].str, "🥀");	// \u1F940	
	strcpy(symbolEmoji[999].str, "🥁");	// \u1F941	
	strcpy(symbolEmoji[1000].str, "🥂");	// \u1F942	
	strcpy(symbolEmoji[1001].str, "🥃");	// \u1F943	
	strcpy(symbolEmoji[1002].str, "🥄");	// \u1F944	
	strcpy(symbolEmoji[1003].str, "🥅");	// \u1F945	
	strcpy(symbolEmoji[1004].str, "🥇");	// \u1F947	
	strcpy(symbolEmoji[1005].str, "🥈");	// \u1F948	
	strcpy(symbolEmoji[1006].str, "🥉");	// \u1F949	
	strcpy(symbolEmoji[1007].str, "🥊");	// \u1F94A	
	strcpy(symbolEmoji[1008].str, "🥋");	// \u1F94B	
	strcpy(symbolEmoji[1009].str, "🥌");	// \u1F94C	
	strcpy(symbolEmoji[1010].str, "🥍");	// \u1F94D	
	strcpy(symbolEmoji[1011].str, "🥎");	// \u1F94E	
	strcpy(symbolEmoji[1012].str, "🥏");	// \u1F94F	
	strcpy(symbolEmoji[1013].str, "🥐");	// \u1F950	
	strcpy(symbolEmoji[1014].str, "🥑");	// \u1F951	
	strcpy(symbolEmoji[1015].str, "🥒");	// \u1F952	
	strcpy(symbolEmoji[1016].str, "🥓");	// \u1F953	
	strcpy(symbolEmoji[1017].str, "🥔");	// \u1F954	
	strcpy(symbolEmoji[1018].str, "🥕");	// \u1F955	
	strcpy(symbolEmoji[1019].str, "🥖");	// \u1F956	
	strcpy(symbolEmoji[1020].str, "🥗");	// \u1F957	
	strcpy(symbolEmoji[1021].str, "🥘");	// \u1F958	
	strcpy(symbolEmoji[1022].str, "🥙");	// \u1F959	
	strcpy(symbolEmoji[1023].str, "🥚");	// \u1F95A	
	strcpy(symbolEmoji[1024].str, "🥛");	// \u1F95B	
	strcpy(symbolEmoji[1025].str, "🥜");	// \u1F95C	
	strcpy(symbolEmoji[1026].str, "🥝");	// \u1F95D	
	strcpy(symbolEmoji[1027].str, "🥞");	// \u1F95E	
	strcpy(symbolEmoji[1028].str, "🥟");	// \u1F95F	
	strcpy(symbolEmoji[1029].str, "🥠");	// \u1F960	
	strcpy(symbolEmoji[1030].str, "🥡");	// \u1F961	
	strcpy(symbolEmoji[1031].str, "🥢");	// \u1F962	
	strcpy(symbolEmoji[1032].str, "🥣");	// \u1F963	
	strcpy(symbolEmoji[1033].str, "🥤");	// \u1F964	
	strcpy(symbolEmoji[1034].str, "🥥");	// \u1F965	
	strcpy(symbolEmoji[1035].str, "🥦");	// \u1F966	
	strcpy(symbolEmoji[1036].str, "🥧");	// \u1F967	
	strcpy(symbolEmoji[1037].str, "🥨");	// \u1F968	
	strcpy(symbolEmoji[1038].str, "🥩");	// \u1F969	
	strcpy(symbolEmoji[1039].str, "🥪");	// \u1F96A	
	strcpy(symbolEmoji[1040].str, "🥫");	// \u1F96B	
	strcpy(symbolEmoji[1041].str, "🦀");	// \u1F980	
	strcpy(symbolEmoji[1042].str, "🦁");	// \u1F981	
	strcpy(symbolEmoji[1043].str, "🦂");	// \u1F982	
	strcpy(symbolEmoji[1044].str, "🦃");	// \u1F983	
	strcpy(symbolEmoji[1045].str, "🦄");	// \u1F984	
	strcpy(symbolEmoji[1046].str, "🦅");	// \u1F985	
	strcpy(symbolEmoji[1047].str, "🦆");	// \u1F986	
	strcpy(symbolEmoji[1048].str, "🦇");	// \u1F987	
	strcpy(symbolEmoji[1049].str, "🦈");	// \u1F988	
	strcpy(symbolEmoji[1050].str, "🦉");	// \u1F989	
	strcpy(symbolEmoji[1051].str, "🦊");	// \u1F98A	
	strcpy(symbolEmoji[1052].str, "🦋");	// \u1F98B	
	strcpy(symbolEmoji[1053].str, "🦌");	// \u1F98C	
	strcpy(symbolEmoji[1054].str, "🦍");	// \u1F98D	
	strcpy(symbolEmoji[1055].str, "🦎");	// \u1F98E	
	strcpy(symbolEmoji[1056].str, "🦏");	// \u1F98F	
	strcpy(symbolEmoji[1057].str, "🦐");	// \u1F990	
	strcpy(symbolEmoji[1058].str, "🦑");	// \u1F991	
	strcpy(symbolEmoji[1059].str, "🦒");	// \u1F992	
	strcpy(symbolEmoji[1060].str, "🦓");	// \u1F993	
	strcpy(symbolEmoji[1061].str, "🦔");	// \u1F994	
	strcpy(symbolEmoji[1062].str, "🦕");	// \u1F995	
	strcpy(symbolEmoji[1063].str, "🦖");	// \u1F996	
	strcpy(symbolEmoji[1064].str, "🦗");	// \u1F997	
	strcpy(symbolEmoji[1065].str, "🧀");	// \u1F9C0	
	strcpy(symbolEmoji[1066].str, "🧐");	// \u1F9D0	
	strcpy(symbolEmoji[1067].str, "🧑");	// \u1F9D1	
	strcpy(symbolEmoji[1068].str, "🧒");	// \u1F9D2	
	strcpy(symbolEmoji[1069].str, "🧓");	// \u1F9D3	
	strcpy(symbolEmoji[1070].str, "🧔");	// \u1F9D4	
	strcpy(symbolEmoji[1071].str, "🧕");	// \u1F9D5	
	strcpy(symbolEmoji[1072].str, "🧖");	// \u1F9D6	
	strcpy(symbolEmoji[1073].str, "🧗");	// \u1F9D7	
	strcpy(symbolEmoji[1074].str, "🧘");	// \u1F9D8	
	strcpy(symbolEmoji[1075].str, "🧙");	// \u1F9D9	
	strcpy(symbolEmoji[1076].str, "🧚");	// \u1F9DA	
	strcpy(symbolEmoji[1077].str, "🧛");	// \u1F9DB	
	strcpy(symbolEmoji[1078].str, "🧜");	// \u1F9DC	
	strcpy(symbolEmoji[1079].str, "🧝");	// \u1F9DD	
	strcpy(symbolEmoji[1080].str, "🧞");	// \u1F9DE	
	strcpy(symbolEmoji[1081].str, "🧟");	// \u1F9DF	
	strcpy(symbolEmoji[1082].str, "🧠");	// \u1F9E0	
	strcpy(symbolEmoji[1083].str, "🧡");	// \u1F9E1	
	strcpy(symbolEmoji[1084].str, "🧢");	// \u1F9E2	
	strcpy(symbolEmoji[1085].str, "🧣");	// \u1F9E3	
	strcpy(symbolEmoji[1086].str, "🧤");	// \u1F9E4	
	strcpy(symbolEmoji[1087].str, "🧥");	// \u1F9E5	
	strcpy(symbolEmoji[1088].str, "🧦");	// \u1F9E6	

	strcpy(symbolSmiley[1].str, "😀");	// \u1F600	GRINNING FACE
	strcpy(symbolSmiley[2].str, "😁");	// \u1F601	GRINNING FACE WITH SMILING EYES
	strcpy(symbolSmiley[3].str, "😂");	// \u1F602	FACE WITH TEARS OF JOY
	strcpy(symbolSmiley[4].str, "😃");	// \u1F603	SMILING FACE WITH OPEN MOUTH
	strcpy(symbolSmiley[5].str, "😄");	// \u1F604	SMILING FACE WITH OPEN MOUTH AND SMILING EYES
	strcpy(symbolSmiley[6].str, "😅");	// \u1F605	SMILING FACE WITH OPEN MOUTH AND COLD SWEAT
	strcpy(symbolSmiley[7].str, "😆");	// \u1F606	SMILING FACE WITH OPEN MOUTH AND TIGHTLY-CLOSED EYES
	strcpy(symbolSmiley[8].str, "😇");	// \u1F607	SMILING FACE WITH HALO
	strcpy(symbolSmiley[9].str, "😈");	// \u1F608	SMILING FACE WITH HORNS
	strcpy(symbolSmiley[10].str, "😉");	// \u1F609	WINKING FACE
	strcpy(symbolSmiley[11].str, "😊");	// \u1F60A	SMILING FACE WITH SMILING EYES
	strcpy(symbolSmiley[12].str, "😋");	// \u1F60B	FACE SAVOURING DELICIOUS FOOD
	strcpy(symbolSmiley[13].str, "😌");	// \u1F60C	RELIEVED FACE
	strcpy(symbolSmiley[14].str, "😍");	// \u1F60D	SMILING FACE WITH HEART-SHAPED EYES
	strcpy(symbolSmiley[15].str, "😎");	// \u1F60E	SMILING FACE WITH SUNGLASSES
	strcpy(symbolSmiley[16].str, "😏");	// \u1F60F	SMIRKING FACE
	strcpy(symbolSmiley[17].str, "😐");	// \u1F610	NEUTRAL FACE
	strcpy(symbolSmiley[18].str, "😑");	// \u1F611	EXPRESSIONLESS FACE
	strcpy(symbolSmiley[19].str, "😒");	// \u1F612	UNAMUSED FACE
	strcpy(symbolSmiley[20].str, "😓");	// \u1F613	FACE WITH COLD SWEAT
	strcpy(symbolSmiley[21].str, "😔");	// \u1F614	PENSIVE FACE
	strcpy(symbolSmiley[22].str, "😕");	// \u1F615	CONFUSED FACE
	strcpy(symbolSmiley[23].str, "😖");	// \u1F616	CONFOUNDED FACE
	strcpy(symbolSmiley[24].str, "😗");	// \u1F617	KISSING FACE
	strcpy(symbolSmiley[25].str, "😘");	// \u1F618	FACE THROWING A KISS
	strcpy(symbolSmiley[26].str, "😙");	// \u1F619	KISSING FACE WITH SMILING EYES
	strcpy(symbolSmiley[27].str, "😚");	// \u1F61A	KISSING FACE WITH CLOSED EYES
	strcpy(symbolSmiley[28].str, "😛");	// \u1F61B	FACE WITH STUCK-OUT TONGUE
	strcpy(symbolSmiley[29].str, "😜");	// \u1F61C	FACE WITH STUCK-OUT TONGUE AND WINKING EYE
	strcpy(symbolSmiley[30].str, "😝");	// \u1F61D	FACE WITH STUCK-OUT TONGUE AND TIGHTLY-CLOSED EYES
	strcpy(symbolSmiley[31].str, "😞");	// \u1F61E	DISAPPOINTED FACE
	strcpy(symbolSmiley[32].str, "😟");	// \u1F61F	WORRIED FACE
	strcpy(symbolSmiley[33].str, "😠");	// \u1F620	ANGRY FACE
	strcpy(symbolSmiley[34].str, "😡");	// \u1F621	POUTING FACE
	strcpy(symbolSmiley[35].str, "😢");	// \u1F622	CRYING FACE
	strcpy(symbolSmiley[36].str, "😣");	// \u1F623	PERSEVERING FACE
	strcpy(symbolSmiley[37].str, "😤");	// \u1F624	FACE WITH LOOK OF TRIUMPH
	strcpy(symbolSmiley[38].str, "😥");	// \u1F625	DISAPPOINTED BUT RELIEVED FACE
	strcpy(symbolSmiley[39].str, "😦");	// \u1F626	FROWNING FACE WITH OPEN MOUTH
	strcpy(symbolSmiley[40].str, "😧");	// \u1F627	ANGUISHED FACE
	strcpy(symbolSmiley[41].str, "😨");	// \u1F628	FEARFUL FACE
	strcpy(symbolSmiley[42].str, "😩");	// \u1F629	WEARY FACE
	strcpy(symbolSmiley[43].str, "😪");	// \u1F62A	SLEEPY FACE
	strcpy(symbolSmiley[44].str, "😫");	// \u1F62B	TIRED FACE
	strcpy(symbolSmiley[45].str, "😬");	// \u1F62C	GRIMACING FACE
	strcpy(symbolSmiley[46].str, "😭");	// \u1F62D	LOUDLY CRYING FACE
	strcpy(symbolSmiley[47].str, "😮");	// \u1F62E	FACE WITH OPEN MOUTH
	strcpy(symbolSmiley[48].str, "😯");	// \u1F62F	HUSHED FACE
	strcpy(symbolSmiley[49].str, "😰");	// \u1F630	FACE WITH OPEN MOUTH AND COLD SWEAT
	strcpy(symbolSmiley[50].str, "😱");	// \u1F631	FACE SCREAMING IN FEAR
	strcpy(symbolSmiley[51].str, "😲");	// \u1F632	ASTONISHED FACE
	strcpy(symbolSmiley[52].str, "😳");	// \u1F633	FLUSHED FACE
	strcpy(symbolSmiley[53].str, "😴");	// \u1F634	SLEEPING FACE
	strcpy(symbolSmiley[54].str, "😵");	// \u1F635	DIZZY FACE
	strcpy(symbolSmiley[55].str, "😶");	// \u1F636	FACE WITHOUT MOUTH
	strcpy(symbolSmiley[56].str, "😷");	// \u1F637	FACE WITH MEDICAL MASK
	strcpy(symbolSmiley[57].str, "🙁");	// \u1F641	
	strcpy(symbolSmiley[58].str, "🙂");	// \u1F642	
	strcpy(symbolSmiley[59].str, "🙃");	// \u1F643	
	strcpy(symbolSmiley[60].str, "🙄");	// \u1F644	
	strcpy(symbolSmiley[61].str, "🤐");	// \u1F910	
	strcpy(symbolSmiley[62].str, "🤑");	// \u1F911	
	strcpy(symbolSmiley[63].str, "🤒");	// \u1F912	
	strcpy(symbolSmiley[64].str, "🤓");	// \u1F913	
	strcpy(symbolSmiley[65].str, "🤔");	// \u1F914	
	strcpy(symbolSmiley[66].str, "🤕");	// \u1F915	
	strcpy(symbolSmiley[67].str, "🤠");	// \u1F920	
	strcpy(symbolSmiley[68].str, "🤡");	// \u1F921	
	strcpy(symbolSmiley[69].str, "🤢");	// \u1F922	
	strcpy(symbolSmiley[70].str, "🤣");	// \u1F923	
	strcpy(symbolSmiley[71].str, "🤤");	// \u1F924	
	strcpy(symbolSmiley[72].str, "🤥");	// \u1F925	
	strcpy(symbolSmiley[73].str, "🤧");	// \u1F927	
	strcpy(symbolSmiley[74].str, "🤨");	// \u1F928	
	strcpy(symbolSmiley[75].str, "🤩");	// \u1F929	
	strcpy(symbolSmiley[76].str, "🤪");	// \u1F92A	
	strcpy(symbolSmiley[77].str, "🤫");	// \u1F92B	
	strcpy(symbolSmiley[78].str, "🤬");	// \u1F92C	
	strcpy(symbolSmiley[79].str, "🤭");	// \u1F92D	
	strcpy(symbolSmiley[80].str, "🤮");	// \u1F92E	
	strcpy(symbolSmiley[81].str, "🤯");	// \u1F92F	
	strcpy(symbolSmiley[82].str, "🧐");	// \u1F9D0	


/*
	All PowerLine and PowerLineExtra Symbols
	extracted from: https://www.nerdfonts.com/cheat-sheet
	Powerline by: https://github.com/ryanoasis/powerline-extra-symbols
	(Thanks a lot to both of you!)
*/
	// Power-Line
	strcpy(symbolPowerLine[1].str, "");		// \xE0A0
	strcpy(symbolPowerLine[2].str, "");		// \xE0A1
	strcpy(symbolPowerLine[3].str, "");		// \xE0A2
	strcpy(symbolPowerLine[4].str, "");		// \xE0B0
	strcpy(symbolPowerLine[5].str, "");		// \xE0B1
	strcpy(symbolPowerLine[6].str, "");		// \xE0B2
	strcpy(symbolPowerLine[7].str, "");		// \xE0B3

	// Power-Line-Extra
	strcpy(symbolPowerLineXtra[1].str, "");		// \xE0B9
	strcpy(symbolPowerLineXtra[2].str, "");		// \xE0BF
	strcpy(symbolPowerLineXtra[3].str, "");		// \xE0A3
	strcpy(symbolPowerLineXtra[4].str, "");		// \xE0C0
	strcpy(symbolPowerLineXtra[5].str, "");		// \xE0C2
	strcpy(symbolPowerLineXtra[6].str, "");		// \xE0C1
	strcpy(symbolPowerLineXtra[7].str, "");		// \xE0C3
	strcpy(symbolPowerLineXtra[8].str, "");		// \xE0BB
	strcpy(symbolPowerLineXtra[9].str, "");		// \xE0BD
	strcpy(symbolPowerLineXtra[10].str, "");	// \xE0CC
	strcpy(symbolPowerLineXtra[11].str, "");	// \xE0CD
	strcpy(symbolPowerLineXtra[12].str, "");	// \xE0C8
	strcpy(symbolPowerLineXtra[13].str, "");	// \xE0CA
	strcpy(symbolPowerLineXtra[14].str, "");	// \xE0B6
	strcpy(symbolPowerLineXtra[15].str, "");	// \xE0B7
	strcpy(symbolPowerLineXtra[16].str, "");	// \xE0D0
	strcpy(symbolPowerLineXtra[17].str, "");	// \xE0D1
	strcpy(symbolPowerLineXtra[18].str, "");	// \xE0CE
	strcpy(symbolPowerLineXtra[19].str, "");	// \xE0CF
	strcpy(symbolPowerLineXtra[20].str, "");	// \xE0B8
	strcpy(symbolPowerLineXtra[21].str, "");	// \xE0BA
	strcpy(symbolPowerLineXtra[22].str, "");	// \xE0C6
	strcpy(symbolPowerLineXtra[23].str, "");	// \xE0C7
	strcpy(symbolPowerLineXtra[24].str, "");	// \xE0C4
	strcpy(symbolPowerLineXtra[25].str, "");	// \xE0C5
	strcpy(symbolPowerLineXtra[26].str, "");	// \xE0B4
	strcpy(symbolPowerLineXtra[27].str, "");	// \xE0B5
	strcpy(symbolPowerLineXtra[28].str, "");	// \xE0D2
	strcpy(symbolPowerLineXtra[29].str, "");	// \xE0D4
	strcpy(symbolPowerLineXtra[30].str, "");	// \xE0BC
	strcpy(symbolPowerLineXtra[31].str, "");	// \xE0BE
}
	

struct UTFconst{
	union directions{
		// 1st Bit = single, 2nd = double, 3rd = bold
		u_int32_t north	:3;		// 1 / 2 / 4
		u_int32_t east 	:3;		// 8 / 16 / 32
		u_int32_t south :3;		// 64 / 128 / 256
		u_int32_t west 	:3;		// 512 / 1024 / 2048
	};	
};

char TuiDecBoxDraw[16][2] = {" ", " ", " ", "m", " ", "x",
							"l", "t", " ", "j", "q", "v",
							"k", "u", "w", "n" };

// []
