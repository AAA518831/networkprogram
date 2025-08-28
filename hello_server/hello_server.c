//서버 프로그램(정보주는 쪽)
//클라이언트와 연결하여 "Hello World!" 메시지를 보내는 역할
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>//중요: 소켓 프로그램을 위한 헤더파일
void ErrorHandling(char* message); //에러를 처리하는 함수

int main(int argc, char* argv[])
{ 
    WSADATA  wsaData; //윈도우소켓 초기화정보를 저장하는 변수 선언
    SOCKET hServSock, hClntSock; //서버 소켓과 클라이언트 소켓을 위한 변수
    SOCKADDR_IN servAddr, clntAddr; //서버와 클라이언트의 주소 정보를 저장할 구조체(서로 다른 타입의 변수를 모음)

    int szClntAddr; //클라이언트 주소의 크기(사이즈)를 저장할 변수
    char message[] = "Hello World!"; //문자열, 클라이언트에 전송할 메세지

    if (argc != 2)//실행시킬 때 명령인자를 2개 써야 하는데 그게 틀렸을 경우 
    {
        printf("올바른 사용법 : %s <port>\n", argv[0]); //올바른 방법이라고 출력
        exit(1); //프로그램 종료
    } //예) hello_server(파일명) 9190(포트번호)
    //1)WSAStartup() : Windows 소켓 초기화를 위한 함수
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //성공하면 0
        //2.2는 소켓 라이브러리의 버전을 의미. 2.2 버전을 사용하겠다는 의미
        ErrorHandling("WSAStartup() error!"); //2.2가 아니면 에러 함수로 이동


    //2)socket() : 서버 소켓(리스닝소켓) 생성. (새 전화기 장만)
    hServSock = socket(PF_INET, SOCK_STREAM, 0);
    //새로운 소켓을 생성, PF_INET은 IPv4 프로토콜을 사용하겠다는 의미
    //SOCK_STREAM은 TCP 소켓을 의미   
    //0: 프로토콜 인수입니다. SOCK_STREAM 소켓의 경우 프로토콜은 일반적으로 TCP인 기본 프로토콜을 선택하기 위해 0으로 설정
    if (hServSock == INVALID_SOCKET) //소켓 생성 안됨?
        ErrorHandling("socket() error"); //에러 함수로 이동

    //3)memset() :주소정보 초기화
    memset(&servAddr, 0, sizeof(servAddr)); //servAddr 구조체를 초기화
    servAddr.sin_family = AF_INET; //ipv4(인터넷) 주소체계 정의 
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); //서버의 주소 설정
     //INADDR_ANY : 모든 ip 의미(어떤 ip가 되든 수락하도록 설정)
    servAddr.sin_port = htons(atoi(argv[1])); //포트번호 설정
     //addr->ip 주소 의미(32비트), port->포트번호(16비트)

    //4)bind() : 소켓에 주소정보 할당(IP와 PORT번호의 할당). (전화번호 부여)
    if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        ErrorHandling("bind() error");

    //5)listen() : 연결요청이 가능한 상태의 소켓(연결요청 가능상태로 변경). (전화를 기다리는 상태)
    if (listen(hServSock, 5) == SOCKET_ERROR)
        ErrorHandling("listen() error");


    //6)accept() : 연결요청이 수락되어야 데이터의 송수신이 가능. (전화가 걸려옴)
    szClntAddr = sizeof(clntAddr); //클라이언트 주소의 크기를 저장
    hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
    if (hClntSock == INVALID_SOCKET)
        ErrorHandling("accept() error");


    //7)send() : 소켓을 통해 클라이언트에게 메시지 전송. (전화를 말을 함.)
    send(hClntSock, message, sizeof(message), 0);
    //send() 함수는 클라이언트 소켓을 통해 "Hello World!" 메시지를 전송


    //8)closesocket() : 소켓 연결 종료. (전화 종료)
    closesocket(hClntSock); //클라이언트 종료
    closesocket(hServSock); //서버 종료

    //9)WSACleanup() : 윈도우 소켓 라이브러리 해제(종료).                                                                                                                                                                          
    WSACleanup();
    return 0;
}


//에러가 발생시 에러메시지를 출력하고 종료하는 함수
void ErrorHandling(char* message)
{
    fputs(message, stderr); //에러메시지 출력
    fputc('\n', stderr); //줄바꿈
    exit(1); //프로그램 종료
}
