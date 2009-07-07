#pragma once
#ifndef WIRESYSTEM_H
#define WIRESYSTEM_H

#ifdef WIRE_WIN
#ifdef WIRE_WII
#undef WIRE_WII
#endif /* WIRE_WII */
#else
#define WIRE_WII
#endif /* WIRE_WIN */

#ifdef WIRE_WII
#include <math.h>
#include <float.h>
#include "Wii/WireOgcDefines.h"
#define WIRE_ASSERT(condition)
#else
#include <assert.h>
#include <cmath>
#define WIRE_ASSERT(condition) assert(condition)
#endif /* WIRE_WII */

#include "WireTypes.h"
#include "WireMemory.h"

#endif

