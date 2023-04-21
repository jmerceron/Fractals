#pragma once

//#define WIDTH       800
//#define HEIGHT      600
#define START_POS   -0.5
//#define START_ZOOM  (WIDTH * 0.25296875f)
#define START_ZOOM  (800 * 0.25296875f)

#define BAIL_OUT        2.0
#define FLIPS           24

#define ZOOM_FACTOR     4

# define M_LN2          0.69314718055994530942  /* log_e 2 */

void sdl_draw_mandelbrot(SDL_Window* window, const int width, const int height, _Dcomplex center, double zoom)
{
    int f, x, y, n;
    int maxiter = (width / 2) * 0.049715909 * log10(zoom);
    _Dcomplex z, c;
    float C;
//    static SDL_Rect rects[height / FLIPS];

    fprintf(stderr, "zoom: %f\n", zoom);
    fprintf(stderr, "center point: %f %+fi\n", creal(center),cimag(center));
    fprintf(stderr, "iterations: %d\n", maxiter);

    // Create the surface
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    
    SDL_LockSurface(surface);

    for (f = 0; f < FLIPS; f++)
    {
        for (y = f; y < height; y += FLIPS)
        {
            for (x = 0; x < width; x++)
            {
                /* Get the complex point on gauss space to be calculated */
                z = c = { (creal(center) + (x - (width/ 2)) / zoom),
                    (cimag(center) + (y - (height / 2)) / zoom) };

#define X creal(z)
#define Y cimag(z)

                /* Check if point lies within the main cardiod or
                   in the period-2 buld */
                if ((pow(X - .25, 2) + pow(Y, 2)) * (pow(X, 2) + (X / 2) + pow(Y, 2) - .1875) < pow(Y, 2) / 4 ||
                    pow(X + 1, 2) + pow(Y, 2) < .0625)
                    n = maxiter;
                else
                    /* Applies the actual mandelbrot formula on that point */
                    for (n = 0; n <= maxiter && cabs(z) < BAIL_OUT; n++)
                        z = { creal(cpow(z, { 2,0 })) + creal(c),
                                cimag(cpow(z, { 2,0 })) + cimag(c) };

                C = n - log2f(logf(cabs(z)) / M_LN2);

                /* Paint the pixel calculated depending on the number
                   of iterations found */
                ((Uint32*)surface->pixels)[(y * surface->w) + x] = (n >= maxiter) ? 0 :
                    SDL_MapRGB(surface->format,
                        (1 + sin(C * 0.27 + 5)) * 127., (1 + cos(C * 0.85)) * 127., (1 + sin(C * 0.15)) * 127.);
            }
 //           rects[y / FLIPS].x = 0;
 //           rects[y / FLIPS].y = y;
 //           rects[y / FLIPS].w = width;
 //           rects[y / FLIPS].h = 1;
        }
        SDL_UnlockSurface(surface);
        SDL_UpdateWindowSurface(window);
    }
}

