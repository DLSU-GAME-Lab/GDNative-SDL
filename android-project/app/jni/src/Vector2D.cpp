#include "Vector2D.h"

Vector2D::Vector2D(float n) {
    this->x = n;
    this->y = n;
}

Vector2D::Vector2D(float x, float y) {
    this->x = x;
    this->y = y;
}

Vector2D::Vector2D(const Vector2D& vector2) {
    this->x = vector2.x;
    this->y = vector2.y;
}

Vector2D Vector2D::Zero() {
    return Vector2D(0.0f);
}

Vector2D Vector2D::One() {
    return Vector2D(1.0f);
}

void Vector2D::Normalize() {
    *this = this->Normalized();
}

Vector2D Vector2D::Normalized() const {
    float mag = this->Magnitude();
    if (mag != 0) return *this / mag;
    return Vector2D::Zero();
}

float Vector2D::Direction() const {
    return atan2f(this->y, this->x);
}

float Vector2D::Magnitude() const {
    return sqrtf((this->x * this->x) + (this->y * this->y));
}

float Vector2D::SqrMagnitude() const
{
    return (this->x * this->x) + (this->y * this->y);
}

float Vector2D::Dot(const Vector2D& left, const Vector2D& right) {
    Vector2D Prod = left * right;
    return Prod.x + Prod.y;
}

Vector2D Vector2D::Lerp(const Vector2D& start, const Vector2D& end, float delta)
{
    Vector2D v;
    v.x = (start.x * (1.0f - delta)) + (end.x * delta);
    v.y = (start.y * (1.0f - delta)) + (end.y * delta);

    return v;
}

Vector2D& Vector2D::operator=(const Vector2D& right)
{
    if (this != &right)
    {
        this->x = right.x;
        this->y = right.y;
    }

    return *this;
}

Vector2D operator-(const Vector2D& right) {
    return Vector2D(-right.x, -right.y);
}

Vector2D operator+(const Vector2D& left, const Vector2D& right) {
    return Vector2D(left.x + right.x, left.y + right.y);
}

Vector2D& operator+=(Vector2D& left, const Vector2D& right) {
    left = left + right;
    return left;
}

Vector2D operator-(const Vector2D& left, const Vector2D& right) {
    return Vector2D(left.x - right.x, left.y - right.y);
}

Vector2D& operator-=(Vector2D& left, const Vector2D& right) {
    left = left - right;
    return left;
}

Vector2D operator*(const Vector2D& left, const Vector2D& right) {
    return Vector2D(left.x * right.x, left.y * right.y);
}

Vector2D& operator*=(Vector2D& left, const Vector2D& right) {
    left = left * right;
    return left;
}

Vector2D operator*(const Vector2D& left, const float& right) {
    return Vector2D(left.x * right, left.y * right);
}

Vector2D operator*(const float& left, const Vector2D& right) {
    return Vector2D(left * right.x, left * right.y);
}

Vector2D& operator*=(Vector2D& left, const float& right) {
    left = left * right;
    return left;
}

Vector2D operator/(const Vector2D& left, const Vector2D& right) {
    return Vector2D(left.x / right.x, left.y / right.y);
}

Vector2D& operator/=(Vector2D& left, const Vector2D& right) {
    left = left / right;
    return left;
}

Vector2D operator/(const Vector2D& left, const float& right) {
    return Vector2D(left.x / right, left.y / right);
}

Vector2D operator/(const float& left, const Vector2D& right) {
    return Vector2D(left / right.x, left / right.y);
}

Vector2D& operator/=(Vector2D& left, const float& right) {
    left = left / right;
    return left;
}

bool operator==(const Vector2D& left, const Vector2D& right) {
    return (left.x == right.x && left.y == right.y);
}

bool operator!=(const Vector2D& left, const Vector2D& right) {
    return (left.x != right.x || left.y != right.y);
}

std::ostream& operator<<(std::ostream& out, const Vector2D& vector2)
{
    out << "(" << vector2.x << ", " << vector2.y << ")";
    return out;
}
