#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>


#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#include "sock.h"

nipc_packet *next_packet=0;
uint32_t buffer[2000];
//static uint32_t i=0;
uint8_t handshake=0;


/**
 * Inicia un socket
 *
 * @nipc_socket estructura del socket del protocolo NIPC
 * @return el descriptor del socket creado
 */
nipc_socket createSocket(char *host, uint16_t port)
{
    nipc_socket sock_new;
    struct sockaddr_in addr_raid;
    uint32_t i = 1;
    if ((sock_new = socket(AF_INET,SOCK_STREAM,0))<0)
    {
      printf("Error socket");
      exit(EXIT_FAILURE);
    }
    if (setsockopt(sock_new, SOL_SOCKET, SO_REUSEADDR, &i,sizeof(sock_new)) < 0)
    {
      printf("\nError: El socket ya esta siendo utilizado...");
      exit(EXIT_FAILURE);
    }
    addr_raid.sin_family = AF_INET;
    addr_raid.sin_port=htons(50003);
    addr_raid.sin_addr.s_addr=inet_addr("127.0.0.1");
    if (bind(sock_new,(struct sockaddr *)&addr_raid,sizeof(struct sockaddr_in))<0)
    {
      printf("Error bind");
      exit(EXIT_FAILURE);
    }
    if ((listen(sock_new,10))<0)
    {
      puts("Error listen");
      exit(EXIT_FAILURE);
    }
    return sock_new;
}

/**
 * Envia un paquete con socket indicado
 *
 */
uint32_t recvSocket(nipc_packet *packet, nipc_socket sock)
{
	uint32_t Leido = 0;
	uint32_t Aux = 0;
	/*
	* Comprobacion de que los parametros de entrada son correctos
	*/
	if ((sock == -1) || (packet == NULL) || (sizeof(packet->buffer) < 1))
		return -1;
	/*
	* Mientras no hayamos leido todos los datos solicitados
	*/
	while (Leido < packet->len)
	{
		Aux = recv (sock, packet->buffer + Leido, sizeof(packet->buffer) - Leido,0);
		if (Aux > 0)
		{
			/* 
			* Si hemos conseguido leer datos, incrementamos la variable
			* que contiene los datos leidos hasta el momento
			*/
			Leido = Leido + Aux;
		}
		else
		{
			/*
			* Si read devuelve 0, es que se ha cerrado el socket. Devolvemos
			* los caracteres leidos hasta ese momento
			*/
			if (Aux == 0) 
				return Leido;
			if (Aux == -1)
			{
				/*
				* En caso de error, la variable errno nos indica el tipo
				* de error. 
				* El error EINTR se produce si ha habido alguna
				* interrupcion del sistema antes de leer ningun dato. No
				* es un error realmente.
				* El error EGAIN significa que el socket no esta disponible
				* de momento, que lo intentemos dentro de un rato.
				* Ambos errores se tratan con una espera de 100 microsegundos
				* y se vuelve a intentar.
				* El resto de los posibles errores provocan que salgamos de 
				* la funcion con error.
				*/
				switch (errno)
				{
					case EINTR:
					case EAGAIN:
						usleep(100);
						break;
					default:
						return -1;
				}
			}
		}
	}
	/*
	* Se devuelve el total de los caracteres leidos
	*/
	return Leido;
}

/**
 * Recive un paquete en el socket especificado
 *
 * @nipc_packet estructura del protocolo NIPC
 * @return el paquete recivido
 */
uint32_t sendSocket(nipc_packet *packet, nipc_socket sock)
{
	uint32_t Escrito = 0;
	uint32_t Aux = 0;

	/*
	* Comprobacion de los parametros de entrada
	*/
	if ((sock == -1) || (packet == NULL) || (packet->len < 1))
		return -1;
	
	/*
	* Bucle hasta que hayamos escrito todos los caracteres que nos han
	* indicado.
	*/
	while (Escrito < packet->len)
	{
		Aux = send(sock, packet->buffer + Escrito, packet->len - Escrito,0);
		if (Aux > 0)
		{
			/*
			* Si hemos conseguido escribir caracteres, se actualiza la
			* variable Escrito
			*/
			Escrito = Escrito + Aux;
		}
		else
		{
			/*
			* Si se ha cerrado el socket, devolvemos el numero de caracteres
			* leidos.
			* Si ha habido error, devolvemos -1
			*/
			if (Aux == 0)
				return Escrito;
			else
				return -1;
		}
	}
	/*
	* Devolvemos el total de caracteres leidos
	*/
	return Escrito;
}