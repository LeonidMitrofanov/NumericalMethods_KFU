#include <stdio.h>
#include <vector>

#define EPS 1e-6
class Intorpolate {
   private:
    double startOfRange, endOfRange, step;
    int countOfPoints;

    std::vector<double> table_x;
    std::vector<double> table_y;
    double (*Q)(double x, int n);

    std::vector<double> calculateRange() {
        int i = 0;
        for (double x = startOfRange; x <= endOfRange; x += step) {
            table_x[i] = x;
            table_y[i++] = calculate(x);
        }
        return table_y;
    }

    // TODO: Вывод таблицы на экран

   public:
    Intorpolate(double a, double b, double h, double (*Q)(double, int)) {  // TODO: Провека на a <= b
        this->startOfRange = a;
        this->endOfRange = b;
        this->step = h;
        this->countOfPoints = (int)((b - a) / h) + 1;
        table_x.resize(countOfPoints);
        table_y.resize(countOfPoints);
        this->Q = Q;
        calculateRange();
    }
    Intorpolate(double a, double b, int n, double (*Q)(double, int)) {  // TODO: Провека на a <= b
        this->startOfRange = a;
        this->endOfRange = b;
        this->countOfPoints = n;
        table_x.resize(countOfPoints);
        table_y.resize(countOfPoints);
        this->step = (b - a) / (double)(n - 1);
        this->Q = Q;
        calculateRange();
    }
    // Получени значения функции
    double calculate(double x) {
        double a_n = x;
        double sum = a_n;
        for (int i = 0; a_n > EPS || a_n < -EPS; i++) {
            a_n *= Q(x, i);  // a_n = a_(n+1)
            sum += a_n;
        }
        return sum;
    }
    void printTable() {
        printf("%5c %9c\n", 'x', 'y');
        for (int i = 0; i < countOfPoints; i++) printf("%-6lf | %-6lf\n", table_x[i], table_y[i]);
    }
};

double Q(double x, int n) {  // n - порядковый номер Q
    return -(2. * n + 1.) * x * x /
           ((2. * n + 3.) * (2. * n + 3.) * (2. * n + 2.));  // return Q(n) = a_n+1/a_n
}

int main() {
    double a, b;
    int n;
    // double h;
    a = 0.;
    b = 4.;
    n = 6;  // разбиение на отрезки
    // h = 0.0001;
    Intorpolate Si = Intorpolate(a, b, n, Q);
    //printf("F(x) = %lf\n", Si.calculate(3.2));
    Si.printTable();
}
