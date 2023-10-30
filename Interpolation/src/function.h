#pragma once

#include <concepts>
#include <iostream>
#include <vector>

using namespace std;
#define EPS 0  // TODO: Значение должно зависеть от типа (для float 1e-6) или задаваться вручную

template <typename T>
concept Numeric = is_arithmetic_v<T>;

template <Numeric T>
class IFunction {
   public:
    // Получение значения функции
    virtual auto calculate(T x) -> T const = 0;

    // Получение значений функции в узлах
    auto calculateByNodes(const vector<T> &x_nodes) -> vector<T> const; // DEBUG: Сделать метод перегрузкой calculate

    // Преобразование промежутка в узлы
    static auto getNodesInRange(const T startOfRange, const T endOfRange, const ushort countOfPoints)
        -> vector<T>;
    static auto getNodesInRange(const T startOfRange, const T endOfRange, const double step) -> vector<T>;

    // Преобразование промежутка в узлы Чебышева
    static auto getChebyshevNodes(const T startOfRange, const T endOfRange, const ushort countOfPoints)
        -> vector<T>;
};

template <Numeric T>
class FunctionSeries : public IFunction<T> {
   private:
    T (*Q)(T x, ushort n);
    T (*a_0)(T x);

   public:
    // Задаем функцию с помощью ряда, где a_0 - первый элемент ряда, Q_i = a_(i+1)/a_i
    FunctionSeries(T (*a_0)(T), T (*Q)(T, ushort));

    // Получение значения функции
    auto calculate(T x) -> T const override;
};

// ------------ IFunction ------------

// Получение значений функции в узлах
template <Numeric T>
auto IFunction<T>::calculateByNodes(const vector<T> &x_nodes) -> vector<T> const {
    vector<T> y_nodes(x_nodes.size());
    for (ushort i = 0; i < x_nodes.size(); i++) {
        y_nodes[i] = calculate(x_nodes[i]);
    }
    return y_nodes;
}

// Преобразование промежутка в узлы
template <Numeric T>
auto IFunction<T>::getNodesInRange(const T startOfRange, const T endOfRange, const ushort countOfPoints) // DEBUG: Сделать метод перегрузкой calculate
    -> vector<T> {
    ushort countOfIntervals = countOfPoints - 1;
    vector<T> nodes(countOfPoints);
    for (ushort i = 0; i < countOfPoints; ++i) {
        T alpha = static_cast<T>(i) / countOfIntervals;
        nodes[i] = startOfRange + alpha * (endOfRange - startOfRange);
    }
    return nodes;
}
template <Numeric T>
auto IFunction<T>::getNodesInRange(const T startOfRange, const T endOfRange, const double step) -> vector<T> {
    ushort countOfPoints = (int)((endOfRange - startOfRange) / step) + 1;
    return getNodesInRange(startOfRange, endOfRange, countOfPoints);
}

// Преобразование промежутка в узлы Чебышева // FIXME: 1)Узлы в обратном порядке; 2)Не включает концы промежутка
template <Numeric T>
auto IFunction<T>::getChebyshevNodes(const T startOfRange, const T endOfRange, const ushort countOfPoints)
    -> vector<T> {
    vector<T> nodes(countOfPoints);
    for (ushort i = 0; i < countOfPoints; i++) {
        nodes[i] = (T)0.5 * ((startOfRange + endOfRange) +
                             (endOfRange - startOfRange) * cos(M_PI * (i + (T)0.5) / countOfPoints));
    }
    return nodes;
}

// ------------ FunctionSeries ------------

// Задаем функцию с помощью ряда, где a_0 - первый элемент ряда, Q_i = a_(i+1)/a_i
template <Numeric T>
FunctionSeries<T>::FunctionSeries(T (*a_0)(T), T (*Q)(T, ushort)) : Q(Q), a_0(a_0){};

// Получени значения функции
template <Numeric T>
auto FunctionSeries<T>::calculate(T x) -> T const {
    T a_n = a_0(x);
    T sum = a_n;
    for (ushort i = 0; abs(a_n) > EPS; i++) {
        a_n *= Q(x, i);  // a_n = a_(n+1)
        sum += a_n;
    }
    return sum;
}