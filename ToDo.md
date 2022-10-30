**ToDo**

<br>

**035. DblClick TimeOut (on Mac - ? but rather CPU-depending ?)**
- Raspi on table...
- ~~generic Area-Tolerance during DblClick time...~~
- M1 Mac suffers like an i5 Mac - so, it's rather not the CPU...
- MAC - MouseDown comes "too" late - oh this explains a lot - damn...  
but this differs also between regular/magic/airpad - mouse
- MAC - (sometimes) Mouse-Up before MouseDown while DblClicking


<br>

**034. Change GetESC27 (-6)-Response to a positive response...**
- ~~*faster exit...*~~
- ~~the (-6) - solution is much too good to read/understandable (for me)~~  
~~I give a shit on the two "if(){}" which could be faster...~~

<br>

**033. While 025. ...**
- ~~CR / LF - splittet - **right return missing**~~
- ~~if (!(c == 82 && gScreenSizeInCursorPos || gCursorWaitFor)){ **MOFF**~~

<br>

**032. ANSI ESC Injection**  
*I found a guy who investigated malleolus ESC-Sequences. It led me to multiple cognations.*  
- ~~The byte-wise approach of GetESC27() to rather declare a sequence as early as possible as 'broken' than to wait for something with sense - and to never wait for an 'end of sequence' was smart...~~  
  - ~~But it's hardcore missing a 'FlushInKey()' after recognizing a broken or regular sequence...~~  
**This Flush must stop on next ESC!**
- ~~Cause this base-concept seems for security reasons very worth to stay as it is, it now gets attention!~~  
  - ~~there are at least plenty IFs which should be switched...~~

<br>

**031. Next Time On A Mac...**
- ~~Find reason for UserKey slowness (99% it's compile/debug related~~  
~~or the '333333'-scale is 'wrong' on Macs~~  
~~!! But SecondChangeEvent is right !!)~~

<br>

**030. Do 'something' with Ctrl-C**
- ~~ooops... need full implementation of all Ctrl-A - Ctrl-Z~~

<br>

**029. Just MAC! Something messy with ZSH & Sleep/WakeUp...**
- ~~done~~ *(trailing space - overflow seconds...)*

<br>

**028. Just Billy-OS! Something messy with '\n' at Quit-Procedure**
- ~~done~~

<br>

**027. Move ToDo to GitHub-Issues...**

<br>

**026. Normalize variable Naming**
- **~~!!! Do 022. first !!!~~**
- ~~pXYZ - pointer~~
- ~~gXYZ - global~~
- ~~gpXYZ - global pointer~~
- ~~lXYZ - local~~
- **little HowTo to the whole naming thing**

<br>

**025. Esc27Events change to alt+shift+ctrl flags**
- ~~Esc27Events change to alt+shift+ctrl flags~~
- ~~identify missing MAC-Key-Sequences *not really anymore*~~

<br>

**024. Optimize CheckOnTimeChange() within core-loop**
- **~~!!! Do 022. first !!!~~**
- ~~look for signal (On timer) sets the flag~~
- ~~move flag-check into idle-time~~
- ~~check signal() for calling "function(set)-see 022." on exit and err cases~~

<br>

**023. Check on if the "silly-thing" in InKey() is gone with introduced FlushInKey()...**
- **~~!!! Do 022. first !!!~~**
- ~~done~~ (after tested)

<br>

**022. Check for MAC and CLEAN UP**
- **~~nothing else anymore before this isn't done!~~**
- (1) ~~remove case 109 & 158 within core-loop~~
- (4) ~~declaration of all functions in all *.h~~
- (5) ~~add function-header to all functions (look for vs-code documentation addin)~~ (almost done)
- ~~(3) Init/Exit processes into function(set)~~ (actually still part of main())
  - ~~c_break +etc. globals to statics in function~~
  - ~~add serious error-handling~~
- (2) ~~Use UserESC-Timeout for recognizing unfinished but valid (so, broken & unknown) sequences~~  
~~use the missing Mac-Keys (see 025.) to praxis-test...~~  
*~~(019. wasn't fully done - just inside GetESC27() but not inside Core-Loop)~~*


<br>

**021. Use 1st GetTerminalSize()-Result to define the HowTo-CLS**
- ~~static int saves last non-zero function-call~~
- ~~done~~

<br>

**020. Make an InKey-Function from getch/getch mess**
- ~~done~~ (together with SetVT())

<br>

**019. User ESC TimeOut can be used to recognize and abort "open/broken" Sequences, too!**
- ~~done~~ (was existent 'from binary mouse' - but optimized now)

<br>

**018. F3 + Combination from one or more Shift-Alt-Ctrl**

    // Shift OR Alt OR Ctrl + F1 - F4
	// F3 could be CurserPos, too... WTF !
	// 	CSI 1;2 R  -  Shift
	// 	CSI 1;3 R  -  Alt
	// 	CSI 1;4 R  -  ShiftAlt
	// 	CSI 1;5 R  -  Ctrl
	// 	CSI 1;6 R  -  ShiftCtrl
	// 	CSI 1;7 R  -  AltCtrl
	// 	CSI 1;8 R  -  ShiftAltCtrl
	// Previous 7 Combinations Are CursorPositions, too... WTF !

  - ~~Need of a **Next 'R' Is A CursorPos** - Flag~~
  - ~~done~~

<br>

**017. WIN DoEvents() is still a mess**
- ~~Check SwitchToThread() vs Sleep()~~
- ~~done~~

<br>

**016. WIN MS-Terminal Git-Issue "TerminalSize In Chars"**
- https://github.com/microsoft/terminal/issues/13944
- ~~temporary fix needed (kernel-function - or ESC CursorPosTilt)~~
- **~~On Start~~**  
~~we check on Terminal Size~~  
1. ~~`CSI 18 t`~~
2. ~~`ESC7 CSI9999;999H CSI6n ESC8`~~
3. ~~OS functions~~  
~~Check on fail/success (from 1-3) gives us almost all we need to know about the used Terminal...~~
- ~~done~~
<br>

**015. Mac & WIN TerminalSizeChanged**
- ~~**This need Signal()** - forget the rest...~~
- ~~Mac: Lost- and GotFocus doesn't happened while changing the Size  
so, it may depend on Linux also on the WindowManager (I'm just using IceWM)~~
- ~~WIN: Lost- and GotFocus doesn't happened while changing the Size~~
- ~~done~~ - WITHOUT Signal()...


<br>

**015 is definitly a THING**... shit...
- ~~https://www.rkoucha.fr/tech_corner/sigwinch.html~~
- ~~https://www.geeksforgeeks.org/write-a-c-program-that-doesnt-terminate-when-ctrlc-is-pressed/~~
- ~~https://manpages.ubuntu.com/manpages/bionic/de/man7/signal.7.html~~

<br>

**014. Mac MouseScroll Y-Axis value isn't stable**  
- even protocol is perfect (no char loss)  
so, it seems (actually) that the Terminal is doing the shit

<br>

**013. WaitForESC27**
- ~~Catch 2nd Level Mouse Commands~~ 
- ~~done~~
<br>

**012. CLOCKS_PER_SECOND is not working as expected...**
- ~~done~~

<br>

**011. ADD FocusIn / FocusOut**
- ~~https://invisible-island.net/xterm/ctlseqs/ctlseqs.htmlDef_h3-Any-event-tracking~~
- ~~done~~

<br>

**010. Missing enumerations -- Key-IDs**
- But who knows if I ever use them ?!?  
I will (get rid of Deci-Numbers in EventHandler)

<br>

**009. ClearLine & ClearPartial -- FromTo**
- ~~done~~

<br>

**008. Add TerminalSizeChangeEVENT**
- ~~Is there anything possible, better than just polling the Screen?  
Ok ! We could put the processing on the HMI-Terminal, but not on the µCs! µCs just get the Event via a regular ESC-Screensize-Command after Terminal recognized it...  
µCs must be able to do, too! Otherwise we lose the "just a Terminal-Option" (without any kind of Win/Mac/Linux binaries)~~
- ~~MouseXY > ScreenXY is a nice indicator...~~
- ~~FocusIn/FocusOut (see 011.) is a nice indicator...~~
- ~~HAVE AN EYE on Minimize - Effects... *take care on different Terminals*~~  
**~~Send GetTerminalSize after GotFocus was the simple solution~~**  
*which was just working with Linux - see 015*
- ~~done~~

<br>

**007. Add "Alt-Back"**
- ~~done~~ 

<br>

**006. EVENT - HANDLER**
- ~~Mouse - almost done~~
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
~~then it flips to ByteValues smaller 32  "01" & "02" **fixed** (covered by ByteModePositionErr)  
then after "some" of the following sequence it's a "???" what kills the app~~  
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

**Thoughts about the CMD/Interpretation Structure**

Out- & Inputs
- "OUT1 - OUTn" 
- "IN1 - INn"

After Function brackets... Results To...
- ""   Stack-0
- "~n" Stack-1-n
- "@n" Mem-n

| ID | Command(ID) | ( |(ParameterList) | ) | ResultTo () | Additional Memory Needs |
|-|-|-|-|-|-|-|
|Def|Def_OutAddr|**(**|Name, OutPort-Address, OutPort-Type, 1stPortOnAdr, PortCnt, ReWriteInterval|**)**||
|Def|Def_OutPrt|**(**|Name, OutSettingName, 1stPortOnAdr, PortCnt, Invert|**)**||
|Def|Def_InAddr|**(**|Name, InPort-Address, InPort-Type, 1stPortOnAdr, PortCnt, ReadInterval|**)**||
|Def|Def_InPrt|**(**|Name, InSettingName, 1stPortOnAdr, PortCnt, Invert|**)**||
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
||=M|**(**|RefVal, 1stVal, LastVal|**)**| | |
||<|**(**|1stVal, 2ndVal|**)**| | |
||<M|**(**|RefVal, 1stVal, LastVal|**)**| | |
||>|**(**|1stVal, 2ndVal|**)**| | |
||>M|**(**|RefVal, 1stVal, LastVal|**)**| | |
||<=|**(**|1stVal, 2ndVal|**)**| | |
||<=M|**(**|RefVal, 1stVal, LastVal|**)**| | |
||>=|**(**|1stVal, 2ndVal|**)**| | |
||>=M|**(**|RefVal, 1stVal, LastVal|**)**| | |
||!=|**(**|1stVal, 2ndVal|**)**| | |
||!=M|**(**|RefVal, 1stVal, LastVal|**)**| | |
|-|-|-|-|-|-|-|
||Or|**(**|1stVal, 2ndVal|**)**| |
||OrM|**(**|RefVal, 1stVal, LastVal|**)**| |
||And|**(**|1stVal, 2ndVal|**)**| |
||AndM|**(**|RefVal, 1stVal, LastVal|**)**| |
||Xor|**(**|1stVal, 2ndVal|**)**| |
||Not|**(**|Val|**)**| |
|-|-|-|-|-|-|-|
||Min|**(**|1stVal, 2ndVal|**)**| |
||Max|**(**|1stVal, 2ndVal|**)**| |
||Mid|**(**|1stVal, 2ndVal|**)**| |
||Inside|**(**|Val, LowVal, HighVal|**)**| |
||MinM|**(**|1stVal, LastVal|**)**| |
||MaxM|**(**|1stVal, LastVal|**)**| |
||MidM|**(**|1stVal, LastVal|**)**| |
||InsideM|**(**|1stVal, LastVal, LowVal, HighVal|**)**| |
||Avg|**(**|Val, Count, High, Low|**)**| |Count times the Val, CountPos|
|-|-|-|-|-|-|-|
||Date|**(**|DD.MM.YYYY|**)**| | |
||Time|**(**|hh:mm:ss|**)**| | |
||DateTime|**(**|DD.MM.YYYY, hh:mm:ss|**)**| | |
||NowDay|**(**||**)**| | |
||NowDate|**(**||**)**| | |
||NowTime|**(**||**)**| | |
||NowDateTime|**(**||**)**| | |
||DaysSince|**(**|DD.MM.YYYY, hh:mm:ss, Hrs/Day|**)**| | |
|-|-|-|-|-|-|-|
||Timer24h|**(**|OnTime, OffTime|**)**| | |
||DayTimer|**(**|OnTime, OffTime, Hrs/Day, (StartDate, StartTime)|**)**| | |
||Interval|**(**|OnTime, OffTime, Offset|**)**| | |
||InterStep|**(**|OnTime, OffTime, Repeats, StepTime, Offset|**)**| | |
||OnDelay|**(**|ValIn, OnDelayTime|**)**| | WentOnTime |
||OffDelay|**(**|ValIn, OffDelayTime|**)**| | WentOffTime |
||OnOffDelay|**(**|ValIn, OnDelayTime, OffDelayTime|**)**| | WentOnTime, WentOffTime |
|-|-|-|-|-|-|-|
||CountHigh|**(**|ValIn, Start, Step, ResetBit, ResetVal|**)**| |ActVal|
||CountLow|**(**|ValIn, Start, Step, ResetBit, ResetVal|**)**| |ActVal|
||CountChange|**(**|ValIn, Start, Step, ResetBit, ResetVal|**)**| |ActVal|
|-|-|-|-|-|-|-|
||UserCtrl|**(**|ValIn, Mode|**)**| | Mode, OnUntil, OffUntil |
||UserIn|**(**|Name, ValBoot|**)**| | ValRun |
||UserOut|**(**|Name|**)**| | Val |
||UserVal|**(**|Name, ValBoot, Min, Max|**)**| | ValRun |



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

`code`

    12345678901234567890123456789012345678901234567890123456789012345678901234567890
    0000 | Function(UserA,UserB,UserC)UserD         | Function(@1,@2,@3)@4 | 000 (000,000,000,000)