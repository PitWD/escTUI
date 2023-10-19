
//		   ANSI-ESC - Terminal Functions - See detailed description at the EOF !
//				! Using 3rd party MIT/GPL/"FreeSoftware" technologies !
//				  (see related declarations/functions and at the EOF)
// This is a  ! p_o_o_r !  Header by (c) Pit Demmer. (PIT-Licensed 01.04.2022 -           )

#define AnsiESCVersion "1.00pa"

// WIN & Linux
// #include "Terminal.h"
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include "ini.h"

typedef struct {
	int mode;			// 0 = B/W, 1 = 16, 2 = 255, 3 = RGB
	char *groupName;
	char *colorName;
	int groupID;
	int colorID;
	union ColorFG {
		uint32_t Color;	// !! 32 bit !!
		uint8_t dummy;	// ZERO - Then direct int32 -> Hex will work for RGB
		uint8_t R;
		uint8_t G;
		uint8_t B;
	}fg;						// ForeGround
	union ColorBG {
		uint32_t Color;
		uint8_t dummy;					
		uint8_t R;
		uint8_t G;
		uint8_t B;
	}bg;						// BackGround
	union ColorUL {
		uint32_t Color;	// UL - color just relevant in modes 1 & 2 (in mode '0': 'ul' == 'fg')
		uint8_t dummy;
		uint8_t R;					
		uint8_t G;
		uint8_t B;
	}ul;						// Underline
} EscColorSTRUCT;

typedef struct {
	/*		Actual State in [0]
			Default in [1]
			[2] - [n] free for user
	*/
	// 5 int
	char *fontName;
	char *styleName;
	int fontID;
	int styleID;
	uint32_t bold				:1;
	uint32_t faint				:1;
	uint32_t italic			:1;
	uint32_t blink				:1;
	uint32_t fast				:1;
	uint32_t reverse			:1;
	uint32_t invisible			:1;
	uint32_t strike			:1;

	uint32_t superscript		:1;
	uint32_t subscript			:1;
	uint32_t proportional		:1;
	uint32_t framed			:1;
	uint32_t encircled			:1;
	uint32_t overline			:1;
	uint32_t ideo_right		:1;		// CSI60m (Ideogram 'rarely supported')
	uint32_t ideo_dbl_right	:1;		// CSI61m ( "    "    "   "   "     "  )
	
	uint32_t ideo_left			:1;		// CSI62m ( "    "    "   "   "     "  )
	uint32_t ideo_dbl_left		:1;		// CSI63m ( "    "    "   "   "     "  )
	uint32_t ideo_stress		:1;		// CSI64m ( "    "    "   "   "     "  )
	uint32_t dbl_width			:1;
	uint32_t dbl_height		:1;
	uint32_t dummy1			:1;		// Fill the 3rd int
	uint32_t dummy2			:1;		// ( "   "   "   " ) 
	uint32_t dummy3			:1;		// ( "   "   "   " )
	
	uint32_t font				:4;		// CSI10m (Standard = 0) - CSI20m (Fraktur = 10) 
	uint32_t underline			:4;		// 0 = None, 1 = single, 2 = double, 3 = curl, 4 = dot, 5 = dash 
											// 6 = dashdot, 7 = dbl_curl, 8 = dbl_dot, 9 = dbl_dash
	// EscColorSTRUCT *pColor;
} EscStyleSTRUCT; 

struct CanvasSTRUCT {
    EscColorSTRUCT *color;
    EscStyleSTRUCT *style;
    wchar_t c;
    int width;
}***myCanvas;
int canvasMaxX = 0;
int canvasMaxY = 0;
#define canvasMaxZ 4

// *** 3rd Party START ***
struct width_interval {
	// from https://github.com/PitWD/termux_wcwidth
	// which is a copy of https://github.com/termux/wcwidth
	// all MIT licensed
	int start;
	int end;
};

