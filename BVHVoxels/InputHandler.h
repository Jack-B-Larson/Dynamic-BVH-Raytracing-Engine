#pragma once
#include <glm/glm.hpp>
#include <unordered_map>
namespace InputHandler
{
	void ResetMouseDelta();
	void FlushKeyStatus();
	void UpdateMousePosition(double x, double y);
	void UpdateKeyStatus(int key, bool down);
	const glm::vec2& const GetRawMouseDelta();
	const glm::vec2& const GetBufferedMouseDelta();
	bool GetKeyDown(int key);
	bool GetKeyHeld(int key);
	bool GetKeyUp(int key);

	namespace
	{
		bool firstMousePosition = true;

		glm::vec2 mousePosition = glm::vec2(0);
		glm::vec2 mouseDelta = glm::vec2(0);
		glm::vec2 accumulatedMouseDelta = glm::vec2(0);
		glm::vec2 bufferedMousePosition = glm::vec2(0);
		glm::vec2 bufferedMouseDelta = glm::vec2(0);
		std::unordered_map<int, unsigned char> keysBuffer = std::unordered_map<int, unsigned char>();
		std::unordered_map<int, unsigned char> keysStatus = std::unordered_map<int, unsigned char>();
	}
}