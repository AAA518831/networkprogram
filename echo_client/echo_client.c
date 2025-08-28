//���� Ŭ���̾�Ʈ
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ������ ���� �Լ��� ����� �� ��� �����ϱ� ���� ���
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h> //������ ���� ���α׷��ֿ� �ʿ��� �Լ��� ����ü���� ����

void ErrorHandling(char* message); //���� �޽����� ����ϰ� ���α׷��� �����ϴ� �Լ�

int main(int argc, char* argv[]) //argc�� ����� ������ ��, argv�� ����� ������ �迭
{

    WSADATA wsaData; //���� ������ ����ü, ������ �ʱ�ȭ ������ ����
    SOCKET hSocket; //������ ������ ����
    char message[1024]; //������(�޽���)�� ������ ����
    int strLen; //������ ���ڿ��� ���̸� ������ ����
    SOCKADDR_IN servAdr;

    if (argc != 3) { // ����� ���ڰ� 3���� �ƴ� ���, �ùٸ� ������ ����ϰ� ����
        printf("�ùٸ� ���� : %s <IP> <port>\n", argv[0]);
        exit(1); //echo_client 127.0.0.1(������) 9800
    } 
    //��WSAStartup() : ���� �ʱ�ȭ
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("ERROR: ������ �ʱ�ȭ �� �� �����ϴ�.");

    //1. socket() : ���Ӵ�� ���� ����
    hSocket = socket(PF_INET, SOCK_STREAM, 0); //Ŭ���̾�Ʈ ���� ����
    if (hSocket == INVALID_SOCKET)
        ErrorHandling("socket() error");

    memset(&servAdr, 0, sizeof(servAdr));  //memset() : ���� �ּ� ����ü�� 0���� �ʱ�ȭ
    //�����û�� ������ ������ ä��
    servAdr.sin_family = AF_INET; //�ּ� ü�踦 IPv4�� ����
    servAdr.sin_addr.s_addr = inet_addr(argv[1]); //ù ��° ����� ���ڷ� ���޵� IP �ּҸ� ����
    servAdr.sin_port = htons(atoi(argv[2])); //�� ��° ����� ���ڷ� ���޵� ��Ʈ ��ȣ�� ����, 
    // htons �Լ��� ȣ��Ʈ ����Ʈ ������ ��Ʈ��ũ ����Ʈ ������ ��ȯ

//2. connect() : ���� ��û �� ����
    if (connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR) //���� ��û
        ErrorHandling("ERROR: ������ ������ �� �����ϴ�.");
    else
        puts("�����...........");

    //3. send() / recv()  :  ä�� �޽��� ��/����
    while (1) //���ѷ���
    {
        //����ڷκ��� ���ڿ��� �Է� �޴´�.
        fputs("������ �Է��ϼ���(����� Q to quit): ", stdout); //����ڿ��� �޽����� �Է��϶�� ���
        fgets(message, 1024, stdin);

        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) //�Է��� �޽����� 'q'�� 'Q'�̸� �ݺ����� ����
            break; //while �� Ż��
        //������ 0(q-> ����) -> !(����) -> 1
        //q-> 1 -> ||(or)�� �� �� �ϳ��� 1�̸� ��
        //str~:string:(���ڿ�) �Լ� 
        //strcmp : ���ڿ� �� 
        //&& -> and || -> or ! -> not(����=�ݴ�)

        //����ڰ� �Է��� ���ڿ��� ������ �����Ѵ�.
        send(hSocket, message, strlen(message), 0); 

        //�����κ��� ��� ���� ���ڿ��� ���� ���� �޽����� �����Ѵ�.
        strLen = recv(hSocket, message, 1024 - 1, 0);

        message[strLen] = 0; //���ڿ��� �� �а� null���� ����            
        printf("�����κ��� ���۵� �޽���: %s", message);
    }  //while �ݺ��� ��

    //4. closesocket() : ������ �ݰ� ����.
    closesocket(hSocket);

    //��   WSACleanup() : ���� ����
    WSACleanup();
    return 0;
}

void ErrorHandling(char* message) //���� �޽����� ����ϰ� ���α׷��� �����ϴ� �Լ�
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}