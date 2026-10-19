#include "ds.h"
#include <iostream>
#include <queue>
#include <algorithm>
using namespace std;

// Complete Binary Tree
Tree::Tree() : root(nullptr) {}

Tree::~Tree() {
    clear(root);
}

void Tree::clear(Node* node) {
    if (node == nullptr) return;
    clear(node->left);
    clear(node->right);
    delete node;
}

void Tree::insert(int value) {
    Node* newNode = new Node(value);
    if (root == nullptr) {
        root = newNode;
        return;
    }

    queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        if (current->left == nullptr) {
            current->left = newNode;
            return;
        }
        q.push(current->left);

        if (current->right == nullptr) {
            current->right = newNode;
            return;
        }
        q.push(current->right);
    }
}

bool Tree::search(int value) {
    if (root == nullptr) return false;
    queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();
        if (current->value == value) return true;
        if (current->left != nullptr) q.push(current->left);
        if (current->right != nullptr) q.push(current->right);
    }
    return false;
}

bool Tree::isComplete() {
    if (root == nullptr) return true;

    queue<Node*> q;
    q.push(root);
    bool gap = false;

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        if (current->left != nullptr) {
            if (gap) return false;
            q.push(current->left);
        } else {
            gap = true;
        }

        if (current->right != nullptr) {
            if (gap) return false;
            q.push(current->right);
        } else {
            gap = true;
        }
    }
    return true;
}

void Tree::print() {
    if (root == nullptr) {
        cout << "Tree is empty\n";
        return;
    }

    queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        Node* current = q.front();
        q.pop();
        cout << current->value << " ";
        if (current->left != nullptr) q.push(current->left);
        if (current->right != nullptr) q.push(current->right);
    }
    cout << "\n";
}

// Dynamic array
IntArray::IntArray() : data(new int[4]), size(0), capacity(4) {}

IntArray::~IntArray() {
    delete[] data;
}

void IntArray::grow() {
    capacity *= 2;
    int* temp = new int[capacity];
    for (int i = 0; i < size; i++) temp[i] = data[i];
    delete[] data;
    data = temp;
}

void IntArray::pushBack(int value) {
    if (size == capacity) grow();
    data[size++] = value;
}

bool IntArray::insert(int index, int value) {
    if (index < 0 || index > size) return false;
    if (size == capacity) grow();
    for (int i = size; i > index; i--) data[i] = data[i - 1];
    data[index] = value;
    size++;
    return true;
}

bool IntArray::get(int index, int& value) const {
    if (index < 0 || index >= size) return false;
    value = data[index];
    return true;
}

bool IntArray::set(int index, int value) {
    if (index < 0 || index >= size) return false;
    data[index] = value;
    return true;
}

bool IntArray::remove(int index) {
    if (index < 0 || index >= size) return false;
    for (int i = index; i < size - 1; i++) data[i] = data[i + 1];
    size--;
    return true;
}

int IntArray::length() const {
    return size;
}

void IntArray::print() const {
    cout << "[";
    for (int i = 0; i < size; i++) {
        cout << data[i];
        if (i < size - 1) cout << ", ";
    }
    cout << "]\n";
}

// Singly linked list
SinglyList::SinglyList() : head(nullptr), tail(nullptr), size(0) {}

SinglyList::~SinglyList() {
    while (head != nullptr) {
        SNode* temp = head;
        head = head->next;
        delete temp;
    }
}

void SinglyList::pushFront(int value) {
    SNode* node = new SNode(value);
    node->next = head;
    head = node;
    if (tail == nullptr) tail = node;
    size++;
}

void SinglyList::pushBack(int value) {
    SNode* node = new SNode(value);
    if (tail == nullptr) head = tail = node;
    else {
        tail->next = node;
        tail = node;
    }
    size++;
}

bool SinglyList::insertBefore(int target, int value) {
    if (head == nullptr) return false;
    if (head->value == target) {
        pushFront(value);
        return true;
    }
    SNode* prev = head;
    SNode* cur = head->next;
    while (cur != nullptr && cur->value != target) {
        prev = cur;
        cur = cur->next;
    }
    if (cur == nullptr) return false;
    SNode* node = new SNode(value);
    prev->next = node;
    node->next = cur;
    size++;
    return true;
}

bool SinglyList::insertAfter(int target, int value) {
    SNode* cur = head;
    while (cur != nullptr && cur->value != target) cur = cur->next;
    if (cur == nullptr) return false;
    SNode* node = new SNode(value);
    node->next = cur->next;
    cur->next = node;
    if (tail == cur) tail = node;
    size++;
    return true;
}

