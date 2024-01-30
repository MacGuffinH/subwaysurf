#include "mg_subway.h"

const int obstacle_dis = 650;
const int hit_obs = 100;
const int hit_ob = 100;
const int vplus = 5000;
const int again = 400;
int ok = 1;
int n;
int survive[2] = {0};
char a[10] = {0};
int money, s, score;
int get_m, get_s, get_sc;//刷新前得到的金币，距离和分数
void change(SDL_Event event, Character *m);

void Draw(Obstacle *obstacle);

void LoadTexture(SDL_Surface *sur, SDL_Texture **texture);

int ti ,ti_ob,  ti_fly,ti_magnet;
SDL_Window *Window;
SDL_Texture *cha_, *ch_jump_, *ch_down_, *coin_, *ob_up_, *ob_down_, *ob_all_, *ob_death_, *back, *tip_, *ch_sk_, *score__, *fly_,*magnet_ = NULL;
SDL_Surface *tip, *background, *cha, *ch_jump, *ch_down, *coin, *ob_up, *ob_down, *ob_all, *ob_death, *ch_sk, *score_, *fly,*magnet = NULL;
TTF_Font *f = NULL;
SDL_Color color = {0};
SDL_Rect reob, reo;
SDL_Rect i_rect = {0, 0, 550, 650};
SDL_Renderer *Renderer = NULL;
SDL_Rect kpi = {0, 0, 50, 50};

void DrawCh(Character *m);

void Draw2(Object *object);

void wait();

void shutdown();

void createOb(int line, int distance, Object *object);

int main() {
    wait();
    load();
    SDL_Event event;
    while (ok) {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)break;
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p) {
            ti = ti_ob = ti_fly =ti_magnet= n = 0;
            SDL_RenderClear(Renderer);
            SDL_RenderCopy(Renderer, back, NULL, NULL);
            reob.w = reob.h = 100;
            reo.w = reo.h = 60;
            money = s = score = 0;//3的倍数位为幸存路
            Character m = {10, 2, RUN, 1, 1};
            DrawCh(&m);
            Obstacle obstacle[8] = {0};
            Object object[8] = {0};
            srand(time(0));
            int x;
            x = rand();
            survive[0] = x % 3 + 1;
            CreateSurvive(survive[0], obstacle_dis, &obstacle[3], object);
            for (int j = 1; j <= 3; j++) {
                if (j == survive[0])continue;
                else {
                    create(j, obstacle_dis, &obstacle[j - 1]);
                }
            }
            SDL_RenderPresent(Renderer);
            for (int i = 1; i <= 3; i++)printf("%d ", obstacle[i - 1].exist == 1 ? obstacle[i - 1].x : obstacle[3].x);
            printf("s:%d l:%d h:%d m:%d %d\n", survive[0], m.line, m.height, money, again - s);
            n = 4;
            //printf("%d %d\n",reo.x,reo.y);
            while (ok && m.life) {
                SDL_Delay(40);
                refresh(1, obstacle, object, &m);
                s += m.v * 1;
                if (s >= again) {
                    for (int i = 0 + n; i <= 3 + n; i++) {
                        obstacle[i].exist = 0;
                        object[i].exist = 0;
                    }
                    s = 0;
                    x = rand();
                    survive[n / 4] = x % 3 + 1;
                    CreateSurvive(survive[n / 4], obstacle_dis, &obstacle[3 + n], object);
                    for (int j = 1 + n; j <= 3 + n; j++) {
                        if (j - n == survive[n / 4])continue;
                        else if (x % 6 != 5)create(j - n, obstacle_dis, &obstacle[j - 1]);
                        else {
                            obstacle[j - 1].exist = 0;
                            createOb(j - n, obstacle_dis, &object[j - 1]);

                        }
                    }
                    if (n == 4)n = 0;
                    else n = 4;
                }
                SDL_PollEvent(&event);
                if (event.type == SDL_KEYDOWN)change(event, &m);
                if (event.type == SDL_QUIT)shutdown();
                SDL_RenderPresent(Renderer);
            }
        }
    }
    shutdown();
    return 0;
}


