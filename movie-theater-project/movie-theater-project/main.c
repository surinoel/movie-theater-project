#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initmovielist(void);
int displaymenu(void);
int displayadminmenu(void);
void displayerrorhandler(long long);
void displaywriteerrorhandler(long long);
void displayexithandler(long long);
int displaymovielist(void);
void displaymovietimelist(int);
void addmovielist(void);
void displayseat(int, int);
void dipslayseatadmin(void);

typedef struct Moviedata
{
	char movieTitle[50];
	char movieDirector[50];
	int yearReleased;
	int startTime;
	int exitTime;
	int runningTime;
	int **seat;
	int emptyseatcnt;
	struct Moviedata *next;
} Movie;

Movie **movielist;

struct Moviedetaildata
{
	char movieTitle[50];
	char movieDirector[50];
	int yearReleased;
	int runningTime;
};

struct Moviedetaildata *movieinfo;

int main(void) {
	srand((unsigned)time(NULL));
	long long curtimetick, flagtick;
	initmovielist();

	do {
		system("cls");
		int num = displaymenu();
		int choicenum;
		switch (num) {
		case 1:
			choicenum = displayadminmenu();
			system("cls");
			switch (choicenum) {
			case 1:
				// ���� �����ϱ�
				break;
			case 2:
				dipslayseatadmin();
				break;
			case 3:
				// �����߰�
				break;
			default:
				break;
			}
			break;
		case 2:
			choicenum = displaymovielist();
			switch (choicenum) {
			case 0:
				break;
			default:
				displaymovietimelist(choicenum);
				// ��ȭ �̸��� �ð� ����
				// �Է� ��� Ʋ���� �� ���� ���
				break;
			}
			break;
		case 3:
			system("cls");
			system("pause");
			break;
		case 4:
			curtimetick = clock();
			flagtick = curtimetick / CLOCKS_PER_SEC;
			displayexithandler(curtimetick);
			while ((clock() - curtimetick) / CLOCKS_PER_SEC < 5) {
				if (flagtick != (clock() - curtimetick) / CLOCKS_PER_SEC) {
					flagtick = (clock() - curtimetick) / CLOCKS_PER_SEC;
					displayexithandler(curtimetick);
				}
			}
			system("cls");
			return 0;
			break;
		default:
			curtimetick = clock();
			flagtick = curtimetick / CLOCKS_PER_SEC;
			displaywriteerrorhandler(curtimetick);
			while ((clock() - curtimetick) / CLOCKS_PER_SEC < 3) {
				if (flagtick != (clock() - curtimetick) / CLOCKS_PER_SEC) {
					flagtick = (clock() - curtimetick) / CLOCKS_PER_SEC;
					displaywriteerrorhandler(curtimetick);
				}
			}
			break;
		}
	} while (1);
	return 0;
}

