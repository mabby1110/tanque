#include "common.h"

int client_n = 0;
int client_list[MAX_CONN];

uint8_t buff_tx[BUFFER];
uint8_t buff_rx[MAX_CONN][BUFFER];
int i, test_send;

// variables de prueba

void *lupe(void *param);

void *lupe(void *param)
{
    // ncurses.h
    initscr();
    noecho();
    cbreak();

    while(1) {
        clear();
        printw("[SERVER]: Clientes conectados %d\n\n", client_n);
        // Itera la lista de clientes
        for (i = 0; i < MAX_CONN; i++) {
            if (client_list[i] <= 0) {
                continue;
            } else {
                printw("\t[CLIENTE %d]: fd %d\tframe [%d:%d:%d]\t", client_n, client_list[i], buff_rx[i][0], buff_rx[i][1], buff_rx[i][2]);

                // comprueba conexion
                test_send = send(client_list[i], buff_tx, sizeof(buff_tx), 0);
                // El cliente no recibio el mensaje
                if (test_send < 0) {
                    close(client_list[i]);
                    printw("desconectado\n", client_list[i]);
                    client_list[i] = 0;
                    client_n--;
                
                // el cliente recibio el mensaje
                } else if (test_send > 0) {
                    recv(client_list[i], buff_rx[i], sizeof(buff_rx[i]), 0);
                    if (buff_rx[i][1] < KILL_CONN) {
                        if(buff_rx[i][1] == MOTORES) {
                            printw("MOTORES %d\n", buff_rx[i][2]);
                        } else if(buff_rx[i][1] == CAÑON) {
                            printw("CAÑON %d\n", buff_rx[i][2]);
                        } else if(buff_rx[i][1] == KEEP_CONN) {
                            printw("KEEP_CONN %d\n", buff_rx[i][2]);
                        }
                    } else {
                        printw("desconectado\n", client_list[i]);
                        crear_frame(buff_tx, SOF, KILL_CONN, KILL_CONN);
                        send(client_list[i], buff_tx, sizeof(buff_tx), 0);
                        close(client_list[i]);
                        client_list[i] = 0;
                        client_n--;
                        endwin();
                        exit(1);
                    }
                }
            }
        }
        refresh();
        // sleep(1);
    }
    
    endwin();
    exit(1);
}

int main() {
    int sockfd, binded, test_accept;
    struct sockaddr_in serverAddr;
    struct sockaddr_in newAddr;

    crear_frame(&buff_tx[0], SOF, MOTORES, KEEP_CONN);

    /* socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        fprintf(stderr, "[SERVER-error]: %d: %s \n", errno, strerror( errno ));
        return -1;
    } else {
        printf("[SERVER]: Socket creado.\n");
        
        // Configurar socket
        memset(&serverAddr, '\0', sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(PORT);
        serverAddr.sin_addr.s_addr = inet_addr(IP);

        //Asociar a direccion ip
        binded = bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
        if (binded < 0) {
            fprintf(stderr, "[SERVER-error]: socket creation failed. %d: %s \n", errno, strerror( errno ));
            return -1;
        }

        // Escuchar socket
        if (listen(sockfd, MAX_CONN) == 0) {
            printf("[SERVER]: Esperando conexion ...\n");
        } else {
            printf("[SERVER]: Error al momento de escuchar conexiones.\n");
        }

        /*hilo, despacha a los clientes*/
        socklen_t addr_size;
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&tid, &attr, lupe, NULL);

        while (1) {
            /*aceptar cliente */
            if (client_n < MAX_CONN) {
                // guardar en la lista de conexiones
                test_accept = accept(sockfd, (struct sockaddr *)&newAddr, &addr_size);

                if (test_accept == -1) {
                    fprintf(stderr, "[SERVER-error]: %d: %s \n", errno, strerror( errno ));
                    return -1;
                } else {
                    client_list[client_n] = test_accept;
                    client_n++;
                }
            } else {
                continue;
            }
        }
    }
    return 0;
}