#include <stdio.h>
#include "ini.h"

int main(){

    char strOut[255];
    sprintf(strOut, "   1X- jhk\"#\"jhhkh#jhkjhhgfhgfhfhfhgfjhkjh -X0           ");
    printf("\nSTART->%s<-END\n\n",strOut);

    StrTrimWS_L(strOut);
    printf("START->%s<-END\n",strOut);

    StrTrimWS_R(strOut);
    printf("START->%s<-END\n",strOut);

    IniTrimRemark(strOut);
    printf("START->%s<-END\n\n",strOut);

    strcpy(strOut,"   1X- hk\"#\"jhhkh#jhkjhjhkj -X0           ");
    printf("START->%s<-END\n\n",strOut);

    StrTrimWS(strOut);
    printf("START->%s<-END\n",strOut);

    StrTrimCnt_LR(strOut,1,1);
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
        StrUcase(tokens[i]);
        printf("%s \n", tokens[i]);

        // Lower case the tokens
        StrLcase(tokens[i]);
        printf("%s \n", tokens[i]);

        // Upper case the 1st 3 chars of the tokens
        StrUcaseLen(tokens[i],3);
        printf("%s \n", tokens[i]);
    }

    // Free the allocated memory
    for (i = 0; i <= count; i++)
        free(tokens[i]);
    
    printf("\n");

    //char strReturn[255];
    // strcpy(strOut, "global.definitions.fixedareas.1.top.captionTEXT");
    i = IniGetValue("desktops.ini", "global.definitions.fixedareas.1.top.captionTEXT", "moff", INI_TYPE_AsItIs, strOut);
    switch (i){
    case -2:
        // Broken-Token
        printf("Broken Token!: ");
        break;
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
    printf("%s<-\n", strOut);

    // strcpy(strOut, "global.definitions.fixedareas.1.top.captioncolorid");
    i = IniGetValue("desktops.ini", "global.definitions.fixedareas.1.top.captioncolorid", "moff", INI_TYPE_Text, strOut);
    switch (i){
    case -2:
        // Broken-Token
        printf("Broken Token!: ");
        break;
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
    printf("%s<-\n\n", strOut);

    strcpy(strOut,"1234 1234567890 1234");
    StrTrimCharsLR(strOut,' ',1,1);
    printf("->%s<-\n", strOut);

    strcpy(strOut,"1234 1234567890 1234");
    StrTrimCnt_LR(strOut,4,4);
    printf("->%s<-\n\n", strOut);

    strcpy(strOut, "                    CaptionText = \"FixedTopNeuerVersuch\"");
    IniReplaceLine("desktops.ini",strOut,52);

    strcpy(strOut, "global.definitions.fixedareas.1.top.captionTEXT");
    i = IniGetValue("desktops.ini", "global.definitions.fixedareas.1.top.captionTEXT", "&habcdef", INI_TYPE_Hex, strOut);
    switch (i){
    case -2:
        // Broken-Token
        printf("Broken Token!: ");
        break;
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
    printf("%s<-\n\n", strOut);    

//return 0;
    strcpy(strOut, "                    CaptionText = \"FixedTop\"");
    IniReplaceLine("desktops.ini",strOut,52);
    //strcpy(strOut, "global.definitions.fixedareas.1.top.captioncolorid");
    i = IniGetValue("desktops.ini", "global.definitions.fixedareas.1.top.captioncolorid", "1,234moff", INI_TYPE_Float, strOut);
    switch (i){
    case -2:
        // Broken-Token
        printf("Broken Token!: ");
        break;
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
    printf("%s<-\n\n", strOut);    

    strcpy(strOut, "global.definitions.fixedareas.1.top.captiontext");
    i = IniGetValue("desktops.ini", "global.definitions.fixedareas.1.top.captiontext", "moff1234", INI_TYPE_Float, strOut);
    switch (i){
    case -2:
        // Broken-Token
        printf("Broken Token!: ");
        break;
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
    printf("%s<-\n\n", strOut);

    strcpy(strOut, "Value = 1234          # My Remark");
    i = IniGetRemark(strOut);    
    printf("%s<- :%d\n\n", strOut, i);

    strcpy(strOut, "        Value = 1234          # My Remark");
    printf("%s\n", strOut);
    char strVal[255];
    strcpy(strVal, "12340");
    IniChangeValueLine(strOut, strVal, 0);
    printf("%s\n\n", strOut);

    strcpy(strOut, "        Value = 1234          # My Remark");
    printf("%s\n", strOut);
    strcpy(strVal, "123401");
    IniChangeValueLine(strOut, strVal, 0);
    printf("%s\n\n", strOut);

    strcpy(strOut, "        Value = 1234          # My Remark");
    printf("%s\n", strOut);
    strcpy(strVal, "1234012");
    IniChangeValueLine(strOut, strVal, 0);
    printf("%s\n\n", strOut);

    strcpy(strOut, "        Value = 1234          # My Remark");
    printf("%s\n", strOut);
    strcpy(strVal, "12340123");
    IniChangeValueLine(strOut, strVal, 0);
    printf("%s\n\n", strOut);

    strcpy(strOut, "        Value = 1234          # My Remark");
    printf("%s\n", strOut);
    strcpy(strVal, "123401234");
    IniChangeValueLine(strOut, strVal, 0);
    printf("%s\n\n", strOut);

    strcpy(strOut, "        Value = 1234          # My Remark");
    printf("%s\n", strOut);
    strcpy(strVal, "1234012345");
    IniChangeValueLine(strOut, strVal, 0);
    printf("%s\n\n", strOut);

    strcpy(strOut, "        Value = 1234          # My Remark");
    printf("%s\n", strOut);
    strcpy(strVal, "12340123456");
    IniChangeValueLine(strOut, strVal, 0);
    printf("%s\n\n", strOut);

    strcpy(strOut, "        Value = 1234          # My Remark");
    printf("%s\n", strOut);
    strcpy(strVal, "123401234567");
    IniChangeValueLine(strOut, strVal, 0);
    printf("%s\n\n", strOut);

    strcpy(strOut, "        Value = 1234          # My Remark");
    printf("%s\n", strOut);
    strcpy(strVal, "1234012345678");
    IniChangeValueLine(strOut, strVal, 0);
    printf("%s\n\n", strOut);

    strcpy(strOut, "        Value = 1234          # My Remark");
    printf("%s\n", strOut);
    strcpy(strVal, "12340123456789");
    IniChangeValueLine(strOut, strVal, 0);
    printf("%s\n\n", strOut);

    strcpy(strOut, "        Value = 1234          # My Remark");
    printf("%s\n", strOut);
    strcpy(strVal, "123401234567890");
    IniChangeValueLine(strOut, strVal, 0);
    printf("%s\n\n", strOut);

    strcpy(strOut, "        Value = 1234          # My Remark");
    printf("%s\n", strOut);
    strcpy(strVal, "1234012345678901");
    IniChangeValueLine(strOut, strVal, 0);
    printf("%s\n\n", strOut);

    strcpy(strOut, "        Value = 1234          # My Remark");
    printf("%s\n", strOut);
    strcpy(strVal, "12340123456789012");
    IniChangeValueLine(strOut, strVal, 0);
    printf("%s\n\n", strOut);

    strcpy(strOut, "        Value = 1234          # My Remark");
    printf("%s\n", strOut);
    strcpy(strVal, "123401234567890123");
    IniChangeValueLine(strOut, strVal, 0);
    printf("%s\n\n", strOut);

    strcpy(strOut, "        Value = 1234          # My Remark");
    printf("%s\n", strOut);
    strcpy(strVal, "1234012345678901234");
    IniChangeValueLine(strOut, strVal, 0);
    printf("%s\n\n", strOut);

    strcpy(strOut, "        Value = 1234          # My Remark");
    printf("%s\n", strOut);
    strcpy(strVal, "12340123456789012345");
    IniChangeValueLine(strOut, strVal, 0);
    printf("%s\n\n", strOut);

    strcpy(strOut, "        Value = 1234          # My Remark");
    printf("%s\n", strOut);
    strcpy(strVal, "123401234567890123456");
    IniChangeValueLine(strOut, strVal, 0);
    printf("%s\n\n", strOut);

    strcpy(strOut, "        Value = 1234          # My Remark");
    printf("%s\n", strOut);
    strcpy(strVal, "5678");
    IniChangeValueLine(strOut, strVal, 0);
    printf("%s\n\n", strOut);

    strcpy(strOut, "        Value = 1234          # My Remark");
    printf("%s\n", strOut);
    strcpy(strVal, "123");
    IniChangeValueLine(strOut, strVal, 0);
    printf("%s\n\n", strOut);

    strcpy(strOut, "        Value = 1234          # My Remark");
    printf("%s\n", strOut);
    strcpy(strVal, "12");
    IniChangeValueLine(strOut, strVal, 0);
    printf("%s\n\n", strOut);

    strcpy(strOut, "        Value = 1234          # My Remark");
    printf("%s\n", strOut);
    strcpy(strVal, "1");
    IniChangeValueLine(strOut, strVal, 0);
    printf("%s\n\n", strOut);

    strcpy(strOut, "global1.definitions.fixedareas.1.top.captiontext");
    i = IniGetValue("desktops.ini", "global1.definitions.fixedareas.1.top.captiontext", "1moff1", INI_TYPE_Int, strOut);
    switch (i){
    case -2:
        // Broken-Token
        printf("Broken Token!: ");
        break;
    case 0:
        // no value found
        printf("Value not found!: ");
        break;
    default:
        // unexpected found
        printf("Unexpected!: ");
        break;
    }
    printf("%s<-:%d\n\n", strOut, i);

    i = IniGetValue("desktops.ini", "global.definitions1.fixedareas.1.top.captiontext", "12moff2", INI_TYPE_Bool, strOut);
    switch (i){
    case -2:
        // Broken-Token
        printf("Broken Token!: ");
        break;
    case 0:
        // no value found
        printf("Value not found!: ");
        break;
    default:
        // unexpected found
        printf("Unexpected!: ");
        break;
    }
    printf("%s<-:%d\n\n", strOut, i);

    i = IniGetValue("desktops.ini", "global.definitions.fixedareas1.1.top.captiontext", "0moff3", INI_TYPE_Bool, strOut);
    switch (i){
    case -2:
        // Broken-Token
        printf("Broken Token!: ");
        break;
    case 0:
        // no value found
        printf("Value not found!: ");
        break;
    default:
        // unexpected found
        printf("Unexpected!: ");
        break;
    }
    printf("%s<-:%d\n\n", strOut, i);

    i = IniGetValue("desktops.ini", "global.definitions.fixedareas.11.top.captiontext", "TrUemoff4", INI_TYPE_Bool, strOut);
    switch (i){
    case -2:
        // Broken-Token
        printf("Broken Token!: ");
        break;
    case 0:
        // no value found
        printf("Value not found!: ");
        break;
    default:
        // unexpected found
        printf("Unexpected!: ");
        break;
    }
    printf("%s<-:%d\n\n", strOut, i);

    i = IniGetValue("desktops.ini", "global.definitions.fixedareas.1.top1.captiontext", "moff5", INI_TYPE_Bool, strOut);
    switch (i){
    case -2:
        // Broken-Token
        printf("Broken Token!: ");
        break;
    case 0:
        // no value found
        printf("Value not found!: ");
        break;
    default:
        // unexpected found
        printf("Unexpected!: ");
        break;
    }
    printf("%s<-:%d\n\n", strOut, i);

    i = IniGetValue("desktops.ini", "global.definitions.fixedareas.1.top.captiontext1", "12,34moff6", INI_TYPE_Float, strOut);
    switch (i){
    case -2:
        // Broken-Token
        printf("Broken Token!: ");
        break;
    case 0:
        // no value found
        printf("Value not found!: ");
        break;
    default:
        // unexpected found
        printf("Unexpected!: ");
        break;
    }
    printf("%s<-:%d\n\n", strOut, i);


    strcpy(strOut, "global2.definitions.fixedareas.1.top.captiontext");
    IniSetValue("desktops.ini", strOut, "moff2", 0);

    strcpy(strOut, "global.definitions2.fixedareas.1.top.captiontext");
    IniSetValue("desktops.ini", strOut, "1234", 1);

    strcpy(strOut, "global.definitions.fixedareas2.1.top.captiontext");
    IniSetValue("desktops.ini", strOut, "1234,5678", 2);

    strcpy(strOut, "global.definitions.fixedareas.12.top.captiontext");
    IniSetValue("desktops.ini", strOut, "&Hff", 3);

    strcpy(strOut, "global.definitions.fixedareas.1.top2.captiontext");
    IniSetValue("desktops.ini", strOut, "moff2", 4);

    strcpy(strOut, "global.definitions.fixedareas.1.top.captiontext2");
    IniSetValue("desktops.ini", strOut, "0", 5);


    strcpy(strOut, "global3.definitions.fixedareas.1.top.captiontext");
    IniSetValue("desktops.ini", strOut, "1234", 0);

    strcpy(strOut, "global.definitions3.fixedareas.1.top.captiontext");
    IniSetValue("desktops.ini", strOut, "ABCD", 1);

    strcpy(strOut, "global.definitions.fixedareas3.1.top.captiontext");
    IniSetValue("desktops.ini", strOut, "12345678", 2);

    strcpy(strOut, "global.definitions.fixedareas.13.top.captiontext");
    IniSetValue("desktops.ini", strOut, "0xFf", 3);

    strcpy(strOut, "global.definitions.fixedareas.1.top3.captiontext");
    IniSetValue("desktops.ini", strOut, "1234", 4);

    strcpy(strOut, "global.definitions.fixedareas.1.top.captiontext3");
    IniSetValue("desktops.ini", strOut, "tRuE", 5);




return 0;
}
