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

void Camera::SetOrtho(float width, float height, float nearClip, float farClip)
{
	mUseOrthographicProjection = true;
	mViewWidth = glm::max(width, 1.0f);
	mViewHeight = glm::max(height, 1.0f);
	mNearClip = nearClip;
	mFarClip = farClip;
	UpdateProjectionMatrix();
}

void Camera::SetPerspective(float width, float height, float fovDegrees, float nearClip, float farClip)
{
	mUseOrthographicProjection = false;
	mViewWidth = glm::max(width, 1.0f);
	mViewHeight = glm::max(height, 1.0f);
	mFovDegrees = glm::max(fovDegrees, 1.0f);
	mNearClip = nearClip;
	mFarClip = farClip;
	UpdateProjectionMatrix();
}

void Camera::SetViewSize(const glm::vec2& size)
{
	mViewWidth = size.x;
	mViewHeight = size.y;
	UpdateProjectionMatrix();
}

void Camera::SetViewWidth(float width)
{
	mViewWidth = width;
	UpdateProjectionMatrix();
}

void Camera::SetViewHeight(float height)
{
	mViewHeight = height;
	UpdateProjectionMatrix();
}

void Camera::SetNearClip(float nearClip)
{
	mNearClip = nearClip;
	UpdateProjectionMatrix();
}

void Camera::SetFarClip(float farClip)
{
	mFarClip = farClip;
	UpdateProjectionMatrix();
}

void Camera::SetFovDegrees(float fovDegrees)
{
	mFovDegrees = fovDegrees;
	UpdateProjectionMatrix();
}

void Camera::SetFoVRadians(float fovRadians)
{
	mFovDegrees = glm::degrees(fovRadians);
	UpdateProjectionMatrix();
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

glm::mat4 Camera::GetProjectionMatrix() const
{
	return mProjection;
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

void Camera::UpdateProjectionMatrix()
{
	mProjection = mUseOrthographicProjection
		? glm::ortho(0.0f, mViewWidth, 0.0f, mViewHeight, mNearClip, mFarClip)
		: glm::perspective(glm::radians(mFovDegrees), mViewWidth / mViewHeight, mNearClip, mFarClip);
}
