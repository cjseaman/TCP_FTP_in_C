/*	
 *	Collin Seaman
 * 	COEN 146
 * 	Socket Lab
 *
 * 	Socket server, accepts files transmitted to its IP and saves them using the filepath provided by the client. 
*/

#include "server.h"

int main(int argc, char *argv[]) {
	
	int i = 0;
	int n;
	FILE *fileo;
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr;
	struct sockaddr_in *servinfo;
	void *buffer;
	char filename[256];
	char c = 'a';
	int port;
	
	port = atoi(argv[1]);
	if(*argv[1] >= 1024) {
		printf("Value below 1024\n");
		return 1;
	}	
	
	//set up
	printf("initializing...\n");
	memset (&serv_addr, '0', sizeof (serv_addr));
	buffer = malloc(6);
	servinfo = malloc(sizeof(struct sockaddr_in));
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));	

	//create
	
	printf("creating socket...\n");
	listenfd = socket (AF_INET, SOCK_STREAM, 0);
	bind (listenfd, (struct sockaddr*)&serv_addr, sizeof (serv_addr));
	listen (listenfd, 10);

	//accept and read file name and data	

	while (1) {
		
		printf("waiting for connection...\n");
		connfd = accept (listenfd, (struct sockaddr*)NULL, NULL);
		printf("connection recieved\n");
		i = 0;
		while(c != '\0') {
			read (connfd, &c, 1);
			filename[i] = c;
			i++;
		}
		
		filename[i] = c;
		c = 'a';
		printf("recieving file %s...\n", filename); 
		if((fileo = fopen(filename, "w")) == NULL) {
			printf("Could not create file, invalid filename!\n");
		} else {
			printf("writing file...\n");
			while((n = read (connfd, buffer, sizeof(buffer))) > 0) {
				fwrite(buffer, 1, n, fileo); 
			}
			fclose(fileo);
			printf("done.\n");
		}
		close(connfd);
	}
}
