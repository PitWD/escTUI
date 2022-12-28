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

void IniTrimWhiteSpacesLR(char *strIN, int l, int r){
    
    // L & R are bool to remove whitespaces
    // Left and/or Right

    // Initialize two pointers left and right
    char *pLeft = strIN;
    char *pRight = strIN + strlen(strIN) - 1;

    // Traverse string from both ends until a non-space character is found
    if (l){
        while (pRight > pLeft && (*pLeft == ' ' || *pLeft == '\t'))
            pLeft++;
    }
    
    if (r){
        while (pRight > pLeft && (*pRight == ' ' || *pRight == '\t'))
            pRight--;
    }
    
    pRight++;
    memmove(strIN, pLeft, pRight - pLeft);
    strIN[pRight - pLeft] = '\0';
}
#define IniTrimWS_L(strIN) IniTrimWhiteSpacesLR(strIN,1,0)
#define IniTrimWS_R(strIN) IniTrimWhiteSpacesLR(strIN,0,1)
#define IniTrimWS(strIN) IniTrimWhiteSpacesLR(strIN,1,1)

void IniTrimCharsLR(char *strIN, const char c, int l, int r){
    
    // L & R are bool to remove all chars != c
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
#define IniTrimChar_L(strIN, c) IniTrimCharsLR(strIN, c, 1, 0)
#define IniTrimChar_R(strIN, c) IniTrimCharsLR(strIN, c, 0, 1)
#define IniTrimChar_LR(strIN, c) IniTrimCharsLR(strIN, c, 1, 1)

void IniTrimCntLR(char *strIN, int l, int r){
    
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
        tokens[count] = malloc(strlen(token) + 3 + count);
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

        tokens[count][strlen(tokens[count])] = ']';
        tokens[count][strlen(tokens[count]) + 1] = '\0';

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

void IniUcaseLen(char strIN[], int len) {
   
   // Convert 1st len chars to upper case

    if (len <= strlen(strIN)){
        for (int c = 0; c < len; c++){
            strIN[c] = toupper(strIN[c]);
        }        
    }
}
#define IniUcase(strIN) IniUcaseLen(strIN, strlen(strIN))

void IniLcaseLen(char strIN[], int len) {
   
   // Convert 1st len chars to lower case

    if (len <= strlen(strIN)){
        for (int c = 0; c < len; c++){
            strIN[c] = tolower(strIN[c]);
        }        
    }
}
#define IniLcase(strIN) IniLcaseLen(strIN, strlen(strIN))

int IniFindValueLineNr(char *fileName, char *strSearch){

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
    //         -1 = File Error
    //         -2 = Broken Token
    //         >0 = Line with Value

    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        // File-Error
        return -1;
    }

    int r = 0;
    int i = 0;
    int cntLine = 0;

    // Tokens
    char *strTokens[STR_SMALL_SIZE] = {NULL};  
    int cntTokens = IniGetTokens(strSearch, strTokens) - 1;
    int actToken = 0;
    int isToken = 0;

    char strIN[STR_SMALL_SIZE];
    while (fgets(strIN, STR_SMALL_SIZE, file) != NULL) {

        cntLine++;

        // Trim Left & Right Whitespaces
        IniTrimWS_L(strIN);

        // Check if line is above actual token-level
        if (strIN[0] == '['){
            if (actToken){
                if (strIN[actToken] != '.'){
                    // Broken Token
                    r = -2;
                    break;
                }
            }         
        }
        
        i = strlen(strTokens[actToken]);

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

    fclose(file);
    for (actToken = 0; actToken <= cntTokens; actToken++){
        free(strTokens[actToken]);
    }

    return r;
} 

int IniGetValue(char *fileName, char *strReturn, char *strSearch){

    // Returns  0 = Value does not exist
    //         -1 = File Error.
    //         -2 = Broken Token
    //          1 = Value is a text
    //          2 = Value is a bool
    //          3 = Value is an int
    //          4 = Value is a float
    //
    // strReturn, returns from a line like:
    // "          Value = 123,456  # MyRemark"
    // just the Value:
    // "123.456"
    // "," & "." are seen as valid decimal points

    int cntLine = IniFindValueLineNr(fileName, strSearch);
    int r = cntLine;

    if (cntLine > 0){
        // File and Search exist

        int i = 0;
        char strIN[STR_SMALL_SIZE];

        FILE *file = fopen(fileName, "r");
        if (file == NULL) {
            // File-Error
            return - 1;
        }

        // Trash Lines before our line....
        while (cntLine > 0){
            fgets(strIN, STR_SMALL_SIZE, file);
            cntLine--;
        }

        // Remove remarks
        IniTrimRemark(strIN);

        // Remove all chars left of "="
        IniTrimChar_L(strIN, '=');

        // Remove equal
        IniTrimCnt_L(strIN, 1);

        // Trim whitespaces
        IniTrimWS_L(strIN);

        // Remove 1st and last " from Text if they're present
        if (strIN[0] == '"'){
            // Value is a text

            // Remove leading '"'
            IniTrimCnt_L(strIN,1);

            // Remove all trailing nonsense
            IniTrimChar_R(strIN, '"');
            if (strIN[strlen(strIN) - 1] == '"'){
                // Text was fully encapsulated
                IniTrimCnt_R(strIN,1);
            }
            
            sprintf(strReturn, "%s",strIN);
            r = 1;
        }
        else{
            // Value is a number or True or False
            if(strncasecmp(strIN, "true", 4) == 0){
                // True
                sprintf(strReturn, "1");
                r = 2;
            }
            else if(strncasecmp(strIN, "false", 5) == 0){
                // False
                sprintf(strReturn, "0");
                r = 2;
            }
            else{
                // Number
                r = 3;                
                // remove all non-numeric trailing characters
                IniTrimNonNumeric(strIN);
                // Make 1st comma to dot...
                char *pComma = strchr(strIN, ',');
                if (pComma != NULL) {
                    r++;
                    *pComma = '.';
                }
                sprintf(strReturn, "%s",strIN);
            }                    
        }   
    }

    return r;     
}

int IniInsertReplaceLine (char *fileName, char *strIN, int linePos, int insert){
    
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
    if (remove(buffer) != 0) {
        return -4;
    }

    return actLine;
}
#define IniInsertLine(fileName, strIN, linePos) IniInsertReplaceLine(fileName, strIN, linePos, 1)
#define IniReplaceLine(fileName, strIN, linePos) IniInsertReplaceLine(fileName, strIN, linePos, 0)

int IniChangeValueLine (char *strIN, char *strValue, int valType){

    // a string like:

    // "             Value = 1234     # MyRemark"
    // gets splitted into
    // "             Value =" (strValue)
    // and
    // "     # MyRemark" (strRemark)

    // when we replace "1234" with strVal
    // we try to preserve the position of the "#"
    // if strIn is too long, we place it on a tab
    //

    // Returns  -1 for no success
    //           0 text as it is success 
    //           4 text embedded in ""
    //           1 int
    //           2 float
    //           3 hex

    char strPreVal[STR_SMALL_SIZE];
    strcpy(strPreVal, strIN);
    IniTrimRemark(strPreVal);
    IniTrimChar_R(strPreVal, '=');

    char strRemark[STR_SMALL_SIZE];
    strcpy(strRemark, strIN);
    IniGetRemark(strRemark);
 
    char *pEnd;
    long lNum = 0;
    double dNum = 0;
    long hNum = 0;

    // Check and Format strValue
    char strValNew[STR_SMALL_SIZE];
    switch (valType){
    case 1:
        // as int
        lNum = strtol(strValue, &pEnd, 10);   
        if (*pEnd != '\0'){
            // No int found
            return -1;
        }
        sprintf(strValNew, " %ld", lNum);
        break;
    case 2:
        // as float
        dNum = strtod(strValue, &pEnd);
        if (*pEnd != '\0'){
            // No float found
            return -1;
        }
        sprintf(strValNew, " %.6f", dNum);
        break;
    case 3:
        // as hex
        hNum = strtol(strValue, &pEnd, 16);
        if (*pEnd != '\0'){
            // No hex found
            return -1;
        }
        sprintf(strValNew, " 0x%lX", hNum);
        break;
    case 4:
        // as text embedded in ""
        sprintf(strValNew, " \"%s\"", strValue);
        break;
    default:
        // as it is, but with leading space
        sprintf(strValNew, " %s", strValue);
        break;
    }

    // Check and place Remark
    if (strlen(strRemark)){
        // Line contains a remark

        int lenDiff = strlen(strValNew) - (strlen(strIN) - strlen(strPreVal) - strlen(strRemark));

        int lenSpaces = IniCountFrontSpaces(strRemark);

        if (lenDiff > 0){
            // New Value is longer than the old one
            // check if the longer value fit's into the leading spaces of strRemark

            if (lenSpaces < lenDiff){
                // Missing space in total

                // Remove all spaces
                sprintf(strRemark, "%s", &strRemark[lenSpaces]);

                // add missing spaces (Tab adjusted)
                sprintf(strIN, "%*c%s", INI_TAB_LEN - ((lenDiff - lenSpaces) % INI_TAB_LEN), ' ', strRemark);
            }
            else if (lenSpaces > lenDiff){
                // Enough spaces
                
                // Remove the too much spaces
                sprintf(strIN, "%s", &strRemark[lenDiff]);
            }
            else{
                // Like spaces

                // Remove all spaces, but Add a 'TAB'
                sprintf(strIN, "%*c%s", INI_TAB_LEN, ' ', &strRemark[lenDiff]);
            } 
       }
        else if (lenDiff < 0){
            // New Value is shorter than the old one
            // Add spaces
            sprintf(strIN, "%*c%s", -lenDiff, ' ', strRemark);
        }
        else{
            // Same length
            sprintf(strIN, "%s", strRemark);                
        }
        sprintf(strRemark, "%s", strIN);                
    }

    // Join them together
    sprintf(strIN, "%s%s%s", strPreVal, strValNew, strRemark);

    return valType;

}

/*
int IniFindValueFAST(char *fileName, char *strReturn, char *strSearch){

    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        // File-Error
        sprintf(strReturn, "-1");
        return -1;
    }

    int r = 0;
    int i = 0;

    sprintf(strReturn, "0");

    // Tokens
    char *strTokens[255] = {NULL};  
    int cntTokens = IniGetTokens(strSearch, strTokens) - 1;
    int actToken = 0;
    int isToken = 0;

    char strIN[STR_SMALL_SIZE];

    while (fgets(strIN, STR_SMALL_SIZE, file) != NULL) {

        // Trim Left & Right Whitespaces
        IniTrimWS_L(strIN);

        // Check if line is above actual token-level
        if (strIN[0] == '['){
            if (actToken){
                if (strIN[actToken] != '.'){
                    // Broken Token
                    r = -2;
                    sprintf(strReturn, "-2");
                    break;
                }
            }         
        }
        
        i = strlen(strTokens[actToken]);

        // Check if line starts with actual token
        if(strncasecmp(strIN, strTokens[actToken], i) == 0){

            // Prevent that Token in File is longer (3rd case is just for [Main]-Level)
            if ((strIN[i] == ' ')  || (strIN[i] == '=') || (strlen(strIN)-1 == i)){
                // Token has the right length

                if (actToken == cntTokens){
                    // It's the Value-Token

                    // Remove remarks
                    IniTrimRemark(strIN);

                    // Remove all chars left of "="
                    IniTrimChar_L(strIN, '=');

                    // Remove equal
                    IniTrimCnt_L(strIN, 1);

                    // Trim whitespaces
                    IniTrimWS_L(strIN);

                    // Remove 1st and last " from Text if they're present
                    if (strIN[0] == '"'){
                        // Value is a text

                        // Remove leading '"'
                        IniTrimCnt_L(strIN,1);

                        // Remove all trailing nonsense
                        IniTrimChar_R(strIN, '"');
                        if (strIN[strlen(strIN) - 1] == '"'){
                            // Text was fully encapsulated
                            IniTrimCnt_R(strIN,1);
                        }
                        
                        sprintf(strReturn, "%s",strIN);
                        r = 1;
                    }
                    else{
                        // Value is a number or True or False
                        if(strncasecmp(strIN, "true", 4) == 0){
                            // True
                            sprintf(strReturn, "1");
                            r = 2;
                        }
                        else if(strncasecmp(strIN, "false", 5) == 0){
                            // False
                            sprintf(strReturn, "0");
                            r = 2;
                        }
                        else{
                            // Number

                            // Make comma to dot...
                            char *pComma = strchr(strIN, ',');
                            if (pComma != NULL) {
                                *pComma = '.';
                            }

                            // remove all non-numeric trailing characters
                            IniTrimNonNumeric(strIN);

                            sprintf(strReturn, "%s",strIN);
                            r = 3;
                        }                    
                    }   
                    break;    
                }
                else{
                    actToken++;
                }                        
            }               
        }
    }

    fclose(file);
    for (actToken = 0; actToken <= cntTokens; actToken++){
        free(strTokens[actToken]);
    }

    return r;
    
}
*/
