/*
The following is the format used by the Client: executable server_ip_address port_number

argv[0] executable
argv[1] server_ip_address
argv[2] port_number

*/

//includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

//Error Function
void error(const char *err)
{
	perror(err);
	exit(0);
}

int main(int argc, char *argv[])
{
	//Initialize variables
	int sockfd, portnum, i;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer [255];
	
	//If less than 3 args then exit.
	if (argc < 3) 
	{
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(1);	
	}
	
	//Setup portnum
	portnum = atoi(argv[2]);
	
	//Setup Sock
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	//Check for Socket 
	if (sockfd < 0)
	{
	error("ERROR opening socket");
	}	
	
	//Setup Server	
	server = gethostbyname(argv[1]);
		
	//Check for the Server
	if(server == NULL)
	{
		fprintf(stderr , "Error , no such host");
	}	
	
	//Client Server Setup
	bzero((char *) &serv_addr , sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr , (char *) &serv_addr.sin_addr.s_addr , server->h_length);
	serv_addr.sin_port = htons(portnum);
		
	//Check for Connection.	
	if(connect(sockfd , (struct sockaddr *) &serv_addr , sizeof(serv_addr))<0)	
	{
	error("Connection Failed");
	}
	
	//Connect to the Server.
	printf("Connected to the Server. \n");
	while(1)
	{
		
		printf("Please enter a command: ");
		bzero(buffer , 255);
		fgets(buffer , 255 , stdin);
		
		//Write the Command.
		i = write(sockfd , buffer ,strlen(buffer));
		if(i < 0)
			error("Error on writing");
			
		//Read the Response.
		bzero(buffer , 255);
		i = read(sockfd , buffer , 255);
		if (i < 0) 
			error("Error on reading.");
			
		printf("Server: %s" , buffer);			
		int i = strncmp("End" , buffer , 3);
		if(i == 0)
		{
			break;
		}	
	}
	
	//Close the Socket and End the program.
	close(sockfd);
	return 0;			
}
