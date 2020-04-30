#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

void batch_LLGMN(int c, int H, int m, int data, double epsilon, double** input_con, double** input_T, double*** w);
void online_LLGMN(int c, int H, int m, int data, double epsilon, double** input_con, double** input_T, double*** w);
void identification(int c, int H, int m, int data, double** Input_con, double** Input_T, double*** w);

int main()
{
	int i, j, k;
	int answer = 0;//逐次か一括か
	int c = 4;//クラス
	int d = 2;//入力次元
	int H;//非線形変換後の次元
	int m = 2;//コンポーネント数
	int data = 800;//データ数
	int cnt = 0;//カウント
	double epsilon = 0.001;
	double*** w;//重み
	double** input;//学習入力データ
	double** input_con;//入力データを変換したやつ
	double** input_T;//学習データ
	double** Input;//識別入力データ
	double** Input_con;//識別データを変換したやつ
	double** Input_T;//識別データ
	H = 1 + d * (d + 3) / 2;

	FILE* fp1,*fp2,*fp3,*fp4;
	errno_t error,error2,error3,error4;
	error = fopen_s(&fp1, "lea_sig.csv", "r");
	error2 = fopen_s(&fp2, "lea_T_sig.csv", "r");
	error3 = fopen_s(&fp3, "dis_sig.csv", "r");
	error4 = fopen_s(&fp4, "dis_T_sig.csv", "r");
	//動的確保
	input = (double**)malloc(sizeof(double*) * data); //double*型のスペースを要素数（[☆][]）の分だけ確保する。
	Input = (double**)malloc(sizeof(double*) * data);
	for (i = 0; i < data; i++)
	{
		input[i] = (double*)malloc(sizeof(double) * d); //double型のスペースを要素数（[][☆]）の分だけ確保する。
		Input[i] = (double*)malloc(sizeof(double) * d);
	}

	input_con = (double**)malloc(sizeof(double*) * data); //double*型のスペースを要素数（[☆][]）の分だけ確保する。
	Input_con = (double**)malloc(sizeof(double*) * data);
	for (i = 0; i < data; i++)
	{
		input_con[i] = (double*)malloc(sizeof(double) * H); //double型のスペースを要素数（[][☆]）の分だけ確保する。
		Input_con[i] = (double*)malloc(sizeof(double) * H);
	}

	input_T = (double**)malloc(sizeof(double*) * data); //double*型のスペースを要素数（[☆][]）の分だけ確保する。
	Input_T = (double**)malloc(sizeof(double*) * data);
	for (i = 0; i < data; i++)
	{
		input_T[i] = (double*)malloc(sizeof(double) * c); //double型のスペースを要素数（[][☆]）の分だけ確保する。
		Input_T[i] = (double*)malloc(sizeof(double) * c);
	}
	if (error != 0)
		printf("ファイルを開けませんでした");
	else {
		for (i = 0; i < data; i++) {
			for (j = 0; j < d; j++) {
				//教師入力データ読み込み
				if(fscanf_s(fp1, "%lf, ", &input[i][j])!='\0');
			}
			printf("\n");
		}
	}
	if (error2 != 0)
		printf("ファイルを開けませんでした");
	else {
		for (i = 0; i < data; i++) {
			for (j = 0; j < c; j++) {
				//教師入力データ読み込み
				if (fscanf_s(fp2, "%lf, ", &input_T[i][j]) != '\0');
			}
			printf("\n");
		}
	}
	if (error3 != 0)
		printf("ファイルを開けませんでした");
	else {
		for (i = 0; i < data; i++) {
			for (j = 0; j < d; j++) {
				//教師入力データ読み込み
				if (fscanf_s(fp3, "%lf, ", &Input[i][j]) != '\0');
			}
			printf("\n");
		}
	}
	if (error4 != 0)
		printf("ファイルを開けませんでした");
	else {
		for (i = 0; i < data; i++) {
			for (j = 0; j < c; j++) {
				//教師入力データ読み込み
				if (fscanf_s(fp4, "%lf, ", &Input_T[i][j]) != '\0');
			}
			printf("\n");
		}
	}
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	//入力ベクトルに非線形演算による前処理を施し、変換
	for (i = 0;i < data;i++) {
		input_con[i][0] = 1;
		input_con[i][1] = input[i][0];
		input_con[i][2] = input[i][1];
		input_con[i][3] = input[i][0] * input[i][0];
		input_con[i][4] = input[i][0] * input[i][1];
		input_con[i][5] = input[i][1] * input[i][1];
	}
	for (i = 0;i < data;i++) {
		Input_con[i][0] = 1;
		Input_con[i][1] = Input[i][0];
		Input_con[i][2] = Input[i][1];
		Input_con[i][3] = Input[i][0] * Input[i][0];
		Input_con[i][4] = Input[i][0] * Input[i][1];
		Input_con[i][5] = Input[i][1] * Input[i][1];
	}
	printf("クラス数、コンポーネント数、入力次元をそれぞれ順に指定してください\n");
	printf("クラス数 = ");
	scanf_s("%d", &c);
	printf("コンポーネント数 =");
	scanf_s("%d", &m);
	printf("入力次元数 = ");
	scanf_s("%d", &d);

	w = (double***)malloc(sizeof(double**) * c);
	for (i = 0; i < c; i++)
	{
		w[i] = (double**)malloc(sizeof(double*) * m);
		for (j = 0; j < m; j++) {
			w[i][j] = (double*)malloc(sizeof(double) * H);
		}
	}
	
	srand(time(NULL));
	for (i = 0;i < c;i++) {
		for (j = 0;j < m;j++) {
			for (k = 0;k < H;k++) {
				w[i][j][k] = ((double)rand() / RAND_MAX) * 2 - 1;
			}
		}
	}
	//重みが0のところを0にする
	for (i = 0;i < H;i++) {
		w[c-1][m-1][i] = 0;
	}

	for (i = 0;i < c;i++) {
		for (j = 0;j < m;j++) {
			for (k = 0;k < H;k++) {
			}
		}
	}

	printf("逐次学習1 一括学習2\n");
	scanf_s("%d", &answer);
	if (answer == 1) {
		online_LLGMN(c, H, m, data, epsilon, input_con, input_T, w);
	}
	else if (answer == 2) {
		batch_LLGMN(c, H, m, data, epsilon, input_con, input_T, w);
	}
	identification(c, H, m, data, Input_con, Input_T, w);

	return 0;

}