static struct width_interval ZERO_WIDTH[] = {
	// from https://github.com/PitWD/termux_wcwidth
	// which is a copy of https://github.com/termux/wcwidth
	// all MIT licensed
	{0x00300, 0x0036f},  // Combining Grave Accent  ..Combining Latin Small Le
	{0x00483, 0x00489},  // Combining Cyrillic Titlo..Combining Cyrillic Milli
	{0x00591, 0x005bd},  // Hebrew Accent Etnahta   ..Hebrew Point Meteg
	{0x005bf, 0x005bf},  // Hebrew Point Rafe       ..Hebrew Point Rafe
	{0x005c1, 0x005c2},  // Hebrew Point Shin Dot   ..Hebrew Point Sin Dot
	{0x005c4, 0x005c5},  // Hebrew Mark Upper Dot   ..Hebrew Mark Lower Dot
	{0x005c7, 0x005c7},  // Hebrew Point Qamats Qata..Hebrew Point Qamats Qata
	{0x00610, 0x0061a},  // Arabic Sign Sallallahou ..Arabic Small Kasra
	{0x0064b, 0x0065f},  // Arabic Fathatan         ..Arabic Wavy Hamza Below
	{0x00670, 0x00670},  // Arabic Letter Superscrip..Arabic Letter Superscrip
	{0x006d6, 0x006dc},  // Arabic Small High Ligatu..Arabic Small High Seen
	{0x006df, 0x006e4},  // Arabic Small High Rounde..Arabic Small High Madda
	{0x006e7, 0x006e8},  // Arabic Small High Yeh   ..Arabic Small High Noon
	{0x006ea, 0x006ed},  // Arabic Empty Centre Low ..Arabic Small Low Meem
	{0x00711, 0x00711},  // Syriac Letter Superscrip..Syriac Letter Superscrip
	{0x00730, 0x0074a},  // Syriac Pthaha Above     ..Syriac Barrekh
	{0x007a6, 0x007b0},  // Thaana Abafili          ..Thaana Sukun
	{0x007eb, 0x007f3},  // Nko Combining Short High..Nko Combining Double Dot
	{0x007fd, 0x007fd},  // Nko Dantayalan          ..Nko Dantayalan
	{0x00816, 0x00819},  // Samaritan Mark In       ..Samaritan Mark Dagesh
	{0x0081b, 0x00823},  // Samaritan Mark Epentheti..Samaritan Vowel Sign A
	{0x00825, 0x00827},  // Samaritan Vowel Sign Sho..Samaritan Vowel Sign U
	{0x00829, 0x0082d},  // Samaritan Vowel Sign Lon..Samaritan Mark Nequdaa
	{0x00859, 0x0085b},  // Mandaic Affrication Mark..Mandaic Gemination Mark
	{0x00898, 0x0089f},  // Arabic Small High Word A..Arabic Half Madda Over M
	{0x008ca, 0x008e1},  // Arabic Small High Farsi ..Arabic Small High Sign S
	{0x008e3, 0x00902},  // Arabic Turned Damma Belo..Devanagari Sign Anusvara
	{0x0093a, 0x0093a},  // Devanagari Vowel Sign Oe..Devanagari Vowel Sign Oe
	{0x0093c, 0x0093c},  // Devanagari Sign Nukta   ..Devanagari Sign Nukta
	{0x00941, 0x00948},  // Devanagari Vowel Sign U ..Devanagari Vowel Sign Ai
	{0x0094d, 0x0094d},  // Devanagari Sign Virama  ..Devanagari Sign Virama
	{0x00951, 0x00957},  // Devanagari Stress Sign U..Devanagari Vowel Sign Uu
	{0x00962, 0x00963},  // Devanagari Vowel Sign Vo..Devanagari Vowel Sign Vo
	{0x00981, 0x00981},  // Bengali Sign Candrabindu..Bengali Sign Candrabindu
	{0x009bc, 0x009bc},  // Bengali Sign Nukta      ..Bengali Sign Nukta
	{0x009c1, 0x009c4},  // Bengali Vowel Sign U    ..Bengali Vowel Sign Vocal
	{0x009cd, 0x009cd},  // Bengali Sign Virama     ..Bengali Sign Virama
	{0x009e2, 0x009e3},  // Bengali Vowel Sign Vocal..Bengali Vowel Sign Vocal
	{0x009fe, 0x009fe},  // Bengali Sandhi Mark     ..Bengali Sandhi Mark
	{0x00a01, 0x00a02},  // Gurmukhi Sign Adak Bindi..Gurmukhi Sign Bindi
	{0x00a3c, 0x00a3c},  // Gurmukhi Sign Nukta     ..Gurmukhi Sign Nukta
	{0x00a41, 0x00a42},  // Gurmukhi Vowel Sign U   ..Gurmukhi Vowel Sign Uu
	{0x00a47, 0x00a48},  // Gurmukhi Vowel Sign Ee  ..Gurmukhi Vowel Sign Ai
	{0x00a4b, 0x00a4d},  // Gurmukhi Vowel Sign Oo  ..Gurmukhi Sign Virama
	{0x00a51, 0x00a51},  // Gurmukhi Sign Udaat     ..Gurmukhi Sign Udaat
	{0x00a70, 0x00a71},  // Gurmukhi Tippi          ..Gurmukhi Addak
	{0x00a75, 0x00a75},  // Gurmukhi Sign Yakash    ..Gurmukhi Sign Yakash
	{0x00a81, 0x00a82},  // Gujarati Sign Candrabind..Gujarati Sign Anusvara
	{0x00abc, 0x00abc},  // Gujarati Sign Nukta     ..Gujarati Sign Nukta
	{0x00ac1, 0x00ac5},  // Gujarati Vowel Sign U   ..Gujarati Vowel Sign Cand
	{0x00ac7, 0x00ac8},  // Gujarati Vowel Sign E   ..Gujarati Vowel Sign Ai
	{0x00acd, 0x00acd},  // Gujarati Sign Virama    ..Gujarati Sign Virama
	{0x00ae2, 0x00ae3},  // Gujarati Vowel Sign Voca..Gujarati Vowel Sign Voca
	{0x00afa, 0x00aff},  // Gujarati Sign Sukun     ..Gujarati Sign Two-circle
	{0x00b01, 0x00b01},  // Oriya Sign Candrabindu  ..Oriya Sign Candrabindu
	{0x00b3c, 0x00b3c},  // Oriya Sign Nukta        ..Oriya Sign Nukta
	{0x00b3f, 0x00b3f},  // Oriya Vowel Sign I      ..Oriya Vowel Sign I
	{0x00b41, 0x00b44},  // Oriya Vowel Sign U      ..Oriya Vowel Sign Vocalic
	{0x00b4d, 0x00b4d},  // Oriya Sign Virama       ..Oriya Sign Virama
	{0x00b55, 0x00b56},  // Oriya Sign Overline     ..Oriya Ai Length Mark
	{0x00b62, 0x00b63},  // Oriya Vowel Sign Vocalic..Oriya Vowel Sign Vocalic
	{0x00b82, 0x00b82},  // Tamil Sign Anusvara     ..Tamil Sign Anusvara
	{0x00bc0, 0x00bc0},  // Tamil Vowel Sign Ii     ..Tamil Vowel Sign Ii
	{0x00bcd, 0x00bcd},  // Tamil Sign Virama       ..Tamil Sign Virama
	{0x00c00, 0x00c00},  // Telugu Sign Combining Ca..Telugu Sign Combining Ca
	{0x00c04, 0x00c04},  // Telugu Sign Combining An..Telugu Sign Combining An
	{0x00c3c, 0x00c3c},  // Telugu Sign Nukta       ..Telugu Sign Nukta
	{0x00c3e, 0x00c40},  // Telugu Vowel Sign Aa    ..Telugu Vowel Sign Ii
	{0x00c46, 0x00c48},  // Telugu Vowel Sign E     ..Telugu Vowel Sign Ai
	{0x00c4a, 0x00c4d},  // Telugu Vowel Sign O     ..Telugu Sign Virama
	{0x00c55, 0x00c56},  // Telugu Length Mark      ..Telugu Ai Length Mark
	{0x00c62, 0x00c63},  // Telugu Vowel Sign Vocali..Telugu Vowel Sign Vocali
	{0x00c81, 0x00c81},  // Kannada Sign Candrabindu..Kannada Sign Candrabindu
	{0x00cbc, 0x00cbc},  // Kannada Sign Nukta      ..Kannada Sign Nukta
	{0x00cbf, 0x00cbf},  // Kannada Vowel Sign I    ..Kannada Vowel Sign I
	{0x00cc6, 0x00cc6},  // Kannada Vowel Sign E    ..Kannada Vowel Sign E
	{0x00ccc, 0x00ccd},  // Kannada Vowel Sign Au   ..Kannada Sign Virama
	{0x00ce2, 0x00ce3},  // Kannada Vowel Sign Vocal..Kannada Vowel Sign Vocal
	{0x00d00, 0x00d01},  // Malayalam Sign Combining..Malayalam Sign Candrabin
	{0x00d3b, 0x00d3c},  // Malayalam Sign Vertical ..Malayalam Sign Circular
	{0x00d41, 0x00d44},  // Malayalam Vowel Sign U  ..Malayalam Vowel Sign Voc
	{0x00d4d, 0x00d4d},  // Malayalam Sign Virama   ..Malayalam Sign Virama
	{0x00d62, 0x00d63},  // Malayalam Vowel Sign Voc..Malayalam Vowel Sign Voc
	{0x00d81, 0x00d81},  // Sinhala Sign Candrabindu..Sinhala Sign Candrabindu
	{0x00dca, 0x00dca},  // Sinhala Sign Al-lakuna  ..Sinhala Sign Al-lakuna
	{0x00dd2, 0x00dd4},  // Sinhala Vowel Sign Ketti..Sinhala Vowel Sign Ketti
	{0x00dd6, 0x00dd6},  // Sinhala Vowel Sign Diga ..Sinhala Vowel Sign Diga
	{0x00e31, 0x00e31},  // Thai Character Mai Han-a..Thai Character Mai Han-a
	{0x00e34, 0x00e3a},  // Thai Character Sara I   ..Thai Character Phinthu
	{0x00e47, 0x00e4e},  // Thai Character Maitaikhu..Thai Character Yamakkan
	{0x00eb1, 0x00eb1},  // Lao Vowel Sign Mai Kan  ..Lao Vowel Sign Mai Kan
	{0x00eb4, 0x00ebc},  // Lao Vowel Sign I        ..Lao Semivowel Sign Lo
	{0x00ec8, 0x00ece},  // Lao Tone Mai Ek         ..(nil)
	{0x00f18, 0x00f19},  // Tibetan Astrological Sig..Tibetan Astrological Sig
	{0x00f35, 0x00f35},  // Tibetan Mark Ngas Bzung ..Tibetan Mark Ngas Bzung
	{0x00f37, 0x00f37},  // Tibetan Mark Ngas Bzung ..Tibetan Mark Ngas Bzung
	{0x00f39, 0x00f39},  // Tibetan Mark Tsa -phru  ..Tibetan Mark Tsa -phru
	{0x00f71, 0x00f7e},  // Tibetan Vowel Sign Aa   ..Tibetan Sign Rjes Su Nga
	{0x00f80, 0x00f84},  // Tibetan Vowel Sign Rever..Tibetan Mark Halanta
	{0x00f86, 0x00f87},  // Tibetan Sign Lci Rtags  ..Tibetan Sign Yang Rtags
	{0x00f8d, 0x00f97},  // Tibetan Subjoined Sign L..Tibetan Subjoined Letter
	{0x00f99, 0x00fbc},  // Tibetan Subjoined Letter..Tibetan Subjoined Letter
	{0x00fc6, 0x00fc6},  // Tibetan Symbol Padma Gda..Tibetan Symbol Padma Gda
	{0x0102d, 0x01030},  // Myanmar Vowel Sign I    ..Myanmar Vowel Sign Uu
	{0x01032, 0x01037},  // Myanmar Vowel Sign Ai   ..Myanmar Sign Dot Below
	{0x01039, 0x0103a},  // Myanmar Sign Virama     ..Myanmar Sign Asat
	{0x0103d, 0x0103e},  // Myanmar Consonant Sign M..Myanmar Consonant Sign M
	{0x01058, 0x01059},  // Myanmar Vowel Sign Vocal..Myanmar Vowel Sign Vocal
	{0x0105e, 0x01060},  // Myanmar Consonant Sign M..Myanmar Consonant Sign M
	{0x01071, 0x01074},  // Myanmar Vowel Sign Geba ..Myanmar Vowel Sign Kayah
	{0x01082, 0x01082},  // Myanmar Consonant Sign S..Myanmar Consonant Sign S
	{0x01085, 0x01086},  // Myanmar Vowel Sign Shan ..Myanmar Vowel Sign Shan
	{0x0108d, 0x0108d},  // Myanmar Sign Shan Counci..Myanmar Sign Shan Counci
	{0x0109d, 0x0109d},  // Myanmar Vowel Sign Aiton..Myanmar Vowel Sign Aiton
	{0x0135d, 0x0135f},  // Ethiopic Combining Gemin..Ethiopic Combining Gemin
	{0x01712, 0x01714},  // Tagalog Vowel Sign I    ..Tagalog Sign Virama
	{0x01732, 0x01733},  // Hanunoo Vowel Sign I    ..Hanunoo Vowel Sign U
	{0x01752, 0x01753},  // Buhid Vowel Sign I      ..Buhid Vowel Sign U
	{0x01772, 0x01773},  // Tagbanwa Vowel Sign I   ..Tagbanwa Vowel Sign U
	{0x017b4, 0x017b5},  // Khmer Vowel Inherent Aq ..Khmer Vowel Inherent Aa
	{0x017b7, 0x017bd},  // Khmer Vowel Sign I      ..Khmer Vowel Sign Ua
	{0x017c6, 0x017c6},  // Khmer Sign Nikahit      ..Khmer Sign Nikahit
	{0x017c9, 0x017d3},  // Khmer Sign Muusikatoan  ..Khmer Sign Bathamasat
	{0x017dd, 0x017dd},  // Khmer Sign Atthacan     ..Khmer Sign Atthacan
	{0x0180b, 0x0180d},  // Mongolian Free Variation..Mongolian Free Variation
	{0x0180f, 0x0180f},  // Mongolian Free Variation..Mongolian Free Variation
	{0x01885, 0x01886},  // Mongolian Letter Ali Gal..Mongolian Letter Ali Gal
	{0x018a9, 0x018a9},  // Mongolian Letter Ali Gal..Mongolian Letter Ali Gal
	{0x01920, 0x01922},  // Limbu Vowel Sign A      ..Limbu Vowel Sign U
	{0x01927, 0x01928},  // Limbu Vowel Sign E      ..Limbu Vowel Sign O
	{0x01932, 0x01932},  // Limbu Small Letter Anusv..Limbu Small Letter Anusv
	{0x01939, 0x0193b},  // Limbu Sign Mukphreng    ..Limbu Sign Sa-i
	{0x01a17, 0x01a18},  // Buginese Vowel Sign I   ..Buginese Vowel Sign U
	{0x01a1b, 0x01a1b},  // Buginese Vowel Sign Ae  ..Buginese Vowel Sign Ae
	{0x01a56, 0x01a56},  // Tai Tham Consonant Sign ..Tai Tham Consonant Sign
	{0x01a58, 0x01a5e},  // Tai Tham Sign Mai Kang L..Tai Tham Consonant Sign
	{0x01a60, 0x01a60},  // Tai Tham Sign Sakot     ..Tai Tham Sign Sakot
	{0x01a62, 0x01a62},  // Tai Tham Vowel Sign Mai ..Tai Tham Vowel Sign Mai
	{0x01a65, 0x01a6c},  // Tai Tham Vowel Sign I   ..Tai Tham Vowel Sign Oa B
	{0x01a73, 0x01a7c},  // Tai Tham Vowel Sign Oa A..Tai Tham Sign Khuen-lue
	{0x01a7f, 0x01a7f},  // Tai Tham Combining Crypt..Tai Tham Combining Crypt
	{0x01ab0, 0x01ace},  // Combining Doubled Circum..Combining Latin Small Le
	{0x01b00, 0x01b03},  // Balinese Sign Ulu Ricem ..Balinese Sign Surang
	{0x01b34, 0x01b34},  // Balinese Sign Rerekan   ..Balinese Sign Rerekan
	{0x01b36, 0x01b3a},  // Balinese Vowel Sign Ulu ..Balinese Vowel Sign Ra R
	{0x01b3c, 0x01b3c},  // Balinese Vowel Sign La L..Balinese Vowel Sign La L
	{0x01b42, 0x01b42},  // Balinese Vowel Sign Pepe..Balinese Vowel Sign Pepe
	{0x01b6b, 0x01b73},  // Balinese Musical Symbol ..Balinese Musical Symbol
	{0x01b80, 0x01b81},  // Sundanese Sign Panyecek ..Sundanese Sign Panglayar
	{0x01ba2, 0x01ba5},  // Sundanese Consonant Sign..Sundanese Vowel Sign Pan
	{0x01ba8, 0x01ba9},  // Sundanese Vowel Sign Pam..Sundanese Vowel Sign Pan
	{0x01bab, 0x01bad},  // Sundanese Sign Virama   ..Sundanese Consonant Sign
	{0x01be6, 0x01be6},  // Batak Sign Tompi        ..Batak Sign Tompi
	{0x01be8, 0x01be9},  // Batak Vowel Sign Pakpak ..Batak Vowel Sign Ee
	{0x01bed, 0x01bed},  // Batak Vowel Sign Karo O ..Batak Vowel Sign Karo O
	{0x01bef, 0x01bf1},  // Batak Vowel Sign U For S..Batak Consonant Sign H
	{0x01c2c, 0x01c33},  // Lepcha Vowel Sign E     ..Lepcha Consonant Sign T
	{0x01c36, 0x01c37},  // Lepcha Sign Ran         ..Lepcha Sign Nukta
	{0x01cd0, 0x01cd2},  // Vedic Tone Karshana     ..Vedic Tone Prenkha
	{0x01cd4, 0x01ce0},  // Vedic Sign Yajurvedic Mi..Vedic Tone Rigvedic Kash
	{0x01ce2, 0x01ce8},  // Vedic Sign Visarga Svari..Vedic Sign Visarga Anuda
	{0x01ced, 0x01ced},  // Vedic Sign Tiryak       ..Vedic Sign Tiryak
	{0x01cf4, 0x01cf4},  // Vedic Tone Candra Above ..Vedic Tone Candra Above
	{0x01cf8, 0x01cf9},  // Vedic Tone Ring Above   ..Vedic Tone Double Ring A
	{0x01dc0, 0x01dff},  // Combining Dotted Grave A..Combining Right Arrowhea
	{0x020d0, 0x020f0},  // Combining Left Harpoon A..Combining Asterisk Above
	{0x02cef, 0x02cf1},  // Coptic Combining Ni Abov..Coptic Combining Spiritu
	{0x02d7f, 0x02d7f},  // Tifinagh Consonant Joine..Tifinagh Consonant Joine
	{0x02de0, 0x02dff},  // Combining Cyrillic Lette..Combining Cyrillic Lette
	{0x0302a, 0x0302d},  // Ideographic Level Tone M..Ideographic Entering Ton
	{0x03099, 0x0309a},  // Combining Katakana-hirag..Combining Katakana-hirag
	{0x0a66f, 0x0a672},  // Combining Cyrillic Vzmet..Combining Cyrillic Thous
	{0x0a674, 0x0a67d},  // Combining Cyrillic Lette..Combining Cyrillic Payer
	{0x0a69e, 0x0a69f},  // Combining Cyrillic Lette..Combining Cyrillic Lette
	{0x0a6f0, 0x0a6f1},  // Bamum Combining Mark Koq..Bamum Combining Mark Tuk
	{0x0a802, 0x0a802},  // Syloti Nagri Sign Dvisva..Syloti Nagri Sign Dvisva
	{0x0a806, 0x0a806},  // Syloti Nagri Sign Hasant..Syloti Nagri Sign Hasant
	{0x0a80b, 0x0a80b},  // Syloti Nagri Sign Anusva..Syloti Nagri Sign Anusva
	{0x0a825, 0x0a826},  // Syloti Nagri Vowel Sign ..Syloti Nagri Vowel Sign
	{0x0a82c, 0x0a82c},  // Syloti Nagri Sign Altern..Syloti Nagri Sign Altern
	{0x0a8c4, 0x0a8c5},  // Saurashtra Sign Virama  ..Saurashtra Sign Candrabi
	{0x0a8e0, 0x0a8f1},  // Combining Devanagari Dig..Combining Devanagari Sig
	{0x0a8ff, 0x0a8ff},  // Devanagari Vowel Sign Ay..Devanagari Vowel Sign Ay
	{0x0a926, 0x0a92d},  // Kayah Li Vowel Ue       ..Kayah Li Tone Calya Plop
	{0x0a947, 0x0a951},  // Rejang Vowel Sign I     ..Rejang Consonant Sign R
	{0x0a980, 0x0a982},  // Javanese Sign Panyangga ..Javanese Sign Layar
	{0x0a9b3, 0x0a9b3},  // Javanese Sign Cecak Telu..Javanese Sign Cecak Telu
	{0x0a9b6, 0x0a9b9},  // Javanese Vowel Sign Wulu..Javanese Vowel Sign Suku
	{0x0a9bc, 0x0a9bd},  // Javanese Vowel Sign Pepe..Javanese Consonant Sign
	{0x0a9e5, 0x0a9e5},  // Myanmar Sign Shan Saw   ..Myanmar Sign Shan Saw
	{0x0aa29, 0x0aa2e},  // Cham Vowel Sign Aa      ..Cham Vowel Sign Oe
	{0x0aa31, 0x0aa32},  // Cham Vowel Sign Au      ..Cham Vowel Sign Ue
	{0x0aa35, 0x0aa36},  // Cham Consonant Sign La  ..Cham Consonant Sign Wa
	{0x0aa43, 0x0aa43},  // Cham Consonant Sign Fina..Cham Consonant Sign Fina
	{0x0aa4c, 0x0aa4c},  // Cham Consonant Sign Fina..Cham Consonant Sign Fina
	{0x0aa7c, 0x0aa7c},  // Myanmar Sign Tai Laing T..Myanmar Sign Tai Laing T
	{0x0aab0, 0x0aab0},  // Tai Viet Mai Kang       ..Tai Viet Mai Kang
	{0x0aab2, 0x0aab4},  // Tai Viet Vowel I        ..Tai Viet Vowel U
	{0x0aab7, 0x0aab8},  // Tai Viet Mai Khit       ..Tai Viet Vowel Ia
	{0x0aabe, 0x0aabf},  // Tai Viet Vowel Am       ..Tai Viet Tone Mai Ek
	{0x0aac1, 0x0aac1},  // Tai Viet Tone Mai Tho   ..Tai Viet Tone Mai Tho
	{0x0aaec, 0x0aaed},  // Meetei Mayek Vowel Sign ..Meetei Mayek Vowel Sign
	{0x0aaf6, 0x0aaf6},  // Meetei Mayek Virama     ..Meetei Mayek Virama
	{0x0abe5, 0x0abe5},  // Meetei Mayek Vowel Sign ..Meetei Mayek Vowel Sign
	{0x0abe8, 0x0abe8},  // Meetei Mayek Vowel Sign ..Meetei Mayek Vowel Sign
	{0x0abed, 0x0abed},  // Meetei Mayek Apun Iyek  ..Meetei Mayek Apun Iyek
	{0x0fb1e, 0x0fb1e},  // Hebrew Point Judeo-spani..Hebrew Point Judeo-spani
	{0x0fe00, 0x0fe0f},  // Variation Selector-1    ..Variation Selector-16
	{0x0fe20, 0x0fe2f},  // Combining Ligature Left ..Combining Cyrillic Titlo
	{0x101fd, 0x101fd},  // Phaistos Disc Sign Combi..Phaistos Disc Sign Combi
	{0x102e0, 0x102e0},  // Coptic Epact Thousands M..Coptic Epact Thousands M
	{0x10376, 0x1037a},  // Combining Old Permic Let..Combining Old Permic Let
	{0x10a01, 0x10a03},  // Kharoshthi Vowel Sign I ..Kharoshthi Vowel Sign Vo
	{0x10a05, 0x10a06},  // Kharoshthi Vowel Sign E ..Kharoshthi Vowel Sign O
	{0x10a0c, 0x10a0f},  // Kharoshthi Vowel Length ..Kharoshthi Sign Visarga
	{0x10a38, 0x10a3a},  // Kharoshthi Sign Bar Abov..Kharoshthi Sign Dot Belo
	{0x10a3f, 0x10a3f},  // Kharoshthi Virama       ..Kharoshthi Virama
	{0x10ae5, 0x10ae6},  // Manichaean Abbreviation ..Manichaean Abbreviation
	{0x10d24, 0x10d27},  // Hanifi Rohingya Sign Har..Hanifi Rohingya Sign Tas
	{0x10eab, 0x10eac},  // Yezidi Combining Hamza M..Yezidi Combining Madda M
	{0x10efd, 0x10eff},  // (nil)                   ..(nil)
	{0x10f46, 0x10f50},  // Sogdian Combining Dot Be..Sogdian Combining Stroke
	{0x10f82, 0x10f85},  // Old Uyghur Combining Dot..Old Uyghur Combining Two
	{0x11001, 0x11001},  // Brahmi Sign Anusvara    ..Brahmi Sign Anusvara
	{0x11038, 0x11046},  // Brahmi Vowel Sign Aa    ..Brahmi Virama
	{0x11070, 0x11070},  // Brahmi Sign Old Tamil Vi..Brahmi Sign Old Tamil Vi
	{0x11073, 0x11074},  // Brahmi Vowel Sign Old Ta..Brahmi Vowel Sign Old Ta
	{0x1107f, 0x11081},  // Brahmi Number Joiner    ..Kaithi Sign Anusvara
	{0x110b3, 0x110b6},  // Kaithi Vowel Sign U     ..Kaithi Vowel Sign Ai
	{0x110b9, 0x110ba},  // Kaithi Sign Virama      ..Kaithi Sign Nukta
	{0x110c2, 0x110c2},  // Kaithi Vowel Sign Vocali..Kaithi Vowel Sign Vocali
	{0x11100, 0x11102},  // Chakma Sign Candrabindu ..Chakma Sign Visarga
	{0x11127, 0x1112b},  // Chakma Vowel Sign A     ..Chakma Vowel Sign Uu
	{0x1112d, 0x11134},  // Chakma Vowel Sign Ai    ..Chakma Maayyaa
	{0x11173, 0x11173},  // Mahajani Sign Nukta     ..Mahajani Sign Nukta
	{0x11180, 0x11181},  // Sharada Sign Candrabindu..Sharada Sign Anusvara
	{0x111b6, 0x111be},  // Sharada Vowel Sign U    ..Sharada Vowel Sign O
	{0x111c9, 0x111cc},  // Sharada Sandhi Mark     ..Sharada Extra Short Vowe
	{0x111cf, 0x111cf},  // Sharada Sign Inverted Ca..Sharada Sign Inverted Ca
	{0x1122f, 0x11231},  // Khojki Vowel Sign U     ..Khojki Vowel Sign Ai
	{0x11234, 0x11234},  // Khojki Sign Anusvara    ..Khojki Sign Anusvara
	{0x11236, 0x11237},  // Khojki Sign Nukta       ..Khojki Sign Shadda
	{0x1123e, 0x1123e},  // Khojki Sign Sukun       ..Khojki Sign Sukun
	{0x11241, 0x11241},  // (nil)                   ..(nil)
	{0x112df, 0x112df},  // Khudawadi Sign Anusvara ..Khudawadi Sign Anusvara
	{0x112e3, 0x112ea},  // Khudawadi Vowel Sign U  ..Khudawadi Sign Virama
	{0x11300, 0x11301},  // Grantha Sign Combining A..Grantha Sign Candrabindu
	{0x1133b, 0x1133c},  // Combining Bindu Below   ..Grantha Sign Nukta
	{0x11340, 0x11340},  // Grantha Vowel Sign Ii   ..Grantha Vowel Sign Ii
	{0x11366, 0x1136c},  // Combining Grantha Digit ..Combining Grantha Digit
	{0x11370, 0x11374},  // Combining Grantha Letter..Combining Grantha Letter
	{0x11438, 0x1143f},  // Newa Vowel Sign U       ..Newa Vowel Sign Ai
	{0x11442, 0x11444},  // Newa Sign Virama        ..Newa Sign Anusvara
	{0x11446, 0x11446},  // Newa Sign Nukta         ..Newa Sign Nukta
	{0x1145e, 0x1145e},  // Newa Sandhi Mark        ..Newa Sandhi Mark
	{0x114b3, 0x114b8},  // Tirhuta Vowel Sign U    ..Tirhuta Vowel Sign Vocal
	{0x114ba, 0x114ba},  // Tirhuta Vowel Sign Short..Tirhuta Vowel Sign Short
	{0x114bf, 0x114c0},  // Tirhuta Sign Candrabindu..Tirhuta Sign Anusvara
	{0x114c2, 0x114c3},  // Tirhuta Sign Virama     ..Tirhuta Sign Nukta
	{0x115b2, 0x115b5},  // Siddham Vowel Sign U    ..Siddham Vowel Sign Vocal
	{0x115bc, 0x115bd},  // Siddham Sign Candrabindu..Siddham Sign Anusvara
	{0x115bf, 0x115c0},  // Siddham Sign Virama     ..Siddham Sign Nukta
	{0x115dc, 0x115dd},  // Siddham Vowel Sign Alter..Siddham Vowel Sign Alter
	{0x11633, 0x1163a},  // Modi Vowel Sign U       ..Modi Vowel Sign Ai
	{0x1163d, 0x1163d},  // Modi Sign Anusvara      ..Modi Sign Anusvara
	{0x1163f, 0x11640},  // Modi Sign Virama        ..Modi Sign Ardhacandra
	{0x116ab, 0x116ab},  // Takri Sign Anusvara     ..Takri Sign Anusvara
	{0x116ad, 0x116ad},  // Takri Vowel Sign Aa     ..Takri Vowel Sign Aa
	{0x116b0, 0x116b5},  // Takri Vowel Sign U      ..Takri Vowel Sign Au
	{0x116b7, 0x116b7},  // Takri Sign Nukta        ..Takri Sign Nukta
	{0x1171d, 0x1171f},  // Ahom Consonant Sign Medi..Ahom Consonant Sign Medi
	{0x11722, 0x11725},  // Ahom Vowel Sign I       ..Ahom Vowel Sign Uu
	{0x11727, 0x1172b},  // Ahom Vowel Sign Aw      ..Ahom Sign Killer
	{0x1182f, 0x11837},  // Dogra Vowel Sign U      ..Dogra Sign Anusvara
	{0x11839, 0x1183a},  // Dogra Sign Virama       ..Dogra Sign Nukta
	{0x1193b, 0x1193c},  // Dives Akuru Sign Anusvar..Dives Akuru Sign Candrab
	{0x1193e, 0x1193e},  // Dives Akuru Virama      ..Dives Akuru Virama
	{0x11943, 0x11943},  // Dives Akuru Sign Nukta  ..Dives Akuru Sign Nukta
	{0x119d4, 0x119d7},  // Nandinagari Vowel Sign U..Nandinagari Vowel Sign V
	{0x119da, 0x119db},  // Nandinagari Vowel Sign E..Nandinagari Vowel Sign A
	{0x119e0, 0x119e0},  // Nandinagari Sign Virama ..Nandinagari Sign Virama
	{0x11a01, 0x11a0a},  // Zanabazar Square Vowel S..Zanabazar Square Vowel L
	{0x11a33, 0x11a38},  // Zanabazar Square Final C..Zanabazar Square Sign An
	{0x11a3b, 0x11a3e},  // Zanabazar Square Cluster..Zanabazar Square Cluster
	{0x11a47, 0x11a47},  // Zanabazar Square Subjoin..Zanabazar Square Subjoin
	{0x11a51, 0x11a56},  // Soyombo Vowel Sign I    ..Soyombo Vowel Sign Oe
	{0x11a59, 0x11a5b},  // Soyombo Vowel Sign Vocal..Soyombo Vowel Length Mar
	{0x11a8a, 0x11a96},  // Soyombo Final Consonant ..Soyombo Sign Anusvara
	{0x11a98, 0x11a99},  // Soyombo Gemination Mark ..Soyombo Subjoiner
	{0x11c30, 0x11c36},  // Bhaiksuki Vowel Sign I  ..Bhaiksuki Vowel Sign Voc
	{0x11c38, 0x11c3d},  // Bhaiksuki Vowel Sign E  ..Bhaiksuki Sign Anusvara
	{0x11c3f, 0x11c3f},  // Bhaiksuki Sign Virama   ..Bhaiksuki Sign Virama
	{0x11c92, 0x11ca7},  // Marchen Subjoined Letter..Marchen Subjoined Letter
	{0x11caa, 0x11cb0},  // Marchen Subjoined Letter..Marchen Vowel Sign Aa
	{0x11cb2, 0x11cb3},  // Marchen Vowel Sign U    ..Marchen Vowel Sign E
	{0x11cb5, 0x11cb6},  // Marchen Sign Anusvara   ..Marchen Sign Candrabindu
	{0x11d31, 0x11d36},  // Masaram Gondi Vowel Sign..Masaram Gondi Vowel Sign
	{0x11d3a, 0x11d3a},  // Masaram Gondi Vowel Sign..Masaram Gondi Vowel Sign
	{0x11d3c, 0x11d3d},  // Masaram Gondi Vowel Sign..Masaram Gondi Vowel Sign
	{0x11d3f, 0x11d45},  // Masaram Gondi Vowel Sign..Masaram Gondi Virama
	{0x11d47, 0x11d47},  // Masaram Gondi Ra-kara   ..Masaram Gondi Ra-kara
	{0x11d90, 0x11d91},  // Gunjala Gondi Vowel Sign..Gunjala Gondi Vowel Sign
	{0x11d95, 0x11d95},  // Gunjala Gondi Sign Anusv..Gunjala Gondi Sign Anusv
	{0x11d97, 0x11d97},  // Gunjala Gondi Virama    ..Gunjala Gondi Virama
	{0x11ef3, 0x11ef4},  // Makasar Vowel Sign I    ..Makasar Vowel Sign U
	{0x11f00, 0x11f01},  // (nil)                   ..(nil)
	{0x11f36, 0x11f3a},  // (nil)                   ..(nil)
	{0x11f40, 0x11f40},  // (nil)                   ..(nil)
	{0x11f42, 0x11f42},  // (nil)                   ..(nil)
	{0x13440, 0x13440},  // (nil)                   ..(nil)
	{0x13447, 0x13455},  // (nil)                   ..(nil)
	{0x16af0, 0x16af4},  // Bassa Vah Combining High..Bassa Vah Combining High
	{0x16b30, 0x16b36},  // Pahawh Hmong Mark Cim Tu..Pahawh Hmong Mark Cim Ta
	{0x16f4f, 0x16f4f},  // Miao Sign Consonant Modi..Miao Sign Consonant Modi
	{0x16f8f, 0x16f92},  // Miao Tone Right         ..Miao Tone Below
	{0x16fe4, 0x16fe4},  // Khitan Small Script Fill..Khitan Small Script Fill
	{0x1bc9d, 0x1bc9e},  // Duployan Thick Letter Se..Duployan Double Mark
	{0x1cf00, 0x1cf2d},  // Znamenny Combining Mark ..Znamenny Combining Mark
	{0x1cf30, 0x1cf46},  // Znamenny Combining Tonal..Znamenny Priznak Modifie
	{0x1d167, 0x1d169},  // Musical Symbol Combining..Musical Symbol Combining
	{0x1d17b, 0x1d182},  // Musical Symbol Combining..Musical Symbol Combining
	{0x1d185, 0x1d18b},  // Musical Symbol Combining..Musical Symbol Combining
	{0x1d1aa, 0x1d1ad},  // Musical Symbol Combining..Musical Symbol Combining
	{0x1d242, 0x1d244},  // Combining Greek Musical ..Combining Greek Musical
	{0x1da00, 0x1da36},  // Signwriting Head Rim    ..Signwriting Air Sucking
	{0x1da3b, 0x1da6c},  // Signwriting Mouth Closed..Signwriting Excitement
	{0x1da75, 0x1da75},  // Signwriting Upper Body T..Signwriting Upper Body T
	{0x1da84, 0x1da84},  // Signwriting Location Hea..Signwriting Location Hea
	{0x1da9b, 0x1da9f},  // Signwriting Fill Modifie..Signwriting Fill Modifie
	{0x1daa1, 0x1daaf},  // Signwriting Rotation Mod..Signwriting Rotation Mod
	{0x1e000, 0x1e006},  // Combining Glagolitic Let..Combining Glagolitic Let
	{0x1e008, 0x1e018},  // Combining Glagolitic Let..Combining Glagolitic Let
	{0x1e01b, 0x1e021},  // Combining Glagolitic Let..Combining Glagolitic Let
	{0x1e023, 0x1e024},  // Combining Glagolitic Let..Combining Glagolitic Let
	{0x1e026, 0x1e02a},  // Combining Glagolitic Let..Combining Glagolitic Let
	{0x1e08f, 0x1e08f},  // (nil)                   ..(nil)
	{0x1e130, 0x1e136},  // Nyiakeng Puachue Hmong T..Nyiakeng Puachue Hmong T
	{0x1e2ae, 0x1e2ae},  // Toto Sign Rising Tone   ..Toto Sign Rising Tone
	{0x1e2ec, 0x1e2ef},  // Wancho Tone Tup         ..Wancho Tone Koini
	{0x1e4ec, 0x1e4ef},  // (nil)                   ..(nil)
	{0x1e8d0, 0x1e8d6},  // Mende Kikakui Combining ..Mende Kikakui Combining
	{0x1e944, 0x1e94a},  // Adlam Alif Lengthener   ..Adlam Nukta
	{0xe0100, 0xe01ef},  // Variation Selector-17   ..Variation Selector-256
};

