#ifndef _INC_LIST_H
#define _INC_LIST_H

/**
 * @file List.hpp
 * @author DanArmor (https://github.com/DanArmor)
 * @brief Двусвязный список - интерфейс
 * @version 1.0
 */

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <vector>

// Предварительное объявление для списка
template <typename T>
class List;

// Предварительное объявление для итератора узла
template <typename T>
class NodeIterator;

/**
 * @brief Узел с одним указателм на данные и с указателями на предыдущий и следующий узлы
 * @tparam U - тип хранящихся значений
 */
template <typename U>
class Node {
    /// Список, способный получить доступ к защищенным полям узла
    friend class List<U>;
    friend class NodeIterator<U>;

   public:
    /// @brief Конструктор по-умолчанию
    Node();

    /// @brief Конструктор из значения
    explicit Node(U const value);

    /**
     * @brief Возвращает ссылку на данные узла
     * @return U - данные узла
     */
    U &data(void) noexcept;

    /// @brief Присоединяет узел pNext в качестве следующего узла
    void connectAsNext(Node *pNext) noexcept;

    /// @brief Присоединяет узел pPrev в качестве предыдущего узла
    void connectAsPrev(Node *pPrev) noexcept;

    /// @brief Создает узел из value и присоединяет в качестве следующего узла к
    /// вызывающему
    void connectAsNext(U const &value) noexcept;

    /// @brief Создает узел из value и присоединяет в качестве предыдущего узла
    /// к вызывающему
    void connectAsPrev(U const &value) noexcept;

    /**
     * @brief Исключает узел из цепи, в которой находится.
     * Пример, y - узел. к которому вызван метод:
     * x <-> y <-> z переходит в x <-> z
     */
    void exclude(void) noexcept;

    /**
     * @brief Исключает узел из цепи, в которой находится.
     * Пример, y - узел. к которому вызван метод:
     * x <-> y <-> z переходит в x <-> z.
     * Возвращает узел y.
     */
    Node *excludeAndReturn(void) noexcept;

   protected:
    /// @brief Данные
    U innerData;
    /// @brief Указатель на следующий узел
    Node *nextPtr;
    /// @brief Указатель на предыдущий узел
    Node *prevPtr;
};

/// @brief Итератор для СД, построенных на базе узлов (Node)
template <typename T>
struct NodeIterator {
   public:
    /// @brief аллаисы, требуемые для итераторов
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using iterator_category = std::bidirectional_iterator_tag;

    /// @brief Конструктор из указателя
    explicit NodeIterator(Node<T> *inPtr);

    /// @brief Конструктор для двунаправленного узла
    NodeIterator(Node<T> *inPtr, bool isEnd);

    /// @brief Перемещение вперед на одну позицию
    NodeIterator &operator++(void);

    /// @brief Перемещение назад на одну позицию
    NodeIterator &operator--(void);

    /// @brief Получение данных итератора
    T &operator*(void);

    /// @brief Не указывают ли итераторы на один и тот же узел
    friend bool operator!=(NodeIterator const &lhs, NodeIterator const &rhs) {
        return lhs.ptr != rhs.ptr;
    }

    /// @brief Указывают ли итераторы на один и тот же узел
    friend bool operator==(NodeIterator const &lhs, NodeIterator const &rhs) {
        return lhs.ptr == rhs.ptr;
    }

    ///@brief Дефолтные операторы и т п
    NodeIterator(NodeIterator &&) = default;
    NodeIterator(NodeIterator const &) = default;
    NodeIterator &operator=(NodeIterator &&) = default;
    NodeIterator &operator=(NodeIterator const &) = default;
    NodeIterator() = default;

   private:
    Node<T> *ptr;
    Node<T> *innerEnd;
};

/**
 * @brief Двусвязный список данных одного типа
 * @tparam T - тип хранящихся значений
 */
template <typename T>
class List {
   public:
    /// @brief Конструктор по-умолчанию
    List();

    /// @brief Конструктор из initializer_list
    List(std::initializer_list<T> const &initData);

    /// @brief Конструктор из initializer_list
    explicit List(std::vector<T> const &initData);

    /// @brief Конструктор из размера
    explicit List(std::size_t n);

    /// @brief Конструктор из другого списка
    List(List<T> const &list);

    ///@brief Деструктор
    ~List();

    /// @return std::size_t - размер списка
    std::size_t size(void) noexcept;

    /**
     * @brief Вставка в конец списка
     * @param[in] value значение для вставки
     */
    virtual void pushBack(T const &value) {
        sizeOfList++;
        goToEnd();
        if (getWorkPtr() == nullptr) {
            getWorkPtr() = new Node<T>(value);
            startPtr = getWorkPtr();
            endPtr = getWorkPtr();
        } else [[likely]] {
            getWorkPtr()->connectAsNext(value);
            endPtr = getWorkPtr()->nextPtr;
            workPtr = getWorkPtr()->nextPtr;
        }
    }

    /**
     * @brief Вставка в начало списка
     * @param[in] value - значение для вставки
     */
    virtual void pushFront(T const &value) {
        sizeOfList++;
        goToBegin();
        if (getWorkPtr() == nullptr) {
            getWorkPtr() = new Node<T>(value);
            startPtr = getWorkPtr();
            endPtr = getWorkPtr();
        } else [[likely]] {
            getWorkPtr()->connectAsPrev(value);
            startPtr = getWorkPtr()->prevPtr;
            workPtr = getWorkPtr()->prevPtr;
        }
    }

    /**
     * @param[in] index - индекс элемента в списке
     * @return T& - ссылку на элемент с номером index.
     */
    T &operator[](std::size_t const index);

    /**
     * @brief Безопасная функция для удаления, т.к.
     * учитывается изменение указателя на начало и конец списка
     * @param i - позиция для удаления
     */
    void removeAt(std::size_t const i);

    /// @brief Удаление из конца списка
    void popBack(void);

    /// @brief Удаление из начала списка
    void popFront(void);

    /// @brief очищает память, выделенную под узлы списка
    void clear(void);

    /// @brief Итератор для начала списка
    NodeIterator<T> begin(void) const;

    /// @brief Конец списка
    NodeIterator<T> end() const;

    List<T> &operator=(List<T> const &list);

   private:
    /**
     * @brief внутренняя функция удаления. Небезопасна, т.к.
     * не меняет указатель на начало и конец списка после удаления
     * @param i - позиция для удаления
     */
    void removeAt_inner(std::size_t i);

    /// @brief Возвращает указатель по рабочему указателю
    Node<T> *&getWorkPtr(void) noexcept;

    /// @brief Перемещает рабочий указатель в начало списка
    void goToBegin(void) noexcept;

    /// @brief Перемещает рабочий указатель в конец списка
    void goToEnd(void) noexcept;

    /// @brief перемещает рабочий указатель вперед на один шаг
    void moveWorkPtrNext(void);

    /// @brief перемещает рабочий указатель назад на один шаг
    void moveWorkPtrBack(void);

    /// @brief перемещает рабочий указатель вперед до указанного индекса
    void moveFowardUntil(std::size_t const index);

    /// @brief перемещает рабочий указатель назад до указанного индекса
    void moveBackUntil(std::size_t const index);

    /// @param[in] index - индекс необходимого элемента, на которой переместится
    /// рабочий указатель
    void goToIndex(std::size_t const index);

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

#include "List.tpp"

#endif
