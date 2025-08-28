#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
    WSADATA wsaData;
    SOCKET hSocket;
    char opmsg[BUF_SIZE];
    int result, opndCnt, i;
    SOCKADDR_IN servAdr;
    if (argc != 3) { ///���α׷��� ����� �� IP �ּҿ� ��Ʈ ��ȣ�� ���ڷ� ���� �ʾҴٸ�, ������ ����ϰ� ���α׷� ����
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    //����������� �ʱ�ȭ,  �����ϸ� 0�� ��ȯ�ϰ�, �����ϸ� 0�� �ƴ� ���� ��ȯ
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");

    //���� ����
    hSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (hSocket == INVALID_SOCKET)
        ErrorHandling("socket() error");

    //�ּ� ����ü�� 0���� �ʱ�ȭ
    memset(&servAdr, 0, sizeof(servAdr));
    // �����ּҸ� ����
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = inet_addr(argv[1]);
    servAdr.sin_port = htons(atoi(argv[2]));

    //������ IP�� ��Ʈ�� ������ ������ �õ�
    if (connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
        ErrorHandling("connect() error!");
    else
        puts("���� ���� ...........");

    fputs("�����  ���� ���� : ", stdout);
    scanf("%d", &opndCnt);
    opmsg[0] = (char)opndCnt;

    for (i = 0; i < opndCnt; i++) //�ǿ�����(����)�� ������ŭ �ݺ����� ����
    {
        printf("�����  ���� %d: ", i + 1);
        scanf("%d", (int*)&opmsg[i * OPSZ + 1]); //����ڰ� �Է��� �ǿ����� ���� �迭�� ������� ����
    }
    fgetc(stdin); //���ۿ� ���� �ִ� �ٹٲ� ���ڸ� �����մϴ�. (scanf ���� ���۸� ���� ���� ���)
    fputs("������(+, - , * �� �ϳ�): ", stdout); //����ڿ��� �����ڸ� �Է¹ޱ� ���� �޽����� ���
    scanf("%c", &opmsg[opndCnt * OPSZ + 1]);  //����ڰ� �Է��� �����ڸ� opmsg �迭�� ����

    //������ �Էµ� �����͸� ����
    send(hSocket, opmsg, opndCnt * OPSZ + 2, 0);   //������ opmsg �迭�� ����� �����͸� �����մϴ�

    //�����κ��� ���� ����� �޾� result ������ ����
    recv(hSocket, &result, RLT_SIZE, 0);

    printf("������ ���� ���� ������ : %d \n", result); //�����κ��� ���� ��� ����� ���
    closesocket(hSocket); //����� ������ ����
    WSACleanup(); // //���� ���̺귯���� �����ϰ� ��� ���ҽ��� ����
    return 0;
}

void ErrorHandling(char* message) //���� �޽����� ����ϰ� ���α׷��� �����ϴ� �Լ�
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}