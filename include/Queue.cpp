#include "List.cpp"

/**
 * @brief Класс очереди
 * 
 * @tparam T тип данных очереди 
 */
template<typename T>
class Queue : private List<T>{
    public:
    /// @brief Конструктор по-умолчанию
    Queue(){};

    /**
     * @brief Вставляет значение в хвост очереди
     * @param[in] value значение для вставки
     */
    void push(T const &value){
        List<T>::pushBack(value);
    }

    /// @brief Удаляет значение из головы очереди
    void pop(void){
        List<T>::popFront();
    }

    /// @brief Возвращает ссылку на значение в главе очереди
    T& pick(void){
        return List<T>::operator[](0);
    }

    /// @brief Возвращает размер очереди
    std::size_t size(void){
        return List<T>::size();
    }
};