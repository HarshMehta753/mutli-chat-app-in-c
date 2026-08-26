#include "../inc/define.h"

int main(int argc, char *argv[])
{
	printf("The socket programming statrt.\r\n");
#ifdef BUILD_CLIENT

	printf("Starting client application...\n");
	client_process_function(argc, argv);

#elif defined(BUILD_SERVER)

	printf("Starting server application...\n");
	server_process_function(argc, argv);

#else

	printf("ERROR: No application type selected.\n");
	return 1;

#endif

	return 0;
}

