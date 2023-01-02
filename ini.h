#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define STR_SMALL_SIZE 255
#define STR_MID_SIZE 1024
#define INI_TAB_LEN 4

void IniTrimRemark (char *strIN){
    
    // Remove all trailing text after
    // the first non "" encapsulated #

    /*
    // From back need one int less, but is slower!

    int i = strlen(strIN) - 1;
    int inQuote = 0;


    for (i; i >= 0; i--){
        if (strIN[i] == '#' && !inQuote)
            strIN[i] = '\0';
            // Don't break to find the very first !
        else if (strIN[i] == '"')
            inQuote = !inQuote;
    }
    */
    
    int l = strlen(strIN) - 1; 
    int i = 0;
    int inQuote = 0;

    for (i = 0; i <= l; i++){
        if (strIN[i] == '#' && !inQuote){
            strIN[i] = '\0';
            break;
        }
        else if (strIN[i] == '"')
            inQuote = !inQuote;
    }

}

int IniGetRemark (char *strIN){
    
    // Returns Remark with leading whitespaces
    // "Value = 1234       # MyRemark"
    // results in:
    // "       # MyRemark"
    
    int l = strlen(strIN) - 1; 
    int i = 0;
    int inQuote = 0;
    int nonSpace = 0;

    for (i = 0; i < l; i++){
        if (!inQuote){
            if (strIN[i] == '#'){
                // Remark found
                memmove(strIN, strIN + nonSpace + 1, l - nonSpace);
                strIN[l - nonSpace] = '\0';
                return 1;
            }
            else if (strIN[i] != ' ' && strIN[i] != '\t'){
                // Last Non-Whitespace
                nonSpace = i;
            }  
        }
        if (strIN[i] == '"')
            inQuote = !inQuote;
    }
    // No remark in line
    strIN[0] = '\0';
    return 0;
}

int IniCountFrontSpaces (char *strIN){

    int l = strlen(strIN);
    int r = 0;

    for (int i = 0; i < l; i++){
        if (strIN[i] == ' '){
            r++;
        }
        else if (strIN[i] == '\t'){
            r += INI_TAB_LEN;
        }
        else{
            return r;
        }
    }
    return r;
}

void IniTrimNonNumeric(char *strIN){

    // Removes from Right all non-numeric and non-hex characters until the first
    // decimal or hex is found
    // "Value = &h234af0 some BS"
    // results in:
    // "Value = &h234af0"

    // int i = strlen(strIN) - 1;
    char c = 0;

    for (int i = strlen(strIN) - 1; i >= 0; i--){
        c = strIN[i];
        if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
            break;
        else
            strIN[i] = '\0';
    }
}

int IniIsNonNumeric(const char *strIN){

    // With the first non "0-9", ".", "," it's non-numeric
    char c = 0;

    for (int i = strlen(strIN) - 1; i >= 0; i--){
        c = strIN[i];
        if ((c <= '0' || c >= '9') && (c != ',' && c != '.')){
            return 1;
        }
    }
    return 0;

}

