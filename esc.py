# self._dblTop = False            # If it's a double height line, is it the top line?

class TXTcolor:
    def __init__(self):
        self._type = 16                 # (16) colors / (255) colors / (24) rgb colors
        self._r = 0                     # 0-255  /  red for rgb  / color-no. for 16 / 255  
        self._g = 0
        self._b = 0

    @property
    def type(self):
        return self._type
    @type.setter
    def type(self, value):
        if value in (16, 255, 24):
            self._type = value

    @property
    def r(self):
        return self._r
    @r.setter
    def r(self, value):
        if self._type == 16:
            if 0 <= value <= 15:
                self._r = value
        else:
            if 0 <= value <= 255:
                self._r = value

    @property
    def g(self):
        return self._g
    @g.setter
    def g(self, value):
        if self._type == 16:
            if 0 <= value <= 15:
                self._g = value
        else:
            if 0 <= value <= 255:
                self._g = value
    
    @property
    def b(self):
        return self._b
    @b.setter
    def b(self, value):
        if self._type == 16:
            if 0 <= value <= 15:
                self._b = value
        else:
            if 0 <= value <= 255:
                self._b = value

    @property
    def color(self):
        return self._r
    @color.setter
    def color(self, value):
        self.r = value

    @property
    def rgb(self):
        return (self._r, self._g, self._b)
    @rgb.setter
    def rgb(self, red, green, blue):
        if self._type == 24:
            self.r = red
            self.g = green
            self.b = blue

class TXTstyle:
    def __init__(self):
        self._bold = False
        self._faint = False
        self._italic = False
        self._fraktur = False
        self._inverse = False
        self._hidden = False
        self._underline = False
        self._dblUnderline = False
        self._curlUnderline = False
        self._dotUnderline = False
        self._dashUnderline = False
        self._dashdotUnderline = False
        self._overline = False
        self._strikethrough = False
        self._subscript = False
        self._superscript = False
        self._dblHeight = False
        self._simDblHeight = False
        self._dblWidth = False
        self._simDblWidth = False
        self._blink = False
        self._blinkFast = False
        self._boxMode = False

class Cursor:
    def __init__(self):
        self._x = 1
        self._y = 1
        self._visible = True

class Terminal:
    def __init__(self):
        self._width = 80
        self._height = 24
        self._raw = False
        self._echo = True
        self._iOS = False               # a synonym for crashed Linux-VT
        self._aShell = False            # running in a-shell (activates iOS)
        self._cursor = Cursor()         # cursor position
        self._canStyle = TXTstyle()     # style capabilities
        self._style = TXTstyle()        # current style
        self._fg = TXTcolor()           # foreground color
        self._bg = TXTcolor()           # background color
        self._ul = TXTcolor()           # underline color (255 & rgb)

                