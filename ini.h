#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define STR_SMALL_SIZE 255
#define STR_MID_SIZE 1024

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

void IniTrimNonNumeric(char *strIN){

    // Removes from Right all non-numeric and non-hex characters until the first
    // decimal or hex is found
    // "Value = &h234af0 some BS"
    // results in:
    // "Value = &h234af0"

    int i = strlen(strIN) - 1;
    char c = 0;

    for (i; i >= 0; i--){
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
    
    // Split string like "Main.Sub1.Sub1.Value" into their tokens
    // like:    [Main]
    //          [.Sub1]
    //          [.Sub2]
    //          Value

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
   
   // Convert string to lower case

    if (len <= strlen(strIN)){
        for (int c = 0; c < len; c++){
            strIN[c] = tolower(strIN[c]);
        }        
    }
}
#define IniLcase(strIN) IniLcaseLen(strIN, strlen(strIN))

int IniFindValueLineNr(char *fileName, char *strSearch){
    
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

    int cntLine = IniFindValueLineNr(fileName, strSearch);
    int r = 0;

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
    }

    return r;     
}

int IniInsertReplaceString (char *fileName, char *strIN, int linePos, int insert){
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
#define IniInsertString(fileName, strIN, linePos) IniInsertReplaceString(fileName, strIN, linePos, 1)
#define IniReplaceString(fileName, strIN, linePos) IniInsertReplaceString(fileName, strIN, linePos, 0)


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
