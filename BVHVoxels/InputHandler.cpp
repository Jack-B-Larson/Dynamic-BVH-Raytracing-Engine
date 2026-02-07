#include <glm/glm.hpp>
#include <utility>
#include "InputHandler.h"
namespace InputHandler
{
	void ResetMouseDelta()
	{
		mouseDelta = accumulatedMouseDelta;
		accumulatedMouseDelta = glm::vec2(0);
	}
	void FlushKeyStatus()
	{
		keysStatus = keysBuffer;
		for (const std::pair<int, unsigned char>& keyStatus : keysBuffer)
		{
			if (keyStatus.second & 8)
			{
				keysBuffer[keyStatus.first] = 9; //set 1001
			}
			else
			{
				keysBuffer[keyStatus.first] = 0;
			}
		}
		bufferedMouseDelta = mousePosition - bufferedMousePosition;
		bufferedMousePosition = mousePosition;
	}
	void UpdateMousePosition(double x, double y)
	{
		glm::vec2 newPosition = glm::vec2(x, y);
		if (firstMousePosition)
		{
			firstMousePosition = false;
			bufferedMousePosition = mousePosition;
		}
		else
		{
			accumulatedMouseDelta += newPosition - mousePosition;
		}
		mousePosition = newPosition;
	}
	void UpdateKeyStatus(int key, bool down)
	{
		if (down)
		{
			keysBuffer[key] |= 11; //or 1011
		}
		else
		{
			keysBuffer[key] |= 5; //or 0101
			keysBuffer[key] &= 7; //and 0111
		}
	}
	const glm::vec2& const GetRawMouseDelta()
	{
		return mouseDelta;
	}
	const glm::vec2& const GetBufferedMouseDelta()
	{
		return bufferedMouseDelta;
	}
	bool GetKeyDown(int key)
	{
		return keysStatus[key] & 2;
	}
	bool GetKeyHeld(int key)
	{
		return keysStatus[key] & 1;
	}
	bool GetKeyUp(int key)
	{
		return keysStatus[key] & 4;
	}
}