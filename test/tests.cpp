// Copyright 2022 GHA Test Team

#include <gtest/gtest.h>
#include "Automata.h"
#include <iostream>
#include <cassert>
#include <string>

void test_on_off() {
    Automata a;
    assert(a.getState() == "OFF (Выключен)");
    a.on();
    assert(a.getState() == "WAIT (Ожидание)");
    a.off();
    assert(a.getState() == "OFF (Выключен)");
    std::cout << "test_on_off passed\n";
}

void test_coin() {
    Automata a;
    a.on();
    a.coin(50);
    assert(a.getState() == "ACCEPT (Приём денег)");
    a.coin(30);
    a.choice(0);
    assert(a.getState() == "CHECK (Проверка средств)");
    a.check();
    assert(a.getState() == "COOK (Приготовление)");
    a.cook();
    assert(a.getState() == "WAIT (Ожидание)");
    std::cout << "test_coin passed\n";
}

void test_cancel() {
    Automata a;
    a.on();
    a.coin(100);
    a.cancel();
    assert(a.getState() == "WAIT (Ожидание)");
    a.choice(0);
    assert(a.getState() == "WAIT (Ожидание)");
    a.cancel();
    assert(a.getState() == "WAIT (Ожидание)");
    std::cout << "test_cancel passed\n";
}

void test_choice_invalid() {
    Automata a;
    a.on();
    a.coin(50);
    a.choice(10);
    assert(a.getState() == "ACCEPT (Приём денег)");
    a.choice(-1);
    assert(a.getState() == "ACCEPT (Приём денег)");
    std::cout << "test_choice_invalid passed\n";
}

void test_insufficient_funds() {
    Automata a;
    a.on();
    a.coin(20);
    a.choice(0);
    assert(a.getState() == "CHECK (Проверка средств)");
    bool ok = a.check();
    assert(!ok);
    assert(a.getState() == "ACCEPT (Приём денег)");
    a.coin(10);
    a.check();
    assert(a.getState() == "COOK (Приготовление)");
    a.cook();
    assert(a.getState() == "WAIT (Ожидание)");
    std::cout << "test_insufficient_funds passed\n";
}

void test_cook_without_check() {
    Automata a;
    a.on();
    a.coin(50);
    a.choice(0);
    a.cook();
    assert(a.getState() == "CHECK (Проверка средств)");
    std::cout << "test_cook_without_check passed\n";
}

void test_finish_direct() {
    Automata a;
    a.on();
    a.coin(50);
    a.choice(0);
    a.check();
    a.cook();
    assert(a.getState() == "WAIT (Ожидание)");
    a.finish();
    assert(a.getState() == "WAIT (Ожидание)");
    std::cout << "test_finish_direct passed\n";
}

void test_multiple_coins() {
    Automata a;
    a.on();
    a.coin(10);
    a.coin(20);
    a.coin(5);
    a.choice(1);
    a.check();
    assert(a.getState() == "ACCEPT (Приём денег)");
    a.coin(15);
    a.check();
    assert(a.getState() == "COOK (Приготовление)");
    a.cook();
    assert(a.getState() == "WAIT (Ожидание)");
    std::cout << "test_multiple_coins passed\n";
}

void test_off_during_accept() {
    Automata a;
    a.on();
    a.coin(100);
    a.off();
    assert(a.getState() == "OFF (Выключен)");
    a.choice(0);
    assert(a.getState() == "OFF (Выключен)");
    std::cout << "test_off_during_accept passed\n";
}

void test_getMenu() {
    Automata a;
    std::string menu = a.getMenu();
    assert(!menu.empty());
    std::cout << "test_getMenu passed\n";
}

void test_state_transitions() {
    Automata a;
    assert(a.getState() == "OFF (Выключен)");
    a.on();
    assert(a.getState() == "WAIT (Ожидание)");
    a.coin(10);
    assert(a.getState() == "ACCEPT (Приём денег)");
    a.choice(0);
    assert(a.getState() == "CHECK (Проверка средств)");
    a.check();
    assert(a.getState() == "COOK (Приготовление)");
    a.cook();
    assert(a.getState() == "WAIT (Ожидание)");
    std::cout << "test_state_transitions passed\n";
}