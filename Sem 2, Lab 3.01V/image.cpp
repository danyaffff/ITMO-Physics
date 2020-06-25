#include "image.hpp"

Image::Image(int height, int width, std::vector<Pixel> image): height(height), width(width), image(image) {}  // Конструктор

void Image::save(std::string filename) {
    std::ofstream fout(filename);  // ОТкрываем файл filename на запись
    
    fout << "P6" << '\n';  // Вводим версию формата .pnm (в данном слуачае P6 говорит нам, что мы будем работать с raw .ppm)
    fout << width << ' ' << height << '\n';  // Вводим высоту и ширину картинки
    fout << 255 << '\n';  // Вводим глуину цвета
    
    for (int i = 0; i < width * height; i++) { // Вробегаемся циклом по вектору
        fout << image[i].red << image[i].green << image[i].blue;  // И выводим каждую компонену цвета
    }
    
    fout.close();  // Зыкрываем файл
}

Image::~Image() {  // Дестуктор
    image.clear();  // Очищаем наш вектор
}
