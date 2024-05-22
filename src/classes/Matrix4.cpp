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

void Matrix4::rotateX(float angle) {
	float rad = angle * M_PI / 180.0f;
	float cosA = cos(rad);
	float sinA = sin(rad);
	Matrix4 rotationMatrix;
	
	rotationMatrix.data[5] = cosA;
	rotationMatrix.data[6] = -sinA;
	rotationMatrix.data[9] = sinA;
	rotationMatrix.data[10] = cosA;
	*this = multiply(rotationMatrix);
}

void Matrix4::rotateY(float angle) {
	float rad = angle * M_PI / 180.0f;
	float cosA = cos(rad);
	float sinA = sin(rad);
	Matrix4 rotationMatrix;
	rotationMatrix.data[0] = cosA;
	rotationMatrix.data[2] = sinA;
	rotationMatrix.data[8] = -sinA;
	rotationMatrix.data[10] = cosA;
	*this = multiply(rotationMatrix);
}

void Matrix4::rotateZ(float angle) {
	float rad = angle * M_PI / 180.0f;
	float cosA = cos(rad);
	float sinA = sin(rad);
	Matrix4 rotationMatrix;
	rotationMatrix.data[0] = cosA;
	rotationMatrix.data[1] = -sinA;
	rotationMatrix.data[4] = sinA;
	rotationMatrix.data[5] = cosA;
	*this = multiply(rotationMatrix);
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