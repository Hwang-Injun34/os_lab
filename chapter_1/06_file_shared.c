#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>


int main(int arch, char *argv[]){
    int fd = open("test.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);

    if(fd < 0){
        perror("open error");
        return 1;
    }

    write(fd, "BEFORE_FORK\n", 12);

    pid_t pid = fork();

    if(pid == 0){
        write(fd, "CHILD_WRITE\n", 12);
        printf("[자식] 파일에 쓰기 완료\n");
    }
    else{
        wait(NULL); // 자식 먼저 쓸 때가지 대기
        write(fd, "PARENT_WRITE\n", 13);
        printf("[부모] 파일에 쓰기 완료\n");
        close(fd);
    }
    return 0;
}