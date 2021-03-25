#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <strings.h>

int main (int argc, char *argv[]){

int sock;
struct sockaddr_in server;
struct hostent *hp, *gethostbyname();
char buf[1024];

/* Cria o socket TCP/IP */
sock = socket(AF_INET, SOCK_STREAM, 0);
if (sock<0){
  perror ("Abertura de socket");
  exit(1);
}

server.sin_family = AF_INET;
server.sin_port = htons(atoi(argv[2]));

hp = gethostbyname(argv[1]);
if (hp==0){
  fprintf(stderr, "%s: Host Desconhecido\n", argv[1]);
  exit(2);
}

bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);

if (connect(sock, (struct sockaddr *)&server, sizeof server) <0){
  perror("Erro de conexao");
  exit(1);
}

printf("Digite a mensagem que deseja enviar:");
fgets(buf, 1023, stdin);
if (write(sock, buf, sizeof buf)<0)
  perror("Erro no envio");

close(sock);
exit(0);

}