void initmovielist(void)
{
	movielist = (struct Moviedata **)malloc(sizeof(struct Moviedata *) * 4);
	movieinfo = (struct Moviedetaildata *)malloc(sizeof(struct Moviedetaildata) * 4);
	strcpy(movieinfo[0].movieTitle, "�˶��"); strcpy(movieinfo[0].movieDirector, "���� ��ġ"); movieinfo[0].yearReleased = 2019, movieinfo[0].runningTime = 130;
	strcpy(movieinfo[1].movieTitle, "�����̴���"); strcpy(movieinfo[1].movieDirector, "�� ����"); movieinfo[1].yearReleased = 2019, movieinfo[1].runningTime = 130;
	strcpy(movieinfo[2].movieTitle, "���̽��丮"); strcpy(movieinfo[2].movieDirector, "���� ��"); movieinfo[2].yearReleased = 2019, movieinfo[2].runningTime = 130;
	strcpy(movieinfo[3].movieTitle, "�����"); strcpy(movieinfo[3].movieDirector, "����ȣ"); movieinfo[3].yearReleased = 2019, movieinfo[3].runningTime = 130;

	for (int i = 0; i < 4; i++) {
		movielist[i] = (struct Moviedata *)malloc(sizeof(struct Moviedata) * 4);
		for (int j = 0; j < 4; j++) {
			strcpy(movielist[i][j].movieTitle, movieinfo[i].movieTitle);
			strcpy(movielist[i][j].movieDirector, movieinfo[i].movieDirector);
			movielist[i][j].yearReleased = movieinfo[i].yearReleased;
			movielist[i][j].runningTime = movieinfo[i].runningTime;
			if (j == 0) { movielist[i][j].startTime = 12, movielist[i][j].exitTime = 14; }
			else if (j == 1) { movielist[i][j].startTime = 15, movielist[i][j].exitTime = 17; }
			else if (j == 2) { movielist[i][j].startTime = 18, movielist[i][j].exitTime = 20; }
			else if (j == 3) { movielist[i][j].startTime = 21, movielist[i][j].exitTime = 23; }

			movielist[i][j].seat = (int **)malloc(10 * sizeof(int *));
			for (int k = 0; k < 10; k++) {
				*(movielist[i][j].seat + k) = (int *)malloc(10 * sizeof(int));
			}
			movielist[i][j].emptyseatcnt = 0;
			for (int u = 0; u < 10; u++) {
				for (int v = 0; v < 10; v++) {
					if (rand() % 2) movielist[i][j].seat[u][v] = 1;
					else {
						movielist[i][j].seat[u][v] = -1;
						movielist[i][j].emptyseatcnt += 1;
					}
				}
			}
			if (j < 3) {
				movielist[i][j].next = &movielist[i][j + 1];
			}
		}
	}
}

int displaymenu(void)
{
	int num;
	printf("\n\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t    "); printf("            �泲 �η°��߿� ��ȭ���� ���� ���� ȯ���մϴ�   \n");
	printf("\t\t\t    "); printf(" ==================================================================\n");
	printf("\t\t\t    "); printf("||             1- �����ڸ��:                                     ||\n");
	printf("\t\t\t    "); printf("||             2- Ƽ�� �����ϱ�:                                  ||\n");
	printf("\t\t\t    "); printf("||             3- Ƽ�� ����ϱ�:                                  ||\n");
	printf("\t\t\t    "); printf("||             4- ������:                                         ||\n");
	printf("\t\t\t    "); printf("||================================================================||\n");
	printf("\t\t\t    "); printf("	��ȣ�� �������ּ���: ");
	scanf("%d", &num);
	return num;
}

int displayadminmenu(void)
{
	int num;
	while (1) {
		system("cls");
		printf("\n\n\n\n\n\n\n\n\n\n");
		printf("\t\t\t    "); printf("            �泲 �η°��߿� ��ȭ���� ���� ���� ȯ���մϴ�   \n");
		printf("\t\t\t    "); printf(" ==================================================================\n");
		printf("\t\t\t    "); printf("||             1- ���� �����ϱ�:                                  ||\n");
		printf("\t\t\t    "); printf("||             2- �ڼ� ��Ȳ����:                                  ||\n");
		printf("\t\t\t    "); printf("||             3- ���� �߰��ϱ�:                                  ||\n");
		printf("\t\t\t    "); printf("||================================================================||\n");
		printf("\t\t\t    "); printf("	�ڷ� ������ 0���� �����ּ��� \n");
		printf("\t\t\t    "); printf("	��ȣ�� �������ּ���: ");
		scanf("%d", &num);
		if (num == 1 || num == 2 || num == 3 || num == 0) {
			return num;
		}
		long long curtimetick, flagtick;
		curtimetick = clock();
		flagtick = curtimetick / CLOCKS_PER_SEC;
		displaywriteerrorhandler(curtimetick);
		while ((clock() - curtimetick) / CLOCKS_PER_SEC < 3) {
			if (flagtick != (clock() - curtimetick) / CLOCKS_PER_SEC) {
				flagtick = (clock() - curtimetick) / CLOCKS_PER_SEC;
				displaywriteerrorhandler(curtimetick);
			}
		}
	}
}

