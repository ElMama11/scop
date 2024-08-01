#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Vec3.hpp"
#include "Matrix4.hpp"
#include <cmath>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	UP,
	BOTTOM,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class Camera {
	public:
		// camera Attributes
		Vec3 Position;
		Vec3 Front;
		Vec3 Up;
		Vec3 Right;
		Vec3 WorldUp;
		// euler Angles
		float Yaw;
		float Pitch;
		// camera options
		float MovementSpeed;
		float MouseSensitivity;
		float Zoom;

		// constructors
		Camera(Vec3 position = Vec3(0.0f, 0.0f, 0.0f), Vec3 up = Vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
		Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
		~Camera();

		Matrix4 GetViewMatrix();
		void ProcessKeyboard(Camera_Movement direction, float deltaTime);
		void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
		void ProcessMouseScroll(float yoffset);

	private:
		// calculates the front vector from the Camera's (updated) Euler Angles
		void updateCameraVectors();
};

#endif