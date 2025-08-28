//���� ���α׷����� Ŭ���̾�Ʈ�κ��� ������ ����(���ϱ�, ����, ���ϱ�)�� ��û�޾� �� ����� ����� �� Ŭ���̾�Ʈ���� ������
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUF_SIZE 1024  //���� ũ�⸦ ����
#define OPSZ 4 //�� �ǿ������� ũ��(4����Ʈ)�� ����

void ErrorHandling(char* message); //���� �߻� �� �޽����� ����ϰ� ���α׷��� ����
int calculate(int opnum, int opnds[], char oprator);  //Ŭ���̾�Ʈ�κ��� ���� ������ �����ϴ� �Լ�

int main(int argc, char* argv[])
{
    WSADATA wsaData;
    SOCKET hServSock, hClntSock;
    char opinfo[BUF_SIZE];
    int result, opndCnt, i;
    int recvCnt, recvLen;
    SOCKADDR_IN servAdr, clntAdr;
    int clntAdrSize;
    if (argc != 2) { //����� ���ڷ� ��Ʈ ��ȣ�� ���޵��� ������ ������ ����ϰ� ���α׷��� ����
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    //��������� ���̺귯���� �ʱ�ȭ��. �ʱ�ȭ ���� �� ���� �޽����� ���
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");

    //���� ������ ����. ���� ������ �����ϸ� ���� �޽����� ���
    hServSock = socket(PF_INET, SOCK_STREAM, 0);
    if (hServSock == INVALID_SOCKET)
        ErrorHandling("socket() error");

    memset(&servAdr, 0, sizeof(servAdr));////�ּұ���ü�� 0���� �ʱ�ȭ(clear) 
    //���� IP �ּҿ� ��Ʈ ��ȣ�� ����
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAdr.sin_port = htons(atoi(argv[1]));

    //���Ͽ�  IP �ּҿ� ��Ʈ ��ȣ�� �Ҵ�. ���� �� ���� �޽����� ���
    if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
        ErrorHandling("bind() error");

    //������ ���� ��� ���·� ����. ���� �� ���� �޽����� ���.
    if (listen(hServSock, 5) == SOCKET_ERROR)
        ErrorHandling("listen() error");
    clntAdrSize = sizeof(clntAdr); //Ŭ���̾�Ʈ �ּ� ����ü�� ũ�⸦ ����

    for (i = 0; i < 5; i++) //������ �ִ� 5���� Ŭ���̾�Ʈ ������ ó���ϱ� ���� �ݺ����� ���
    {
        opndCnt = 0;   //�������� ����

        //Ŭ���̾�Ʈ ���� ��û�� �����ϰ� Ŭ���̾�Ʈ ������ ����
        hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSize);
        //Ŭ���̾�Ʈ�κ��� �ǿ������� ������ ����(Ŭ���̾�Ʈ�κ��� ������ �޾ƿ�)
        recv(hClntSock, &opndCnt, 1, 0);

        recvLen = 0;
        //Ŭ���̾�Ʈ�κ��� �ǿ�����(����)�� ���� ��ȣ�� ��� ������ ������ �ݺ��ؼ� �����͸� ����
                                         //�ݺ����� ���� Ŭ���̾�Ʈ�κ��� ��� �����͸� ���� ������ ���
        while ((opndCnt * OPSZ + 1) > recvLen) //opndCnt * OPSZ + 1: �����ؾ� �� �� �������� ũ�⸦ ���
            //������ ������� ���ŵ� ������(recvLen)�� �� �� ������ ũ�⺸�� ���� ��� ��� ����
        /* opndCnt�� �������� ������ ��Ÿ��
            OPSZ�� �� �������� ũ��(4����Ʈ)
            + 1�� �������� ���� ��ȣ(+, -, *) �ϳ��� �߰��� ���ԵǱ� ������ �ʿ�*/
        {
            recvCnt = recv(hClntSock, &opinfo[recvLen], BUF_SIZE - 1, 0);
            //recvCnt�� �̹��� ���� �������� ���̸� ����
            recvLen += recvCnt;
            //recvLen�� ������� ���� �������� ���̸� ����  


        }
        // ������ �����ϴ� �Լ� ȣ���Ͽ� �� ����� ���
        result = calculate(opndCnt, (int*)opinfo, opinfo[recvLen - 1]);
        //������ �����ϴ� �Լ� ȣ���Ͽ� �� ����� ���

//��� ����� Ŭ���̾�Ʈ���� ����
        send(hClntSock, (char*)&result, sizeof(result), 0);

        closesocket(hClntSock); //Ŭ���̾�Ʈ ������ ����
    }
    closesocket(hServSock);  //���� ������ ����
    WSACleanup();  //���� ������ �ݰ�, ��������� ���̺귯���� ����
    return 0;
}

int calculate(int opnum, int opnds[], char op) // ������ �����ȣ�� �ǿ����ڸ� �޾� ����� ����
{
    int result = opnds[0], i; //ù ��° �ǿ����ڸ� result�� �����ϰ�, �ݺ����� ���� ������ �ǿ����ڵ�� ������ ����

    switch (op) //���� ��ȣ�� ���� ����, ����, ������ ����
    {
    case '+':
        for (i = 1; i < opnum; i++) result += opnds[i];
        // ù��° �ǿ����ڰ� result�� �ְ� �����ڿ� ���� �ǿ����ڰ� �ִµ��� ������
        break;
    case '-':
        for (i = 1; i < opnum; i++) result -= opnds[i];
        break;
    case '*':
        for (i = 1; i < opnum; i++) result *= opnds[i];
        break;
    }
    return result; //��� ����� ��ȯ
}

void ErrorHandling(char* message) // ���� �޽����� ����ϰ� ���α׷��� ����
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}