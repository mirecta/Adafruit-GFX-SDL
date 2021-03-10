
//#include <stdlib.h>
#include <inttypes.h>
//https://ee-programming-notepad.blogspot.com/2016/10/16-bit-color-generator-picker.html
#ifndef _WINDOW_H_
#define _WINDOW_H_
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define GREY 0x8410
#define LIGHT_GREY 0xCE38

#define MAX_TEXTWINDOW_LINES 30

class Adafruit_GFX;
class GFXFont;

class Window
{
public:
    enum
    {
        BORDER_NONE = 0,
        BORDER_SIMPLE,
        BORDER_DOUBLE,
        BORDER_TRIPLE
    };

public:
    Window(Adafruit_GFX &gd, int16_t x, int16_t y, int16_t w,
           int16_t h, uint8_t margin = 0, uint8_t padding = 0) : gd(gd),
                                                                 windowX(x), windowY(y),
                                                                 windowWidth(w), windowHeight(h),
                                                                 margin(margin), padding(padding),
                                                                 border(BORDER_NONE), bgcolor(BLACK)
    {
    }
    virtual ~Window() {}

    void setBorderType(uint8_t type, uint16_t c1 = GREY, uint16_t c2 = LIGHT_GREY, uint16_t c3 = GREY)
    {
        border = type;
        borderColor[0] = c1;
        borderColor[1] = c2;
        borderColor[3] = c3;
    }

    void clear();
    virtual void drawBorder();
    virtual void draw();

    int16_t intX(int16_t x) { return (x + windowX + margin + border + padding); }
    int16_t intY(int16_t y) { return (y + windowY + margin + border + padding); }
    int16_t maxX() { return (windowWidth - (margin << 1) - (padding << 1) - (border << 1)); }
    int16_t maxY() { return (windowHeight - (margin << 1) - (padding << 1) - (border << 1)); }

private:
    Adafruit_GFX &gd;
    int16_t windowX;
    int16_t windowY;
    int16_t windowWidth;
    int16_t windowHeight;
    uint8_t border;
    uint8_t margin;
    uint8_t padding;
    uint16_t bgcolor;
    uint16_t borderColor[3];
};

class TextWindow : public Window
{
public:
    TextWindow(Adafruit_GFX &gd, int16_t x, int16_t y, int16_t w,
               int16_t h, uint8_t margin = 0,
               uint8_t padding = 0) : Window(gd, x, y, w, h, margin, padding),
                                      font(0), textSize(1), textColor(WHITE),
                                      startLine(0) {}

    virtual ~TextWindow(){}

    void setText(const char *text = 0);
    void computeLines();
    void setFont(GFXFont *font);
    virtual void draw();
private:
    GFXFont *font;
    uint8_t textSize;
    uint16_t textColor;
    uint8_t startLine;
    uint8_t maxLines;
    uint8_t linesCount;
   

    const char *lines[MAX_TEXTWINDOW_LINES];
    const char *text;

};

#endif //_WINDOW_H_