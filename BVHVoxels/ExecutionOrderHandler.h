#pragma once
#include <typeindex>
#include <vector>
namespace ExecutionOrderHandler
{
	void PopulateOrder();
	const std::vector<std::type_index>& const GetOrder();
	namespace
	{
		std::vector<std::type_index> order = std::vector<std::type_index>();
	}
}