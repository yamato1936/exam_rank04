#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void unexpected(char c) {
    if (c) printf("Unexpected token '%c'\n", c);
    else printf("Unexpected end of input\n");
    exit(1); // エラー時はすぐ終了してOK
}

int accept(char **s, char c) {
    if (**s == c) {
        (*s)++;
        return 1;
    }
    return 0;
}

void expect(char **s, char c) {
    if (!accept(s, c)) unexpected(**s);
}

// 関数のプロトタイプ宣言
int parse_expr(char **s);

// 1. 数字とカッコの処理
int parse_factor(char **s) {
    if (isdigit(**s)) {
        int val = **s - '0';
        (*s)++;
        return val;
    }
    if (accept(s, '(')) {
        int val = parse_expr(s); // カッコの中は新しい式として計算
        expect(s, ')');
        return val;
    }
    unexpected(**s);
    return 0;
}

// 2. 掛け算の処理
int parse_term(char **s) {
    int val = parse_factor(s);
    while (accept(s, '*')) {
        val *= parse_factor(s); // その場で掛け算
    }
    return val;
}

// 3. 足し算の処理
int parse_expr(char **s) {
    int val = parse_term(s);
    while (accept(s, '+')) {
        val += parse_term(s); // その場で足し算
    }
    return val;
}

int main(int argc, char **argv) {
    if (argc != 2) return 1;
    char *s = argv[1];
    
    int result = parse_expr(&s); // 式を計算
    
    if (*s) unexpected(*s);      // 最後に余計な文字が残っていないかチェック
    
    printf("%d\n", result);
    return 0;
}
