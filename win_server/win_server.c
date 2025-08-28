#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h> //윈도우소켓 사용
void ErrorHandling(char* message); //에러 함수 선언

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;

	int szClntAddr;
	char message[] = "Hello World!";

	if (argc != 2) //인자가 2개->파일명 포트번호
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1); // 강제 종료
	}

	//1. WSAStartup
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!"); //오류 메시지를 가지고 오류를 처리할 함수로 이동

	//2. socket()
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	//3. memset()
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	//4. bind()
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");
	//5. listen()
	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");
	//6. accept()
	szClntAddr = sizeof(clntAddr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
	if (hClntSock == INVALID_SOCKET)
		ErrorHandling("accept() error");
	//7. send()
	send(hClntSock, message, sizeof(message), 0);
	//8. closesocket()
	closesocket(hClntSock);
	closesocket(hServSock);
	//9. WSAcleanup()
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

