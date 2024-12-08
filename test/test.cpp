#include <iostream>
#include <filesystem>
#include <regex>
#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;

void processImage(const fs::path& filePath) {
    try
    {
        // Загрузка изображения
        cv::Mat img = cv::imread(filePath.string());
        if (img.empty()) {
            std::cerr << "Ошибка: Невозможно открыть файл " << filePath << " как изображение.\n";
            return;
        }

        // Отражение изображения по вертикали
        cv::Mat mirroredImg;
        cv::flip(img, mirroredImg, 0); // 0 — вертикальное отражение

        // Создание нового имени файла
        std::string newFileName = filePath.stem().string() + "-mirrored.gif";
        fs::path newFilePath = filePath.parent_path() / newFileName;

        // Сохранение обработанного изображения
        if (!cv::imwrite(newFilePath.string(), mirroredImg)) {
            std::cerr << "Ошибка: Не удалось сохранить файл " << newFilePath << ".\n";
        }
        else {
            std::cout << "Файл успешно обработан и сохранен: " << newFilePath << "\n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при обработке файла " << filePath << ": " << e.what() << "\n";
    }
}

int main() {
    try {
        // Папка для обработки
        fs::path currentDir = fs::current_path();

        // Регулярное выражение для проверки форматов изображений
        std::regex regexExt("(\\.(bmp|gif|tiff|jpeg|jpg|png))$", std::regex::icase);

        // Перебор всех файлов в папке
        for (const auto& entry : fs::directory_iterator(currentDir)) {
            if (fs::is_regular_file(entry)) {
                fs::path filePath = entry.path();

                // Проверка формата файла
                if (std::regex_search(filePath.extension().string(), regexExt)) {
                    std::cout << "Обработка файла: " << filePath << "\n";
                    processImage(filePath);
                }
                else {
                    std::cerr << "Пропуск: " << filePath << " (не поддерживаемый формат).\n";
                }
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }

    return 0;
}
