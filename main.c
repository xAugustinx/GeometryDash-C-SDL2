#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define true 1
#define false 0

int wysokoscPoprzedniejPrzeszkody = 37;
int licznikNygerJakDalekoPoszedlTenPion = 0;

int skokUpadek[4] = {0,0,0,0};
int ktoryElementSkoku = 0;
int turnOn = true;
int plansza[40][61];
int timer = 0;
int playerPosition[2] = {36,15};
int kolory[3][3] =
{
    {59, 135, 235},   //ground
    {13, 2, 110},    //background
    {217, 232, 104} //player
};

void cofniecie() {
    for (int x = 1; x < 61; x++) {
        for (int y = 0; y < 40; y++)
        {plansza[y][x-1] = plansza[y][x];}
    }
}
void generowanieNowegoFragmentu() {
    int x = 60;
    for (int y = 0; y < 40; y++) {plansza[y][x] = 0;}
    if (! (rand() % 10) ) {
        wysokoscPoprzedniejPrzeszkody = wysokoscPoprzedniejPrzeszkody + (rand() % 2) - 1 ;
        printf("%d%c",wysokoscPoprzedniejPrzeszkody,'\n');
    }
    if (wysokoscPoprzedniejPrzeszkody > 37) {wysokoscPoprzedniejPrzeszkody = 37;}
    for (int y = wysokoscPoprzedniejPrzeszkody; y < 40; y++) { plansza[y][x] = 1; }
}


int main()
{
    srand(time(NULL));
    for (int i = 0; i < 40; i++) {for (int j = 0; j < 61; j++) {plansza[i][j] = 0;}}
    for (int i = 0; i < 61; i++) {for (int j = 37; j < 40; j++) {plansza[j][i] = 1;}}

    SDL_Window* meowOkno = SDL_CreateWindow (
        "FajneOkno",
        60,
        40,
        1200,
        800,
        SDL_WINDOW_SHOWN);
    SDL_Renderer* meowRender = SDL_CreateRenderer (meowOkno, -1, SDL_RENDERER_ACCELERATED);
    SDL_Event meowEvent;
    SDL_RenderSetLogicalSize(meowRender,400,200);

    while (turnOn)
    {
        //renderowanie
        for (int x = 0+timer; x < 400+timer; x++) {
            for (int y = 200; y < 400; y++)
            {
                SDL_SetRenderDrawColor(meowRender,kolory[ plansza[y/10][x/10] ] [0],kolory[plansza[ y/10 ][ x/10 ]] [1],kolory[plansza[y/10][x/10]][2] ,0 );
                if (floor((y+ktoryElementSkoku)/10) == playerPosition[0] && floor((x-timer)/10) == playerPosition[1] ) {
                    SDL_SetRenderDrawColor(meowRender,kolory[2][0],kolory[2][1],kolory[2][2],0 );
                }
                SDL_RenderDrawPoint(meowRender,x-timer,y-200 );
            }
        }
        //silnik
        while (SDL_PollEvent(&meowEvent)) {
            if (meowEvent.type == SDL_QUIT ) { turnOn = false; }
            else if (meowEvent.type == SDL_KEYDOWN && skokUpadek[0]+skokUpadek[1]+skokUpadek[3] == 0) { skokUpadek[0] = true; skokUpadek[2] = false; }
        }
        SDL_RenderPresent(meowRender);
        SDL_Delay(15 );
        timer++;
        if (timer > 9 ) { timer = 0;
            cofniecie();
            generowanieNowegoFragmentu();
            if (plansza[playerPosition[0]][playerPosition[1]+1] != 0) {turnOn = false; }}
        if (skokUpadek[0] + skokUpadek[1] == false ) {
            if (ktoryElementSkoku == -10) {
                ktoryElementSkoku = 0;
                playerPosition[0]++;
                skokUpadek[3] = true;
            }
            else if (plansza[playerPosition[0]+1][playerPosition[1]] == 0 ) {
                ktoryElementSkoku--;
                skokUpadek[3] = true;
            }
            else {
                skokUpadek[3] = false;
            }
        }
        //skakanie
        if (skokUpadek[0]) {ktoryElementSkoku++; skokUpadek[3] = true;
            if (ktoryElementSkoku == 10) {  playerPosition[0]--; ktoryElementSkoku-=10; skokUpadek[2] = true; }
            else if (ktoryElementSkoku == 5 && skokUpadek[2]) { skokUpadek[0] = false;  skokUpadek[1] = true;}}
        else if (skokUpadek[1]) { ktoryElementSkoku--; skokUpadek[3] = true;
            if (ktoryElementSkoku == 0) { skokUpadek[0] = false;  skokUpadek[1] = false; }}
    }
    SDL_DestroyRenderer (meowRender);
    SDL_DestroyWindow (meowOkno);
}