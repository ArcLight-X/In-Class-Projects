#include "Player.h"
#include "Music.h"
#include <limits>
using namespace std;

Player::Player() : m_currentNode(nullptr)
{
    m_music = new Music;
    m_currentNode = m_music->getCurrentNode(); // 初始化当前节点
    m_cmd.push_back("open ");
    m_cmd.push_back("play ");
    m_cmd.push_back("pause ");
    m_cmd.push_back("resume ");
    m_cmd.push_back("close ");
    m_cmd.push_back("moveup ");
    m_cmd.push_back("movedown ");
    m_cmd.push_back("delete ");
}

Player::~Player()
{
    delete m_music; // 释放音乐对象
}

void Player::menu()
{
    cout << "=========音乐播放器=========\n"
        "0.查询歌曲信息\n"
        "1.开始播放\n"
        "2.暂停\n"
        "3.继续\n"
        "4.上一首\n"
        "5.下一首\n"
        "6.上移歌曲\n"
        "7.下移歌曲\n"
        "8.删除歌曲\n"
        "9.统计歌单信息\n"
        "============================\n";
}

void Player::keyDown()
{
    int userKey = 0;
    cout << "请输入操作指令" << endl;
    
    // 测试后改进：应对非数字输入
    if (!(cin >> userKey)) {
        cout << "输入不合法！请输入0-9之间的数字" << endl;
        cin.clear(); // 清除错误状态
        cin.ignore(1024, '\n'); // 用足够大的字符量忽略非法输入
        system("pause");
        return;
    }

    // 测试后改进：应对超限数字输入
    if (userKey < 0 || userKey > 9) {
        cout << "输入不合法！请输入0-9之间的数字" << endl;
        system("pause");
        return;
    }
    switch (userKey)//按0-9，执行不同命令
    {
    case 0:
        findmusic();
        break;
    case 1:
        playmusic();
        break;
    case 2:
        pausemusic();
        break;
    case 3:
        resumemusic();
        break;
    case 5:
        nextmusic();
        break;
    case 4:
        premusic();
        break;
    case 6:
        moveupmusic();
        break;
    case 7:
        movedownmusic();
        break;
    case 8:
        deletemusic();
        break;
    case 9:
        countmusic();
        break;
    }
}


void Player::showmusic()
{
    m_music->show(m_currentNode); // 传入当前节点
}

void Player::playmusic()
{
    if (!m_currentNode) return;
    string opencmd = m_cmd[CMD::Open] + m_currentNode->path;//用CMD命令组合来实现播放，暂停等，以下同理
    mciSendString(opencmd.c_str(), 0, 0, 0);
    string playcmd = m_cmd[CMD::Play] + m_currentNode->path;
    mciSendString(playcmd.c_str(), 0, 0, 0);
}

void Player::pausemusic()
{
    if (!m_currentNode) return;
    string pausecmd = m_cmd[CMD::Pause] + m_currentNode->path;
    mciSendString(pausecmd.c_str(), 0, 0, 0);
}

void Player::resumemusic()
{
    if (!m_currentNode) return;
    string resumecmd = m_cmd[CMD::Resume] + m_currentNode->path;
    mciSendString(resumecmd.c_str(), 0, 0, 0);
}


void Player::nextmusic()
{
    if (!m_currentNode) return; // 先判断节点是否有效

    //关闭当前音乐
    string closecmd = m_cmd[CMD::Close] + m_currentNode->path;
    mciSendString(closecmd.c_str(), 0, 0, 0);

    //切换到下一个节点
    m_currentNode = (m_currentNode->next) ? m_currentNode->next : m_music->getHead(); //（如果是尾节点，循环到头节点）
}


void Player::premusic()
{
    if (!m_currentNode) return;

    string closecmd = m_cmd[CMD::Close] + m_currentNode->path;
    mciSendString(closecmd.c_str(), 0, 0, 0);

    // 切换到上一个节点（如果是头节点，循环到尾节点）
    m_currentNode = (m_currentNode->prev) ? m_currentNode->prev : m_music->getTail();
}

