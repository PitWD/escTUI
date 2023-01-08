#include <stdio.h>
#include "ini.h"

void Print_IniGetValue( const char *strSearch, const char *strDefault, const int type){

    char strOut[STR_SMALL_SIZE];

    int i = IniGetValue("desktops.ini", strSearch, strDefault, type, strOut);
    switch (i){
    case -2:
        // Broken-Token
        printf("Broken Token!: ");
        break;
    case -1:
        // File Error
        printf("FileError!: ");
        break;
    case INI_TYPE_Text:
        // text value found
        printf("Text: ");
        break;
    case INI_TYPE_Bool:
        // false/true value found
        printf("True/False: ");
        break;
    case INI_TYPE_Int:
        // number value found
        printf("Int_Value: ");
        break;
    case INI_TYPE_Float:
        // number value found
        printf("Float_Value: ");
        break;
    case INI_TYPE_Hex:
        // number value found
        printf("Hex_Value: ");
        break;
    case INI_TYPE_Bin:
        // number value found
        printf("Bin_Value: ");
        break;
    case INI_TYPE_AsItIs:
        // number value found
        printf("AsItIs: ");
        break;
    default:
        // unexpected found
        printf("Unexpected!: ");
        break;
    }
    printf("->%s<-:%d\n", strOut, i);

}


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


    Print_IniGetValue("global.definitions.fixedareas.1.top.captionTEXT", "moff", INI_TYPE_Text);

    Print_IniGetValue("global.definitions.fixedareas.1.top.captioncolorid", "moff", INI_TYPE_Text);
    printf("\n");

//return 0;

    strcpy(strOut,"1234 1234567890 1234");
    StrTrimCharsLR(strOut,' ',1,1);
    printf("->%s<-\n", strOut);

    strcpy(strOut,"1234 1234567890 1234");
    StrTrimCnt_LR(strOut,4,4);
    printf("->%s<-\n\n", strOut);



    Print_IniGetValue("global.definitions.fixedareas.1.top.captionTEXT", "&habcdef", INI_TYPE_Hex);
    printf("\n");    

    Print_IniGetValue("global.definitions.fixedareas.1.top.captioncolorid", "1,234moff", INI_TYPE_Float);
    printf("\n");    

    Print_IniGetValue("global.definitions.fixedareas.1.top.captiontext", "moff1234", INI_TYPE_Float);
    printf("\n");



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

    Print_IniGetValue("global1.definitions.fixedareas.1.top.captiontext", "1moff1", INI_TYPE_Int);
    printf("\n");

    Print_IniGetValue("global.definitions1.fixedareas.1.top.captiontext", "12moff2", INI_TYPE_Bool);
    printf("\n");

    Print_IniGetValue("global.definitions.fixedareas1.1.top.captiontext", "0moff3", INI_TYPE_Bool);
    printf("\n");

    Print_IniGetValue("global.definitions.fixedareas.11.top.captiontext", "TrUemoff4", INI_TYPE_Bool);
    printf("\n");

    Print_IniGetValue("global.definitions.fixedareas.1.top1.captiontext", "moff5", INI_TYPE_Bool);
    printf("\n");

    Print_IniGetValue("global.definitions.fixedareas.1.top.captiontext1", "12,34moff6", INI_TYPE_Float);
    printf("\n");


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

    IniSet("desktops.ini", "global1.definitions.fixedareas.1.top.AsItIs", "AsItIs");
    IniSetStr("desktops.ini", "global1.definitions.fixedareas.1.top.AsSTR", "AsSTR");
    IniSetLong("desktops.ini", "global1.definitions.fixedareas.1.top.AsLong", 1234);
    IniSetInt("desktops.ini", "global1.definitions.fixedareas.1.top.AsInt", 5678);
    IniSetDouble("desktops.ini", "global1.definitions.fixedareas.1.top.AsDouble", 1.234);
    IniSetFloat("desktops.ini", "global1.definitions.fixedareas.1.top.AsFloat", 5.678);
    IniSetBool("desktops.ini", "global1.definitions.fixedareas.1.top.AsBoolTrue", 1);
    IniSetBool("desktops.ini", "global1.definitions.fixedareas.1.top.AsBoolFalse", 0);
    IniSetLongHex("desktops.ini", "global1.definitions.fixedareas.1.top.AsLongHex", 255255);
    IniSetHex("desktops.ini", "global1.definitions.fixedareas.1.top.AsHex", 255);

    IniSetLongBin("desktops.ini", "global1.definitions.fixedareas.1.top.AsLongBIN", -198198198);
    IniSetIntBin("desktops.ini", "global1.definitions.fixedareas.1.top.AsIntBIN", -198198);
    IniSetByteBin("desktops.ini", "global1.definitions.fixedareas.1.top.AsByteBIN", 198);

long l = 0;
double d = 0;
float f = 0;
unsigned char c;

    IniGet("desktops.ini", "global1.definitions.fixedareas.1.top.AsItIs", "NOPE", strOut);
    printf("AsItIs = %s\n", strOut);
    IniGetStr("desktops.ini", "global1.definitions.fixedareas.1.top.AsSTR", "NOPE", strOut);
    printf("AsSTR = %s\n", strOut);
    l = IniGetLong("desktops.ini", "global1.definitions.fixedareas.1.top.AsLong", 234);
    printf("AsLong = %ld\n", l);
    i = IniGetInt("desktops.ini", "global1.definitions.fixedareas.1.top.AsInt", 678);
    printf("AsInt = %d\n", i);
    d = IniGetDouble("desktops.ini", "global1.definitions.fixedareas.1.top.AsDouble", 2.34);
    printf("AsDouble = %f\n", d);
    f = IniGetFloat("desktops.ini", "global1.definitions.fixedareas.1.top.AsFloat", 6.78);
    printf("AsFloat = %f\n", f);
    i = IniGetBool("desktops.ini", "global1.definitions.fixedareas.1.top.AsBoolTrue", 0);
    printf("AsBoolTrue = %i\n", i);
    i = IniGetBool("desktops.ini", "global1.definitions.fixedareas.1.top.AsBoolFalse", 1);
    printf("AsBoolFalse = %i\n", i);
    l = IniGetLongHex("desktops.ini", "global1.definitions.fixedareas.1.top.AsLongHex", 16);
    printf("AsLongHex = %ld\n", l);
    i = IniGetHex("desktops.ini", "global1.definitions.fixedareas.1.top.AsHex", 32);
    printf("AsHex = %i\n", i);

    l = IniGetLongBin("desktops.ini", "global1.definitions.fixedareas.1.top.AsLongBIN", 123123123);
    printf("AsLongBIN = %ld\n", l);
    i = IniGetIntBin("desktops.ini", "global1.definitions.fixedareas.1.top.AsIntBIN", 123123);
    printf("AsIntBIN = %d\n", i);
    c = IniGetByteBin("desktops.ini", "global1.definitions.fixedareas.1.top.AsByteBIN", 123);
    printf("AsByteBIN = %d\n", c);

return 0;
}
