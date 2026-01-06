#include "common.h"
#pragma once
class Music;
struct MusicNode; // 声明链表节点

class Player
{
public:
    enum CMD
    {
        Open, Play, Pause, Resume, Close,
        MoveUp, MoveDown, Delete // 枚举命令
    };
    Player();
    ~Player(); 

    void menu();
    void keyDown();
    void showmusic();
    void playmusic();
    void pausemusic();
    void resumemusic();
    void nextmusic();
    void premusic();
    void moveupmusic();
    void movedownmusic();
    void deletemusic();
    void countmusic();
    void findmusic();


protected:
    MusicNode* m_currentNode; //当前节点
    vector<string> m_cmd;     //cmd命令
    Music* m_music;           //音乐对象
};