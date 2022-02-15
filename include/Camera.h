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

	void ProcessMovementInput(const glm::vec3& input, float deltaTime);
	void ProcessLookInput(const float xoffset, const float yoffset);

	glm::vec3 GetLocation() const;
	glm::vec3 GetUpVector() const;
	glm::vec3 GetRightVector() const;
	glm::vec3 GetForwardVector() const;
	glm::mat4 GetViewMatrix() const;
	float GetPanSpeed() const;
	float GetLookSensitivity() const;

protected:
	void UpdateVectors();

private:
	float mPitch = 0.0f, mYaw = -90.0f, mRoll = 0.0f;
	glm::vec3 mWorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 mUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 mRight = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 mForward = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 mLocation = glm::vec3(0.0f, 0.0f, 0.0f);
	float mPanSpeed = 2.0f;
	float mLookSensitivity = 0.1f;
};