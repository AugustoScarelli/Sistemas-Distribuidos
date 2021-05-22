#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>

struct mensagem
{
  int codigo;
  char linha[1024];
};

main()
{
	int sock, length;
	struct sockaddr_in name;
	char buf[1024];
  //---------------
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

//Le a mensagem de cadastramento do cliente
//----------------------------------------------------------

  bzero(buf, 1024);
  recvfrom(sock,(char *)&mens,sizeof mens, 0, (struct sockaddr *)&name, &length);

  printf("Endereco do cliente:\n");
  printf("IP: %s  porta:%d\n\n", inet_ntoa(name.sin_addr), name.sin_port);

  printf("SSSS: Mensagem Recebida: %d\n", mens.codigo); //cod de cadastramento

//----------------------------------------------------------

  if(fork()==0)
  {
    //Filho
    do
    {
      bzero(mens.linha, 1024);
      recvfrom(sock,(char *)&mens,sizeof mens, 0, (struct sockaddr *)&name, &length);
      printf("SSSS: Mensagem Recebida: %s\n", mens.linha);
    } while(1);
  }
  else
  {
    //Pai
    do
    {
      printf("\nSSSS: Digite a Mensagem: ");
      bzero(mens.linha, 1024); //zera a struck buf ali......pesquisar +
      fgets(mens.linha, 1023, stdin);
      sendto(sock,(char *)&mens,sizeof mens, 0, (struct sockaddr *)&name, sizeof name);
    } while(1);
  }

  close(sock);
  exit(0);
}
