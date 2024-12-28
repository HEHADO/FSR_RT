#ifndef LONGDOUBLE_H
#define LONGDOUBLE_H
#include <string>
#include <iostream>
#include <stdexcept>

class LongDouble {
public:
    // Конструкторы
    LongDouble();
    LongDouble(double value);
    LongDouble(const std::string &value);

    // Операторы
    LongDouble operator+(const LongDouble &other) const;
    LongDouble operator-(const LongDouble &other) const;
    LongDouble operator*(const LongDouble &other) const;
    LongDouble operator/(const LongDouble &other) const;

    bool operator==(const LongDouble &other) const;
    bool operator!=(const LongDouble &other) const;
    bool operator>(const LongDouble &other) const;
    bool operator<(const LongDouble &other) const;
    bool operator>=(const LongDouble &other) const;
    bool operator<=(const LongDouble &other) const;

    // Методы
    std::string toString() const;
    double toDouble() const;

private:
    long double value;  // Переменная для хранения значения с высокой точностью
};

#endif // LONGDOUBLE_H
