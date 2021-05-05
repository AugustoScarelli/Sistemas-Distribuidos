#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>

main()
{
	int sock, length;
  int cont=0;
	struct sockaddr_in name;
	char buf[1024];

        /* Cria o socket de comunicacao */
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock<0) {
	/*
	/- houve erro na abertura do socket
	*/
		perror("opening datagram socket");
		exit(1);
	}
	/* Associa */
	name.sin_family = AF_INET;
	name.sin_addr.s_addr = INADDR_ANY;
	name.sin_port = htons(1234);
	if (bind(sock,(struct sockaddr *)&name, sizeof name ) < 0) {
		perror("binding datagram socket");
		exit(1);
	}
        /* Imprime o numero da porta */
	length = sizeof(name);
	if (getsockname(sock,(struct sockaddr *)&name, &length) < 0) {
		perror("getting socket name");
		exit(1);
	}
	printf("SSSSSSS  porta: %d\n",ntohs(name.sin_port));

	/* Le */
//	if (read(sock,buf,1024)<0)
//                perror("receiving datagram packet");

do{
recvfrom (sock,buf,1024,0,(struct sockaddr *)&name, &length);

printf("SSSS  Endereco do cliente:\n");
printf("SSSS  Familia: %d\n", name.sin_family);
printf("SSSS  IP: %s\n", inet_ntoa(name.sin_addr));
printf("SSSS  Porta: %d\n", name.sin_port);

printf("SSSSSS   mensagem recebida: %s\n", buf);

cont++;

sendto(sock, (char *)&cont, sizeof cont, 0, (struct sockaddr *)&name, sizeof name);

} while(cont<4);

        close(sock);
        exit(0);
}