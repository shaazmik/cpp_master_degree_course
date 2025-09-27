#include <iostream>
#include <cmath>
#include <limits>

int main() {
    const int MAX_SIZE = 1000;
    double numbers[MAX_SIZE] = {};
    int count = 0;
    
    double value = NAN;
    
    std::cout << "Enter the numbers separated by a space: ";
    while (std::cin >> value && count < MAX_SIZE) {
        numbers[count++] = value;
    }
    
    if (count == 0) {
        std::cout << "No numbers entered" << std::endl;
        return 0;
    }
    
    // Вычисление минимального и максимального значений
    double min_val = numbers[0];
    double max_val = numbers[0];
    double sum = numbers[0];
    
    for (int i = 1; i < count; i++) {
        if (numbers[i] < min_val) {
            min_val = numbers[i];
        }
        if (numbers[i] > max_val) {
            max_val = numbers[i];
        }
        sum += numbers[i];
    }
    
    // Вычисление среднего арифметического
    double mean = sum / count;
    
    // Вычисление стандартного отклонения
    double variance = 0.0;
    for (int i = 0; i < count; ++i) {
        double deviation = numbers[i] - mean;
        variance += deviation * deviation;
    }
    variance /= count; // Дисперсия
    double std_dev = std::sqrt(variance);
    
    // Вывод результатов
    std::cout << "Minimum: " << min_val << std::endl;
    std::cout << "Maximum: " << max_val << std::endl;
    std::cout << "Mean: " << mean << std::endl;
    std::cout << "Standard deviation: " << std_dev << std::endl;
    
    return 0;
}