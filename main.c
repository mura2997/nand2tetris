
#include <stdio.h>
// #include "variable.c"
// #include "add_table.c" 
#include "functions.c"
// #include "createMachineCode.c"

int main(int argc, char *argv[]) {
    FILE *fpr;
    
    fpr = fopen(argv[1], "r"); 
    // ファイルが存在しない場合はファイルを作成する
    
    if (fpr == NULL) {
        printf("Cannot open this file\n");
        return -1;
    }

    /*
    (Xxx)を変数表symbolTableに追加する
    ((Xxx)を見つけるだけならtrimStrは過剰か...)
    */
    VARIABLE *symbolTable;
    symbolTable = (VARIABLE*)malloc(sizeof(VARIABLE)*10000);
    // ここで予約語を追加する?

    int len = 21;  // 予約語は21個存在するので21番から変数を追加する
    int rn = 0;  // running number: ROMの番号
    char buf[256];
    while(fgets(buf, sizeof(buf), fpr) != NULL) {
        trimStr(buf);
        // printf("%s", buf);
        if (*buf == '(') {
            VARIABLE v;
            v = addVariable(buf, rn);
            symbolTable[len] = v;
            ++len;
            continue;  // bufは(Xxx)なのでrnは変化せず，++rnを実行しない
        }
        if (*buf != '\0') {
            ++rn;
        }
    }
    printf("\n");


    for (int i=0; i < 30; ++i) {
        printf("%d\n", i);
        printf("%s\n", symbolTable[i].name);
        printf("%d\n", symbolTable[i].address);
    }

    /*==========================================*/

    /*
    機械語を出力していく
    変数表にない変数が登場した場合は適宜追加する
    */
    fpr = fopen(argv[1], "r");//これでもう一度はじめから開ける?
    FILE *fpw;
    fpw = fopen(argv[2],"w");

    if (fpr == NULL) {
        printf("Cannot open this file\n");
        return -1;
    }

    short ramAddress = 16;
    while(fgets(buf, sizeof(buf), fpr) != NULL) {
        trimStr(buf);

        if (buf[0] == '\0') continue;
        short mc = machineCode(buf, symbolTable, &ramAddress, &len);

        char mcs[16];
        sprintf(mcs, "%d", mc);
        fputs(mcs, fpw);
        fputs("\n", fpw);

        

    

}
}
