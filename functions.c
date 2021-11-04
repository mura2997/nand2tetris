
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct variable {
    char name[20];  // 変数の名前
    short address;  // 変数のアドレス
} VARIABLE;

VARIABLE addVariable(char *buf, int rn) {
    VARIABLE v;
    
    char name[20];
    sscanf(buf, "(%[^)])", name);  // (%s[^)]はLOOP)のままになる
    // printf("%s\n", name);  
    strcpy(v.name, name);
    v.address = rn;
    
    return v;
}



void trimStr(char *str)  
    // スペースを削除する
    // コメントを削除する
    // 文字列が結果的に改行('\n')だけになったときは,存在を消す(先頭を'\0'にする)
{
	char *p = str;				// 当該文字列の先頭を指すポインターを用意する
    char *q = str;
	while (*str != '\0') {		// 当該文字列の末尾に到達しない間、処理を続ける
        if (*str == '/') {     //コメント行は読まない
            break;
        }
		if (!isspace(*str)) {	// 着目している文字が空白でなければ
			*p = *str;			// その文字を答えに格納する
			p++;				// 答えのポインターを１つ進める
		}
        
		str++;					// 当該文字列用のポインターは、とにかく１つ進める
	}
    *p = '\n';
	*(p+1) = '\0';					// 答えの文字列を終端させる

    if (*q == '\n'){            //結果的に改行文字だけなら，文字列を"\0"にする
    *p = '\0';
    }
}



/*機械語用の関数 */

int existInSymbolTable(char s[], VARIABLE *symbolTable, int len) {
    // symbolTableに存在するなら，何番目かを返す
    
    for (int i = 0; i <= len; ++i) {
        int c = strcmp(s, symbolTable[i].name);
        if (c == 0) {
            return i ;
        } 
    }
    return -1; // 存在しない場合
}

short machineCode(char buf[], VARIABLE *symbolTable, short *ramAddress, int *len) {
    if (buf[0] == '@') {
        // A-命令
        char *s = &(buf[1]);  // buf[0]は@
        if ('0' <= *s && *s <= '9') {
            // ただの数字の時は,そのまま機械語にする
            return atoi(s);
            
        } else {
            int i = existInSymbolTable(s, symbolTable, *len); // symbolTableに存在するか
            if ( i > -1) {
                // symbolTableに存在するときは,その変数のアドレスを返す
                return symbolTable[i].address;
            } else {
                // symbolTableに存在しないときは作る
                VARIABLE v;
                strcpy(v.name, s);
                v.address = *ramAddress;
                
                ++*ramAddress;
                return *ramAddress - 1;
            }

        }

    }
}