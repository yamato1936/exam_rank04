#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int picoshell(char **cmds[]) {
    int i = 0;
    int prev_fd = -1;
    int fd[2];
    int status;
    int ret = 0;

    if (!cmds || !cmds[0]) return 0;

    while (cmds[i]) {
        // 次のコマンドがある場合のみパイプを作成
        int has_next = (cmds[i + 1] != NULL);
        if (has_next) {
            if (pipe(fd) == -1) return 1;
        }

        pid_t pid = fork();
        if (pid < 0) return 1;

        if (pid == 0) { // 子プロセス
            if (prev_fd != -1) {
                dup2(prev_fd, 0); // 前のパイプの読み込み口を標準入力に
                close(prev_fd);
            }
            if (has_next) {
                dup2(fd[1], 1); // 今のパイプの書き込み口を標準出力に
                close(fd[0]);
                close(fd[1]);
            }
            execvp(cmds[i][0], cmds[i]);
            exit(1);
        }

        // 親プロセス
        if (prev_fd != -1) close(prev_fd); // 使い終わった前のパイプを閉じる
        if (has_next) {
            close(fd[1]);       // 親は書き込まないので閉じる
            prev_fd = fd[0];    // 次のループのために読み込み口を保存
        }
        i++;
    }

    // すべての子プロセスを待つ
    while (wait(&status) != -1) {
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            ret = 1;
        }
    }
    return ret;
}
