#include "HealthMonitor.h"

using namespace Wire;

WIRE_IMPLEMENT_RTTI_NO_NAMESPACE(HealthMonitor, Controller);

//----------------------------------------------------------------------------
HealthMonitor::HealthMonitor(Geometry* pGreenHealthBar, Geometry* pRedHealthBar, 
	Player* pPlayer, ProbeRobot* pProbeRobot)
{
	WIRE_ASSERT(pGreenHealthBar);
	WIRE_ASSERT(pRedHealthBar);
	WIRE_ASSERT(pPlayer);
	WIRE_ASSERT(pProbeRobot);

	mspGreenHealthBar = pGreenHealthBar;
	mspRedHealthBar = pRedHealthBar;
	mspPlayer = pPlayer;
	mspProbeRobot = pProbeRobot;

	mGreenHealthBarScale = mspGreenHealthBar->Local.GetScale();
	mRedHealthBarScale = mspRedHealthBar->Local.GetScale();
}

//----------------------------------------------------------------------------
Bool HealthMonitor::Update(Double appTime)
{
	Float healthRatio = mspProbeRobot->GetHealth() / mspProbeRobot->GetTotalHealth();
	Vector3F healthBarScale = mRedHealthBarScale;
	healthBarScale.X() = MathF::Max(healthBarScale.X() * healthRatio, 0.0001F);
	mspRedHealthBar->Local.SetScale(healthBarScale);

	healthRatio = mspPlayer->GetHealth() / mspPlayer->GetTotalHealth();
	healthBarScale = mGreenHealthBarScale;
	healthBarScale.X() = MathF::Max(healthBarScale.X() * healthRatio, 0.0001F);
	mspGreenHealthBar->Local.SetScale(healthBarScale);

	return true;
}
