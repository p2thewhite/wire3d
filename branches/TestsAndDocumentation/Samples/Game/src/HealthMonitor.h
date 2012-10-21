#ifndef HEALTHMONITOR_H
#define HEALTHMONITOR_H

#include "WireController.h"
#include "WireGeometry.h"
#include "WireMesh.h"
#include "Player.h"
#include "ProbeRobot.h"

class HealthMonitor : public Wire::Controller
{
	WIRE_DECLARE_RTTI;

public:
	HealthMonitor(Wire::Geometry* pGreenHealthBar, Wire::Geometry* pRedHealthBar,
		Player* pPlayer, ProbeRobot* pProbeRobot);

	virtual Bool Update(Double appTime);

private:
	Wire::GeometryPtr mspGreenHealthBar;
	Wire::GeometryPtr mspRedHealthBar;
	Wire::Pointer<Player> mspPlayer;
	Wire::Pointer<ProbeRobot> mspProbeRobot;
	Wire::Vector3F mGreenHealthBarScale;
	Wire::Vector3F mRedHealthBarScale;
};

#endif
