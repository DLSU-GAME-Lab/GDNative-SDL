#pragma once
#include "math.h"
#include "iostream"

class Vector2D
{
public:
    float x;
    float y;

public:
    Vector2D(float n = 0.0f);
    Vector2D(float x, float y);
    Vector2D(const Vector2D& v);

public:
    void Normalize();
    Vector2D Normalized() const;
    float Direction() const;
    float Magnitude() const;
    float SqrMagnitude() const;

    static Vector2D Zero();
    static Vector2D One();
    static float Dot(const Vector2D& left, const Vector2D& right);
    static Vector2D Lerp(const Vector2D& start, const Vector2D& end, float delta);

    Vector2D& operator=(const Vector2D& right);

    friend Vector2D operator-(const Vector2D& right);
    friend Vector2D operator+(const Vector2D& left, const Vector2D& right);
    friend Vector2D& operator+=(Vector2D& left, const Vector2D& right);
    friend Vector2D operator-(const Vector2D& left, const Vector2D& right);
    friend Vector2D& operator-=(Vector2D& left, const Vector2D& right);
    friend Vector2D operator*(const Vector2D& left, const Vector2D& right);
    friend Vector2D& operator*=(Vector2D& left, const Vector2D& right);
    friend Vector2D operator*(const Vector2D& left, const float& right);
    friend Vector2D operator*(const float& left, const Vector2D& right);
    friend Vector2D& operator*=(Vector2D& left, const float& right);
    friend Vector2D operator/(const Vector2D& left, const Vector2D& right);
    friend Vector2D& operator/=(Vector2D& left, const Vector2D& right);
    friend Vector2D operator/(const Vector2D& left, const float& right);
    friend Vector2D operator/(const float& left, const Vector2D& right);
    friend Vector2D& operator/=(Vector2D& left, const float& right);
    friend bool operator==(const Vector2D& left, const Vector2D& right);
    friend bool operator!=(const Vector2D& left, const Vector2D& right);

    friend std::ostream& operator<<(std::ostream& out, const Vector2D& vector2);
};