// Copyright 2022 GHA Test Team

#include <gtest/gtest.h>
#include "Automata.h"
#include <string>

TEST(AutomataTest, on_off) {
    Automata a;
    EXPECT_EQ(a.getState(), "OFF (Выключен)");
    a.on();
    EXPECT_EQ(a.getState(), "WAIT (Ожидание)");
    a.off();
    EXPECT_EQ(a.getState(), "OFF (Выключен)");
}

TEST(AutomataTest, coin) {
    Automata a;
    a.on();
    a.coin(50);
    EXPECT_EQ(a.getState(), "ACCEPT (Приём денег)");
    a.coin(30);
    a.choice(0);
    EXPECT_EQ(a.getState(), "CHECK (Проверка средств)");
    a.check();
    EXPECT_EQ(a.getState(), "COOK (Приготовление)");
    a.cook();
    EXPECT_EQ(a.getState(), "WAIT (Ожидание)");
}

TEST(AutomataTest, cancel) {
    Automata a;
    a.on();
    a.coin(100);
    a.cancel();
    EXPECT_EQ(a.getState(), "WAIT (Ожидание)");
    a.choice(0);
    EXPECT_EQ(a.getState(), "WAIT (Ожидание)");
    a.cancel();
    EXPECT_EQ(a.getState(), "WAIT (Ожидание)");
}

TEST(AutomataTest, choice_invalid) {
    Automata a;
    a.on();
    a.coin(50);
    a.choice(10);
    EXPECT_EQ(a.getState(), "ACCEPT (Приём денег)");
    a.choice(-1);
    EXPECT_EQ(a.getState(), "ACCEPT (Приём денег)");
}

TEST(AutomataTest, insufficient_funds) {
    Automata a;
    a.on();
    a.coin(20);
    a.choice(0);
    EXPECT_EQ(a.getState(), "CHECK (Проверка средств)");
    bool ok = a.check();
    EXPECT_FALSE(ok);
    EXPECT_EQ(a.getState(), "ACCEPT (Приём денег)");
    a.coin(10);
    a.check();
    EXPECT_EQ(a.getState(), "COOK (Приготовление)");
    a.cook();
    EXPECT_EQ(a.getState(), "WAIT (Ожидание)");
}

TEST(AutomataTest, cook_without_check) {
    Automata a;
    a.on();
    a.coin(50);
    a.choice(0);
    a.cook();
    EXPECT_EQ(a.getState(), "CHECK (Проверка средств)");
}

TEST(AutomataTest, finish_direct) {
    Automata a;
    a.on();
    a.coin(50);
    a.choice(0);
    a.check();
    a.cook();
    EXPECT_EQ(a.getState(), "WAIT (Ожидание)");
    a.finish();
    EXPECT_EQ(a.getState(), "WAIT (Ожидание)");
}

TEST(AutomataTest, multiple_coins) {
    Automata a;
    a.on();
    a.coin(10);
    a.coin(20);
    a.coin(5);
    a.choice(1);
    a.check();
    EXPECT_EQ(a.getState(), "ACCEPT (Приём денег)");
    a.coin(15);
    a.check();
    EXPECT_EQ(a.getState(), "COOK (Приготовление)");
    a.cook();
    EXPECT_EQ(a.getState(), "WAIT (Ожидание)");
}

TEST(AutomataTest, off_during_accept) {
    Automata a;
    a.on();
    a.coin(100);
    a.off();
    EXPECT_EQ(a.getState(), "OFF (Выключен)");
    a.choice(0);
    EXPECT_EQ(a.getState(), "OFF (Выключен)");
}

TEST(AutomataTest, getMenu) {
    Automata a;
    std::string menu = a.getMenu();
    EXPECT_FALSE(menu.empty());
}

TEST(AutomataTest, state_transitions) {
    Automata a;
    EXPECT_EQ(a.getState(), "OFF (Выключен)");
    a.on();
    EXPECT_EQ(a.getState(), "WAIT (Ожидание)");
    a.coin(10);
    EXPECT_EQ(a.getState(), "ACCEPT (Приём денег)");
    a.choice(0);
    EXPECT_EQ(a.getState(), "CHECK (Проверка средств)");
    a.check();
    EXPECT_EQ(a.getState(), "COOK (Приготовление)");
    a.cook();
    EXPECT_EQ(a.getState(), "WAIT (Ожидание)");
}
