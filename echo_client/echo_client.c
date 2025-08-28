//에코 클라이언트
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 오래된 소켓 함수를 사용할 때 경고를 억제하기 위해 사용
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h> //윈도우 소켓 프로그래밍에 필요한 함수와 구조체들을 포함

void ErrorHandling(char* message); //에러 메시지를 출력하고 프로그램을 종료하는 함수

int main(int argc, char* argv[]) //argc는 명령줄 인자의 수, argv는 명령줄 인자의 배열
{

    WSADATA wsaData; //윈속 데이터 구조체, 윈속의 초기화 정보가 저장
    SOCKET hSocket; //소켓을 저장할 변수
    char message[1024]; //데이터(메시지)를 저장할 버퍼
    int strLen; //수신한 문자열의 길이를 저장할 변수
    SOCKADDR_IN servAdr;

    if (argc != 3) { // 명령행 인자가 3개가 아닌 경우, 올바른 사용법을 출력하고 종료
        printf("올바른 사용법 : %s <IP> <port>\n", argv[0]);
        exit(1); //echo_client 127.0.0.1(루프백) 9800
    } 
    //※WSAStartup() : 윈속 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("ERROR: 윈속을 초기화 할 수 없습니다.");

    //1. socket() : 접속대기 소켓 생성
    hSocket = socket(PF_INET, SOCK_STREAM, 0); //클라이언트 소켓 생성
    if (hSocket == INVALID_SOCKET)
        ErrorHandling("socket() error");

    memset(&servAdr, 0, sizeof(servAdr));  //memset() : 서버 주소 구조체를 0으로 초기화
    //연결요청할 서버의 정보를 채움
    servAdr.sin_family = AF_INET; //주소 체계를 IPv4로 설정
    servAdr.sin_addr.s_addr = inet_addr(argv[1]); //첫 번째 명령줄 인자로 전달된 IP 주소를 설정
    servAdr.sin_port = htons(atoi(argv[2])); //두 번째 명령줄 인자로 전달된 포트 번호를 설정, 
    // htons 함수는 호스트 바이트 순서를 네트워크 바이트 순서로 변환

//2. connect() : 연결 요청 및 연결
    if (connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR) //연결 요청
        ErrorHandling("ERROR: 서버에 연결할 수 없습니다.");
    else
        puts("연결됨...........");

    //3. send() / recv()  :  채팅 메시지 송/수신
    while (1) //무한루프
    {
        //사용자로부터 문자열을 입력 받는다.
        fputs("데이터 입력하세요(종료는 Q to quit): ", stdout); //사용자에게 메시지를 입력하라고 출력
        fgets(message, 1024, stdin);

        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) //입력한 메시지가 'q'나 'Q'이면 반복문을 종료
            break; //while 문 탈출
        //같으면 0(q-> 종료) -> !(부정) -> 1
        //q-> 1 -> ||(or)은 둘 중 하나라도 1이면 참
        //str~:string:(문자열) 함수 
        //strcmp : 문자열 비교 
        //&& -> and || -> or ! -> not(부정=반대)

        //사용자가 입력한 문자열을 서버에 전송한다.
        send(hSocket, message, strlen(message), 0); 

        //서버로부터 방금 보낸 문자열에 대한 에코 메시지를 수신한다.
        strLen = recv(hSocket, message, 1024 - 1, 0);

        message[strLen] = 0; //문자열을 다 읽고 null문자 넣음            
        printf("서버로부터 전송된 메시지: %s", message);
    }  //while 반복문 끝

    //4. closesocket() : 소켓을 닫고 종료.
    closesocket(hSocket);

    //※   WSACleanup() : 윈속 해제
    WSACleanup();
    return 0;
}

void ErrorHandling(char* message) //에러 메시지를 출력하고 프로그램을 종료하는 함수
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}