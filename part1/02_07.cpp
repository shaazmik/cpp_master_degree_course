#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include <limits>

double* own_realloc(double* old_memory, int* old_capacity) {
    int new_capacity = *old_capacity * 2;
    double* temp = new double[new_capacity](); // Новый временный массив

    for (int i = 0; i < *old_capacity; i++) {
        temp[i] = old_memory[i];
    }
    
    delete[] old_memory; // Освобождаем старую память
    *old_capacity = new_capacity;
    return temp;
}

int main() {
    int capacity = 5; // Начальная вместимость массива
    double* numbers = new double[capacity](); // Динамическое выделение памяти
    int count = 0;
    
    double value = NAN;
    
    std::cout << "Enter the numbers separated by a space:" << std::endl;
    std::string line;
    std::getline(std::cin, line); // Считываем всю строку
    std::istringstream input_string_stream(line);

    while (input_string_stream >> value) {
        if (count == capacity) {
            std::cout << "\nRealloc data" << "\n";
            numbers = own_realloc(numbers, &capacity);
            std::cout << "capacity: " << capacity << " numbers pointer: " << numbers << "\n"; 
        }

        numbers[count++] = value;
    }

    if (count == 0) {
        std::cout << "No numbers entered" << std::endl;
        delete[] numbers; // Освобождаем память перед выходом
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
    std::cout << "\n";
    std::cout << "Minimum: "            << min_val << "\n";
    std::cout << "Maximum: "            << max_val << "\n";
    std::cout << "Mean: "               << mean    << "\n";
    std::cout << "Standard deviation: " << std_dev << std::endl;
    
    delete[] numbers; // Освобождение динамической памяти
    return 0;
}