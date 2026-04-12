// Copyright 2026
#include "Automata.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>
#include <string>
Automata::Automata() : cash(0), state(States::OFF), selected_item(-1) {
    menu = { "Чай", "Кофе", "Молоко", "Какао", "Горячий шоколад" };
    prices = { 30, 50, 40, 45, 60 };
}

void Automata::on() {
    if (state == States::OFF) {
        state = States::WAIT;
        std::cout << "Автомат включен. Состояние: WAIT\n";
    } else {
        std::cout << "Автомат уже включен или не в состоянии OFF\n";
    }
}

void Automata::off() {
    if (state != States::COOK) {
        state = States::OFF;
        cash = 0;
        selected_item = -1;
        std::cout << "Автомат выключен. Состояние: OFF\n";
    } else {
        std::cout << "Невозможно выключить во время приготовления\n";
    }
}

void Automata::coin(int amount) {
    if (amount <= 0) {
        std::cout << "Некорректная сумма\n";
        return;
    }
    if (state == States::WAIT || state == States::ACCEPT) {
        cash += amount;
        state = States::ACCEPT;
        std::cout << "Внесено " << amount << " руб. Текущая сумма: "
                  << cash << "\n";
    } else {
        std::cout << "Нельзя внести деньги в текущем состоянии\n";
    }
}

std::string Automata::getMenu() const {
    std::stringstream ss;
    ss << "Меню:\n";
    for (size_t i = 0; i < menu.size(); ++i) {
        ss << i + 1 << ". " << menu[i] << " - "
           << prices[i] << " руб.\n";
    }
    return ss.str();
}

std::string Automata::getState() const {
    switch (state) {
        case States::OFF:    return "OFF (Выключен)";
        case States::WAIT:   return "WAIT (Ожидание)";
        case States::ACCEPT: return "ACCEPT (Приём денег)";
        case States::CHECK:  return "CHECK (Проверка средств)";
        case States::COOK:   return "COOK (Приготовление)";
        default:             return "UNKNOWN";
    }
}

void Automata::choice(int index) {
    if (state != States::ACCEPT) {
        std::cout << "Выбор напитка возможен только в состоянии ACCEPT\n";
        return;
    }
    if (index < 0 || index >= static_cast<int>(menu.size())) {
        std::cout << "Неверный номер напитка\n";
        return;
    }
    selected_item = index;
    state = States::CHECK;
    std::cout << "Выбран напиток: " << menu[selected_item] << "\n";
}

bool Automata::check() {
    if (state != States::CHECK) {
        std::cout << "Проверка возможна только в состоянии CHECK\n";
        return false;
    }
    if (cash >= prices[selected_item]) {
        state = States::COOK;
        std::cout << "Денег достаточно. Начинаем приготовление...\n";
        return true;
    } else {
        std::cout << "Недостаточно средств. Не хватает "
                  << (prices[selected_item] - cash) << " руб.\n";
        state = States::ACCEPT;
        return false;
    }
}

void Automata::cancel() {
    if (state == States::WAIT || state == States::ACCEPT) {
        if (cash > 0) {
            std::cout << "Возврат денег: " << cash << " руб.\n";
            cash = 0;
        }
        selected_item = -1;
        state = States::WAIT;
        std::cout << "Операция отменена. Состояние: WAIT\n";
    } else {
        std::cout << "Отмена невозможна в текущем состоянии\n";
    }
}

void Automata::cook() {
    if (state != States::COOK) {
        std::cout << "Приготовление возможно только в состоянии COOK\n";
        return;
    }
    std::cout << "Приготовление " << menu[selected_item] << "...\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Напиток готов!\n";
    finish();
}

void Automata::finish() {
    if (state != States::COOK && state != States::ACCEPT) {
        std::cout << "Завершение невозможно в текущем состоянии\n";
        return;
    }
    if (state == States::COOK) {
        int price = prices[selected_item];
        cash -= price;
        std::cout << "Списано " << price << " руб. Остаток: "
                  << cash << " руб.\n";
        if (cash > 0) {
            std::cout << "Ваша сдача: " << cash << " руб.\n";
            cash = 0;
        }
    }
    selected_item = -1;
    state = States::WAIT;
    std::cout << "Сеанс завершен. Автомат в состоянии WAIT\n";
}