#include "longdouble.h"


// Конструкторы
LongDouble::LongDouble() : value(0.0) {}

LongDouble::LongDouble(double value) : value(value) {}

LongDouble::LongDouble(const std::string &value) {
    this->value = std::stold(value); // Преобразуем строку в long double
}

// Операторы
LongDouble LongDouble::operator+(const LongDouble &other) const {
    return LongDouble(value + other.value);
}

LongDouble LongDouble::operator-(const LongDouble &other) const {
    return LongDouble(value - other.value);
}

LongDouble LongDouble::operator*(const LongDouble &other) const {
    return LongDouble(value * other.value);
}

LongDouble LongDouble::operator/(const LongDouble &other) const {
    if (other.value == 0.0) {
        throw std::overflow_error("Division by zero");
    }
    return LongDouble(value / other.value);
}

bool LongDouble::operator==(const LongDouble &other) const {
    return value == other.value;
}

bool LongDouble::operator!=(const LongDouble &other) const {
    return value != other.value;
}

bool LongDouble::operator>(const LongDouble &other) const {
    return value > other.value;
}

bool LongDouble::operator<(const LongDouble &other) const {
    return value < other.value;
}

bool LongDouble::operator>=(const LongDouble &other) const {
    return value >= other.value;
}

bool LongDouble::operator<=(const LongDouble &other) const {
    return value <= other.value;
}

// Методы
std::string LongDouble::toString() const {
    return std::to_string(value);
}

double LongDouble::toDouble() const {
    return static_cast<double>(value);
}
