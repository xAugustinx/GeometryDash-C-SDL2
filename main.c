#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define true 1
#define false 0

int wysokoscPoprzedniejPrzeszkody = 37;
int licznikNygerJakDalekoPoszedlTenPion = 0;
int szybkiLicznikSpadku = 0;
int czyMuzyka = 0;
int wysokoscSkoku;
int skokUpadek[4] = {0,0,0,0};
int ktoryElementSkoku = 0;
int ktoryElementSkokuRenderowany = 0;
int turnOn = true;
int plansza[40][61];
int planszaOnlyBackGround[40][61];
int timer = 0;
int timerOfBackGround = 0;
int playerPosition[2] = {36,25};
int kolory[6][3] =
{
    {29, 67, 181},        //ground
    {13, 2, 110},        //background
    {217, 232, 104},    //player
    {32, 29, 181},     //background2
    {14, 235, 21},    //Jumper
    {125, 17, 17}    //DeathBlock
};

int cofniecie(int mangosTable[40][61] ) {
    for (int x = 1; x < 61; x++) {
        for (int y = 0; y < 40; y++)
        {mangosTable[y][x-1] = mangosTable[y][x];}
    }
}
void generowanieNowegoFragmentu() {
    int x = 60;
    for (int y = 0; y < 40; y++) {plansza[y][x] = 0;}
    if (! (rand() % 10) ) {
        wysokoscPoprzedniejPrzeszkody = wysokoscPoprzedniejPrzeszkody - (rand() % 3) -1;
    }

    if (wysokoscPoprzedniejPrzeszkody > 37) {
        wysokoscPoprzedniejPrzeszkody = 37;
    }
    else if (wysokoscPoprzedniejPrzeszkody < 0) {
        wysokoscPoprzedniejPrzeszkody = 1;
    }


    if (wysokoscPoprzedniejPrzeszkody > 37) {wysokoscPoprzedniejPrzeszkody = 37;}
    for (int y = wysokoscPoprzedniejPrzeszkody; y < 40; y++) { plansza[y][x] = 1; }

    if (! (rand() % 10)) {
        plansza[wysokoscPoprzedniejPrzeszkody-1][x] = 4 + (rand() % 2);
    }
}
void * muzykaWTle() {
    if (czyMuzyka) {
        int licznikCzasu = 198;
        system("Assets/GDSMusicPlayer.unit Assets/gdm.wav  > /dev/null ");
        while (turnOn) {
            licznikCzasu++;
            SDL_Delay(1000);
            if (licznikCzasu == 200) {
                licznikCzasu = 0;
                system("killall GDSMusicPlayer.unit");
                system("Assets/GDSMusicPlayer.unit Assets/gdm.wav  > /dev/null ");
            }
        }
    }
}
void* mainGame()
{
    for (int i = 0; i < 40; i++) {for (int j = 0; j < 61; j++) {plansza[i][j] = 0; planszaOnlyBackGround[i][j] = 0;}}
    for (int i = 0; i < 61; i++) {for (int j = 37; j < 40; j++) {plansza[j][i] = 1;}}

    for (double y = 0; y < 40; y=y+3) {for (int x = 0; x < 61; x=x+3) {
        for (int i = 0; i < 2; i++) { for (int j = 0; j < 2; j++) { planszaOnlyBackGround[(int)y+j][x+i] = 3;  }}
    }}

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
    {   //renderowanie
        for (int x = 100+timer; x < 500+timer; x++) {
            for (int y = 200; y < 400; y++)
            {
                SDL_SetRenderDrawColor(meowRender,kolory[ plansza[y/10][x/10] ] [0],kolory[plansza[ y/10 ][ x/10 ]] [1],kolory[plansza[y/10][x/10]][2] ,0 );

                int iks = floor(x-timer+ timerOfBackGround)/10  ;  int igrek = floor(y)/10;
                int iks2 = floor(x)/10;
                int mangosY = (int)(((double)y/10 - (double)floor(y/10))*10);
                int mangosX = (int)(((double)(x - timer) /10 - (double)floor((x - timer)/10))*10);

                if (floor((y+ktoryElementSkokuRenderowany)/10) == playerPosition[0] && floor((x-timer)/10) == playerPosition[1] ) {
                    SDL_SetRenderDrawColor(meowRender,kolory[2][0],kolory[2][1],kolory[2][2],0 );

                    if ( 7 == ((int)(((double)(y+ ktoryElementSkokuRenderowany)/10 - (double)floor((y+ ktoryElementSkokuRenderowany)/10))*10))){
                        if (mangosX > 2 && mangosX < 8) {
                            SDL_SetRenderDrawColor(meowRender,kolory[4][0],kolory[4][1],kolory[4][2],0 );
                        }}
                    else if (((int)(((double)(y+ ktoryElementSkokuRenderowany)/10 - (double)floor((y+ ktoryElementSkokuRenderowany)/10))*10)) == 3) {
                        if (mangosX == 1 || mangosX == 8) {
                            SDL_SetRenderDrawColor(meowRender,kolory[4][0],kolory[4][1],kolory[4][2],0 );
                        }}
                }
                else if (plansza[ igrek ] [ iks2 ] == 0 || plansza[y/10][x/10] == 4 && mangosY < 5 || plansza[ igrek ] [ iks2 ] == 0 || plansza[y/10][x/10] == 5 && mangosY < 5) {
                    SDL_SetRenderDrawColor(meowRender,kolory[ planszaOnlyBackGround[igrek-20][iks-5] ] [0],kolory[planszaOnlyBackGround[igrek-20][iks-5]] [1],kolory[planszaOnlyBackGround[igrek-20][iks-5]][2] ,0 );}

                SDL_RenderDrawPoint(meowRender,x-timer-100,y-200 );
            }
        }
        //silnik
        while (SDL_PollEvent(&meowEvent)) {
            if (meowEvent.type == SDL_QUIT ) { turnOn = false; }
            else if (meowEvent.type == SDL_KEYDOWN && skokUpadek[0]+skokUpadek[1]+skokUpadek[3] == 0) {
                skokUpadek[0] = true; skokUpadek[2] = false; wysokoscSkoku = 10;
            }
        }
        SDL_RenderPresent(meowRender);
        SDL_Delay(5); //czekanie
        timer++;
        if (timer > 9 ) { timer = 0;
            timerOfBackGround++;
            cofniecie(plansza);
            generowanieNowegoFragmentu();
        }
        if (timerOfBackGround > 30 ) {timerOfBackGround = 0;}

        if (!(plansza[playerPosition[0]][playerPosition[1]+1] == 0 || plansza[playerPosition[0]][playerPosition[1]+1] == 4) )
            {turnOn = false; } //czyPrzegrana
        else if (!(plansza[playerPosition[0]][playerPosition[1]+1] == 5) && wysokoscPoprzedniejPrzeszkody < 5 )
            {turnOn = false; }
        else if (plansza[playerPosition[0]][playerPosition[1]+1] == 4 ) {
            skokUpadek[0] = true; skokUpadek[2] = false; wysokoscSkoku = 50;
        }

        if (skokUpadek[0]+skokUpadek[1] == 0 ) {
            //upadek
            if (plansza[playerPosition[0]+1][playerPosition[1]] == 0 || plansza[playerPosition[0]+1][playerPosition[1]] == 4 || plansza[playerPosition[0]+1][playerPosition[1]] == 5) {
                ktoryElementSkokuRenderowany--;
                if (ktoryElementSkokuRenderowany < -10 ) {
                    ktoryElementSkokuRenderowany = 0;
                    playerPosition[0]++;
                }
            }
            else {skokUpadek[3] = false; ktoryElementSkokuRenderowany = 0; ktoryElementSkoku = 0; }
        }
        //skakanie
        if (skokUpadek[0]) {ktoryElementSkoku++; ktoryElementSkokuRenderowany++; skokUpadek[3] = true;
            if (ktoryElementSkoku!=0&& ((int)(ktoryElementSkoku/10))*10 == ktoryElementSkoku ) { playerPosition[0]--; ktoryElementSkokuRenderowany = 0;   }
            if (ktoryElementSkoku == wysokoscSkoku) {  playerPosition[0]--; ktoryElementSkoku-=10; ktoryElementSkokuRenderowany-=10; skokUpadek[2] = true; }
            else if (ktoryElementSkoku == wysokoscSkoku-5 && skokUpadek[2]) { skokUpadek[0] = false;  skokUpadek[1] = true; szybkiLicznikSpadku = 5; }}
        else if (skokUpadek[1]) {
            szybkiLicznikSpadku--;
            ktoryElementSkoku--; ktoryElementSkokuRenderowany--; skokUpadek[3] = true;
            if (szybkiLicznikSpadku == 0) { skokUpadek[0] = false;  skokUpadek[1] = false; playerPosition[0]++;  ktoryElementSkokuRenderowany = 0; }}
    }
    SDL_DestroyRenderer (meowRender);
    SDL_DestroyWindow (meowOkno);
    if (czyMuzyka) {
        system("killall GDSMusicPlayer.unit");
        system("Assets/GDSMusicPlayer.unit Assets/exit.wav  > /dev/null");}
}
int main() {
    srand(time(NULL));
    if (access("Assets/GDSMusicPlayer.unit", F_OK) == 0) {czyMuzyka = 1;}
    czyMuzyka = 0;
    pthread_t gra;
    pthread_t muzykaWTle2;
    pthread_create(&gra, NULL, mainGame,NULL );
    pthread_create(&muzykaWTle2, NULL, muzykaWTle,NULL );
    pthread_join(gra, NULL);
    pthread_join(muzykaWTle2, NULL);
}
