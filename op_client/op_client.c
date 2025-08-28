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
    if (argc != 3) { ///프로그램이 실행될 때 IP 주소와 포트 번호를 인자로 받지 않았다면, 사용법을 출력하고 프로그램 종료
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    //윈도우소켓을 초기화,  성공하면 0을 반환하고, 실패하면 0이 아닌 값을 반환
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");

    //소켓 생성
    hSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (hSocket == INVALID_SOCKET)
        ErrorHandling("socket() error");

    //주소 구조체를 0으로 초기화
    memset(&servAdr, 0, sizeof(servAdr));
    // 서버주소를 설정
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = inet_addr(argv[1]);
    servAdr.sin_port = htons(atoi(argv[2]));

    //지정된 IP와 포트로 서버에 연결을 시도
    if (connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
        ErrorHandling("connect() error!");
    else
        puts("연결 성공 ...........");

    fputs("계산할  숫자 개수 : ", stdout);
    scanf("%d", &opndCnt);
    opmsg[0] = (char)opndCnt;

    for (i = 0; i < opndCnt; i++) //피연산자(숫자)의 개수만큼 반복문을 실행
    {
        printf("계산할  숫자 %d: ", i + 1);
        scanf("%d", (int*)&opmsg[i * OPSZ + 1]); //사용자가 입력한 피연산자 값을 배열에 순서대로 저장
    }
    fgetc(stdin); //버퍼에 남아 있는 줄바꿈 문자를 제거합니다. (scanf 이후 버퍼를 비우기 위해 사용)
    fputs("연산자(+, - , * 중 하나): ", stdout); //사용자에게 연산자를 입력받기 위해 메시지를 출력
    scanf("%c", &opmsg[opndCnt * OPSZ + 1]);  //사용자가 입력한 연산자를 opmsg 배열에 저장

    //서버로 입력된 데이터를 전송
    send(hSocket, opmsg, opndCnt * OPSZ + 2, 0);   //서버로 opmsg 배열에 저장된 데이터를 전송합니다

    //서버로부터 계산된 결과를 받아 result 변수에 저장
    recv(hSocket, &result, RLT_SIZE, 0);

    printf("서버로 부터 받은 연산결과 : %d \n", result); //서버로부터 받은 계산 결과를 출력
    closesocket(hSocket); //사용한 소켓을 닫음
    WSACleanup(); // //윈속 라이브러리를 종료하고 모든 리소스를 해제
    return 0;
}

void ErrorHandling(char* message) //에러 메시지를 출력하고 프로그램을 종료하는 함수
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}