static struct width_interval WIDE_EASTASIAN[] = {
	// from https://github.com/PitWD/termux_wcwidth
	// which is a copy of https://github.com/termux/wcwidth
	// all MIT licensed
	{0x01100, 0x0115f},  // Hangul Choseong Kiyeok  ..Hangul Choseong Filler
	{0x0231a, 0x0231b},  // Watch                   ..Hourglass
	{0x02329, 0x0232a},  // Left-pointing Angle Brac..Right-pointing Angle Bra
	{0x023e9, 0x023ec},  // Black Right-pointing Dou..Black Down-pointing Doub
	{0x023f0, 0x023f0},  // Alarm Clock             ..Alarm Clock
	{0x023f3, 0x023f3},  // Hourglass With Flowing S..Hourglass With Flowing S
	{0x025fd, 0x025fe},  // White Medium Small Squar..Black Medium Small Squar
	{0x02614, 0x02615},  // Umbrella With Rain Drops..Hot Beverage
	{0x02648, 0x02653},  // Aries                   ..Pisces
	{0x0267f, 0x0267f},  // Wheelchair Symbol       ..Wheelchair Symbol
	{0x02693, 0x02693},  // Anchor                  ..Anchor
	{0x026a1, 0x026a1},  // High Voltage Sign       ..High Voltage Sign
	{0x026aa, 0x026ab},  // Medium White Circle     ..Medium Black Circle
	{0x026bd, 0x026be},  // Soccer Ball             ..Baseball
	{0x026c4, 0x026c5},  // Snowman Without Snow    ..Sun Behind Cloud
	{0x026ce, 0x026ce},  // Ophiuchus               ..Ophiuchus
	{0x026d4, 0x026d4},  // No Entry                ..No Entry
	{0x026ea, 0x026ea},  // Church                  ..Church
	{0x026f2, 0x026f3},  // Fountain                ..Flag In Hole
	{0x026f5, 0x026f5},  // Sailboat                ..Sailboat
	{0x026fa, 0x026fa},  // Tent                    ..Tent
	{0x026fd, 0x026fd},  // Fuel Pump               ..Fuel Pump
	{0x02705, 0x02705},  // White Heavy Check Mark  ..White Heavy Check Mark
	{0x0270a, 0x0270b},  // Raised Fist             ..Raised Hand
	{0x02728, 0x02728},  // Sparkles                ..Sparkles
	{0x0274c, 0x0274c},  // Cross Mark              ..Cross Mark
	{0x0274e, 0x0274e},  // Negative Squared Cross M..Negative Squared Cross M
	{0x02753, 0x02755},  // Black Question Mark Orna..White Exclamation Mark O
	{0x02757, 0x02757},  // Heavy Exclamation Mark S..Heavy Exclamation Mark S
	{0x02795, 0x02797},  // Heavy Plus Sign         ..Heavy Division Sign
	{0x027b0, 0x027b0},  // Curly Loop              ..Curly Loop
	{0x027bf, 0x027bf},  // Double Curly Loop       ..Double Curly Loop
	{0x02b1b, 0x02b1c},  // Black Large Square      ..White Large Square
	{0x02b50, 0x02b50},  // White Medium Star       ..White Medium Star
	{0x02b55, 0x02b55},  // Heavy Large Circle      ..Heavy Large Circle
	{0x02e80, 0x02e99},  // Cjk Radical Repeat      ..Cjk Radical Rap
	{0x02e9b, 0x02ef3},  // Cjk Radical Choke       ..Cjk Radical C-simplified
	{0x02f00, 0x02fd5},  // Kangxi Radical One      ..Kangxi Radical Flute
	{0x02ff0, 0x02ffb},  // Ideographic Description ..Ideographic Description
	{0x03000, 0x0303e},  // Ideographic Space       ..Ideographic Variation In
	{0x03041, 0x03096},  // Hiragana Letter Small A ..Hiragana Letter Small Ke
	{0x03099, 0x030ff},  // Combining Katakana-hirag..Katakana Digraph Koto
	{0x03105, 0x0312f},  // Bopomofo Letter B       ..Bopomofo Letter Nn
	{0x03131, 0x0318e},  // Hangul Letter Kiyeok    ..Hangul Letter Araeae
	{0x03190, 0x031e3},  // Ideographic Annotation L..Cjk Stroke Q
	{0x031f0, 0x0321e},  // Katakana Letter Small Ku..Parenthesized Korean Cha
	{0x03220, 0x03247},  // Parenthesized Ideograph ..Circled Ideograph Koto
	{0x03250, 0x04dbf},  // Partnership Sign        ..Cjk Unified Ideograph-4d
	{0x04e00, 0x0a48c},  // Cjk Unified Ideograph-4e..Yi Syllable Yyr
	{0x0a490, 0x0a4c6},  // Yi Radical Qot          ..Yi Radical Ke
	{0x0a960, 0x0a97c},  // Hangul Choseong Tikeut-m..Hangul Choseong Ssangyeo
	{0x0ac00, 0x0d7a3},  // Hangul Syllable Ga      ..Hangul Syllable Hih
	{0x0f900, 0x0faff},  // Cjk Compatibility Ideogr..(nil)
	{0x0fe10, 0x0fe19},  // Presentation Form For Ve..Presentation Form For Ve
	{0x0fe30, 0x0fe52},  // Presentation Form For Ve..Small Full Stop
	{0x0fe54, 0x0fe66},  // Small Semicolon         ..Small Equals Sign
	{0x0fe68, 0x0fe6b},  // Small Reverse Solidus   ..Small Commercial At
	{0x0ff01, 0x0ff60},  // Fullwidth Exclamation Ma..Fullwidth Right White Pa
	{0x0ffe0, 0x0ffe6},  // Fullwidth Cent Sign     ..Fullwidth Won Sign
	{0x16fe0, 0x16fe4},  // Tangut Iteration Mark   ..Khitan Small Script Fill
	{0x16ff0, 0x16ff1},  // Vietnamese Alternate Rea..Vietnamese Alternate Rea
	{0x17000, 0x187f7},  // (nil)                   ..(nil)
	{0x18800, 0x18cd5},  // Tangut Component-001    ..Khitan Small Script Char
	{0x18d00, 0x18d08},  // (nil)                   ..(nil)
	{0x1aff0, 0x1aff3},  // Katakana Letter Minnan T..Katakana Letter Minnan T
	{0x1aff5, 0x1affb},  // Katakana Letter Minnan T..Katakana Letter Minnan N
	{0x1affd, 0x1affe},  // Katakana Letter Minnan N..Katakana Letter Minnan N
	{0x1b000, 0x1b122},  // Katakana Letter Archaic ..Katakana Letter Archaic
	{0x1b132, 0x1b132},  // (nil)                   ..(nil)
	{0x1b150, 0x1b152},  // Hiragana Letter Small Wi..Hiragana Letter Small Wo
	{0x1b155, 0x1b155},  // (nil)                   ..(nil)
	{0x1b164, 0x1b167},  // Katakana Letter Small Wi..Katakana Letter Small N
	{0x1b170, 0x1b2fb},  // Nushu Character-1b170   ..Nushu Character-1b2fb
	{0x1f004, 0x1f004},  // Mahjong Tile Red Dragon ..Mahjong Tile Red Dragon
	{0x1f0cf, 0x1f0cf},  // Playing Card Black Joker..Playing Card Black Joker
	{0x1f18e, 0x1f18e},  // Negative Squared Ab     ..Negative Squared Ab
	{0x1f191, 0x1f19a},  // Squared Cl              ..Squared Vs
	{0x1f200, 0x1f202},  // Square Hiragana Hoka    ..Squared Katakana Sa
	{0x1f210, 0x1f23b},  // Squared Cjk Unified Ideo..Squared Cjk Unified Ideo
	{0x1f240, 0x1f248},  // Tortoise Shell Bracketed..Tortoise Shell Bracketed
	{0x1f250, 0x1f251},  // Circled Ideograph Advant..Circled Ideograph Accept
	{0x1f260, 0x1f265},  // Rounded Symbol For Fu   ..Rounded Symbol For Cai
	{0x1f300, 0x1f320},  // Cyclone                 ..Shooting Star
	{0x1f32d, 0x1f335},  // Hot Dog                 ..Cactus
	{0x1f337, 0x1f37c},  // Tulip                   ..Baby Bottle
	{0x1f37e, 0x1f393},  // Bottle With Popping Cork..Graduation Cap
	{0x1f3a0, 0x1f3ca},  // Carousel Horse          ..Swimmer
	{0x1f3cf, 0x1f3d3},  // Cricket Bat And Ball    ..Table Tennis Paddle And
	{0x1f3e0, 0x1f3f0},  // House Building          ..European Castle
	{0x1f3f4, 0x1f3f4},  // Waving Black Flag       ..Waving Black Flag
	{0x1f3f8, 0x1f43e},  // Badminton Racquet And Sh..Paw Prints
	{0x1f440, 0x1f440},  // Eyes                    ..Eyes
	{0x1f442, 0x1f4fc},  // Ear                     ..Videocassette
	{0x1f4ff, 0x1f53d},  // Prayer Beads            ..Down-pointing Small Red
	{0x1f54b, 0x1f54e},  // Kaaba                   ..Menorah With Nine Branch
	{0x1f550, 0x1f567},  // Clock Face One Oclock   ..Clock Face Twelve-thirty
	{0x1f57a, 0x1f57a},  // Man Dancing             ..Man Dancing
	{0x1f595, 0x1f596},  // Reversed Hand With Middl..Raised Hand With Part Be
	{0x1f5a4, 0x1f5a4},  // Black Heart             ..Black Heart
	{0x1f5fb, 0x1f64f},  // Mount Fuji              ..Person With Folded Hands
	{0x1f680, 0x1f6c5},  // Rocket                  ..Left Luggage
	{0x1f6cc, 0x1f6cc},  // Sleeping Accommodation  ..Sleeping Accommodation
	{0x1f6d0, 0x1f6d2},  // Place Of Worship        ..Shopping Trolley
	{0x1f6d5, 0x1f6d7},  // Hindu Temple            ..Elevator
	{0x1f6dc, 0x1f6df},  // (nil)                   ..Ring Buoy
	{0x1f6eb, 0x1f6ec},  // Airplane Departure      ..Airplane Arriving
	{0x1f6f4, 0x1f6fc},  // Scooter                 ..Roller Skate
	{0x1f7e0, 0x1f7eb},  // Large Orange Circle     ..Large Brown Square
	{0x1f7f0, 0x1f7f0},  // Heavy Equals Sign       ..Heavy Equals Sign
	{0x1f90c, 0x1f93a},  // Pinched Fingers         ..Fencer
	{0x1f93c, 0x1f945},  // Wrestlers               ..Goal Net
	{0x1f947, 0x1f9ff},  // First Place Medal       ..Nazar Amulet
	{0x1fa70, 0x1fa7c},  // Ballet Shoes            ..Crutch
	{0x1fa80, 0x1fa88},  // Yo-yo                   ..(nil)
	{0x1fa90, 0x1fabd},  // Ringed Planet           ..(nil)
	{0x1fabf, 0x1fac5},  // (nil)                   ..Person With Crown
	{0x1face, 0x1fadb},  // (nil)                   ..(nil)
	{0x1fae0, 0x1fae8},  // Melting Face            ..(nil)
	{0x1faf0, 0x1faf8},  // Hand With Index Finger A..(nil)
	{0x20000, 0x2fffd},  // Cjk Unified Ideograph-20..(nil)
	{0x30000, 0x3fffd},  // Cjk Unified Ideograph-30..(nil)
};

