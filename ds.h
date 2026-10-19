#pragma once

struct Node {
    int value;
    Node* left;
    Node* right;

    Node(int v) : value(v), left(nullptr), right(nullptr) {}
};

class Tree {
private:
    Node* root;

public:
    Tree();
    ~Tree();

    void insert(int value);
    bool search(int value);
    bool isComplete();
    void print();

private:
    void clear(Node* node);
};

class IntArray {
private:
    int* data;
    int size;
    int capacity;
    void grow();

public:
    IntArray();
    ~IntArray();

    void pushBack(int value);
    bool insert(int index, int value);
    bool get(int index, int& value) const;
    bool set(int index, int value);
    bool remove(int index);
    int length() const;
    void print() const;
};

struct SNode {
    int value;
    SNode* next;
    SNode(int v) : value(v), next(nullptr) {}
};

class SinglyList {
private:
    SNode* head;
    SNode* tail;
    int size;

public:
    SinglyList();
    ~SinglyList();

    void pushFront(int value);
    void pushBack(int value);
    bool insertBefore(int target, int value);
    bool insertAfter(int target, int value);
    bool popFront();
    bool popBack();
    bool eraseBefore(int target);
    bool eraseAfter(int target);
    bool removeValue(int value);
    bool contains(int value) const;
    bool get(int index, int& value) const;
    int length() const;
    void print() const;
};

struct DNode {
    int value;
    DNode* prev;
    DNode* next;
    DNode(int v) : value(v), prev(nullptr), next(nullptr) {}
};

class DoublyList {
private:
    DNode* head;
    DNode* tail;
    int size;

public:
    DoublyList();
    ~DoublyList();

    void pushFront(int value);
    void pushBack(int value);
    bool insertBefore(int target, int value);
    bool insertAfter(int target, int value);
    bool popFront();
    bool popBack();
    bool eraseBefore(int target);
    bool eraseAfter(int target);
    bool removeValue(int value);
    bool contains(int value) const;
    bool get(int index, int& value) const;
    int length() const;
    void print() const;
    void printReverse() const;
};

struct StackNode {
    int value;
    StackNode* next;
    StackNode(int v) : value(v), next(nullptr) {}
};

class StackDS {
private:
    StackNode* top;

public:
    StackDS();
    ~StackDS();
    void push(int value);
    bool pop(int& value);
    bool empty() const;
    void print() const;
};

struct QueueNode {
    int value;
    QueueNode* next;
    QueueNode(int v) : value(v), next(nullptr) {}
};

class QueueDS {
private:
    QueueNode* head;
    QueueNode* tail;

public:
    QueueDS();
    ~QueueDS();
    void push(int value);
    bool pop(int& value);
    bool empty() const;
    void print() const;
};
