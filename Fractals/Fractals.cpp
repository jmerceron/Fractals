
// SpaceInvaders_Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Julien Merceron - 2023
// code generated by chatGPT
/*
ChatGPT Dec 15th 2022 Version
https://chat.openai.com/chat
*/
/*
SDL
https://github.com/libsdl-org/SDL/releases/tag/release-2.26.2
SDL2-devel-2.26.2-VC.zip
sdl/include
sdl/lib
wrt dlls, ...
... copy \SDL2\lib\x64\SDL2.dll to C:\Windows\System32
... and \SDL2\lib\x86\SDL2.dll to C:\Windows\SysWOW64. Voila
*/
/*
SDL Image
https://github.com/libsdl-org/SDL_image
TBC
*/
/*
create a CONSOLE APP on VC++
copy paste base code in c++ file
------
add include path to SDL in VC++ Directories
add lib path to SDL in VC++ Directories
add sdl2.lib in libs to link in Linker -> Input
change from console to windows in Linker -> System
*/
/*
PROMPT MODIFIERS
resolution_driven
add_comments
*/


/*
https://www.geeksforgeeks.org/fractals-in-cc/

https://rosettacode.org/wiki/Julia_set
https://www.geeksforgeeks.org/julia-fractal-set-in-c-c-using-graphics/
*/


#include <iostream>
#include "sdl.h"
#include "SDL_render.h"
//#include "sdl_image.h"
#include <windows.h>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


// Screen dimensions (as percentages of the actual screen dimensions)
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
const int SCREEN_WIDTH_PERCENT = 80;
const int SCREEN_HEIGHT_PERCENT = 80;
int screen_width, screen_height;
int game_screen_width;
int game_screen_height;


#include <complex.h>



#include "mandelbrot.h"
#define MANDELBROT 1
#define JULIA 0



typedef struct {
    double x, y;
}complex;

complex add(complex a, complex b) {
    complex c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    return c;
}

complex sqr(complex a) {
    complex c;
    c.x = a.x * a.x - a.y * a.y;
    c.y = 2 * a.x * a.y;
    return c;
}

double mod(complex a) {
    return sqrt(a.x * a.x + a.y * a.y);
}

complex mapPoint(int width, int height, double radius, int x, int y) {
    complex c;
    int l = (width < height) ? width : height;

    c.x = 2 * radius * (x - width / 2.0) / l;
    c.y = 2 * radius * (y - height / 2.0) / l;

    return c;
}

void juliaSet(SDL_Window* window, int width, int height, complex c, double radius, int n) {
    
    // https://rosettacode.org/wiki/Julia_set
    
    int x, y, i;
    complex z0, z1;

    // Create the surface
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    
    SDL_LockSurface(surface);

    for (x = 0; x < width; x++)
    {
        for (y = 0; y < height; y++) 
        {
            z0 = mapPoint(width, height, radius, x, y);
            for (i = 1; i <= n; i++) {
                z1 = add(sqr(z0), c);
                if (mod(z1) > radius) {

                    /* Paint the pixel calculated depending on the number of iterations found */
//                    putpixel(x, y, i % 15 + 1);
                    ((Uint32*)surface->pixels)[(y * surface->w) + x] = i % 15 + 1;

                    break;
                }
                z0 = z1;
            }
            if (i > n)
            {
                /* Paint the pixel calculated depending on the number of iterations found */
//                putpixel(x, y, 0);
                ((Uint32*)surface->pixels)[(y * surface->w) + x] = 0;
            }
        }
    }
    SDL_UnlockSurface(surface);
    SDL_UpdateWindowSurface(window);

}




