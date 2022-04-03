#ifndef _INC_LIST_H
#define _INC_LIST_H

/**
 * @file List.cpp
 * @author DanArmor (https://github.com/DanArmor)
 * @brief Двусвязный список
 * @version 0.5
 * @date 2022-03-17
 */

#include <cstddef>
#include <vector>
#include <stdexcept>
#include <iterator>

/// Предварительное объявление для списка
template<typename T>
class List;

/// Предварительное объявление для итератора узла
template<typename T>
class NodeIterator;

/**
 * @brief Узел с одним указателм на данные и одним указателем на следующий узел
 * @tparam U - тип хранящихся значений
 */
template<typename U>
class Node{

    /// Список, способный получить доступ к защищенным полям узла
    friend class List<U>;
    friend class NodeIterator<U>;

    public:

    /// @brief Конструктор по-умолчанию
    Node() : innerData(nullptr), nextPtr(nullptr), prevPtr(nullptr){};

    /// @brief Конструктор из значения
    Node(U const value) : nextPtr(nullptr), prevPtr(nullptr){
        innerData = value;
    }

    /**
     * @brief Возвращает ссылку на данные узла
     * @return U - данные узла
     */
    U& data(void) noexcept{
        return innerData; 
    }

    /// @brief Присоединяет узел pNext в качестве следующего узла
    void connectAsNext(Node *pNext) noexcept{
        nextPtr = pNext;
        pNext->prevPtr = this;
    }

    /// @brief Присоединяет узел pPrev в качестве предыдущего узла
    void connectAsPrev(Node *pPrev) noexcept{
        prevPtr = pPrev;
        pPrev->nextPtr = this;
    }

    /// @brief Создает узел из value и присоединяет в качестве следующего узла к вызывающему
    void connectAsNext(U const &value) noexcept{
        connectAsNext(new Node<U>(value));
    }

    /// @brief Создает узел из value и присоединяет в качестве предыдущего узла к вызывающему
    void connectAsPrev(U const &value) noexcept{
        connectAsPrev(new Node<U>(value));
    }

    /**
     * @brief Исключает узел из цепи, в которой находится.
     * Пример, y - узел. к которому вызван метод: 
     * x <-> y <-> z переходит в x <-> z
     */
    void exclude(void) noexcept{
        if(prevPtr != nullptr)
            prevPtr->nextPtr = nextPtr;
        if(nextPtr != nullptr)
            nextPtr->prevPtr = prevPtr;
    }

    /**
     * @brief Исключает узел из цепи, в которой находится.
     * Пример, y - узел. к которому вызван метод: 
     * x <-> y <-> z переходит в x <-> z.
     * Возвращает узел y.
     */
    Node *excludeAndReturn(void) noexcept{
        exclude();
        return this;
    }

    protected:
    /// @brief Данные
    U innerData;
    /// @brief Указатель на следующий узел
    Node *nextPtr;
    /// @brief Указатель на предыдущий узел
    Node *prevPtr;
};

/// @brief Итератор для СД, построенных на базе узлов (Node)
template<typename T>
struct NodeIterator{
    public:

    /// @brief аллаисы, требуемые для итераторов
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using iterator_category = std::bidirectional_iterator_tag;

    /// @brief Конструктор из указателя 
    NodeIterator(Node<T> *inPtr) : ptr(inPtr){};

    /// @brief Конструктор для двунаправленного узла
    NodeIterator(Node<T> *inPtr, bool isEnd) : ptr(inPtr){
        if(isEnd && inPtr != nullptr){
            ptr = inPtr->nextPtr;
            innerEnd = inPtr;
        } else{
            ptr = inPtr;
        }
    };

    /// @brief Перемещение вперед на одну позицию
    NodeIterator &operator++(void){
        ptr = ptr->nextPtr;
        return *this;
    }

    /// @brief Перемещение назад на одну позицию
    NodeIterator &operator--(void){
        if(ptr == nullptr)
            ptr = innerEnd;
        else [[likely]]
            ptr = ptr->prevPtr;
        return *this;
    }

    /// @brief Получение данных итератора
    T &operator*(void){
        return ptr->data();
    }

    /// @brief Не указывают ли итераторы на один и тот же узел
    friend bool operator!=(NodeIterator const &lhs, NodeIterator const &rhs){
        return lhs.ptr != rhs.ptr;
    }

    /// @brief Указывают ли итераторы на один и тот же узел
    friend bool operator==(NodeIterator const &lhs, NodeIterator const &rhs){
        return lhs.ptr == rhs.ptr;
    }

