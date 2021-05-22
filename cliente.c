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
  int qtd_users;
  char username_cliente[50];
  char linha[1024];
};

main(argc, argv)
     int argc;
     char *argv[];
{
	int sock, tam, length, sair = 0;
  char buf[1024]; 
	struct sockaddr_in name;
	struct hostent *hp, *gethostbyname();
  struct mensagem mens;
  struct mensagem lista_usuarios[10];

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

  //Codigo de cadastramento no servidor----------------------------------------------
  do
  {
    printf("DIGITE SEU USERNAME: ");
    bzero(mens.username_cliente, 50);
    fgets(mens.username_cliente, 49, stdin); 
    mens.codigo = 0; //Código que informa cadastramento
    sendto (sock,(char *)&mens,sizeof mens, 0, (struct sockaddr *)&name, sizeof name);
    
    bzero(buf, 1024);
    recvfrom(sock,(char *)&mens,sizeof mens, 0, (struct sockaddr *)&name, &length);

    if(mens.codigo == 10)
    {
      printf("Nome de usuário ja está em uso, digite outro\n");
    }

  }while(mens.codigo != 11);

  //---------------------------------------------------------------------------------

  do
  {
  //Menu do Sistema------------------------------------------------------------------

    printf("\nSeja bem vindo. O que deseja fazer?\n");
    printf("Digite 1 para mandar mensagem para outro usuario\n");
    printf("Digite 2 para ver os usuário que estao online\n");
    printf("Digite 3 para se desconectar\n");

    scanf("%i", &mens.codigo);
  //---------------------------------------------------------------------------------

  //Printa Lista Usuário-------------------------------------------------------------
    if(mens.codigo == 2)
    {
      sendto (sock,(char *)&mens,sizeof mens, 0, (struct sockaddr *)&name, sizeof name);
      recvfrom(sock,(char *)&lista_usuarios,sizeof lista_usuarios, 0, (struct sockaddr *)&name, &tam);
      for(int i = 0; i < 10; i++)
      {
        if(strcmp(lista_usuarios[i].username_cliente, "")!=0)
        {
          printf("%s\n", lista_usuarios[i].username_cliente);
        }
        else{
          printf("--\n");
        }
        
      }
    }

    if(mens.codigo == 3)
    {
      sendto (sock,(char *)&mens,sizeof mens, 0, (struct sockaddr *)&name, sizeof name);
      sair = 1;
    }

  }while(sair == 0);
}

  //---------------------------------------------------------------------------------
  //--------------------------------------------------------------------------------- 
  //---------------------------------------------------------------------------------
  //---------------------------------------------------------------------------------
  //---------------------------------------------------------------------------------
  //---------------------------------------------------------------------------------
  //---------------------------------------------------------------------------------
  //---------------------------------------------------------------------------------
  //---------------------------------------------------------------------------------
  //---------------------------------------------------------------------------------
  //---------------------------------------------------------------------------------
  //---------------------------------------------------------------------------------
  //---------------------------------------------------------------------------------
  //---------------------------------------------------------------------------------
  //---------------------------------------------------------------------------------
  //---------------------------------------------------------------------------------
  /*if(fork() == 0)
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
}*/
