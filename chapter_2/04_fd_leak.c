#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
    int fd = open("secret.txt", O_RDWR | O_CREAT, 0644);
    write(fd, "TOP_SECRET_DATA\n", 16);

    printf("[부모] - 비밀 파일 열기(FD: %d)\n", fd);
    printf("[부모] - cat으로 변신\n");

    char fd_path[50];
    sprintf(fd_path, "/dev/fd/%d", fd);
    execl("/bin/cat", "cat", fd_path, NULL);
    return 0;
}