void IniTrimWhiteSpacesLR(char *strIN, const int l, const int r){
    
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
#define IniTrimWS_L(strIN) IniTrimWhiteSpacesLR(strIN,1,0)
#define IniTrimWS_R(strIN) IniTrimWhiteSpacesLR(strIN,0,1)
#define IniTrimWS(strIN) IniTrimWhiteSpacesLR(strIN,1,1)

void IniTrimCharsLR(char *strIN, const char c, const int l, const int r){
    
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
#define IniTrimChars_L(strIN, c) IniTrimCharsLR(strIN, c, 1, 0)
#define IniTrimChars_R(strIN, c) IniTrimCharsLR(strIN, c, 0, 1)
#define IniTrimChars_LR(strIN, c) IniTrimCharsLR(strIN, c, 1, 1)

void IniTrimCntLR(char *strIN, const int l, const int r){
    
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
#define IniTrimCnt_L(strIN, count) IniTrimCntLR(strIN, count, 0)
#define IniTrimCnt_R(strIN, count) IniTrimCntLR(strIN, 0, count)
#define IniTrimCnt_LR(strIN, left, right) IniTrimCntLR(strIN, left, right)

void IniTrimACharLR(char *strIN, const char c, const int l, const int r){
    
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
#define IniTrimAChar_L(strIN, c) IniTrimACharLR(strIN, c, 1, 0)
#define IniTrimAChar_R(strIN, c) IniTrimACharLR(strIN, c, 0, 1)
#define IniTrimAChar_LR(strIN, c) IniTrimACharLR(strIN, c, 1, 1)

int IniCommaToDot(char *strIN){
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

int IniNormDoubleString(char *strIN){

    char *pEnd;

    double dNum = strtod(strIN, &pEnd);

    if (*pEnd == '\0'){

        sprintf(strIN, "%.8f", dNum);

        // Remove too much trailing zeros
        IniTrimAChar_R(strIN, '0');

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

int IniGetTokens(char *strIN, char **tokens){
    
    // Split string like "Main.Sub1.Sub1.Value" into **tokens
    // like:    [Main]
    //          [.Sub1]
    //          [..Sub2]
    //          Value
    //
    // Returns the count of tokens
    
    int count = 0; 
    int i = 0;

    char *token = strtok(strIN, ".");
 
    while (token != NULL){

        // Size actual Token Length + "[.]" 
        tokens[count] = malloc(strlen(token) + 4 + count);
        tokens[count][0] = '[';
        tokens[count][1] = '\0';

        for (i = 0; i < count; i++){
            tokens[count][i + 1] = '.';
        }
        if (count){
            tokens[count][i + 1] = '\0';
        }

        strcat(tokens[count], token);
        token = strtok(NULL, ".");

        i = strlen(tokens[count]);
        tokens[count][i] = ']';
        tokens[count][i + 1] = '\0';

        count++;
    }
    
    count--;

    // remove "[." and "]" from the Value token
    if(count){
        IniTrimCnt_LR(tokens[count], count + 1, 1);
    }
    count++;
    return count;
}

void IniUcaseLen(char *strIN, const int len) {
   
   // Convert 1st len chars to upper case

    if (len <= strlen(strIN)){
        for (int c = 0; c < len; c++){
            strIN[c] = toupper(strIN[c]);
        }        
    }
}
#define IniUcase(strIN) IniUcaseLen(strIN, strlen(strIN))

void IniLcaseLen(char *strIN, const int len) {
   
   // Convert 1st len chars to lower case

    if (len <= strlen(strIN)){
        for (int c = 0; c < len; c++){
            strIN[c] = tolower(strIN[c]);
        }        
    }
}
#define IniLcase(strIN) IniLcaseLen(strIN, strlen(strIN))

int IniFindValueLineNr(const char *fileName, char *strSearch){

    //  [Global]
    //      [.Sub1]
    //          [..SubSub]
    //              Value = 1234
    //      [.Sub2]
    //          [..SubSub]
    //              Value = 1234

    // With a strSearch like "global.sub1.subsub.value"
    // we get a return of
    // 4 
    // for the 4th Line... containing the searched value...

    // Returns  0 = Value does not exist
    //              strSearch contains ("" encapsulated and ':' separated)
    //                  index of invalid token
    //                  index of broken line in file (always 0)
    //         -1 = File Error
    //         -2 = Broken Token
    //              strSearch contains ("" encapsulated and ':' separated)
    //                  index of invalid token
    //                  index of broken line in file
    //         >0 = Line with Value
    //              strSearch contains the raw line

    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        // File-Error
        return -1;
    }

    int r = 0;
    int cntLine = 0;

    // Tokens
    char *strTokens[STR_SMALL_SIZE] = {NULL};  
    int cntTokens = IniGetTokens(strSearch, strTokens) - 1;
    int actToken = 0;
    int isToken = 0;

    char strIN[STR_SMALL_SIZE];
    while (fgets(strIN, STR_SMALL_SIZE, file) != NULL) {

        cntLine++;

        // Save original line for return
        strcpy(strSearch, strIN);

        // Trim Left Whitespaces
        IniTrimWS_L(strIN);

        // Check if line is below actual token-level
        if (strIN[0] == '['){
            if (actToken){
                if (strIN[actToken] != '.'){
                    // Broken Token
                    r = -2;
                    sprintf(strSearch, "\"%d:%d\"", actToken, cntLine);
                    break;
                }
            }         
        }

        int i = strlen(strTokens[actToken]);

        // Check if line starts with actual token
        if(strncasecmp(strIN, strTokens[actToken], i) == 0){
            // Prevent that Token in File is longer (3rd case is just for [Main]-Level)
            if ((strIN[i] == ' ')  || (strIN[i] == '=') || (strlen(strIN)-1 == i)){
                // Token has the right length

                if (actToken == cntTokens){
                    // It's the Value-Token
                    r = cntLine;
                    break;
                }
                else{
                    actToken++;
                }                        
            }               
        }
    }

    if (!r){
        sprintf(strSearch, "\"%d:0\"", actToken);
    }
    
    fclose(file);

    cntTokens++;
    for (actToken = 0; actToken < cntTokens; actToken++){
        free(strTokens[actToken]);
    }

    return r;
} 

int IniInsertReplaceLine (const char *fileName, char *strIN, const int linePos, const int insert){
    
    // Copies fpRead Line for Line to fpWrite
    // When linePos is reached
    //  strIN gets inserted as line (!! with attached '\n' !!)
    // If insert is set
    //  the original line on linePos gets copied on linePos++
    // If insert is not set
    //  the original line on linePos gets deleted
    //
    // Returns  -1 fpRead File Error
    //          -2 fpWrite File Error (temp - file)
    //          -3 rename File Error
    //          -4 delete Temp-File Error
    //         >=0 copied lines

    FILE *fpRead; 
    FILE *fpWrite; 
    
    char buffer[STR_SMALL_SIZE];
    int actLine = 0;

    // Open the file in read mode
    fpRead = fopen(fileName, "r");
    if (fpRead == NULL) {
        return -1;
    }

    // TempFile
    sprintf(buffer, "%s.tmp", fileName);
    fpWrite = fopen(buffer, "w");
    if (fpWrite == NULL) {
        return -2;
    }

    // Copy the contents of the file to a temporary buffer
    while (fgets(buffer, STR_SMALL_SIZE, fpRead) != NULL) {
        actLine++;

        // Insert the string into the given line
        if (actLine == linePos){ 
            fprintf(fpWrite, "%s\n", strIN);
            if (insert){
                // Preserve original Line
                fprintf(fpWrite, "%s", buffer);
            }
        }    
        else{
            // Copy the existing line to the file
            fprintf(fpWrite, "%s", buffer);
        }
    }

    // Close the files
    fclose(fpRead);
    fclose(fpWrite);

    // Replace the original file with the temporary file
   sprintf(buffer, "%s.tmp", fileName);
    if (rename(buffer, fileName) != 0) {
        return -3;
    }
    // Delete Temp File
    if (remove(buffer) != -1) {
        return -4;
    }

    return actLine;
}
#define IniInsertLine(fileName, strIN, linePos) IniInsertReplaceLine(fileName, strIN, linePos, 1)
#define IniReplaceLine(fileName, strIN, linePos) IniInsertReplaceLine(fileName, strIN, linePos, 0)

int IniSetTypeToValue(char *strValue, const int valType){

    // Function converts strValue into a value of valType
    // and converts the value than back into a normalized string
    
    // valType  = 0     as it is
    //          = 1     int
    //          = 2     float
    //          = 3     hex
    //          = 4     as it is but embedded in ""
    //          = 5     boolean

    // most common human and OS errors get fixed
    // no case sensitivity


    // For the case float has "," instead of "." as delimiter
    char strValue2[strlen(strValue) + 1];

    char *pEnd;

    long lNum = 0;
    double dNum = 0;
    #define hNum lNum
    #define bNum lNum

    int r = 0;

    switch (valType){
    case 1:
        // as int
        lNum = strtol(strValue, &pEnd, 10);   
        if (*pEnd == '\0'){
            sprintf(strValue, "%ld", lNum);
            r = 1;
        }
        break;
    case 2:
        // as float
        // Make 1st comma to dot...
        IniCommaToDot(strValue);
        if(IniNormDoubleString(strValue)){
            r = 2;
        }
    case 3:
        // as hex
        if(strncasecmp(strValue, "&h", 2) == 0){
            hNum = strtol(&strValue[2], &pEnd, 16);
        }
        else{
            hNum = strtol(strValue, &pEnd, 16);
        }    
        if (*pEnd == '\0'){
            sprintf(strValue, "0x%lX", hNum);
            r = 3;
        }
        break;
    case 5:
        // as bool
        if(strncasecmp(strValue, "true", 4) == 0){
            // True
            bNum = 1;
        }
        else if(strncasecmp(strValue, "false", 5) == 0){
            // False
        }
        else{
            // from number
            bNum = strtol(strValue, &pEnd, 10);
        }
        if(bNum){
            // True
            sprintf(strValue, "true");
        }
        else{
            // False
            sprintf(strValue, "false");
        }
        r = 5;
        break;
    case 4:
        // as text embedded in ""
        sprintf(strValue2, "\"%s\"", strValue);
        strcpy(strValue, strValue2);
        r = 4;
        break;
    default:
        // Untouched
        break;
    }

    return r;
}

int IniGetTypeFromValue(char *strValue){

    char *pEnd;

    long lNum = 0;
    double dNum = 0;

    char strIN[strlen(strValue) + 16];
    strcpy(strIN, strValue);

    int r = 0;

    // Remove 1st and last " from Text if they're present
    if (strIN[0] == '"'){
        // Value is a text

        // Remove leading '"'
        IniTrimCnt_L(strIN,1);

        if (strIN[strlen(strIN) - 1] == '"'){
            // Text was fully encapsulated
            IniTrimCnt_R(strIN,1);
        }
        else{
            // Don't do this !
        }
        
        strcpy(strValue, strIN);
        r = 4;
    }
    else{
        // Value is a number or True or False or hex or (not embedded text - don't do this !)

        // as return for bad formatted (not "" - embedded text) - three cases following
        strcpy(strValue, strIN);

        if(strncasecmp(strIN, "true", 4) == 0){
            // True
            sprintf(strValue, "true");
            r = 2;
        }
        else if(strncasecmp(strIN, "false", 5) == 0){
            // False
            sprintf(strValue, "false");
            r = 2;
        }
        else if(strncasecmp(strIN, "0x", 2) == 0 || strncasecmp(strIN, "&h", 2) == 0){
            // hex
            // remove all non-numeric trailing characters
            IniTrimNonNumeric(strIN);
            lNum = strtol(&strIN[2], &pEnd, 16);
            if (*pEnd == '\0'){
                sprintf(strValue, "%ld", lNum);
                r = 5;
            }   
        }
        else if (IniIsNonNumeric(strIN)){
            // Bad formatted (not "" - embedded text)
        }
        else{
            // Number

            r = 3;                

            // Is there a dot ?
            if (IniCommaToDot(strIN)){
                r++;
            }

            // Int or Float
            if (r == 4){
                // float
                if (!IniNormDoubleString(strIN)){
                    r = 0;
                }
            }
            else{
                // int
                lNum = strtol(strIN, &pEnd, 10);
                if (*pEnd == '\0'){
                    sprintf(strIN, "%ld", lNum);
                }
                else{
                    r = 0;
                }
            }                
        }                    
    }
    return r;   
}

int IniChangeValueLine (char *strIN, const char *strValue, const int valType){

    // a string like:

    // "             Value = 1234     # MyRemark"
    // gets splitted into
    // "             Value =" (strPreVal)
    // and
    // "     # MyRemark" (strRemark)

    // when we replace "1234" with strVal
    // we try to preserve the position of the "#"
    // if strIn is too long, we place it on a tab
    //

    // Returns  -1 Overflow return string
    //           0 text as it is success and when type conversion failed
    //           1 int
    //           2 float
    //           3 hex
    //           4 text embedded in ""
    //           5 bool as true/false

    int r = 0;

    // PreValue - part
    char strPreVal[STR_SMALL_SIZE];
    strcpy(strPreVal, strIN);
    IniTrimRemark(strPreVal);
    IniTrimChars_R(strPreVal, '=');

    // Copy & Check & Format strValue
    char strValNew[strlen(strValue) + 16];
    strcpy(strValNew, strValue);
    r = IniSetTypeToValue(strValNew, valType);
    
    // Remark - part
    char strRemark[STR_SMALL_SIZE];
    strcpy(strRemark, strIN);
    IniGetRemark(strRemark);
 
    // Check and (re)place Remark
    if (strlen(strRemark)){
        // Line contains a remark

        int lenDiff = strlen(strValNew) - (strlen(strIN) - strlen(strPreVal) - strlen(strRemark)) + 1;

        if (lenDiff > 0){
            // New Value is longer than the old one
            // check if the longer value fit's into the leading spaces of strRemark

            // Count spaces (respecting tabs)
            int lenSpaces = IniCountFrontSpaces(strRemark);
            // Remove all spaces
            IniTrimWS(strRemark);

            if (lenSpaces < lenDiff){
                // Missing space in total
                // add missing spaces (Tab adjusted)
                sprintf(strIN, "%*c%s", INI_TAB_LEN - ((lenDiff - lenSpaces) % INI_TAB_LEN), ' ', strRemark);
            }
            else if (lenSpaces > lenDiff){
                // Enough spaces
                // Add back the needed spaces
                sprintf(strIN, "%*c%s", lenSpaces - lenDiff, ' ', strRemark);
            }
            else{
                // Like spaces
                // Add a 'TAB'
                sprintf(strIN, "%*c%s", INI_TAB_LEN, ' ', strRemark);
            } 
        }
        else if (lenDiff < 0){
            // New Value is shorter than the old one
            // Add spaces
            sprintf(strIN, "%*c%s", -lenDiff, ' ', strRemark);
        }
        else{
            // Same length
            strcpy(strIN, strRemark);
        }
        strcpy(strRemark, strIN);
    }

    if ((strlen(strPreVal) + strlen(strValNew) + strlen(strRemark)) < STR_SMALL_SIZE){
        // Join them together
        sprintf(strIN, "%s %s%s", strPreVal, strValNew, strRemark);
        return r;
    }
    else{
        // String would been longer than STR_SMALL_SIZE
        strIN[0] = '\0';
        return -1;
    }
}

int IniCreateMissingValue(const char *fileName, const char *strSearch, const char *strValue, const int typValue, const int missingToken, int insertLine){

    // We already know from IniGetValue() or IniSetValue() that
    // strSearch (e.g. "Main.Sub.SubSub.Value") doesn't exist (fully) in file
    // missingToken =   index of missed token (0 = main / 3 = Value)
    // insertLine   =   where the missed stuff need to get inserted

    /*  strValue    =   the value to write
        typValue    =   0 = as it is
                        1 = Value is a text
                        2 = Value is a bool
                        3 = Value is an int
                        4 = Value is a float
                        5 = Value is a hex
    */

   // Returns   0 = written as it is
   //          -1 = File Error
   //          -2 = Temp File Error
   //          -3 = Rename File Error
   //          -4 = Del Temp File Error   
   //          >0 = type of written value

    int r = 1;
    int i = 0;
    char strWork[STR_SMALL_SIZE];
    strcpy(strWork, strSearch);

    // Get formatted TokenList
    char *tokens[STR_SMALL_SIZE] = {NULL};
    int cntToken = IniGetTokens(strWork, tokens);

    // Is the whole token-value-chain missing?
    if (!insertLine){
        // Search for 1st non-empty and non-remark line
        FILE *file = fopen(fileName, "r");
        if (file == NULL) {
            // File-Error
            r = -1;
        }
        while (fgets(strWork, STR_SMALL_SIZE, file) != NULL){

            insertLine++;

            // Remove Remark
            IniTrimRemark(strWork);
            // Remove white-spaces
            IniTrimWS(strWork);

            if (strlen(strWork)){
                // Line found
                break; 
            }
        }
        fclose(file);
    }
    
    if(r > 0){
        // Run missing tokens
        for (i = missingToken; i < cntToken - 1; i++){            
            sprintf(strWork, "%*c%s", i * INI_TAB_LEN, ' ', tokens[i]);
            if (!i){
                // One trailing space too much.
                IniTrimCnt_L(strWork, 1);
            }
            r = IniInsertLine(fileName, strWork, insertLine);
            insertLine++;
        }
        
        // Finally the value
        if (r > 0){
            sprintf(strWork, "%*c%s = %s", i * INI_TAB_LEN, ' ', tokens[i], strValue);
            // Check & normalize value
            r = IniChangeValueLine(strWork, strValue, typValue);
            if (r > -1){
                IniInsertLine(fileName, strWork, insertLine);
            }
        }
    }

    // Free the allocated memory
    for (i = 0; i < cntToken + 1; i++){
        free(tokens[i]);
    }

    return r;

} 

int IniGetValue(const char *fileName, const char *strSearch, const char *strDefault, char *strReturn){

    // Returns  0 = Value is "as it is"
    //         -1 = File Error.
    //         -2 = Broken Token (not anymore - jumps now into IniCreateMissingValue() )
    //          1 = Value is a text
    //          2 = Value is a bool
    //          3 = Value is an int
    //          4 = Value is a float
    //          5 = Value is a hex
    //
    // strSearch, returns from a line like:
    // "          Value = 123,456  # MyRemark"
    // just the Value:
    // "123.456"
    // "," & "." are seen as valid decimal points

    // Working string
    strcpy(strReturn, strSearch);

    int cntLine = IniFindValueLineNr(fileName, strReturn);
    int r = 0;
    char *pEnd;

    if (cntLine > 0){
        // File and Search exist

        // Remove remarks
        IniTrimRemark(strReturn);

        // Remove all chars left of "="
        IniTrimChars_L(strReturn, '=');

        // Remove equal
        IniTrimCnt_L(strReturn, 1);

        // Trim whitespaces
        IniTrimWS(strReturn);

        // get type of value and normalize value
        r = IniGetTypeFromValue(strReturn);

    }
    else if (cntLine == 0 || cntLine == -2){
        // Value / Token does not exist

        // strReturn    contains (""-embedded and :-separated)
        //              index of 1st missing token
        //              index of broken line in file        
        IniTrimCnt_LR(strReturn, 1, 1);
        int missingToken = strtol(strReturn, &pEnd, 10);
        int insertLine = strtol(strchr(strReturn, ':') + 1, &pEnd, 10);

        r = IniCreateMissingValue(fileName, strSearch, strDefault, 0, missingToken, insertLine);
        strcpy(strReturn, strDefault);
    }
    else{
        // FileError
    }

    return r;     
}

int IniSetValue(const char *fileName, const char *strSearch, const char *strValue, const int typValue){

    // Copy for the case of value/token does not exist.
    char strWork[STR_SMALL_SIZE];
    strcpy(strWork, strSearch);

    int cntLine = IniFindValueLineNr(fileName, strWork);
    int r = cntLine;
    char *pEnd;

    if (cntLine > 0){
        // File & Search exist

        IniChangeValueLine(strWork, strValue, typValue);
        return IniReplaceLine(fileName, strWork, cntLine);
    }
    else if (cntLine == 0 || cntLine == -2){
        // Value / Token does not exist

        // strWork  contains (""-embedded and :-separated)
        //              index of 1st missing token
        //              index of broken line in file        
        IniTrimCnt_LR(strWork, 1, 1);
        int missingToken = strtol(strWork, &pEnd, 10);
        int insertLine = strtol(strchr(strWork, ':') + 1, &pEnd, 10);

        r = IniCreateMissingValue(fileName, strSearch, strValue, typValue, missingToken, insertLine);
    }
    else{
        // FileError
    }

    return r;     

}

