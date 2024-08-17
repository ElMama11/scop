#include "Matrix4.hpp"

// Initialize to an identity matrix
Matrix4::Matrix4() {
	for (int i = 0; i < 16; i++) {
		if (i % 5 == 0)
			data[i] = 1.0f;
		else
			data[i] = 0.0f;
	}
}

Matrix4::~Matrix4() {
}

void Matrix4::resetToIdentityMatrix() {
	for (int i = 0; i < 16; i++) {
		if (i % 5 == 0)
			data[i] = 1.0f;
		else
			data[i] = 0.0f;
	}
}

Matrix4 Matrix4::multiply(const Matrix4 &other) const {
	Matrix4 result;
	for (int col = 0; col < 4; col++) {
		for (int row = 0; row < 4; row++) {
			result.data[col * 4 + row] = 0.0f;
			for (int k = 0; k < 4; k++)
				result.data[col * 4 + row] += data[k * 4 + row] * other.data[col * 4 + k];
		}
	}
	return result;
}

void Matrix4::translate(const Vec3 &translationVec) {
	Matrix4 translationMatrix;
	translationMatrix.data[12] = translationVec.x;
	translationMatrix.data[13]= translationVec.y;
	translationMatrix.data[14] = translationVec.z;
	*this = multiply(translationMatrix);
}

void Matrix4::scale(Vec3 scaleVec) {
	Matrix4 scaleMatrix;
	scaleMatrix.data[0] = scaleVec.x;
	scaleMatrix.data[5] = scaleVec.y;
	scaleMatrix.data[10] = scaleVec.z;
	*this = multiply(scaleMatrix);
}

void Matrix4::rotate(float angle, float x, float y, float z) {
		float rad = angle * M_PI / 180.0f;
		float cosA = cos(rad);
		float sinA = sin(rad);
		float len = sqrt(x*x + y*y + z*z);
		if (len != 1.0f) {
			float invLen = 1.0f / len;
			x *= invLen;
			y *= invLen;
			z *= invLen;
		}
		Matrix4 rotation;
		rotation.data[0] = cosA + x*x*(1-cosA);
		rotation.data[1] = x*y*(1-cosA) - z*sinA;
		rotation.data[2] = x*z*(1-cosA) + y*sinA;

		rotation.data[4] = y*x*(1-cosA) + z*sinA;
		rotation.data[5] = cosA + y*y*(1-cosA);
		rotation.data[6] = y*z*(1-cosA) - x*sinA;

		rotation.data[8] = z*x*(1-cosA) - y*sinA;
		rotation.data[9] = z*y*(1-cosA) + x*sinA;
		rotation.data[10] = cosA + z*z*(1-cosA);
		*this = this->multiply(rotation);
}

void Matrix4::perspective(float fovY, float aspect, float zNear, float zFar) {
	float tanHalfFovy = tan(fovY / 2.0f);
	for (int i = 0; i < 16; ++i)
		data[i] = 0.0f;
	data[0] = 1.0f / (aspect * tanHalfFovy);
	data[5] = 1.0f / (tanHalfFovy);
	data[10] = -(zFar + zNear) / (zFar - zNear);
	data[11] = -1.0f;
	data[14] = -(2.0f * zFar * zNear) / (zFar - zNear);
}

Matrix4 Matrix4::lookAt(const Vec3 &cameraPos, const Vec3 &center, const Vec3 &up) {
		Vec3 f = (center - cameraPos).normalize();
		Vec3 u = up.normalize();
		Vec3 s = Vec3::cross(f, u).normalize();
		u = Vec3::cross(s, f);
		Matrix4 result;
		result.data[0] = s.x;
		result.data[4] = s.y;
		result.data[8] = s.z;
		result.data[1] = u.x;
		result.data[5] = u.y;
		result.data[9] = u.z;
		result.data[2] = -f.x;
		result.data[6] = -f.y;
		result.data[10] = -f.z;
		result.data[12] = -s.x * cameraPos.x - s.y * cameraPos.y - s.z * cameraPos.z;
		result.data[13] = -u.x * cameraPos.x - u.y * cameraPos.y - u.z * cameraPos.z;
		result.data[14] = f.x * cameraPos.x + f.y * cameraPos.y + f.z * cameraPos.z;
		return result;
}

const float *Matrix4::getValuePtr() const {
	return &data[0];
}

void Matrix4::print() const {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			std::cout << std::setw(10) << data[j * 4 + i] << " ";
		std::cout << std::endl;
	}
}