    ///@brief Дефолтные операторы и т п
    NodeIterator(NodeIterator&&) = default;
    NodeIterator(NodeIterator const&) = default;
    NodeIterator& operator=(NodeIterator&&) = default;
    NodeIterator& operator=(NodeIterator const&) = default;
    NodeIterator() = default;
    
    private:
    Node<T> *ptr;
    Node<T> *innerEnd;
};

/**
 * @brief Двусвязный список данных одного типа
 * @tparam T - тип хранящихся значений
 */
template<typename T>
class List{
    public:

    /// @brief Конструктор по-умолчанию
    List() : sizeOfList(0), startPtr(nullptr), endPtr(nullptr), workPtr(nullptr), currIndex(0) {};

    /// @brief Конструктор из вектора
    List(std::vector<T> const &initData) : sizeOfList(0), startPtr(nullptr), endPtr(nullptr), workPtr(nullptr), currIndex(0) {
        for(auto value : initData)
            pushBack(value);
        sizeOfList = initData.size();
    }

    /// @brief Конструктор из размера
    List(std::size_t n) : sizeOfList(0), startPtr(nullptr), endPtr(nullptr), workPtr(nullptr), currIndex(0) {
        for(std::size_t i = 0; i < n; i++)
            pushBack(T{});
        sizeOfList = n;
    }
    
    /// @brief Конструктор из другого списка
    List(List<T> const &list): sizeOfList(0), startPtr(nullptr), endPtr(nullptr), workPtr(nullptr), currIndex(0){
        clear();
        for(auto x : list)
            pushBack(x);
    }

    ///@brief Деструктор
    ~List(){
        clear();
    }

    /// @return std::size_t - размер списка
    std::size_t size(void) noexcept{
        return sizeOfList;
    }

    /**
     * @brief Вставка в конец списка
     * @param[in] value значение для вставки
     */
    virtual void pushBack(T const &value){
        sizeOfList++;
        goToEnd();
        if(getWorkPtr() == nullptr){
            getWorkPtr() = new Node<T>(value);
            startPtr = getWorkPtr();
            endPtr = getWorkPtr();
        } else  [[likely]]{
            getWorkPtr()->connectAsNext(value);
            endPtr = getWorkPtr()->nextPtr;
            workPtr = getWorkPtr()->nextPtr;
        }
    }

    /**
     * @brief Вставка в начало списка 
     * @param[in] value - значение для вставки
     */
    virtual void pushFront(T const &value){
        sizeOfList++;
        goToBegin();
        if(getWorkPtr() == nullptr){
            getWorkPtr() = new Node<T>(value);
            startPtr = getWorkPtr();
            endPtr = getWorkPtr();
        } else [[likely]]{
            getWorkPtr()->connectAsPrev(value);
            startPtr = getWorkPtr()->prevPtr;
            workPtr = getWorkPtr()->prevPtr;
        }
    }

    /**
     * @param[in] index - индекс элемента в списке
     * @return T& - ссылку на элемент с номером index. 
     */
    T& operator[](std::size_t const index){
        if(index > sizeOfList)
            throw std::out_of_range("Выход за пределы списка!");
        else if(sizeOfList == 0)
            throw std::out_of_range("Попытка получения значения из пустого списка!");
        goToIndex(index);
        return getWorkPtr()->data();
    }

    /**
     * @brief Безопасная функция для удаления, т.к.
     * учитывается изменение указателя на начало и конец списка
     * @param i - позиция для удаления
     */
    void removeAt(std::size_t const i){
        if(sizeOfList == 0)
            throw(std::out_of_range("Попытка удаления элемента из пустого списка!"));
        else if(i >= sizeOfList)
            throw(std::out_of_range("Индекс элемента для удаления лежит за пределами списка!"));
        if(i == size()-1)
            popBack();
        else if(i == 0)
            popFront();
        else
            removeAt_inner(i);
    }

    /// @brief Удаление из конца списка
    void popBack(void){
        if(sizeOfList == 0)
            throw(std::out_of_range("Попытка удаления элемента из пустого списка!"));
        Node<T> *reserveEnd = endPtr->prevPtr;
        removeAt_inner(size()-1);
        endPtr = reserveEnd;
    }

