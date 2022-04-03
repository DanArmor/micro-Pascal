#include "Queue.cpp"
#include <gtest/gtest.h>

/**
 * @file testQueue.cpp
 * @author DanArmor (https://github.com/DanArmor)
 * @brief Тесты для очереди на базе List
 * @version 1.0
 * @date 2022-03-27
 */

/// @brief Тест вставки
TEST(QUEUE_SIMPLE, push){
    Queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);
    EXPECT_EQ(q.size(), 3);
}

/// @brief Тест исключения элементов
TEST(QUEUE_SIMPLE, push_pop){
    Queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);
    q.pop();
    q.pop();
    q.pop();
    EXPECT_EQ(q.size(), 0);
}

/// @brief Тест взятия первого элемента
TEST(QUEUE_SIMPLE, push_pick){
    Queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);

   EXPECT_EQ(q.pick(), 1);
}

/// @brief Тест исключения и взятия первого элемента
TEST(QUEUE_SIMPLE, push_pop_pick){
    Queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);

   EXPECT_EQ(q.pick(), 1);
   q.pop();
   EXPECT_EQ(q.pick(), 2);
   q.pop();
   EXPECT_EQ(q.pick(), 3);
}

/// @brief Тест выброса исключения при попытке взять элемент из пустой очереди
TEST(QUEUE_SIMPLE, push_pop_pick_throw){
    Queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);

   EXPECT_EQ(q.pick(), 1);
   q.pop();
   EXPECT_EQ(q.pick(), 2);
   q.pop();
   EXPECT_EQ(q.pick(), 3);
   q.pop();
   EXPECT_ANY_THROW(q.pick());
}

int main(void){
    testing::InitGoogleTest();

    return ::RUN_ALL_TESTS();
}