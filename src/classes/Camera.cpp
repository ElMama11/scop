#include "Camera.hpp"

// constructor with vectors
Camera::Camera(Vec3 position, Vec3 up, float yaw, float pitch) : Front(Vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}

// constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)  : Front(Vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
	Position = Vec3(posX, posY, posZ);
	WorldUp = Vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}

Camera::~Camera() {
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
Matrix4 Camera::GetViewMatrix() {
	Matrix4 view;
	return view.lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Position = Position + Front * velocity;
	if (direction == BACKWARD)
		Position = Position - Front * velocity;
	if (direction == LEFT)
		Position = Position - Right * velocity;
	if (direction == RIGHT)
		Position = Position + Right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw   += xoffset;
	Pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch) {
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}
	updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
	Zoom -= (float)yoffset;
	if (Zoom < 1.0f)
		Zoom = 1.0f;
	if (Zoom > 45.0f)
		Zoom = 45.0f;
}

// calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors() {
	// calculate the new Front vector
	Vec3 front;
	front.x = cos(Vec3::degreesToRadians(Yaw)) * cos(Vec3::degreesToRadians(Pitch));
	front.y = sin(Vec3::degreesToRadians(Pitch));
	front.z = sin(Vec3::degreesToRadians(Yaw)) * cos(Vec3::degreesToRadians(Pitch));
	Front = front.normalize();
	// also re-calculate the Right and Up vector
	Right = Vec3::cross(Front, WorldUp);  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Right.normalize();
	Up = Vec3::cross(Right, Front);
	Up.normalize();
}