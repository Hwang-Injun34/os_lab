#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    printf("-- Hello -- (PID: %d)\n", getpid());

    execl("/bin/ls", "ls", "-l", NULL);

    printf("이 문구가 보이면 exec 실패\n");
    
    return 0;
} 