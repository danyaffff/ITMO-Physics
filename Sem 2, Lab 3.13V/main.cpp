#define μ 4 * M_PI * 10e-7
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>

struct Point {
    double x;
    double y;
    double z;
};

struct Vector {
    double x;
    double y;
    double z;
};

struct Interval {
    double from = -1;
    double to = -1;
};

struct Field {
    Point coordinates;
    Vector field;
     
    bool operator>(const Field & right) const {
        return coordinates.z > right.coordinates.z;
    }
    bool operator<(const Field & right) const {
        return coordinates.z < right.coordinates.z;
    }
};

double distance(double x, double y, double z) {
    return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

double distance(double x, double y) {
    return sqrt(pow(x, 2) + pow(y, 2));
}

int main(int argc, char* argv[]) {
    std::ifstream fin;
    fin.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    std::ofstream fout;
    fout.exceptions(std::ofstream::badbit | std::ofstream::failbit);
    
    std::vector<Field> magneticField;
    
    std::vector<double> flatGradient;
    std::vector<double> gradient;
    std::vector<double> theoreticalMagneticInduction;
    std::vector<double> experimentalMagneticInduction;
    std::vector<double> fieldModule;
    
    Field field;
    
    std::vector<std::vector<Interval>> deltaZ;
    
    std::string temp;
    
    double I;
    double d;
    double R;
    
    try {
        fin.open(argv[1]);
        fout.open(argv[2]);
        
        std::cout << "Введите силу тока >> ";
        std::cin >> I;
        std::cout << "Введите расстояние между кольцами >> ";
        std::cin >> d;
        std::cout << "Введите радиус кольца >> ";
        std::cin >> R;
        
        for (int i = 0; i < 9; i++) {
            getline(fin, temp);
        }
        
        while (!fin.eof()) {
            fin >> field.coordinates.x >> field.coordinates.y >> field.coordinates.z >> field.field.x >> field.field.y >> field.field.z;
            
            magneticField.push_back(field);
        }
        
        magneticField.pop_back();
        
        fin.close();
        
        deltaZ.resize(magneticField.size());
        for (int i = 0; i < magneticField.size(); i++) {
            deltaZ[i].resize(10);
        }
        
        std::sort(magneticField.begin(), magneticField.end());
        
        for (int i = 0; i < magneticField.size(); i++) {
            fieldModule.push_back(distance(μ * magneticField[i].field.x, μ * magneticField[i].field.y, μ * magneticField[i].field.z));  // |B|
            
            if (i != magneticField.size() - 1) {
                flatGradient.push_back(distance(μ * magneticField[i].field.x, μ * magneticField[i].field.y) / distance(μ * magneticField[i + 1].field.x, μ * magneticField[i].field.y));
                gradient.push_back(magneticField[i].field.z / magneticField[i + 1].field.z);  // μ сокращается
            } else {
                flatGradient.push_back(-1);
                gradient.push_back(-1);
            }
            
            theoreticalMagneticInduction.push_back(μ * magneticField[i].field.z);
            
            double first = μ * I * R * R * 0.5;
            double second = 1 / pow(pow(magneticField[i].coordinates.z - d * 0.5, 2) + R * R, 1.5);
            double third = 1 / pow(pow(magneticField[i].coordinates.z + d * 0.5, 2) + R * R, 1.5);
            
            experimentalMagneticInduction.push_back(first * (second + third));
            
            if (i == 0) {
                for (double k = 0.005; k <= 0.05; k += 0.005) {
                    deltaZ[0][static_cast<int>(k * 200 - 1)].from = magneticField[0].coordinates.z;
                    
                    bool isFirst = true;
                    
                    for (int j = 1; j < magneticField.size(); j++) {
                        if (abs(magneticField[0].coordinates.z - magneticField[j].coordinates.z) < abs(k * magneticField[0].coordinates.z)) {
                            deltaZ[0][static_cast<int>(k * 200 - 1)].to = magneticField[j].coordinates.z;
                            isFirst = false;
                        } else {
                            if (isFirst) {
                                deltaZ[0][static_cast<int>(k * 200 - 1)].to = magneticField[0].coordinates.z;
                            }
                            break;
                        }
                    }
                }
            } else if (i == magneticField.size() - 1) {
                for (double k = 0.005; k <= 0.05; k += 0.005) {
                    deltaZ[magneticField.size() - 1][static_cast<int>(k * 200 - 1)].to = magneticField[i].coordinates.z;
                    
                    bool isFirst = true;
                    
                    for (int j = static_cast<int>(magneticField.size() - 2); j >= 0; j--) {
                        if (abs(magneticField[magneticField.size() - 1].coordinates.z - magneticField[j].coordinates.z) < abs(k * magneticField[magneticField.size() - 1].coordinates.z)) {
                            deltaZ[magneticField.size() - 1][static_cast<int>(k * 200 - 1)].from = magneticField[j].coordinates.z;
                            isFirst = false;
                        } else {
                            if (isFirst) {
                                deltaZ[magneticField.size() - 1][static_cast<int>(k * 200 - 1)].from = magneticField[i].coordinates.z;
                            }
                            break;
                        }
                    }
                }
            } else {
                for (double k = 0.005; k <= 0.05; k += 0.005) {
                    bool isFirstBefore = true;
                    bool isFirstAfter = true;
                    
                    for (int j = i - 1; j >= 0; j--) {
                        if (abs(magneticField[i].coordinates.z - magneticField[j].coordinates.z) < abs(k * magneticField[i].coordinates.z)) {
                            deltaZ[i][static_cast<int>(k * 200 - 1)].from = magneticField[j].coordinates.z;
                            isFirstBefore = false;
                        } else {
                            if (isFirstBefore) {
                                deltaZ[i][static_cast<int>(k * 200 - 1)].from = magneticField[i].coordinates.z;
                            }
                            break;
                        }
                    }
                    
                    for (int j = i + 1; j < magneticField.size(); j++) {
                        if (abs(magneticField[i].coordinates.z - magneticField[j].coordinates.z) < abs(k * magneticField[i].coordinates.z)) {
                            deltaZ[i][static_cast<int>(k * 200 - 1)].to = magneticField[j].coordinates.z;
                            isFirstAfter = false;
                        } else {
                            if (isFirstAfter) {
                                deltaZ[i][static_cast<int>(k * 200 - 1)].to = magneticField[i].coordinates.z;
                            }
                            break;
                        }
                    }
                }
            }
        }

        fout << "Z\t\t{Bx\t\tBy\t\tBz}\t\t|B|\t\t∇Bz\t\t∇Bxy\t\tBz(э)\t\tBz(т)\n";
        for (int i = 0; i < magneticField.size(); i++) {
            fout << std::fixed << std::setprecision(8) << magneticField[i].coordinates.z << "\t" << magneticField[i].field.x << "\t" << magneticField[i].field.y << "\t" << magneticField[i].field.z << "\t" << fieldModule[i] << "\t" << gradient[i] << "\t" << flatGradient[i] << "\t" << theoreticalMagneticInduction[i] << "\t" << experimentalMagneticInduction[i] << std::endl;
        }
        fout << std::endl << "Интервалы:";
        for (int i = 0; i < magneticField.size(); i++) {
            fout << std::endl << magneticField[i].coordinates.z << std::endl;
            for (int j = 0; j < 10; j++) {
                fout << "\t" << j + 1 << "%: " << std::setprecision(8) << deltaZ[i][j].from << "..." << deltaZ[i][j].to << std::endl;
            }
        }
        fout.close();
    } catch (std::ifstream::failure &) {
        std::cerr << "Ошибка при открытии входного файла!" << std::endl;
        return 1;
    } catch (std::ofstream::failure &) {
        std::cerr << "Ошибка при открытии выходного файла!" << std::endl;
        return 1;
    }
}
