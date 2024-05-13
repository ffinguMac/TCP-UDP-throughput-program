#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

void send_data_tcp(char *server_ip, int server_port, int data_rate) {
    int c_socket;
    struct sockaddr_in serv_addr;
    char msg[1024];
    int i = 0;
    // 소켓 생성
    c_socket = socket(PF_INET, SOCK_STREAM, 0);
    // 서버 주소 구조체 초기화
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(server_ip);
    serv_addr.sin_port = htons(server_port);
    // 서버에 연결
    connect(c_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    while(i < 1000){
        memset(msg, 'z', data_rate);
        send(c_socket, msg, data_rate, 0);
        usleep(1000000 / data_rate);
        i++;
    }

    close(c_socket);
}

void send_data_udp(char *server_ip, int server_port, int data_rate) {
    int c_socket;
    struct sockaddr_in serv_addr;
    char msg[1024];
    int i = 0;
    // 소켓 생성
    c_socket = socket(PF_INET, SOCK_DGRAM, 0);

    // 서버 주소 구조체 초기화
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(server_ip);
    serv_addr.sin_port = htons(server_port);

    while(i < 1000){
        memset(msg, 'z', data_rate);
        sendto(c_socket, msg, data_rate, 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        usleep(1000000 / data_rate);
        i++;
    }

    close(c_socket);
}

int main(int argc, char* argv[]) {
    char *server_ip = argv[1];
    int server_port = atoi(argv[2]);
    int data_rate = atoi(argv[3]);
    if (strcmp(argv[4], "tcp") == 0) {
        send_data_tcp(server_ip, server_port, data_rate);
    } else if (strcmp(argv[4], "udp") == 0) {
        send_data_udp(server_ip, server_port, data_rate);
    } 
}
