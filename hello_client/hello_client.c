//클라이언트 프로그램(정보 받는 쪽)
//IP주소와 포트번호를 사용하여 서버에 연결하고, 서버로부터 메시지를 받아 출력하는 역할 수행
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h> //윈도우소켓 이용 헤더파일

void ErrorHandling(char* message); // 에러 처리 함수 선언

int main(int argc, char* argv[])
{
    WSADATA wsaData; //윈도우 소켓 사용하기 위한 구조체 선언
    SOCKET hSocket; //소켓을 저장할 변수
    SOCKADDR_IN servAddr; //서버 주소 기억 변수

    char message[30]; // 메시지 받을 배열 크기(30)
    int strLen; //문자열 사이즈 기억

    if (argc != 3) //명령줄 인자가 3개 나와야 함
    {
        printf("올바른 사용법 : %s <IP> <port>\n", argv[0]); //3개가 안 나오면 올바른 방법을 알려주고 오류 처리
        exit(1); //프로그램 종료
    } //예) hello_client(파일명) 127.0.0.1(IP주소) 9190(포트번호)

    //1)WSAStartup() : Windows 소켓 초기화를 위한 함수
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");

    //새 전화기 장만
    //서버 소켓(리스닝소켓) 생성
    //2)socket()
    hSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (hSocket == INVALID_SOCKET)
        ErrorHandling("socket() error");

    //3)memset(): 주소정보 초기화
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));


    //4)connect() :  서버의 IP 주소와 포트 번호로 연결을 시도(전화를 건다)
    if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        //SOCKADDR 구조체를 사용하여 서버의 주소 정보를 전달
        ErrorHandling("connect() error!");


    //5)receive() : 소켓으로부터 데이터를 수신. (전화를 듣는다)
    strLen = recv(hSocket, message, sizeof(message) - 1, 0); //서버로부터 메시지를 전송받음
    if (strLen == -1)
        ErrorHandling("read() error!"); //에러 있을때 에러 함수 호출하여 이동
    printf("서버로 부터 온 메시지: %s \n", message); //메시지 출력


    //6)closesocket() : 소켓 연결 종료(전화 종료)
    closesocket(hSocket);

    //7)WSACleanup() : 윈속 라이브러리의 해제
    WSACleanup();
    return 0;
}


// 에러 처리 함수
void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}