#ifndef VEC3_HPP
#define VEC3_HPP

class Vec3 {
	public:
		float x, y, z;

		Vec3() : x(0), y(0), z(0) {}
		Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

		friend std::ostream& operator<<(std::ostream& os, const Vec3& vec) {
        	os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
        	return os;
   		}
};

#endif
