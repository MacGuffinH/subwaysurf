//
// Created by zuy12 on 2024/1/18.
//
#include "mg_subway.h"
/*
 * 项目分析
 * 包含对象：人物，障碍，金币，玩家
 * 游戏操作：跳，下滚，变轨，开始，暂停
 * 实现逻辑：
 * 储存得分和金币数
 * 所有物体都有距离人物距离，轨道和高度
 * 物体和人物同一轨道，同一高度，距离相差不大时会相撞
 * 障碍与主角相撞，主角生命减一
 * 金币与主角相撞，主角金币数加一，金币消失
 * 道具和主角相撞，主角获得道具，道具消失
 * 按特殊键开始，初始化人物，随机出可通过轨道，唯一可通过轨道上有金币
 * 按照位置生成东西
 * 每隔一段时间刷新一次
 */
typedef struct{
    int v;
    int line;
    int action;
    int height;
    int life;
}Character;//人物的速度，轨道和当前动作,高度,生命数
#define UP_OB 0;
#define DOWN_OB 1;
#define ALL_OB 2;
#define DEATH_OB 3;
typedef struct{
    int exist;
    int distance;
    int line;
    int x;
}Obstacle;
typedef struct{
    int line;
    int height;
    int x;
}Object;
void start();
void create(int line,int distance,Obstacle *obs);
void CreateSurvive(int line,int distance,Obstacle *obs);
const int obstacle_dis=4;
int main(){
    start();
    return 0;
}
void start(){
    Character m={100,2,RUN,1,1};
    Obstacle obstacle[20]={0};//3的倍数位为幸存路
    srand(time(0));
    int x;int survive;//随机数
    for(int i=1;i<=6;i++){
        x=rand();survive=x%3+1;
        CreateSurvive(survive,i*obstacle_dis,&obstacle[i*3]);
        for(int j=1;j<=3;j++){
            if(i==survive)continue;
            else create(j,i*obstacle_dis,&obstacle[i*3-3+j]);
        }
    }
}
void create(int line,int distance,Obstacle *obs){
    int x=rand();x=x%5;
    if(x==4)return;
    else{
        obs->exist=1;
        obs->x=x;
        obs->distance=distance;
        obs->line=line;
    }
}
void CreateSurvive(int line,int distance,Obstacle *obs){
    int x=rand();x=x%4;
    if(x==3)return;
    else{
        obs->exist=1;
        obs->x=x;
        obs->distance=distance;
        obs->line=line;
    }
}