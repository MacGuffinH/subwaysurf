//
// Created by zuy12 on 2024/1/18.
//

#ifndef SUBWAYSURFER_MG_SUBWAY_H
#define SUBWAYSURFER_MG_SUBWAY_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#define RUN 0
#define T_LEFT 1
#define T_RIGHT 2
#define UP 3
#define DOWN 4

#define MAX 9

#define UP_OB 0
#define DOWN_OB 1
#define ALL_OB 2
#define DEATH_OB 3

#define COIN 0

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
void load();
void stop();


#undef main
#endif //SUBWAYSURFER_MG_SUBWAY_H
