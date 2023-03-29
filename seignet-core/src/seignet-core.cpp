#include "seignet-core.h"

int main(int argc, char* argv[])
{
	try
	{
		SchoolNet* a = new SchoolNet("1234", "5678");
		a->Go();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}

	return 0;
}