void displayerrorhandler(long long curtimetick)
{
	system("cls");
	printf("\n\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\t#   ");
	printf("����� �Է��� �ٽ� Ȯ�����ּ���\n");
	printf("\t\t\t\t\t#   ");
	printf("%lld�� �ȿ� �޴��� �Ѿ�ϴ�\n", 5 - (clock() - curtimetick) / CLOCKS_PER_SEC);
}

void displaywriteerrorhandler(long long curtimetick)
{
	system("cls");
	printf("\n\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\t#   ");
	printf("����� �Է��� �ٽ� Ȯ�����ּ���\n");
	printf("\t\t\t\t\t#   ");
	printf("%lld�� �ȿ� ���Է����� �Ѿ�ϴ�\n", 3 - (clock() - curtimetick) / CLOCKS_PER_SEC);
}


void displayexithandler(long long curtimetick)
{
	system("cls");
	printf("\n\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\t#   ");
	printf("�ȳ��� ������\n");
	printf("\t\t\t\t\t#   ");
	printf("%lld�� �ȿ� ���α׷��� ����˴ϴ�\n", 5 - (clock() - curtimetick) / CLOCKS_PER_SEC);
}

int displaymovielist(void)
{
	// ��ȭ��� �߰�
	int movienumber;
	while (1) {
		system("cls");
		printf("\n\n\n\n\n\n\n\n\n");
		printf("\t\t\t    "); printf("                     ������ ��ȭ����Ʈ�Դϴ�                     \n");
		printf("\t\t\t    "); printf("                  ���Ͻô� ��ȭ ��ȣ�� �������ּ���                \n");
		printf("\t\t\t    "); printf("  =================================================================  \n");
		printf("\t\t\t    "); printf("||                   1- �˶��                                     ||\n");
		printf("\t\t\t    "); printf("||                   2- �����̴���                                 ||\n");
		printf("\t\t\t    "); printf("||                   3- ���̽��丮                                 ||\n");
		printf("\t\t\t    "); printf("||                   4- �����                                     ||\n");
		printf("\t\t\t    "); printf("||=================================================================||\n");
		printf("\t\t\t    "); printf("	�ڷ� ������ 0���� �����ּ��� \n");
		printf("\t\t\t    "); printf("	��ȣ�� �������ּ���: ");
		scanf("%d", &movienumber);
		if (movienumber == 1 || movienumber == 2 || movienumber == 3 || movienumber == 4 || movienumber == 0) {
			return movienumber;
		}
		long long curtimetick, flagtick;
		curtimetick = clock();
		flagtick = curtimetick / CLOCKS_PER_SEC;
		displaywriteerrorhandler(curtimetick);
		while ((clock() - curtimetick) / CLOCKS_PER_SEC < 3) {
			if (flagtick != (clock() - curtimetick) / CLOCKS_PER_SEC) {
				flagtick = (clock() - curtimetick) / CLOCKS_PER_SEC;
				displaywriteerrorhandler(curtimetick);
			}
		}
	}
}

void displaymovietimelist(int movienum) 
{
	Movie **ptr = movielist;
	system("cls");
	printf("\n\n\n\n\n\n");
	printf("\t\t%s�� �� �ð�ǥ�Դϴ�\n\n", ptr[movienum - 1]->movieTitle);

	for (int i = 0; i < _msize(ptr[movienum - 1]) / sizeof(*ptr[movienum - 1]); i++) {
		printf("\t\t  [%d]\n", i + 1);
		printf("\t\t  ���۽ð� : %d:00 | ����ð� : %d:00 | ����Ÿ�� : %d��\n", ptr[movienum - 1][i].startTime, ptr[movienum - 1][i].exitTime, ptr[movienum - 1][i].runningTime);
		printf("\t\t  �������� : %d\n\n", ptr[movienum - 1][i].emptyseatcnt);
	}

	int selecttime; 
	printf("\t\t  ���Ͻô� �ð��� ���� ��ȣ�� �Է��ϼ��� : "); 
	scanf("%d", &selecttime);
	displayseat(movienum - 1, selecttime - 1);
	system("pause");
}

