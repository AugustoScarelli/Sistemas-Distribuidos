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
	struct sockaddr_in name;
	char buf[1024], saida[1024] = "sair\n\0";;

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
	printf("Socket port #%d\n",ntohs(name.sin_port));
  do{
    /* Recebe e printa a mensagem e os dados do cliente */
    recvfrom(sock,buf,sizeof buf, 0, (struct sockaddr *)&name, &length);

    printf("  %s\n", buf); // Printa a mensagem que veio do cliente pela variavel "buf"

    /* Le a string */
    printf("Digite a mensagem resposta que deseja enviar:");
    fgets(buf, 1023, stdin);

    if(strcmp(buf,saida) == 0)
    {
      break;
    }

    /* Envia Resposta pro Cliente */
    sendto (sock,buf,sizeof buf, 0, (struct sockaddr *)&name, sizeof name);
  }while(1);

  close(sock);
  exit(0);
}

//Lupar a func dentro do loop
