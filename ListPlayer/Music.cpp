#include "Music.h"
namespace fs = std::filesystem;

Music::Music() : m_head(nullptr), m_tail(nullptr), m_currentNode(nullptr), m_totalCount(0)
{
    const string folder = "./music";//音乐路径（要在VS工程文件夹下）
    for (const auto& entry : fs::directory_iterator(folder))
    {
        if (entry.is_regular_file())
        {
            string path = fs::relative(entry.path()).string();
            addNode(path); // 用链表存储
        }
    }
    if (m_head) m_currentNode = m_head;   //初始选中第一个节点
}

Music::~Music()      // 释放链表内存
{
    MusicNode* cur = m_head;
    while (cur)
    {
        MusicNode* temp = cur;
        cur = cur->next;
        delete temp;
    }
}

void Music::addNode(string path)   //插入节点
{
    MusicNode* newNode = new MusicNode(path);
    m_totalCount++;

    if (!m_head) // 空链表
    {
        m_head = newNode;
        m_tail = newNode;
    }
    else // 尾插法
    {
        m_tail->next = newNode;
        newNode->prev = m_tail;
        m_tail = newNode;
    }
}

void Music::removeNode(MusicNode* node)       //移除节点
{
    if (!node) return;

    if (node->prev)
        node->prev->next = node->next;
    else // 删除头节点
        m_head = node->next;

    if (node->next)
        node->next->prev = node->prev;
    else // 删除尾节点
        m_tail = node->prev;

    delete node;
    m_totalCount--;
}

MusicNode* Music::findNode(const string& name)
{
    MusicNode* cur = m_head;
    while (cur)
    {
        // 提取文件名（同原逻辑）
        size_t lastSlash = cur->path.find_last_of("/\\");
        string fileName = (lastSlash == string::npos) ? cur->path : cur->path.substr(lastSlash + 1);
        if (fileName.size() <= 4) continue;
        string songName = fileName.substr(0, fileName.size() - 4);

        if (songName == name)
            return cur;
        cur = cur->next;
    }
    return nullptr;
}

void Music::show(MusicNode* current)
{
    cout << "音乐列表" << endl;
    MusicNode* cur = m_head;
    int index = 1;
    while (cur)
    {
        if (cur == current)
            cout << index << "\t" << cur->path << "\t" << "<---当前选中" << endl;
        else
            cout << index << "\t" << cur->path << endl;
        cur = cur->next;
        index++;
    }
}