void batch_LLGMN(int c, int H, int m, int data, double epsilon, double** input_con, double** input_T, double*** w)
{
	int i, j, k,l;
	int cnt = 0;//カウント用
	double I[800][4][2] = { 0 };//前の層の出力を受け取る2層目の入力配列
	double I_sum[800] = { 0 };//2層目の分母の値を格納する変数
	double Y[800][6] = { 0 };//3層目の出力
	double O[800][6] = { 0 };//出力配列
	double O_2[800][4][2] = { 0 };//2層目の出力を格納する配列
	double J = 0;//評価関数
	double sum_grad[10][10][10] = { 0 };//勾配を求めるときの配列、左からクラス、コンポーネント、Hに対応
	
	do {
		//初期化
		J = 0;
		for (i = 0;i < 800;i++) {
			I_sum[i] = 0;
		}
		for (i = 0;i < 800;i++) {
			for (j = 0;j < 6;j++) {
				Y[i][j] = 0;
				O[i][j] = 0;
			}
		}

		for (i = 0;i < 800;i++) {
			for (j = 0;j < 4;j++) {
				for (k = 0;k < 2;k++) {
					I[i][j][k] = 0;
					O_2[i][j][k] = 0;
				}
			}
		}
		for (i = 0;i < 10;i++) {
			for (j = 0;j < 10;j++) {
				for (k = 0;k < 10;k++) {
					sum_grad[i][j][k] = 0;
				}
			}
		}

		for (l = 0;l < data;l++) {
			//1層目の出力
			for (i = 0;i < H;i++) {
				O[l][i] = input_con[l][i];
			}

			//2層目の入力
			for (i = 0;i < c;i++) {
				for (j = 0;j < m;j++) {
					for (k = 0;k < H;k++) {
						I[l][i][j] += O[l][k] * w[i][j][k];
					}
				}
			}
			//出力
			for (i = 0;i < c;i++) {
				for (j = 0;j < m;j++) {
					I_sum[l] += exp(I[l][i][j]);//分母をまず求めている
				}
			}

			for (i = 0;i < c;i++) {
				for (j = 0;j < m;j++) {
					O_2[l][i][j] = exp(I[l][i][j]) / I_sum[l];
				}
			}
			//3層目
			for (i = 0;i < c;i++) {
				for (j = 0;j < m;j++) {
					Y[l][i] += O_2[l][i][j];
				}
			}
		}

		for (l = 0;l < data;l++) {
			for (i = 0;i < c;i++) {
				J += -input_T[l][i] * log(Y[l][i]);
			}
		}

		//重みの更新（一括学習）
		for (k = 0;k < H;k++) {
			for (i = 0;i < c;i++) {
				for (j = 0;j < m;j++) {
					for (l = 0;l < data;l++) {
						sum_grad[i][j][k] += (Y[l][i] - input_T[l][i]) * (O_2[l][i][j] / Y[l][i]) * O[l][k];
					}
				}
			}
		}
		for (k = 0;k < H;k++) {
			for (i = 0;i < c;i++) {
				for (j = 0;j < m;j++) {
					w[i][j][k] = w[i][j][k] - epsilon * sum_grad[i][j][k];
				}
			}
		}
		cnt++;
		printf("count = %d J = %lf\n", cnt, J);

	} while (cnt<1000);
	printf("学習の終了\n");
	printf("count = %d  J = %f\n", cnt, J);

}

