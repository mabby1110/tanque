#include "common.h"

uint8_t buff_tx[BUFFER];
uint8_t buff_rx[BUFFER];
int input = 9;

void *maria(void *param);

void *maria(void *param)
{
    // ncurses.h
    initscr();
    noecho();
    cbreak();

    while(1) {
        // actualiza la pantalla
        clear();
        refresh();
        printw("[CLIENT]: n.n\tframe [%d:%d:%d]\n\n", buff_rx[0], buff_rx[1], buff_rx[2]);
        printw("[CLIENT]: input [%d]\tW A S D para moverte\tesc para salir\n", input);
        // user input
        input = getch();

    /*TERMINAR PROGRAMA: ESC*/
        if (input == 27) {
            printw("u.u bye");
            endwin();
            exit(1);
    /* MOTORES: w a s d*/
        } else if(input == 119){
            crear_frame(&buff_tx[0], SOF, MOTORES, AVANZAR);
        } else if(input == 97){
            crear_frame(&buff_tx[0], SOF, MOTORES, G_IZQ);
        } else if(input == 115){
            crear_frame(&buff_tx[0], SOF, MOTORES, RETROCEDER);
        } else if(input == 100){
            crear_frame(&buff_tx[0], SOF, MOTORES, G_DER);
    /* CAÑON: A D*/
        } else if(input == 65){
            crear_frame(&buff_tx[0], SOF, CAÑON, G_IZQ);
        } else if(input == 68){
            crear_frame(&buff_tx[0], SOF, CAÑON, G_DER);
        } 
    }
    endwin();
    exit(1);
}

int main() {
    int sockfd, test_connect, test_recv;
    struct sockaddr_in serverAddr;

    /*hilo, despacha a los clientes*/
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    crear_frame(&buff_tx[0], SOF, KEEP_CONN, KEEP_CONN);
    crear_frame(&buff_rx[0], SOF, KEEP_CONN, KEEP_CONN);

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

        test_connect = connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        
        if (test_connect < 0) {
            printf("[CLIENT]: Error en la conexion.\n");
            exit(1);
        } else {
            pthread_create(&tid, &attr, maria, NULL);

            while(1) {
                // esperando llamada del servidor
                test_recv = recv(sockfd, buff_rx, sizeof(buff_rx), 0);
                if (test_recv >= 0) {
                    send(sockfd, buff_tx, sizeof(buff_tx), 0);
                }
            }
        }
    }
	return 0;
}