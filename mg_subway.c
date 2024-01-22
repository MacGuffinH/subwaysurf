#include "mg_subway.h"
typedef struct {
    int v,line;
    int action;
    int height;
    int life;
} Character;//人物的速度，轨道和当前动作,高度,生命数
typedef struct {
    int exist;
    int distance;
    int line;
    int x;
} Obstacle;
typedef struct {
    int distance;
    int line;
    int height;
    int exist;
    int x;
} Object;
void create(int line, int distance, Obstacle *obs);
void CreateSurvive(int line, int distance, Obstacle *obs, Object object[]);
void refresh(int t, Obstacle obs[], Object ob[], Character *ch);
void stop();
const int obstacle_dis = 5;
const int hit_obs = 1;
const int hit_ob = 0;
const int vplus = 10;
const int again = 5;
int money, s, score;
int get_m, get_s, get_sc;//刷新前得到的金币，距离和分数
void change(char action, Character *m);
SDL_Window *Window;
SDL_Renderer *Renderer;
int main() {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
    SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
    SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
    SDL_EventState(SDL_KEYUP, SDL_IGNORE);
    Window = SDL_CreateWindow("HonokaGo!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 725, 800, SDL_WINDOW_SHOWN);
    SDL_Surface *surface= SDL_GetWindowSurface(Window);
    SDL_Surface *background= SDL_LoadBMP("1.bmp");
    SDL_BlitSurface(background,NULL,surface,NULL);
    SDL_UpdateWindowSurface(Window);
    char action;
    SDL_Event event;
    while (1){
        if(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT||event.type==SDL_KEYDOWN)break;
        }
    }
    if (event.key.keysym.sym==SDLK_p) {
        money = s = score = 0;//3的倍数位为幸存路
        Character m = {1, 2, RUN, 1, 1};
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
            else create(j, obstacle_dis, &obstacle[j - 1]);
        }
        for (int i = 1; i <= 3; i++) {
            printf("%d ", obstacle[i - 1].exist == 1 ? obstacle[i - 1].x : obstacle[3].x);
        }
        printf("s:%d l:%d h:%d m:%d %d\n", survive, m.line, m.height, money,again-s);
        scanf("%c", &action);
        getchar();
        change(action, &m);
        while (1) {
            refresh(1, obstacle, object, &m);
            s += m.v * 1;
            if (s >= again) {
                x = rand();
                survive = x % 3 + 1;
                CreateSurvive(survive, obstacle_dis, &obstacle[3], object);
                for (int j = 1; j <= 3; j++) {
                    if (j == survive)continue;
                    else create(j, obstacle_dis, &obstacle[j - 1]);
                }
            }
            for (int i = 1; i <= 3; i++) {
                printf("%d ", obstacle[i - 1].exist == 1 ? obstacle[i - 1].x : obstacle[3].x);
            }
            printf("s:%d l:%d h:%d m:%d %d\n", survive, m.line, m.height, money,again-s);
            scanf("%c", &action);
            getchar();
            change(action, &m);
        }
    }
    SDL_DestroyWindow(Window);
    SDL_Quit();
    return 0;
}

void refresh(int t, Obstacle obs[4], Object ob[4], Character *ch) {
    for (int i = 1; i <= 3; i++) {
        obs[i].distance -= t * (ch->v);
        ob[i].distance -= t * (ch->v);
        if (obs[i].exist && obs[i].line == ch->line && obs[i].distance >= -hit_obs && obs[i].distance <= hit_obs) {
            {
                obs[i].exist = 0;
                printf("cool\n");
            }
            if ((obs[i].x == UP_OB && (ch->height == 1 || ch->height == 0)) ||
                (obs[i].x == DOWN_OB && (ch->height == 1 || ch->height == 2)) ||
                (obs[i].x == ALL_OB && ch->height == 1) ||
                (obs[i].x == DEATH_OB && (ch->height == 1 || ch->height == 0 || ch->height == 2))) {
                ch->life--;
                printf("Game over!\n");
            }
            //if(ch->life==0)stop();
        }
        if (ob[i].exist && ob[i].line == ch->line && ob[i].distance >= -hit_ob && ob[i].distance <= hit_ob &&
            ob[i].height == ch->height) {
            if (ob[i].x == COIN)get_m++;
            ob[i].exist = 0;
        }
    }
    score += ch->v * t + get_m;
    ch->v += score / vplus;
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
        if (x == UP_OB) {
            object[3].height = 2;
        } else if (x == DOWN_OB) {
            object[3].height = 0;
        } else {
            object[3].height = 2;
        }
    }
}
void change(char action, Character *m) {
    if (action == 'A' && m->line != 1)m->line--;
    else if (action == 'D' && m->line != 3)m->line++;
    else if (action == 'W')m->height++;
    else if (action == 'S')m->height--;
}
