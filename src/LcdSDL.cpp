#include "LcdSDL.h"


#ifndef pgm_read_byte
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif
#ifndef pgm_read_word
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif
#ifndef pgm_read_dword
#define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#endif

// Pointers are a peculiar case...typically 16-bit on AVR boards,
// 32 bits elsewhere.  Try to accommodate both...

#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
#define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))
#else
#define pgm_read_pointer(addr) ((void *)pgm_read_word(addr))
#endif

inline GFXglyph *pgm_read_glyph_ptr(const GFXfont *gfxFont, uint8_t c) {
#ifdef __AVR__
  return &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c]);
#else
  // expression in __AVR__ section may generate "dereferencing type-punned
  // pointer will break strict-aliasing rules" warning In fact, on other
  // platforms (such as STM32) there is no need to do this pointer magic as
  // program memory may be read in a usual way So expression may be simplified
  return gfxFont->glyph + c;
#endif //__AVR__
}

inline uint8_t *pgm_read_bitmap_ptr(const GFXfont *gfxFont) {
#ifdef __AVR__
  return (uint8_t *)pgm_read_pointer(&gfxFont->bitmap);
#else
  // expression in __AVR__ section generates "dereferencing type-punned pointer
  // will break strict-aliasing rules" warning In fact, on other platforms (such
  // as STM32) there is no need to do this pointer magic as program memory may
  // be read in a usual way So expression may be simplified
  return gfxFont->bitmap;
#endif //__AVR__
}

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif


LcdSDL::LcdSDL(int16_t width, int16_t height) : Adafruit_GFX(width, height)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
        throw(-1);
    }
    window = SDL_CreateWindow(
        "SDL Display",
        500, 500,
        width, height,
        0);
    if (window == NULL)
    {
        fprintf(stderr, "could not create window: %s\n", SDL_GetError());
        throw(-1);
    }
    //screenSurface = SDL_GetWindowSurface(window);
    renderer = SDL_CreateRenderer(window, -1, 0);
}

void LcdSDL::setColor(uint16_t color)
{
    SDL_SetRenderDrawColor(renderer, (color >> 8) & 0xf8, (color >> 3) & 0xfc, (color & 0x1f) << 3, 255);
}

void LcdSDL::fillScreen(uint16_t color)
{
    setColor(color);
    SDL_RenderClear(renderer);
}

void LcdSDL::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    setColor(color);
    SDL_RenderDrawPoint(renderer, x, y);
}

void LcdSDL::update()
{
    //SDL_UpdateWindowSurface(window);
    SDL_RenderPresent(renderer);
}

const char *LcdSDL::drawTextToBox(int16_t x, int16_t y, int16_t bw, int16_t bh, const char *str)
{
    int16_t tcursor_x = x;
    int16_t tcursor_y = y;

    while (*str != '\0')
    {
        //something
        if (!gfxFont)
        { // 'Classic' built-in font

            if (*str == '\n')
            {                               // Newline?
                tcursor_x = x;               // Reset x to zero,
                tcursor_y += textsize_y * 8; // advance y one line
            }
            else if (*str != '\r')
            { // Ignore carriage returns
                if (wrap && ((tcursor_x + textsize_x * 6) > x + bw))
                {                               // Off right?
                    tcursor_x = x;               // Reset x to zero,
                    tcursor_y += textsize_y * 8; // advance y one line
                }
                if (tcursor_y + textsize_y * 8 > y + bh){
                    return str;
                }
                drawChar(tcursor_x, tcursor_y, *str, textcolor, textbgcolor, textsize_x,
                         textsize_y);
                tcursor_x += textsize_x * 6; // Advance x one char
            }
        }
        else
        { // Custom font

            if (*str == '\n')
            {
                tcursor_x = x;
                tcursor_y +=
                    (int16_t)textsize_y * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
            }
            else if (*str != '\r')
            {
                uint8_t first = pgm_read_byte(&gfxFont->first);
                if ((*str >= first) && (*str <= (uint8_t)pgm_read_byte(&gfxFont->last)))
                {
                    GFXglyph *glyph = pgm_read_glyph_ptr(gfxFont, *str - first);
                    uint8_t w = pgm_read_byte(&glyph->width),
                            h = pgm_read_byte(&glyph->height);
                    if ((w > 0) && (h > 0))
                    {                                                        // Is there an associated bitmap?
                        int16_t xo = (int8_t)pgm_read_byte(&glyph->xOffset); // sic
                        if (wrap && ((tcursor_x + textsize_x * (xo + w)) > x + bw))
                        {
                            tcursor_x = x;
                            tcursor_y += (int16_t)textsize_y *
                                        (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
                        }
                        if(tcursor_y + (int16_t)textsize_y *
                                        (uint8_t)pgm_read_byte(&gfxFont->yAdvance) > y + bh){
                                            return str;
                                        }
                        drawChar(tcursor_x, tcursor_y, *str, textcolor, textbgcolor, textsize_x,
                                 textsize_y);
                    }
                    tcursor_x +=
                        (uint8_t)pgm_read_byte(&glyph->xAdvance) * (int16_t)textsize_x;
                }
            }
        }

        ++str;
    }
    return str;
}

LcdSDL::~LcdSDL()
{
    SDL_DestroyWindow(window);
}
