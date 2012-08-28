#ifndef HEALTHMONITOR_H_
#define HEALTHMONITOR_H_

#include "WireController.h"
#include "WireMesh.h"
#include "WireGeometry.h"
#include "Player.h"
#include "ProbeRobot.h"

using namespace Wire;

class HealthMonitor : public Controller
{
public:
	HealthMonitor(Geometry* pGreenHealthBar, Geometry* pRedHealthBar, Player* pPlayer, ProbeRobot* pProbeRobot);

	virtual Bool Update(Double appTime);

private:
	GeometryPtr mspGreenHealthBar;
	GeometryPtr mspRedHealthBar;
	Pointer<Player> mspPlayer;
	Pointer<ProbeRobot> mspProbeRobot;
	Vector3F mGreenHealthBarScale;
	Vector3F mRedHealthBarScale;

};

#endif