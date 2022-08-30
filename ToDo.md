Def_ ToDo

<br>

**013. WaitForESC27**
- Catch 2nd Level Mouse Commands 

<br>

**012. CLOCKS_PER_SECOND is not working as expected...**
- done

<br>

**011. ADD FocusIn / FocusOut**
- ~~https://invisible-island.net/xterm/ctlseqs/ctlseqs.htmlDef_h3-Any-event-tracking~~
- done

<br>

**010. Missing enumerations -- Key-IDs**
- But who knows if I ever use them ?!?

<br>

**009. ClearLine & ClearPartial -- FromTo**
- done

<br>

**008. Add TerminalSizeChangeEVENT**
- ~~Is there anything possible, better than just polling the Screen?  
Ok ! We could put the processing on the HMI-Terminal, but not on the µCs! µCs just get the Event via a regular ESC-Screensize-Command after Terminal recognized it...  
µCs must be able to do, too! Otherwise we lose the "just a Terminal-Option" (without any kind of Win/Mac/Linux binaries)~~
- ~~MouseXY > ScreenXY is a nice indicator...~~
- ~~FocusIn/FocusOut (see 011.) is a nice indicator...~~
- ~~HAVE AN EYE on Minimize - Effects... *take care on different Terminals*~~  
**Send GetTerminalSize after GotFocus was the simple solution**
- done

<br>

**007. Add "Alt-Back"**
- ~~done~~ 

<br>

**006. EVENT - HANDLER**
- Mouse - almost done
- Keys - almost done

<br>

**005. Constants for ESC & Mouse Timing**
- ~~done. but with Variables~~

<br>

**001. Separate BinaryMouse Up's OR equalize DecimalMouse Ups**
- ~~recognizer for active button~~

<br>

**002. Click & DblClick**
- ~~recognizer/tracker for active down-button and position~~
- ~~'use' UsrESC recognizer (follow 4 click) - *position*~~
- ~~'use' UsrESC recognizer (timing 4 dblClick) - *position*~~
- ~~move from ESCsequence auto-disables click/dbClick~~

<br>

**003. Mouse Selection**
- ~~Up isn't Down...~~

<br>

