 /*	
 *	Collin Seaman
 * 	COEN 146
 * 	Socket Lab
 *
 * 	Contacts a server using its ip and a port number, then passes a file name and data to the server using TCP.
 * 	The server saves the file locally and the client closes the connection.
*/

#include "client.h"

int main(int argc, char *argv[]) {
	
	if (argc != 5) {
		printf("Usage: ./client <file input name> <file output name> <server ip> <port number>\n");
	}
	
	int i;
	int sockfd = 0, n = 0;
	void *buffer;
	struct sockaddr_in serv_addr;
	char *finname = argv[1];
	char *foutname = argv[2];
	FILE *filei;
	FILE *fileo;

	//init mem
	printf("initializing memory...");
	buffer = malloc(10);
	memset(&serv_addr, '0', sizeof(serv_addr));
	
	if((filei = fopen(finname, "r")) == NULL) {
		printf("Error reading file\n");
		return 1;
	}
	fileo = fopen(foutname, "w");
	
	//create socket
	printf("creating socket...");
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 


	if(sockfd < 0) {
		printf("socket() exited with a return value of %d\n", &sockfd);
		return 1;
	}

	//set address
	printf("setting address...\n");
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons (atoi(argv[4]));
	
	if (inet_pton (AF_INET, argv[3], &serv_addr.sin_addr) <= 0) {
		printf("Invalid IP or port number. Is server running?\n");
		return 1;
	}

	//connect
	printf("connecting...\n");

	if (connect (sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf ("Error : Connect Failed, sockfd %d, address %s \n", &sockfd);
	}

	//read and send, filename first
	printf("sending filename...\n");
	write(sockfd, foutname, strlen(foutname)+1);
	printf("reading %s...\nwriting %s...\n", finname, foutname);
	while((n = fread(buffer, 1, 10, filei)) > 0) {
		write (sockfd, buffer, n);
	}
	printf("\ndone.\n");
	fclose(filei);	
	close(sockfd);
		
	return 0;
}
