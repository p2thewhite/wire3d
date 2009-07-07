#pragma once
#ifndef WIREMAIN_H
#define WIREMAIN_H

namespace Wire
{

class Main
{

public:
	typedef void (*Initializer)();
	static void AddInitializer(Initializer initializer);

};

}

#endif
