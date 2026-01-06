#pragma once
#include "common.h"


struct MusicNode {     // 双向链表节点
    string path;       // 音乐文件路径
    MusicNode* prev;   // 前节点
    MusicNode* next;   // 后节点
    MusicNode(string p) : path(p), prev(nullptr), next(nullptr) {}
};

class Music
{
public:
    Music();
    ~Music();

    MusicNode* getTail() { return m_tail; }  //尾节点获取
    MusicNode* getHead() { return m_head; }  //头节点获取
    MusicNode* getCurrentNode() { return m_currentNode; }  //回到当前节点
    void setCurrentNode(MusicNode* node) { m_currentNode = node; }
    int getTotalCount() { return m_totalCount; }

    void show(MusicNode* current);
    void addNode(string path);
    void removeNode(MusicNode* node);
    MusicNode* findNode(const string& name);

protected:
    MusicNode* m_head;        // 链表头节点
    MusicNode* m_tail;        // 链表尾节点
    MusicNode* m_currentNode; // 当前选中节点
    int m_totalCount;         // 总音乐数量
};