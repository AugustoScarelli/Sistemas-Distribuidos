#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
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

main()
{
	int sock, length, qtd_usuarios = 0, valid = 0, posi, existe;
	struct sockaddr_in name;
	char buf[1024], zera[50]="--", username_cliente1[50], username_cliente2[50];
  //---------------
  struct mensagem mens;
  struct mensagem lista_usuarios[10];
  int portaC1, portaC2, addrC1, addrC2;

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

//Le a mensagem do cliente
//----------------------------------------------------------
do
{
  bzero(buf, 1024);
  recvfrom(sock,(char *)&mens,sizeof mens, 0, (struct sockaddr *)&name, &length);

  switch(mens.menu)
  {
    //Cadastramento
    case 0: 
      //Verifica se já não existe um usuário com o mesmo nome
      for(int i = 0; i < 10; i++)
      {
        if(strcmp(mens.username_cliente, lista_usuarios[i].username_cliente) == 0)
        {
          valid++;
        }
      }
       //Coloca o username inserido pelo usuário na lista de users cadastrados/online
      if(valid == 0)
      {
        strcpy(lista_usuarios[qtd_usuarios].username_cliente, mens.username_cliente);
        lista_usuarios[qtd_usuarios].cliente_addr = name.sin_addr.s_addr;
        lista_usuarios[qtd_usuarios].cliente_port = name.sin_port;

        //Envia pro cliente um feedback quando cadastrado
        bzero(buf, 1024);
        mens.codigo = 11;
        qtd_usuarios++;
        sendto (sock,(char *)&mens,sizeof mens, 0, (struct sockaddr *)&name, sizeof name);
        valid = 0;
        break;
      }
      else
      {
        mens.codigo = 10;
        sendto (sock,(char *)&mens,sizeof mens, 0, (struct sockaddr *)&name, sizeof name);
        valid = 0;
        break;
      }

    //Falar com outro usuário
    case 1:
      existe = 0;
      //Verifica se existe um usuário conectado com o nome que foi passado e, se existir, pega os dados para se conecar com ele.
      for(int i = 0; i < 10; i++)
      { 
        if(strcmp(mens.username_cliente, lista_usuarios[i].username_cliente) == 0)
        {
          printf("if");
          mens.cliente_port = lista_usuarios[i].cliente_port;
          mens.cliente_addr = lista_usuarios[i].cliente_addr;
          strcpy(mens.username_cliente, lista_usuarios[i].username_cliente);
          existe = 1;
        }
      }
      mens.codigo = existe;
      sendto(sock,(char *)&mens,sizeof mens, 0, (struct sockaddr *)&name, sizeof name);

    //Listar Usuários Online
    case 2:
      sendto(sock,(char *)&lista_usuarios,sizeof lista_usuarios, 0, (struct sockaddr *)&name, sizeof name);
      break;

    //Funcionalidade não encontrada/listada.
    default: printf("\nOpcao nao encontrada.Tente novamente mais tarde ou tente outra funcionalidade.\n");

    case 3:
      for(int i = 0; i < 10; i++)
      {
        if(strcmp(mens.username_cliente, lista_usuarios[i].username_cliente) == 0)
        {
          strcpy(lista_usuarios[i].username_cliente, "--");
          posi = i;
        }
      }
      for(int i = posi; i < 10-posi; i++)
      {
        strcpy(lista_usuarios[i].username_cliente, lista_usuarios[i+1].username_cliente);
      }
      qtd_usuarios--;
  }
}while(1);

//----------------------------------------------------------

/*  if(fork()==0)
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
  exit(0);*/
}