//逐次学習
void online_LLGMN(int c, int H, int m, int data, double epsilon, double** input_con, double** input_T, double*** w)
{
	int i, j, k, l;
	int cnt = 0;//カウント用
	double I[10][10] = { 0 };//前の層の出力を受け取る2層目の入力配列
	double I_sum = 0;//2層目の分母の値を格納する変数
	double Y[10] = { 0 };//3層目の出力
	double O[10] = { 0 };//出力配列
	double O_2[10][10] = { 0 };//2層目の出力を格納する配列
	double J[800] = { 0 };
	double J_sum = 0;//評価関数
	double sum_grad[10][10][10] = { 0 };//勾配を求めるときの配列、左からクラス、コンポーネント、Hに対応

	do {
		J_sum = 0;
		for (i = 0;i < data;i++) {
			J[i] = 0;
		}
		for (l = 0;l < data;l++) {
			//初期化
			I_sum = 0;
			for (i = 0;i < 10;i++) {
				Y[i] = 0;
				O[i] = 0;
			}
			for (i = 0;i < 10;i++) {
				for (j = 0;j < 10;j++) {
					I[i][j] = 0;
					O_2[i][j] = 0;
				}
			}
			for (i = 0;i < 10;i++) {
				for (j = 0;j < 10;j++) {
					for (k = 0;k < 10;k++) {
						sum_grad[i][j][k] = 0;
					}
				}
			}

			//1層目の出力
			for (i = 0;i < H;i++) {
				O[i] = input_con[l][i];
			}

			//2層目の入力
			for (i = 0;i < c;i++) {
				for (j = 0;j < m;j++) {
					for (k = 0;k < H;k++) {
						I[i][j] += O[k] * w[i][j][k];
					}
				}
			}
			//出力
			for (i = 0;i < c;i++) {
				for (j = 0;j < m;j++) {
					I_sum += exp(I[i][j]);//分母をまず求めている
				}
			}

			for (i = 0;i < c;i++) {
				for (j = 0;j < m;j++) {
					O_2[i][j] = exp(I[i][j]) / I_sum;
				}
			}
			//3層目
			for (i = 0;i < c;i++) {
				for (j = 0;j < m;j++) {
					Y[i] += O_2[i][j];
				}
			}
		
			for (i = 0;i < c;i++) {
				J[l] += -input_T[l][i] * log(Y[i]);
			}

			//重みの更新
			for (k = 0;k < H;k++) {
				for (i = 0;i < c;i++) {
					for (j = 0;j < m;j++) {
						sum_grad[i][j][k] = (Y[i] - input_T[l][i]) * (O_2[i][j] / Y[i]) * O[k];
					}
				}
			}
			for (k = 0;k < H;k++) {
				for (i = 0;i < c;i++) {
					for (j = 0;j < m;j++) {
						w[i][j][k] = w[i][j][k] - epsilon * sum_grad[i][j][k];
					}
				}
			}
		}
		for (i = 0;i < data;i++) {
			J_sum += J[i];
		}
		cnt++;
		printf("count = %d J = %lf\n", cnt, J_sum);
	} while (cnt < 1000);
	printf("学習の終了\n");
	printf("count = %d  J = %f\n", cnt, J_sum);

}

