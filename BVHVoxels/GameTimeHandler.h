#pragma once
namespace GameTimeHandler
{
	double GetElapsedTime();
	double GetLastUpdateTime();
	double GetUpdateTime();
	double GetLastRenderTime();
	double GetRenderTime();
	double GetUpdateDeltaTime();
	double GetRenderDeltaTime();

	float GetSubUpdatePercent();

	bool ShouldUpdate();
	void Update();
	bool ShouldRender();

	void TargetUpdateRateVariableCallback();
	void TargetRenderRateVariableCallback();
	namespace
	{
		double targetUpdateTime = 1;
		double targetRenderTime = 1;


		double subUpdatePercent = 0;
		double lastUpdateTime = 0;
		double updateTime = 0;
		double lastRenderTime = 0;
		double renderTime = 0;
		double updateDeltaTime = 0;
		double renderDeltaTime = 0;
	}
}