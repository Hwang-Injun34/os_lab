#include <unistd.h>
#include <stdio.h>

int main() {
    char *argv[] = {"ls", "-al", NULL};

    // 사용자 정의 환경 변수 설정
    char *envp[] = {
        "MY_NAME=SU_USER",
        "CONF_LEVEL=HIGH",
        NULL
    };

    printf("[부모] 커스텀 환경 변수를 설정하고 ls를 실행합니다.\n");

    if (execve("/bin/ls", argv, envp) == -1) {
        perror("execve 실패");
    }

    return 0;
}
