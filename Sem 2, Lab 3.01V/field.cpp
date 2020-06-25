#include "field.hpp"

void Field::print(std::string outputFilename) {
    Image picture((yEnd - yBegin) / step + 1, (xEnd - xBegin) / step + 1, image);  // Вычисление размеров изображения
    
    picture.save(outputFilename);  // Вызываем функцию создания картинки
}

void Field::calculatePixels() {
    for (int i = 0; i < image.size(); i++) {
        double electricFieldInPoint = distance(0, 0, abs(electricField[i].electricFieldX), abs(electricField[i].electricFieldY)) / 100;  // Вычисляем значение напряженности в точке. Деление указываем насколько сильно будем колебаться значение напряженности (чем выше число, тем меньше значение)
        if (electricFieldInPoint > 1) {  // Если значение больше максимального
            electricFieldInPoint = 1;  // То устанавливаем в максимальное
        } else if (electricFieldInPoint < 0.5) {  // Если значение меньше 0.5, то красная компонента учитываться не будет
            image[i].blue = electricFieldInPoint * 4 * 200;  // Записываем значение синей компоненты
        } else {
            image[i].red = ((electricFieldInPoint - 0.25) * 255) / 0.75;  // Значение красной компоненты
            image[i].blue = 255 - image[i].red;  // Значение синей компоненты
        }
    }
}

std::pair<int, int> Field::findNearestY(double x, double y) {
    double minBeforeY = std::numeric_limits<double>::max();  // Чтобы найти минимум, мы должны в самом начале установить значение в максимум
    double minAfterY = std::numeric_limits<double>::max();  // Чтобы найти минимум, мы должны в самом начале установить значение в максимум
    
    int indexMinBeforeY = 0;  // Индекс точки с минимальным меньшим игреком (минимальным расстоянием)
    int indexMinAfterY = 0;  // Индекс точки с минимальным большим игреком (минимальным расстоянием)
    
    for (int i = 0; i < potential.size(); i++) {  // Просто проходимся по всем точками и ищем минимум
        if ((distance(potential[i].x, potential[i].y, x, y) < minBeforeY) && (potential[i].y > y)) {
            minBeforeY = distance(potential[i].x, potential[i].y, x, y);
            indexMinBeforeY = i;
        }
        if ((distance(potential[i].x, potential[i].y, x, y) < minAfterY) && (potential[i].y < y)) {
            minAfterY = distance(potential[i].x, potential[i].y, x, y);
            indexMinAfterY = i;
        }
    }
    return {indexMinBeforeY, indexMinAfterY};  // Возвращаем индексы
}

std::pair<int, int> Field::findNearestX(double x, double y) {
    double minBeforeX = std::numeric_limits<double>::max();
    double minAfterX = std::numeric_limits<double>::max();
    
    int indexMinBeforeX = 0;
    int indexMinAfterX = 0;
    
    for (int i = 0; i < potential.size(); i++) {
        if ((distance(potential[i].x, potential[i].y, x, y) < minBeforeX)&&(potential[i].x > x)) {
            minBeforeX = distance(potential[i].x, potential[i].y, x, y);
            indexMinBeforeX = i;
        }
        if ((distance(potential[i].x, potential[i].y, x, y) < minAfterX) && (potential[i].x < x)) {
            minAfterX = distance(potential[i].x, potential[i].y, x, y);
            indexMinAfterX = i;
        }
    }
    return {indexMinBeforeX, indexMinAfterX};
}

void Field::input(std::string inputFilename) {
    std::ifstream fin(inputFilename);  // Открываем файл
    fin.exceptions(std::ifstream::badbit | std::ifstream::failbit);  // Подключаем исключения файла
    
    while (!fin.eof()) {  // Читаем до конца файла
        fin >> point.x >> point.y >> point.potential;  // Вводим x, y и potential
        
        getline(fin, temp);  // Считываем комплексную компоненту как ненужное (я не нашел информации про комплексный электромтатический потенциал (только в гидродинамике), так что отсекаем комплексную часть)
        
        potential.push_back(point);  // Записываем точку
    }
        
    fin.close();  // Закрываем файл
    
    for (double y = yBegin; y <= yEnd; y += step) {  // Проходимся по координатной сетке и ищем для каждой точки сетки точку из файла с минимальным расстоянием до нее
        for (double x = xBegin; x <= xEnd; x += step) {
            std::pair<int, int> x_dots = findNearestX(x, y);
            std::pair<int, int>  y_dots = findNearestY(x, y);

            double electricFieldX = (potential[x_dots.first].potential - potential[x_dots.second].potential) / (potential[x_dots.first].x - potential[x_dots.second].x);
            double electricFieldY = (potential[y_dots.first].potential - potential[y_dots.second].potential) / (potential[y_dots.first].y - potential[y_dots.second].y);
            
            electricField.push_back({x, y, electricFieldX, electricFieldY});  // Сохраняем координаты точек с напряженностями и сами напряженности
        }
    }
    
    image.resize(electricField.size());  // Изменяем размер вектора
    
    calculatePixels();  // Высчитываем значение цвета пикселей
}

double Field::distance(double minBeforeX, double minBeforeY, double minAfterX, double minAfterY) {
    return sqrt(pow(minBeforeX - minAfterX, 2) + pow(minBeforeY - minAfterY, 2));
}
