#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <strings.h>

int main()
{
  int sock, msgsock, tam, n;
  struct sockaddr_in server;
  char buf[1024];

  //Cria o socket TCP/IP
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock<0)//se não conseguir criar o sock
  {
    perror("Abertura de socket");
    exit(1);
  }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY; //IN ADDR_ ANY -> Me comunico com qualquer endereço
  server.sin_port = 0;//o SO atribui a porta pra gente

  if(bind(sock, (struct sockaddr *)&server, sizeof server)<0)
  {
    perror("Erro no bind");
    exit(1);
  }

  //É importante inicializar variavel TAM para não dar problema em alguns sistemas
  tam = sizeof server;

  if(getsockname(sock, (struct sockaddr *)&server, &tam)<0)
  {
    perror("Erro no getsockname\n");
    exit(1);
  }

  printf("Numero da Porta> %d\n", ntohs(server.sin_port));

  //Para configurar o tamanho da lista: Esse socket sock vai ter um tamanho de fila X, no caso do exemplo, ele pode ter até 5 mensagens na fila. Tudo que passar de 5, será perdido a info.
  listen(sock, 5);

  do{
    //aqui tem que aceitar a conexão do cliente com o servidor. O accept que se sincorniza com o "connect" do client
    msgsock = accept(sock, (struct sockaddr *)0, (int *)0); //colocar 0, n usa nada de dado do cliente
    if(msgsock == -1)
    {
      perror("Erro no Accept\n");
    }
    else do //Loop da conexão
    {
      bzero(buf, sizeof buf);//Mesma func do FFLUSH(que zera arquivos), zera a variavel buf.
      n = read(msgsock, buf, 1024); //lê quantos bytes recebeu
      if(n < 0)
      {
        perror("Erro na leitura\n");
      }
        else if(n == 0)
        {
          printf("Desconectando\n");
        }
          else
          {
            printf("Mensagem recebida:%s\n", buf);
          }
    }while(n!=0);
  }while(1);

  close(sock);
  exit(0);
}