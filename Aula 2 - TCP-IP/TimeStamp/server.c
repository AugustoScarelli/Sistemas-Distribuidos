#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <strings.h>

int main(){

int sock, msgsock, tam, n;
struct sockaddr_in server;
char buf[1024];
int cont = 0; //Contador

/* Cria o socket TCP/IP */
sock = socket(AF_INET, SOCK_STREAM, 0);
if (sock<0){
  perror ("Abertura de socket");
  exit(1);
}

server.sin_family = AF_INET;
server.sin_addr.s_addr = INADDR_ANY;
server.sin_port = 0;

if (bind(sock, (struct sockaddr *)&server, sizeof server)<0){
  perror ("Erro no bind");
  exit(1);
}

tam = sizeof server;
if (getsockname(sock, (struct sockaddr *)&server, &tam) <0){
  perror("Erro no getsockname");
  exit(1);
}
printf("Numero da porta: %d\n", ntohs(server.sin_port));

listen(sock, 5);

do{
  msgsock = accept(sock, (struct sockaddr *)0, (int *)0 );
  if (msgsock == -1)
    perror ("Accept");

  else do{ /*loop da conexao*/
        bzero(buf, sizeof buf);
        n=read(msgsock, buf, 1024);
        if (n<0) perror ("Erro na leitura");
          else if (n==0) printf ("Desconectando\n");
            else 
            {
              printf("Mensagem recebida: %s\n", buf);
              cont++; //Le a mensagem, incrementa o contador e envia o valor de contador pro cliente
              write(sock, (char *)&cont, sizeof cont);//Aqui ele só aceita char, por isso fazemos um cast - (char *) - para passa cont como se fosse um char
            }
  } while (n!=0);
  close (msgsock);

}while(1);

close (sock);
exit(0);
}