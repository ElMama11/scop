#ifndef VEC4_HPP
#define VEC4_HPP

#include "Matrix4.hpp"

class Vec4 {
	public:
		float x, y, z, w;

		Vec4() : x(0), y(0), z(0), w(0) {}
		Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	// 	Vec4 multiplyWithMatrix4(const Matrix4 &tranformationMatrix) const {
	// 		Vec4 result;
	// 		result.x = tranformationMatrix.data[0][0] * x + tranformationMatrix.data[0][1] * y + tranformationMatrix.data[0][2] * z + tranformationMatrix.data[0][3] * w;
	// 		result.y = tranformationMatrix.data[1][0] * x + tranformationMatrix.data[1][1] * y + tranformationMatrix.data[1][2] * z + tranformationMatrix.data[1][3] * w;
	// 		result.z = tranformationMatrix.data[2][0] * x + tranformationMatrix.data[2][1] * y + tranformationMatrix.data[2][2] * z + tranformationMatrix.data[2][3] * w;
	// 		result.w = tranformationMatrix.data[3][0] * x + tranformationMatrix.data[3][1] * y + tranformationMatrix.data[3][2] * z + tranformationMatrix.data[3][3] * w;
	// 		return result;
    // }
};

#endif