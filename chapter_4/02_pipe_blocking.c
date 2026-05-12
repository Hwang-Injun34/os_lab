#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
    int pipe_fds[2];
    char buf[100];

    pipe(pipe_fds);

    pid_t pid = fork();

    if(pid == 0){
        close(pipe_fds[0]);

        printf("[자식] 5초 뒤 데이터 전송 ...\n");
        sleep(5);

        write(pipe_fds[1], "Wake up, Parent", 16);
        printf("[자식] 전송 완료\n");
        close(pipe_fds[1]);
    }
    else{
        close(pipe_fds[1]);

        printf("[부모] 자식에게 데이터 요청(Read 호출)\n");

        read(pipe_fds[0], buf, sizeof(buf));

        printf("[부모] 자식에게 받은 데이터: %s\n", buf);
        close(pipe_fds[0]);
    }

    return 0;
}