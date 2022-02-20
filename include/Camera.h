#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera();
	Camera(const glm::vec3& location, const glm::vec3& worldUp = glm::vec3(0.0f, 1.0f, 0.0f));

	void Translate(glm::vec3 translation);
	void AddRotation(float pitch, float yaw, float roll);
	void AddPitch(float pitch);
	void AddYaw(float yaw);
	void AddRoll(float roll);
	void SetPanSpeed(float newSpeed);
	void SetLookSensitivity(float newSensitivity);
	void SetOrtho(float width, float height, float nearClip, float farClip);
	void SetPerspective(float width, float height, float fovDegrees, float nearClip, float farClip);
	void SetViewSize(const glm::vec2& size);
	void SetViewWidth(float width);
	void SetViewHeight(float height);
	void SetNearClip(float nearClip);
	void SetFarClip(float farClip);
	void SetFovDegrees(float fovDegrees);
	void SetFoVRadians(float fovRadians);

	void ProcessMovementInput(const glm::vec3& input, float deltaTime);
	void ProcessLookInput(const float xoffset, const float yoffset);

	glm::vec3 GetLocation() const;
	glm::vec3 GetUpVector() const;
	glm::vec3 GetRightVector() const;
	glm::vec3 GetForwardVector() const;
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
	float GetPanSpeed() const;
	float GetLookSensitivity() const;
	glm::vec2 GetViewSize() const { return glm::vec2(mViewWidth, mViewHeight); }
	float GetViewWidth() const { return mViewWidth; }
	float GetViewHeight() const { return mViewHeight; }
	float GetNearClip() const { return mNearClip; }
	float GetFarClip() const { return mFarClip; }
	float GetFovDegrees() const { return mFovDegrees; }
	float GetFoVRadians() const { return glm::radians(mFovDegrees); }
	bool IsOrthographic() const { return mUseOrthographicProjection; }

protected:
	void UpdateVectors();
	void UpdateProjectionMatrix();

private:
	float mPitch = 0.0f, mYaw = -90.0f, mRoll = 0.0f;
	glm::vec3 mWorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 mUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 mRight = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 mForward = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 mLocation = glm::vec3(0.0f, 0.0f, 0.0f);

	// Camera controls
	float mPanSpeed = 2.0f;
	float mLookSensitivity = 0.1f;

	// Viewport settings.
	glm::mat4 mProjection = glm::mat4(1.0f);
	float mFovDegrees = 45.0f;
	float mViewHeight = 600.0f;
	float mViewWidth = 800.0f;
	float mNearClip = 0.1f;
	float mFarClip = 100.0f;
	bool mUseOrthographicProjection = false;	
};