void LoadTexture(SDL_Surface *sur, SDL_Texture **texture) {
    if (texture)SDL_DestroyTexture(*texture);
    *texture = SDL_CreateTextureFromSurface(Renderer, sur);
    SDL_FreeSurface(sur);
}

void load() {
    magnet= IMG_Load("Image/12.png");
    LoadTexture(magnet,&magnet_);
    fly = IMG_Load("Image/13.png");
    LoadTexture(fly, &fly_);
    coin = IMG_Load("Image/8.png");
    LoadTexture(coin, &coin_);
    cha = IMG_Load("Image/3.png");
    LoadTexture(cha, &cha_);
    ch_jump = IMG_Load("Image/10.png");
    LoadTexture(ch_jump, &ch_jump_);
    ch_down = IMG_Load("Image/9.png");
    LoadTexture(ch_down, &ch_down_);
    ob_up = IMG_Load("Image/4.png");
    LoadTexture(ob_up, &ob_up_);
    ob_down = IMG_Load("Image/6.png");
    LoadTexture(ob_down, &ob_down_);
    ob_all = IMG_Load("Image/5.png");
    LoadTexture(ob_all, &ob_all_);
    ob_death = IMG_Load("Image/7.png");
    LoadTexture(ob_death, &ob_death_);
    ch_sk = IMG_Load("Image/11.png");
    LoadTexture(ch_sk, &ch_sk_);
    f = TTF_OpenFont("Image/GenshinDefault.ttf", 16);
}

void wait() {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
    SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
    SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
    SDL_EventState(SDL_KEYUP, SDL_IGNORE);
    Window = SDL_CreateWindow("HonokaGo!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 800, SDL_WINDOW_SHOWN);
    Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderClear(Renderer);
    background = IMG_Load("1.png");
    LoadTexture(background, &back);
    tip = IMG_Load("Image/0.png");
    LoadTexture(tip, &tip_);
    score_ = TTF_RenderText_Blended(f, "H", color);
    LoadTexture(score_, &score__);
    SDL_RenderCopy(Renderer, back, NULL, NULL);
    SDL_RenderCopy(Renderer, tip_, NULL, &i_rect);
    SDL_RenderCopy(Renderer, score__, NULL, NULL);
    SDL_RenderPresent(Renderer);
}


void DrawCh(Character *m) {
    reob.x = 105 * m->line;
    reob.y = 650;
    switch (m->action) {
        case RUN:
            if(!m->have)SDL_RenderCopy(Renderer, cha_, &i_rect, &reob);
            else SDL_RenderCopy(Renderer,magnet_,NULL,&reob);
            break;
        case UP:
            SDL_RenderCopy(Renderer, ch_jump_, &i_rect, &reob);
            break;
        case SKATEBOARD:
            SDL_RenderCopy(Renderer, ch_sk_, NULL, &reob);
            break;
        case FLYING:
            SDL_RenderCopy(Renderer, fly_, NULL, &reob);
            break;
        default:
            SDL_RenderCopy(Renderer, ch_down_, &i_rect, &reob);
            break;
    }
}

void Draw(Obstacle *obstacle) {
    if (!(obstacle->exist))return;
    reob.x = obstacle->line * 105;
    reob.y = 650 - obstacle->distance;
    if (obstacle->x == UP_OB)SDL_RenderCopy(Renderer, ob_up_, NULL, &reob);
    else if (obstacle->x == DOWN_OB)SDL_RenderCopy(Renderer, ob_down_, &i_rect, &reob);
    else if (obstacle->x == ALL_OB)SDL_RenderCopy(Renderer, ob_all_, &i_rect, &reob);
    else if (obstacle->x == DEATH_OB)SDL_RenderCopy(Renderer, ob_death_, &i_rect, &reob);
}

void Draw2(Object *object) {
    if (!(object->exist))return;
    if (object->x == COIN) {
        reo.y = 650 - object->distance;
        reo.x = object->line * 105;
        SDL_RenderCopy(Renderer, coin_, NULL, &reo);
        return;
    }
    reob.y = 650 - object->distance;
    reob.x = object->line * 105;
    if (object->x == FLY)SDL_RenderCopy(Renderer, fly_, NULL, &reob);
    else if(object->x==MAGNET)SDL_RenderCopy(Renderer,magnet_,NULL,&reob);
}


void CreateSurvive(int line, int distance, Obstacle *obs, Object object[]) {
    int x = rand();
    x = x % 4;
    int b = 3 + n;
    object[b].exist = 1;
    object[b].x = COIN;
    object[b].line = line;
    object[b].distance = distance;
    if (x == 3) {
        object[b].height = 1;
        obs->exist = 0;
        Draw2(&object[b]);
        return;
    } else {
        obs->exist = 1;
        obs->x = x;
        obs->distance = distance;
        obs->line = line;
        x = rand();
        x = x % 3 - 1;
        object[b].v = obs->v = x;
        if (obs->x == UP_OB) {
            object[b].height = 2;
        } else if (obs->x == DOWN_OB) {
            object[b].height = 0;
        } else {
            object[b].height = 2;
        }
    }
    Draw(obs);
    Draw2(&object[b]);
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
        x = x % 5 - 2;
        obs->v = x;
        Draw(obs);
    }
}

