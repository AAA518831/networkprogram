#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h> // ��������� ����ϱ� ���� ���
#include <Ws2tcpip.h> 
void ErrorHandling(char* message); //���� �Լ� ����

int main(int argc, char* argv[])
{
    WSADATA wsaData; 
    SOCKET hSocket;
    SOCKADDR_IN servAddr; 
    char message[30]; //������(���ڿ�) �ޱ����� �迭
    int strLen;
    if (argc != 3) { //���� 3�� -> ����) ���ϸ� ip�ּ� ��Ʈ��ȣ
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    //1. WSAstartup()
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");
    //2. socket()
    hSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (hSocket == INVALID_SOCKET)
        ErrorHandling("socket() error");
    //3. memset()
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));
    //4. connect()
    if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        ErrorHandling("connect() error!");
    //5. recv()
    strLen = recv(hSocket, message, sizeof(message) - 1, 0);
    if (strLen == -1)
        ErrorHandling("read() error!");
    printf("Message from server: %s \n", message);
    //6. closesocket()
    closesocket(hSocket);
    //7. WSACleanup()
    WSACleanup();
    return 0;
}
//�����Լ�
void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
