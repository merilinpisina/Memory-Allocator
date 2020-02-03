#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

template <class T>
class DoublyLinkedList {
public:
    struct Node {
        T data;
        Node* prev;
        Node* next;
    };
    Node* head;
public:
    DoublyLinkedList();

    void insert(Node* prevNode, Node* newNode);
    void remove(Node* toDelete);
};

#include "DoublyLinkedListImpl.h"

#endif