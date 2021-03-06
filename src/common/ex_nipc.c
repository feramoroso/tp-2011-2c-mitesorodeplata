#include "nipc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FAT32_VOLUME_NAME "fat32.disk"

nipc_packet *next_packet=0;
uint32_t buffer[2000];
static uint32_t i=0;
uint8_t handshake=0;

nipc_socket *nipc_init(uint8_t *host, uint16_t port)
{
    if(!host) return NULL;

    nipc_socket *fp=0;
    fp = (nipc_socket *) fopen(FAT32_VOLUME_NAME, "rb");
    return fp;
}

void nipc_close(nipc_socket *socket)
{
    if(socket)
        fclose((FILE *)socket);
}

void nipc_send_packet(nipc_packet *packet, nipc_socket *socket)
{
    switch (packet->type) {
    case nipc_req_packet:
        memcpy(buffer+i, packet->payload, 4);
        i++;
        break;
    case nipc_handshake:
        handshake=1;
        next_packet = calloc(sizeof(nipc_packet), 1);
        next_packet->type = nipc_handshake;
        next_packet->len = 0;
        break;
    //  Faltan agregar los "case" para la escritura en disco
    }
}

nipc_packet *nipc_recv_packet(nipc_socket *socket)
{
    if (!handshake) {
        nipc_packet *packet = malloc(sizeof(nipc_packet));

        i--;

        fseek((FILE *)socket, buffer[i] * 512, SEEK_SET);

        packet->type = nipc_req_packet;
        packet->len = 516;
        memcpy(packet->payload, buffer+i, 4);

        fread((void*)(packet->payload+4), 512, 1, (FILE *) socket);

        return packet;
    } else {
        handshake--;
        return next_packet;
    }

}

