/**
 * @file testList.cpp
 * @author DanArmor (https://github.com/DanArmor)
 * @brief Тесты для двусвязного списка
 * @version 1.0
 * @date 2022-03-17
 */

#include <vector>
#include <iostream>
#include <utility>
#include <algorithm>
#include <numeric>

#include <gtest/gtest.h>
#include "List.cpp"

/// @brief Тест стандартного конструктора
TEST(LIST_SIMPLE_TESTS, Create_default){
    List<int> list{};
    EXPECT_EQ(list.size(), 0);
}

/// @brief Тест конструктора из вектора
TEST(LIST_SIMPLE_TESTS, Create_vector){
    std::vector<int> testData = {1, 2, 3};
    List<int> list{testData};
    EXPECT_EQ(list.size(), testData.size());
}

/// @brief Тест конструктора из вектора значений
TEST(LIST_SIMPLE_TESTS, Create_vector_values){
    std::vector<int> testData = {1, 2, 3};
    List<int> list{testData};
    for(std::size_t i = 0; i < testData.size(); i++)
        EXPECT_EQ(list[i], testData[i]);
}

/// @brief Тест конструктора из размера
TEST(LIST_SIMPLE_TESTS, Create_size){
    std::vector<int> testData = {0, 0, 0};
    List<int> list{3};
    EXPECT_EQ(list.size(), testData.size());
}

/// @brief Тест конструктора из размера значений
TEST(LIST_SIMPLE_TESTS, Create_size_values){
    std::vector<int> testData = {0, 0, 0};
    List<int> list{3};
    for(std::size_t i = 0; i < testData.size(); i++)
        EXPECT_EQ(list[i], testData[i]);
}

/// @brief сравнение значений с вектором, из которого создан список
TEST(LIST_SIMPLE_TESTS, Walk_vector_foward){
    std::vector<int> testData = {1, 2, 3};
    List<int> list{testData};
    for(int i = 0; i < testData.size(); i++)
        EXPECT_EQ(list[i], testData[i]);
}

/// @brief сравнение значений с вектором, из которого создан список
TEST(LIST_SIMPLE_TESTS, Walk_vector_back){
    std::vector<int> testData = {1, 2, 3};
    List<int> list{testData};
    for(int i = testData.size()-1; i >= 0; i--)
        EXPECT_EQ(list[i], testData[i]);
}

/// @brief сравнение значений с вектором, из которого создан список
TEST(LIST_SIMPLE_TESTS, Walk_vector_foward_and_back){
    std::vector<int> testData = {1, 2, 3};
    List<int> list{testData};
    for(int i = testData.size()-1; i >= 0; i--)
        EXPECT_EQ(list[i], testData[i]);
    for(int i = 0; i < testData.size(); i++)
        EXPECT_EQ(list[i], testData[i]);
    for(int i = testData.size()-1; i >= 0; i--)
        EXPECT_EQ(list[i], testData[i]);
    for(int i = 0; i < testData.size(); i++)
        EXPECT_EQ(list[i], testData[i]);
}

/// @brief Тест размера после вставки в конец
TEST(LIST_SIMPLE_TESTS, PushBack_size){
    std::vector<int> testData = {1, 2, 3};
    List<int> list{};
    for(int i = 0; i < testData.size(); i++)
        list.pushBack(testData[i]);
    EXPECT_EQ(list.size(), testData.size());
}

/// @brief Тест размера после вставки в начало
TEST(LIST_SIMPLE_TESTS, PushBack_eq_elements){
    std::vector<int> testData = {1, 2, 3};
    List<int> list{};
    for(int i = 0; i < testData.size(); i++)
        list.pushBack(testData[i]);
    for(int i = 0; i < testData.size(); i++)
        EXPECT_EQ(list[i], testData[i]);
}

/// @brief Тест значений после вставки в конец
TEST(LIST_SIMPLE_TESTS, PushFront_size){
    std::vector<int> testData = {1, 2, 3};
    List<int> list{};
    for(int i = 0; i < testData.size(); i++)
        list.pushFront(testData[i]);
    EXPECT_EQ(list.size(), testData.size());
}

/// @brief Тест значений после вставки в начало
TEST(LIST_SIMPLE_TESTS, PushFront_eq_elements){
    std::vector<int> testData = {1, 2, 3};
    List<int> list{};
    for(int i = 0; i < testData.size(); i++)
        list.pushFront(testData[i]);
    // Мы вставляем выше 3 2 1, поэтому testData[size-i-1]
    for(int i = 0; i < testData.size(); i++)
        EXPECT_EQ(list[i], testData[testData.size()-i-1]);
}

/// @brief Тест размера после удаления из конца
TEST(LIST_SIMPLE_TESTS, PopBack_size){
    std::vector<int> testData = {1, 2, 3};
    List<int> list{testData};
    for(int i = 0; i < testData.size(); i++)
        list.popBack();
    EXPECT_EQ(list.size(), 0);
}

/// @brief Тест размера после удаления из начала
TEST(LIST_SIMPLE_TESTS, PopFront_size){
    std::vector<int> testData = {1, 2, 3};
    List<int> list{testData};
    for(int i = 0; i < testData.size(); i++)
        list.popFront();
    EXPECT_EQ(list.size(), 0);
}

/// @brief Тест значений после удаления из конца
TEST(LIST_SIMPLE_TESTS, PopBack_values){
    std::vector<int> testData = {1, 2, 3};
    List<int> list{testData};
    for(int i = 0; i < testData.size(); i++){
        EXPECT_EQ(list[list.size()-1], testData[testData.size()-i-1]);
        list.popBack();
    }
}

