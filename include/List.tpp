#ifndef _INC_LIST_TPP
#define _INC_LIST_TPP

#ifndef _INC_LIST_H
#error "List.tpp was included before the List.hpp!"
#endif

/**
 * @file List.tpp
 * @author DanArmor (https://github.com/DanArmor)
 * @brief Двусвязный список - реализация
 * @version 1.0
 */

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <vector>

template <typename T>
class List;

template <typename T>
class NodeIterator;

template <typename U>
Node<U>::Node() : innerData(nullptr), nextPtr(nullptr), prevPtr(nullptr){};

template <typename U>
Node<U>::Node(U const value)
    : nextPtr(nullptr), prevPtr(nullptr), innerData(value){};

template <typename U>
U &Node<U>::data(void) noexcept {
    return innerData;
}

template <typename U>
void Node<U>::connectAsNext(Node *pNext) noexcept {
    nextPtr = pNext;
    pNext->prevPtr = this;
}

template <typename U>
void Node<U>::connectAsPrev(Node *pPrev) noexcept {
    prevPtr = pPrev;
    pPrev->nextPtr = this;
}

template <typename U>
void Node<U>::connectAsNext(U const &value) noexcept {
    connectAsNext(new Node<U>(value));
}

template <typename U>
void Node<U>::connectAsPrev(U const &value) noexcept {
    connectAsPrev(new Node<U>(value));
}

template <typename U>
void Node<U>::exclude(void) noexcept {
    if (prevPtr != nullptr) prevPtr->nextPtr = nextPtr;
    if (nextPtr != nullptr) nextPtr->prevPtr = prevPtr;
}

template <typename U>
Node<U> *Node<U>::excludeAndReturn(void) noexcept {
    exclude();
    return this;
}

template <typename T>
NodeIterator<T>::NodeIterator(Node<T> *inPtr) : ptr(inPtr){};

template <typename T>
NodeIterator<T>::NodeIterator(Node<T> *inPtr, bool isEnd) : ptr(inPtr) {
    if (isEnd && inPtr != nullptr) {
        ptr = inPtr->nextPtr;
        innerEnd = inPtr;
    } else {
        ptr = inPtr;
    }
};

template <typename T>
NodeIterator<T> &NodeIterator<T>::operator++(void) {
    ptr = ptr->nextPtr;
    return *this;
}

template <typename T>
NodeIterator<T> &NodeIterator<T>::operator--(void) {
    if (ptr == nullptr)
        ptr = innerEnd;
    else [[likely]]
        ptr = ptr->prevPtr;
    return *this;
}

template <typename T>
T &NodeIterator<T>::operator*(void) {
    return ptr->data();
}

template <typename T>
List<T>::List()
    : sizeOfList(0),
      startPtr(nullptr),
      endPtr(nullptr),
      workPtr(nullptr),
      currIndex(0){};

template <typename T>
List<T>::List(std::initializer_list<T> const &initData)
    : sizeOfList(0),
      startPtr(nullptr),
      endPtr(nullptr),
      workPtr(nullptr),
      currIndex(0) {
    for (auto value : initData) pushBack(value);
    sizeOfList = initData.size();
}

template <typename T>
List<T>::List(std::vector<T> const &initData)
    : sizeOfList(0),
      startPtr(nullptr),
      endPtr(nullptr),
      workPtr(nullptr),
      currIndex(0) {
    for (auto value : initData) pushBack(value);
    sizeOfList = initData.size();
}

template <typename T>
List<T>::List(std::size_t n)
    : sizeOfList(0),
      startPtr(nullptr),
      endPtr(nullptr),
      workPtr(nullptr),
      currIndex(0) {
    for (std::size_t i = 0; i < n; i++) pushBack(T{});
    sizeOfList = n;
}

template <typename T>
List<T>::List(List<T> const &list)
    : sizeOfList(0),
      startPtr(nullptr),
      endPtr(nullptr),
      workPtr(nullptr),
      currIndex(0) {
    clear();
    for (auto x : list) pushBack(x);
}

template <typename T>
List<T>::~List() {
    clear();
}

template <typename T>
std::size_t List<T>::size(void) noexcept {
    return sizeOfList;
}

template <typename T>
T &List<T>::operator[](std::size_t const index) {
    if (index > sizeOfList)
        throw std::out_of_range("Выход за пределы списка!");
    else if (sizeOfList == 0)
        throw std::out_of_range(
            "Попытка получения значения из пустого списка!");
    goToIndex(index);
    return getWorkPtr()->data();
}