static int intable(struct width_interval* table, int table_length, int c) {
	// from https://github.com/PitWD/termux_wcwidth
	// which is a copy of https://github.com/termux/wcwidth
	// all MIT licensed

	// First quick check for Latin1 etc. characters.
	if (c < table[0].start) return 0;

	// Binary search in table.
	int bot = 0;
	int top = table_length - 1;
	while (top >= bot) {
			int mid = (bot + top) / 2;
			if (table[mid].end < c) {
					bot = mid + 1;
			} else if (table[mid].start > c) {
					top = mid - 1;
			} else {
					return 1;
			}
	}
	return 0;
}

int CharLen(wchar_t ucs) {
	// from https://github.com/PitWD/termux_wcwidth
	// which is a MODYFIED copy of https://github.com/termux/wcwidth
	// all MIT licensed

	// NOTE: created by hand, there isn't anything identifiable other than
	// general Cf category code to identify these, and some characters in Cf
	// category code are of non-zero width.
	if (ucs == 0 ||
					ucs == 0x034F ||
					(ucs > 0x200A && ucs < 0x2010) ||
					(ucs > 0x2027 && ucs < 0x202F) ||
					(ucs > 0x205F && ucs < 0x2064)) {
			return 0;
	}

	// C0/C1 control characters.
	if (ucs < 32 || (ucs > 0x07E && ucs < 0x0A0)) return -1;

	// Combining characters with zero width.
	if (intable(ZERO_WIDTH, sizeof(ZERO_WIDTH)/sizeof(struct width_interval), ucs)) return 0;

	return intable(WIDE_EASTASIAN, sizeof(WIDE_EASTASIAN)/sizeof(struct width_interval), ucs) ? 2 : 1;
}
// *** 3rd Party END ***

int SizeCanvas(int x, int y) {
	// Function to allocate, reallocate, or free myCanvas
    static int isSized = 0;

    // If x or y is zero and isSized, free the array, reset isSized, and return
    if ((x == 0 || y == 0) && isSized) {
        for (int i = 0; i < canvasMaxX; i++) {
            for (int j = 0; j < canvasMaxY; j++) {
                free(myCanvas[i][j]);
            }
            free(myCanvas[i]);
        }
        free(myCanvas);
        isSized = 0;
        canvasMaxX = canvasMaxY = 0;
        return 0;
    }

    // If x and y are not zero and isSized, reallocate the array and return
    if (x > 0 && y > 0 && isSized) {
        myCanvas = (struct CanvasSTRUCT ***)realloc(myCanvas, x * sizeof(struct CanvasSTRUCT **));
        if (!myCanvas) {
            // Handle memory allocation error
            return -1;
        }

        for (int i = canvasMaxX; i < x; i++) {
            myCanvas[i] = (struct CanvasSTRUCT **)malloc(y * sizeof(struct CanvasSTRUCT *));
            if (!myCanvas[i]) {
                // Handle memory allocation error
                return -1;
            }

            for (int j = 0; j < y; j++) {
                myCanvas[i][j] = (struct CanvasSTRUCT *)malloc(canvasMaxZ * sizeof(struct CanvasSTRUCT));
                if (!myCanvas[i][j]) {
                    // Handle memory allocation error
                    return -1;
                }
            }
        }

        canvasMaxX = x;
        canvasMaxY = y;
        return 0;
    }

    // If x and y are not zero and not isSized, allocate the array and return
    if (x > 0 && y > 0 && !isSized) {
        myCanvas = (struct CanvasSTRUCT ***)malloc(x * sizeof(struct CanvasSTRUCT **));
        if (!myCanvas) {
            // Handle memory allocation error
            return -1;
        }

        for (int i = 0; i < x; i++) {
            myCanvas[i] = (struct CanvasSTRUCT **)malloc(y * sizeof(struct CanvasSTRUCT *));
            if (!myCanvas[i]) {
                // Handle memory allocation error
                return -1;
            }

            for (int j = 0; j < y; j++) {
                myCanvas[i][j] = (struct CanvasSTRUCT *)malloc(canvasMaxZ * sizeof(struct CanvasSTRUCT));
                if (!myCanvas[i][j]) {
                    // Handle memory allocation error
                    return -1;
                }
            }
        }

        canvasMaxX = x;
        canvasMaxY = y;
        isSized = 1;
        return 0;
    }

    return -1; // Return -1 for invalid input
}

int CompareCanvasPos(int x, int y, int z1, int z2) {
    // Check if x, y, z1, and z2 are within the valid range
    if (x < 0 || x >= canvasMaxX || y < 0 || y >= canvasMaxY || z1 < 0 || z1 >= canvasMaxZ || z2 < 0 || z2 >= canvasMaxZ) {
        // Invalid indices, return -1 to indicate an error
        return -1;
    }


    if (myCanvas[x][y][z1].c != myCanvas[x][y][z2].c) {
        return 1;
    }
    else if (myCanvas[x][y][z1].style != myCanvas[x][y][z2].style) {
        return 2;
    }
    else if (myCanvas[x][y][z1].color != myCanvas[x][y][z2].color) {
        return 4;
    }
    // else if (myCanvas[x][y][z1].width != myCanvas[x][y][z2].width) {
    //    // will never happen - already covered by comparing .c
	//	return 8;
    // }

    return 0;
}
int CompareCanvasPosUnsafe(int x, int y, int z1, int z2) {

    if (myCanvas[x][y][z1].c != myCanvas[x][y][z2].c) {
        return 1;
    }
    else if (myCanvas[x][y][z1].style != myCanvas[x][y][z2].style) {
        return 2;
    }
    else if (myCanvas[x][y][z1].color != myCanvas[x][y][z2].color) {
        return 4;
    }
    // else if (myCanvas[x][y][z1].width != myCanvas[x][y][z2].width) {
    //    // will never happen - already covered by comparing .c
	//	return 8;
    // }

    return 0;
}

int CopyCanvasPos(int x, int y, int z1, int z2) {
    // Check if x, y, z1, and z2 are within the valid range
    if (x < 0 || x >= canvasMaxX || y < 0 || y >= canvasMaxY || z1 < 0 || z1 >= canvasMaxZ || z2 < 0 || z2 >= canvasMaxZ) {
        // Invalid indices, return 0 to indicate an error
        return 0;
    }

    // Copy the content of myCanvas[x][y][z1] to myCanvas[x][y][z2]
    myCanvas[x][y][z2] = myCanvas[x][y][z1];

    // Return 1 to indicate a successful copy
    return 1;
}
void CopyCanvasPosUnsafe(int x, int y, int z1, int z2) {

    // Copy the content of myCanvas[x][y][z1] to myCanvas[x][y][z2]
    myCanvas[x][y][z2] = myCanvas[x][y][z1];

}

