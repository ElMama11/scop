#ifndef MATRIX4_HPP
# define MATRIX4_HPP

# include <iostream>
# include <iomanip>
# include "Vec3.hpp"
# include <cmath>

class Matrix4 {

    public:
        float data[4][4];

        Matrix4();
        ~Matrix4();
        Matrix4 multiply(const Matrix4 &other) const;
        void translate(const Vec3 &translationVec);
        void scale(Vec3 scaleVec);
        void rotateX(float angle);
        void rotateY(float angle);
        void rotateZ(float angle);
        const float *getValuePtr() const;
        void print() const;
};
#endif