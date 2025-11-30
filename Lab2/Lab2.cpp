#include <exception>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

using namespace std;

SDL_Window* window = NULL;
SDL_Surface* screen = NULL;

#define szerokosc 640
#define wysokosc 400

#define tytul "GK2025 - Lab 2 - Sarna Piotr"


void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);
SDL_Color getPixel (int x, int y);

void czyscEkran(Uint8 R, Uint8 G, Uint8 B);

void Funkcja1();
void Funkcja2();
void Funkcja3();
void Funkcja4();
void Funkcja5();
void Funkcja6();
void Funkcja7();
void Funkcja8();
void Funkcja9();

struct CMYK{
    Uint8 c, m, y, k;
};

void setCMYK(int xx, int yy, Uint8 c, Uint8 m, Uint8 y, Uint8 k){
    int C, M, Y;
    int R, G, B;

    C = c + k;
    M = m + k;
    Y = y + k;

    R = 255 - C;
    G = 255 - M;
    B = 255 - Y;

    setPixel(xx, yy, R, G, B);
}

CMYK getCMYK(int xx, int yy){
    int R, G, B;
    int C, M, Y;
    CMYK cmyk;

    R = getPixel(xx, yy).r;
    G = getPixel(xx, yy).g;
    B = getPixel(xx, yy).b;

    C = 255 - R;
    M = 255 - G;
    Y = 255 - B;

    cmyk.k = min(min(C, M), Y);
    cmyk.c = C - cmyk.k;
    cmyk.m = M - cmyk.k;
    cmyk.y = Y - cmyk.k;

    return cmyk;
}

// RGB -> CMY -> cmyk -> CMY -> RGB
void Funkcja1() {
    int R, G, B;
    int C, M, Y;
    int c, m, y, k;

    for(int yy = 0; yy < wysokosc/2; yy++){
        for(int xx = 0; xx < szerokosc/2; xx++){
            // Wyzerowanie skladowej y
            R = getPixel(xx, yy).r;
            G = getPixel(xx, yy).g;
            B = getPixel(xx, yy).b;

            C = 255 - R;
            M = 255 - G;
            Y = 255 - B;

            k = min(min(C, M), Y);
            c = C - k;
            m = M - k;
            y = 0;

            C = c + k;
            M = m + k;
            Y = y + k;

            R = 255 - C;
            G = 255 - M;
            B = 255 - Y;

            setPixel(xx + szerokosc/2, yy, R, G, B);

            // Wyzerowanie skladowej c
            R = getPixel(xx, yy).r;
            G = getPixel(xx, yy).g;
            B = getPixel(xx, yy).b;

            C = 255 - R;
            M = 255 - G;
            Y = 255 - B;

            k = min(min(C, M), Y);
            c = 0;
            m = M - k;
            y = Y - k;

            C = c + k;
            M = m + k;
            Y = y + k;

            R = 255 - C;
            G = 255 - M;
            B = 255 - Y;

            setPixel(xx, yy + wysokosc/2, R, G, B);

            // Wyzerowanie skladowej m
            R = getPixel(xx, yy).r;
            G = getPixel(xx, yy).g;
            B = getPixel(xx, yy).b;

            C = 255 - R;
            M = 255 - G;
            Y = 255 - B;

            k = min(min(C, M), Y);
            c = C - k;
            m = 0;
            y = Y - k;

            C = c + k;
            M = m + k;
            Y = y + k;

            R = 255 - C;
            G = 255 - M;
            B = 255 - Y;

            setPixel(xx + szerokosc/2, yy + wysokosc/2, R, G, B);
        }
    }

    SDL_UpdateWindowSurface(window);
}

// Wyzerowanie skladowej k
void Funkcja2() {
    int R, G, B;
    int C, M, Y;
    int c, m, y, k;
    int czarny;

    for(int yy = 0; yy < wysokosc/2; yy++){
        for(int xx = 0; xx < szerokosc/2; xx++){
            R = getPixel(xx, yy).r;
            G = getPixel(xx, yy).g;
            B = getPixel(xx, yy).b;

            C = 255 - R;
            M = 255 - G;
            Y = 255 - B;

            k = min(min(C, M), Y);

            czarny = 255 - k;
            setPixel(xx + szerokosc/2, yy + wysokosc/2, czarny, czarny, czarny);

            c = C - k;
            m = M - k;
            y = Y - k;
            k = 0;

            C = c + k;
            M = m + k;
            Y = y + k;

            R = 255 - C;
            G = 255 - M;
            B = 255 - Y;

            setPixel(xx + szerokosc/2, yy, R, G, B);
        }
    }

    SDL_UpdateWindowSurface(window);
}

// Wykorzystanie funkcji pomocniczych dla modelu CMYK
void Funkcja3() {
    for(int yy = 0; yy < wysokosc/2; yy++){
        for(int xx = 0; xx < szerokosc/2; xx++){
            CMYK cmyk = getCMYK(xx, yy);
            setCMYK(xx + szerokosc/2, yy, cmyk.c, cmyk.m, cmyk.y, cmyk.k);
        }
    }

    SDL_UpdateWindowSurface(window);
}

