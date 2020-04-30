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
	int answer = 0;//�������ꊇ��
	int c = 4;//�N���X
	int d = 2;//���͎���
	int H;//����`�ϊ���̎���
	int m = 2;//�R���|�[�l���g��
	int data = 800;//�f�[�^��
	int cnt = 0;//�J�E���g
	double epsilon = 0.001;
	double*** w;//�d��
	double** input;//�w�K���̓f�[�^
	double** input_con;//���̓f�[�^��ϊ��������
	double** input_T;//�w�K�f�[�^
	double** Input;//���ʓ��̓f�[�^
	double** Input_con;//���ʃf�[�^��ϊ��������
	double** Input_T;//���ʃf�[�^
	H = 1 + d * (d + 3) / 2;

	FILE* fp1,*fp2,*fp3,*fp4;
	errno_t error,error2,error3,error4;
	error = fopen_s(&fp1, "lea_sig.csv", "r");
	error2 = fopen_s(&fp2, "lea_T_sig.csv", "r");
	error3 = fopen_s(&fp3, "dis_sig.csv", "r");
	error4 = fopen_s(&fp4, "dis_T_sig.csv", "r");
	//���I�m��
	input = (double**)malloc(sizeof(double*) * data); //double*�^�̃X�y�[�X��v�f���i[��][]�j�̕������m�ۂ���B
	Input = (double**)malloc(sizeof(double*) * data);
	for (i = 0; i < data; i++)
	{
		input[i] = (double*)malloc(sizeof(double) * d); //double�^�̃X�y�[�X��v�f���i[][��]�j�̕������m�ۂ���B
		Input[i] = (double*)malloc(sizeof(double) * d);
	}

	input_con = (double**)malloc(sizeof(double*) * data); //double*�^�̃X�y�[�X��v�f���i[��][]�j�̕������m�ۂ���B
	Input_con = (double**)malloc(sizeof(double*) * data);
	for (i = 0; i < data; i++)
	{
		input_con[i] = (double*)malloc(sizeof(double) * H); //double�^�̃X�y�[�X��v�f���i[][��]�j�̕������m�ۂ���B
		Input_con[i] = (double*)malloc(sizeof(double) * H);
	}

	input_T = (double**)malloc(sizeof(double*) * data); //double*�^�̃X�y�[�X��v�f���i[��][]�j�̕������m�ۂ���B
	Input_T = (double**)malloc(sizeof(double*) * data);
	for (i = 0; i < data; i++)
	{
		input_T[i] = (double*)malloc(sizeof(double) * c); //double�^�̃X�y�[�X��v�f���i[][��]�j�̕������m�ۂ���B
		Input_T[i] = (double*)malloc(sizeof(double) * c);
	}
	if (error != 0)
		printf("�t�@�C�����J���܂���ł���");
	else {
		for (i = 0; i < data; i++) {
			for (j = 0; j < d; j++) {
				//���t���̓f�[�^�ǂݍ���
				if(fscanf_s(fp1, "%lf, ", &input[i][j])!='\0');
			}
			printf("\n");
		}
	}
	if (error2 != 0)
		printf("�t�@�C�����J���܂���ł���");
	else {
		for (i = 0; i < data; i++) {
			for (j = 0; j < c; j++) {
				//���t���̓f�[�^�ǂݍ���
				if (fscanf_s(fp2, "%lf, ", &input_T[i][j]) != '\0');
			}
			printf("\n");
		}
	}
	if (error3 != 0)
		printf("�t�@�C�����J���܂���ł���");
	else {
		for (i = 0; i < data; i++) {
			for (j = 0; j < d; j++) {
				//���t���̓f�[�^�ǂݍ���
				if (fscanf_s(fp3, "%lf, ", &Input[i][j]) != '\0');
			}
			printf("\n");
		}
	}
	if (error4 != 0)
		printf("�t�@�C�����J���܂���ł���");
	else {
		for (i = 0; i < data; i++) {
			for (j = 0; j < c; j++) {
				//���t���̓f�[�^�ǂݍ���
				if (fscanf_s(fp4, "%lf, ", &Input_T[i][j]) != '\0');
			}
			printf("\n");
		}
	}
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	//���̓x�N�g���ɔ���`���Z�ɂ��O�������{���A�ϊ�
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
	printf("�N���X���A�R���|�[�l���g���A���͎��������ꂼ�ꏇ�Ɏw�肵�Ă�������\n");
	printf("�N���X�� = ");
	scanf_s("%d", &c);
	printf("�R���|�[�l���g�� =");
	scanf_s("%d", &m);
	printf("���͎����� = ");
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
	//�d�݂�0�̂Ƃ����0�ɂ���
	for (i = 0;i < H;i++) {
		w[c-1][m-1][i] = 0;
	}

	for (i = 0;i < c;i++) {
		for (j = 0;j < m;j++) {
			for (k = 0;k < H;k++) {
			}
		}
	}

	printf("�����w�K1 �ꊇ�w�K2\n");
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
	int cnt = 0;//�J�E���g�p
	double I[800][4][2] = { 0 };//�O�̑w�̏o�͂��󂯎��2�w�ڂ̓��͔z��
	double I_sum[800] = { 0 };//2�w�ڂ̕���̒l���i�[����ϐ�
	double Y[800][6] = { 0 };//3�w�ڂ̏o��
	double O[800][6] = { 0 };//�o�͔z��
	double O_2[800][4][2] = { 0 };//2�w�ڂ̏o�͂��i�[����z��
	double J = 0;//�]���֐�
	double sum_grad[10][10][10] = { 0 };//���z�����߂�Ƃ��̔z��A������N���X�A�R���|�[�l���g�AH�ɑΉ�
	
	do {
		//������
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
			//1�w�ڂ̏o��
			for (i = 0;i < H;i++) {
				O[l][i] = input_con[l][i];
			}

			//2�w�ڂ̓���
			for (i = 0;i < c;i++) {
				for (j = 0;j < m;j++) {
					for (k = 0;k < H;k++) {
						I[l][i][j] += O[l][k] * w[i][j][k];
					}
				}
			}
			//�o��
			for (i = 0;i < c;i++) {
				for (j = 0;j < m;j++) {
					I_sum[l] += exp(I[l][i][j]);//������܂����߂Ă���
				}
			}

			for (i = 0;i < c;i++) {
				for (j = 0;j < m;j++) {
					O_2[l][i][j] = exp(I[l][i][j]) / I_sum[l];
				}
			}
			//3�w��
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

		//�d�݂̍X�V�i�ꊇ�w�K�j
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
	printf("�w�K�̏I��\n");
	printf("count = %d  J = %f\n", cnt, J);

}

