//서버 프로그램으로 클라이언트로부터 간단한 연산(더하기, 빼기, 곱하기)을 요청받아 그 결과를 계산한 후 클라이언트에게 돌려줌
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUF_SIZE 1024  //버퍼 크기를 정의
#define OPSZ 4 //각 피연산자의 크기(4바이트)를 정의

void ErrorHandling(char* message); //오류 발생 시 메시지를 출력하고 프로그램을 종료
int calculate(int opnum, int opnds[], char oprator);  //클라이언트로부터 받은 연산을 수행하는 함수

int main(int argc, char* argv[])
{
    WSADATA wsaData;
    SOCKET hServSock, hClntSock;
    char opinfo[BUF_SIZE];
    int result, opndCnt, i;
    int recvCnt, recvLen;
    SOCKADDR_IN servAdr, clntAdr;
    int clntAdrSize;
    if (argc != 2) { //명령줄 인자로 포트 번호가 전달되지 않으면 사용법을 출력하고 프로그램을 종료
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    //윈도우소켓 라이브러리를 초기화함. 초기화 실패 시 오류 메시지를 출력
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");

    //서버 소켓을 생성. 소켓 생성에 실패하면 오류 메시지를 출력
    hServSock = socket(PF_INET, SOCK_STREAM, 0);
    if (hServSock == INVALID_SOCKET)
        ErrorHandling("socket() error");

    memset(&servAdr, 0, sizeof(servAdr));////주소구조체를 0으로 초기화(clear) 
    //서버 IP 주소와 포트 번호를 설정
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAdr.sin_port = htons(atoi(argv[1]));

    //소켓에  IP 주소와 포트 번호를 할당. 실패 시 오류 메시지를 출력
    if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
        ErrorHandling("bind() error");

    //소켓을 수신 대기 상태로 만듬. 실패 시 오류 메시지를 출력.
    if (listen(hServSock, 5) == SOCKET_ERROR)
        ErrorHandling("listen() error");
    clntAdrSize = sizeof(clntAdr); //클라이언트 주소 구조체의 크기를 설정

    for (i = 0; i < 5; i++) //서버는 최대 5번의 클라이언트 연결을 처리하기 위해 반복문을 사용
    {
        opndCnt = 0;   //연산자의 개수

        //클라이언트 연결 요청을 수락하고 클라이언트 소켓을 생성
        hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSize);
        //클라이언트로부터 피연산자의 개수를 수신(클라이언트로부터 데이터 받아옴)
        recv(hClntSock, &opndCnt, 1, 0);

        recvLen = 0;
        //클라이언트로부터 피연산자(숫자)와 연산 기호를 모두 수신할 때까지 반복해서 데이터를 수신
                                         //반복문을 통해 클라이언트로부터 모든 데이터를 받을 때까지 대기
        while ((opndCnt * OPSZ + 1) > recvLen) //opndCnt * OPSZ + 1: 수신해야 할 총 데이터의 크기를 계산
            //루프는 현재까지 수신된 데이터(recvLen)가 이 총 데이터 크기보다 작을 경우 계속 실행
        /* opndCnt는 연산자의 개수를 나타냄
            OPSZ는 각 연산자의 크기(4바이트)
            + 1은 마지막에 연산 기호(+, -, *) 하나가 추가로 포함되기 때문에 필요*/
        {
            recvCnt = recv(hClntSock, &opinfo[recvLen], BUF_SIZE - 1, 0);
            //recvCnt는 이번에 받은 데이터의 길이를 저장
            recvLen += recvCnt;
            //recvLen은 현재까지 받은 데이터의 길이를 저장  


        }
        // 연산을 수행하는 함수 호출하여 그 결과를 계산
        result = calculate(opndCnt, (int*)opinfo, opinfo[recvLen - 1]);
        //연산을 수행하는 함수 호출하여 그 결과를 계산

//계산 결과를 클라이언트에게 전송
        send(hClntSock, (char*)&result, sizeof(result), 0);

        closesocket(hClntSock); //클라이언트 소켓을 닫음
    }
    closesocket(hServSock);  //서버 소켓을 닫음
    WSACleanup();  //서버 소켓을 닫고, 윈도우소켓 라이브러리를 정리
    return 0;
}

int calculate(int opnum, int opnds[], char op) // 수신한 연산기호와 피연산자를 받아 계산을 수행
{
    int result = opnds[0], i; //첫 번째 피연산자를 result에 저장하고, 반복문을 통해 나머지 피연산자들과 연산을 수행

    switch (op) //연산 기호에 따라 덧셈, 뺄셈, 곱셈을 수행
    {
    case '+':
        for (i = 1; i < opnum; i++) result += opnds[i];
        // 첫번째 피연산자가 result에 있고 연산자에 따라 피연산자가 있는동안 연산함
        break;
    case '-':
        for (i = 1; i < opnum; i++) result -= opnds[i];
        break;
    case '*':
        for (i = 1; i < opnum; i++) result *= opnds[i];
        break;
    }
    return result; //계산 결과를 반환
}

void ErrorHandling(char* message) // 오류 메시지를 출력하고 프로그램을 종료
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}