int CopyCanvasAll(int z1, int z2) {

    if (z1 < 0 || z1 >= canvasMaxZ || z2 < 0 || z2 >= canvasMaxZ || canvasMaxX <= 0 || canvasMaxY <= 0) {
        // Invalid indices, return 0 to indicate an error
        return 0;
    }

	size_t size = sizeof(struct CanvasSTRUCT) * canvasMaxX * canvasMaxY;
    // Use a single memcpy to copy the entire array
    memcpy(&myCanvas[0][0][z2], &myCanvas[0][0][z1], size);

    // Return 1 to indicate a successful copy
    return size;
}



// Collector for all strings of EscColor and EscStyle

// Standard, but optional, ESC Sequences
char RUL[] = "24";				// 'ResetUnderLine' alternativ: "4:0" 

// Actual Settings / States
static EscStyleSTRUCT ActTxtStyle;
static EscColorSTRUCT ActTxtColor;

// global access on colors and styles
EscColorSTRUCT *userColors = NULL;
EscStyleSTRUCT *userStyles = NULL;

void ResFBU(void);
void SetColorStyle(EscColorSTRUCT *pColor, int set);
void SetTxtStyle(EscStyleSTRUCT *pStyle, int set);
void LocateX(int x);
void SetFg255(int c);
void SetBg255(int c);
void TxtDblWidth(int set);
void TxtDblTop(int set);
void TxtDblBot(int set);

// 'VGA'16-Pallet - colors
enum {
	fgBlack = 30, fgRed, fgGreen, fgYellow, fgBlue, fgMagenta, fgCyan, fgWhite,
	bgBlack = 40, bgRed, bgGreen, bgYellow, bgBlue, bgMagenta, bgCyan, bgWhite,
	fgBlackB = 90, fgRedB, fgGreenB, fgYellowB, fgBlueB, fgMagentaB, fgCyanB, fgWhiteB,
	bgBlackB = 100, bgRedB, bgGreenB, bgYellowB, bgBlueB, bgMagentaB, bgCyanB, bgWhiteB
};

// 'VGAs' as 256 - colors
enum {
	cBlack, cRed, cGreen, cYellow, cBlue, cMagenta, cCyan, cWhite,
	cBlackB, cRedB, cGreenB, cYellowB, cBlueB, cMagentaB, cCyanB, cWhiteB
};

// 256 Grey's White -> Black 
int ESC_Grey[27];		// initialization in InitColors

void ESCinit(void) {

	// The 24 Greys (+ Black/White) from 256 Palette	
	ESC_Grey[1] = 15; ESC_Grey[26] = 0;
	int p = 1;
	for (int t = 255; t > 231; t--) {
		p++;
		ESC_Grey[p] = t;
	}

	// Default ColorStyle
	// ActTxtStyle.pColor = &ActTxtColor;
}

int ESCinitColors(char *strFile, EscColorSTRUCT **userColor){

	// Users Colors - Helper
	char strSearch[STR_SMALL_SIZE];
	char strGroupName[STR_SMALL_SIZE];
	char strColorName[STR_SMALL_SIZE];
	char strHLP[STR_SMALL_SIZE];

	// count of color groups
	int colorsGroupsCount = IniGetInt(strFile, "global.colors.GroupCount", 0);
	// groups color model
	int colorsModel = IniGetInt(strFile, "global.colors.ColorModel", 0);
	
	// memory to store count of colors in each group
	int *colorsCount;
	colorsCount = (int*)malloc(colorsGroupsCount * sizeof(int));
	
	int colorsCountSum = 0;

	// read counts of colors and calc sum
	for (int i = 0; i < colorsGroupsCount; i++){
		sprintf(strSearch, "global.colors.group%d.Count", i + 1);
		colorsCount[i] = IniGetInt(strFile, strSearch, 0);
		colorsCountSum += colorsCount[i];
	}
	
	// memory to store all colors
	*userColor = (EscColorSTRUCT*)malloc(colorsCountSum * sizeof(EscColorSTRUCT));

	colorsCountSum = 0;

	for (int i = 0; i < colorsGroupsCount; i++){
		sprintf(strSearch, "global.colors.group%d.Name", i + 1);
		sprintf(strHLP, "Group%d", i + 1);
		IniGetStr(strFile, strSearch, strHLP, strGroupName);
		for (int j = 0; j < colorsCount[i]; j++){

			(*userColor)[colorsCountSum].groupName = IniStrToMem(strGroupName, 0);
			(*userColor)[colorsCountSum].groupID = i + 1;

			sprintf(strSearch, "global.colors.group%d.%d.Name", i + 1, j + 1);
			sprintf(strHLP, "Color%d", j + 1);
			IniGetStr(strFile, strSearch, strHLP, strColorName);
			(*userColor)[colorsCountSum].colorName = IniStrToMem(strColorName, 0);
			(*userColor)[colorsCountSum].colorID = j + 1;

			sprintf(strSearch, "global.colors.group%d.%d.ForeGround", i + 1, j + 1);
			(*userColor)[colorsCountSum].fg.Color = IniGetInt(strFile, strSearch, 15);
			sprintf(strSearch, "global.colors.group%d.%d.BackGround", i + 1, j + 1);
			(*userColor)[colorsCountSum].bg.Color = IniGetInt(strFile, strSearch, 0);
			sprintf(strSearch, "global.colors.group%d.%d.UnderLine", i + 1, j + 1);
			(*userColor)[colorsCountSum].ul.Color = IniGetInt(strFile, strSearch, 15);
			
			(*userColor)[colorsCountSum].mode = colorsModel;
			
			// printf("%04d. %s_%s:\n",colorsCountSum + 1, strGroupName, strColorName);
			printf("%04d. %s_%s: ", colorsCountSum , (*userColor)[colorsCountSum].groupName, (*userColor)[colorsCountSum].colorName);

			colorsCountSum++;

			LocateX(36);
			SetColorStyle(&(*userColor)[colorsCountSum - 1], 1);
			//printf("Res - Done\n");
			//return;
			printf("->   (%03d:%03d)   <-", (*userColor)[colorsCountSum - 1].fg.Color, (*userColor)[colorsCountSum - 1].bg.Color);
			fflush(stdout);
			ResFBU();
			printf("\n");
			fflush(stdout);
		}
		printf("\n");
		fflush(stdout);		
	}

	/*
	for (size_t i = 0; i < colorsCountSum; i++)
	{
		SetColorStyle(&(*userColor)[i], 1);
		printf("%04d. %s.%s\n", i, (*userColor)[i].groupName, (*userColor)[i].colorName);
	}
	*/

	free(colorsCount);
	return colorsCountSum;
}

int ESCinitTxtStyles(char *strFile, EscStyleSTRUCT **userTxtStyles){

	// Users Colors
	char strSearch[STR_SMALL_SIZE];
	char strGroupName[STR_SMALL_SIZE];
	char strStyleName[STR_SMALL_SIZE];
	char strHLP[STR_SMALL_SIZE];

	//char strFile[] = "desktops.ini";
	int stylesGroupsCount = IniGetInt(strFile, "global.txtStyles.GroupCount", 0);
	
	int *stylesCount;
	stylesCount = (int*)malloc(stylesGroupsCount * sizeof(int));
	
	int stylesCountSum = 0;

	for (int i = 0; i < stylesGroupsCount; i++){
		sprintf(strSearch, "global.txtStyles.group%d.Count", i + 1);
		stylesCount[i] = IniGetInt(strFile, strSearch, 0);
		stylesCountSum += stylesCount[i];
	}
	
	*userTxtStyles = (EscStyleSTRUCT*)malloc(stylesCountSum * sizeof(EscStyleSTRUCT));
	
	stylesCountSum = 0;

	for (int i = 0; i < stylesGroupsCount; i++){
		sprintf(strSearch, "global.txtStyles.group%d.Name", i + 1);
		sprintf(strHLP, "Font%d", i + 1);
		IniGetStr(strFile, strSearch, strHLP, strGroupName);
		for (int j = 0; j < stylesCount[i]; j++){

			(*userTxtStyles)[stylesCountSum].fontName = IniStrToMem(strGroupName, 0);
			(*userTxtStyles)[stylesCountSum].fontID = i + 1;

			sprintf(strSearch, "global.txtStyles.group%d.%d.Name", i + 1, j + 1);
			sprintf(strHLP, "Style%d", j + 1);
			IniGetStr(strFile, strSearch, strHLP, strStyleName);
			(*userTxtStyles)[stylesCountSum].styleName = IniStrToMem(strStyleName, 0);
			(*userTxtStyles)[stylesCountSum].styleID = j + 1;

			sprintf(strSearch, "global.txtStyles.group%d.%d.bold", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].bold = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.faint", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].faint = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.italic", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].italic = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.blink", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].blink = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.fastblink", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].fast = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.reverse", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].reverse = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.invisible", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].invisible = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.strikethrough", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].strike = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.superscript", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].superscript = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.subscript", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].subscript = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.proportional", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].proportional = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.framed", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].framed = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.encircled", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].encircled = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.overline", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].overline = IniGetBool(strFile, strSearch, 0);

			/*
			sprintf(strSearch, "global.txtStyles.group%d.%d.ideoright", i + 1, j + 1);
			userTxtStyles[stylesCountSum].ideo_right = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.ideoleft", i + 1, j + 1);
			userTxtStyles[stylesCountSum].ideo_left = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.ideodblright", i + 1, j + 1);
			userTxtStyles[stylesCountSum].ideo_dbl_right = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.ideodblleft", i + 1, j + 1);
			userTxtStyles[stylesCountSum].ideo_dbl_left = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.ideostress", i + 1, j + 1);
			userTxtStyles[stylesCountSum].ideo_stress = IniGetBool(strFile, strSearch, 0);
			*/

			sprintf(strSearch, "global.txtStyles.group%d.%d.dblwidth", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].dbl_width = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.dblheight", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].dbl_height = IniGetBool(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.font", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].font = IniGetInt(strFile, strSearch, 0);

			sprintf(strSearch, "global.txtStyles.group%d.%d.underline", i + 1, j + 1);
			(*userTxtStyles)[stylesCountSum].underline = IniGetInt(strFile, strSearch, 0);

		    // *userTxtStyles)[stylesCountSum].pColor = &ActTxtColor;

			printf("%04d. %s_%s: ",stylesCountSum + 1, (*userTxtStyles)[stylesCountSum].fontName, (*userTxtStyles)[stylesCountSum].styleName);

			stylesCountSum++;

			LocateX(45);
			SetTxtStyle(&(*userTxtStyles)[stylesCountSum - 1], 1);
			if ((*userTxtStyles)[stylesCountSum - 1].dbl_height){
				printf("\n");
				TxtDblTop(1);
				printf("->   ( TEST )   <-\n");
				TxtDblBot(1);
				printf("->   ( TEST )   <-");
			}
			else if ((*userTxtStyles)[stylesCountSum - 1].dbl_width){
				printf("\n");
				TxtDblWidth(1);
				printf("->   ( TEST )   <-");				
			}
			else{
				printf("->   ( TEST )   <-");
			}					
			fflush(stdout);
			SetTxtStyle(&(*userTxtStyles)[stylesCountSum - 1], 0);
			printf("\n");
			fflush(stdout);
		}
		printf("\n");
		fflush(stdout);		
	}

	free(stylesCount);
	
	return stylesCountSum;
}

// Cursor Positions
void Locate(int x, int y) {
	// printf("%c[%d;%df", 0x01b, y, x);		//('f' instead 'H') is working, at least on WIN, too. (more... see wikipedia 'ANSI-ESC') 
	printf("\x1B[%d;%dH", y, x);
	TERM_CursorPosY = y;
	TERM_CursorPosX = x;
}
void LocateX(int x) {
	printf("\x1B[%dG", x);
}
void CursorUp(int y) {
	printf("\x1B[%dA", y);
}
void CursorUp1st(int y) {
	printf("\x1B[%dF", y);
}
void CursorDown(int y) {
	printf("\x1B[%dB", y);
}
void CursorMoveY(int y){
	if (y < 0){
		// Down
		CursorDown(y * -1);
	}
	else if (y > 0){
		// Up
		CursorUp(y);
	}
}
void CursorDown1st(int y) {
	printf("\x1B[%dE", y);
}
void CursorRight(int x) {
	printf("\x1B[%dC", x);
}
void CursorLeft(int x) {
	printf("\x1B[%dD", x);
}
void CursorMoveX(int x){
	if (x < 0){
		// Left
		CursorLeft(x * -1);
	}
	else if (x > 0){
		// Right
		CursorRight(x);
	}
}
void GetAnsiCursorPos(void){
	TERM_CursorWaitFor = 1;
	printf("\x1B[6n");
}
void CursorSave(){
  printf("\0337");
}
void CursorRestore(){
  printf("\0338");
}

#define CursorMoveXY(x, y) CursorMoveX(x); CursorMoveY(y)

// DEC BoxDraw
/*
DEC char to BoxDraw:
		printf("a b c d e f g h i j k l m n o p q r s t u v w x y z\n");
		printf("\x1B(0"); // enable BoxDraw
		printf("a b c d e f g h i j k l m n o p q r s t u v w x y z\n");
		printf("\x1B(B"); // disable BoxDraw
		                    1     1     1       1 1 1
                          9 0 6 5 5     2     7 1 3 4 3		
		a b c d e f g h i j k l m n o p q r s t u v w x y z
		▒ ␉ ␌ ␍ ␊ ° ± ␤ ␋ ┘ ┐ ┌ └ ┼ ⎺ ⎻ ─ ⎼ ⎽ ├ ┤ ┴ ┬ │ ≤ ≥

If frames are overlapping, it's just a simple bitwise OR to get the
right char for displaying the crossing point right.

(00)  = n/a					
(01)  = n/a					(Top)
(02)  = n/a					(Bottom)
(03) │ = Top-Bottom			
(04)  = n/a					(Right)
(05) └ = Top-Right
(06) ┌ = Bottom-Right
(07) ├ = Top-Right-Bottom
(08)  = n/a					(Left)
(09) ┘ = Top-Left
(10) ┐ = Bottom-Left
(11) ┤ = Top-Bottom-Left
(12) ─ = Left-Right
(13) ┴ = Left-Right-Top
(14) ┬ = Bottom-Left-Right
(15) ┼ = Top-Right-Bottom-Left

static char TuiDecBoxDraw[16] = {' ', ' ', ' ', '*', ' ', '*',
									'*', '*', ' ', '*', '*', '*',
									'*', '*', '*', '*' };

static char TuiDecBoxDraw[16] = {' ', ' ', ' ', 'x', ' ', 'm',
									'l', 't', ' ', 'j', 'k', 'u',
									'q', 'v', 'w', 'n' };

*/
static char TuiDecBoxDraw[16] = {' ', ' ', ' ', 'x', ' ', 'm',
									'l', 't', ' ', 'j', 'k', 'u',
									'q', 'v', 'w', 'n' };

