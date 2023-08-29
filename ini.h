#include "str.h"

#define INI_TAB_LEN 4

#define INI_TYPE_AsItIs 0
#define INI_TYPE_Int 1
#define INI_TYPE_Float 2
#define INI_TYPE_Hex 3
#define INI_TYPE_Text 4
#define INI_TYPE_Bool 5
#define INI_TYPE_Bin 6

char *IniStrToMem(const char *strIN, int reset){
	// Add strIN to strArray
	// return pointer on embedded strIN

	static char **strArray = NULL;
	static int cnt = 0;
	
	if (reset){
		// free memory		
		if (strArray){
			for (size_t i = 0; i < cnt; i++){
				free(strArray[i]);
				strArray[i] = NULL;
			}
			free(strArray);
			strArray = NULL;
		}
	}
	else{
		// Put string to memory

		if (cnt){
			// look, if the string already exist...
			for (size_t i = 0; i < cnt; i++){
				if (!strcmp(strIN, strArray[i])){
					// exist
					return strArray[i];
				}
			}
		}
		

		cnt++;
		strArray = (char**)realloc(strArray, cnt * sizeof(char*));
		strArray[cnt - 1] = (char*)malloc((strlen(strIN)+1) * sizeof(char));
		strcpy(strArray[cnt-1], strIN);
		return strArray[cnt-1];

	}

	return NULL;
}

