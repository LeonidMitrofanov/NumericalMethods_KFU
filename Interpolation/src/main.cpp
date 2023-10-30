#include <iostream>
#include <vector>

#include "function.h"
#include "interpolation.h"

using namespace std;
#define TYPE double

// TODO: Поместить в отдельный класс (Print/Draw)
void PrintTable(std::vector<TYPE> table_x, std::vector<TYPE> table_y, std::vector<TYPE> table_Lag) {
    ushort size = table_x.size();
    if (table_y.size() != size || table_Lag.size() != size)
        throw std::invalid_argument("Tables must have the same size");
    TYPE max_dif = -1;
    printf("%-10c %-10c %-10s %-10s\n", 'x', 'y', "Lagrange", "Deviation");
    for (ushort i = 0; i < size; i++) {
        printf("%.6Lf | %.6Lf | %.6Lf | %.6Lf\n", (long double)table_x[i], (long double)table_y[i],
               (long double)table_Lag[i], (long double)abs(table_Lag[i] - table_y[i]));
        max_dif = max_dif > abs(table_Lag[i] - table_y[i]) ? max_dif : abs(table_Lag[i] - table_y[i]);
    }
    printf("\nThe largest deviation: %.55Lf", (long double)max_dif);
}

// Максимальная погрешность
TYPE maxDeviation(std::vector<TYPE> table_1, std::vector<TYPE> table_2) {
    if (table_1.size() != table_2.size()) throw std::invalid_argument("Tables must have the same size");
    TYPE max_dif = -1;
    for (ushort i = 0; i < table_1.size(); i++) {
        max_dif = max_dif > abs(table_1[i] - table_2[i]) ? max_dif : abs(table_1[i] - table_2[i]);
    }
    return max_dif;
}

int main() {
    TYPE a, b;
    ushort n, n_2;
    a = 0;                  // Начало отрезка
    b = 20;                 // Конец отрезка
    n = 11;                 // Количесво узлов для интерполяции
    n_2 = (n - 1) * 2 - 1;  // Количество узлов для сравнения значений истинной и интерполирующей функций

    // Коэффициент ряда
    auto Q = [](TYPE x, ushort n) -> TYPE {
        return -(2. * n + 1.) * x * x / ((2. * n + 3.) * (2. * n + 3.) * (2. * n + 2.));
    };

    // Первый член ряда
    auto a_0 = [](TYPE x) { return x; };

    // Истинная функция
    auto Si = FunctionSeries<TYPE>(a_0, Q);

    auto x_nodes_n = FunctionSeries<TYPE>::getNodesInRange(a, b, n);
    auto x_nodes_n2 = FunctionSeries<TYPE>::getNodesInRange(a, b, n_2);

    auto y_nodes_n = Si.calculateByNodes(x_nodes_n);
    auto y_nodes_n2 = Si.calculateByNodes(x_nodes_n2);

    // Функция интерполирующая истинную в n узлах
    auto Si_Lag_n = Lagrange<TYPE>(x_nodes_n, y_nodes_n);
    auto y_lagrange_n2 = Si_Lag_n.calculateByNodes(x_nodes_n2);

    PrintTable(x_nodes_n2, y_nodes_n2, y_lagrange_n2);
    // cout << "Max diff = " << maxDeviation(y_nodes_12, y_lagrange_12);
}