/// @brief Тест значений после удаления из начала
TEST(LIST_SIMPLE_TESTS, PopFront_values){
    std::vector<int> testData = {1, 2, 3};
    List<int> list{testData};
    for(int i = 0; i < testData.size(); i++){
        EXPECT_EQ(list[0], testData[i]);
        list.popFront();
    }
}

/// @brief Тест размера списка после удаления
TEST(LIST_SIMPLE_TESTS, removeAt_size_1){
    std::vector<int> testData = {1, 2, 3};
    List<int> list{testData};
    list.removeAt(0);
    list.removeAt(0);
    list.removeAt(0);
    EXPECT_EQ(list.size(), 0);
}

/// @brief Тест размера списка после удаления
TEST(LIST_SIMPLE_TESTS, removeAt_size_2){
    std::vector<int> testData = {1, 2, 3};
    List<int> list{testData};
    list.removeAt(1);
    list.removeAt(1);
    list.removeAt(0);
    EXPECT_EQ(list.size(), 0);
}

/// @brief Тест размера списка после удаления
TEST(LIST_SIMPLE_TESTS, removeAt_size_3){
    std::vector<int> testData = {1, 2, 3};
    List<int> list{testData};
    list.removeAt(0);
    list.removeAt(1);
    list.removeAt(0);
    EXPECT_EQ(list.size(), 0);
}

/// @brief Тест значений списка после удаления
TEST(LIST_SIMPLE_TESTS, removeAt_values){
    std::vector<int> testData = {1, 2, 3};
    List<int> list{testData};
    EXPECT_EQ(list[1], testData[1]);
    list.removeAt(1);
    EXPECT_EQ(list[1], testData[2]);
    list.removeAt(1);
    EXPECT_EQ(list[0], testData[0]);
}

TEST(LIST_ITERATORS, for_based_loop){
    std::vector<int> testData = {1, 2, 3};
    List<int> list{testData};
    int i = 0;
    for(auto x : list){
        EXPECT_EQ(x, testData[i]);
        i++;
    }
}

TEST(LIST_ITERATORS, for_iterator_based){
    std::vector<int> testData = {1, 2, 3};
    List<int> list{testData};
    int i = 0;
    for(auto it = list.begin(); it != list.end(); ++it){
        EXPECT_EQ(*it, testData[i]);
        i++;
    }
}

/// @brief Тесты LIST_AND_STL - на совместимость со стандартными алгоритмами
TEST(LIST_AND_STL, numeric_accumulate){
    std::vector<int> testData = {1, 2, 3};
    List<int> list{testData};
    int sumTestData = std::accumulate(testData.begin(), testData.end(), 0);
    int sumList = std::accumulate(list.begin(), list.end(), 0);
    EXPECT_EQ(sumTestData, sumList);
}

TEST(LIST_AND_STL, algorithm_transform){
    std::vector<int> testData = {1, 2, 3};
    List<int> list{testData};
    List<int> listR{testData};
    std::transform(list.begin(), list.end(), listR.begin(), [](auto x){return x+5;});
    std::vector<int> checkData = {6, 7, 8};
    int i = 0;
    for(auto x : listR){
        EXPECT_EQ(x, checkData[i]);
        i++;
    }
}

TEST(LIST_AND_STL, algorithm_generate){
    List<int> list{5};
    std::generate(list.begin(), list.end(), [n = 0] () mutable {return n++;});
    std::vector<int> checkData = {0, 1, 2, 3, 4};
    int i = 0;
    for(auto x : list){
        EXPECT_EQ(x, checkData[i]);
        i++;
    }
}

TEST(LIST_AND_STL, algorithm_reverse){
    List<int> list{5};
    std::generate(list.begin(), list.end(), [n = 0] () mutable {return n++;});
    std::vector<int> checkData = {4, 3, 2, 1, 0};
    std::reverse(list.begin(), list.end());
    int i = 0;
    for(auto x : list){
        EXPECT_EQ(x, checkData[i]);
        i++;
    }
}

TEST(LIST_AND_STL, algorithm_iota){
    List<int> list{5};
    std::iota(list.begin(), list.end(), 0);
    std::vector<int> checkData = {0, 1, 2, 3, 4};
    for(std::size_t i = 0; i < list.size(); i++)
        EXPECT_EQ(list[i], checkData[i]);
}

TEST(LIST_AND_STL, algorithm_find){
    List<int> list{100};
    std::iota(list.begin(), list.end(), 0);
    EXPECT_NE(std::find(list.begin(), list.end(), 50), list.end());
}

TEST(LIST_AND_STL, algorithm_find_and_distance){
    List<int> list{100};
    std::iota(list.begin(), list.end(), 0);
    std::size_t pos = std::distance(list.begin(), std::find(list.begin(), list.end(), 50));
    EXPECT_EQ(pos, 50);
}

TEST(LIST_EXCEPTIONS, throw_popBack){
    List<int> list{};
    EXPECT_THROW(list.popBack(), std::out_of_range);
}

TEST(LIST_EXCEPTIONS, throw_popFront){
    List<int> list{};
    EXPECT_THROW(list.popFront(), std::out_of_range);
}

TEST(LIST_EXCEPTIONS, throw_removeAt){
    List<int> list{};
    EXPECT_THROW(list.removeAt(1), std::out_of_range);
}

TEST(LIST_EXCEPTIONS, throw_selector_1){
    List<int> list{};
    EXPECT_THROW(list.removeAt(1), std::out_of_range);
}

TEST(LIST_EXCEPTIONS, throw_selector_2){
    List<int> list{1};
    EXPECT_THROW(list.removeAt(1), std::out_of_range);
}

TEST(LIST_EXCEPTIONS, NO_throw_clear){
    List<int> list{};
    EXPECT_NO_THROW(list.clear());
}

int main(void){
    testing::InitGoogleTest();

    return ::RUN_ALL_TESTS();
}