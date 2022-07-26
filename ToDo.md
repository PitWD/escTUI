# ToDo  

<br>

**006. EVENT - HANDLER**
- Mouse

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

