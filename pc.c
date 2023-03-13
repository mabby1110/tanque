#include "common.h"

uint8_t buff_tx[BUFFER];
uint8_t buff_rx[BUFFER];
uint8_t test_buff[BUFFER];
uint8_t comando = 0;
int input = 9;

void *lupe(void *param);

void *lupe(void *param)
{
    // inicialia la pantalla
    // prepara la memoria y limpia la pantalla
    initscr();
    noecho();
    cbreak();

    // imprime en pantalla


    while(1) {
        // actualiza la pantalla
        clear();
        refresh();
        printw("[CLIENT]: status %d n.n ok!\tinput %d\n\n",buff_rx[2], input);
        printw("[CLIENT]: usa W A S D para moverte\tesc para salir\n");
        // user input
        input = getch();
        if (input == 27) {
            printw("u.u bye");
            break;
        } else if(input == 119){
            crear_frame(&buff_tx[0], SOF, MOTORES, AVANZAR);
        } else if(input == 97){
            crear_frame(&buff_tx[0], SOF, MOTORES, G_IZQ);
        } else if(input == 115){
            crear_frame(&buff_tx[0], SOF, MOTORES, RETROCEDER);
        } else if(input == 100){
            crear_frame(&buff_tx[0], SOF, MOTORES, G_DER);
        }
    }
    endwin();
}

int main() {
    int sockfd, test_connect, test_recv, test_send;
    struct sockaddr_in serverAddr;

    /*hilo, despacha a los clientes*/
    socklen_t addr_size;
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    crear_frame(&buff_tx[0], SOF, CONN, KEEP_CONN);

    /* socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        fprintf(stderr, "[CLIENT-error]: %d: %s \n", errno, strerror( errno ));
        return -1;
    } else {
        printf("[CLIENT]: Socket creado.\n");

        // Configurar socket
        memset(&serverAddr, '\0', sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(PORT);
        serverAddr.sin_addr.s_addr = inet_addr(IP);
        

        printf("[CLIENT]: Conectando a %s:%d...\n", IP, PORT);

        test_connect = connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        
        if (test_connect < 0) {
            printf("[CLIENT]: Error en la conexion.\n");
            exit(1);
        } else {
            pthread_create(&tid, &attr, lupe, NULL);

            while(1) {
                test_recv = recv(sockfd, buff_rx, sizeof(buff_rx), 0);

                if (test_recv >= 0) {
                    if (buff_rx[2] == KEEP_CONN) {
                        send(sockfd, buff_tx, sizeof(buff_tx), 0);
                    } else if (buff_rx[2] == KILL_CONN) {
                        close(sockfd);
                        return 0;
                    }
                }
                sleep(1);
            }
        }
    }
	return 0;
}