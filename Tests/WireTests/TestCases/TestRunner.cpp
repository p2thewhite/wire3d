#include <gmock/gmock.h>

int main(int argc, char** argv)
{
	::testing::InitGoogleMock(&argc, argv);
	int exit = RUN_ALL_TESTS();
	system("PAUSE");
	return exit;
}