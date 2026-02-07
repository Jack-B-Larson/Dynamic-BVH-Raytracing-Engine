#include <typeindex>
#include <vector>
#include "BobAndSpin.h"
#include "Camera.h"
#include "ExecutionOrderHandler.h"
#include "MouseLook.h"
#include "Movement.h"
#include "Transform.h"
#include "VoxelObject.h"
namespace ExecutionOrderHandler
{
	void PopulateOrder()
	{
		order.push_back(Transform::GetComponentType());
		order.push_back(BobAndSpin::GetComponentType());
		order.push_back(MouseLook::GetComponentType());
		order.push_back(Movement::GetComponentType());
		order.push_back(Camera::GetComponentType());
		order.push_back(VoxelObject::GetComponentType());
	}

	const std::vector<std::type_index>& const GetOrder()
	{
		return order;
	}
}