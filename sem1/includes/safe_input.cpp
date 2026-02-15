#pragma once
#include <functional>
#include <string>
#include <limits>
/// @brief шаблонная функция безопасного ввода
/// @tparam T
/// @param prompt пояснение пользователю
/// @param error_message
/// @param validator функция проверки корректности ввода
/// @return обработанный и безопасный ввод
template <typename T>
T safe_input(const std::string &prompt, const std::string &error_message, std::function<bool(T)> validator = nullptr)
{
    T input;
    std::cout << prompt;
    while (!(std::cin >> input) || (std::cin.peek() != '\n') || (validator && !validator(input)))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << error_message << std::endl;
        std::cout << prompt;
    }
    return input;
}