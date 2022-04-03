#include "List.cpp"

/**
 * @brief Класс стека
 * 
 * @tparam T тип данных стека
 */
template<typename T>
class Stack : private List<T>{
    public:
    /// @brief Конструктор по-умолчанию
    Stack(){};

    /**
     * @brief Кладет значение на вершину стека
     * @param[in] value значение для вставки
     */
    void push(T const &value){
        List<T>::pushFront(value);
    }

    /// @brief Удаляет значение с вершины стека
    void pop(void){
        List<T>::popFront();
    }

    /// @brief Возвращает ссылку на значение на вершине стека
    T& pick(void){
        return List<T>::operator[](0);
    }

    /// @brief Возвращает размер стека
    std::size_t size(void){
        return List<T>::size();
    }
};