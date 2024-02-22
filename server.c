#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h> 
#include <netinet/in.h>
#define SERVER_UDP_PORT 5000 /* well-known port */
#define MAXLEN 4096 /* maximum data length */

int main(int argc, char **argv)
{
	int sd, client_len, port, n;
	char buf[4];
	char rbuf[4];
	struct sockaddr_in server, client;
	const char input_string[4] = "ftp";
	switch(argc) {
		case 1:
			port = SERVER_UDP_PORT;
			//printf("Port 5000 default \n");
			break;
		case 2:
			port = atoi(argv[1]);
			//printf("Using other port %d \n", argv[1]);
			break;
		default:
			fprintf(stderr, "Usage: %s [port]\n", argv[0]);
			exit(1);
	}

	/* Create a datagram socket */	
	if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		fprintf(stderr, "Can’t create a socket\n");
		exit(1);
	}
	/* Bind an address to the socket */
	bzero((char *)&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sd, (struct sockaddr *) &server,
	sizeof(server)) == -1) {
		fprintf(stderr, "Can’t bind name to socket\n");
		exit(1);
	}

	while (1) {
		client_len = sizeof(client);
		if ((n = recvfrom(sd, buf, 4, 0, (struct sockaddr *)&client, &client_len)) < 0) {
			fprintf(stderr, "Can’t receive datagram\n");
			exit(1);
		}
		else{
			printf("buf  %s \n", buf);
			//it means we recieved a message. Then we check if the message is ftp 
			if(strcmp(buf, "ftp") == 0){
				strcpy(rbuf, "yes");
				//here we send back the answer
				if ( sendto(sd, rbuf, 4, 0,(struct sockaddr *)&client, client_len) < 0) {
					fprintf(stderr, "Can’t send datagram\n");
					exit(1);
				}
				else{
					//here we can start our while loop to recieve the data. 
						//TWO ARRAYS WILL BE SEND. Then compared if same we write in new file if not 
							//we ask for the repetition of the message by sending a ok or sendAgain. 
					printf("Thank you. we will get you after we finsh this. DO ABOVE IN COMMENTS \n");
				}
			}
			else{
				strcpy(rbuf,"no");
				if ( sendto(sd, rbuf, 4, 0,(struct sockaddr *)&client, client_len) < 0) {
					fprintf(stderr, "Can’t send datagram\n");
					exit(1);
				}
			} 
		}
		
		
	
	}
	close(sd);
	return(0);
}