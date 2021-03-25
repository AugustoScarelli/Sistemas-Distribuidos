#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>

main()
{
	int sock, length;
	struct sockaddr_in name; //endereços de socket sockaddr_in
	char buf[1024];

        /* Cria o socket de comunicacao */
	sock = socket(AF_INET, SOCK_DGRAM, 0);// Protocolo padrão "0" sem conexão
	if(sock<0) {
	/*
	/- houve erro na abertura do socket
	*/
		perror("opening datagram socket");
		exit(1);
	}
	/* Associa */ //Só faz esse bind em servidor, nunca no cliente
	name.sin_family = AF_INET; //familia (internet) com quem quero me comunicar
	name.sin_addr.s_addr = INADDR_ANY; //qual endereço (vale mascara) quero me comunicar (esse mascara que esta no exemplo: Qualquer endereço da internet)
	name.sin_port = 0; //Nmro da porta onde o servidor vai atender. Quando está 0, o sistema define a porta que não esta ocupada automaticamente e não da erro.
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
	printf("Socket port #%d\n",ntohs(name.sin_port));//ntohs -> converte o numero de Network to HoSt para ficar no padrão do seu computador.

	/* Le */
	if (read(sock,buf,1024)<0)
                perror("receiving datagram packet");
        printf("  %s\n", buf);
        close(sock);
        exit(0);
}
