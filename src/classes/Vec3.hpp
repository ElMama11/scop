#ifndef VEC3_HPP
#define VEC3_HPP

# include <cmath>

class Vec3 {
	public:
		float x, y, z;

		Vec3() : x(0), y(0), z(0) {}
		Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

		Vec3 normalize() const {
			float len = std::sqrt(x*x + y*y + z*z);
			return Vec3(x / len, y / len, z / len);
		}

		static Vec3 cross(const Vec3& v1, const Vec3& v2) {
			return Vec3(
				v1.y * v2.z - v1.z * v2.y,
				v1.z * v2.x - v1.x * v2.z,
				v1.x * v2.y - v1.y * v2.x
			);
   		}

		// Operator overload
		Vec3 operator+(const Vec3& other) const {
			return Vec3(x + other.x, y + other.y, z + other.z);
		}

		Vec3 operator-(const Vec3& other) const {
			return Vec3(x - other.x, y - other.y, z - other.z);
		}

		Vec3 operator*(float scalar) const {
			return Vec3(x * scalar, y * scalar, z * scalar);
		}

		friend Vec3 operator*(float scalar, const Vec3& vec) {
			return vec * scalar;
		}

		Vec3 operator/(float scalar) const {
			return Vec3(x / scalar, y / scalar, z / scalar);
		}

		friend std::ostream &operator<<(std::ostream& os, const Vec3& vec) {
			os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
			return os;
		}
};

#endif
