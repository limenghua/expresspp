#include <gtest\gtest.h>
#include <iostream>
#include <stdio.h>

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	int ret = RUN_ALL_TESTS();

	getchar();
	return ret;
}