bool SinglyList::popFront() {
    if (head == nullptr) return false;
    SNode* temp = head;
    head = head->next;
    delete temp;
    size--;
    if (head == nullptr) tail = nullptr;
    return true;
}

bool SinglyList::popBack() {
    if (tail == nullptr) return false;
    if (head == tail) return popFront();
    SNode* cur = head;
    while (cur->next != tail) cur = cur->next;
    delete tail;
    tail = cur;
    tail->next = nullptr;
    size--;
    return true;
}

bool SinglyList::eraseBefore(int target) {
    if (head == nullptr || head->value == target) return false;
    SNode* prevPrev = nullptr;
    SNode* prev = head;
    SNode* cur = head->next;
    while (cur != nullptr && cur->value != target) {
        prevPrev = prev;
        prev = cur;
        cur = cur->next;
    }
    if (cur == nullptr) return false;
    if (prevPrev == nullptr) return popFront();
    prevPrev->next = cur;
    delete prev;
    size--;
    return true;
}

bool SinglyList::eraseAfter(int target) {
    SNode* cur = head;
    while (cur != nullptr && cur->value != target) cur = cur->next;
    if (cur == nullptr || cur->next == nullptr) return false;
    SNode* temp = cur->next;
    cur->next = temp->next;
    if (tail == temp) tail = cur;
    delete temp;
    size--;
    return true;
}

bool SinglyList::removeValue(int value) {
    if (head == nullptr) return false;
    if (head->value == value) return popFront();
    SNode* prev = head;
    SNode* cur = head->next;
    while (cur != nullptr && cur->value != value) {
        prev = cur;
        cur = cur->next;
    }
    if (cur == nullptr) return false;
    prev->next = cur->next;
    if (tail == cur) tail = prev;
    delete cur;
    size--;
    return true;
}

bool SinglyList::contains(int value) const {
    SNode* cur = head;
    while (cur != nullptr) {
        if (cur->value == value) return true;
        cur = cur->next;
    }
    return false;
}

bool SinglyList::get(int index, int& value) const {
    if (index < 0 || index >= size) return false;
    SNode* cur = head;
    for (int i = 0; i < index; i++) cur = cur->next;
    value = cur->value;
    return true;
}

int SinglyList::length() const { return size; }

void SinglyList::print() const {
    cout << "[";
    SNode* cur = head;
    while (cur != nullptr) {
        cout << cur->value;
        if (cur->next != nullptr) cout << ", ";
        cur = cur->next;
    }
    cout << "]\n";
}

// Doubly linked list
DoublyList::DoublyList() : head(nullptr), tail(nullptr), size(0) {}

DoublyList::~DoublyList() {
    while (head != nullptr) {
        DNode* temp = head;
        head = head->next;
        delete temp;
    }
}

void DoublyList::pushFront(int value) {
    DNode* node = new DNode(value);
    if (head == nullptr) head = tail = node;
    else {
        node->next = head;
        head->prev = node;
        head = node;
    }
    size++;
}

void DoublyList::pushBack(int value) {
    DNode* node = new DNode(value);
    if (tail == nullptr) head = tail = node;
    else {
        node->prev = tail;
        tail->next = node;
        tail = node;
    }
    size++;
}

bool DoublyList::insertBefore(int target, int value) {
    DNode* cur = head;
    while (cur != nullptr && cur->value != target) cur = cur->next;
    if (cur == nullptr) return false;
    if (cur == head) {
        pushFront(value);
        return true;
    }
    DNode* node = new DNode(value);
    node->prev = cur->prev;
    node->next = cur;
    cur->prev->next = node;
    cur->prev = node;
    size++;
    return true;
}

bool DoublyList::insertAfter(int target, int value) {
    DNode* cur = head;
    while (cur != nullptr && cur->value != target) cur = cur->next;
    if (cur == nullptr) return false;
    if (cur == tail) {
        pushBack(value);
        return true;
    }
    DNode* node = new DNode(value);
    node->next = cur->next;
    node->prev = cur;
    cur->next->prev = node;
    cur->next = node;
    size++;
    return true;
}

bool DoublyList::popFront() {
    if (head == nullptr) return false;
    DNode* temp = head;
    head = head->next;
    if (head != nullptr) head->prev = nullptr;
    else tail = nullptr;
    delete temp;
    size--;
    return true;
}