template <typename T>
void List<T>::removeAt(std::size_t const i) {
    if (sizeOfList == 0)
        throw(
            std::out_of_range("Попытка удаления элемента из пустого списка!"));
    else if (i >= sizeOfList)
        throw(std::out_of_range(
            "Индекс элемента для удаления лежит за пределами списка!"));
    if (i == size() - 1)
        popBack();
    else if (i == 0)
        popFront();
    else
        removeAt_inner(i);
}

template <typename T>
void List<T>::popBack(void) {
    if (sizeOfList == 0)
        throw(
            std::out_of_range("Попытка удаления элемента из пустого списка!"));
    Node<T> *reserveEnd = endPtr->prevPtr;
    removeAt_inner(size() - 1);
    endPtr = reserveEnd;
}

template <typename T>
void List<T>::popFront(void) {
    if (sizeOfList == 0)
        throw(
            std::out_of_range("Попытка удаления элемента из пустого списка!"));
    Node<T> *reserveStart = startPtr->nextPtr;
    removeAt_inner(0);
    startPtr = reserveStart;
}

template <typename T>
void List<T>::clear(void) {
    std::size_t toClear = sizeOfList;
    for (std::size_t i = 0; i < toClear; i++) popBack();
}

template <typename T>
NodeIterator<T> List<T>::begin(void) const {
    return NodeIterator<T>(startPtr, false);
}

template <typename T>
NodeIterator<T> List<T>::end() const {
    return NodeIterator<T>(endPtr, true);
}

template <typename T>
List<T> &List<T>::operator=(List<T> const &list) {
    if (this == &list) return *this;

    clear();
    for (auto x : list) pushBack(x);
    sizeOfList = list.sizeOfList;

    return *this;
}

template <typename T>
void List<T>::removeAt_inner(std::size_t i) {
    goToIndex(i);
    Node<T> *ptr = getWorkPtr()->excludeAndReturn();
    /// Если есть предыдущий узел - зацепимся за него
    if (ptr->prevPtr != nullptr) {
        workPtr = ptr->prevPtr;
        currIndex = i - 1;
    } else if (ptr->nextPtr != nullptr) {
        workPtr = ptr->nextPtr;
        currIndex = i;
    } else {
        goToBegin();
        currIndex = 0;
    }
    delete ptr;
    /// самое важное в ветках выше - не забыть обновить текущий индекс
    sizeOfList--;
}

template <typename T>
Node<T> *&List<T>::getWorkPtr(void) noexcept {
    return workPtr;
}

template <typename T>
void List<T>::goToBegin(void) noexcept {
    workPtr = startPtr;
    currIndex = 0;
}

template <typename T>
void List<T>::goToEnd(void) noexcept {
    workPtr = endPtr;
    currIndex = sizeOfList ? sizeOfList - 1 : 0;
}

template <typename T>
void List<T>::moveWorkPtrNext(void) {
    if (getWorkPtr() == nullptr)
        throw(
            std::out_of_range("Попытка перемещения по рабочему указателю, "
                              "когда он равен nullptr"));
    workPtr = getWorkPtr()->nextPtr;
    currIndex++;
}

template <typename T>
void List<T>::moveWorkPtrBack(void) {
    if (getWorkPtr() == nullptr)
        throw(
            std::out_of_range("Попытка перемещения по рабочему указателю, "
                              "когда он равен nullptr"));
    workPtr = getWorkPtr()->prevPtr;
    currIndex--;
}

template <typename T>
void List<T>::moveFowardUntil(std::size_t const index) {
    while (currIndex < index) moveWorkPtrNext();
}

template <typename T>
void List<T>::moveBackUntil(std::size_t const index) {
    while (currIndex > index) moveWorkPtrBack();
}

template <typename T>
void List<T>::goToIndex(std::size_t const index) {
    /// 0 - start, 1 - end, 2 - current
    enum positionToStart : std::size_t { start, end, current };
    std::size_t searchFrom = size() - 1 - index < index
                                 ? end
                                 : start;  /// Выбор между стартом и концом
    std::size_t dist = searchFrom ? size() - 1 - index : index;

    if (currIndex < index)
        if (dist >= index - currIndex)
            searchFrom = current;
        else if (dist >= currIndex - index)
            searchFrom = current;

    switch (searchFrom) {
        case start:
            goToBegin();
            moveFowardUntil(index);
            break;
        case end:
            goToEnd();
            moveBackUntil(index);
            break;
        case current:
            if (currIndex < index)
                moveFowardUntil(index);
            else
                moveBackUntil(index);
            break;
    }
}

#endif