void DECboxMode(int set){
	if (set){
		printf("\x1B(0"); // enable BoxDraw Mode
	}
	else{
		printf("\x1B(B"); // disable BoxDraw Mode
	}
	fflush(stdout);
}
#define DECboxON DECboxMode(1)
#define DECboxOFF DECboxMode(0)
void DEClineX(int len){

	int isLastLine = 0;

	#if __APPLE__
		// we're fine
	//#elif __WIN32__ || _MSC_VER || __WIN64__
		// no clue actually 
	#else
		// we can't print regularly, if we're on the last column 
		if (TERM_CursorPosX == TERM_ScreenWidth){
			isLastLine = 1;
		}		
	#endif

	DECboxON;

	if (len < 0){
		// Right to left
		for (int i = 0; i > len; i--){
			printf("q");
			if (!isLastLine){
				CursorLeft(2);
			}
			else{
				CursorLeft(1);
				isLastLine = 0;
			}			
		}
	}
	else{
		// Left to right
		StrPrintChars('q', len);
	}
	
	DECboxOFF;

}
void DEClineY(int len){
	
	DECboxON;

	int isLastLine = 0;

	#if __APPLE__
		// we're fine
	//#elif __WIN32__ || _MSC_VER || __WIN64__
		// like linux 
	#else
		// we can't print regularly, if we're on the last column 
		if (TERM_CursorPosX == TERM_ScreenWidth){
			isLastLine = 1;
		}		
	#endif

	if (len < 0){
		// bottom to top
		for (int i = 0; i > len; i--){
			printf("x");
			if (!isLastLine){
				CursorLeft(1);
			}
			CursorUp(1);
		}
	}
	else{
		// top to bottom
		while (len--){
			printf("x");
			if (!isLastLine){
				CursorLeft(1);
			}
			CursorDown(1);
		}
	}
	
	DECboxOFF;
		
}

void CHRline(int spX, int spY, int epX, int epY, int newLine, char c) {
    
	static int lastX;
	static int lastY;
	static int polyStartX;
	static int polyStartY;

	static char myChar;
	
	if (c){
		// just set the char
		myChar = c;
	}
	
	if (newLine == 1){
		// just a moveTo for the 1st line
		lastX = spX;
		lastY = spY;
		polyStartX = spX;
		polyStartY = spY;
		return;
	}
	else if (newLine == 3){
		// close poly-ine
		spX = lastX;
		spY = lastY;
		epX = polyStartX;
		epY = polyStartY;
	}
	else if (newLine == 2){
		// full (new) line
		polyStartX = spX;
		polyStartY = spY;
	}
	else{
		// lineTo
		spX = lastX;
		spY = lastY;
	}

	double spXd = spX;
	double spYd = spY;
	double epXd = epX;
	double epYd = epY;
	
	int r = LineInRect(&spXd, &spYd, &epXd, &epYd, 1, 1, TERM_ScreenWidth, TERM_ScreenHeight);

	if (r){
		// We have a line to draw

		// back to int
		spX = (spXd + 0.5);
		spY = (spYd + 0.5);
		epX = (epXd + 0.5);
		epY = (epYd + 0.5);
	}
	else{
		lastX = (epXd + 0.5);
		lastY = (epYd + 0.5);
		return;
	}
	
	int dx = abs(epX - spX);
    int dy = abs(epY - spY);
    int sx = (spX < epX) ? 1 : -1;
    int sy = (spY < epY) ? 1 : -1;

    int diff = dx - dy;

	char nextHorz = 0;

    while (1) {
		Locate(spX, spY);

		printf("%c", myChar);
		
        if (spX == epX && spY == epY) {
			lastX = spX;
			lastY = spY;
			fflush(stdout);
			break;
        }

        int diff2 = 2 * diff;

        if (diff2 > -dy) {
            diff -= dy;
            spX += sx;
        }

        if (diff2 < dx) {
            diff += dx;
            spY += sy;
        }
    }
}
#define CHRlineXY(startX, startY, stopX, stopY, c) CHRline(startX, startY, stopX, stopY, 2, c)
#define CHRmoveTo(x, y, c) CHRline(x, y, 0, 0, 1, c)
#define CHRlineTo(x, y) CHRline(0, 0, x, y, 0, 0)
#define CHRclose CHRline(0, 0, 0, 0, 3, 0)

void LINline(int spX, int spY, int epX, int epY, int newLine) {
    
	static int lastX;
	static int lastY;
	static int polyStartX;
	static int polyStartY;
	
	if (newLine == 1){
		// just a moveTo for the 1st line
		lastX = spX;
		lastY = spY;
		polyStartX = spX;
		polyStartY = spY;
		return;
	}
	else if (newLine == 3){
		// close poly-ine
		spX = lastX;
		spY = lastY;
		epX = polyStartX;
		epY = polyStartY;
	}
	else if (newLine == 2){
		// full (new) line
		polyStartX = spX;
		polyStartY = spY;
	}
	else{
		// lineTo
		spX = lastX;
		spY = lastY;
	}

	double spXd = spX;
	double spYd = spY;
	double epXd = epX;
	double epYd = epY;
	
	int r = LineInRect(&spXd, &spYd, &epXd, &epYd, 1, 1, TERM_ScreenWidth, TERM_ScreenHeight);

	if (r){
		// We have a line to draw

		// back to int
		spX = (spXd + 0.5);
		spY = (spYd + 0.5);
		epX = (epXd + 0.5);
		epY = (epYd + 0.5);
	}
	else{
		lastX = (epXd + 0.5);
		lastY = (epYd + 0.5);
		return;
	}
	
	
	int dx = abs(epX - spX);
    int dy = abs(epY - spY);
    int sx = (spX < epX) ? 1 : -1;
    int sy = (spY < epY) ? 1 : -1;

    int diff = dx - dy;

	char nextHorz = 0;

    while (1) {
		Locate(spX, spY);

		if (nextHorz){
			// we're on a Y-jump of a more horizontal line....
			printf("%c", nextHorz);
			nextHorz = 0;
			DECboxOFF;
		}
		else if (dx > dy){
			// more horizontal
			printf("-");
		}
		else if (dx < dy){
			// more vertical
	        printf("|");
		}
		else{
			// 45°
			if (sx == sy ){
		        printf("\\");
			}
			else{
		        printf("/");
			}			
		}
		
        if (spX == epX && spY == epY) {
			lastX = spX;
			lastY = spY;
			fflush(stdout);
			break;
        }

		lastX = spX;
		lastY = spY;

        int diff2 = 2 * diff;

        if (diff2 > -dy) {
            diff -= dy;
            spX += sx;
			if (dx < dy){
				// more vertical - is a jump
				Locate(lastX, lastY);
				if (sx == sy){
					printf("\\");
				}
				else{
					printf("/");
				}
			}
        }

        if (diff2 < dx) {
            diff += dx;
            spY += sy;
			if (dx > dy){
				// more horizontal - is a jump
				Locate(lastX, lastY);
				DECboxON;
				if (sy == 1){
					printf("s");
					nextHorz = 'o';
				}
				else{
					printf("o");
					nextHorz = 's';
				}				
			}
        }
    }
}
#define LINlineXY(startX, startY, stopX, stopY) LINline(startX, startY, stopX, stopY, 2)
#define LINmoveTo(x, y) LINline(x, y, 0, 0, 1)
#define LINlineTo(x, y) LINline(0, 0, x, y, 0)
#define LINclose LINline(0, 0, 0, 0, 3)

void LINline2(int spX, int spY, int epX, int epY, int newLine) {
    
	static int lastX;
	static int lastY;
	static int lastChar;
	static int lastDir;

	static int polyStartX;
	static int polyStartY;
	static int polyStartChar;
	static int polyStartDir;

	static int firstLine = 0;	// Becomes true after 1st line of a poly-line is drawn
								// to signalize, that start-point intersection need to get calculated
	int doInterStart = 0;
	int doInterStop = 0;

	int actChar = 0;
	int actDir = 0;

	if (newLine == 1){
		// just a moveTo for the 1st line
		lastX = spX;
		lastY = spY;
		polyStartX = spX;
		polyStartY = spY;
		polyStartChar = 0;
		polyStartDir = 0;
		lastChar = 0;
		lastDir = 0;
		firstLine = 0;
		return;
	}
	else if (newLine == 3){
		// close poly-ine
		spX = lastX;
		spY = lastY;
		epX = polyStartX;
		epY = polyStartY;
		doInterStop = 1;
	}
	else if (newLine == 2){
		// full (new) line
		polyStartX = spX;
		polyStartY = spY;
		polyStartChar = 0;
		polyStartDir = 0;
		lastChar = 0;
		lastDir = 0;
		firstLine = 0;
	}
	else{
		// lineTo
		spX = lastX;
		spY = lastY;
	}

	double spXd = spX;
	double spYd = spY;
	double epXd = epX;
	double epYd = epY;
	
	int r = LineInRect(&spXd, &spYd, &epXd, &epYd, 1, 1, TERM_ScreenWidth, TERM_ScreenHeight);

	#define SBS 139		// '/' + '\'

	if (r){
		// We have a line to draw

		// back to int
		spX = (spXd + 0.5);
		spY = (spYd + 0.5);
		epX = (epXd + 0.5);
		epY = (epYd + 0.5);
	}
	else{
		lastX = (epXd + 0.5);
		lastY = (epYd + 0.5);
		return;
	}
	
	int dx = abs(epX - spX);
    int dy = abs(epY - spY);
    int sx = (spX < epX) ? 1 : -1;
    int sy = (spY < epY) ? 1 : -1;

    int diff = dx - dy;

	char nextHorz = 0;

	
	actDir = sy;
	
	
	doInterStart = firstLine;

    while (1) {
		Locate(spX, spY);

		actChar = 0;

		if (nextHorz){
			// we're on a Y-jump of a more horizontal line....
			printf("%c", nextHorz);
			nextHorz = 0;
			DECboxOFF;
		}
		else if (dx > dy){
			// more horizontal
			printf("-");
		}
		else if (dx < dy){
			// more vertical
	        printf("|");
		}
		else{
			// 45°
			if (sx == sy ){
		        printf("\\");
				actChar = '\\';
			}
			else{
		        printf("/");
				actChar = '/';
			}			
		}
		
        if (spX == epX && spY == epY){
			if (doInterStop){
				Locate(spX, spY);
				actChar += polyStartChar;
				if (polyStartDir != actDir && actChar == SBS){
					printf("-");
				}
				else if(actChar == SBS){
					printf("|");
				}
			}

			if (!firstLine){
				polyStartChar = actChar;
				polyStartDir = actDir;
			}			
			lastX = spX;
			lastY = spY;
			lastChar = actChar;
			lastDir = actDir;
			fflush(stdout);
			firstLine = 1;
			break;
        }

		lastX = spX;
		lastY = spY;

        int diff2 = 2 * diff;

        if (diff2 > -dy) {
            diff -= dy;
            spX += sx;
			if (dx < dy){
				// more vertical - is a jump
				Locate(lastX, lastY);
				if (sx == sy){
					printf("\\");
					actChar = '\\';
				}
				else{
					printf("/");
					actChar = '/';
				}
			}
        }

        if (diff2 < dx) {
            diff += dx;
            spY += sy;
			if (dx > dy){
				// more horizontal - is a jump
				Locate(lastX, lastY);
				DECboxON;
				if (sy == 1){
					printf("s");
					nextHorz = 'o';
				}
				else{
					printf("o");
					nextHorz = 's';
				}				
			}
        }

		if (doInterStart){
			Locate(lastX, lastY);
			lastChar += actChar;

			if (lastDir != actDir && lastChar == SBS){
				printf("-");
			}
			else if(lastChar == SBS){
				printf("|");
			}

			doInterStart = 0;
		}

		lastChar = actChar;
		lastDir = actDir;
    }
}
#define LINlineXY2(startX, startY, stopX, stopY) LINline2(startX, startY, stopX, stopY, 2)
#define LINmoveTo2(x, y) LINline2(x, y, 0, 0, 1)
#define LINlineTo2(x, y) LINline2(0, 0, x, y, 0)
#define LINclose2 LINline2(0, 0, 0, 0, 3)

