//
// Created by Adil Gadi on 3/20/25.
//
#define LOG(x) std::cout << x << std::endl

template<typename T>
void Dequeue<T>::push(T data) {
    DequeueItem<T>* prev = nullptr;

    if (this->head != nullptr) {
        prev = this->head;
    }

    this->head = new DequeueItem<T>();
    this->head->after = nullptr;
    this->head->data = data;

    if (this->tail == nullptr) {
        this->tail = this->head;
    } else if (prev != nullptr) {
        this->head->after = prev;
        prev->before = this->head;
    }

}

template<typename T>
void Dequeue<T>::pop() {
    DequeueItem<T>* head = this->head;

    if (head == nullptr) return;

    if (this->head == this->tail) {
        this->tail = nullptr;
    }

    DequeueItem<T>* after = head->after;

    if (after != nullptr) {
        this->head = after;
    } else {
        this->head = nullptr;
    }

    delete head;
}

template<typename T>
void Dequeue<T>::shift(T data) {
    DequeueItem<T>* prev = this->tail;

    this->tail = new DequeueItem<T>();
    this->tail->after = nullptr;
    this->tail->before = nullptr;
    this->tail->data = data;

    if (this->tail == nullptr) {
        this->tail = this->head;
    } else if (prev != nullptr) {
        prev->after = this->tail;
        this->tail->before = prev;
    }

}

template<typename T>
void Dequeue<T>::unshift() {
    DequeueItem<T>* tail = this->tail;

    if (tail == nullptr) return;

    if (this->head == this->tail) {
        this->head = nullptr;
    }

    DequeueItem<T>* before = tail->before;

    if (before != nullptr) {
        this->tail = before;
    } else {
        this->tail = nullptr;
    }

    delete tail;
}

template<typename T>
Dequeue<T>::~Dequeue() {
    this->freeDequeueItem(this->head);
}

template<typename T>
DequeueItem<T> * Dequeue<T>::getHeadItem() const {
    return this->head;
}

template<typename T>
DequeueItem<T> * Dequeue<T>::getTail() const {
    return this->tail;
}

template<typename T>
void Dequeue<T>::freeDequeueItem(DequeueItem<T>* item) {
    if (item == nullptr) return;

    freeDequeueItem(item->after);

    delete item;
}
