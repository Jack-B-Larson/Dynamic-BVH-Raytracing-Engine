#include <glad/glad.h>
#include "BVH.h"
#include "GameObject.h"
#include "Scene.h"
#include "Transform.h"
#include "VoxelObject.h"

void VoxelObject::PreUpdate()
{
	if (objectIndex == -1 && modelIndex != -1)
	{
		BVH* bvh = GetOwner()->GetScene()->GetBVH();
		objectIndex = bvh->RegisterObject(modelIndex);
		Transform* transform = GetOwner()->GetComponentOfType<Transform>();
		bvh->SetTransformationData(objectIndex, transform->GetTransformation(), transform->GetInverseTransformation());
		bvh->SetRenderTransformationData(objectIndex, transform->GetTransformation(), transform->GetInverseTransformation());
	}
	if (objectIndex != -1 && modelIndex == -1)
	{
		GetOwner()->GetScene()->GetBVH()->DeRegisterObject(objectIndex);
		objectIndex = -1;
	}
}

void VoxelObject::PostUpdate()
{
	if (objectIndex != -1)
	{
		Transform* transform = GetOwner()->GetComponentOfType<Transform>();
		if (transform->GetChanged())
		{
			GetOwner()->GetScene()->GetBVH()->SetTransformationData(objectIndex, transform->GetTransformation(), transform->GetInverseTransformation());
		}
	}
}

void VoxelObject::Render()
{
	if (objectIndex != -1)
	{
		Transform* transform = GetOwner()->GetComponentOfType<Transform>();
		if (transform->GetChanged())
		{
			BVH* bvh = GetOwner()->GetScene()->GetBVH();
			bvh->SetRenderTransformationData(objectIndex, transform->GetInterpolatedTransformation(), transform->GetInterpolatedInverseTransformation());
		}
	}
}

void VoxelObject::SetModelIndex(GLuint newModelIndex)
{
	modelIndex = newModelIndex;
	if (objectIndex != -1)
	{
		GetOwner()->GetScene()->GetBVH()->SetObjectModel(objectIndex, modelIndex);
	}
}