void DECline(int spX, int spY, int epX, int epY, int newLine) {
    
	static int lastX;
	static int lastY;
	static int polyStartX;
	static int polyStartY;
	static int polyStartDir;
	static int polyFromDir;
	static int polyToDir;
	
	static int firstLine = 0;	// Becomes true after 1st line of a poly-line is drawn
								// to signalize, that start-point intersection need to get calculated
	int doInterStart = 0;
	int doInterStop = 0;
	
	static int fromDirLast;
	static int toDirLast;
	int fromDir = 0;
	int toDir = 0;
	
	if (newLine == 1){
		// just a moveTo for the 1st line
		lastX = spX;
		lastY = spY;
		polyStartX = spX;
		polyStartY = spY;
		firstLine = 0;
		return;
	}
	else if (newLine == 3){
		// close poly-ine
		spX = lastX;
		spY = lastY;
		epX = polyStartX;
		epY = polyStartY;
		doInterStop = 1;
	}
	else if (newLine == 2){
		// full (new) line
		polyStartX = spX;
		polyStartY = spY;
		firstLine = 0;
	}
	else{
		// lineTo
		spX = lastX;
		spY = lastY;
	}

	double spXd = spX;
	double spYd = spY;
	double epXd = epX;
	double epYd = epY;
	
	int r = LineInRect(&spXd, &spYd, &epXd, &epYd, 1, 1, TERM_ScreenWidth, TERM_ScreenHeight);

	if (r){
		// We have a line to draw

		// back to int
		spX = (spXd + 0.5);
		spY = (spYd + 0.5);
		epX = (epXd + 0.5);
		epY = (epYd + 0.5);
	}
	else{
		lastX = epX;
		lastY = epY;
		firstLine = 0;
		return;
	}
	
	
	int dx = abs(epX - spX);
    int dy = abs(epY - spY);
    int sx = (spX < epX) ? 1 : -1;
    int sy = (spY < epY) ? 1 : -1;

    int diff = dx - dy;

	char nextHorz = 0;

	doInterStart = firstLine;

	DECboxON;
    while (1) {
		Locate(spX, spY);

		if (dx > dy){
			// more horizontal
			if (sx == 1){
				// Left -> Right
				fromDir = 8;		// Left (West)
				toDir = 4;
			}
			else{
				// Left <- Right
				fromDir = 4;		// Right (East)
				toDir = 8;
			}
			printf("q");
		}
		else if (dx < dy){
			// more vertical
			if (sy == 1){
				// Top -> Down
				fromDir = 1;		// Top (North)
				toDir = 2;
			}
			else{
				// Down -> Top
				fromDir = 2;		// Down (South)
				toDir = 1;
			}
			printf("x");
		}
		else{
			// 45°
			char char2nd = 0;
			if (sx == sy){
				// Backslash - TopLeft - BottomRight
		        if (sy == 1){
					// Top -> Bottom └┐
					printf("m");
					char2nd= 'k';			// └┐
					fromDir = 1;		// Top (North)
					toDir = 4;
				}
				else{
					// Bottom -> Top └┐
					printf("k");			// ┐
					Locate(spX - 1, spY);
					char2nd = 'm';			// └
					fromDir = 2;		// Down (South)
					toDir = 8;
				}
			}
			else{
				// Slash - TopRight - BottomLeft
		        if (sy == 1){
					// Top -> Bottom ┌┘
					printf("j");			// ┘
					Locate(spX - 1, spY);
					char2nd = 'l';			// ┌
					fromDir = 1;		// Top (North)
					toDir = 8;
				}
				else{
					// Bottom -> Top ┌┘
					printf("l");			// ┌┘
					fromDir = 2;		// Down (South)
					toDir = 4;
					char2nd = 'j';
				}
			}			
			if (!(spX == epX && spY == epY)){
				printf("%c", char2nd);
			}
		}

		lastX = spX;
		lastY = spY;
        if (spX == epX && spY == epY) {
			if (doInterStop){
				Locate(spX, spY);
				if (((fromDir + toDir) == 3 && (polyFromDir + polyToDir) == 12) || ((fromDir + toDir) == 12 && (polyFromDir + polyToDir) == 3)){
					// Vert meets Horz
					printf("%c", TuiDecBoxDraw[polyToDir + fromDir]);
				}
				else{
					// no 90°
					printf("%c", TuiDecBoxDraw[(fromDir + toDir) | (polyFromDir + polyToDir)]);
				}
			}
			if (!firstLine){
				polyFromDir = fromDir;
				polyToDir = toDir;
			}
			DECboxOFF;
			fflush(stdout);
			firstLine = 1;
			break;
        }

        int diff2 = 2 * diff;

        if (diff2 > -dy) {
            diff -= dy;
            spX += sx;
			if (dx < dy){
				// more vertical - is a jump
				Locate(lastX, lastY);
				if (sx == sy){
					// Backslash - TopLeft - BottomRight
					if (sy == 1){
						// Top -> Bottom └┐
						printf("mk");			// └┐
						fromDir = 1;
						toDir = 4;
					}
					else{
						// Bottom -> Top └┐
						printf("k");			// ┐
						Locate(lastX - 1, lastY);
						printf("m");			// └
						fromDir = 2;
						toDir = 8;
					}
				}
				else{
					// Slash - TopRight - BottomLeft
					if (sy == 1){
						// Top -> Bottom ┌┘
						printf("j");			// ┘
						Locate(lastX - 1, lastY);
						printf("l");
						fromDir = 1;
						toDir = 8;			// ┌
					}
					else{
						// Bottom -> Top ┌┘
						printf("lj");			// ┌┘
						fromDir = 2;
						toDir = 4;
					}
				}
			}
        }

        if (diff2 < dx) {
            diff += dx;
            spY += sy;
			if (dx > dy){
				// more horizontal - is a jump
				Locate(lastX, lastY);
				if (sx == sy){
					// Backslash - TopLeft - BottomRight
					if (sx == 1){
						// Left -> Right ┐
						//               └ Top -> Down
						printf("k");
						Locate(lastX, lastY + 1);
						printf("m");
						fromDir = 8;
						toDir= 2;
					}
					else{
						// Left <- Right ┐
						//               └ Up <- Down
						printf("m");
						Locate(lastX, lastY - 1);
						printf("k");
						fromDir = 4;
						toDir = 1;
					}
				}
				else{
					// Slash - TopRight - BottomLeft
					if (sx == 1){
						//             ┌ Left -> Right
						// Down -> Top ┘
						printf("j");
						Locate(lastX, lastY - 1);
						printf("l");
						fromDir = 8;
						toDir = 1;
					}
					else{
						//             ┌ Left <- Right
						// Down <- Top ┘
						printf("l");
						Locate(lastX, lastY + 1);
						printf("j");
						fromDir = 4;
						toDir = 2;
					}
				}
			}
        }
		if (doInterStart){
			Locate(lastX, lastY);
			if (((fromDir + toDir) == 3 && (fromDirLast + toDirLast) == 12) || ((fromDir + toDir) == 12 && (fromDirLast + toDirLast) == 3)){
				// Vert meets Horz
				printf("%c", TuiDecBoxDraw[fromDirLast + toDir]);
			}
			else{
				// no 90°
				printf("%c", TuiDecBoxDraw[(fromDir + toDir) | (fromDirLast + toDirLast)]);
			}
			doInterStart = 0;
		}
		fromDirLast = fromDir;
		toDirLast = toDir;
	}
}
#define DEClineXY(startX, startY, stopX, stopY) DECline(startX, startY, stopX, stopY, 2)
#define DECmoveTo(x, y) DECline(x, y, 0, 0, 1)
#define DEClineTo(x, y) DECline(0, 0, x, y, 0)
#define DECclose DECline(0, 0, 0, 0, 3)

void DECrect(int startX, int startY, int stopX, int stopY){
	
	// normalize rect

	int temp = 0;
	if (stopX < startX){
		temp = stopX;
		stopX = startX;
		startX = temp;
	}
	if (stopY < startY){
		temp = stopY;
		stopY = startY;
		startY = temp;
	}

	if (PointInRect(startX, startY, 1, 1, TERM_ScreenWidth, TERM_ScreenHeight) && PointInRect(stopX, stopY, 1, 1, TERM_ScreenWidth, TERM_ScreenHeight)){
		// rect is fully on screen - simplified creation
		Locate(startX, startY);
		DECboxON;
		printf("l");					// top-left edge
		DEClineX(stopX - startX - 1);
		DECboxON;
		printf("k");
		
		Locate(stopX, startY + 1);
		DEClineY(stopY - startY - 1);
		DECboxON;
		printf("j");					// bottom-right edge
				
		Locate(stopX - 1, stopY);
		DEClineX(startX - stopX + 1);
		DECboxON;
		printf("m");					// bottom-left edge
				
		Locate(startX, stopY - 1);
		DEClineY(startY - stopY + 1);
	}
	else{
		// (parts of) the rect are out of screen
		
		// Horz top
		double spX1 = startX;
		double spY1 = startY;
		double epX1 = stopX;
		double epY1 = startY;

		// Vert right
		double spX2 = stopX;
		double spY2 = startY;
		double epX2 = stopX;
		double epY2 = stopY;

		// Horz bottom
		double spX3 = stopX;
		double spY3 = stopY;
		double epX3 = startX;
		double epY3 = stopY;

		// Vert left
		double spX4 = startX;
		double spY4 = stopY;
		double epX4 = startX;
		double epY4 = startY;

		int horzTop = LineInRect(&spX1, &spY1, &epX1, &epY1, 1, 1, TERM_ScreenWidth, TERM_ScreenHeight);
		int vertRight = LineInRect(&spX2, &spY2, &epX2, &epY2, 1, 1, TERM_ScreenWidth, TERM_ScreenHeight);
		int horzBot = LineInRect(&spX3, &spY3, &epX3, &epY3, 1, 1, TERM_ScreenWidth, TERM_ScreenHeight);
		int vertLeft = LineInRect(&spX4, &spY4, &epX4, &epY4, 1, 1, TERM_ScreenWidth, TERM_ScreenHeight);

		// round to 'int'...
		spX1 += 0.5; spX2 += 0.5; spX3 += 0.5; spX4 += 0.5; 
		spY1 += 0.5; spY2 += 0.5; spY3 += 0.5; spY4 += 0.5; 

		// Draw valid lines
		if (horzTop){
			Locate(spX1, spY1);
			DEClineX(epX1 - spX1 + 2);
		}
		if (vertRight){
			Locate(spX2, spY2);
			DEClineY(epY2 - spY2 + 2);
		}
		if (horzBot){
			Locate(spX3, spY3);
			DEClineX(epX3 - spX3);
		}
		if (vertLeft){
			Locate(spX4, spY4);
			DEClineY(epY4 - spY4 - 1);
		}

		// draw valid edges
		DECboxON;
		if (horzTop && vertLeft && (int)spX1 == startX && (int)spY1 == startY){
			// top-left edge
			Locate(startX, startY);
			printf("l");
		}
		if (horzTop && vertRight && (int)epX1 == stopX && (int)epY1 == startY){
			// top-right edge
			Locate(stopX, startY);
			printf("k");
		}
		if (horzBot && vertRight && (int)spX3 == stopX && (int)spY3 == stopY){
			// bottom-right edge
			Locate(stopX, stopY);
			printf("j");
		}
		if (horzBot && vertLeft && (int)spX4 == startX && (int)spY4 == stopY){
			// bottom left edge
			Locate(startX, stopY);
			printf("m");
		}
		DECboxOFF;
	}
}

// Clear Lines
void ClrLineA(int xS, int xE){
	// ClearLine from Start(xS) to End(xE)

	int x = 0;

	// Normalize  
	if (xS > xE){
		x = xS;
		xS = xE;
		xE = x;
	}
	// Line Width
	x = xE - xS + 1;

	ResFBU();
	LocateX(xS);
	printf("%*s", x, "");
}
void ClrLine(void) {
	// ClearLine
	printf("\x1B[2K");
}
void ClrLineL(void) {
	// ClearLine from cursor to Left
	printf("\x1B[1K");
}
void ClrLineR(void) {
	// ClearLine from cursor to Right
	printf("\x1B[K");
	// printf("%s0K", CSI);	// The '0' isn't needed.
}

// Clear Screens
void ClrScrA(int xS, int yS, int xE, int yE){
	// Area CLS from Start(xS/Ys) to End(xE/yE)
	
	if (yS == yE){
		// just one Line
		Locate(xS, yS);
		ClrLineA(xS, xE);
	}
	else{
		int x = 0;
		int y = 0;
		int i = 0;

		// Normalize  
		if (xS > xE){
			x = xS;
			xS = xE;
			xE = x;
		}
		if (yS > yE){
			y = yS;
			yS = yE;
			yE = y;
		}
		//Area Width / Height
		x = xE - xS + 1;
		y = yE - yS + 1;

		// Reset all colors
		ResFBU();

		for (i = 0; i < y; i++){
			Locate(xS, yS + i);
			printf("%*s", x, "");
		}
	}
}
void ClrScrL(void) {
	// Cls from cursor to Upper Left
	printf("\x1B[1J");
}
void ClrScrR(void) {
	// Cls from cursor to LowerRight
	printf("\x1B[J");
	// printf("%s0J", CSI);	// The '0' isn't needed.
}

// Reset To Default Colors
void ResFg(void) {
	ActTxtColor.fg.Color = 39; ActTxtColor.mode = 1; // ActTxtStyle.pColor = &ActTxtColor;
	printf("\x1B[39m");
}
void ResBg(void) {
	ActTxtColor.bg.Color = 49; ActTxtColor.mode = 1; // ActTxtStyle.pColor = &ActTxtColor;
	printf("\x1B[49m");
}
void ResUl(void) {
	ActTxtColor.ul.Color = 59; ActTxtColor.mode = 1; // ActTxtStyle.pColor = &ActTxtColor;
	printf("\x1B[59m");
}
void ResFB(void) {
	ResFg();
	ResBg();
}
void ResFBU(void) {
	ResFg();
	ResBg();
	ResUl();
}

// Set 24 bit Color
void SetFgRGB(int r, int g, int b) {
	// ActTxtStyle.pColor->fg.R = r; ActTxtStyle.pColor->fg.G = g; ActTxtStyle.pColor->fg.B = b; ActTxtStyle.pColor->mode = 3;
	ActTxtColor.fg.R = r; ActTxtColor.fg.G = g; ActTxtColor.fg.B = b;  ActTxtColor.mode = 3;
	printf("\x1B[38;2;%d;%d;%dm", r, g, b);
}
void SetBgRGB(int r, int g, int b) {
	// ActTxtStyle.pColor->bg.R = r; ActTxtStyle.pColor->bg.G = g; ActTxtStyle.pColor->bg.B = b; ActTxtStyle.pColor->mode = 3;
	ActTxtColor.bg.R = r; ActTxtColor.bg.G = g; ActTxtColor.bg.B = b;  ActTxtColor.mode = 3;
	printf("\x1B[48;2;%d;%d;%dm", r, g, b);
}
void SetUlRGB(int r, int g, int b) {
	// ActTxtStyle.pColor->ul.R = r; ActTxtStyle.pColor->ul.G = g; ActTxtStyle.pColor->ul.B = b; ActTxtStyle.pColor->mode = 3;
	ActTxtColor.ul.R = r; ActTxtColor.ul.G = g; ActTxtColor.ul.B = b; ActTxtColor.mode = 3;
	printf("\x1B[58;2;%d;%d;%dm", r, g, b);
}
void SetFBrgb(int fgR, int fgG, int fgB, int bgR, int bgG, int bgB) {
	SetFgRGB(fgR, fgG, fgB);
	SetBgRGB(bgR, bgG, bgB);
}
void SetFBUrgb(int fgR, int fgG, int fgB, int bgR, int bgG, int bgB, int ulR, int ulG, int ulB) {
	SetFgRGB(fgR, fgG, fgB);
	SetBgRGB(bgR, bgG, bgB);
	SetUlRGB(ulR, ulG, ulB);
}

// Set 256 Colors
void SetFg255(int c) {
	// ActTxtStyle.pColor->fg.Color = c; ActTxtStyle.pColor->mode = 2;
	ActTxtColor.fg.Color = c; ActTxtColor.mode = 2;
	printf("\x1B[38;5;%dm", c);
}
void SetBg255(int c) {
	// ActTxtStyle.pColor->bg.Color = c; ActTxtStyle.pColor->mode = 2;
	ActTxtColor.bg.Color = c; ActTxtColor.mode = 2;
	printf("\x1B[48;5;%dm", c);
}
void SetUl255(int c) {
	// ActTxtStyle.pColor->ul.Color = c; ActTxtStyle.pColor->mode = 2;
	ActTxtColor.ul.Color = c; ActTxtColor.mode = 2;
	printf("\x1B[58;5;%dm", c);
}
void SetFB255(int fg, int bg) {
	SetFg255(fg);
	SetBg255(bg);
}
void SetFBU255(int fg, int bg, int ul) {
	SetFg255(fg);
	SetBg255(bg);
	SetUl255(ul);
}

//Set 16 Colors
void SetFg16(int c){
	// ForeGround	
	if (!((c > 29 && c < 38) || (c > 89 && c < 98))){
		ResFg();
	}
	else{
		// ActTxtStyle.pColor->fg.Color = c; ActTxtStyle.pColor->mode = 0;
		ActTxtColor.fg.Color = c; ActTxtColor.mode = 0;
		printf("\x1B[%dm", c);
	}	
}
void SetBg16(int c) {	
	// BackGround
	if (!((c > 39 && c < 48) || (c > 99 && c < 108))){
		ResBg();
	}
	else{
		// ActTxtStyle.pColor->bg.Color = c; ActTxtStyle.pColor->mode = 0;
		ActTxtColor.bg.Color = c; ActTxtColor.mode = 0;
		printf("\x1B[%dm", c);
	}
}
void SetFB16(int fg, int bg) {
	SetFg16(fg);
	SetBg16(bg);
}

