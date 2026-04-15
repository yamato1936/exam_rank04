#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int ft_popen(const char *file, char *const argv[], char type)
{
    int fd[2];
    pid_t pid;

    // 1. 引数チェックとパイプ作成
    if (!file || !argv || !argv[0] || (type != 'r' && type != 'w')) return -1;
    if (pipe(fd) == -1) return -1;

    // 2. forkでプロセスを分ける
    pid = fork();
    if (pid < 0) {
        close(fd[0]);
        close(fd[1]);
        return -1;
    }

    // 3. 子プロセスの処理 (コマンドの実行)
    if (pid == 0) {
        if (type == 'r') 
            dup2(fd[1], 1); // コマンドの出力(1)をパイプの書き込み口に繋ぐ
        else 
            dup2(fd[0], 0); // コマンドの入力(0)をパイプの読み込み口に繋ぐ
        
        // 繋ぎ終わったら元のパイプは両方閉じる
        close(fd[0]);
        close(fd[1]);
        
        execvp(file, argv);
        exit(1); // execvpが失敗した場合
    }

    // 4. 親プロセスの処理 (ファイルディスクリプタを返す)
    if (type == 'r') {
        close(fd[1]); // 親は読み込むだけなので書き込み口を閉じる
        return fd[0]; // 読み込み口を返す
    } else {
        close(fd[0]); // 親は書き込むだけなので読み込み口を閉じる
        return fd[1]; // 書き込み口を返す
    }
}