void addmovielist(void)
{

}


void displayseat(int movienum, int stime)
{
	Movie ptr = movielist[movienum][stime];
	system("cls");
	printf("\n\n");
	printf("\t\t  ���۽ð� : %d:00 | ����ð� : %d:00 | ����Ÿ�� : %d��\n", ptr.startTime, ptr.exitTime, ptr.runningTime);
	printf("\t    =================================================================  \n");
	printf("\t\t\t\t\t  SCREEN\t\t\t\t\n");
	printf("\t    =================================================================  \n");

	for (int j = 0; j < _msize(ptr.seat) / sizeof(*ptr.seat); j++) {
		for (int k = 0; k < _msize(ptr.seat[j]) / sizeof(*ptr.seat[j]); k++) {
			printf("\t%d", j * 10 + k);
		}
		printf("\n");
		for (int k = 0; k < _msize(ptr.seat[j]) / sizeof(*ptr.seat[j]); k++) {
			if (ptr.seat[j][k] == 1) printf("\to");
			else printf("\tx");
		}
		printf("\n\n");
	}
	printf("\n\n");

	system("pause");
}

void dipslayseatadmin(void) {
	Movie **ptr = movielist;
	char moviebuf[50];
	int idx;
	while (1) {
		system("cls");
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t "); printf("Ȯ���� ��ȭ�� �˻����ּ���: ");
		scanf("%s", moviebuf);
		idx = -1;
		
		for (int i = 0; i < _msize(ptr) / sizeof(*ptr); i++) {
			if (!strcmp(ptr[i]->movieTitle, moviebuf)) {
				idx = i;
			}
		}
		
		if (idx == -1) {
			long long curtimetick, flagtick; 
			curtimetick = clock();
			flagtick = curtimetick / CLOCKS_PER_SEC;
			displaywriteerrorhandler(curtimetick);
			while ((clock() - curtimetick) / CLOCKS_PER_SEC < 3) {
				if (flagtick != (clock() - curtimetick) / CLOCKS_PER_SEC) {
					flagtick = (clock() - curtimetick) / CLOCKS_PER_SEC;
					displaywriteerrorhandler(curtimetick);
				}
			}
			continue;
		}

		system("cls");
		printf("\n");
		for (int i = 0; i < _msize(ptr[idx]) / sizeof(*ptr[idx]); i++) {
			printf("\t\t  ���۽ð� : %d:00 | ����ð� : %d:00 | ����Ÿ�� : %d��\n", ptr[idx][i].startTime, ptr[idx][i].exitTime, ptr[idx][i].runningTime);
			printf("\t    =================================================================  \n");
			printf("\t\t\t\t\t  SCREEN\t\t\t\t\n");
			printf("\t    =================================================================  \n");

			for (int j = 0; j < _msize(ptr[idx][i].seat) / sizeof(*ptr[idx][i].seat); j++) {
				for (int k = 0; k < _msize(ptr[idx][i].seat[j]) / sizeof(*ptr[idx][i].seat[j]); k++) {
					printf("\t%d", j * 10 + k);
				}
				printf("\n");
				for (int k = 0; k < _msize(ptr[idx][i].seat[j]) / sizeof(*ptr[idx][i].seat[j]); k++) {
					if (ptr[idx][i].seat[j][k] == 1) printf("\to");
					else printf("\tx");
				}
				printf("\n\n");
			}
			printf("\n\n");
		}
		break;
	}

	system("pause");
}