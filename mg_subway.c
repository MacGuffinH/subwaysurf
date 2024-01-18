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
#define RUN 0
#define T_LEFT 1
#define T_RIGHT 2
#define UP 3
#define DOWN 4
struct character{
    int v;
    int line;
    int action;
    int height;
    int life;
}Character;//人物的速度，轨道和当前动作,高度,生命数
int main(){


    return 0;
}