// Wizualizacja modelu YCbCr
void Funkcja4() {
    Uint8 R, G, B;
    Uint8 Y, Cb, Cr;

    for(int yy = 0; yy < wysokosc/2; yy++){
        for(int xx = 0; xx < szerokosc/2; xx++){
            R = getPixel(xx, yy).r;
            G = getPixel(xx, yy).g;
            B = getPixel(xx, yy).b;

            Y = 0 + 0.299 * R + 0.587 * G + 0.114 * B;
            Cb = 128 - 0.168736 * R - 0.331264 * G + 0.5 * B;
            Cr = 128 + 0.5 * R - 0.418688 * G - 0.081312 * B;

            setPixel(xx + szerokosc/2, yy, Y, Y, Y);
            setPixel(xx, yy + wysokosc/2, Cb, Cb, Cb);
            setPixel(xx + szerokosc/2, yy + wysokosc/2, Cr, Cr, Cr);
        }
    }

    SDL_UpdateWindowSurface(window);
}

void Funkcja5() {

    //...

    SDL_UpdateWindowSurface(window);
}

void Funkcja6() {

    //...

    SDL_UpdateWindowSurface(window);
}

void Funkcja7() {

    //...

    SDL_UpdateWindowSurface(window);
}

void Funkcja8() {

    //...

    SDL_UpdateWindowSurface(window);
}

void Funkcja9() {

    //...

    SDL_UpdateWindowSurface(window);
}

void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
  if ((x>=0) && (x<szerokosc) && (y>=0) && (y<wysokosc))
  {
    /* Zamieniamy poszczególne składowe koloru na format koloru piksela */
    Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);

    /* Pobieramy informację ile bajtów zajmuje jeden piksel */
    int bpp = screen->format->BytesPerPixel;

    /* Obliczamy adres piksela */
    Uint8 *p1 = (Uint8 *)screen->pixels + (y*2) * screen->pitch + (x*2) * bpp;
    Uint8 *p2 = (Uint8 *)screen->pixels + (y*2+1) * screen->pitch + (x*2) * bpp;
    Uint8 *p3 = (Uint8 *)screen->pixels + (y*2) * screen->pitch + (x*2+1) * bpp;
    Uint8 *p4 = (Uint8 *)screen->pixels + (y*2+1) * screen->pitch + (x*2+1) * bpp;

    /* Ustawiamy wartość piksela, w zależnoœci od formatu powierzchni*/
    switch(bpp)
    {
        case 1: //8-bit
            *p1 = pixel;
            *p2 = pixel;
            *p3 = pixel;
            *p4 = pixel;
            break;

        case 2: //16-bit
            *(Uint16 *)p1 = pixel;
            *(Uint16 *)p2 = pixel;
            *(Uint16 *)p3 = pixel;
            *(Uint16 *)p4 = pixel;
            break;

        case 3: //24-bit
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p1[0] = (pixel >> 16) & 0xff;
                p1[1] = (pixel >> 8) & 0xff;
                p1[2] = pixel & 0xff;
                p2[0] = (pixel >> 16) & 0xff;
                p2[1] = (pixel >> 8) & 0xff;
                p2[2] = pixel & 0xff;
                p3[0] = (pixel >> 16) & 0xff;
                p3[1] = (pixel >> 8) & 0xff;
                p3[2] = pixel & 0xff;
                p4[0] = (pixel >> 16) & 0xff;
                p4[1] = (pixel >> 8) & 0xff;
                p4[2] = pixel & 0xff;
            } else {
                p1[0] = pixel & 0xff;
                p1[1] = (pixel >> 8) & 0xff;
                p1[2] = (pixel >> 16) & 0xff;
                p2[0] = pixel & 0xff;
                p2[1] = (pixel >> 8) & 0xff;
                p2[2] = (pixel >> 16) & 0xff;
                p3[0] = pixel & 0xff;
                p3[1] = (pixel >> 8) & 0xff;
                p3[2] = (pixel >> 16) & 0xff;
                p4[0] = pixel & 0xff;
                p4[1] = (pixel >> 8) & 0xff;
                p4[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4: //32-bit
            *(Uint32 *)p1 = pixel;
            *(Uint32 *)p2 = pixel;
            *(Uint32 *)p3 = pixel;
            *(Uint32 *)p4 = pixel;
            break;

        }
    }
}

