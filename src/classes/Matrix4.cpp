#include "Matrix4.hpp"

// Initialize to an identity matrix
Matrix4::Matrix4() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j)
				data[i][j] = 1.0f;
			else
				data[i][j] = 0.0f;
		}
	}
}

Matrix4::~Matrix4() {
}

void Matrix4::resetToIdentityMatrix() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j)
				data[i][j] = 1.0f;
			else
				data[i][j] = 0.0f;
		}
	}
}

Matrix4 Matrix4::multiply(const Matrix4 &other) const {
	Matrix4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.data[i][j] = 0;
			for (int k = 0; k < 4; k++)
				result.data[i][j] += data[i][k] * other.data[k][j];
		}
	}
		return result;
}

void Matrix4::translate(const Vec3 &translationVec) {
	Matrix4 translationMatrix;
	translationMatrix.data[0][3] = translationVec.x;
	translationMatrix.data[1][3] = translationVec.y;
	translationMatrix.data[2][3] = translationVec.z;
	*this = multiply(translationMatrix);
}

void Matrix4::scale(Vec3 scaleVec) {
	Matrix4 scaleMatrix;
	scaleMatrix.data[0][0] = scaleVec.x;
	scaleMatrix.data[1][1] = scaleVec.y;
	scaleMatrix.data[2][2] = scaleVec.z;
	*this = multiply(scaleMatrix);
    }

void Matrix4::rotateX(float angle) {
	Matrix4 rotationMatrix;
	float rad = angle * M_PI / 180.0f;
	rotationMatrix.data[1][1] = cos(rad);
	rotationMatrix.data[1][2] = -sin(rad);
	rotationMatrix.data[2][1] = sin(rad);
	rotationMatrix.data[2][2] = cos(rad);
	*this = multiply(rotationMatrix);
}

void Matrix4::rotateY(float angle) {
	Matrix4 rotationMatrix;
	float rad = angle * M_PI / 180.0f;
	rotationMatrix.data[0][0] = cos(rad);
	rotationMatrix.data[0][2] = sin(rad);
	rotationMatrix.data[2][0] = -sin(rad);
	rotationMatrix.data[2][2] = cos(rad);
	*this = multiply(rotationMatrix);
}

void Matrix4::rotateZ(float angle) {
	Matrix4 rotationMatrix;
	float rad = angle * M_PI / 180.0f;
	rotationMatrix.data[0][0] = cos(rad);
	rotationMatrix.data[0][1] = -sin(rad);
	rotationMatrix.data[1][0] = sin(rad);
	rotationMatrix.data[1][1] = cos(rad);
	*this = multiply(rotationMatrix);
}

const float *Matrix4::getValuePtr() const {
	return &data[0][0];
}

void Matrix4::print() const {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			std::cout << std::setw(10) << data[i][j] << " ";
		std::cout << std::endl;
	}
}