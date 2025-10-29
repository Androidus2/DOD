#include "Vector2.h"
#include <cmath>

float Vector2::GetSqrMagnitude() const {
    return x * x + y * y;
}
float Vector2::GetMagnitude() const {
    return std::sqrt(GetSqrMagnitude());
}

void Vector2::Normalize() {
    float length = GetMagnitude();
    if (length == 0.0f)
        return;
    *this /= length;
}
const Vector2& Vector2::GetNormalized() const {
    Vector2 copy(*this);
    copy.Normalize();
    return copy;
}

Vector2& Vector2::operator+=(const Vector2& other) {
    x += other.x;
    y += other.y;
    return *this;
}
Vector2& Vector2::operator-=(const Vector2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}
Vector2& Vector2::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}
Vector2& Vector2::operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
}

Vector2 Vector2::operator+(const Vector2& other) const {
    return Vector2(*this) += other;
}
Vector2 Vector2::operator-(const Vector2& other) const {
    return Vector2(*this) -= other;
}
Vector2 Vector2::operator*(float scalar) const {
    return Vector2(*this) *= scalar;
}
Vector2 Vector2::operator/(float scalar) const {
    return Vector2(*this) /= scalar;
}

Vector2 operator*(float scalar, const Vector2& v) {
    return v * scalar;
}