int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{

#if 0
    std::cout << "Hello World!\n";
    return 0;
#else
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Flag to track whether the window is in full screen mode
    bool fullScreen = false;

    // Get the actual screen dimensions
    SDL_DisplayMode displayMode;

    // SDL_GetCurrentDisplayMode(0, &screen_width, &screen_height);
    if (SDL_GetCurrentDisplayMode(0, &displayMode) == 0)
    {
        // Success! The current display mode is stored in the "displayMode" variable
        screen_width = displayMode.w;
        screen_height = displayMode.h;
        std::cout << "Success! The current display mode is stored in the displayMode variable, and we have " << screen_width << ", " << screen_height << std::endl;
    }
    else
    {
        // An error occurred. You can use SDL_GetError() to get the error message.
        std::cout << "An error occurred. You can use SDL_GetError() to get the error message" << std::endl;
        return -1;
    }

    // Calculate the dimensions of the game screen (as pixels)
//    game_screen_width = screen_width * SCREEN_WIDTH_PERCENT / 100;
//    game_screen_height = screen_height * SCREEN_HEIGHT_PERCENT / 100;
    // Override calculations
    game_screen_width = SCREEN_WIDTH;
    game_screen_height = SCREEN_HEIGHT;

    // Create the window
    SDL_Window* window = SDL_CreateWindow("Fractals", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, game_screen_width, game_screen_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
    {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create the renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }


    // one time init for fractals 
#if MANDELBROT
    _Dcomplex center = { START_POS, 0.0 };
    double zoom = START_ZOOM;
    sdl_draw_mandelbrot(window, game_screen_width, game_screen_height, center, zoom);
#endif
#if JULIA
    complex reference_c = {0.5, 0.5};
    juliaSet(window, game_screen_width, game_screen_height, reference_c, 1.0, 10);
#endif

    // Set the frame rate
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    // Game loop
    bool running = true;
    bool bReady_1 = 1;
    bool bReady_3 = 1;
    while (running)
    {
        // Handle input


        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if ((event.type == SDL_QUIT) || (event.key.keysym.sym == SDLK_ESCAPE))
            {
                running = false;
            }
            if (event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    // Handle window resized event
                    game_screen_width = event.window.data1;
                    game_screen_height = event.window.data2;
                    // Do something with newWidth and newHeight
#if MANDELBROT
                    center = { START_POS, 0.0 };
                    zoom = START_ZOOM;
                    sdl_draw_mandelbrot(window, game_screen_width, game_screen_height, center, zoom);
#endif
#if JULIA
                    reference_c = { 0.5, 0.5 };
                    juliaSet(window, game_screen_width, game_screen_height, reference_c, 1.0, 10);
#endif
                }
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_f)
            {
                // Toggle full screen mode
                fullScreen = !fullScreen;
                SDL_SetWindowFullscreen(window, fullScreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
#if MANDELBROT
                center = { START_POS, 0.0 };
                zoom = START_ZOOM;
                sdl_draw_mandelbrot(window, game_screen_width, game_screen_height, center, zoom);
#endif
#if JULIA
                reference_c = { 0.5, 0.5 };
                juliaSet(window, game_screen_width, game_screen_height, reference_c, 1.0, 10);
#endif
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_LEFT)
                {
                    // Do something 
                }
                else if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    // Do something 
                }
                else if (event.key.keysym.sym == SDLK_SPACE) 
                {
                    // Do something 
#if MANDELBROT
                    center = { START_POS, 0.0 };
                    zoom = START_ZOOM;
                    sdl_draw_mandelbrot(window, game_screen_width, game_screen_height, center, zoom);
#endif
#if JULIA
                    reference_c = { 0.5, 0.5 };
                    juliaSet(window, game_screen_width, game_screen_height, reference_c, 1.0, 10);
#endif
                }
            }
            else if (event.type == SDL_KEYUP)
            {
                if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT)
                {
                    // Do something 
                }
                else if (event.key.keysym.sym == SDLK_SPACE) 
                {
                    // Do something 
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                // Handle mouse input
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY); // Get the mouse position
                // Do something 
                // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#if JULIA
                // To select the value of 'c'
                // using the position of the mouse and then
                // normalizing it between a value of -1-1i and 1+1i.
                reference_c = { 2 * ((double)(mouseX)) / game_screen_width - 1.0,
                                2 * ((double)(mouseY)) / game_screen_height - 1.0 };
#endif

                if ((event.button.button == 1) && (bReady_1))
                {
                    bReady_1 = 0;

#if MANDELBROT
                    zoom *= ZOOM_FACTOR;
                    center = { creal(center) + (mouseX - (game_screen_width / 2)) / zoom,
                                cimag(center) + (mouseY - (game_screen_height / 2)) / zoom };

                    sdl_draw_mandelbrot(window, game_screen_width, game_screen_height, center, zoom);
#endif
#if JULIA
                    juliaSet(window, game_screen_width, game_screen_height, reference_c, 1.0, 10);
#endif
                }
                if ((event.button.button == 3) && (bReady_3))
                {
                    bReady_3 = 0;

#if MANDELBROT
                    zoom /= ZOOM_FACTOR;
                    center = { creal(center) + (mouseX - (game_screen_width / 2)) / zoom,
                                cimag(center) + (mouseY - (game_screen_height / 2)) / zoom };

                    sdl_draw_mandelbrot(window, game_screen_width, game_screen_height, center, zoom);
#endif
#if JULIA
                    juliaSet(window, game_screen_width, game_screen_height, reference_c, 1.0, 10);
#endif
                }
            }
            else if (event.type == SDL_MOUSEBUTTONUP)
            {
                if (event.button.button == 1)
                    bReady_1 = 1;
                else if (event.button.button == 3)
                    bReady_3 = 1;
            }
        }

        // Per Frame Update state
        // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
        // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
        // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
        // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX


        // Delay to achieve desired frame rate
        SDL_Delay(frameDelay);
    }

    // Clean up SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
#endif
}




// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file


