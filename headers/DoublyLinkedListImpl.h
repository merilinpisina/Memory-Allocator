#include "DoublyLinkedList.h"

template <class T>
DoublyLinkedList<T>::DoublyLinkedList() {}

template <class T>
void DoublyLinkedList<T>::insert(Node* prevNode, Node* newNode) {
    if (prevNode == nullptr) {
        if (head != nullptr) {
            newNode->next = head;           
            newNode->next->prev = newNode;
        }
		
		else {
            newNode->next = nullptr;
        }
        
		head = newNode;
        head->prev = nullptr;
    } 
	
	else {
        if (prevNode->next == nullptr){
            prevNode->next = newNode;
            newNode->next = nullptr;
        }
		
		else {
            newNode->next = prevNode->next;
            if (newNode->next != nullptr){
                newNode->next->prev = newNode;
            }

            prevNode->next = newNode;
            newNode->prev = prevNode;
        }
    }
}

template <class T>
void DoublyLinkedList<T>::remove(Node* toDelete) {
    if (toDelete->prev == nullptr){
        if (toDelete->next == nullptr){
            head = nullptr;            
        }
		
		else {
            head = toDelete->next;
            head->prev = nullptr;
        }
    }
	
	else {
        if (toDelete->next == nullptr){
            toDelete->prev->next = nullptr;
        }
		
		else {
            toDelete->prev->next = toDelete->next;
            toDelete->next->prev = toDelete->prev;
        }
    }
}