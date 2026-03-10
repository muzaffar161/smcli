#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

/**
 * Простой класс для отображения прогресс-бара в консоли.
 */
class ProgressBar {
public:
    static void print(double percentage, const std::string& sizeStr, const std::string& label, int width = 20) {
        int pos = static_cast<int>(width * percentage / 100.0);

        std::cout << "\r[";
        for (int i = 0; i < width; ++i) {
            if (i < pos) std::cout << "█";
            else std::cout << "░";
        }
        std::cout << "] " << std::fixed << std::setprecision(1) << percentage << "% "
                  << "(" << sizeStr << ") " << label << std::flush;
    }

    static void clear() {
        std::cout << "\r" << std::string(100, ' ') << "\r" << std::flush;
    }
};

/**
 * Современный спиннер для анимации поиска.
 */
class Spinner {
private:
    // Используем Braille символы для плавности
    const std::vector<std::string> frames = {"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
    int index = 0;
public:
    void next(const std::string& message) {
        std::cout << "\r" << "\033[33m" << frames[index % frames.size()] << "\033[0m " << message << std::flush;
        index++;
    }
    
    static void clear() {
        // Очищаем строку полностью
        std::cout << "\r\033[K" << std::flush;
    }
};

#endif
