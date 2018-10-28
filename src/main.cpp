#include "pcb.h"

int userMain(int, char*[]);

int main(int argc, char* argv[])
{
	PCB::init();
	int ret = userMain(argc, argv);
	PCB::shutdown();
	return ret;
}

