#pragma once

namespace dnkvw 
{
    class Vec3
    {
        public:
            Vec3();
            Vec3(float x, float y, float z);
            Vec3(const Vec3& other);

            Vec3& operator=(const Vec3& other);

            ~Vec3() {}

            const Vec3& operator+() const;
            Vec3 operator-() const;
            float operator[](int i) const;
            float& operator[](int i);

            Vec3& operator+=(const Vec3& v2);
            Vec3& operator-=(const Vec3& v2);
            Vec3& operator*=(const float t);
            Vec3& operator/=(const float t);

            float length();
            Vec3 norm();
            Vec3 cross(const Vec3& v2);

        private:
            float m_values[3];
    };

    Vec3 operator+(const Vec3& v1, const Vec3& v2);
    Vec3 operator-(const Vec3& v1, const Vec3& v2);
    float operator*(const Vec3& v1, const Vec3& v2);
    Vec3 operator*(const Vec3& v1, const float s);
    Vec3 operator/(const Vec3& v1, const float s);
    Vec3 operator*(const float s, const Vec3& v1);
    Vec3 operator/(const float s, const Vec3& v1);
}
