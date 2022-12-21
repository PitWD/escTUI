#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define STR_SMALL_SIZE 255
#define STR_MID_SIZE 1024

void IniRemoveRemark (char *strIN){
    
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

    for (i; i <= l; i++){
        if (strIN[i] == '#' && !inQuote){
            strIN[i] = '\0';
            break;
        }
        else if (strIN[i] == '"')
            inQuote = !inQuote;
    }

}

void IniTrimLR(char *strIN, int L, int R){
    
    // L & R are bool to remove whitespaces
    // Left and/or Right

    // Initialize two pointers left and right
    char *pLeft = strIN;
    char *pRight = strIN + strlen(strIN) - 1;

    // Traverse string from both ends until a non-space character is found
    if (L){
        while (pRight > pLeft && (*pLeft == ' ' || *pLeft == '\t'))
            pLeft++;
    }
    
    if (R){
        while (pRight > pLeft && (*pRight == ' ' || *pRight == '\t'))
            pRight--;
    }
    
    // Write all non-space characters from left to right
    while (pLeft <= pRight)
        *strIN++ = *pLeft++;
    
    // Terminate the string with a null character
    *strIN = '\0';
}

void IniTrimL(char *strIN){
    IniTrimLR(strIN,1,0);
}
void IniTrimR(char *strIN){
    IniTrimLR(strIN,0,1);
}
void IniTrim(char *strIN){
    IniTrimLR(strIN,1,1);
}

void IniTrimCnt(char *strIN, int lenL, int lenR){
    
    // Remove lenR chars from the left and lenR from the right

    // len & pointers left and right
    int i = strlen(strIN);
    char *pLeft = strIN;
    char *pRight = strIN + i - 1;

    if (i > lenL){
        pLeft += lenL;
    }

    if (i - lenL > lenR){
        pRight -= lenR;
    }
    
    // Write chars from left to right
    while (pLeft <= pRight)
        *strIN++ = *pLeft++;
    
    // Terminate the string with a null character
    *strIN = '\0';
}

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

        printf("%s\n",token);

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

        printf("%s\n",tokens[count]);

        count++;
    }
    
    count--;

    // remove "[." and "]" from the Value token
    if(count)
        IniTrimCnt(tokens[count], count + 1, 1);

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

void IniUcase(char strIN[]) {
   
   // Convert string to upper case
    IniUcaseLen(strIN, strlen(strIN));
}

void IniLcase(char strIN[]) {
   
   // Convert string to lower case

    int c = 0;

    while (strIN[c] != '\0') {
        strIN[c] = tolower(strIN[c]);
        c++;
    }
}

int IniFindValue(char *fileName, char *strReturn, char *strSearch){

    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        // File-Error
        sprintf(strReturn, "-1");
        return -1;
    }

    int r = 0;
    sprintf(strReturn, "0");

    // Tokens
    char *strTokens[255] = {NULL};  
    int cntTokens = IniGetTokens(strSearch, strTokens) - 1;
    int actToken = 0;
    int isToken = 0;

    char strIN[STR_SMALL_SIZE];
    while (fgets(strIN, STR_SMALL_SIZE, file) != NULL) {

        // Remove remarks
        IniRemoveRemark(strIN);

        // Trim Left and Right
        IniTrim(strIN);

        // Uppercase the first chars (length of actual token)
        // IniUcaseLen(strIN, strlen(strTokens[actToken]));

        // Check if line starts with actual token
        if(strncasecmp(strIN, strTokens[actToken], strlen(strTokens[actToken])) == 0){
            if (actToken == cntTokens){
                
                // Remove all chars left of "="
                char *pEqual = strchr(strIN, '=');
                if (pEqual){
                    memmove(strIN, pEqual + 1, strlen(pEqual));
                }

                // Trim whitespaces
                IniTrim(strIN);

                // Remove 1st and last " from Text if they're present
                if (strIN[0] == '"'){
                    // Value is a text
                    IniTrimCnt(strIN,1,0);
                    if (strIN[strlen(strIN) - 2] == '"'){
                        IniTrimCnt(strIN,0,2);
                    }
                    sprintf(strReturn, strIN);
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
                        sprintf(strReturn, strIN);
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

    fclose(file);
    for (actToken = 0; actToken <= cntTokens; actToken++){
        free(strTokens[actToken]);
    }

    return r;
    
}