    /// @brief Удаление из начала списка
    void popFront(void){
        if(sizeOfList == 0)
            throw(std::out_of_range("Попытка удаления элемента из пустого списка!"));
        Node<T> *reserveStart = startPtr->nextPtr;
        removeAt_inner(0);
        startPtr = reserveStart;
    }

    /// @brief очищает память, выделенную под узлы списка
    void clear(void) noexcept{
        std::size_t toClear = sizeOfList;
        for(std::size_t i = 0; i < toClear; i++)
            popBack();
    }

    /// @brief Итератор для начала списка
    NodeIterator<T> begin(void) const{
        return NodeIterator<T>(startPtr, false);
    }

    /// @brief Конец списка
    NodeIterator<T> end() const{
        return NodeIterator<T>(endPtr, true);
    }

    List<T> &operator=(List<T> const &list){
        if(this == &list)
            return *this;
        
        clear();
        for(auto x : list)
            pushBack(x);

        return *this;
    }


    private:

    /**
     * @brief внутренняя функция удаления. Небезопасна, т.к.
     * не меняет указатель на начало и конец списка после удаления 
     * @param i - позиция для удаления
     */
    void removeAt_inner(std::size_t i){
        goToIndex(i);
        Node<T> *ptr = getWorkPtr()->excludeAndReturn();
        /// Если есть предыдущий узел - зацепимся за него
        if(ptr->prevPtr != nullptr){
            workPtr = ptr->prevPtr;
            currIndex = i - 1;
        }else if(ptr->nextPtr != nullptr){
            workPtr = ptr->nextPtr;
            currIndex = i;
        }else{
            goToBegin();
            currIndex = 0;
        }
        delete ptr;
        /// самое важное в ветках выше - не забыть обновить текущий индекс
        sizeOfList--;
    }

    /// @brief Возвращает указатель по рабочему указателю
    Node<T> *&getWorkPtr(void) noexcept{
        return workPtr;
    }

    /// @brief Перемещает рабочий указатель в начало списка
    void goToBegin(void) noexcept{
        workPtr = startPtr;
        currIndex = 0;
    }

    /// @brief Перемещает рабочий указатель в конец списка
    void goToEnd(void) noexcept{
        workPtr = endPtr;
        currIndex = sizeOfList ? sizeOfList - 1 : 0;
    }

    /// @brief перемещает рабочий указатель вперед на один шаг
    void moveWorkPtrNext(void){
        if(getWorkPtr() == nullptr)
            throw(std::out_of_range("Попытка перемещения по рабочему указателю, когда он равен nullptr"));
        workPtr = getWorkPtr()->nextPtr;
        currIndex++;
    }

    /// @brief перемещает рабочий указатель назад на один шаг
    void moveWorkPtrBack(void){
        if(getWorkPtr() == nullptr)
            throw(std::out_of_range("Попытка перемещения по рабочему указателю, когда он равен nullptr"));
        workPtr = getWorkPtr()->prevPtr;
        currIndex--;
    }

    /// @brief перемещает рабочий указатель вперед до указанного индекса
    void moveFowardUntil(std::size_t const index){
        while(currIndex < index)
            moveWorkPtrNext();
    }

    /// @brief перемещает рабочий указатель назад до указанного индекса
    void moveBackUntil(std::size_t const index){
        while(currIndex > index)
            moveWorkPtrBack();
    }

    /// @param[in] index - индекс необходимого элемента, на которой переместится рабочий указатель
    void goToIndex(std::size_t const index){
        /// 0 - start, 1 - end, 2 - current
        enum positionToStart : std::size_t {start, end, current};
        std::size_t searchFrom = size() - 1 - index < index ? end : start; /// Выбор между стартом и концом
        std::size_t dist = searchFrom ? size() - 1 - index : index;

        if(currIndex < index)
            if(dist >= index - currIndex)
                searchFrom = current;
        else
            if(dist >= currIndex - index)
                searchFrom = current;

        switch(searchFrom){
            case start:     goToBegin(); moveFowardUntil(index); break;
            case end:       goToEnd(); moveBackUntil(index); break;
            case current:   if(currIndex < index)
                                moveFowardUntil(index);
                            else
                                moveBackUntil(index);
                            break;
        }
    }

    /// @brief Размер списка
    std::size_t sizeOfList;
    /// @brief Указатель на начало списка
    Node<T> *startPtr;
    /// @brief Указатель на конец списка
    Node<T> *endPtr;
    /// @brief Рабочий указатель
    Node<T> *workPtr;
    /// @brief Индекс текущего элемента
    std::size_t currIndex; 

};

#endif
