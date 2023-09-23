//includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

//Error Function
void error(const char *err)
{
	perror(err);
	exit(1);
}

//Main
int main (int argc, char *argv[])
{
	//Initialize variables
	int sockfd , newsockfd , portnum , i;
	
	//As instructed Port Number is hard coded to 12 + (*** *** 255) ID
	portnum = 12255;
	
	char buffer[255];
	
	struct sockaddr_in serv_addr , cli_addr;
	socklen_t clilen;
	
	//Build Socket
	sockfd = socket(AF_INET , SOCK_STREAM , 0);
	if (sockfd < 0)
	{
		error("Error opening Socket.");
	}	
	
	//Client Server Setup
	bzero((char *) &serv_addr , sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portnum);
	
	//Check for failed Binding
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		close(newsockfd);
		close(sockfd);
		error("Binding Failed. ");
	}
	
	//Listen for Client
	listen(sockfd , 5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd , (struct sockaddr *) &cli_addr , &clilen);
	
	if(newsockfd < 0)
	{
		close(newsockfd);
		close(sockfd);
		error("Error on Accept");
	}
	
	//Accept Inputs from Client
	while(1)
	{
		
		bzero(buffer , 255);
		i = read(newsockfd , buffer , 255);
		if(i < 0)
		{
			close(newsockfd);
			close(sockfd);
			error("Error on reading.");
		}
	
		//Run Command!
		char cmd[100] = "", **p;
		strcat(cmd, buffer);
		
		
		//Changed to send results
		FILE* command = popen(cmd, "r");
		if (!command) return 1;
		
		char bffer[256];
  	  	size_t chread;
   		
   		size_t comalloc = 256;
    		size_t comlen   = 0;
    		char  *comout   = malloc(comalloc);
 
    
    		while ((chread = fread(buffer, 1, sizeof(buffer), command)) != 0) {
        		if (comlen + chread >= comalloc) {
          		  comalloc *= 2;
          		  comout = realloc(comout, comalloc);
       		 }
      		  memmove(comout + comlen, buffer, chread);
       	 comlen += chread;
    		}
		
		

		pclose(command);
		
		int i = strncmp("End", buffer , 3);
		if(i == 0)
		{
			//char* send = "End";
			i = write(newsockfd , comout , strlen(comout));
			if (i < 0)
			{
				close(newsockfd);
				close(sockfd);
				error ("Error on Writing.");
				
			}
			break;
		} else {
			//char* send = "Done \n";
			i = write(newsockfd , comout , strlen(comout));
			if (i < 0)
			{
				close(newsockfd);
				close(sockfd);
				error ("Error on Writing.");
			}
		}	
	}
	
	//Close Ports and End the Program.
	close(newsockfd);
	close(sockfd);
	return 0;
}
