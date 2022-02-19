#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(const glm::vec3& location, const glm::vec3& worldUp)
	: mLocation(location),
	mWorldUp(worldUp)	
{	
}

void Camera::Translate(glm::vec3 translation)
{
	mLocation += translation;
}

void Camera::AddRotation(float pitch, float yaw, float roll)
{
	mPitch += pitch;
	mYaw += yaw;
	mRoll += roll;
}

void Camera::AddPitch(float pitch)
{
	mPitch += pitch;
}

void Camera::AddYaw(float yaw)
{
	mYaw += yaw;
}

void Camera::AddRoll(float roll)
{
	mRoll += roll;
}

void Camera::SetPanSpeed(float newSpeed)
{
	mPanSpeed = newSpeed;
}

void Camera::SetLookSensitivity(float newSensitivity)
{
	mLookSensitivity = newSensitivity;
}

void Camera::ProcessMovementInput(const glm::vec3& input, float deltaTime)
{
	mLocation += mForward * input.z * mPanSpeed * deltaTime;
	mLocation += mRight * input.x * mPanSpeed * deltaTime;
	mLocation += mUp * input.y * mPanSpeed * deltaTime;
}

void Camera::ProcessLookInput(const float xoffset, const float yoffset)
{
	mYaw += xoffset * mLookSensitivity;
	mPitch += yoffset * mLookSensitivity;

	UpdateVectors();
}

glm::vec3 Camera::GetLocation() const
{
	return mLocation;
}

glm::vec3 Camera::GetUpVector() const
{
	return mUp;
}

glm::vec3 Camera::GetRightVector() const
{
	return mRight;
}

glm::vec3 Camera::GetForwardVector() const
{
	return mForward;
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(mLocation, mLocation + GetForwardVector(), mUp);
}

float Camera::GetPanSpeed() const
{
	return mPanSpeed;
}

float Camera::GetLookSensitivity() const
{
	return mLookSensitivity;
}

void Camera::UpdateVectors()
{
	glm::vec3 forward;
	forward.x = glm::cos(glm::radians(mYaw)) * glm::cos(glm::radians(mPitch));
	forward.y = glm::sin(glm::radians(mPitch));
	forward.z = glm::sin(glm::radians(mYaw)) * glm::cos(glm::radians(mPitch));
	mForward = glm::normalize(forward);
	mRight = glm::normalize(glm::cross(mForward, mWorldUp));
	mUp = glm::normalize(glm::cross(mRight, mForward));
}