// TXT Styles - single commands
void TxtBold(int set) {
	ActTxtStyle.bold = set;
	ActTxtStyle.faint = 0;
	if (set) {
		// Set
		printf("\x1B[1m");
	}
	else {
		// Reset
		printf("\x1B[22m");
	}
}
void TxtFaint(int set) {
	ActTxtStyle.faint = set;
	ActTxtStyle.bold = 0;
	if (set) {
		// Set
		printf("\x1B[2m");
	}
	else {
		// Reset
		printf("\x1B[22m");
	}
}
void TxtItalic(int set) {
	ActTxtStyle.italic = set;
	if (set) {
		// Set
		printf("\x1B[3m");
	}
	else {
		// Reset
		printf("\x1B[23m");
	}
}
void TxtUnder(int set) {
	ActTxtStyle.underline = set;
	if (set) {
		// Set
		printf("\x1B[4m");
	}
	else {
		// Reset
		printf("\x1B[%sm", RUL);
	}
}
void TxtDblUnder(int set) {
	if (set) {
		// Set
		ActTxtStyle.underline = 2;
		printf("\x1B[21m");
	}
	else {
		// Reset
		TxtUnder(0);
	}
}
void TxtCurlUnder(int set) {
	if (set) {
		// Set
		ActTxtStyle.underline = 3;
		printf("\x1B[4:3m");
	}
	else {
		// Reset
		TxtUnder(0);
	}
}
void TxtDotUnder(int set) {
	if (set) {
		// Set
		ActTxtStyle.underline = 4;
		printf("\x1B[4:4m");
	}
	else {
		// Reset
		TxtUnder(0);
	}
}
void TxtDashUnder(int set) {
	if (set) {
		// Set
		ActTxtStyle.underline = 5;
		printf("\x1B[4:5m");
	}
	else {
		// Reset
		TxtUnder(0);
	}
}
void TxtDashDotUnder(int set) {
	// Undocumented DashDot - just a prognosis
	if (set) {
		// Set
		ActTxtStyle.underline = 6;
		printf("\x1B[4:6m");
	}
	else {
		// Reset
		TxtUnder(0);
	}
}
void TxtDblCurlUnder(int set) {
	// Undocumented DoubleCurl - just a prognosis
	if (set) {
		// Set
		ActTxtStyle.underline = 7;
		printf("\x1B[4:7m");
	}
	else {
		// Reset
		TxtUnder(0);
	}
}
void TxtDblDotUnder(int set) {
	// Undocumented DoubleDotted - just a prognosis
	if (set) {
		// Set
		ActTxtStyle.underline = 8;
		printf("\x1B[4:8m");
	}
	else {
		// Reset
		TxtUnder(0);
	}
}
void TxtDblDashUnder(int set) {
	// Undocumented DoubleDashed - just a prognosis
	if (set) {
		// Set
		ActTxtStyle.underline = 9;
		printf("\x1B[4:9m");
	}
	else {
		// Reset
		TxtUnder(0);
	}
}

void TxtResetBlink(void){
	printf("\x1B[25m");
}
void TxtBlink(int set) {
	ActTxtStyle.blink = set;
	ActTxtStyle.fast = 0;
	if (set) {
		// Set
		printf("\x1B[5m");
	}
	else {
		// Reset
		TxtResetBlink();
	}
}
void TxtFastBlink(int set) {
	ActTxtStyle.fast = set;
	ActTxtStyle.blink = 0;
	if (set) {
		// Set
		printf("\x1B[6m");
	}
	else {
		// Reset
		TxtResetBlink();
	}
}
void TxtReverse(int set) {
	ActTxtStyle.reverse = set;
	if (set) {
		// Set
		printf("\x1B[7m");
	}
	else {
		// Reset
		printf("\x1B[27m");
	}
}
void TxtInvisible(int set) {
	ActTxtStyle.invisible = set;
	if (set) {
		// Set
		printf("\x1B[8m");
	}
	else {
		// Reset
		printf("\x1B[28m");
	}
}
void TxtStrike(int set) {
	ActTxtStyle.strike = set;
	if (set) {
		// Set
		printf("\x1B[9m");
	}
	else {
		// Reset
		printf("\x1B[29m");
	}
}
void TxtResetScript(void){
	printf("\x1B[75m");
}
void TxtSuperscript(int set) {
	ActTxtStyle.superscript = set;
	ActTxtStyle.subscript = 0;
	if (set) {
		// Set
		printf("\x1B[73m");
	}
	else {
		// Reset
		TxtResetScript();
	}
}
void TxtSubscript(int set) {
	ActTxtStyle.subscript = set;
	ActTxtStyle.superscript = 0;
	if (set) {
		// Set
		printf("\x1B[74m");
	}
	else {
		// Reset
		TxtResetScript();
	}
}
void TxtProportional(int set){
	ActTxtStyle.proportional = set;
	if (set) {
		// Set
		printf("\x1B[26m");
	}
	else {
		// Reset
		printf("\x1B[50m");
	}
}
void TxtFramed(int set) {
	ActTxtStyle.framed = set;
	ActTxtStyle.encircled = 0;
	if (set) {
		// Set
		printf("\x1B[51m");
	}
	else {
		// Reset
		printf("\x1B[54m");
	}
}
void TxtEncircled(int set) {
	ActTxtStyle.encircled = set;
	ActTxtStyle.framed = 0;
	if (set) {
		// Set
		printf("\x1B[52m");
	}
	else {
		// Reset
		printf("\x1B[54m");
	}
}
void TxtOverline(int set) {
	ActTxtStyle.overline = set;
	if (set) {
		// Set
		printf("\x1B[53m");
	}
	else {
		// Reset
		printf("\x1B[55m");
	}
}

void TxtResetIdeo(void) {
	ActTxtStyle.ideo_right = 0; ActTxtStyle.ideo_dbl_right = 0;
	ActTxtStyle.ideo_left = 0; ActTxtStyle.ideo_dbl_left = 0;
	ActTxtStyle.ideo_stress = 0;
	printf("\x1B[65m");
}
void TxtIdeoRight(int set) {
	if (set){
		ActTxtStyle.ideo_right = 1;
		printf("\x1B[60m");
	}
	else{
		TxtResetIdeo();
	}
}
void TxtIdeoDblRight(int set) {
	if (set){
		ActTxtStyle.ideo_dbl_right = 1;
		printf("\x1B[61m");
	}
	else{
		TxtResetIdeo();
	}
}
void TxtIdeoLeft(int set) {
	if (set){
		ActTxtStyle.ideo_left = 1;
		printf("\x1B[62m");
	}
	else{
		TxtResetIdeo();
	}
}
void TxtIdeoDblLeft(int set) {
	if (set){
		ActTxtStyle.ideo_dbl_left = 1;
		printf("\x1B[63m");
	}
	else{
		TxtResetIdeo();
	}
}
void TxtIdeoStress(int set) {
	if (set){
		ActTxtStyle.ideo_stress = 1;
		printf("\x1B[64m");
	}
	else{
		TxtResetIdeo();
	}
}

void TxtResetDblTBW(void){
	ActTxtStyle.dbl_width = 0;
	ActTxtStyle.dbl_height = 0;
	printf("\x1B#5");
}
void TxtDblTop(int set) {
	// DoubleHeight + DoubleWidth Top-Part
	ActTxtStyle.dbl_height = set;
	if (set) {
		// Set
		printf("\x1B#3");
	}
	else {
		// Reset
		TxtResetDblTBW();
	}
}
void TxtDblBot(int set) {
	// DoubleHeight + DoubleWidth Bottom-Part
	ActTxtStyle.dbl_height = set;
	if (set) {
		// Set
		printf("\x1B#4");
	}
	else {
		// Reset
		TxtResetDblTBW();
	}
}
void TxtDblWidth(int set) {
	// DoubleWidth 
	ActTxtStyle.dbl_width = set;
	if (set) {
		// Set
		printf("\x1B#6");
	}
	else {
		// Reset
		TxtResetDblTBW();
	}
}

void TxtFont(int fnt) {

	// 10 = Default Font
	// 11 - 19 User Fonts @INVESTIGATE@
	// 20 = Fraktur

	if (fnt > 10){
		fnt = 10;
	}
	if (fnt < 0){
		fnt = 0;
	}
	
	ActTxtStyle.font = fnt;
	printf("\x1B[%dm", fnt + 10);
}

void SetColorStyle(EscColorSTRUCT *pColor, int set){

	if (set){
		switch (pColor->mode) {
		case 1:		// 16 (has no UnderLineColor)
			if (pColor->fg.Color != ActTxtColor.fg.Color) {
				if (!((pColor->fg.Color > 29 && pColor->fg.Color < 38) || (pColor->fg.Color > 89 && pColor->fg.Color < 98))){
					pColor->fg.Color = 39;
					ResFg();
				}
				else{
					SetFg16(pColor->fg.Color);
				}
			}
			if (pColor->bg.Color != ActTxtColor.bg.Color) {
				if (!((pColor->bg.Color > 39 && pColor->bg.Color < 48) || (pColor->bg.Color > 99 && pColor->bg.Color < 108))){
					pColor->bg.Color = 49;
					ResBg();
				}
				else{
					SetBg16(pColor->bg.Color);
				}
			}
			break;
		
		case 2:		// 255
			if (pColor->fg.Color != ActTxtColor.fg.Color) {
				SetFg255(pColor->fg.Color);
			}
			if (pColor->bg.Color != ActTxtColor.bg.Color) {
				SetBg255(pColor->bg.Color);
			}
			if (pColor->ul.Color != ActTxtColor.ul.Color) {
				//SetUl255(pColor->ul.Color);
			}
			break;
		
		case 3:		// RGB
			if (pColor->fg.Color != ActTxtColor.fg.Color) {
				SetFgRGB(pColor->fg.R, pColor->fg.G, pColor->fg.B);
			}
			if (pColor->bg.Color != ActTxtColor.bg.Color) {
				SetBgRGB(pColor->bg.R, pColor->bg.G, pColor->bg.B);
			}
			if (pColor->ul.Color != ActTxtColor.ul.Color) {
				SetUlRGB(pColor->ul.R, pColor->ul.G, pColor->ul.B);
			}
			break;
		}
	}
	else{
		memset(&ActTxtColor, 0, sizeof(ActTxtColor));
		ResFBU();
	}
}

// TXT Style - combined from Structure
void SetTxtStyle(EscStyleSTRUCT *pTxtStyle, int set) {

	// set == False -> Reset all to default

	if (set) {
		if (pTxtStyle->bold != ActTxtStyle.bold) {
			TxtBold(pTxtStyle->bold);
		}
		if (pTxtStyle->faint != ActTxtStyle.faint) {
			TxtFaint(pTxtStyle->faint);
		}
		if (pTxtStyle->italic != ActTxtStyle.italic) {
			TxtItalic(pTxtStyle->italic);
		}
		if (pTxtStyle->underline != ActTxtStyle.underline) {
			ActTxtStyle.underline = pTxtStyle->underline;
			switch (pTxtStyle->underline) {
			case 0: // None
			case 1: // Single
				TxtUnder(pTxtStyle->underline);
				break;
			case 2: // Double
				TxtDblUnder(1);
				break;
			case 3: // Curly
				TxtCurlUnder(1);
				break;
			case 4: // Dot
				TxtUnder(1);
				break;
			case 5: // Dash
				TxtDashUnder(1);
				break;
			case 6: // DashDot
				TxtDashDotUnder(1);
				break;
			case 7: // Double Curly
				TxtDblCurlUnder(1);
				break;
			case 8: // Double Dot
				TxtDblDotUnder(1);
				break;
			case 9: // Double Dash
				TxtDblDashUnder(1);
				break;
			default:
				// This would be an error !!
				pTxtStyle->underline = 0;
				TxtUnder(0);
				break;
			}
		}

		if (pTxtStyle->blink != ActTxtStyle.blink) {
			if (!pTxtStyle->fast){
				TxtBlink(pTxtStyle->blink);
			}			
		}
		if (pTxtStyle->fast != ActTxtStyle.fast) {
			TxtFastBlink(pTxtStyle->fast);
		}

		if (pTxtStyle->reverse != ActTxtStyle.reverse) {
			TxtReverse(pTxtStyle->reverse);
		}
		
		if (pTxtStyle->invisible != ActTxtStyle.invisible) {
			TxtInvisible(pTxtStyle->invisible);
		}

		if (pTxtStyle->strike != ActTxtStyle.strike) {
			TxtStrike(pTxtStyle->strike);
		}

		if (pTxtStyle->superscript != ActTxtStyle.superscript) {
			if (!pTxtStyle->subscript){
				TxtSuperscript(pTxtStyle->superscript);
			}			
		}
		if (pTxtStyle->subscript != ActTxtStyle.subscript) {
			TxtSubscript(pTxtStyle->subscript);
		}

		if (pTxtStyle->proportional != ActTxtStyle.proportional) {
			TxtProportional(pTxtStyle->proportional);
		}

		if (pTxtStyle->framed != ActTxtStyle.framed) {
			if (!pTxtStyle->encircled){
				TxtFramed(pTxtStyle->framed);
			}			
		}
		if (pTxtStyle->encircled != ActTxtStyle.encircled) {
			TxtEncircled(pTxtStyle->encircled);
		}

		if (pTxtStyle->overline != ActTxtStyle.overline) {
			TxtOverline(pTxtStyle->overline);
		}

		/*
		if ((pTxtStyle->ideo_right != ActTxtStyle.ideo_right) ||
			(pTxtStyle->ideo_dbl_right != ActTxtStyle.ideo_dbl_right) ||
			(pTxtStyle->ideo_left != ActTxtStyle.ideo_left) ||
			(pTxtStyle->ideo_dbl_left != ActTxtStyle.ideo_dbl_left) ||
			(pTxtStyle->ideo_stress != ActTxtStyle.ideo_stress)) {

			TxtResetIdeo();		// 1st... Reset them all...

			if (pTxtStyle->ideo_right) {
				TxtIdeoRight(1);
			}
			else if (pTxtStyle->ideo_dbl_right) {
				TxtIdeoDblRight(1);
			}
			else if (pTxtStyle->ideo_left) {
				TxtIdeoLeft(1);
			}
			else if (pTxtStyle->ideo_dbl_left) {
				TxtIdeoDblLeft(1);
			}
			else if (pTxtStyle->ideo_stress) {
				TxtIdeoStress(1);
			}
		}
		*/

		if (pTxtStyle->font != ActTxtStyle.font) {
			if (pTxtStyle->font > 10) {
				pTxtStyle->font = 10;
			}
			TxtFont(pTxtStyle->font);
		}

		// SetColorStyle(pTxtStyle->pColor, 1);
		
	}
	else {
		// Reset colors, fonts and styles to their defaults
		memset(&ActTxtStyle, 0, sizeof(ActTxtStyle));
		memset(&ActTxtColor, 0, sizeof(ActTxtColor));
		printf("\x1B[10;0;39;49;59m");
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
	<AnsiESC.h>		    	01.04.2022		                                Pit

BugID		Description									Found by / Date		Fixed by / Date


*/
