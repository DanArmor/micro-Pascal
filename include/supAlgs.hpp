#ifndef __INC_SUP_H
#define __INC_SUP_H

/**
 * @file supAlgs.hpp
 * @author DanArmor (https://github.com/DanArmor)
 * @brief Вспомогательные функции
 * @version 1.0
 */

#include <initializer_list>
/**
 * @brief Возвращает истину, если value находится среди значений cont
 * @tparam T тип значений
 * @param value значение для поиска
 * @param cont среди каких значений искать
 * @return true Если value имеется среди cont
 * @return false Если value отсутствует среди cont
 */
template <typename T>
bool isIn(T const &value, std::initializer_list<T> const &cont) {
    return std::find(cont.begin(), cont.end(), value) != cont.end();
}

#endif