void IniTrimRemark (char *strIN){
    // Remove all trailing text after
    // the first non "" encapsulated #
  
    int l = strlen(strIN); 
    int i = 0;
    int inQuote = 0;

    for (i = 0; i < l; i++){
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
            else if (strIN[i] != ' ' && strIN[i] != '\t' && strIN[i] != '\n' && strIN[i] != '\r'){
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

void IniCleanBin(char *strIN){

    // Make all 'i' & 'I' to 1
    // All 'o' and 'O' to 0
    // 'B' on 2nd or last pos to 'b'
    // '&' on 1st pos to 0

    char c = 0;
    int l = strlen(strIN);

    for (int i = 0; i < l; i++){
        c = strIN[i];
        if (c == '1' || c == '0' || (c == 'b' && (i == 1 || i == l - 1))){
            // Fine...
        }        
        else if (c == 'o' || c == 'O'){
            strIN[i] = '0';
        }
        else if (c == 'i' || c == 'I'){
            strIN[i] = '1';
        }
        else if (c == 'B' && (i == 1 || i == l - 1)){
            strIN[i] = 'b';
        }
        else if (c == '&' && i == 0){
            strIN[i] = '0';
        }
        else if(c == ' '){
            // Remove space
            memmove(&strIN[i], &strIN[i + 1], l - i);
            l--;
            i--;
        }
        else {
            // Messy Char
            strIN[i] = '\0';
            break;
        }
    }
}

int IniIsBin(const char *strIN){
    char c = 0;
    int l = strlen(strIN);

    for (int i = 0; i < l; i++){
        c = strIN[i];
        if (c == '1' || c == '0' || (c == 'b' && (i == 1 || i == l - 1))){
            // Fine...
        }        
        else if (c == 'o' || c == 'O'){
        }
        else if (c == 'i' || c == 'I'){
        }
        else if (c == 'B' && (i == 1 || i == l - 1)){
        }
        else if (c == '&' && i == 0){
        }
        else if(c == ' '){
        }
        else {
            // Messy Char
            return 0;
        }
    }
    return 1;
}

int IniIsNonNumeric(const char *strIN){
    // With the first non "0-9", ".", "," it's non-numeric

    char c = 0;

    for (int i = strlen(strIN) - 1; i >= 0; i--){
        c = strIN[i];
        if ((c < '0' || c > '9') && (c != ',' && c != '.')){
            return 1;
        }
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
    if(count > 0){
        StrTrimCnt_LR(tokens[count], count + 1, 1);
    }
    count++;
    return count;
}

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

    int insertLine = 0;
    int allowIncrease = 1;

    char strIN[STR_SMALL_SIZE];
    while (fgets(strIN, STR_SMALL_SIZE, file) != NULL) {

        cntLine++;

        if (allowIncrease){
            insertLine++;
        }
    
        // Save original line for return
        strcpy(strSearch, strIN);

        // Trim Left Whitespaces
        StrTrimWS_L(strIN);

        // Check if line is below actual token-level
        if (strIN[0] == '['){
            allowIncrease = 0;
            if (actToken){
                if (strIN[actToken] != '.'){
                    // Broken Token
                    r = -2;
                    sprintf(strSearch, "\"%d:%d\"", actToken, insertLine);
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
                    insertLine = cntLine;
                    allowIncrease = 1;
                    actToken++;
                }                        
            }               
        }
    }

    if (!r){
        sprintf(strSearch, "\"%d:0\"", actToken);
    }
    
    fclose(file);

    /*
    cntTokens++;
    for (actToken = 0; actToken < cntTokens; actToken++){
        free(strTokens[actToken]);
        strTokens[actToken] = NULL;
    }
    */

    free(*strTokens);

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

    #if __WIN32__ || _MSC_VER || __WIN64__
        remove(fileName);
    #endif
    
    // Replace the original file with the temporary file
    sprintf(buffer, "%s.tmp", fileName);
    if (rename(buffer, fileName) != 0) {
        return -3;
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
    //          = 6     Binary

    // most common human and OS errors get fixed
    // no case sensitivity

    /*
    // For the case float has "," instead of "." as delimiter
    #if __WIN32__ || _MSC_VER || __WIN64__
        char strValue2[STR_SMALL_SIZE];
    #else
        char strValue2[strlen(strValue) + 64];
    #endif
    */

    char *pEnd;

    long lNum = 0;
    double dNum = 0;
    #define hNum lNum
    #define bNum lNum

    int r = INI_TYPE_AsItIs;

    switch (valType){
    case INI_TYPE_Int:
        // as int
        StrTrimNonNumeric(strValue);
        lNum = strtol(strValue, &pEnd, 10);   
        if (*pEnd == '\0'){
            sprintf(strValue, "%ld", lNum);
            r = INI_TYPE_Int;
        }
        break;
    case INI_TYPE_Float:
        // as float
        // Make 1st comma to dot...
        if(StrNormFloatString(strValue)){
            r = INI_TYPE_Float;
        }
    case INI_TYPE_Hex:
        // as hex
        if(strncasecmp(strValue, "&h", 2) == 0){
            hNum = strtol(&strValue[2], &pEnd, 16);
        }
        else{
            hNum = strtol(strValue, &pEnd, 16);
        }    
        if (*pEnd == '\0'){
            if (hNum){
                sprintf(strValue, "%#lx", hNum);
            }
            else{
                strcpy(strValue, "0x0");
            }
            r = INI_TYPE_Hex;
        }
        break;
    case INI_TYPE_Bin:
        // as Bin
        IniCleanBin(strValue);
        lNum = StrBin2Long(strValue);
        StrLong2Bin(lNum, strValue);
        r = INI_TYPE_Bin;
        break;
    case INI_TYPE_Bool:
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
            strcpy(strValue, "true");
        }
        else{
            // False
            strcpy(strValue, "false");
        }
        r = INI_TYPE_Bool;
        break;
    case INI_TYPE_Text:
        // as text embedded in ""
        
        /*
        sprintf(strValue2, "\"%s\"", strValue);
        strcpy(strValue, strValue2);
        */

        r = strlen(strValue);
        memmove(&strValue[1], strValue, r);
        strValue[0] = '"';
        strValue[r + 1] = '"';
        strValue[r + 2] = '\0';
        
        r = INI_TYPE_Text;
        break;
    default:
        // Untouched (INI_TYPE_AsItIs)
        break;
    }

    return r;
}

int IniGetTypeFromValue(const char *strValue){

    int r = INI_TYPE_AsItIs;

    if (strValue[0] == '"'){
        r = INI_TYPE_Text;
    }
    else{
        if(strncasecmp(strValue, "true", 4) == 0 || strncasecmp(strValue, "false", 5) == 0){
            r = INI_TYPE_Bool;
        }
        else if(strncasecmp(strValue, "0x", 2) == 0 || strncasecmp(strValue, "&h", 2) == 0){
            r = INI_TYPE_Hex;
        }
        else if(strncasecmp(strValue, "0b", 2) == 0 || strncasecmp(strValue, "ob", 2) == 0 || strncasecmp(strValue, "&b", 2) == 0){
            r = INI_TYPE_Bin;
        }
        else if (strncasecmp(&strValue[strlen(strValue) - 1], "b", 1) == 0){
            if (IniIsBin(strValue)){
                r = INI_TYPE_Bin;
            }
        }
        else if (IniIsNonNumeric(strValue)){
            // Bad formatted (not "" - embedded text)
        }
        else{
            r = INI_TYPE_Int;                
            // Is there a dot ?
            if (strchr(strValue, ',') != NULL || strchr(strValue, '.') != NULL){
                r++;
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

    StrTrimWS_R(strIN);

    // PreValue - part
    char strPreVal[STR_SMALL_SIZE];
    strcpy(strPreVal, strIN);
    IniTrimRemark(strPreVal);
    StrTrimChars_R(strPreVal, '=');

    // Copy & Check & Format strValue
    #if __WIN32__ || _MSC_VER || __WIN64__
        char strValNew[STR_SMALL_SIZE];
    #else
        char strValNew[strlen(strValue) + 66];
    #endif
    
    strcpy(strValNew, strValue);
    r = IniSetTypeToValue(strValNew, valType);
    
    // Remark - part
    char strRemark[STR_SMALL_SIZE];
    strcpy(strRemark, strIN);
    IniGetRemark(strRemark);
 
    // Check and (re)place Remark
    if (strlen(strRemark)){
        // Line contains a remark

        int lenDiff = strlen(strValNew) - (strlen(strIN) - strlen(strPreVal) - strlen(strRemark) - 1);

        if (lenDiff > 0){
            // New Value is longer than the old one
            // check if the longer value fit's into the leading spaces of strRemark

            // Count spaces (respecting tabs)
            int lenSpaces = IniCountFrontSpaces(strRemark);
            // Remove all spaces
            StrTrimWS(strRemark);

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
            StrTrimWS(strWork);

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
                StrTrimCnt_L(strWork, 1);
            }
            r = IniInsertLine(fileName, strWork, insertLine);
            insertLine++;
        }
        
        // Finally the value
        if (r > 0){
            sprintf(strWork, "%*c%s = %s", i * INI_TAB_LEN, ' ', tokens[i], strValue);
            // Check & normalize value
            //printf("work: %s = %s / ", strWork, strValue);
            r = IniChangeValueLine(strWork, strValue, typValue);
            //printf("%s\n",strWork);
            if (r > -1){
                IniInsertLine(fileName, strWork, insertLine);
            }
        }
    }

    // Free the allocated memory
    /*
    for (i = 0; i < cntToken; i++){
        free(tokens[i]);
        tokens[i] = NULL;
    }
    */
    free(*tokens);

    return r;
} 

int IniGetValue(const char *fileName, const char *strSearch, const char *strDefault, const int typValue, char *strReturn){

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

    // Protection from endless loop, if creation and/or re-read of created value fails. 
    static int imInside = 0;

    // Working string
    strcpy(strReturn, strSearch);

    int cntLine = IniFindValueLineNr(fileName, strReturn);

    char *pEnd;
    int r = 0;

    if (cntLine > 0){
        // File and Search exist

        // Remove remarks
        IniTrimRemark(strReturn);

        // Remove all chars left of "="
        StrTrimChars_L(strReturn, '=');

        // Remove equal
        StrTrimCnt_L(strReturn, 1);

        // Trim whitespaces
        StrTrimWS(strReturn);

        if (typValue){
            // get type of value and normalize value
            
            r = IniGetTypeFromValue(strReturn);
            if(r == INI_TYPE_Text){
                // Remove leading '"'
                StrTrimCnt_L(strReturn, 1);

                if (strReturn[strlen(strReturn) - 1] == '"'){
                    // Text was fully encapsulated
                    StrTrimCnt_R(strReturn,1);
                }
            } 

            if (r != typValue){
                // Type in file is not the type we're expecting
                
                double dVal = 0;
                if (typValue == INI_TYPE_Text){
                    // nothing to do
                }
                else{
                    // convert str as good as possible into double
                    switch (r){
                    case INI_TYPE_Text:
                        StrCommaToDot(strReturn);
                        dVal = strtod(strReturn, &pEnd);
                        break;
                    case INI_TYPE_Bin:
                        IniCleanBin(strReturn);
                        dVal = (double)StrBin2Long(strReturn);
                        break;
                    case INI_TYPE_Hex:
                        dVal = (double)strtol(strReturn, &pEnd, 16);
                        break;
                    case INI_TYPE_Int:
                        dVal = (double)strtol(strReturn, &pEnd, 10);
                        break;
                    case INI_TYPE_Bool:
                        if (strncasecmp(strReturn, "true", 4) == 0){
                            dVal = 1;
                        }
                        break;
                    }
                    // convert the double "rough" into expected type
                    switch (typValue){
                    case INI_TYPE_Bin:
                        StrLong2Bin((long)dVal, strReturn);
                        break;
                    case INI_TYPE_Hex:
                        sprintf(strReturn, "%#lx", (long)dVal);
                        break;
                    case INI_TYPE_Float:
                        sprintf(strReturn, "%.8f", dVal);
                        break;
                    case INI_TYPE_Int:
                        sprintf(strReturn, "%ld", (long)dVal);
                        break;
                    case INI_TYPE_Bool:
                        if (dVal > 0.1){
                            strcpy(strReturn, "true");
                        } 
                        else{
                            strcpy(strReturn, "false");
                        }
                        break;
                    }
                }            
            }
            if(typValue != INI_TYPE_Text){
                // normalize...
                IniSetTypeToValue(strReturn, typValue);
            }
        }  
    }
    else if (cntLine == 0 || cntLine == -2){
        // Value / Token does not exist

        // strReturn    contains (""-embedded and :-separated)
        //              index of 1st missing token
        //              index of broken line in file        
        StrTrimCnt_LR(strReturn, 1, 1);
        int missingToken = strtol(strReturn, &pEnd, 10);
        int insertLine = strtol(strchr(strReturn, ':') + 1, &pEnd, 10);

        if (!imInside){
            imInside = 1;
            char strAutoValue[STR_SMALL_SIZE];
            sprintf(strAutoValue, "%s #!", strDefault);
            r = IniCreateMissingValue(fileName, strSearch, strAutoValue, typValue, missingToken, insertLine);
            // ReRead as it's written...
            r = IniGetValue(fileName, strSearch, strDefault, typValue, strReturn);
        }
        else{
            // WTF - kind of a File-Error ?!?!

            strcpy(strReturn, strSearch);
            r = -1;
        }
    }
    else{
        // FileError
    }
    
    imInside = 0;
    return r;     
}
#define IniGet(fileName, strSearch, strDefault, strReturn) IniGetValue(fileName, strSearch, strDefault, INI_TYPE_AsItIs, strReturn)
#define IniGetStr(fileName, strSearch, strDefault, strReturn) IniGetValue(fileName, strSearch, strDefault, INI_TYPE_Text, strReturn)
long IniGetLong(const char *fileName, const char *strSearch, const long defLong){
    
    char strValue[STR_SMALL_SIZE];
    char strReturn[STR_SMALL_SIZE];
    char *pEnd;

    sprintf(strValue, "%ld", defLong);

    IniGetValue(fileName, strSearch, strValue, INI_TYPE_Int, strReturn);
    long valLong = strtol(strReturn, &pEnd, 10);
    if (*pEnd == '\0'){
        return valLong;
    }
    return defLong;
}
#define IniGetInt(fileName, strSearch, defInt) (int)IniGetLong(fileName, strSearch, (long)defInt)
double IniGetDouble(const char *fileName, const char *strSearch, const double defDouble){

    char strValue[STR_SMALL_SIZE];
    char strReturn[STR_SMALL_SIZE];
    char *pEnd;

    sprintf(strValue, "%.8f", defDouble);
    IniGetValue(fileName, strSearch, strValue, INI_TYPE_Float, strReturn);
    double valDouble = strtod(strReturn, &pEnd);
    if (*pEnd == '\0'){
        return valDouble;
    }
    return defDouble;
}
#define IniGetFloat(fileName, strSearch, defFloat) (float)IniGetDouble(fileName, strSearch, (double)defFloat)
long IniGetLongHex(const char *fileName, const char *strSearch, const long defLong){
    
    char strValue[STR_SMALL_SIZE];
    char strReturn[STR_SMALL_SIZE];
    char *pEnd;

    sprintf(strValue, "%#lx", defLong);
    IniGetValue(fileName, strSearch, strValue, INI_TYPE_Hex, strReturn);
    long valLong = strtol(strReturn, &pEnd, 16);
    if (*pEnd == '\0'){
        return valLong;
    }
    return defLong;
}
#define IniGetHex(fileName, strSearch, defInt) (int)IniGetLongHex(fileName, strSearch, (long)defInt)
int IniGetBool(const char *fileName, const char *strSearch, const int defBool){

    char strValue[STR_SMALL_SIZE];
    char strReturn[STR_SMALL_SIZE];
    char *pEnd;

    if (defBool){
        sprintf(strValue, "true");
    }
    else{
        sprintf(strValue, "false");
    }
    
    
    IniGetValue(fileName, strSearch, strValue, INI_TYPE_Bool, strReturn);

    if(strncasecmp(strReturn, "true", 4) == 0){
        // True
        return 1;
    }
    else{
        return 0;
    }
}
long IniGetLongBin(const char *fileName, const char *strSearch, const long defLong){
    
    char strValue[STR_SMALL_SIZE];
    char strReturn[STR_SMALL_SIZE];

    StrLong2Bin(defLong, strValue);
    IniGetValue(fileName, strSearch, strValue, INI_TYPE_Bin, strReturn);
    return StrBin2Long(strReturn);
    
}
int IniGetIntBin(const char *fileName, const char *strSearch, const int defInt){
    
    char strValue[STR_SMALL_SIZE];
    char strReturn[STR_SMALL_SIZE];

    StrInt2Bin(defInt, strValue);
    IniGetValue(fileName, strSearch, strValue, INI_TYPE_Bin, strReturn);
    return StrBin2Int(strReturn);
    
}
unsigned char IniGetByteBin(const char *fileName, const char *strSearch, unsigned char defByte){
    
    char strValue[STR_SMALL_SIZE];
    char strReturn[STR_SMALL_SIZE];

    StrByte2Bin(defByte, strValue);
    IniGetValue(fileName, strSearch, strValue, INI_TYPE_Bin, strReturn);
    return StrBin2Byte(strReturn);
    
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
        r = IniReplaceLine(fileName, strWork, cntLine);
    }
    else if (cntLine == 0 || cntLine == -2){
        // Value / Token does not exist

        // strWork  contains (""-embedded and :-separated)
        //              index of 1st missing token
        //              index of broken line in file        
        StrTrimCnt_LR(strWork, 1, 1);
        int missingToken = strtol(strWork, &pEnd, 10);
        int insertLine = strtol(strchr(strWork, ':') + 1, &pEnd, 10);

        r = IniCreateMissingValue(fileName, strSearch, strValue, typValue, missingToken, insertLine);
    }
    else{
        // FileError
    }

    return r;     
}
#define IniSet(fileName, strSearch, strValue) IniSetValue(fileName, strSearch, strValue, INI_TYPE_AsItIs)
#define IniSetStr(fileName, strSearch, strValue) IniSetValue(fileName, strSearch, strValue, INI_TYPE_Text)
int IniSetLong(const char *fileName, const char *strSearch, const long lngValue){
    char strValue[STR_SMALL_SIZE];
    sprintf(strValue, "%ld", lngValue);
    return IniSetValue(fileName, strSearch, strValue, INI_TYPE_Int);
}
#define IniSetInt(fileName, strSearch, intValue) IniSetLong(fileName, strSearch, (long)intValue)
int IniSetDouble(const char *fileName, const char *strSearch, const double dblValue){
    char strValue[STR_SMALL_SIZE];
    sprintf(strValue, "%.8f", dblValue);
    return IniSetValue(fileName, strSearch, strValue, INI_TYPE_Float);
}
#define IniSetFloat(fileName, strSearch, fltValue) IniSetDouble(fileName, strSearch, (double)fltValue)
int IniSetLongHex(const char *fileName, const char *strSearch, const long hexValue){
    char strValue[STR_SMALL_SIZE];
    sprintf(strValue, "%#lx", hexValue);
    return IniSetValue(fileName, strSearch, strValue, INI_TYPE_Hex);
}
#define IniSetHex(fileName, strSearch, hexValue) (int)IniSetLongHex(fileName, strSearch, (long)hexValue)
int IniSetBool(const char *fileName, const char *strSearch, const int boolValue){
    char strValue[STR_SMALL_SIZE];
    if (boolValue){
        strcpy(strValue, "true");
    }
    else{
        strcpy(strValue, "false");
    }
    return IniSetValue(fileName, strSearch, strValue, INI_TYPE_Bool);
}
int IniSetLongBin(const char *fileName, const char *strSearch, const long lValue){
    char strValue[STR_SMALL_SIZE];
    StrLong2Bin(lValue, strValue);
    return IniSetValue(fileName, strSearch, strValue, INI_TYPE_Bin);
}
int IniSetIntBin(const char *fileName, const char *strSearch, const int iValue){
    char strValue[STR_SMALL_SIZE];
    StrInt2Bin(iValue, strValue);
    return IniSetValue(fileName, strSearch, strValue, INI_TYPE_Bin);
}
int IniSetByteBin(const char *fileName, const char *strSearch, const unsigned char bValue){
    char strValue[STR_SMALL_SIZE];
    StrByte2Bin(bValue, strValue);
    return IniSetValue(fileName, strSearch, strValue, INI_TYPE_Bin);
}