void createOb(int line, int distance, Object *object) {
    int x = rand();
    x = x % 3;
    object->height = 1;
    object->exist = 1;
    object->x = x;
    object->distance = distance;
    object->line = line;
    x = x % 5 - 2;
    object->v = x;
    Draw2(object);
}

void refresh(int t, Obstacle obs[8], Object ob[8], Character *ch) {
    if (ti)ti++;
    if (ti >= 25) {
        ti = 0;
        ch->height = 1;
        if (ch->life == 1)ch->action = RUN;
        else if (ch->life == 2)ch->action = SKATEBOARD;
    }
    if (ti_ob && !ti_fly)ti_ob++;
    if (ti_ob == 250) {
        ch->action = RUN;
        ch->life--;
        ti_ob = 0;
    }
    if (ti_fly&&!ti_magnet)ti_fly++;
    if (ti_fly == 225) {
        if (ti_ob)ch->action = SKATEBOARD;
        else ch->action = RUN;
    }
    if(ti_fly==250)ch->height=1;
    if (ti_magnet)ti_magnet++;
    if (ti_magnet == 250) {
        ti_magnet=0;
        ch->have=0;
    }
    for (int i = 0; i <= 7; i++) {
        obs[i].distance -= t * ((ch->v) + obs[i].v);
        ob[i].distance -= t * ((ch->v) + ob[i].v);
        if (obs[i].exist && obs[i].line == ch->line && obs[i].distance <= hit_obs) {
            printf("Over:%d %d %d\n", obs[i].x, obs[i].exist, i);
            //printf("cool\n");
            if (obs[i].exist && (obs[i].x == UP_OB && (ch->height == 1 || ch->height == 0)) ||
                (obs[i].x == DOWN_OB && (ch->height == 1 || ch->height == 2)) ||
                (obs[i].x == ALL_OB && ch->height == 1) ||
                (obs[i].x == DEATH_OB && (ch->height == 1 || ch->height == 0 || ch->height == 2))) {
                if (ch->life == 2) {
                    ch->action = RUN;
                    ch->height = 1;
                    ti = 0;
                }
                ch->life--;
            }
            obs[i].exist = 0;
        }
        if (obs[i].distance <= hit_obs)obs[i].exist = 0;
        if (ob[i].exist && (ob[i].line == ch->line||(ch->have&&ob[i].x==COIN) )&& ob[i].distance >= -hit_ob && ob[i].distance <= hit_ob &&
                (ob[i].height == ch->height||ch->height==3)) {
            if (ob[i].x == COIN)get_m++;
            if (ob[i].x == FLY) {
                ch->action = FLYING;
                ch->height = 3;
                ti_fly = 1;
                ch->action = FLYING;
            }
            if(ob[i].x==MAGNET){
                ch->have=1;
                ti_magnet=1;
            }
            ob[i].exist = 0;
            //printf("Cool\n");
        }
    }
    SDL_RenderClear(Renderer);
    SDL_RenderCopy(Renderer, back, NULL, NULL);
    DrawCh(ch);
    /*for (int i = 1; i <= 3; i++) {
        if (i == survive[0])Draw(&obs[3]);
        else Draw(&obs[i - 1]);
    }
    for (int i = 5; i <= 7; i++) {
        if (i - 4 == survive[1])Draw(&obs[7]);
        else Draw(&obs[i - 1]);
    }*/
    for (int i = 0; i <= 7; i++) {
        Draw(&obs[i]);
    }
    for (int i = 0; i <= 7; i++) {
        Draw2(&ob[i]);
    }
    score += ch->v * t + get_m;
    if (ch->v + score / vplus >= MAX)ch->v = MAX;
    else ch->v += score / vplus;
    money += get_m;
    if (get_m)printf("%d %d\n", money, score);
    get_m = 0;
    itoa(score, a, 10);
    score_ = TTF_RenderText_Blended(f, a, color);
    LoadTexture(score_, &score__);
    kpi.y=0;
    SDL_RenderCopy(Renderer, score__, NULL, &kpi);
    itoa(money,a,10);
    score_= TTF_RenderText_Blended(f,a,color);
    LoadTexture(score_,&score__);
    kpi.y=30;
    SDL_RenderCopy(Renderer,score__,NULL,&kpi);
    if(ch->have)score_= TTF_RenderText_Blended(f,"with",color);
    else score_= TTF_RenderText_Blended(f,"without",color);
    LoadTexture(score_,&score__);
    kpi.y=60;
    SDL_RenderCopy(Renderer,score__,NULL,&kpi);
}

