#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <strings.h>

//argc = contador de parametros e argv = vetor de valores dos argumentos
int main(int argc, char *argv[]){

int sock; //Discritor do socket (nome logico)
struct sockaddr_in server; //onde tem as infos do server com a in -> internet
struct hostent *hp, *gethostbyname(); // ele usa a função gethostbyname()
char buf[1024];

// Cria o Socket TCP/IP

sock = socket(AF_INET, SOCK_STREAM, 0);
if(sock<0)//se não conseguir criar o sock
{
  perror("Abertura de socket");
  exit(1);
}

server.sin_family = AF_INET;
server.sin_port = atoi(argv[2]); //atoi transforma de letra para inteiro

hp = gethostbyname(argv[1]);
if(hp==0) //se retornar null => Erro de não encontrado os servidor
{
  fprintf(stderr, "$s: Host Desconhecido\n", argv[1]);
  exit(2);
}

bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);

if(connect(sock, (struct sockaddr *)&server, sizeof server)<0)//conect com o server &server com tamanho sizeof server
{
  perror("Erro de conexão");
  exit(1);
}

printf("Digite a mensagem que deseja enviar:");
fgets(buf, 1023, stdin);//comando seguro para leitura de string. Pois alem da leitura da string ainda tem 2 parametros de segurança que contem a quantidade max. de bytes que serão lidos e de onde vai ser lido (stdin = teclado).
//da para enviar com write ou send (pesquisar dpois as diferenças)
if(write(sock, buf, sizeof buf)<0)
{
  perror("Erro no envio");
}

close(sock);
exit(0);
} 