void setPixelSurface(int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
  if ((x>=0) && (x<szerokosc*2) && (y>=0) && (y<wysokosc*2))
  {
    /* Zamieniamy poszczególne składowe koloru na format koloru piksela */
    Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);

    /* Pobieramy informację ile bajtów zajmuje jeden piksel */
    int bpp = screen->format->BytesPerPixel;

    /* Obliczamy adres piksela */
    Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

    /* Ustawiamy wartość piksela, w zależności od formatu powierzchni*/
    switch(bpp)
    {
        case 1: //8-bit
            *p = pixel;
            break;

        case 2: //16-bit
            *(Uint16 *)p = pixel;
            break;

        case 3: //24-bit
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4: //32-bit
            *(Uint32 *)p = pixel;
            break;
        }
    }
}

SDL_Color getPixel(int x, int y) {
    SDL_Color color ;
    Uint32 col = 0 ;
    if ((x>=0) && (x<szerokosc) && (y>=0) && (y<wysokosc)) {
        //określamy pozycję
        char* pPosition=(char*)screen->pixels ;

        //przesunięcie względem y
        pPosition+=(screen->pitch*y*2) ;

        //przesunięcie względem x
        pPosition+=(screen->format->BytesPerPixel*x*2);

        //kopiujemy dane piksela
        memcpy(&col, pPosition, screen->format->BytesPerPixel);

        //konwertujemy kolor
        SDL_GetRGB(col, screen->format, &color.r, &color.g, &color.b);
    }
    return ( color ) ;
}

SDL_Color getPixelSurface(int x, int y, SDL_Surface *surface) {
    SDL_Color color ;
    Uint32 col = 0 ;
    if ((x>=0) && (x<szerokosc) && (y>=0) && (y<wysokosc)) {
        //określamy pozycję
        char* pPosition=(char*)surface->pixels ;

        //przesunięcie względem y
        pPosition+=(surface->pitch*y) ;

        //przesunięcie względem x
        pPosition+=(surface->format->BytesPerPixel*x);

        //kopiujemy dane piksela
        memcpy(&col, pPosition, surface->format->BytesPerPixel);

        //konwertujemy kolor
        SDL_GetRGB(col, surface->format, &color.r, &color.g, &color.b);
    }
    return ( color ) ;
}


void ladujBMP(char const* nazwa, int x, int y)
{
    SDL_Surface* bmp = SDL_LoadBMP(nazwa);
    if (!bmp)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
    }
    else
    {
        SDL_Color kolor;
        for (int yy=0; yy<bmp->h; yy++) {
			for (int xx=0; xx<bmp->w; xx++) {
				kolor = getPixelSurface(xx, yy, bmp);
				setPixel(xx, yy, kolor.r, kolor.g, kolor.b);
			}
        }
		SDL_FreeSurface(bmp);
        SDL_UpdateWindowSurface(window);
    }

}


void czyscEkran(Uint8 R, Uint8 G, Uint8 B)
{
    SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, R, G, B));
    SDL_UpdateWindowSurface(window);
}



int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
    }

    window = SDL_CreateWindow(tytul, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szerokosc*2, wysokosc*2, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    screen = SDL_GetWindowSurface(window);
    if (screen == NULL) {
        fprintf(stderr, "SDL_GetWindowSurface Error: %s\n", SDL_GetError());
    return false;
    }
    SDL_UpdateWindowSurface(window);


    bool done = false;
    SDL_Event event;
    // główna pętla programu
    while (SDL_WaitEvent(&event)) {
        // sprawdzamy czy pojawiło się zdarzenie
        switch (event.type) {
            case SDL_QUIT:
                done = true;
                break;

            // sprawdzamy czy został wciśnięty klawisz
            case SDL_KEYDOWN: {
                // wychodzimy, gdy wciśnięto ESC
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    done = true;
                if (event.key.keysym.sym == SDLK_1)
                    Funkcja1();
                if (event.key.keysym.sym == SDLK_2)
                    Funkcja2();
                if (event.key.keysym.sym == SDLK_3)
                    Funkcja3();
                if (event.key.keysym.sym == SDLK_4)
                    Funkcja4();
                if (event.key.keysym.sym == SDLK_5)
                    Funkcja5();
                if (event.key.keysym.sym == SDLK_6)
                    Funkcja6();
                if (event.key.keysym.sym == SDLK_7)
                    Funkcja7();
                if (event.key.keysym.sym == SDLK_8)
                    Funkcja8();
                if (event.key.keysym.sym == SDLK_9)
                    Funkcja9();
                if (event.key.keysym.sym == SDLK_a)
                    ladujBMP("obrazek1.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_s)
                    ladujBMP("obrazek2.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_d)
                    ladujBMP("obrazek3.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_f)
                    ladujBMP("obrazek4.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_g)
                    ladujBMP("obrazek5.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_h)
                    ladujBMP("obrazek6.bmp", 0, 0);
                if (event.key.keysym.sym == SDLK_b)
                    czyscEkran(0, 0, 0);
                else
                    break;
               }
        }
        if (done) break;
    }

    if (screen) {
        SDL_FreeSurface(screen);
    }

    if (window) {
        SDL_DestroyWindow(window);
    }


    SDL_Quit();
    return 0;
}
