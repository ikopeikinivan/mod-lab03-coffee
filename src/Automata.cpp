// Copyright 2026
#include "Automata.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <string>

Automata::Automata() : cash(0), state(States::OFF), selected_item(-1) {
    menu = {"Чай", "Кофе"};
    prices = {30, 50};
}

void Automata::on() {
    if (state == States::OFF) {
        state = States::WAIT;
        std::cout << "Автомат включен. Состояние: WAIT" << std::endl;
    } else {
        std::cout << "Автомат уже включен" << std::endl;
    }
}

void Automata::off() {
    if (state != States::OFF && state != States::COOK) {
        state = States::OFF;
        cash = 0;
        selected_item = -1;
        std::cout << "Автомат выключен. Состояние: OFF" << std::endl;
    } else if (state == States::COOK) {
        std::cout << "Нельзя выключить во время приготовления" << std::endl;
    } else {
        std::cout << "Автомат уже выключен" << std::endl;
    }
}

void Automata::coin(int amount) {
    if (state == States::WAIT) {
        // Первая монета переводит из WAIT в ACCEPT
        state = States::ACCEPT;
        cash += amount;
        std::cout << "Внесено " << amount << " руб. Текущая сумма: "
                  << cash << std::endl;
    } else if (state == States::ACCEPT || state == States::CHECK) {
        cash += amount;
        std::cout << "Внесено " << amount << " руб. Текущая сумма: "
                  << cash << std::endl;
    } else {
        std::cout << "Внесение монет возможно только в состояниях "
                  << "WAIT, ACCEPT или CHECK" << std::endl;
    }
}

std::string Automata::getMenu() const {
    std::string result;
    for (size_t i = 0; i < menu.size(); ++i) {
        result += std::to_string(i + 1) + ". " + menu[i] + " - "
                  + std::to_string(prices[i]) + " руб.\n";
    }
    return result;
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
    if (state == States::ACCEPT) {
        if (index >= 0 && index < static_cast<int>(menu.size())) {
            selected_item = index;
            state = States::CHECK;
            std::cout << "Выбран напиток: " << menu[index] << std::endl;
        } else {
            std::cout << "Неверный номер напитка" << std::endl;
        }
    } else {
        std::cout << "Выбор напитка возможен только в состоянии ACCEPT"
                  << std::endl;
    }
}

bool Automata::check() {
    if (state == States::CHECK) {
        if (selected_item != -1) {
            int price = prices[selected_item];
            if (cash >= price) {
                state = States::COOK;
                std::cout << "Денег достаточно. Начинаем приготовление..."
                          << std::endl;
                return true;
            } else {
                std::cout << "Недостаточно средств. Не хватает "
                          << (price - cash) << " руб." << std::endl;
                return false;
            }
        } else {
            std::cout << "Напиток не выбран" << std::endl;
            state = States::ACCEPT;
            return false;
        }
    } else {
        std::cout << "Проверка возможна только в состоянии CHECK" << std::endl;
        return false;
    }
}

void Automata::cancel() {
    if (state == States::ACCEPT || state == States::CHECK) {
        std::cout << "Возврат денег: " << cash << " руб." << std::endl;
        cash = 0;
        selected_item = -1;
        state = States::WAIT;
        std::cout << "Операция отменена. Состояние: WAIT" << std::endl;
    } else {
        std::cout << "Отмена возможна только в состояниях ACCEPT или CHECK"
                  << std::endl;
    }
}

void Automata::cook() {
    if (state == States::COOK) {
        if (selected_item != -1) {
            int price = prices[selected_item];
            cash -= price;
            std::cout << "Приготовление " << menu[selected_item] << "..."
                      << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            std::cout << "Напиток готов!" << std::endl;
            if (cash > 0) {
                std::cout << "Списано " << price << " руб. Остаток: "
                          << cash << " руб." << std::endl;
                std::cout << "Ваша сдача: " << cash << " руб." << std::endl;
                cash = 0;
            } else {
                std::cout << "Списано " << price << " руб." << std::endl;
            }
            selected_item = -1;
            state = States::WAIT;
            std::cout << "Сеанс завершен. Автомат в состоянии WAIT"
                      << std::endl;
        } else {
            std::cout << "Ошибка: напиток не выбран" << std::endl;
            state = States::WAIT;
        }
    } else {
        std::cout << "Приготовление возможно только в состоянии COOK"
                  << std::endl;
    }
}

void Automata::finish() {
    if (state == States::WAIT) {
        std::cout << "Завершение невозможно в текущем состоянии" << std::endl;
    } else {
        cancel();
    }
}