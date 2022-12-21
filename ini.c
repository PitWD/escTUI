#include <stdio.h>
#include "ini.h"

int main(){

    char strOut[] = "   1X- jhk\"#\"jhhkh#jhkjhjhkjh -X0           ";
    printf("\nSTART->%s<-END\n\n",strOut);

    IniTrimL(strOut);
    printf("START->%s<-END\n",strOut);

    IniTrimR(strOut);
    printf("START->%s<-END\n",strOut);

    IniRemoveRemark(strOut);
    printf("START->%s<-END\n\n",strOut);

    strcpy(strOut,"   1X- hk\"#\"jhhkh#jhkjhjhkj -X0           ");
    printf("START->%s<-END\n\n",strOut);

    IniTrim(strOut);
    printf("START->%s<-END\n",strOut);

    IniTrimCnt(strOut,1,1);
    printf("START->%s<-END\n\n",strOut);



    // Tokens
    strcpy(strOut, "Main.Sub1.Sub2.Value");
    char *tokens[255] = {NULL};  
    int count = 0;
    int i = 0;

    printf("%s\n",strOut);

    // Extract the tokens
    count = IniGetTokens(strOut, tokens);

    // Print the tokens
    for (i = 0; i < count; i++){
        printf("%s \n", tokens[i]);

        // Upper case the tokens
        IniUcase(tokens[i]);
        printf("%s \n", tokens[i]);

        // Lower case the tokens
        IniLcase(tokens[i]);
        printf("%s \n", tokens[i]);

        // Upper case the 1st 3 chars of the tokens
        IniUcaseLen(tokens[i],3);
        printf("%s \n", tokens[i]);
    }

    // Free the allocated memory
    for (i = 0; i < count; i++)
        free(tokens[i]);
    
    printf("\n");

    char strReturn[255];
    strcpy(strOut, "global.size.width");
    i = IniFindValue("desktops.ini", strReturn, strOut);
    switch (i){
    case -1:
        // File Error
        printf("FileError!: ");
        break;
    case 0:
        // no value found
        printf("Value not found!: ");
        break;
    case 1:
        // text value found
        printf("Text: ");
        break;
    case 2:
        // false/true value found
        printf("True/False: ");
        break;
    case 3:
        // number value found
        printf("Value: ");
        break;
    default:
        // unexpected found
        printf("Unexpected!: ");
        break;
    }
    printf("%s\n\n", strReturn);

}
