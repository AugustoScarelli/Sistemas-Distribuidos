#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>

//#define DATA "Esta eh a mensagem que quero enviar"

main(argc, argv)
     int argc;
     char *argv[];
{
	int sock, tam;
	struct sockaddr_in name;
	struct hostent *hp, *gethostbyname();
  char buf[1024], saida[1024] = "sair\n\0"; //string da mensagem que o cliente vai enviar

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
    fprintf(stderr, "Unknown host ");
    exit(2);
  }
  bcopy ((char *)hp->h_addr, (char *)&name.sin_addr, hp->h_length);
	name.sin_family = AF_INET;
	name.sin_port = htons(1234);

  while(1){
    /* Digita a mensagem */
    printf("Digite a mensagem que deseja enviar:");
    fgets(buf, 1023, stdin); //le o que o usuário digita. fgets é a forma mais segura de receber o conteudo da string. Coloca sempre 1 a byte a menos pra garantir que não leia o "final da string"

    if(strcmp(buf,saida) == 0)
    {
      break;
    }

    /* Envia */
    if (sendto (sock,buf,sizeof buf, 0, (struct sockaddr *)&name, sizeof name)<0)
          perror("sending datagram message");
    
    /* Recebe a Mensagem do servidor */
    recvfrom(sock,buf,sizeof buf, 0, (struct sockaddr *)&name, &tam);
    printf("  %s\n", buf); // Printa a mensagem que veio do cliente pela variavel "buf"
  }
  
  close(sock);
  exit(0);
}
