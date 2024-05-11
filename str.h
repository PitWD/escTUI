#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "mathe.h"

#define STR_XS_SIZE 128
#define STR_SMALL_SIZE 256
#define STR_MID_SIZE 512
#define STR_LARGE_SIZE 1024
#define STR_XL_SIZE 2048

FILE *LOG_Errors = NULL;
FILE *LOG_Debug = NULL;

#if __WIN32__ || _MSC_VER || __WIN64__
    #define strncasecmp(str1, str2, len) _strnicmp(str1, str2, len)
#endif

void STRtrimWhiteSpacesLR(char *strIN, const int l, const int r){
    
    // L & R are bool to remove whitespaces
    // Left and/or Right

    // Initialize two pointers left and right
    char *pLeft = strIN;
    char *pRight = strIN + strlen(strIN) - 1;

    // Traverse string from both ends until a non-space character is found
    if (l){
        while (pRight > pLeft && (*pLeft == ' ' || *pLeft == '\t' || *pLeft == '\n' || *pLeft == '\r'))
            pLeft++;
    }
    
    if (r){
        while (pRight > pLeft && (*pRight == ' ' || *pRight == '\t' || *pRight == '\n' || *pRight == '\r'))
            pRight--;
    }
    
    pRight++;
    memmove(strIN, pLeft, pRight - pLeft);
    strIN[pRight - pLeft] = '\0';
}
#define STRtrimWS_L(strIN) STRtrimWhiteSpacesLR(strIN,1,0)
#define STRtrimWS_R(strIN) STRtrimWhiteSpacesLR(strIN,0,1)
#define STRtrimWS(strIN) STRtrimWhiteSpacesLR(strIN,1,1)

void STRtrimCharsLR(char *strIN, const char c, const int l, const int r){
    
    // L & R are bool to remove all chars, until 1st is != c
    // Left and/or Right

    // Initialize two pointers left and right
    char *pLeft = strIN;
    char *pRight = strIN + strlen(strIN) - 1;

    if (l){
        // Remove all chars left of c
        pLeft = strchr(strIN, c);
        if (pLeft == NULL){
            pLeft = strIN;
        }
    }

    if (r){
        while (pRight > pLeft && (*pRight != c))
            pRight--;
    }

    pRight++;
    memmove(strIN, pLeft, pRight - pLeft);
    strIN[pRight - pLeft] = '\0';
}
#define STRtrimChars_L(strIN, c) STRtrimCharsLR(strIN, c, 1, 0)
#define STRtrimChars_R(strIN, c) STRtrimCharsLR(strIN, c, 0, 1)
#define STRtrimChars_LR(strIN, c) STRtrimCharsLR(strIN, c, 1, 1)

void STRtrimCntLR(char *strIN, const int l, const int r){
    
    // Remove l chars from the left and r from the right

    // len & pointer left 
    int i = strlen(strIN);
    char *pLeft = strIN;
 
    if (i > l){
        pLeft += l;
        i -= l;
    }

    if (i > r){
        i -= r;
    }
    
    memmove(strIN, pLeft, i);
    strIN[i] = '\0';
}
#define STRtrimCnt_L(strIN, count) STRtrimCntLR(strIN, count, 0)
#define STRtrimCnt_R(strIN, count) STRtrimCntLR(strIN, 0, count)
#define STRtrimCnt_LR(strIN, left, right) STRtrimCntLR(strIN, left, right)

void STRtrimACharLR(char *strIN, const char c, const int l, const int r){
    
    // L & R are bool to remove all chars, as long they're without a break == c
    // Left and/or Right

    // Initialize two pointers left and right
    char *pLeft = strIN;
    char *pRight = strIN + strlen(strIN) - 1;

    if (l){
        while (pRight > pLeft && (*pLeft == c))
            pLeft++;
    }

    if (r){
        while (pRight > pLeft && (*pRight == c))
            pRight--;
    }

    pRight++;
    memmove(strIN, pLeft, pRight - pLeft);
    strIN[pRight - pLeft] = '\0';
}
#define STRtrimAChar_L(strIN, c) STRtrimACharLR(strIN, c, 1, 0)
#define STRtrimAChar_R(strIN, c) STRtrimACharLR(strIN, c, 0, 1)
#define STRtrimAChar_LR(strIN, c) STRtrimACharLR(strIN, c, 1, 1)

