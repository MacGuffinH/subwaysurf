#include "mg_subway.h"
#define MAX 50
const int obstacle_dis = 600;
const int hit_obs = 50;
const int hit_ob = 25;
const int vplus = 10;
const int again = 500;
int money, s, score;
int get_m, get_s, get_sc;//刷新前得到的金币，距离和分数
void change(char action, Character *m);
void Draw(int x, SDL_Rect *rect);
SDL_Window *Window;
SDL_Surface *surface, *background, *cha, *coin, *ob_up, *ob_down, *ob_all, *ob_death = NULL;
SDL_Rect rech, reob, reo;
void Draw2(int x, SDL_Rect *rect) {
    if (x == COIN)SDL_BlitSurface(coin, NULL, surface, &reob);
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
        Character m = {1, 2, RUN, 1, 1};
        rech.x = 200;
        rech.y = 650;
        SDL_BlitSurface(cha,NULL,surface,&rech);
        Obstacle obstacle[4] = {0};
        Object object[4] = {0};
        srand(time(0));
        int x;
        int survive;//随机数
        x = rand();
        survive = x % 3 + 1;
        CreateSurvive(survive, obstacle_dis, &obstacle[3], object);
        for (int j = 1; j <= 3; j++) {
            if (j == survive)continue;
            else {
                reob.x = j * 100;
                create(j, obstacle_dis, &obstacle[j - 1]);
            }
        }
        SDL_UpdateWindowSurface(Window);
        //for (int i = 1; i <= 3; i++)printf("%d ", obstacle[i - 1].exist == 1 ? obstacle[i - 1].x : obstacle[3].x);
        //printf("s:%d l:%d h:%d m:%d %d\n", survive, m.line, m.height, money, again - s);
        printf("%d %d\n",reo.x,reo.y);
        while (1) {
            SDL_BlitSurface(background, NULL, surface, NULL);
            refresh(1, obstacle, object, &m);
            s += m.v * 1;
            if (0 && s >= again) {
                s = 0;
                x = rand();
                survive = x % 3 + 1;
                CreateSurvive(survive, obstacle_dis, &obstacle[3], object);
                for (int j = 1; j <= 3; j++) {
                    if (j == survive)continue;
                    else create(j, obstacle_dis, &obstacle[j - 1]);
                }
            }
            printf("%d %d\n ",reo.x,reo.y);
            //printf("s:%d l:%d h:%d m:%d %d\n", survive, m.line, m.height, money, again - s);
            SDL_Delay(1000);
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
    SDL_BlitSurface(cha, NULL, surface, &rech);
    for (int i = 1; i <= 3; i++) {
        obs[i].distance -= t * (ch->v);
        ob[i].distance -= t * (ch->v);
        if (obs[i].exist && obs[i].line == ch->line && obs[i].distance >= -hit_obs && obs[i].distance <= hit_obs) {
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
    reo.y = reob.y += t * (ch->v);
    reob.x = 100;
    for (int i = 1; i <= 3; i++) {
        if (ob[i].exist)Draw(ob[i].x, &reob);
        reob.x += 100;
    }
    Draw2(COIN, &reo);
    score += ch->v * t + get_m;
    if (ch->v + score / vplus >= MAX)ch->v = MAX;
    else ch->v += score / vplus;
    money += get_m;
    get_m = 0;
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
        reob.y = 50;
    }
    switch (x) {
        case UP_OB:
            SDL_BlitSurface(ob_up, NULL, surface, &reob);
            break;
        case DOWN_OB:
            SDL_BlitSurface(ob_down, NULL, surface, &reob);
            break;
        case ALL_OB:
            SDL_BlitSurface(ob_all, NULL, surface, &reob);
            break;
        case DEATH_OB:
            SDL_BlitSurface(ob_death, NULL, surface, &reob);
            break;
        default:
    }
}

void CreateSurvive(int line, int distance, Obstacle *obs, Object object[]) {
    int x = rand();
    x = x % 4;
    object[3].exist = 1;
    object[3].x = COIN;
    object[3].line = line;
    object[3].distance = distance;
    if (x == 3) {
        object[3].height = 1;
        return;
    } else {
        obs->exist = 1;
        obs->x = x;
        obs->distance = distance;
        obs->line = line;
        reo.x = reob.x = line * 100;
        reo.y = reob.y = 50;
        reob.h = 10;
        reob.w = 10;
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

void change(char action, Character *m) {
    if (action == 'A' && m->line != 1)m->line--;
    else if (action == 'D' && m->line != 3)m->line++;
    else if (action == 'W' && m->height == 1)m->height++;
    else if (action == 'S' && m->height == 1)m->height--;
}

void load() {
    coin = IMG_Load("Image/8.png");
    cha = IMG_Load("Image/3.png");
    ob_up = IMG_Load("Image/4.jpg");
    ob_down = IMG_Load("Image/6.jpg");
    ob_all = IMG_Load("Image/5.jpg");
    ob_death = IMG_Load("Image/7.png");
}

void Draw(int x, SDL_Rect *rect) {
    if (x == UP_OB)SDL_BlitSurface(ob_up, NULL, surface, &reob);
    if (x == DOWN_OB)SDL_BlitSurface(ob_down, NULL, surface, &reob);
    if (x == ALL_OB)SDL_BlitSurface(ob_all, NULL, surface, &reob);
    if (x == DEATH_OB)SDL_BlitSurface(ob_all, NULL, surface, &reob);
}
