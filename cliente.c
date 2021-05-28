 #include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

void clean_stdin(void)
{
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

struct mensagem
{
  int menu;
  int codigo;
  int qtd_users;
  char username_cliente[50];
  char linha[1024];
  int cliente_port;
  int cliente_addr;
};

main(argc, argv)
     int argc;
     char *argv[];
{
	int sock, tam, length, sair = 0, escolha;
  char buf[1024]; 
	struct sockaddr_in name;
	struct hostent *hp, *gethostbyname();
  struct mensagem mens;
  struct mensagem lista_usuarios[10];
  int portaDest, addrDest;

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
    mens.menu = 0; //Código que informa cadastramento
    sendto(sock,(char *)&mens,sizeof mens, 0, (struct sockaddr *)&name, sizeof name);
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

    scanf("%i", &mens.menu);
  //---------------------------------------------------------------------------------

//Conversa com Outro Usuário---------------------------------------------------------
    if(mens.menu == 1)
    {
      //Pergunta com quem quer falar e manda para o servidor o código e o nome
      printf("Com quem você gostaria de conversar?\n");
      clean_stdin();
      fgets(mens.username_cliente, 49, stdin);

      sendto(sock,(char *)&mens,sizeof mens, 0, (struct sockaddr *)&name, sizeof name);
      recvfrom(sock,(char *)&mens,sizeof mens, 0, (struct sockaddr *)&name, &tam);

      if(mens.codigo == 1)
      {
        if(fork() == 0)
        {
          //Filho
          name.sin_addr.s_addr = mens.cliente_addr;
          name.sin_port = mens.cliente_port;
          while(strcmp(mens.linha, "Sair\n\0") != 0)
          {
            bzero(mens.linha, 1024);
            recvfrom(sock,(char *)&mens,sizeof mens, 0, (struct sockaddr *)&name, &length);
            printf("%s\n", mens.linha);
          }
        }
        else
        {
          name.sin_addr.s_addr = mens.cliente_addr;
          name.sin_port = mens.cliente_port;
          //Pai
          while(strcmp(mens.linha, "Sair\n\0") != 0)
          {
            printf("\nDigite a Mensagem: ");
            bzero(mens.linha, 1024);
            fgets(mens.linha, 1023, stdin);
            sendto(sock,(char *)&mens,sizeof mens, 0, (struct sockaddr *)&name, sizeof name);
          }
        }
      }
      if(mens.codigo == 0)
      {
        printf("Usuário não encontrado\n");
      }
    }

//Printa Lista Usuário-------------------------------------------------------------
    if(mens.menu == 2)
    {
      sendto (sock,(char *)&mens,sizeof mens, 0, (struct sockaddr *)&name, sizeof name);
      recvfrom(sock,(char *)&lista_usuarios,sizeof lista_usuarios, 0, (struct sockaddr *)&name, &tam);
      for(int i = 0; i < 10; i++)
      {
        if(strcmp(lista_usuarios[i].username_cliente, "") != 0)
        {
          printf("%s\n", lista_usuarios[i].username_cliente);
        }
        else{
          printf("--\n");
        }
        
      }
    }
//Desloga do Sistema-------------------------------------------------------------
    if(mens.menu == 3)
    {
      sendto (sock,(char *)&mens,sizeof mens, 0, (struct sockaddr *)&name, sizeof name);
      printf("Até logo!\n");
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