void change(SDL_Event event, Character *m) {
    if (event.key.keysym.sym == SDLK_a && m->line != 1) {
        m->line--;
    } else if (event.key.keysym.sym == SDLK_d && m->line != 3) {
        m->line++;
    } else if (event.key.keysym.sym == SDLK_w && m->height <= 1) {
        ti = 1;
        m->height = 2;
        m->action = UP;
    } else if (event.key.keysym.sym == SDLK_s && m->height >= 1 && m->height < 3) {
        ti = 1;
        m->height = 0;
        m->action = DOWN;
    } else if (event.key.keysym.sym == SDLK_p) {
        SDL_RenderCopy(Renderer, tip_, NULL, &i_rect);
        SDL_RenderPresent(Renderer);
        while (1) {
            SDL_Delay(20);
            SDL_PollEvent(&event);
            if (event.key.keysym.sym == SDLK_p)break;
            if (event.type == SDL_QUIT) {
                shutdown();
                break;
            }
        }
    } else if (event.key.keysym.sym == SDLK_SPACE && m->action != FLYING) {
        ti_ob = 1;
        m->life = 2;
        m->action = SKATEBOARD;
    }
}

void shutdown() {
    ok = 0;
    SDL_DestroyTexture(magnet_);
    SDL_DestroyTexture(fly_);
    SDL_DestroyTexture(ch_sk_);
    SDL_DestroyTexture(tip_);
    SDL_DestroyTexture(cha_);
    SDL_DestroyTexture(ch_jump_);
    SDL_DestroyTexture(ch_down_);
    SDL_DestroyTexture(coin_);
    SDL_DestroyTexture(ob_up_);
    SDL_DestroyTexture(ob_down_);
    SDL_DestroyTexture(ob_all_);
    SDL_DestroyTexture(ob_death_);
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    IMG_Quit();
    SDL_Quit();
}