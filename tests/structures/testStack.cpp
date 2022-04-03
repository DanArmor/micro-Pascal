#include <gtest/gtest.h>
#include "Stack.cpp"

/**
 * @file testQueue.cpp
 * @author DanArmor (https://github.com/DanArmor)
 * @brief Тесты для стека на базе List
 * @version 1.0
 * @date 2022-03-28
 */

/// @brief Тест вставки
TEST(STACK_SIMPLE, push){
    Stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);
    EXPECT_EQ(s.size(), 3);
}

/// @brief Тест исключения элементов
TEST(STACK_SIMPLE, push_pop){
    Stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);
    s.pop();
    s.pop();
    s.pop();
    EXPECT_EQ(s.size(), 0);
}

/// @brief Тест взятия первого элемента
TEST(STACK_SIMPLE, push_pick){
    Stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);

   EXPECT_EQ(s.pick(), 3);
}

/// @brief Тест исключения и взятия первого элемента
TEST(STACK_SIMPLE, push_pop_pick){
    Stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);

   EXPECT_EQ(s.pick(), 3);
   s.pop();
   EXPECT_EQ(s.pick(), 2);
   s.pop();
   EXPECT_EQ(s.pick(), 1);
}

/// @brief Тест выброса исключения при попытке взять элемент из пустой очереди
TEST(STACK_SIMPLE, push_pop_pick_throw){
    Stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);

   EXPECT_EQ(s.pick(), 3);
   s.pop();
   EXPECT_EQ(s.pick(), 2);
   s.pop();
   EXPECT_EQ(s.pick(), 1);
   s.pop();
   EXPECT_ANY_THROW(s.pick());
}

int main(void){
    testing::InitGoogleTest();
    return ::RUN_ALL_TESTS();
}