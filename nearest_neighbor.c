//#define _CRT_SECURE_NO_WARNINGS 1 // 入力エラー

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // 文字列
#include <time.h>   // random
#include <math.h>   // ルート

#define MAX 1500 // 最大配列サイズ

double dis(double x1, double x2, double y1, double y2); // 2次元ユークリッド距離

int main(int argc, char *argv[]) {
    double Data_x[MAX], Data_y[MAX]; // data
    int num[MAX]; // インスタンス
    double X[MAX + 1], Y[MAX + 1]; // 際近傍法で使うdata
    int number; // 列番号
    double x, y; // 座標
    char fname[128]; // 読み込むファイルの名前を格納する変数
    FILE *fp; // ファイル名は 128 文字まで対応可能にする
    char buf[1024];
    int i, j; // for
    int pos; // 位置を決める
    double distance; // 距離
    double min; // 最小値
    double solution; // 最適解
    double error; // 誤差率
    FILE *FILE; // 書き込みファイル名
    int flag[MAX]; // 既につながっている点の保存
    int keep; // flag点の一時的の保存
    int N; // データ数
    double LOPT;

    LOPT = atof(argv[2]); // char → double
    N = atoi(argv[4]); // char → int
    for(i = 0;i < N; i++){
		flag[i] = 0;
    }

    if(strcmp(argv[1],"LOPT") != 0 && strcmp(argv[3],"N") != 0){
    	printf("入力形式：./nn LOPT 理論値\n");
    	return -1; // エラーでプログラムを終了する
    }
    printf("LOPT = %f, N = %d\n", LOPT, N);

    printf("input filename: "); // ファイル名の入力を要求
    fgets(fname, sizeof(fname), stdin); // 標準入力からファイル名を取得
    fname[strlen(fname) - 1] = '\0'; // 最後の改行コードを除去
    fflush(stdin); // 128文字を超えた入力を標準入力から捨てる
    fp = fopen(fname, "r"); // ファイルを読み込みモードで開く

    for(i = 0; i < N; i++) { // データを格納する
		fscanf(fp, "%d %lf %lf", &number, &x, &y);
		Data_x[i] = x; // x座標
		Data_y[i] = y; // y座標
		num[i] = number; // 列番号

		// printf("data_x[%d] = %f, data_y[%d] = %f\n", i, Data_x[i], i, Data_y[i]);
		//N++;
    }

    fclose(fp); // ファイルを閉じる
    // 始点を先に格納する
    srand((unsigned)time(NULL));
    pos = rand() % (N + 1);

    X[0] = Data_x[pos];
    Y[0] = Data_y[pos];
    printf("初期点：X[0] = %f, Y[0] = %f\n", X[0], Y[0]);
    flag[pos] = 1;

    for (i = 0; i < N - 1; i++) {
		min = 1000000; // 最小値の更新
		for (j = 0; j < N; j++) {
	    	if(X[i] == Data_x[j]){ // 同じ点pass
	    	}
	    	else{
				if (flag[j] == 1) { // もう既につながった点はpass
				}
				else {
		    		distance = dis(X[i], Data_x[j], Y[i], Data_y[j]); // 距離の計算
		    		// printf("dis = %f, j = %d\n", distance, j);
		    		if (distance < min) { // 最小値より小さかったら最短距離を更新する
						X[i + 1] = Data_x[j];
						Y[i + 1] = Data_y[j];

						min = distance; // 最小値を更新する
						keep = j; // その点をマークする
		    		}
				}
	    	}
		}
		// printf("i = %d, keep = %d, X[%d] = %f\n", i, keep, i+1, X[i+1]);
		flag[keep] = 1; // つながった点を更新する
		solution += min; // 距離を更新する
    }

    solution += dis(X[0], X[N-1], Y[0], Y[N-1]); // 終点と始点の距離
    X[N] = X[0]; // 最後終点と始点をつながる
    Y[N] = Y[0];
    printf("x[%d] = %f\n", N, X[N]);

    printf("最適解: %f\n", solution);

    error = (solution - LOPT) / LOPT; // 誤差率
    printf("誤差率: %f\n", error);

    // データをファイルに書き込み
    printf("output filename: "); // ファイル名の入力を要求
    fgets(fname, sizeof(fname), stdin); // 標準入力からファイル名を取得
    fname[strlen(fname) - 1] = '\0'; // 最後の改行コードを除去
    fflush(stdin); // 128文字を超えた入力を標準入力から捨てる

    FILE = fopen(fname, "w");
    if (FILE == NULL) { // 失敗する場合
		printf("error\n");
		return -1;
    }

    fprintf(FILE, "    \n"); // ファイルからデータを読み込めるために，一行目を空く
    for (i = 0; i <= N; i++) {
		fprintf(FILE, "%f %f\n", X[i], Y[i]);
    }
    fclose(FILE);

    //system("pause"); // 続行するには何かキーを押してください . . .

    return 0;
}

// 2次元ユークリッド距離
double dis(double x1, double x2, double y1, double y2) {
    double dis;

    dis = pow((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2), 0.5) + 0.5;

    return dis;
}
