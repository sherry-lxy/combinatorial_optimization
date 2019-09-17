// #define _CRT_SECURE_NO_WARNINGS 1 // 入力エラー

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // 文字列
#include <time.h>   // random
#include <math.h>   // ルート

#define MAX 1500 // データ数
// #define LOPT 675 // 理論値

double distance(double x1, double x2, double y1, double y2); // 2次元ユークリッド距離

int main(int argc, char *argv[]) {
    double X[MAX], Y[MAX]; // data
    double x, y; // 座標
    char fname[128]; // 読み込むファイルの名前を格納する変数
    FILE *fp; // ファイル名は 128 文字まで対応可能にする
    FILE *FILE, *error_file; // 書き込みファイル名
    double dis; // 距離
    double dis1, dis2, dis3, dis4; // 2本枝それぞれの距離
    int i, j, p, q; // for
    double before, after; // 変換前と変換後の距離
    double tmp; // 交換
    double error; // 誤差率
    double e[1000]; // イタレーションごとの誤差率
    int ineration; // イタレーション
    int count; // if文の数 交差有り無しの検査
    double LOPT;
    char buf[1024];
    int N = 0;


    LOPT = atof(argv[2]); // char → double
    if(strcmp(argv[1],"LOPT") != 0){
        printf("入力形式：./nn LOPT 理論値 N データ数\n");
        return -1; // エラーでプログラムを終了する
    }
    printf("LOPT = %f\n", LOPT);

    printf("input filename: "); // ファイル名の入力を要求
    fgets(fname, sizeof(fname), stdin); // 標準入力からファイル名を取得
    fname[strlen(fname) - 1] = '\0'; // 最後の改行コードを除去
    fflush(stdin); // 128文字を超えた入力を標準入力から捨てる
    fp = fopen(fname, "r"); // ファイルを読み込みモードで開く

    while(fgets(buf, sizeof(buf), fp)) { // データを格納する
        fscanf(fp, "%lf %lf", &x, &y);
        X[N] = x; // x座標
        Y[N] = y; // y座標

        N++;
        // printf("N = %d\n", N);
    }

    fclose(fp); // ファイルを閉じる

    N = N-1;
    // 最近傍法で求めた距離を計算する
    dis = 0;
    for (i = 0; i < N - 1; i++) {
        dis += distance(X[i], X[i + 1], Y[i], Y[i + 1]);
        //printf("dis = %f\n", dis);
        //printf("Data_x[%d] = %f\n", i, Data_x[i]);
        // printf("dis = %f\n", dis);
    }

    printf("before: dis = %f, X[%d] = %f, X[%d] = %f\n", dis, 0, X[0], N - 1, X[N - 1]);

    e[0] = (dis - LOPT) / LOPT;
    printf("e[0] = %f\n", e[0]);


    ineration = 0; // 初期化
    while(1){
	count = 0;
	for (i = 0; i < N - 1; i++) {
	    for (j = i + 2; j < N - 1; j++) {
		dis1 = distance(X[i], X[i + 1], Y[i], Y[i + 1]); // i ---- i+1
		dis2 = distance(X[j], X[j + 1], Y[j], Y[j + 1]); // j ---- j+1

		before = dis1 + dis2; // 変換前の距離を計算する

		dis3 = distance(X[i], X[j], Y[i], Y[j]); // i ---- j
		dis4 = distance(X[i + 1], X[j + 1], Y[i + 1], Y[j + 1]); // i+1 ---- j+1

		after = dis3 + dis4; // 変換前の距離を計算する

		if (after < before) { // 小さかったら交換する
		    p = i + 1;
		    q = j;
		    while (p != q && p < q) {  // 中の順番も入れ替える
    			tmp = X[p];
    			X[p] = X[q];
    			X[q] = tmp;

    			tmp = Y[p];
    			Y[p] = Y[q];
    			Y[q] = tmp;

    			p++;
    			q--;
    		    }
    		    ineration += 1;
    		    dis = dis - before + after;
    		    e[ineration] = (dis - LOPT) / LOPT;
    		    //printf("e[%d] = %f\n", ineration, e[ineration]);

    		    count += 1;
    		}
	    }
	}
	if(count == 0){
	    break; // 更新なかったらwhile文から抜く
	}
    }

    dis = 0;
    for (i = 0; i < N - 1; i++) {
        dis += distance(X[i], X[i + 1], Y[i], Y[i + 1]);
        //printf("dis = %f\n", dis);
        //printf("Data_x[%d] = %f\n", i, Data_x[i]);
    }

    printf("after: dis = %f, X[%d] = %f, X[%d] = %f\n", dis, 0, X[0], N-1, X[N-1]);

    error = (dis - LOPT) / LOPT; // 誤差率
    printf("誤差率: %f\n", error);

    // 標準入力でデータをファイルに書き込み
    printf("result output filename: "); // ファイル名の入力を要求
    fgets(fname, sizeof(fname), stdin); // 標準入力からファイル名を取得
    fname[strlen(fname) - 1] = '\0'; // 最後の改行コードを除去
    fflush(stdin); // 128文字を超えた入力を標準入力から捨てる

    FILE = fopen(fname, "w");
    if (FILE == NULL) { // 失敗する場合
        printf("error\n");
        return -1;
    }
    for (i = 0; i < N; i++) {
        fprintf(FILE, "%f %f\n", X[i], Y[i]);
    }
    fclose(FILE);

    printf("error output filename: "); // ファイル名の入力を要求
    fgets(fname, sizeof(fname), stdin); // 標準入力からファイル名を取得
    fname[strlen(fname) - 1] = '\0'; // 最後の改行コードを除去
    fflush(stdin); // 128文字を超えた入力を標準入力から捨てる

    FILE = fopen(fname, "w");
    if (FILE == NULL) { // 失敗する場合
        printf("error\n");
        return -1;
    }
    for (i = 0; i < ineration; i++) {
        fprintf(FILE, "%d %f\n", i, e[i]);
    }
    fclose(FILE);

    // system("pause"); // 続行するには何かキーを押してください . . .

    return 0;
}

// 2次元ユークリッド距離
double distance(double x1, double x2, double y1, double y2) {
    double dis;

    dis = pow((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2), 0.5) + 0.5;
	
    return dis;
}
