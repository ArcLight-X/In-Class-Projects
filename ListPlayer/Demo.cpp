#include "Player.h"
#include "Music.h"
int main()
{
	auto p = new Player;
	while (true)
	{
		p->menu();          //生成播放器界面
		p->showmusic();     //生成歌单
		p->keyDown();       //开始检测按键
		system("cls");
	}
	return 0;
}
