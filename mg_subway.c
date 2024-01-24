#include "mg_subway.h"

#define MAX 10
const int obstacle_dis = 600;
const int hit_obs =100;
const int hit_ob = 30;
const int vplus = 10000;
const int again = 600;
int survive;
int money, s, score;
int get_m, get_s, get_sc;//刷新前得到的金币，距离和分数
void change(SDL_Event event, Character *m);
void Draw(int x, int exist);
int ti=0;
SDL_Window *Window;
SDL_Surface *surface, *background, *cha,*ch_jump,*ch_down, *coin, *ob_up, *ob_down, *ob_all, *ob_death = NULL;
SDL_Rect rech, reob, reo;
void DrawCh(Character *m){
    switch (m->action) {
        case RUN:SDL_BlitSurface(cha,NULL,surface,&rech);
            break;
        case UP:SDL_BlitSurface(ch_jump,NULL,surface,&rech);
            break;
            default:SDL_BlitSurface(ch_down,NULL,surface,&rech);
            break;
    }
}
void Draw2(int x, int exist) {
    if (!exist)return;
    if (x == COIN)SDL_BlitSurface(coin, NULL, surface, &reo);
}

void wait();

int main() {
    wait();
    SDL_Event event;
    while (1) {
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p))break;
        }
    }
    if (event.key.keysym.sym == SDLK_p) {
        load();
        money = s = score = 0;//3的倍数位为幸存路
        Character m = {5, 2, RUN, 1, 1};
        rech.x = 200;
        rech.y = 650;
        SDL_BlitSurface(cha, NULL, surface, &rech);
        Obstacle obstacle[4] = {0};
        Object object[4] = {0};
        srand(time(0));
        int x;
        x = rand();
        survive = x % 3 + 1;
        CreateSurvive(survive, obstacle_dis, &obstacle[3], object);
        for (int j = 1; j <= 3; j++) {
            if (j == survive)continue;
            else {
                create(j, obstacle_dis, &obstacle[j - 1]);
            }
        }
        SDL_UpdateWindowSurface(Window);
        for (int i = 1; i <= 3; i++)printf("%d ", obstacle[i - 1].exist == 1 ? obstacle[i - 1].x : obstacle[3].x);
        printf("s:%d l:%d h:%d m:%d %d\n", survive, m.line, m.height, money, again - s);
        //printf("%d %d\n",reo.x,reo.y);
        while (m.life) {
            SDL_Delay(20);
            refresh(1, obstacle, object, &m);
            s += m.v * 1;
            if (s >= again) {
                for(int i=0;i<=3;i++)obstacle[i].exist=0;
                s = 0;
                x = rand();
                survive = x % 3 + 1;
                CreateSurvive(survive, obstacle_dis, &obstacle[3], object);
                for (int j = 1; j <= 3; j++) {
                    if (j == survive)continue;
                    else create(j, obstacle_dis, &obstacle[j - 1]);
                }
            }
            SDL_PollEvent(&event);
            if(event.type==SDL_KEYDOWN)change(event,&m);
            SDL_UpdateWindowSurface(Window);
        }
    }
    SDL_FreeSurface(background);
    SDL_DestroyWindow(Window);
    SDL_Quit();
    return 0;
}

