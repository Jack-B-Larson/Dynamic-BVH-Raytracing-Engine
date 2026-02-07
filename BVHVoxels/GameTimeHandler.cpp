#include <cmath>
#include "EngineVariables.h"
#include "GameTimeHandler.h"
#include "GLFWHandler.h"
namespace GameTimeHandler
{
	double GetElapsedTime()
	{
		return GLFWHandler::GetTime();
	}
	double GetLastUpdateTime()
	{
		return lastUpdateTime;
	}
	double GetUpdateTime()
	{
		return updateTime;
	}
	double GetLastRenderTime()
	{
		return lastRenderTime;
	}
	double GetRenderTime()
	{
		return renderTime;
	}
	double GetUpdateDeltaTime()
	{
		return updateDeltaTime;
	}
	double GetRenderDeltaTime()
	{
		return renderDeltaTime;
	}
	float GetSubUpdatePercent()
	{
		if (targetUpdateTime == 0)
		{
			return float((renderTime - lastUpdateTime) / updateDeltaTime);
		}
		return float((renderTime - lastUpdateTime) / targetUpdateTime);
	}
	bool ShouldUpdate()
	{
		if (GetElapsedTime() >= lastUpdateTime + targetUpdateTime)
		{
			return true;
		}
		return false;
	}
	void Update()
	{
		double newTime = GetElapsedTime();
		updateDeltaTime = newTime - updateTime;
		updateTime = newTime;
		if (targetUpdateTime > 0)
		{
			lastUpdateTime = floor(updateTime / targetUpdateTime) * targetUpdateTime;
		}
		else
		{
			lastUpdateTime = updateTime;
		}
	}
	bool ShouldRender()
	{
		double newTime = GetElapsedTime();
		if (newTime >= lastRenderTime + targetRenderTime)
		{
			renderDeltaTime = newTime - renderTime;
			renderTime = newTime;
			if (targetRenderTime > 0)
			{
				lastRenderTime = floor(renderTime / targetRenderTime) * targetRenderTime;
			}
			else
			{
				lastRenderTime = renderTime;
			}
			return true;
		}
		return false;
	}
	void TargetUpdateRateVariableCallback()
	{
		float targetUpdateRate = EngineVariables::GetVariableFloat("target_update_rate");
		if (targetUpdateRate <= 0)
		{
			targetUpdateTime = 0;
			return;
		}
		targetUpdateTime = 1 / targetUpdateRate;
	}
	void TargetRenderRateVariableCallback()
	{
		float targetRenderRate = EngineVariables::GetVariableFloat("target_render_rate");
		if (targetRenderRate <= 0)
		{
			targetRenderTime = 0;
			return;
		}
		targetRenderTime = 1 / targetRenderRate;
	}
}