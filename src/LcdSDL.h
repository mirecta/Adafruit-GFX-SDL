#include <SDL2/SDL.h>
#include "Adafruit_GFX.h"
#include <stdio.h>


class LcdSDL :public Adafruit_GFX {
    public:
        LcdSDL(int16_t width,int16_t height);
        ~LcdSDL();
        void update();
        void setColor(uint16_t color);
        virtual void drawPixel(int16_t x, int16_t y, uint16_t color);
        virtual void fillScreen(uint16_t color);
        const char* drawTextToBox(int16_t x, int16_t y, int16_t bw, int16_t bh, const char *str); 

    private:
        SDL_Window *window;
        //SDL_Surface *screenSurface;
        SDL_Renderer* renderer;

};

