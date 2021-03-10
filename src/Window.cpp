#include "Window.h"
#include "Adafruit_GFX.h"

void Window::clear()
{
    gd.fillRect(windowX, windowY, windowWidth, windowHeight, bgcolor);
    drawBorder();
}

void Window::drawBorder()
{
    switch (border)
    {

    case BORDER_NONE:
        break;
    case BORDER_TRIPLE:
        gd.drawRect(windowX + margin + 2, windowY + margin + 2,
                     windowWidth - margin - 4,
                     windowHeight - margin - 4, borderColor[2]);
        
    case BORDER_DOUBLE:
        gd.drawRect(windowX + margin + 1, windowY + margin + 1,
                     windowWidth - margin - 2,
                     windowHeight - margin - 2, borderColor[1]);

    case BORDER_SIMPLE:
        gd.drawRect(windowX + margin, windowY + margin,
                     windowWidth - margin,
                     windowHeight - margin, borderColor[0]);
        break;
    }
}

void Window::draw()
{
    clear();
}