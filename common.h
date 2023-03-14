#pragma once
#define HEADER_FILE

#include <pthread.h>
#include <ncurses.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <errno.h>

#define clear_screen() printf("\033[H\033[J")
#define PORT 8080
#define IP "192.168.43.231"
#define BUFFER 4
#define MAX_CONN 2

// start of frame
#define SOF 0xAA

// modulos
#define MOTORES 0x10
#define CAÑON 0x20
#define KEEP_CONN 0x00
#define KILL_CONN 0xFF

// acciones
#define G_IZQ 0X11
#define G_DER 0X12
#define AVANZAR 0X13
#define RETROCEDER 0X14


void leer_frame();
void crear_frame(uint8_t *frame, uint8_t a, uint8_t b, uint8_t c);

void leer_frame(uint8_t *frame) {
    int i = 0;

    for(i=0;i<BUFFER;i++){
        printf("%d ", frame[i]);
    }
    printf("\n");
}

void crear_frame(uint8_t *frame, uint8_t a, uint8_t b, uint8_t c) {
    // printf("variable %d apuntador %d", frame, *frame);
    /* Frame*/
    bzero(frame, BUFFER);
    memset(frame + 0, a, sizeof(char));  // start fo frame
    memset(frame + 1, b, sizeof(char));  // periferico
    memset(frame + 2, c, sizeof(char));  // instrucción
    // memset(buff + 6, d, 2*sizeof(char));  // payload

    leer_frame(frame);
}
