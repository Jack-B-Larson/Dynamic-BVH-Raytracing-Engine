#include <utility>
#include "SpanVector.h"

void SpanVector::Push(size_t index, size_t length)
{
	size_t i = 0;
	while (i < spans.size())
	{
		if (spans[i].first + spans[i].second == index)
		{
			index = spans[i].first;
			length += spans[i].second;

			spans[i] = spans.back();
			spans.pop_back();
			continue;
		}
		if (spans[i].first == index + length)
		{
			length += spans[i].second;

			spans[i] = spans.back();
			spans.pop_back();
			continue;
		}
		i++;
	}
	spans.emplace_back(index, length);
}

void SpanVector::PopIndexed(size_t index, size_t length)
{
	for (size_t i = 0; i < spans.size(); i++)
	{
		if (spans[i].first < index + length && spans[i].first + spans[i].second >= index)
		{
			spans[i].first = std::max(spans[i].first, index + length);
			spans[i].second = std::min(spans[i].second, index - spans[i].first);
			if (spans[i].second == 0)
			{
				spans[i] = spans.back();
				spans.pop_back();
			}
		}
	}
}

size_t SpanVector::Pop(size_t length)
{
	for (size_t i = 0; i < spans.size(); i++)
	{
		if (spans[i].second >= length)
		{
			spans[i].second -= length;
			size_t index = spans[i].first;
			spans[i].first += length;
			if (spans[i].second == 0)
			{
				spans[i] = spans.back();
				spans.pop_back();
			}
			return index;
		}
	}
	return -1;
}

const std::vector<std::pair<size_t, size_t>>& const SpanVector::GetSpans()
{
	return spans;
}

void SpanVector::Clear()
{
	spans.clear();
}

bool SpanVector::Contains(size_t index, size_t length) const
{
	for (size_t i = 0; i < spans.size(); i++)
	{
		if (spans[i].first <= index && spans[i].first + spans[i].second >= index + length)
		{
			return true;
		}
	}
	return false;
}