//�����w�K
void online_LLGMN(int c, int H, int m, int data, double epsilon, double** input_con, double** input_T, double*** w)
{
	int i, j, k, l;
	int cnt = 0;//�J�E���g�p
	double I[10][10] = { 0 };//�O�̑w�̏o�͂��󂯎��2�w�ڂ̓��͔z��
	double I_sum = 0;//2�w�ڂ̕���̒l���i�[����ϐ�
	double Y[10] = { 0 };//3�w�ڂ̏o��
	double O[10] = { 0 };//�o�͔z��
	double O_2[10][10] = { 0 };//2�w�ڂ̏o�͂��i�[����z��
	double J[800] = { 0 };
	double J_sum = 0;//�]���֐�
	double sum_grad[10][10][10] = { 0 };//���z�����߂�Ƃ��̔z��A������N���X�A�R���|�[�l���g�AH�ɑΉ�

	do {
		J_sum = 0;
		for (i = 0;i < data;i++) {
			J[i] = 0;
		}
		for (l = 0;l < data;l++) {
			//������
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

			//1�w�ڂ̏o��
			for (i = 0;i < H;i++) {
				O[i] = input_con[l][i];
			}

			//2�w�ڂ̓���
			for (i = 0;i < c;i++) {
				for (j = 0;j < m;j++) {
					for (k = 0;k < H;k++) {
						I[i][j] += O[k] * w[i][j][k];
					}
				}
			}
			//�o��
			for (i = 0;i < c;i++) {
				for (j = 0;j < m;j++) {
					I_sum += exp(I[i][j]);//������܂����߂Ă���
				}
			}

			for (i = 0;i < c;i++) {
				for (j = 0;j < m;j++) {
					O_2[i][j] = exp(I[i][j]) / I_sum;
				}
			}
			//3�w��
			for (i = 0;i < c;i++) {
				for (j = 0;j < m;j++) {
					Y[i] += O_2[i][j];
				}
			}
		
			for (i = 0;i < c;i++) {
				J[l] += -input_T[l][i] * log(Y[i]);
			}

			//�d�݂̍X�V
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
	printf("�w�K�̏I��\n");
	printf("count = %d  J = %f\n", cnt, J_sum);

}

//���ʊ֐�
void identification(int c, int H, int m, int data, double** Input_con, double** Input_T, double ***w)
{
	int i, j, k, l;
	double cnt = 0;
	double I[800][4][2] = { 0 };//�O�̑w�̏o�͂��󂯎��2�w�ڂ̓��͔z��
	double I_sum[800] = { 0 };//2�w�ڂ̕���̒l���i�[����ϐ�
	double Y[800][6] = { 0 };//3�w�ڂ̏o��
	double O[800][6] = { 0 };//�o�͔z��
	double O_2[800][4][2] = { 0 };//2�w�ڂ̏o�͂��i�[����z��
	double J = 0;//�]���֐�

	for (l = 0;l < data;l++) {
		//1�w�ڂ̏o��
		for (i = 0;i < H;i++) {
			O[l][i] = Input_con[l][i];
		}

		//2�w�ڂ̓���
		for (i = 0;i < c;i++) {
			for (j = 0;j < m;j++) {
				for (k = 0;k < H;k++) {
					I[l][i][j] += O[l][k] * w[i][j][k];
				}
			}
		}
		//�o��
		for (i = 0;i < c;i++) {
			for (j = 0;j < m;j++) {
				I_sum[l] += exp(I[l][i][j]);//������܂����߂Ă���
			}
		}

		for (i = 0;i < c;i++) {
			for (j = 0;j < m;j++) {
				O_2[l][i][j] = exp(I[l][i][j]) / I_sum[l];
			}
		}
		//3�w��
		for (i = 0;i < c;i++) {
			for (j = 0;j < m;j++) {
				Y[l][i] += O_2[l][i][j];
			}
		}
	}
	
	printf("���ʃf�[�^�̏o��\n");
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
	printf("���ʗ� = %lf\n", cnt / data);
}
