#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include "Component.h"
class Transform : public Component<Transform>
{
public:
	void SetLocalPosition(const glm::vec3& newPosition, bool instant);
	void SetLocalRotation(const glm::quat& newRotation, bool instant);
	void SetLocalScale(const glm::vec3& newScale, bool instant);

	void RequestUpdate();

	void SetParent(Transform* newParent);

	const glm::vec3& const GetLocalPosition() const;
	const glm::quat& const GetLocalRotation() const;
	const glm::vec3& const GetLocalScale() const;


	const glm::mat4& const GetTransformation();
	const glm::mat4& const GetInverseTransformation();

	glm::vec3 TransformPoint(const glm::vec3& localPoint);
	glm::vec3 TransformDirection(const glm::vec3& localDirection);

	glm::vec3 InverseTransformPoint(const glm::vec3& worldPoint);
	glm::vec3 InverseTransformDirection(const glm::vec3& worldDirection);

	const std::vector<Transform*>& const GetChildren();
	void RemoveChild(Transform* child);
	void AddChild(Transform* child);

	void PreUpdate() override;

	void PushCurrentToLast();

	const glm::mat4& const GetInterpolatedTransformation();
	const glm::mat4& const GetInterpolatedInverseTransformation();

	bool GetChanged() const;

private:
	std::vector<Transform*> children;

	bool changed = true;

	Transform* parent = nullptr;
	glm::vec3 localPosition = glm::vec3(0);
	glm::quat localRotation = glm::identity<glm::quat>();
	glm::vec3 localScale = glm::vec3(1);

	glm::mat4 transformation = glm::identity<glm::mat4>();
	glm::mat4 inverseTransformation = glm::identity<glm::mat4>();

	bool needsUpdate = true;

	glm::vec3 lastPosition = glm::vec3(0);
	glm::quat lastRotation = glm::identity<glm::quat>();
	glm::vec3 lastScale = glm::vec3(1);

	glm::mat4 interpolatedTransformation = glm::identity<glm::mat4>();
	glm::mat4 interpolatedInverseTransformation = glm::identity<glm::mat4>();

	double interpolatedSavedTime = -1;
};