void Player::moveupmusic()
{
    if (!m_currentNode || !m_currentNode->prev) return; // 头节点不能上移

    MusicNode* prevNode = m_currentNode->prev;
    string tempPath = m_currentNode->path;

    m_currentNode->path = prevNode->path;          // 交换路径（保持节点位置不变，仅交换内容，避免复杂指针操作）
    prevNode->path = tempPath;
    
    m_currentNode = prevNode; // 选中上一个节点（视觉指针指向上一个）
}

void Player::movedownmusic()
{
    if (!m_currentNode || !m_currentNode->next) return; // 尾节点不能下移

    MusicNode* nextNode = m_currentNode->next;
    string tempPath = m_currentNode->path;

    m_currentNode->path = nextNode->path;
    nextNode->path = tempPath;

    m_currentNode = nextNode;
}

void Player::deletemusic()
{
    if (!m_currentNode) return;

    string closecmd = m_cmd[CMD::Close] + m_currentNode->path;   // 关闭当前音乐
    mciSendString(closecmd.c_str(), 0, 0, 0);

    MusicNode* delNode = m_currentNode;    // 记录要删除的节点和下一个节点
    MusicNode* nextNode = delNode->next ? delNode->next : delNode->prev;
 
    m_music->removeNode(delNode);  // 从链表移除

    m_currentNode = nextNode;      // 更新当前节点
}


void Player::countmusic()         // countmusic 链表遍历
{
    int totalSongs = m_music->getTotalCount();
    long long totalDuration = 0;

    MusicNode* cur = m_music->getHead();
    while (cur)
    {
        const string& songPath = cur->path;
        char openCmd[512];
        sprintf_s(openCmd, "open \"%s\" alias tempSong", songPath.c_str());
        MCIERROR err = mciSendString(openCmd, 0, 0, 0);

        char returnBuf[256] = { 0 };
        mciSendString("status tempSong length", returnBuf, sizeof(returnBuf), NULL);
        long long songDuration = atoll(returnBuf);
        totalDuration += songDuration;

        mciSendString("close tempSong", 0, 0, 0);
        cur = cur->next;
    }

    long long totalSeconds = totalDuration / 1000;
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

    cout << "========音乐统计信息========\n";
    cout << "总音乐数：" << totalSongs << " 首\n";
    cout << "总时长：" << hours << "小时 " << minutes << "分钟 " << seconds << "秒\n";
    cout << "============================\n";
    system("pause");
}

void Player::findmusic()
{
    cin.ignore();
    string findName;
    cout << "请输入要查询的音乐名称(不带.mp3)：" << endl;
    getline(cin, findName);

    MusicNode* foundNode = m_music->findNode(findName);
    if (!foundNode)
    {
        cout << "未找到音乐：" << findName << endl;
        system("pause");
        return;
    }

    char openCmd[512];
    sprintf_s(openCmd, "open \"%s\" alias tempSong", foundNode->path.c_str());
    MCIERROR err = mciSendString(openCmd, 0, 0, 0);
    if (err != 0)
    {
        cout << "无法打开文件，无法获取时长" << endl;
        mciSendString("close tempSong", 0, 0, 0);
        system("pause");
        return;
    }

    char returnBuf[256] = { 0 };
    mciSendString("status tempSong length", returnBuf, sizeof(returnBuf), NULL);
    long long duration = atoll(returnBuf);
    mciSendString("close tempSong", 0, 0, 0);

    long long seconds = duration / 1000;
    int minutes = seconds / 60;
    seconds %= 60;

    cout << "==========查询结果==========\n";
    cout << "音乐名称：" << findName << endl;

    int index = 1;
    MusicNode* cur = m_music->getHead();
    while (cur != foundNode)
    {
        index++;
        cur = cur->next;
    }
    cout << "位置：第 " << index << " 首" << endl;
    cout << "时长：" << minutes << "分" << seconds << "秒" << endl;
    cout << "============================\n";
    system("pause");
}