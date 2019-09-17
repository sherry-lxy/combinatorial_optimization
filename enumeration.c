// #define _CRT_SECURE_NO_WARNINGS 1 // 入力エラー

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // 文字列
#include <time.h> // random, time
#include <math.h> // ルート

#define N 493

double dis(double x1, double x2, double y1, double y2); // 2次元ユークリッド距離
double list(int *num, int n, double *X, double *Y); // リストを生成する(n: データ数)
void perm(int i, double *X, double *Y, int n, int *num, double *distance); // 最短距離を求める(i番目固定し i+1 番目以後交換する，x座標，y座標，データ数，列番号，最適解)

int main(void) {
    int num[N]; // イタレーション
    double X[N + 1], Y[N + 1]; // data
    int number; // 列番号
    double x, y; // 座標
    char fname[128]; // 読み込むファイルの名前を格納する変数
    FILE *fp; // ファイル名は 128 文字まで対応可能にする
    int i, j; // for
    int n; // データ数
    double min = 1000000000; // 最小値
    double distance[20]; // 距離
    clock_t start, end; // 処理時間を測る

    printf("input filename: "); // ファイル名の入力を要求
    fgets(fname, sizeof(fname), stdin); // 標準入力からファイル名を取得
    fname[strlen(fname) - 1] = '\0'; // 最後の改行コードを除去
    fflush(stdin); // 128文字を超えた入力を標準入力から捨てる
    fp = fopen(fname, "r"); // ファイルを読み込みモードで開く

    printf("使うデータ数：");
    scanf("%d", &n);

    for (i = 0; i < n; i++) { // データを格納する
	fscanf(fp, "%d %lf %lf", &number, &x, &y);
	X[i] = x;
	Y[i] = y;
	num[i] = number;
	// printf("x=%f,y=%f\n",Data_x[i], Data_y[i]);
    }

    fclose(fp); // ファイルを閉じる

    distance[0] = min; // 0番目は最短距離を格納するためにminで初期化する
    for (i = 1; i <= n; i++) { // 1番目以後はルートを保存する
	distance[i] = -1;
    }

    start = clock(); // time start
    perm(0, X, Y, n, num, distance); // 列挙法で最適解を求める
    end = clock(); // time finish

    // 結果出力
    printf("最適解：dis = %f  \n最短ルート：", distance[0]);
    for (j = 1; j <= n; j++) {
	if (j != n) {
	    printf(" %d →", (int)distance[j]);
	}
	else {
	    printf(" %d ", (int)distance[j]);
	}
    }
    printf("\n");
    printf("処理時間：%f [s]\n", (double)(end - start) / CLOCKS_PER_SEC);

    // system("pause"); // 続行するには何かキーを押してください . . .

    return 0;
}

// 2次元ユークリッド距離
double dis(double x1, double x2, double y1, double y2) {
    double dis;

    dis = pow((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2), 0.5) + 0.5;

    return dis;
}

// 巡回路長を求める
double list(int *num, int n, double *X, double *Y) {
    int i; // for
    double sum; // 総距離

    sum = dis(X[num[0]], X[num[n - 1]], Y[num[0]], Y[num[n - 1]]); // 始点と終点の距離を先に計算する

    for (i = 0; i < n - 1; i++) {
	sum += dis(X[num[i]], X[num[i + 1]], Y[num[i]], Y[num[i + 1]]);
    }

    return sum;
}

// 最短距離を求める(i番目固定し i+1 番目以後交換する，x座標，y座標，データ数，列番号，最適解)
void perm(int i, double *X, double *Y, int n, int *num, double *distance) {
    int j; // 交換するところ
    int tmp; // 交換
    double cost; // 最短コスト
    int ite; // ルート

    if (i < n - 2) {
	perm(i + 1, X, Y, n, num, distance); // i+1 番目以降の順列を生成する
	for (j = i + 1; j < n; j++) { 
	    // i 番目の要素を j 番目の要素と交換
	    tmp = num[i];
	    num[i] = num[j];
	    num[j] = tmp;

	    perm(i + 1, X, Y, n, num, distance); // i+1 番目以降の順列を生成する

	    // 元の順列に戻す
	    tmp = num[i];
	    num[i] = num[j];
	    num[j] = tmp;

	}
    }
    else { // 順列の添字が n-1 逮したら，巡回路長を計算する
	cost = list(num, n, X, Y);
	// printf("dis = %f\n\n", cost);

	if (cost < distance[0]) { // 0番目で格納している距離と比較して，短ったら交換する
	    distance[0] = cost;
	    for (ite = 1; ite <= n; ite++) { // これに対応するルートも格納する
		distance[ite] = num[ite - 1];
	    }
/*
	    printf("dis = %f  \nルート：", distance[0]);
	    for (j = 1; j <= n; j++) {
		if (j != n) {
		    printf(" %d →", (int)distance[j]);
		}
		else {
		    printf(" %d ", (int)distance[j]);
		}
	    }
	    printf("\n");
*/
	}
    }
}
