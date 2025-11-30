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

#define tytul "GK2025 - Lab 3 - Sarna Piotr"


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

int tablicaBayera2[2][2] = {{2, 4}, {3, 1}};
float zaktualizowanaTablicaBayera2[2][2];
int tablicaBayera4[4][4] = {{6, 14, 8, 16}, {10, 2, 12, 4}, {7, 15, 5, 13}, {11, 3, 9, 1}};
float zaktualizowanaTablicaBayera4[4][4];

void zaktualizujTabliceBayera2(){
    int zakres = 256;
    int rozmiar = 2;
    float podzial = zakres*1.0 / (rozmiar*rozmiar);

    for(int y = 0; y < rozmiar; y++){
        for(int x = 0; x < rozmiar; x++){
            zaktualizowanaTablicaBayera2[y][x] = (tablicaBayera2[y][x] * podzial) - podzial/2;
        }
    }

    for(int y = 0; y < rozmiar; y++){
        for(int x = 0; x < rozmiar; x++){
            cout << tablicaBayera2[y][x] << " ";
        }
        cout << endl;
    }
    cout << endl;

    for(int y = 0; y < rozmiar; y++){
        for(int x = 0; x < rozmiar; x++){
            cout << zaktualizowanaTablicaBayera2[y][x] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void zaktualizujTabliceBayera4(){
    int zakres = 256;
    int rozmiar = 4;
    float podzial = zakres*1.0 / (rozmiar*rozmiar);

    for(int y = 0; y < rozmiar; y++){
        for(int x = 0; x < rozmiar; x++){
            zaktualizowanaTablicaBayera4[y][x] = (tablicaBayera4[y][x] * podzial) - podzial/2;
        }
    }

    for(int y = 0; y < rozmiar; y++){
        for(int x = 0; x < rozmiar; x++){
            cout << tablicaBayera4[y][x] << " ";
        }
        cout << endl;
    }
    cout << endl;

    for(int y = 0; y < rozmiar; y++){
        for(int x = 0; x < rozmiar; x++){
            cout << zaktualizowanaTablicaBayera4[y][x] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// Progowanie
void Funkcja1() {
    SDL_Color kolor;
    Uint8 szary;
    Uint8 piksel;

    for(int y = 0; y < wysokosc/2; y++){
        for(int x = 0; x < szerokosc/2; x++){
            kolor = getPixel(x, y);
            szary = 0.299 * kolor.r + 0.587 * kolor.g + 0.114 * kolor.b;
            setPixel(x, y, szary, szary, szary);

            if(szary > 127){
                piksel = 255;
            }
            else{
                piksel = 0;
            }
            setPixel(x + szerokosc/2, y, piksel, piksel, piksel);
        }
    }

    SDL_UpdateWindowSurface(window);
}

// Uporzadkowane rozpraszanie bledow 2x2
void Funkcja2() {
    int rozmiar = 2;
    zaktualizujTabliceBayera2();
    SDL_Color kolor;
    Uint8 szary;
    Uint8 piksel;
    Uint8 tablica;

    for(int y = 0; y < wysokosc/2; y++){
        for(int x = 0; x < szerokosc/2; x++){
            kolor = getPixel(x, y);
            szary = 0.299 * kolor.r + 0.587 * kolor.g + 0.114 * kolor.b;
            setPixel(x, y, szary, szary, szary);

            tablica = zaktualizowanaTablicaBayera2[y % rozmiar][x % rozmiar];
            if(szary > tablica){
                piksel = 255;
            }
            else{
                piksel = 0;
            }
            setPixel(x + szerokosc/2, y, piksel, piksel, piksel);
        }
    }

    SDL_UpdateWindowSurface(window);
}

// Uporzadkowane rozpraszanie bledow 4x4
void Funkcja3() {
    int rozmiar = 4;
    zaktualizujTabliceBayera4();
    SDL_Color kolor;
    Uint8 szary;
    Uint8 piksel;
    Uint8 tablica;

    for(int y = 0; y < wysokosc/2; y++){
        for(int x = 0; x < szerokosc/2; x++){
            kolor = getPixel(x, y);
            szary = 0.299 * kolor.r + 0.587 * kolor.g + 0.114 * kolor.b;
            setPixel(x, y, szary, szary, szary);

            tablica = zaktualizowanaTablicaBayera4[y % rozmiar][x % rozmiar];
            if(szary > tablica){
                piksel = 255;
            }
            else{
                piksel = 0;
            }
            setPixel(x + szerokosc/2, y, piksel, piksel, piksel);
        }
    }

    SDL_UpdateWindowSurface(window);
}

// Algorytm Floyd'a-Steinberga
void Funkcja4() {
    SDL_Color kolor;
    int szary;
    Uint8 szaryOrg;
    Uint8 piksel;
    Uint8 przesuniecie = 1;
    float bledy[(szerokosc/2)+2][(wysokosc/2)+2];
    memset(bledy, 0, sizeof(bledy));
    int blad = 0;

    for(int y = 0; y < wysokosc/2; y++){
        for(int x = 0; x < szerokosc/2; x++){
            kolor = getPixel(x, y);
            szaryOrg = 0.299 * kolor.r + 0.587 * kolor.g + 0.114 * kolor.b;
            szary = szaryOrg + bledy[x + przesuniecie][y];
            setPixel(x, y, szaryOrg, szaryOrg, szaryOrg);

            if(szary > 127){
                piksel = 255;
                blad = szary - 255;
            }
            else{
                piksel = 0;
                blad = szary;
            }
            setPixel(x + szerokosc/2, y, piksel, piksel, piksel);

            bledy[x+1+przesuniecie][y  ] += (blad*7.0/16.0);
            bledy[x-1+przesuniecie][y+1] += (blad*3.0/16.0);
            bledy[x  +przesuniecie][y+1] += (blad*5.0/16.0);
            bledy[x+1+przesuniecie][y+1] += (blad*1.0/16.0);

            if(szary > 255) szary = 255;
            if (szary < 0) szary = 0;
            setPixel(x, y + wysokosc/2, szary, szary, szary);
        }
    }

    SDL_UpdateWindowSurface(window);
}

// Progowanie kolor
void Funkcja5() {
    SDL_Color kolor;
    Uint8 R, G, B;

    for(int y = 0; y < wysokosc/2; y++){
        for(int x = 0; x < szerokosc/2; x++){
            kolor = getPixel(x, y);

            if(kolor.r > 127){
                R = 255;
            }
            else{
                R = 0;
            }
            if(kolor.g > 127){
                G = 255;
            }
            else{
                G = 0;
            }
            if(kolor.b > 127){
                B = 255;
            }
            else{
                B = 0;
            }
            setPixel(x + szerokosc/2, y, R, G, B);
        }
    }

    SDL_UpdateWindowSurface(window);
}

// Uporzadkowane rozpraszanie bledow 4x4 kolor
void Funkcja6() {
    int rozmiar = 4;
    zaktualizujTabliceBayera4();
    SDL_Color kolor;
    Uint8 R, G, B;
    Uint8 tablica;

    for(int y = 0; y < wysokosc/2; y++){
        for(int x = 0; x < szerokosc/2; x++){
            kolor = getPixel(x, y);

            tablica = zaktualizowanaTablicaBayera4[y % rozmiar][x % rozmiar];
            if(kolor.r > tablica){
                R = 255;
            }
            else{
                R = 0;
            }
            if(kolor.g > tablica){
                G = 255;
            }
            else{
                G = 0;
            }
            if(kolor.b > tablica){
                B = 255;
            }
            else{
                B = 0;
            }
            setPixel(x + szerokosc/2, y, R, G, B);
        }
    }

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
