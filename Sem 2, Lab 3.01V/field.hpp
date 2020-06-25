#ifndef field_hpp
#define field_hpp
#define step 0.1  // Устанавливаем шаг нашей сетки как 1/10 сантиметра
#define xBegin -8  // Устаналиваем самую левую точку по иксу
#define xEnd 8  // Устаналиваем самую правую точку по иксу
#define yBegin -5  // Устанавлвиаем самую нижнюю точку по игреку
#define yEnd 5  // Устанавлвиаем самую верхнюю точку по игреку
#include "image.hpp"

struct Point {  // Структура, отвечающая за создание точки с координатами (x, y) и потенциалом potential
    double x;
    double y;
    
    double potential;
};

struct Potential {  // Структура, отвечающая за разбиение потенциала на x- и y- составляющие, и сохранение Ex и Ey компоненты напряженности
    double x;
    double y;
    
    double electricFieldX;
    double electricFieldY;
};

class Field {
    Point point;  // Создаем точку, в окторую мы будем записывать значения
    
    std::vector<Pixel> image;  // Наша картинка, которая будет строиться по ходу считывания данных, и которую мы потом передадим в класс-отрисовщик
    std::vector<Point> potential;  // Вектор, указывающий потенциал в каждой точке
    std::vector<Potential> electricField;  // Вектор, указывающий Ex и Ey для каждой точки
    
    std::string temp;  // Переменная, которая нужна для считывания ненужной информации
    
    void calculatePixels();  // Метод, который будет считать цвет пикселя в конкретной точке
    
    std::pair<int, int> findNearestX(double, double);  // Метод, который будет находить ближайшую по иксу точку

    std::pair<int, int> findNearestY(double, double);  // Метод, который будет находить ближайшую по игреку точку
    
    double distance(double, double, double, double);  // Метод, который вычисляет дистанцию
    
public:
    void input(std::string);  // Ввод данных в векторы
    
    void print(std::string);  // Построение картинки
};

#endif
