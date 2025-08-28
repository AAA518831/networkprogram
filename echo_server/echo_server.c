//에코 서버 :  TCP 서버 프로그램으로, 클라이언트로부터 메시지를 받아서 그대로 다시 클라이언트에게 보냄
#include <stdio.h>
#include <stdlib.h>  // 표준 라이브러리 함수를 사용하기 위해 포함
#include <string.h>  // 문자열 처리를 위한 함수를 사용하기 위해 포함
#include <winsock2.h> // 윈도우 소켓 프로그래밍하기 위해 포함

void ErrorHandling(char* message); //에러(오류) 처리하는 함수 선언(함수원형) 

int main(int argc, char* argv[]) //argc: 인자 개수 
{
    WSADATA wsaData;  // 윈도우 소켓 라이브러리 정보를 저장할 구조체
    SOCKET hServSock, hClntSock;  // 서버(리스닝)소켓(핸들 값), 클라이언트 소켓(accept 호출시 반환되는 값) 변수
    char message[1024];  //배열 즉 데이터를 저장할 버퍼
    int strLen, i;   // 문자열 길이와 반복문에서 사용할 변수

    SOCKADDR_IN servAdr, clntAdr;  // 서버소켓, 클라이언트 소켓의 주소정보 담음(구조체)
                                   // ip주소(4byte)와 포트번호(2byte)를 저장
    int clntAdrSize; // 클라이언트 주소 구조체의 크기(사이즈)를 저장할 변수

    if (argc != 2) {  // 명령행 인자가 2개가 아닌 경우, 올바른 사용법을 출력하고 종료
        printf("올바른 사용법 : %s <port>\n", argv[0]);
        exit(1); // 프로그램을 비정상 종료
    } //echo_server 9800 => 실행


    //※WSAStartup() : 윈도우 소켓(윈속)  초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("ERROR: 윈속을 초기화 할 수 없습니다.");

    //1. socket() : 접속대기 소켓 생성
    hServSock = socket(PF_INET, SOCK_STREAM, 0);
    //PF_TNET : ipv4 형식, SOCK_STREAM : tcp(인터넷) 
    if (hServSock == INVALID_SOCKET)
        ErrorHandling("ERROR: 접속 대기 소켓을 생성할 수 없습니다."); //에러 함수로 이동

    memset(&servAdr, 0, sizeof(servAdr));  // memset() : 서버 주소 구조체를 0으로 초기화
    //서버소켓 주소정보 설정
    servAdr.sin_family = AF_INET; //주소 체계를 인터넷으로 설정
    servAdr.sin_addr.s_addr = htonl(INADDR_ANY); //addr: ip주소(4byte => htonl 사용)
    servAdr.sin_port = htons(atoi(argv[1])); //port:소켓 포트번호(2byte => htons 사용)
                  //htons:호스트(h) 에서 네트워크(n) 변환, s(short=2byte)
                  //htonl:호스트(h) 에서 네트워크(n) 변환, l(long=4byte)
                  //호스트: 리틀앤디안 방식으로 저장 
                  // ==> 네트워크: 빅앤디안 방식으로 저장
                  // INADDR_ANY: 모든 IP주소에서 접속을 허용

    //2. bind() : 포트 바인딩(ip주소와 포트번호를 할당)
    if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
        ErrorHandling("ERROR: 소켓에 IP주소와 포트를 바인드 할 수 없습니다.");

    //3. listen() : 접속대기 상태로 전환(접속하기를 기다림)
    if (listen(hServSock, 3) == SOCKET_ERROR) //대기열이 3
        //접속 가능한 대기열의 크기가 3개(동시에 접속 가능한 클라이언트 3개)
        ErrorHandling("ERROR: 리슨 상태로 전환할 수 없습니다.");

    clntAdrSize = sizeof(clntAdr); // 클라이언트 주소 구조체의 크기를 설정   


    //4.1. accept() : 클라이언트 연결을 받아들이고 새로운 소켓 생성(개방)
    for (i = 0; i < 3; i++)  //3번의 클라이언트에게만 제공
    {
        hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSize);
        if (hClntSock == -1)
            ErrorHandling("ERROR: 연결 요청을 수락할 수 없습니다 ");
        else
            printf("새 클라이언트가  연결됨  %d \n", i + 1);  // 연결 성공 시 메시지를 출력, 연결횟수 출력


        //4.2. recv() : 클라이언트로부터 문자열을 수신함.
        while ((strLen = recv(hClntSock, message, 1024, 0)) != 0) {  //소켓으로부터 연결종료 아닐때까지 메시지 수신
            //recv가 0을 반환하면 상대방에 의해 연결이 종료되었음을 나타냄
            if (strLen == SOCKET_ERROR)
                ErrorHandling("ERROR: 클라이언트로 부터 수신할 수 없습니다.");

            // 받은 메시지 버퍼를 null로 초기화
            memset(message + strLen, 0, 1024 - strLen); //// message 버퍼에서 현재 수신된 데이터의 끝부터 버퍼의 나머지 부분을 0으로 초기화

            // 4.3. 수신한 메시지를 출력
            message[strLen] = 0; // 문자열 종료 문자 추가
            printf("수신한 메시지: %s\n", message);


            // 4.3. send() : 수신한 문자열을 그대로 반대로 전송.
            send(hClntSock, message, strLen, 0);
            printf("수신하여 다시 전송한  메시지: %s\n", message);
        } //while 문 종료

        //4.4  closesocket() : 클라이언트가 연결을 종료함.
        closesocket(hClntSock);
        puts("클라이언트 연결이 끊겼습니다.");

    }
    //5. 리슨 소켓 닫기
    closesocket(hServSock);

    //※  WSACleanup() : 소켓 라이브러리 해제
    WSACleanup();
    return 0;
}

void ErrorHandling(char* message) // 에러 메시지를 출력하는 함수
{
    fputs(message, stderr); // 에러 메시지를 표준 에러 출력에 출력
    fputc('\n', stderr);
    exit(1);//강제 종료
}