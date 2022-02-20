#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

enum class ELightType
{
	Spot, Directional, Point
};

class Light
{	
public:
	Light() {}
	Light(vec3 ambient, vec3 diffuse, vec3 specular)
		: mAmbient(ambient), mDiffuse(diffuse), mSpecular(specular)
	{}
	
	vec3 GetAmbientColor() const { return mAmbient; }
	vec3 GetDiffuseColor() const { return mDiffuse; }
	vec3 GetSpecularColor() const { return mSpecular; }
	void SetAmbientColor(const vec3& color) { mAmbient = color; }
	void SetDiffuseColor(const vec3& color) { mDiffuse = color; }
	void SetSpecularColor(const vec3& color) { mSpecular = color; }

	virtual ELightType GetLightType() const = 0;

protected:
	glm::vec3 mAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	glm::vec3 mDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 mSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
};

class DirectionalLight : public Light
{
public:
	DirectionalLight() : Light() {}
	DirectionalLight(vec3 direction, vec3 ambient, vec3 diffuse, vec3 specular)
		: Light(ambient, diffuse, specular), mDirection(direction)
	{}

	vec3 GetDirection() const { return mDirection; }
	void SetDirection(const vec3& direction) { mDirection = direction; }

	virtual ELightType GetLightType() const override { return ELightType::Directional; }

protected:
	glm::vec3 mDirection = glm::vec3(0.0f, -1.0f, 0.0f);
};

struct PointLight : public Light
{
public:
	PointLight() : Light() {}
	PointLight(vec3 position, vec3 ambient, vec3 diffuse, vec3 specular)
		: Light(ambient, diffuse, specular), mPosition(position)
	{}

	vec3 GetPosition() const { return mPosition; }
	float GetConstantComponent() const { return mConstant; }
	float GetLinearComponent() const { return mLinear; }
	float GetQuadraticComponent() const { return mQuadratic; }
	void SetPosition(vec3 position) { mPosition = position; }
	void SetConstantComponent(float constant) { mConstant = constant; }
	void SetLinearComponent(float linear) { mLinear = linear; }
	void SetQuadraticComponent(float quadratic) { mQuadratic = quadratic; }

	virtual ELightType GetLightType() const override { return ELightType::Point; }

protected:
	glm::vec3 mPosition = glm::vec3(0.0f, 0.0f, 0.0f);

	float mConstant = 1.0f;
	float mLinear = 0.09f;
	float mQuadratic = 0.032f;
};

struct SpotLight : public PointLight
{
public:
	SpotLight() : PointLight() {}
	SpotLight(vec3 position, vec3 direction, vec3 ambient, vec3 diffuse, vec3 specular)
		: PointLight(position, ambient, diffuse, specular), mDirection(direction)
	{}
	
	vec3 GetDirection() const { return mDirection; }
	float GetInnerCutoffDegrees() const { return mInnerCutoff; }
	float GetInnerCutoffRadians() const { return radians(mInnerCutoff); }
	float GetOuterCutoffDegrees() const { return mOuterCutoff; }
	float GetOuterCutoffRadians() const { return radians(mOuterCutoff); }
	void SetDirection(const vec3& direction) { mDirection = direction; }
	void SetInnerCutoffDegrees(float cutoff) { mInnerCutoff = cutoff; }
	void SetInnerCutoffRadians(float cutoff) { mInnerCutoff = degrees(cutoff); }
	void SetOuterCutoffDegrees(float cutoff) { mOuterCutoff = cutoff; }
	void SetOuterCutoffRadians(float cutoff) { mOuterCutoff = degrees(cutoff); }

	virtual ELightType GetLightType() const override { return ELightType::Spot; }

protected:
	glm::vec3 mDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	float mInnerCutoff = 12.5f;
	float mOuterCutoff = 12.0f;
};