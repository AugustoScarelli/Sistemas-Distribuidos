#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>

//#define DATA "Esta eh a mensagem que quero enviar"

int main()

{
	int sock, cont, tam;
	struct sockaddr_in name;
	struct hostent *hp, *gethostbyname();
  char linha[1024];

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
        hp = gethostbyname("localhost");
        if (hp==0) {
            fprintf(stderr, "unknown host ");
            exit(2);
        }
        bcopy ((char *)hp->h_addr, (char *)&name.sin_addr, hp->h_length);
	name.sin_family = AF_INET;
	name.sin_port = htons(1234);

	/* Envia */

printf ("Digite a mensagem: ");
fgets(linha, 1023, stdin);

	if (sendto (sock,linha,sizeof linha, 0, (struct sockaddr *)&name, sizeof name)<0)
                perror("sending datagram message");

recvfrom(sock, (char *)&cont, sizeof cont, 0, (struct sockaddr *)&name, &tam);

printf("Timestamp: %d\n", cont);

close(sock);
exit(0);
}


//loopa os 2

//if(fork()==0{recive ou send}else{recive ou send} dependo do servidor/cliente)

//um loop pra cada processo