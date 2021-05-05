#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>

struct mensagem
{
  int codigo;
  char linha[1024];
};

main(argc, argv)
     int argc;
     char *argv[];
{
	int sock, tam;
  char buf[1024]; 
	struct sockaddr_in name;
	struct hostent *hp, *gethostbyname();
  struct mensagem mens;

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

  //Codigo de cadastramento no servidor
  mens.codigo = 1;
  sendto (sock,(char *)&mens,sizeof mens, 0, (struct sockaddr *)&name, sizeof name);

  if(fork() == 0)
  {
    //Filho
    do
    {
      bzero(mens.linha, 1024);//--------
      recvfrom (sock,(char *)&mens,sizeof mens, 0, (struct sockaddr *)&name, &tam);
      printf("\nCCC Mensagem recebida: %s\n", mens.linha);
    } while(1);
  }
  else
  {
    //Pai
    do
    {
      printf("CCC Digite a Mensagem: ");
      bzero(mens.linha, 1024);//--------
      fgets(mens.linha, 1023, stdin);
      sendto (sock, (char *)&mens,sizeof mens, 0, (struct sockaddr *)&name, sizeof name);
    } while(1);
  }
                
  close(sock);
  exit(0);
}
