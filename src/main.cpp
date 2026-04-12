// Copyright 2026
#include "Automata.h"
#include <iostream>
#include <clocale>

int main() {
    setlocale(LC_ALL, "Russian");

    std::cout << "=== Демонстрация работы автомата ===\n";
    Automata vm;

    vm.on();
    std::cout << vm.getMenu();

    vm.coin(100);
    vm.choice(0);
    vm.check();
    vm.cook();

    std::cout << "\n--- Тест отмены ---\n";
    Automata vm2;
    vm2.on();
    vm2.coin(50);
    vm2.choice(1);
    vm2.cancel();
    std::cout << vm2.getMenu();

    std::cout << "\n--- Тест недостатка денег ---\n";
    Automata vm3;
    vm3.on();
    vm3.coin(20);
    vm3.choice(0);
    vm3.check();
    vm3.coin(10);
    vm3.check();
    vm3.cook();

    return 0;
}