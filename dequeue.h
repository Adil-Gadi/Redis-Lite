//
// Created by Adil Gadi on 3/20/25.
//

#ifndef DEQUE_H
#define DEQUE_H

template<typename T>
struct DequeueItem {
    DequeueItem<T>* before = nullptr;
    DequeueItem<T>* after = nullptr;
    T data;
};

template<typename T>
class Dequeue {
public:
    void push(T data);
    void pop();
    void shift(T data);
    void unshift();

    //TODO: implement deconstructor
    ~Dequeue();

    DequeueItem<T>* getHeadItem() const;
    DequeueItem<T>* getTail() const;
private:
    DequeueItem<T>* head;
    DequeueItem<T>* tail;

    void freeDequeueItem(DequeueItem<T>* item);
};


#include "dequeue.tpp"

#endif //DEQUE_H
