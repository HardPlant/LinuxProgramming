#include <unistd.h>
#include <signal.h>
#include <stdio.h>
void handler(int signum);
int flag = 5;

int main()
{
    struct sigaction act;
    sigset_t set;

//관심있는 시그널 세트 등록
    sigemptyset(&(act.sa_mask));
    sigaddset(&(act.sa_mask), SIGALRM);
    sigaddset(&(act.sa_mask), SIGINT);
    sigaddset(&(act.sa_mask), SIGUSR1);
    sigaddset(&(act.sa_mask), SIGALRM);
//시그널 수신 시 해야 할 행동을 함수로 등록, 지정
    act.sa_handler = handler;
    sigaction(SIGALRM, &act, NULL);
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGUSR1, &act, NULL);

    printf("call raise(SIGUSR1) before blocking\n");
    raise(SIGUSR1);
//시그널 봉쇄
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigprocmask(SIG_SETMASK, &set, NULL);
//총 다섯 번(flag=5)의 시그널 수신, 등록된 함수 호출
    while(flag)
    {
        printf("input SIGINT [%d]\n",flag);
        sleep(1);
    }

//봉쇄된 시그널을 발생시킴 (수신되더라도 무시)
    printf("call kill(getpid(), SIGUSR1) after blocking\n");
    kill(getpid(), SIGUSR1);

//관심 있는 시그널이 도착할 때 까지 무한 대기하는 두 번의 과정

    printf("pause\n");
    printf("pause return %d\n", pause());

    printf("2 sec sleep\n");
    alarm(2);
    pause();
//종료
    return 0;
}
void handler(int signum)
{
    flag--;//플래그 감소시키는 부분

    switch(signum){
        case SIGINT:
            printf("SIGINT(%d)\n", signum);
            break;
        case SIGALRM:
            printf("SIGALRM(%d)\n", signum);
            break;
        case SIGUSR1:
            printf("SIGUSR1(%d)\n", signum);
            break;
        default:
            printf("SIG(%d)\n", signum);
            break;
            
    }

}