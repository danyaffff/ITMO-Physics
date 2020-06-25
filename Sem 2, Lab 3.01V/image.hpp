#ifndef image_hpp
#define image_hpp
#include <fstream>
#include <vector>
#include <cmath>

struct Pixel {  // Струкутра Pixel, которая отвечает за отображение цвета пикселя на картинке. Работает по принципу 0xRRGGBB, где XX — двузначное шестнадцатеричное число (00—FF), отвечающее за присутствие и количество данного цвета в цвете пикселя
    unsigned char red;  // Красный (0—255)
    unsigned char green;  // Зеленый (0—255)
    unsigned char blue;  // Синий (0—255)
};

class Image {  // Класс, отвечающий за рисование картинки
    int width;  // Ширина картинки
    int height;  // Высота картинки
    
    std::vector<Pixel> image;  // Сама картинка, каждый элемент которой — трехначное число (rex, green, blue)
    
public:
    Image(int, int, std::vector<Pixel>);  // Конструктор
    
    ~Image();  // Деструктор
    
    void save(std::string);  // Вывод картинки в файл
};

#endif
