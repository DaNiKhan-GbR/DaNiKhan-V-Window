/**
 * @file vec3.cpp
 * 
 * 3D vector type.
 */

#include "vec3.hpp"

#include <cmath>

using dnkvw::Vec3;

Vec3::Vec3()
{
    m_values[0] = 0.0f;
    m_values[1] = 0.0f;
    m_values[2] = 0.0f;
}

Vec3::Vec3(float x, float y, float z)
{
    m_values[0] = x;
    m_values[1] = y;
    m_values[2] = z;
}

Vec3::Vec3(const Vec3& other)
{
    m_values[0] = other.m_values[0];
    m_values[1] = other.m_values[1];
    m_values[2] = other.m_values[2];
}

Vec3& Vec3::operator=(const Vec3& other)
{
    if (this != &other)
    {
        m_values[0] = other.m_values[0];
        m_values[1] = other.m_values[1];
        m_values[2] = other.m_values[2];
    }

    return *this;
}

const Vec3& Vec3::operator+() const
{
    return *this;
}

Vec3 Vec3::operator-() const
{
    return Vec3(-m_values[0], -m_values[1], -m_values[2]);
}

float Vec3::operator[](int i) const
{
    return m_values[i];
}

float& Vec3::operator[](int i)
{
    return m_values[i];
}

Vec3& Vec3::operator+=(const Vec3& v2)
{
    m_values[0] += v2.m_values[0];
    m_values[1] += v2.m_values[1];
    m_values[2] += v2.m_values[2];

    return *this;
}

Vec3& Vec3::operator-=(const Vec3& v2)
{
    m_values[0] -= v2.m_values[0];
    m_values[1] -= v2.m_values[1];
    m_values[2] -= v2.m_values[2];

    return *this;
}

Vec3& Vec3::operator*=(const float t)
{
    m_values[0] *= t;
    m_values[1] *= t;
    m_values[2] *= t;

    return *this;
}

Vec3& Vec3::operator/=(const float t)
{
    // Precalculate the inverste to avoid 2 divisions.
    float ti = 1.0f / t;

    m_values[0] *= ti;
    m_values[1] *= ti;
    m_values[2] *= ti;

    return *this;
}

float Vec3::length()
{
    // Pythagoras
    return std::sqrtf(m_values[0] * m_values[0] + m_values[1] * m_values[1] + m_values[2] * m_values[2]);
}

Vec3 Vec3::norm()
{
    return Vec3(*this) / this->length();
}

Vec3 Vec3::cross(const Vec3& v2)
{
    return Vec3(
        m_values[1] * v2[2] - m_values[2] * v2[1],
        m_values[2] * v2[0] - m_values[0] * v2[2],
        m_values[0] * v2[1] - m_values[1] * v2[0]
    );
}

Vec3 dnkvw::operator+(const Vec3& v1, const Vec3& v2)
{
    return Vec3(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
}

Vec3 dnkvw::operator-(const Vec3& v1, const Vec3& v2)
{
    return Vec3(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
}

float dnkvw::operator*(const Vec3& v1, const Vec3& v2)
{
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

Vec3 dnkvw::operator*(const Vec3& v1, const float s)
{
    return Vec3(v1[0] * s, v1[1] * s, v1[2] * s);
}

Vec3 dnkvw::operator/(const Vec3& v1, const float s)
{
    // Precalculate inverse to avoid 2 divisions.
    float si = 1.0f / s;
    return Vec3(v1[0] * si, v1[1] * si, v1[2] * si);
}

Vec3 dnkvw::operator*(const float s, const Vec3& v1)
{
    // Vector dot product is commutative.
    // Only mirror existing functionality.
    return v1 * s;
}

Vec3 dnkvw::operator/(const float s, const Vec3& v1)
{
    return Vec3(s / v1[0], s / v1[1], s / v1[2]);
}
