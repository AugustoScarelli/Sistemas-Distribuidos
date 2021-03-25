#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h> //para importar o inet_ntoa()
//COM COMANDO QUE IDENTIFICA QUEM ENVIOU
main()
{
	int sock, length;
	struct sockaddr_in name; //endereços de socket sockaddr_in
	char buf[1024];

        /* Cria o socket de comunicacao */
	sock = socket(AF_INET, SOCK_DGRAM, 0);// Protocolo padrão "0" sem conexão
	if(sock<0) {
	/*
	/- houve erro na abertura do socket
	*/
		perror("opening datagram socket");
		exit(1);
	}
	/* Associa */ //Só faz esse bind em servidor, nunca no cliente
	name.sin_family = AF_INET; //familia (internet) com quem quero me comunicar
	name.sin_addr.s_addr = INADDR_ANY; //qual endereço (vale mascara) quero me comunicar (esse mascara que esta no exemplo: Qualquer endereço da internet)
	name.sin_port = htons(1234); //Aqui fixamos que o server sempre vai abrir nessa porta, 1234. Nunca usar portas pequenas, sempre > que 256. Preciso converter do host para network para que a porta possa trafegar na rede. htons -> Host to Network short (short pq é um valor pequeno)
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
	printf("Socket port #%d\n",ntohs(name.sin_port));//ntohs -> converte o numero de Network to HoSt para ficar no padrão do seu computador.


//--------\/\/\/\/\/\/\/\/\/\/\/\/\/----------
	/* Le */
recvfrom(sock, buf, 1024, 0, (struct sockaddr *)&name, &length);

printf("Endereço do Cliente:\n");
printf("Familia: %d\n", name.sin_family);
printf("IP: %s\n", inet_ntoa(name.sin_addr)); //inet_ntoa transforma de numero para letra. 
printf("Porta: %d\n", name.sin_port);
        printf("Mensagem: %s\n", buf);
        close(sock);
        exit(0);
}
