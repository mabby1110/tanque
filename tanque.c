#include "common.h"

int client_n = 0;
int client_list[MAX_CONN];

uint8_t buff_tx[BUFFER];
uint8_t buff_rx[BUFFER];
pthread_mutex_t request_mutex;

// variables de prueba

void *lupe(void *param);

void *lupe(void *param)
{
    int i, test_send, test_recv;

    while(1) {
        clear_screen();
        printf("\n\n[SERVER]: Clientes conectados %d\n", client_n);
        for (i = 0; i < MAX_CONN; i++) {
            if (client_list[i] <= 0) {
                continue;
            } else {
                printf("\t\tCliente %d\tfd %d\n\n\n", client_n, client_list[i]);
                test_send = send(client_list[i], buff_tx, sizeof(buff_rx), 0);

                if (test_send < 0) {
                    close(client_list[i]);
                    printf("[CLIENT %d]: desconectado\n", client_list[i]);
                    client_list[i] = 0;
                    client_n--;
                } else if (test_send > 0) {
                    test_recv = recv(client_list[i], buff_rx, sizeof(buff_rx), 0);
                    // printf("test_recv %d", test_send);
                    // leer_frame(buff_rx);

                    if(buff_rx[2] == 0x00) {
                        printf("[CLIENT %d]: %d %d\t",client_list[i], buff_rx[0], buff_rx[1]);
                        leer_frame(buff_rx);
                    } else if (buff_rx[2] == 0xFF) {
                        crear_frame(buff_tx, SOF, MOTORES, KILL_CONN);
                        send(client_list[i], buff_tx, sizeof(buff_rx), 0);
                        close(client_list[i]);
                        printf("[CLIENT %d]: desconectado\n", client_list[i]);
                        client_list[i] = 0;
                        client_n--;
                    }
                }
            }
        }
        sleep(1);
    }
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