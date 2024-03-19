#include "pch.h"
#include "App/App.h"

#ifdef NDEBUG
int wWinMain()
#else
int main(int argc, char* argv[])
#endif
{
	App().Run();

	return 0;
}