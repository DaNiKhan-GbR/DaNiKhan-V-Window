//////////////////////////////////////////////////////////////////////////////////////
//  BSD 3-Clause License                                                            //
//                                                                                  //
//  Copyright (c) 2020, Daniel Klintworth and Nicolas Hollmann                      //
//  All rights reserved.                                                            //
//                                                                                  //
//  Redistribution and use in source and binary forms, with or without              //
//  modification, are permitted provided that the following conditions are met:     //
//                                                                                  //
//  * Redistributions of source code must retain the above copyright notice, this   //
//    list of conditions and the following disclaimer.                              //
//                                                                                  //
//  * Redistributions in binary form must reproduce the above copyright notice,     //
//    this list of conditions and the following disclaimer in the documentation     //
//    and/or other materials provided with the distribution.                        //
//                                                                                  //
//  * Neither the name of the copyright holder nor the names of its                 //
//    contributors may be used to endorse or promote products derived from          //
//    this software without specific prior written permission.                      //
//                                                                                  //
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"     //
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE       //
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  //
//  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE    //
//  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL      //
//  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR      //
//  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER      //
//  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   //
//  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE   //
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.            //
//                                                                                  //
//////////////////////////////////////////////////////////////////////////////////////

/**
 * @file
 * @author Nicolas Hollmann
 * @author Daniel Klintworth
 * @brief 3D Vector Type.
 */

#pragma once

namespace dnkvw 
{
    /**
     * This class is a basic 3D float Vector.
     */
    class Vec3
    {
        public:
            /**
             * The default constructor initializes x, y and z to 0.
             */
            Vec3();

            /**
             * This constructor initializes x, y and z with the given values.
             * 
             * @param x - the X value
             * @param y - the Y value
             * @param z - the Z value
             */
            Vec3(float x, float y, float z);

            /**
             * The copy-constructor copies all values from another vector.
             * 
             * @param other the copy source
             */
            Vec3(const Vec3& other);

            /**
             * The copy-assign operator copies all values from another vector.
             * 
             * @param other the copy source
             * @return a reference to this vector
             */
            Vec3& operator=(const Vec3& other);

            /**
             * The destructor does nothing.
             */
            ~Vec3() {}
            
            /**
             * The unary + operator does nothing.
             * 
             * @return a reference to this vector
             */
            const Vec3& operator+() const;

            /**
             * The unary - operator inverts all axis values.
             * 
             * @return the inverted vector
             */
            Vec3 operator-() const;

            /**
             * The index operator returns the value of one axis.
             * 0 = X, 1 = Y, 2 = Z
             * 
             * @param i the index, must be 0, 1 or 2
             * @return the value of this axis
             */
            float operator[](int i) const;

            /**
             * The index reference operator returns a reference to one axis.
             * 0 = X, 1 = Y, 2 = Z
             * 
             * @param i the index, must be 0, 1 or 2
             * @return a modifiable reference to one axis value
             */
            float& operator[](int i);

            /**
             * Component wise vector addition.
             * 
             * @param v2 the other vector
             * @return the vector sum
             */
            Vec3& operator+=(const Vec3& v2);

            /**
             * Component wise vector subtraction.
             * 
             * @param v2 the other vector
             * @return the vector difference
             */
            Vec3& operator-=(const Vec3& v2);

            /**
             * Multiply a vector by a scalar.
             * 
             * @param t the scalar to multiply with
             * @return the scaled vector
             */
            Vec3& operator*=(const float t);

            /**
             * Divide a vector by a scalar.
             * 
             * @param t the scalar to divide by
             * @return the scaled vector
             */
            Vec3& operator/=(const float t);

            /**
             * The length of the vector.
             * 
             * @return the length of the vector
             */
            float length();

            /**
             * Normalizes this vector.
             * 
             * @return the normalized vector
             */
            Vec3 norm();

            /**
             * Calculates the cross product between this and another vector.
             * 
             * @param v2 the other vector
             * @return the cross product
             */
            Vec3 cross(const Vec3& v2);

        private:
            /**
             * The internal axis memory.
             */
            float m_values[3];
    };

    /**
     * Component wise vector addition.
     * 
     * @param v1 the first summand
     * @param v2 the second summand
     * @return the sum
     */
    Vec3 operator+(const Vec3& v1, const Vec3& v2);

    /**
     * Component wise vector subtraction.
     * 
     * @param v1 the minuend
     * @param v2 the subtrahend
     * @return the difference
     */
    Vec3 operator-(const Vec3& v1, const Vec3& v2);

    /**
     * Calculates the dot product between two vectors.
     * 
     * @param v1 the first vector
     * @param v2 the second vector
     * @return the dot product
     */
    float operator*(const Vec3& v1, const Vec3& v2);

    /**
     * Multiply a vector with a scalar.
     * 
     * @param v1 the vector
     * @param s the scalar
     * @return the scaled vector
     */
    Vec3 operator*(const Vec3& v1, const float s);

    /**
     * Divide a vector by a scalar.
     * 
     * @param v1 the vector
     * @param s the scalar to divide by
     * @return the scaled vector
     */
    Vec3 operator/(const Vec3& v1, const float s);

    /**
     * Multiply a scalar with a vector.
     * 
     * @param s the scalar
     * @param v1 the vector
     * @return the scaled vector
     */
    Vec3 operator*(const float s, const Vec3& v1);

    /**
     * Divide a scalar by a vector.
     * 
     * @param s the scalar
     * @param v1 the vector to divide by
     * @return the scaled vector
     */
    Vec3 operator/(const float s, const Vec3& v1);
}
