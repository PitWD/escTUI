#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "mathe.h"

#define STR_SMALL_SIZE 255
#define STR_MID_SIZE 1024

#if __WIN32__ || _MSC_VER || __WIN64__
    #define strncasecmp(str1, str2, len) _strnicmp(str1, str2, len)
#endif

void StrTrimWhiteSpacesLR(char *strIN, const int l, const int r){
    
    // L & R are bool to remove whitespaces
    // Left and/or Right

    // Initialize two pointers left and right
    char *pLeft = strIN;
    char *pRight = strIN + strlen(strIN) - 1;

    // Traverse string from both ends until a non-space character is found
    if (l){
        while (pRight > pLeft && (*pLeft == ' ' || *pLeft == '\t') || *pLeft == '\n' || *pLeft == '\r')
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
#define StrTrimWS_L(strIN) StrTrimWhiteSpacesLR(strIN,1,0)
#define StrTrimWS_R(strIN) StrTrimWhiteSpacesLR(strIN,0,1)
#define StrTrimWS(strIN) StrTrimWhiteSpacesLR(strIN,1,1)

void StrTrimCharsLR(char *strIN, const char c, const int l, const int r){
    
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
#define StrTrimChars_L(strIN, c) StrTrimCharsLR(strIN, c, 1, 0)
#define StrTrimChars_R(strIN, c) StrTrimCharsLR(strIN, c, 0, 1)
#define StrTrimChars_LR(strIN, c) StrTrimCharsLR(strIN, c, 1, 1)

void StrTrimCntLR(char *strIN, const int l, const int r){
    
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
#define StrTrimCnt_L(strIN, count) StrTrimCntLR(strIN, count, 0)
#define StrTrimCnt_R(strIN, count) StrTrimCntLR(strIN, 0, count)
#define StrTrimCnt_LR(strIN, left, right) StrTrimCntLR(strIN, left, right)

void StrTrimACharLR(char *strIN, const char c, const int l, const int r){
    
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
#define StrTrimAChar_L(strIN, c) StrTrimACharLR(strIN, c, 1, 0)
#define StrTrimAChar_R(strIN, c) StrTrimACharLR(strIN, c, 0, 1)
#define StrTrimAChar_LR(strIN, c) StrTrimACharLR(strIN, c, 1, 1)

int StrCommaToDot(char *strIN){
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

void StrTrimNonNumeric(char *strIN){
    // Trim on 1st non-numeric position
    for (int i = 0; i < strlen(strIN); i++){
        if ((strIN[i] < '0' || strIN[i] > '9') && strIN[i] != '.' && strIN[i] != '-'){
            strIN[i] = '\0';
        }
    }
}

int StrNormFloatString(char *strIN){

    char *pEnd;

    StrCommaToDot(strIN);
    StrTrimNonNumeric(strIN);
    
    double dNum = strtod(strIN, &pEnd);

    if (*pEnd == '\0'){

        sprintf(strIN, "%.8f", dNum);

        // Remove too much trailing zeros
        StrTrimAChar_R(strIN, '0');

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

void StrUcaseLen(char *strIN, const int len) { 
   // Convert 1st len chars to upper case

    if (len <= strlen(strIN)){
        for (int c = 0; c < len; c++){
            strIN[c] = toupper(strIN[c]);
        }        
    }
}
#define StrUcase(strIN) StrUcaseLen(strIN, strlen(strIN))

void StrLcaseLen(char *strIN, const int len) {
   
   // Convert 1st len chars to lower case

    if (len <= strlen(strIN)){
        for (int c = 0; c < len; c++){
            strIN[c] = tolower(strIN[c]);
        }        
    }
}
#define StrLcase(strIN) StrLcaseLen(strIN, strlen(strIN))

long StrBinToLong(const char *strValue, const int bits){
 
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
#define StrBin2Long(strValue) StrBinToLong(strValue, 0)
#define StrBin2Int(strValue) (int)StrBinToLong(strValue, 8 * sizeof(int))
#define StrBin2Byte(strValue) (unsigned char)StrBinToLong(strValue, 8)

void StrLongToBin(const unsigned long lValue, char *strResult, const int bits){

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
#define StrLong2Bin(lValue, strResult) StrLongToBin((unsigned long)lValue, strResult, 0)
#define StrInt2Bin(iValue, strResult) StrLongToBin((unsigned long)iValue, strResult, 8 * sizeof(int))
#define StrByte2Bin(bValue, strResult) StrLongToBin((unsigned long)bValue, strResult, 8)

void StrPrintChars(char charIN, int count){
    while (count--){
        printf("%c", charIN);
    }
}
#define StrPrintSpaces(count) StrPrintChars(' ', count)

void StrPrintCentered(char *strIN, int len){

    int spaces = (len - strlen(strIN)) / 2;
    StrPrintSpaces(spaces);
    printf("%s", strIN);
    StrPrintSpaces(len - strlen(strIN) - spaces);

}