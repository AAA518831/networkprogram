#include <stdio.h>
#include <winsock2.h> //�߿�(����=������ ����) ������ ���� ���ؼ� �ʿ���
void ErrorHandling(char* message); //���� ó���ϴ� �Լ� ����. �Լ�����

int main(int argc, char* argv[]) 
{
    WSADATA   wsaData;
    unsigned short host_port = 0x1234; //unsigned: �����(������ ����)>�ּ��̱� ����
      //CPU�� �ִ� ��ǻ��->ȣ��Ʈ
    unsigned short net_port; //��Ʈ��ũ //��Ʈ��ȣ(16��Ʈ)->short(2����Ʈ=16��Ʈ)
    unsigned long host_addr = 0x12345678; //ȣ��Ʈ IP �ּ�(32��Ʈ)->long 
    unsigned long net_addr; //��Ʈ��ũ�� �ش�Ǵ� IP �ּ�

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");

    net_port = htons(host_port); //htons=>ȣ��Ʈ->��Ʈ��ũ ��Ʈ ��ȯ
    net_addr = htonl(host_addr); //htonl=>ȣ��Ʈ->��Ʈ��ũ IP�ּ� ��ȯ
      //ȣ��Ʈ�� ��Ʋ�ص��, ��Ʈ��ũ�� ��ص��
      //��Ʋ�ص��->��ص��

    printf("Host ordered port: %#x \n", host_port);   //Host ordered port: 0x1234
    printf("Network ordered port: %#x \n", net_port);   //Network ordered port: 0x3412
    printf("Host ordered address: %#lx \n", host_addr);   //Host ordered address: 0x12345678
    printf("Network ordered address: %#lx \n", net_addr);  //Network ordered address: 0x78563412
    WSACleanup();
    return 0;
}

void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}