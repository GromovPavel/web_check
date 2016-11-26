#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 80
#define FAIL -1
#define HEAD "HEAD / HTTP/1.0\n\n"
#define GET "GET / HTTP/1.0\n\n"
#define EOL "\r\n"
#define EOL_SIZE 2

int main_programm(int argc, char *argv[]);
void show_help();
int connet_server(char *server, char *flag, char *path);
int send_string(int sock_des, char *buffer);
int recv_line(int sock_des, char *buffer);
int save_to_file(char *buffer, char *path);
struct in_addr *resolve_hostname(char * server);


struct hostent *host_info;  //Here we created strucure for retured value function gethostbyname()
struct in_addr *address; 
struct sockaddr_in remote_server;

int main(int argc, char *argv[])
{
	struct sockaddr_in remote_server;
	
	main_programm(argc, argv);

	return 0;
}

struct in_addr *resolve_hostname(char *server)
{
	if ((host_info = gethostbyname(server)) == NULL)
	{
		herror("gethostbyname");
		address = NULL;
	}
	else
	{
		address = (struct in_addr *) (host_info->h_addr);	
	}						
	return address;
}

int connet_server(char * server, char *flag, char *path)
{
	int sock_des;
	address = resolve_hostname(server);
	if (address == NULL)
	{
		return 1;
	}
	sock_des = socket(AF_INET, SOCK_STREAM, 0);
	remote_server.sin_family = AF_INET;
	remote_server.sin_port = htons(PORT);
	remote_server.sin_addr.s_addr = address->s_addr;
	struct sockaddr *p = (struct sockaddr *)&remote_server;

	if (connect(sock_des, p, sizeof(remote_server)) == FAIL)
	{
		printf("Error... connect to %s fail\n", server);
	}

	if (strcmp(flag, "-w") == 0)
	{
		send_string(sock_des, HEAD);
		unsigned char buffer[8000];

		while(recv_line(sock_des, buffer))
		{
		if (strncasecmp(buffer, "Server:", 7) == 0)
		{
			printf("%s",buffer+8);
			exit(0);
		}
		}
	}

	if (strcmp(flag, "-d") == 0)
	{
		send_string(sock_des, GET);
		unsigned char buffer[26684];
		while(recv_line(sock_des, buffer))
		{
			
		}
		save_to_file(buffer, path);
	}				
}

int send_string(int sock_des, char * buffer)
{
	int send_bytes, bytes_to_send;
	bytes_to_send = strlen(buffer);
		if((send_bytes = send(sock_des, buffer, bytes_to_send, 0)) == FAIL)
		{
			printf("Error... Cannot send data\n");
			return 1;
		}
	return 0;
}

int recv_line(int sock_des, char* destr_buffer)
{
	char *prt;
	int EOL_matched = 0;
	prt = destr_buffer;
	while(recv(sock_des, prt, 1, 0) == 1)
	{
		if (*prt == EOL[EOL_matched])
		{
			EOL_matched++;
			if (EOL_matched == EOL_SIZE)
			{
			*(prt + 1) = '\0';
			return strlen(destr_buffer);
			}
		}
		else
		{
			EOL_matched = 0;
			prt++;
		}
	}
	return 0;
}

int save_to_file(char *buffer, char *path)
{
	FILE *file;

	if ((file = fopen(path, "w")) == NULL)
	{
		printf("Error...cannot open file\n");
	}
	
	if ((fputs(buffer, file)) == 0)
	{
		printf("Error...cannot write in file\n");
	}
	
	fclose(file);
}