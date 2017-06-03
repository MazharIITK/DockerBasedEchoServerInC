#include <sys/socket.h>  
#include <sys/types.h>  
#include <resolv.h>  
#include <string.h>  
#include <unistd.h>
#include<stdio.h>
#include<stdlib.h>

//main
int main(int argc, char* argv[])  
{  
	char port[200];  
      	char buffer[5];  
      	int sd;  
     	struct sockaddr_in client_sd;  
       
      	printf("Enter the port of proxy: ");
      	  
      	fgets(port,sizeof("5000\n")+1,stdin);  
      
     	// create a socket  
	if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)  
        {  
        	printf("socket not created\n");  
        }  
        
        memset(&client_sd, 0, sizeof(client_sd));  
           
        // set socket variables  
        client_sd.sin_family = AF_INET;  
       	client_sd.sin_port = htons(atoi(port));  
           
        // assign any IP address to the client's socket  
        client_sd.sin_addr.s_addr = INADDR_ANY;   
           
        // connect to proxy server at mentioned port number  
        connect(sd, (struct sockaddr *)&client_sd, sizeof(client_sd));  
           
        //send and receive data  
        while(1)  
        {  
        	printf("Message to server: ");  
                fgets(buffer, sizeof(buffer), stdin);  
                write(sd, buffer, strlen(buffer)+1);  
                printf("Message from server: ");  
                read(sd, buffer, 5);  
                fputs(buffer, stdout);
                printf("\n");  
        }
        
        close(sd);  

return 0;  
 }  
