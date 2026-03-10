#include "DUCommand.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <map>
#include "UIUtils.h"
#include "ShowCommand.h"

namespace fs = std::filesystem;

struct ItemInfo {
    std::string name;
    uintmax_t size;
    bool is_dir;
};

void DUCommand::execute(const std::vector<std::string>& args) const {
    fs::path targetPath = args.empty() ? fs::current_path() : fs::path(args[0]);

    if (!fs::exists(targetPath)) {
        std::cerr << "Error: Path does not exist: " << targetPath.u8string() << std::endl;
        return;
    }

    std::cout << "Analyzing: " << fs::absolute(targetPath).u8string() << "\n";

    std::map<std::string, uintmax_t> topLevelSizes;
    uintmax_t totalSize = 0;
    size_t fileCount = 0;
    Spinner spinner;

    // Однопроходное сканирование для скорости и живой анимации
    try {
        // Сначала соберем прямых детей
        for (const auto& entry : fs::directory_iterator(targetPath)) {
            topLevelSizes[entry.path().filename().u8string()] = 0;
            if (fs::is_regular_file(entry.status())) {
                uintmax_t s = fs::file_size(entry.path());
                topLevelSizes[entry.path().filename().u8string()] = s;
                totalSize += s;
                fileCount++;
            }
        }

        // Теперь рекурсивно считаем размеры подпапок
        for (auto& pair : topLevelSizes) {
            fs::path subPath = targetPath / pair.first;
            if (fs::is_directory(subPath)) {
                for (const auto& entry : fs::recursive_directory_iterator(subPath, fs::directory_options::skip_permission_denied)) {
                    if (fs::is_regular_file(entry.status())) {
                        uintmax_t s = fs::file_size(entry.path());
                        pair.second += s;
                        totalSize += s;
                        fileCount++;
                        
                        if (fileCount % 100 == 0) {
                            spinner.next("Scanning... [" + std::to_string(fileCount) + " files] [" + formatSize(totalSize) + "]");
                        }
                    }
                }
            }
        }
        spinner.clear();
    } catch (const std::exception& e) {
        spinner.clear();
        // Продолжаем с тем, что успели собрать
    }

    if (totalSize == 0) {
        std::cout << "Directory is empty or inaccessible." << std::endl;
        return;
    }

    // Подготовка списка для вывода
    std::vector<ItemInfo> items;
    for (auto const& [name, size] : topLevelSizes) {
        items.push_back({name, size, fs::is_directory(targetPath / name)});
    }

    // Сортировка по размеру (от больших к малым)
    std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) {
        return a.size > b.size;
    });

    std::cout << "\r" << std::string(50, ' ') << "\r"; // Очистка строки спиннера
    
    // Вывод главного прогресс-бара (Итого)
    std::cout << "TOTAL: ";
    ProgressBar::print(100.0, formatSize(totalSize), ".");
    std::cout << "\n" << std::string(60, '-') << "\n";

    // Вывод ТОП-10 результатов
    size_t limit = std::min(items.size(), (size_t)10);
    uintmax_t shownSize = 0;

    for (size_t i = 0; i < limit; ++i) {
        double percent = (static_cast<double>(items[i].size) / totalSize) * 100.0;
        ProgressBar::print(percent, formatSize(items[i].size), items[i].name + (items[i].is_dir ? "/" : ""));
        std::cout << "\n";
        shownSize += items[i].size;
    }

    // Если есть еще файлы, группируем их в "Other"
    if (items.size() > limit) {
        uintmax_t otherSize = totalSize - shownSize;
        double percent = (static_cast<double>(otherSize) / totalSize) * 100.0;
        ProgressBar::print(percent, formatSize(otherSize), "[... other " + std::to_string(items.size() - limit) + " items]");
        std::cout << "\n";
    }

    std::cout << "\nTotal analyzed: " << formatSize(totalSize) << " (" << fileCount << " files)\n";
}