int STRcommaToDot(char *strIN){
    // Make 1st comma to dot...
    char *pComma = strchr(strIN, ',');
    if (pComma != NULL){
        *pComma = '.';
    }
    // Is there a dot ?
    if (strchr(strIN, '.') != NULL){
        return 1;
    }

    return 0;
}

void STRtrimNonNumeric(char *strIN){
    // Trim on 1st non-numeric position
    for (int i = 0; i < (int)strlen(strIN); i++){
        if ((strIN[i] < '0' || strIN[i] > '9') && strIN[i] != '.' && strIN[i] != '-'){
            strIN[i] = '\0';
        }
    }
}

int STRnormFloatString(char *strIN){

    char *pEnd;

    STRcommaToDot(strIN);
    STRtrimNonNumeric(strIN);
    
    double dNum = strtod(strIN, &pEnd);

    if (*pEnd == '\0'){

        sprintf(strIN, "%.8f", dNum);

        // Remove too much trailing zeros
        STRtrimAChar_R(strIN, '0');

        // Keep at least one zero
        int len = strlen(strIN);
        if (strIN[len - 1] == '.'){
            strIN[len] = '0';
            strIN[len + 1] = '\0';
        }
        
        return 1;
    }
    return 0;
}

void STRucaseLen(char *strIN, const int len) { 
   // Convert 1st len chars to upper case

    if (len <= (int)strlen(strIN)){
        for (int c = 0; c < len; c++){
            strIN[c] = toupper(strIN[c]);
        }        
    }
}
#define STRucase(strIN) STRucaseLen(strIN, strlen(strIN))

void STRlcaseLen(char *strIN, const int len) {
   
   // Convert 1st len chars to lower case

    if (len <= (int)strlen(strIN)){
        for (int c = 0; c < len; c++){
            strIN[c] = tolower(strIN[c]);
        }        
    }
}
#define STRlcase(strIN) STRlcaseLen(strIN, strlen(strIN))

long STRbinToLong(const char *strValue, const int bits){
 
    int len = strlen(strValue) - 1;

    int maxBits = 8 * sizeof(long);

    if (bits && (bits < maxBits)){
        maxBits = bits;
    }

    int preFix = 0;
    if(strncasecmp(strValue, "0b", 2) == 0 || strncasecmp(strValue, "&b", 2) == 0){
        preFix = 2;
    }
    // suffix
    if(strncasecmp(&strValue[len], "b", 1) == 0){
        len--;
    }

    long r = 0;

    for(int i = len; i >= preFix && len - i < maxBits; i--){
        r += (long)(strValue[i] - '0') << (long)(len - i);
    }

    return r;
}
#define STRbin2Long(strValue) STRbinToLong(strValue, 0)
#define STRbin2Int(strValue) (int)STRbinToLong(strValue, 8 * sizeof(int))
#define STRbin2Byte(strValue) (unsigned char)STRbinToLong(strValue, 8)

void STRlongToBin(const unsigned long lValue, char *strResult, const int bits){

    // MaxBits by OS
    int maxBits = 8 * sizeof(unsigned long) - 1;

    if (bits){
        if ((bits < maxBits)){
            maxBits = bits - 1;
        }        
    }
    else{
        // Reduce Bits to least needed Bytes
        if (lValue == 0){
            maxBits = 7;
        }
        else{
            int minBytes = 0;
            unsigned long value = lValue;
            while (value != 0ul) {
                value = value >> 8ul;
                minBytes++;
            }
            maxBits = minBytes * 8 -1;
        }    
    }    

    strcpy(strResult, "0b");

    for(int i = maxBits; i >= 0; i--){
        strResult[maxBits - i + 2] = (lValue & (1ul << i)) ? '1' : '0';
    }
    
    strResult[maxBits + 3] = '\0';

}
#define STRlong2Bin(lValue, strResult) STRlongToBin((unsigned long)lValue, strResult, 0)
#define STRint2Bin(iValue, strResult) STRlongToBin((unsigned long)iValue, strResult, 8 * sizeof(int))
#define STRbyte2Bin(bValue, strResult) STRlongToBin((unsigned long)bValue, strResult, 8)

void STRprintChars(char charIN, int count){
    while (count--){
        printf("%c", charIN);
    }
}
#define STRprintSpaces(count) STRprintChars(' ', count)

void STRprintCentered(char *strIN, int len){

    int spaces = (len - strlen(strIN)) / 2;
    STRprintSpaces(spaces);
    printf("%s", strIN);
    STRprintSpaces(len - strlen(strIN) - spaces);

}