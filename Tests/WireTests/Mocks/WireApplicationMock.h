#pragma once
#ifndef WIREAPPLICATIONMOCK_H
#define WIREAPPLICATIONMOCK_H

#include <WireApplication.h>

#include <gmock/gmock.h>

namespace Wire
{

class ApplicationMock : public Application
{
public:
	static const UInt WIDTH = 800;
	static const UInt HEIGHT = 600;

	ApplicationMock()
		: Application(ColorRGBA::BLACK, "Mock Application", 0, 0, WIDTH, HEIGHT, false, false)
	{
	}

	virtual ~ApplicationMock()
	{
	}

	MOCK_METHOD0(GetWidth, UInt());
	MOCK_METHOD0(GetHeight, UInt());
	MOCK_METHOD2(Main, Int(Int argumentQuantity, Char* arguments[]));

};

}

#endif