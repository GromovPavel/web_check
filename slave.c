#include <stdio.h>
#define MAX_ARG 4
#define MIN_ARG 3

void show_help()
{
	puts("-w - check version web server");
	puts("-d - download html code web page");
	puts("-p - check opened port [port]");
	puts("example: ./check -w www.google.com");
	puts("example: ./check -d www.google.com [path]   #path is optional");
	puts("example: ./check -p www.google.com [port]");
}

int main_programm(int argc, char *argv[])
{
	int s;
	if ((argc > MAX_ARG) || (argc < MIN_ARG))
	{
		show_help();
		return 0;
	}
	else
	{
		if ((strcmp(argv[1], "-w") == 0) || (strcmp(argv[1], "-d") == 0))
		{	
			if (argv[3] != NULL)
			{
				s = connet_server(argv[2], argv[1], argv[3]);
			}
			else
			{
				char *path = "index.html";
				s = connet_server(argv[2], argv[1], path);				
			}
			return s;
		}

		if ((strcmp(argv[1], "-p") == 0))
		{
			if (argv[3] != NULL)
			{
				s = connet_server(argv[2], argv[1], argv[3]);
			}
			else
			{
				show_help();
			}
		}

		else
		{
			show_help();
			return 0;
		}
	}
}