bool DoublyList::popBack() {
    if (tail == nullptr) return false;
    DNode* temp = tail;
    tail = tail->prev;
    if (tail != nullptr) tail->next = nullptr;
    else head = nullptr;
    delete temp;
    size--;
    return true;
}

bool DoublyList::eraseBefore(int target) {
    DNode* cur = head;
    while (cur != nullptr && cur->value != target) cur = cur->next;
    if (cur == nullptr || cur == head) return false;
    DNode* victim = cur->prev;
    if (victim == head) return popFront();
    victim->prev->next = cur;
    cur->prev = victim->prev;
    delete victim;
    size--;
    return true;
}

bool DoublyList::eraseAfter(int target) {
    DNode* cur = head;
    while (cur != nullptr && cur->value != target) cur = cur->next;
    if (cur == nullptr || cur == tail) return false;
    DNode* victim = cur->next;
    if (victim == tail) return popBack();
    cur->next = victim->next;
    victim->next->prev = cur;
    delete victim;
    size--;
    return true;
}

bool DoublyList::removeValue(int value) {
    DNode* cur = head;
    while (cur != nullptr && cur->value != value) cur = cur->next;
    if (cur == nullptr) return false;
    if (cur == head) return popFront();
    if (cur == tail) return popBack();
    cur->prev->next = cur->next;
    cur->next->prev = cur->prev;
    delete cur;
    size--;
    return true;
}

bool DoublyList::contains(int value) const {
    DNode* cur = head;
    while (cur != nullptr) {
        if (cur->value == value) return true;
        cur = cur->next;
    }
    return false;
}

bool DoublyList::get(int index, int& value) const {
    if (index < 0 || index >= size) return false;
    DNode* cur;
    if (index < size / 2) {
        cur = head;
        for (int i = 0; i < index; i++) cur = cur->next;
    } else {
        cur = tail;
        for (int i = size - 1; i > index; i--) cur = cur->prev;
    }
    value = cur->value;
    return true;
}

int DoublyList::length() const { return size; }

void DoublyList::print() const {
    cout << "[";
    DNode* cur = head;
    while (cur != nullptr) {
        cout << cur->value;
        if (cur->next != nullptr) cout << ", ";
        cur = cur->next;
    }
    cout << "]\n";
}

void DoublyList::printReverse() const {
    cout << "[";
    DNode* cur = tail;
    while (cur != nullptr) {
        cout << cur->value;
        if (cur->prev != nullptr) cout << ", ";
        cur = cur->prev;
    }
    cout << "]\n";
}

// Stack
StackDS::StackDS() : top(nullptr) {}

StackDS::~StackDS() {
    while (top != nullptr) {
        StackNode* temp = top;
        top = top->next;
        delete temp;
    }
}

void StackDS::push(int value) {
    StackNode* node = new StackNode(value);
    node->next = top;
    top = node;
}

bool StackDS::pop(int& value) {
    if (top == nullptr) return false;
    StackNode* temp = top;
    value = temp->value;
    top = top->next;
    delete temp;
    return true;
}

bool StackDS::empty() const { return top == nullptr; }

void StackDS::print() const {
    cout << "[";
    StackNode* cur = top;
    while (cur != nullptr) {
        cout << cur->value;
        if (cur->next != nullptr) cout << ", ";
        cur = cur->next;
    }
    cout << "]\n";
}

// Queue
QueueDS::QueueDS() : head(nullptr), tail(nullptr) {}

QueueDS::~QueueDS() {
    while (head != nullptr) {
        QueueNode* temp = head;
        head = head->next;
        delete temp;
    }
    tail = nullptr;
}

void QueueDS::push(int value) {
    QueueNode* node = new QueueNode(value);
    if (tail == nullptr) head = tail = node;
    else {
        tail->next = node;
        tail = node;
    }
}

bool QueueDS::pop(int& value) {
    if (head == nullptr) return false;
    QueueNode* temp = head;
    value = temp->value;
    head = head->next;
    if (head == nullptr) tail = nullptr;
    delete temp;
    return true;
}

bool QueueDS::empty() const { return head == nullptr; }

void QueueDS::print() const {
    cout << "[";
    QueueNode* cur = head;
    while (cur != nullptr) {
        cout << cur->value;
        if (cur->next != nullptr) cout << ", ";
        cur = cur->next;
    }
    cout << "]\n";
}
