#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include "GameTimeHandler.h"
#include "Transform.h"

void Transform::SetLocalPosition(const glm::vec3& newPosition, bool instant)
{
	if (localPosition == newPosition)
	{
		return;
	}

	//update position
	localPosition = newPosition;

	if (instant)
	{
		lastPosition = localPosition;
	}

	//set to require update
	RequestUpdate();
}

void Transform::SetLocalRotation(const glm::quat& newRotation, bool instant)
{
	if (localRotation == newRotation)
	{
		return;
	}

	//update rotation
	localRotation = newRotation;


	if (instant)
	{
		lastRotation = localRotation;
	}

	//set to require update
	RequestUpdate();
}

void Transform::SetLocalScale(const glm::vec3& newScale, bool instant)
{
	if (localScale == newScale)
	{
		return;
	}

	//update scale
	localScale = newScale;


	if (instant)
	{
		lastScale = localScale;
	}

	//set to require update
	RequestUpdate();
}

void Transform::RequestUpdate()
{
	//recursively request updates in children
	for (Transform* child : children)
	{
		child->RequestUpdate();
	}

	//set current transform to need update
	needsUpdate = true;
	changed = true;
}

void Transform::SetParent(Transform* newParent)
{
	//if old parent exists
	if (parent != nullptr)
	{
		//tell parent to remove this child
		parent->RemoveChild(this);
	}

	//update parent
	parent = newParent;

	//add child to new parent if exists
	if (parent != nullptr)
	{
		parent->AddChild(this);
	}

	//set to require update
	RequestUpdate();
}

const glm::vec3& const Transform::GetLocalPosition() const
{
	//return position
	return localPosition;
}

const glm::quat& const Transform::GetLocalRotation() const
{
	//return rotation
	return localRotation;
}

const glm::vec3& const Transform::GetLocalScale() const
{
	//return scale
	return localScale;
}

const glm::mat4& const Transform::GetTransformation()
{
	//if needs update, update
	if (needsUpdate)
	{
		//set to no longer need update
		needsUpdate = false;

		//set transformation by values
		transformation = glm::scale(glm::translate(glm::identity<glm::mat4>(), localPosition) * glm::mat4(localRotation), localScale);

		//if parent is not null, recursively update parent transforms as needed and apply downwards
		if (parent != nullptr)
		{
			transformation = parent->GetTransformation() * transformation;
		}
		inverseTransformation = glm::inverse(GetTransformation());
	}

	//return
	return transformation;
}

const glm::mat4& const Transform::GetInverseTransformation()
{
	//if needs update, update
	if (needsUpdate)
	{
		GetTransformation();
	}

	//return
	return inverseTransformation;
}

glm::vec3 Transform::TransformPoint(const glm::vec3& localPoint)
{
	//apply matrix and return
	return glm::vec3(GetTransformation() * glm::vec4(localPoint, 1));
}

glm::vec3 Transform::TransformDirection(const glm::vec3& localDirection)
{
	//apply matrix and return
	return glm::vec3(GetTransformation() * glm::vec4(localDirection, 0));
}

glm::vec3 Transform::InverseTransformPoint(const glm::vec3& worldPoint)
{
	//apply matrix and return
	return glm::vec3(GetInverseTransformation() * glm::vec4(worldPoint, 1));
}

glm::vec3 Transform::InverseTransformDirection(const glm::vec3& worldDirection)
{
	//apply matrix and return
	return glm::vec3(GetInverseTransformation() * glm::vec4(worldDirection, 0));
}

const std::vector<Transform*>& const Transform::GetChildren()
{
	//return children
	return children;
}

void Transform::RemoveChild(Transform* child)
{
	//swap child with back of list
	size_t index = std::find(children.begin(), children.end(), child) - children.begin();
	children[index] = children.back();
	//pop back
	children.pop_back();
}

void Transform::AddChild(Transform* child)
{
	//add to vector
	children.push_back(child);
}

void Transform::PreUpdate()
{
	PushCurrentToLast();
	changed = false;
}

void Transform::PushCurrentToLast()
{
	lastPosition = localPosition;
	lastRotation = localRotation;
	lastScale = localScale;
}

const glm::mat4& const Transform::GetInterpolatedTransformation()
{
	float subUpdate = GameTimeHandler::GetSubUpdatePercent();

	//if needs update, update
	if (interpolatedSavedTime != GameTimeHandler::GetRenderTime())
	{
		//set to no longer need update
		interpolatedSavedTime = GameTimeHandler::GetRenderTime();

		//set transformation by values
		interpolatedTransformation = glm::scale(glm::translate(glm::identity<glm::mat4>(),
															   glm::mix(lastPosition, localPosition, subUpdate))
												* glm::mat4(glm::slerp(lastRotation, localRotation, subUpdate)),
												glm::mix(lastScale, localScale, subUpdate));

		//if parent is not null, recursively update parent transforms as needed and apply downwards
		if (parent != nullptr)
		{
			interpolatedTransformation = parent->GetInterpolatedTransformation() * interpolatedTransformation;
		}
		interpolatedInverseTransformation = glm::inverse(GetInterpolatedTransformation());
	}

	//return
	return interpolatedTransformation;
}

const glm::mat4& const Transform::GetInterpolatedInverseTransformation()
{
	//if needs update, update
	if (interpolatedSavedTime != GameTimeHandler::GetRenderTime())
	{
		GetInterpolatedTransformation();
	}

	//return
	return interpolatedInverseTransformation;
}

bool Transform::GetChanged() const
{
	return changed;
}
