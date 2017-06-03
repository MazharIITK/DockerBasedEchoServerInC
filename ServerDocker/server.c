#include<sys/socket.h>  
#include<sys/types.h>
#include<pthread.h> 
#include<resolv.h>  
#include<string.h>   
#include<unistd.h>  
#include<stdlib.h>
#include<stdio.h>

// A thread is created for each accepted client connection  
void *thread_server(void *vargp)  
{  
   	int client_sd = (int)vargp; // get client socket descriptor from arguments passed to the thread  
   	char buffer[5];  
   	int bytes = 0;
   	
      	while(1)  
      	{  
           	//receive data from client  
           	memset(&buffer,'\0',sizeof(buffer));
           	  
           	bytes = read(client_sd, buffer, 5);  
           	if(bytes == 0)  
           	{
           		printf("Error Reading: No message from client\n");  
           	}
           	  
           	else  
           	{   
                	write(client_sd, buffer, strlen(buffer)+1);                
                	fputs(buffer, stdout);       
           	}
           	  
             fflush(stdout);  
      	}      
}  
 

//main
int main(int argc, char **argv)  
{  
	int client_sd;  
      	char buffer[5];  
      	int sd = 0 ;  
      	struct sockaddr_in server_sd;  
	int port = 5010 ;
      	signal(SIGPIPE,SIG_IGN);  //This takes care of the issue that server doesn't close if one of the clients closes
      	
      	// create a socket  
      	sd = socket(AF_INET, SOCK_STREAM, 0);  
      	
      	printf("Server started\n");  
      	memset(&server_sd, 0, sizeof(server_sd));  
      	
      	// set socket variables  
      	server_sd.sin_family = AF_INET;  
      	server_sd.sin_port = htons(port);
      	server_sd.sin_addr.s_addr = INADDR_ANY;  
      	
      	// bind socket to the port  
      	bind(sd, (struct sockaddr*)&server_sd,sizeof(server_sd));  
      	
      	// start listening at the given port for new connection requests  
      	listen(sd, 3);  
      	printf("Listening on port %d\n", port);
      	
      	while(1)  
      	{  
        	client_sd = accept(sd, (struct sockaddr*)NULL ,NULL);  
           
           	if(client_sd > 0)  
           	{  
                	pthread_t tid;  
                	
                	// pass client sd as a thread parameter  
                	pthread_create(&tid, NULL, thread_server, (void *)client_sd);   
           	}  
      	}  
      	close(client_sd);   

return 0;  
}