void wait() {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
    SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
    SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
    SDL_EventState(SDL_KEYUP, SDL_IGNORE);
    Window = SDL_CreateWindow("HonokaGo!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 800, SDL_WINDOW_SHOWN);
    surface = SDL_GetWindowSurface(Window);
    background = IMG_Load("1.png");
    SDL_BlitSurface(background, NULL, surface, NULL);
    SDL_UpdateWindowSurface(Window);
}

void refresh(int t, Obstacle obs[4], Object ob[4], Character *ch) {
    if(ti)ti++;
    if(ti==30){
        ch->action=RUN;
        ch->height=1;
        ti=0;
    }
    for (int i = 1; i <= 3; i++) {
        obs[i].distance -= t * (ch->v);
        ob[i].distance -= t * (ch->v);
        if (obs[i].exist && obs[i].line == ch->line && obs[i].distance <= hit_obs) {
            obs[i].exist = 0;
            printf("cool\n");
            if ((obs[i].x == UP_OB && (ch->height == 1 || ch->height == 0)) ||
                (obs[i].x == DOWN_OB && (ch->height == 1 || ch->height == 2)) ||
                (obs[i].x == ALL_OB && ch->height == 1) ||
                (obs[i].x == DEATH_OB && (ch->height == 1 || ch->height == 0 || ch->height == 2))) {
                ch->life--;
                printf("Game over!\n");
            }
        }
        if (ob[i].exist && ob[i].line == ch->line && ob[i].distance >= -hit_ob && ob[i].distance <= hit_ob &&
            ob[i].height == ch->height) {
            if (ob[i].x == COIN)get_m++;
            ob[i].exist = 0;
        }
    }
    rech.x = 100 * ch->line;
    rech.y = 650;
    reo.y = reob.y += t * (ch->v);
    reob.x = 105;
    reo.x = survive * 105;
    SDL_BlitSurface(background, NULL, surface, NULL);
    DrawCh(ch);
    for (int i = 1; i <= 3; i++) {
        if (i == survive)Draw(obs[3].x, obs[3].exist);
        else Draw(obs[i - 1].x, obs[i - 1].exist);
        reob.x += 105;
    }
    Draw2(COIN, ob[3].exist);
    score += ch->v * t + get_m;
    if (ch->v + score / vplus >= MAX)ch->v = MAX;
    else ch->v += score / vplus;
    money += get_m;
    get_m = 0;
    printf("%d",ch->height);
}

void create(int line, int distance, Obstacle *obs) {
    int x = rand();
    x = x % 5;
    if (x == 4)return;
    else {
        obs->exist = 1;
        obs->x = x;
        obs->distance = distance;
        obs->line = line;
        reob.x = 105 * line;
        reob.y = 50;
        Draw(obs->x,obs->exist);
    }
}

void CreateSurvive(int line, int distance, Obstacle *obs, Object object[]) {
    int x = rand();
    x = x % 4;
    object[3].exist = 1;
    object[3].x = COIN;
    object[3].line = line;
    object[3].distance = distance;
    reo.x = 105 * line;
    reo.y = 50;
    if (x == 3) {
        object[3].height = 1;
        SDL_BlitSurface(coin, NULL, surface, &reo);
        return;
    } else {
        obs->exist = 1;
        obs->x = x;
        obs->distance = distance;
        obs->line = line;
        reob.x = line * 105;
        reob.y = 50;
        if (x == UP_OB) {
            object[3].height = 2;
            SDL_BlitSurface(ob_up, NULL, surface, &reob);
        } else if (x == DOWN_OB) {
            object[3].height = 0;
            SDL_BlitSurface(ob_down, NULL, surface, &reob);
        } else {
            SDL_BlitSurface(ob_all, NULL, surface, &reob);
            object[3].height = 2;
        }
    }
    SDL_BlitSurface(coin, NULL, surface, &reo);
}

void change(SDL_Event event, Character *m) {
    if (event.key.keysym.sym==SDLK_a&& m->line != 1){
        m->line--;
        rech.x-=105;
    }
    else if (event.key.keysym.sym==SDLK_d && m->line != 3){
        m->line++;
        rech.x-=105;
    }
    else if (event.key.keysym.sym==SDLK_w && m->height == 1){
        ti=1;
        m->height=2;
        m->action=UP;
    }
    else if (event.key.keysym.sym==SDLK_s && m->height == 1){
        ti=1;
        m->height=0;
        m->action=DOWN;
    }
}

void load() {
    coin = IMG_Load("Image/8.png");
    cha = IMG_Load("Image/3.png");
    ch_jump= IMG_Load("Image/10.jpg");
    ch_down= IMG_Load("Image/9.jpg");
    ob_up = IMG_Load("Image/4.jpg");
    ob_down = IMG_Load("Image/6.jpg");
    ob_all = IMG_Load("Image/5.jpg");
    ob_death = IMG_Load("Image/7.png");
}

void Draw(int x, int exist) {
    if (!exist)return;
    if (x == UP_OB)SDL_BlitSurface(ob_up, NULL, surface, &reob);
    if (x == DOWN_OB)SDL_BlitSurface(ob_down, NULL, surface, &reob);
    if (x == ALL_OB)SDL_BlitSurface(ob_all, NULL, surface, &reob);
    if (x == DEATH_OB)SDL_BlitSurface(ob_death, NULL, surface, &reob);
}
