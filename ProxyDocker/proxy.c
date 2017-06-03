#include <sys/socket.h> 
#include <sys/types.h>
#include<arpa/inet.h> 
#include <pthread.h>
#include <resolv.h>  
#include <string.h>  
#include <stdlib.h>  
#include<unistd.h>  
#include<netdb.h>    
#include<stdio.h>
 
void hostname_to_ip(char * , char *);

/* 
   A structure to maintain client socket descriptor, and server's ip address and port address  
   client will establish connection to server using given IP and port   
*/
struct serverInfo  
{  
	int client_sd;  
      	char ip[100];  
      	int port ;  
};  

// A thread for each client request  
void *runSocket(void *vargp)  
{  
	struct serverInfo *info = (struct serverInfo *)vargp;  
      	char buffer[5];  
      	int bytes = 0, server_sd = 0;
      	  
      	struct sockaddr_in server_addr;  
      	
      	// create a socket  
      	server_sd = socket(AF_INET, SOCK_STREAM, 0);  
      	if(server_sd < 0)  
      	{  
           	printf("server socket not created\n");  
      	}  
      	printf("server socket created\n");       
      	memset(&server_addr, 0, sizeof(server_sd));  
      	
      	// set socket variables  
      	server_addr.sin_family = AF_INET;  
      	server_addr.sin_port = htons(info->port);  
      	server_addr.sin_addr.s_addr = inet_addr(info->ip);  
      	
      	//connect to main server from this proxy server  
      	if((connect(server_sd, (struct sockaddr *)&server_addr, sizeof(server_addr)))<0)  
      	{  
           	printf("server connection not established\n");
           	return NULL;  
      	}  
      	
      	printf("server socket connected\n");  
      	
      	while(1)  
      	{  
           	//receive data from client  
           	memset(&buffer, '\0', sizeof(buffer));  
           	bytes = read(info->client_sd, buffer, 5);  
           	if(bytes <= 0)  
           	{
           		printf("Nothing recieved from client\n");
           	}
           	 
           	else   
          	{  
                	// send data to main server  
                	write(server_sd, buffer, strlen(buffer)+1);  
                	printf("Message From client : ");                    
                	fputs(buffer,stdout);       
                	fflush(stdout);  
           	}  
           	
           	//recieve response from server  
           	memset(&buffer, '\0', sizeof(buffer));  
           	bytes = read(server_sd, buffer, 5);  
           	
           	if(bytes <= 0)  
           	{
           		printf("Nothing recieved from server\n");
           	}            
           	
           	else  
           	{  
                	// send response back to client  
                	write(info->client_sd, buffer, strlen(buffer)+1);  
                	printf("Message From server : ");                    
                	fputs(buffer,stdout);
                	printf("\n");
                	fflush(stdout);   
           	}  
      	}        
} 


//main
int main(int argc,char *argv[])  
{  
	pthread_t tid;  
     	int port, proxy_port;
     	char ip[100];
     
     	port = 5010 ; //server port
     	proxy_port = 5000 ; //proxy port

     	hostname_to_ip("server", ip);
     
     	printf("\nThis is the Server's ip address = %s\n", ip);
        
     
     	int proxy_sd =0, client_sd=0;  
     	struct sockaddr_in proxy_addr;
     	  
     	signal(SIGPIPE,SIG_IGN);  //This takes care of the issue that server doesn't close if one of the clients closes
      	
      	// create a socket  
      	if((proxy_sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)  
      	{  
          	printf("\nFailed to create socket");  
      	}  
      	printf("Proxy created\n");
      	
      	memset(&proxy_addr, 0, sizeof(proxy_addr));  
      	
      	// set socket variables  
      	proxy_addr.sin_family = AF_INET;  
      	proxy_addr.sin_port = htons(proxy_port);  
      	proxy_addr.sin_addr.s_addr = INADDR_ANY;  
      	
      	// bind the socket  
      	if((bind(proxy_sd, (struct sockaddr*)&proxy_addr,sizeof(proxy_addr))) < 0)  
      	{  
           	printf("Failed to bind a socket\n");  
      	} 
      	 
      	// start listening to the port for new connections  
      	if((listen(proxy_sd, 3)) < 0)  
      	{  
           	printf("Failed to listen");  
      	}  
      	
      	printf("waiting for connection..\n");  
      	
      	//accept all client connections  
      	while(1)  
      	{  
           	client_sd = accept(proxy_sd, (struct sockaddr*)NULL ,NULL);  
           	printf("client with socket descriptor = %d got connected\n",client_sd);  
           	if(client_sd > 0)  
           	{  
                 	//multithreading variables      
                 	struct serverInfo *item = malloc(sizeof(struct serverInfo));  
                 	item->client_sd = client_sd;  
                 	strcpy(item->ip,ip);  
                 	item->port = port ;  
                 	pthread_create(&tid, NULL, runSocket, (void *)item); 
           	}  
      	}  
return 0;  
}


//Utility function to get the IP address from the hostname
void hostname_to_ip(char * hostname , char* ip)  
{  
	struct hostent *he;  
	struct in_addr **addr_list;  
	int i;  
   	if ( (he = gethostbyname( hostname ) ) == NULL)   
   	{  
     		perror("gethostbyname");  
     		return ;  
   	}  
   	addr_list = (struct in_addr **) he->h_addr_list;  
   	for(i = 0; addr_list[i] != NULL; i++)   
   	{  
     		strcpy(ip , inet_ntoa(*addr_list[i]) );  
     		return ;  
   	}  
return ;
}  

