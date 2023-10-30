#pragma once
#include <vector>

#include "function.h"

using namespace std;

template <Numeric T>
class Lagrange : public IFunction<T> {
   private:
    vector<T> x_data;  // Базовые узлы интерполяции
    vector<T> y_data;  // Значения в базовых узлах интерполяции
   public:
    // Задаем базовые узлы интерполяции
    Lagrange(vector<T> x_data, vector<T> y_data);

    // Метод Лагранджа для интерполяции функции в заданной точке x
    auto calculate(T x) -> T const;
};

// ------------ Lagrange ------------

// Задаем полином с помощью базовых узлов
template <Numeric T>
Lagrange<T>::Lagrange(vector<T> x_data, vector<T> y_data) : x_data(x_data), y_data(y_data) {
    if (x_data.size() == 0 || y_data.size() == 0) {
        throw runtime_error("Interpolator has no data points");
    }
    if (x_data.size() != y_data.size()) {
        throw invalid_argument("x_data and y_data must have the same size");
    }
}

// Метод для интерполяции функции в заданной точке x
template <Numeric T>
auto Lagrange<T>::calculate(T x) -> T const {
    T result = 0;
    for (ushort i = 0; i < x_data.size(); ++i) {
        T term = y_data[i];
        for (ushort j = 0; j < x_data.size(); ++j) {
            if (j != i) {
                term *= (x - x_data[j]) / (x_data[i] - x_data[j]);
            }
        }
        result += term;
    }
    return result;
}