//識別関数
void identification(int c, int H, int m, int data, double** Input_con, double** Input_T, double ***w)
{
	int i, j, k, l;
	double cnt = 0;
	double I[800][4][2] = { 0 };//前の層の出力を受け取る2層目の入力配列
	double I_sum[800] = { 0 };//2層目の分母の値を格納する変数
	double Y[800][6] = { 0 };//3層目の出力
	double O[800][6] = { 0 };//出力配列
	double O_2[800][4][2] = { 0 };//2層目の出力を格納する配列
	double J = 0;//評価関数

	for (l = 0;l < data;l++) {
		//1層目の出力
		for (i = 0;i < H;i++) {
			O[l][i] = Input_con[l][i];
		}

		//2層目の入力
		for (i = 0;i < c;i++) {
			for (j = 0;j < m;j++) {
				for (k = 0;k < H;k++) {
					I[l][i][j] += O[l][k] * w[i][j][k];
				}
			}
		}
		//出力
		for (i = 0;i < c;i++) {
			for (j = 0;j < m;j++) {
				I_sum[l] += exp(I[l][i][j]);//分母をまず求めている
			}
		}

		for (i = 0;i < c;i++) {
			for (j = 0;j < m;j++) {
				O_2[l][i][j] = exp(I[l][i][j]) / I_sum[l];
			}
		}
		//3層目
		for (i = 0;i < c;i++) {
			for (j = 0;j < m;j++) {
				Y[l][i] += O_2[l][i][j];
			}
		}
	}
	
	printf("識別データの出力\n");
	for (l = 0;l < data;l++) {
		for (i = 0;i < c;i++) {
			J += -Input_T[l][i] * log(Y[l][i]);
			printf("%lf ", Y[l][i]);
		}
		printf("\n");
	}
	for (l = 0;l < 200;l++) {
		if (Y[l][0] > Y[l][1] && Y[l][0] > Y[l][2] && Y[l][0] > Y[l][3]) {
			cnt++;
		}
	}
	for (l = 200;l < 400;l++) {
		if (Y[l][1] > Y[l][0] && Y[l][1] > Y[l][2] && Y[l][1] > Y[l][3]) {
			cnt++;
		}
	}
	for (l = 400;l < 600;l++) {
		if (Y[l][2] > Y[l][0] && Y[l][2] > Y[l][1] && Y[l][2] > Y[l][3]) {
			cnt++;
		}
	}
	for (l = 600;l < 800;l++) {
		if (Y[l][3] > Y[l][0] && Y[l][3] > Y[l][1] && Y[l][3] > Y[l][2]) {
			cnt++;
		}
	}
	printf("識別率 = %lf\n", cnt / data);
}
