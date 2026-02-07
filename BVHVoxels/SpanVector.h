#pragma once
#include <utility>
#include <vector>
class SpanVector
{
public:
	void Push(size_t index, size_t length);
	void PopIndexed(size_t index, size_t length);
	size_t Pop(size_t length);
	const std::vector<std::pair<size_t, size_t>>& const GetSpans();
	void Clear();
	bool Contains(size_t index, size_t length) const;
private:
	std::vector<std::pair<size_t, size_t>> spans;
};