**004. ByteMouseBug X/Y greater 255**
- ~~It starts, that the axis is deleted in terminal -> TUI communication~~  **fixed** (covered by UsrEscLenErr)  
~~then it flips to ByteValues smaller 32  "01" & "02"~~ **fixed** (covered by ByteModePositionErr)  
then after "some" of the following sequence it's a "???" what kills the app
27: ESC  
91: [  
77: M  
67: C   
2  
51: 3  
  : ByteModePositionError	[MC3

<br>

**00n. Numerate ESC Events an un-trashy way**  
- *I'm reverse-engineering for... I don't know anymore*

<br>

**00n. Check UTF (195)**  
- !! Take Care on BinaryMouseMode !!

<br>

**Thoughts about the CMD/Interprtation Structure**

Out- & Inputs
- "OUT1 - OUTn" 
- "IN1 - INn"

After Function brackets... Results To...
- ""   Stack-0
- "~n" Stack-1-n
- "@n" Mem-n

| ID | Command(ID) | ( |(ParameterList) | ) | ResultTo () | Additional Memory Needs |
|-|-|-|-|-|-|-|
|Def|Def_OutSetting|**(**|Name, OutPort-Address, OutPort-Type, 1stPortOnAdr, PortCnt, ReWriteInterval|**)**||
|Def|Def_Out|**(**|Name, OutSettingName, 1stPortOnAdr, PortCnt, Invert|**)**||
|Def|Def_InSetting|**(**|Name, InPort-Address, InPort-Type, 1stPortOnAdr, PortCnt, ReadInterval|**)**||
|Def|Def_In|**(**|Name, InSettingName, 1stPortOnAdr, PortCnt, Invert|**)**||
|Def|Def_Var|**(**|Name, MemPos, VarType, VarsCnt|**)**||
|Def|Def_Const|**(**|Name, Value|**)**||
|Def|Def_Date|**(**|Name, DD.MM.YYYY|**)**||
|Def|Def_Time|**(**|Name, hh:mm:ss|**)**||
|Def|Def_DateTime|**(**|Name, DD.MM.YYYY, hh:mm:ss|**)**||
|-|-|-|-|-|-|-|
|GPIO|Out (Set)|**(**|FollowPort, OutPort-Id|**)**||
|GPIO|Out (Get)|**(**|OutPort-Id|**)**||
|GPIO|In (Get)|**(**|InPort-Id|**)**||
|-|-|-|-|-|-|-|
|SYS|SecondChanged|**(**||**)**| | |
|SYS|Min1Changed|**(**||**)**| | |
|SYS|Min2Changed|**(**||**)**| | |
|SYS|Min3Changed|**(**||**)**| | |
|SYS|Min4Changed|**(**||**)**| | |
|SYS|Min5Changed|**(**||**)**| | |
|SYS|Min6Changed|**(**||**)**| | |
|SYS|Min10Changed|**(**||**)**| | |
|SYS|Min12Changed|**(**||**)**| | |
|SYS|Min15Changed|**(**||**)**| | |
|SYS|Min20Changed|**(**||**)**| | |
|SYS|Min30Changed|**(**||**)**| | |
|SYS|Hour1Changed|**(**||**)**| | |
|SYS|Hour2Changed|**(**||**)**| | |
|SYS|Hour3Changed|**(**||**)**| | |
|SYS|Hour4Changed|**(**||**)**| | |
|SYS|Hour6Changed|**(**||**)**| | |
|SYS|Hour8Changed|**(**||**)**| | |
|SYS|Hour12Changed|**(**||**)**| | |
|SYS|DayChanged|**(**||**)**| | |
|-|-|-|-|-|-|-|
||If|**(**|FollowPort|**)**|| NOT-ID |
||ElseIf|**(**|FollowPort|**)**|| NOT-ID |
||Else|**(**||**)**||
||EndIf|**(**||**)**| |
||Label|**(**|LabelName|**)**| |
||GoTo|**(**|LabelName|**)**| |
||GoSub|**(**|LabelName|**)**| | |
||Return|**(**||**)**| |Return-ID|
||Do|**(**|FollowPort|**)**| |NOT-ID|
||Loop|**(**|FollowPort|**)**| |Return-ID
||ExitLoop|**(**|FollowPort|**)**| |NOT-ID
|-|-|-|-|-|-|-|
||+ / Add|**(**|1stVal, (+) 2ndVal|**)**| |
||++|**(**|Val|**)**| |
||- / Sub|**(**|1stVal, (-) 2ndVal|**)**| |
||--|**(**|Val|**)**| |
||* / Mul|**(**|1stVal, (*) 2ndVal|**)**| |
||/ / Div|**(**|1stVal, (/) 2ndVal|**)**| |
||Sum|**(**|1stVal, LastVal|**)**| |
|-|-|-|-|-|-|-|
||=|**(**|1stVal, 2ndVal|**)**| | |
||=M|**(**|1stVal, LastVal|**)**| | |
||<|**(**|1stVal, 2ndVal|**)**| | |
||<M|**(**|1stVal, LastVal|**)**| | |
||>|**(**|1stVal, 2ndVal|**)**| | |
||>M|**(**|1stVal, LastVal|**)**| | |
||<=|**(**|1stVal, 2ndVal|**)**| | |
||<=M|**(**|1stVal, LastVal|**)**| | |
||>=|**(**|1stVal, 2ndVal|**)**| | |
||>=M|**(**|1stVal, LastVal|**)**| | |
||!=|**(**|1stVal, 2ndVal|**)**| | |
||!=M|**(**|1stVal, LastVal|**)**| | |
|-|-|-|-|-|-|-|
||Or|**(**|1stVal, 2ndVal|**)**| |
||OrM|**(**|1stVal, LastVal|**)**| |
||And|**(**|1stVal, 2ndVal|**)**| |
||AndM|**(**|1stVal, LastVal|**)**| |
||Xor|**(**|1stVal, 2ndVal|**)**| |
||Not|**(**|Val|**)**| |
|-|-|-|-|-|-|-|
||Min|**(**|1stVal, 2ndVal|**)**| |
||Max|**(**|1stVal, 2ndVal|**)**| |
||Mid|**(**|1stVal, 2ndVal|**)**| |
||InBetween|**(**|Val, LowVal, HighVal|**)**| |
||MinM|**(**|1stVal, LastVal|**)**| |
||MaxM|**(**|1stVal, LastVal|**)**| |
||MidM|**(**|1stVal, LastVal|**)**| |
||InBetweenM|**(**|1stVal, LastVal, LowVal, HighVal|**)**| |
||Avg|**(**|Val, Count, High, Low|**)**| |Count times the Val, CountPos|
|-|-|-|-|-|-|-|
||Date|**(**|DD.MM.YYYY|**)**| | |
||Time|**(**|hh:mm:ss|**)**| | |
||DateTime|**(**|DD.MM.YYYY, hh:mm:ss|**)**| | |
||NowWeekDay|**(**||**)**| | |
||NowDate|**(**||**)**| | |
||NowTime|**(**||**)**| | |
||NowDateTime|**(**||**)**| | |
||DaysSince|**(**|DD.MM.YYYY, hh:mm:ss, Hrs/Day|**)**| | |
|-|-|-|-|-|-|-|
||Timer24h|**(**|OnTime, OffTime|**)**| | |
||DayTimer|**(**|OnTime, OffTime, Hrs/Day, (StartDate, StartTime)|**)**| | |
||Interval|**(**|OnTime, OffTime, Offset|**)**| | |
||IntervalStep|**(**|OnTime, OffTime, Repeats, StepTime, Offset|**)**| | |
||OnDelay|**(**|Val, OnDelayTime|**)**| | WentOnTime |
||OffDelay|**(**|Val, OffDelayTime|**)**| | WentOffTime |
||OnOffDelay|**(**|Val, OnDelayTime, OffDelayTime|**)**| | WentOnTime, WentOffTime |
|-|-|-|-|-|-|-|



10  &emsp; Or (IN1,IN2)>  
10  &emsp; Or (IN5,IN6):  
20  &emsp; If (>,NO-GoTo-Label)  
30  &emsp;&emsp; Or (IN3, IN4).  
40  &emsp;&emsp; Out (>, 1)  
40  &emsp;&emsp; GoTo (Label2)  
50  &emsp; EndIf (Label)  
20  &emsp; (Else)If (>,NO-GoTo-Label2)  
30  &emsp;&emsp; Or (IN3, IN4).  
40  &emsp;&emsp; Out (>, 1)  
50  &emsp; EndIf (Label2)  



<br>

`code`  

                    Label Add     Level
    if                    1         1
      if                  1         2
        if                1         3

        elseif      1     0         3

        else        2     0         3

        endif       3    -1         2 
      
      elseif        4     0         2

      elseif        5     0         2

      else          6     0         2

      endif         7    -1         1

    elseif          8     0         1

    else            9     0         1

      if            10    1         2

      else          11    0         2

      endif         12    0         2

    endif           13   -1         1

`code`  

    Function(Function(Function(Function(A,B),Function(Function(c,d),D))))
    Function(Function(Function(  1stResult  ,Function(Function(c,d),D))))
    Function(Function(Function(  1stResult  ,Function(  2ndResult  ,D))))
    Function(Function(Function(  1stResult  ,        3rdResult        )))
    Function(Function(                    4thResult                    ))
