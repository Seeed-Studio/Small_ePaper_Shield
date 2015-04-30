Small e-Paper Shield
---------------------------------------------------------

[![Small e-Paper Shield](http://www.seeedstudio.com/depot/images/product/e%20shield_04.jpg)](http://www.seeedstudio.com/depot/small-epaper-shield-p-1597.html?cPath=34_82)

# Background
e-paper might be the most comfortable material to read. It reflects light instead of emits light to mimic the experience of conventional paper reading. And in this way much less power is consumed. Small e-paper shield is a driver shield for e-paper of small sizes. It's capable of driving e-papers of 1.44 inch, 2.0 inch and 2.7 inch and supports more than 170 languages. The upper surface of this shield is left flat and clean to give great support to the e-paper attached to it. If you are considering a lightweight and comfortable to read display, e-paper would be a good choice.
 
Attention: Since this driver board supports e-papers of different sizes. e-paper is not included in this product. We have e-paper of 2.0 inch and 2.7 inch on sale simultaneously. Go and add the most suitable one to your cart now!



# Usage:
Small e-Paper library provides complete software interfaces to exercise the capabilities of e-Paper display. There are  specific description about functions.


### 1. void begin(EPD_size sz);
The function is setup screen size.

* sz: can be EPD_1_44, EPD_2_0, EPD_2_7.


### 2. void setDirection(EPD_DIR dir);
The function is used to set the display direction.

* dir: can be DIRLEFT,DIRRIGHT,DIRNORMAL,DIRDOWN


### 3. int drawChar(char c, int x, int y);
The function can be used to display char

* c: the char you want to display. 
* x: the starting X-coordinate of char.
* y: the starting Y-coordinate of char.


### 4. int drawString(char *string, int poX, int poY);
The function can be used to display char

* string: the string you want to display. 
* poX: the starting X-coordinate of string.
* poY: the starting Y-coordinate of string.

### 5. int drawNumber(long long_num,int poX, int poY);
The function can be used to display char

* long_num: the long integer data you want to display. The max is 
* poX: the starting X-coordinate of data
* poY: the starting Y-coordinate of data


### 6. int drawFloat(float floatNumber,int decimal,int poX, int poY);
The function can be used to display float number. The display float data is rounding according to the setting decimal place.

* floatNumber: the float number you want to display. 
* decimal: set the decimal place. 
* poX: the starting X-coordinate of data.
* poY: the starting Y-coordinate of data.

### 7. int drawUnicode(unsigned int uniCode, int x, int y);
The function can be used to display a Character or a Chinese using unicode. See the page 18 to 24 of the [GT20L16P1Y datasheet](http://www.seeedstudio.com/wiki/File:GT20L16P1Y_Datasheet.pdf) to find the Char unicode, the characters include Latin、Hebrew、 Thai、Greek、 Kirill and Arabic. The Chinese unicode  can see [GB2312 (Simplified Chinese) character code table](http://www.seeedstudio.com/wiki/File:Character_code_table.pdf). 

* uniCode:the machine code according to a character or a chinese. 
* x: the starting X-coordinate.
* y: the starting Y-coordinate.
Note: the character unicode between 0x0020 to 0x007E can directly input via the keyboard. Such as the display of the character 'G' ,the function can be drawUnicode(0x0047, 3,10) or displayChar ('s',3,10);


### 8. int drawUnicodeString(unsigned int *uniCode, int len, int x, int y);
The function can be used to display several characters and chinese.

* *uniCode: a unicode array. 
* len: string length. 
* x: the starting X-coordinate of string.
* y: the starting Y-coordinate of string.


### 9. void drawLine(int x0, int y0, int x1, int y1);
The function can be used to display a line.

* x0: the starting X-coordinate of line
* y0: the starting Y-coordinate of line
* x1: the end X-coordinate of line
* y1: the end Y-coordinate of line


### 10. void drawCircle(int poX, int poY, int r);
The function can be used to draw a circle.

* poX: the X-coordinate of the center
* poY: the Y-coordinate of the center 
* r: the radius of center
 

### 11. void drawHorizontalLine( int poX, int poY, int len);
The function can be used to draw a horizontal line.

* poX: the starting X-coordinate of the line.
* poY: the starting Y-coordinate of the line.
* len: the length of the line


### 12. void drawVerticalLine( int poX, int poY, int len);
The function can be used to draw a vertical line.

* poX: the starting X-coordinate of the Line.
* poY: the starting Y-coordinate of the Line.
* len: the length of the line.


### 13. void drawRectangle(int poX, int poY, int len, int width);
The function can be used to draw a rectangle.

* poX: the starting X-coordinate of the rectangle.
* poY: the starting Y-coordinate of the rectangle.
* len: the length of the rectangle.
* width: the width of the rectangle.


### 14. void fillRectangle(int poX, int poY, int len, int width);
The function can be used to draw a fill rectangle.

* poX: the starting X-coordinate of the rectangle.
* poY: the starting Y-coordinate of the rectangle.
* len: the length of the rectangle.
* width: the width of the rectangle.


### 15. void fillCircle(int poX, int poY, int r);
The function can be used to draw a fill circle.

* poX: the X-coordinate of the center.
* poY: the Y-coordinate of the center. 
* r: the radius of center

Example:<pre> 
    EPAPER.drawRectangle(10, 10, 100, 80);
    EPAPER.fillCircle(50, 50, 30);
    EPAPER.fillRectangle(50, 65, 50, 20);
    EPAPER.drawCircle(150, 50, 10);
    EPAPER.fillCircle(150, 50, 5);
    EPAPER.drawHorizontalLine(120, 50, 60);
    EPAPER.drawVerticalLine(150, 20, 60);</pre>


### 16. void drawTraingle( int poX1, int poY1, int poX2, int poY2, int poX3, int poY3);
The function can be used to draw a triangle. It is formed by three points.

* poX1(poX2,poX3): the X-coordinate of the triangle one point.
* poY1(poY2,poY3): the Y-coordinate of the triangle one point.

### 17. void clearDisplay();
This function clears the display. An alias for ```clear_sd()``` which appears to actually clear the display unlike the existing ```clear()``` function. Call this function in between calls to ```display()``` to show different images or "pages" on the display. 


For more information, please refer to [wiki page](http://www.seeedstudio.com/wiki/Small_e-Paper_Shield).

    
----

This software is written by loovee [luweicong@seeedstudio.com](luweicong@seeedstudio.com "luweicong@seeedstudio.com") for seeed studio
and is licensed under [The MIT License](http://opensource.org/licenses/mit-license.php). Check License.txt for more information.

Contributing to this software is warmly welcomed. You can do this basically by
[forking](https://help.github.com/articles/fork-a-repo), committing modifications and then [pulling requests](https://help.github.com/articles/using-pull-requests) (follow the links above
for operating guide). Adding change log and your contact into file header is encouraged.
Thanks for your contribution.

Seeed Studio is an open hardware facilitation company based in Shenzhen, China. 
Benefiting from local manufacture power and convenient global logistic system, 
we integrate resources to serve new era of innovation. Seeed also works with 
global distributors and partners to push open hardware movement.




[![Analytics](https://ga-beacon.appspot.com/UA-46589105-3/Small_ePaper_Shield)](https://github.com/igrigorik/ga-beacon)
