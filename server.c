#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>


void receive_data(int server_port, int protocol) {
    int s_socket;
    int c_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char msg[1024];
    int i = 0;
    printf("server open\n");

    // 서버 소켓 생성
    s_socket = socket(AF_INET, (protocol == IPPROTO_TCP) ? SOCK_STREAM : SOCK_DGRAM, 0);

    // 서버 주소 구조체 초기화
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(server_port);

    // 소켓과 주소를 바인딩
    bind(s_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
 

    if (protocol == IPPROTO_TCP){
        listen(s_socket, 1); // 1개의 클라이언트 연결 대기
        c_socket = accept(s_socket, (struct sockaddr *)&client_addr, &client_addr_len); // 클라이언트 연결 수락
    }
    int total_bytes_received = 0;
    double start_time = (double)clock() / CLOCKS_PER_SEC;
    while(i< 1000){
        int bytes_received;
        if (protocol == IPPROTO_TCP) {
            bytes_received = recv(c_socket, msg, sizeof(msg), 0);
            total_bytes_received += bytes_received;
            double current_time = (double)clock() / CLOCKS_PER_SEC;
            double elapsed_time = current_time - start_time;
            double throughput = (total_bytes_received / elapsed_time) / 1000; // 스루풋 계산
            printf("Throughput: %.2f bytes/second\n", throughput);
        } else {
            bytes_received = recvfrom(s_socket, msg, sizeof(msg), 0, (struct sockaddr *)&client_addr, &client_addr_len);
            total_bytes_received += bytes_received;
            double current_time = (double)clock() / CLOCKS_PER_SEC;
            double elapsed_time = current_time - start_time;
            double throughput = (total_bytes_received / elapsed_time) / 1000; // 스루풋 계산
            printf("Throughput: %.2f bytes/second\n", throughput);
        }
        memset(msg, 0, sizeof(msg));
        i++;
    }
    close(c_socket);
    close(s_socket);
}

int main(int argc, char *argv[]) {

    int server_port = atoi(argv[1]);
    int protocol;

    if (strcmp(argv[2], "tcp") == 0)
        protocol = IPPROTO_TCP;
    else if (strcmp(argv[2], "udp") == 0)
        protocol = IPPROTO_UDP;

    receive_data(server_port, protocol);

    return 0;
}
