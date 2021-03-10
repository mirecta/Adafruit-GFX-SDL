#include "Window.h"
#include "LcdSDL.h"



LcdSDL display = LcdSDL(250, 122);
Window w1(display,0,0,80,100,1,1);


void loop();
void setup();

int main(int argc, char *args[])
{

    setup();
    SDL_Delay(100);
    display.update();
    bool keep_window_open = true;
    while (keep_window_open)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e) > 0)
        {
            switch (e.type)
            {
            case SDL_QUIT:
                keep_window_open = false;
                break;
            }
            
            //display.update();
        }
        loop();
    }
    return 0;
}

const char str[] = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s";
const char* p = str;
void setup(){
   w1.setBorderType(Window::BORDER_TRIPLE);
   w1.draw();
    
}


void loop()
{
   /* display.fillScreen(0);
    if (*p == '\0'){
        p = str;
    }

    p = display.drawTextToBox(10,10,80,80,p);
    display.update();*/

    SDL_Delay(1000);

    display.update();
    
}