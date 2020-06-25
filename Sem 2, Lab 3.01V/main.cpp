#include <iostream>
#include "image.hpp"
#include "field.hpp"

int main() {
    std::string inputFilename;  // Название входного файла
    std::string outputFilename;  // Название выходного файла
    
    Field field;  // Создаем поле, с которрым мы будем взаимодействовать
    
    std::cout << "Введите название входного файла >> ";  // Название должно быть в формате <имя>.txt
    std::cin >> inputFilename;
        
    std::cout << "Введите название выходного файла >> ";  // Название должно быть в формате <имя>.ppm
    std::cin >> outputFilename;
    
    try {
        field.input(inputFilename);  // Вводим значения и считаем значения пикселей
    } catch (std::ifstream::failure &) {  // Если произошла ошибка открытия файла, то останавливаем программу
        std::cerr << "Не удалось открыть входной файл!" << std::endl;
        return 1;
    } catch (std::bad_alloc &) {  // Если не удалось выделить память под что-то, то останавливаем программу
        std::cerr << "Не удалось выделить память!" << std::endl;
        return 1;
    }

    try {
        field.print(outputFilename);
    } catch (std::ofstream::failure &) {  // Если произошла ошибка открытия файла, то останавливаем программу
        std::cerr << "Не удалось открыть выходной файл!" << std::endl;
        return 1;
    } catch (std::bad_alloc &) {  // Если не удалось выделить память под что-то, то останавливаем программу
        std::cerr << "Не удалось выделить память!" << std::endl;
        return 1;
    }
}
