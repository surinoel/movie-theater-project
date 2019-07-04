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
				// 가격 수정하기
				break;
			case 2:
				dipslayseatadmin();
				break;
			case 3:
				// 신작추가
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
				// 영화 이름과 시간 선택
				// 입력 방법 틀렸을 때 오류 출력
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
	strcpy(movieinfo[0].movieTitle, "알라딘"); strcpy(movieinfo[0].movieDirector, "가이 리치"); movieinfo[0].yearReleased = 2019, movieinfo[0].runningTime = 130;
	strcpy(movieinfo[1].movieTitle, "스파이더맨"); strcpy(movieinfo[1].movieDirector, "존 왓츠"); movieinfo[1].yearReleased = 2019, movieinfo[1].runningTime = 130;
	strcpy(movieinfo[2].movieTitle, "토이스토리"); strcpy(movieinfo[2].movieDirector, "조시 쿨리"); movieinfo[2].yearReleased = 2019, movieinfo[2].runningTime = 130;
	strcpy(movieinfo[3].movieTitle, "기생충"); strcpy(movieinfo[3].movieDirector, "봉준호"); movieinfo[3].yearReleased = 2019, movieinfo[3].runningTime = 130;

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
	printf("\t\t\t    "); printf("            충남 인력개발원 영화관에 오신 것을 환영합니다   \n");
	printf("\t\t\t    "); printf(" ==================================================================\n");
	printf("\t\t\t    "); printf("||             1- 관리자모드:                                     ||\n");
	printf("\t\t\t    "); printf("||             2- 티켓 예매하기:                                  ||\n");
	printf("\t\t\t    "); printf("||             3- 티켓 취소하기:                                  ||\n");
	printf("\t\t\t    "); printf("||             4- 나가기:                                         ||\n");
	printf("\t\t\t    "); printf("||================================================================||\n");
	printf("\t\t\t    "); printf("	번호를 선택해주세요: ");
	scanf("%d", &num);
	return num;
}

int displayadminmenu(void)
{
	int num;
	while (1) {
		system("cls");
		printf("\n\n\n\n\n\n\n\n\n\n");
		printf("\t\t\t    "); printf("            충남 인력개발원 영화관에 오신 것을 환영합니다   \n");
		printf("\t\t\t    "); printf(" ==================================================================\n");
		printf("\t\t\t    "); printf("||             1- 가격 수정하기:                                  ||\n");
		printf("\t\t\t    "); printf("||             2- 자석 현황보기:                                  ||\n");
		printf("\t\t\t    "); printf("||             3- 신작 추가하기:                                  ||\n");
		printf("\t\t\t    "); printf("||================================================================||\n");
		printf("\t\t\t    "); printf("	뒤로 가려면 0번을 눌러주세요 \n");
		printf("\t\t\t    "); printf("	번호를 선택해주세요: ");
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
	printf("사용자 입력을 다시 확인해주세요\n");
	printf("\t\t\t\t\t#   ");
	printf("%lld초 안에 메뉴로 넘어갑니다\n", 5 - (clock() - curtimetick) / CLOCKS_PER_SEC);
}

void displaywriteerrorhandler(long long curtimetick)
{
	system("cls");
	printf("\n\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\t#   ");
	printf("사용자 입력을 다시 확인해주세요\n");
	printf("\t\t\t\t\t#   ");
	printf("%lld초 안에 재입력으로 넘어갑니다\n", 3 - (clock() - curtimetick) / CLOCKS_PER_SEC);
}


void displayexithandler(long long curtimetick)
{
	system("cls");
	printf("\n\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\t#   ");
	printf("안녕히 가세요\n");
	printf("\t\t\t\t\t#   ");
	printf("%lld초 안에 프로그램이 종료됩니다\n", 5 - (clock() - curtimetick) / CLOCKS_PER_SEC);
}

int displaymovielist(void)
{
	// 영화목록 추가
	int movienumber;
	while (1) {
		system("cls");
		printf("\n\n\n\n\n\n\n\n\n");
		printf("\t\t\t    "); printf("                     상영중인 영화리스트입니다                     \n");
		printf("\t\t\t    "); printf("                  원하시는 영화 번호를 선택해주세요                \n");
		printf("\t\t\t    "); printf("  =================================================================  \n");
		printf("\t\t\t    "); printf("||                   1- 알라딘                                     ||\n");
		printf("\t\t\t    "); printf("||                   2- 스파이더맨                                 ||\n");
		printf("\t\t\t    "); printf("||                   3- 토이스토리                                 ||\n");
		printf("\t\t\t    "); printf("||                   4- 기생충                                     ||\n");
		printf("\t\t\t    "); printf("||=================================================================||\n");
		printf("\t\t\t    "); printf("	뒤로 가려면 0번을 눌러주세요 \n");
		printf("\t\t\t    "); printf("	번호를 선택해주세요: ");
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
	printf("\t\t%s의 상영 시간표입니다\n\n", ptr[movienum - 1]->movieTitle);

	for (int i = 0; i < _msize(ptr[movienum - 1]) / sizeof(*ptr[movienum - 1]); i++) {
		printf("\t\t  [%d]\n", i + 1);
		printf("\t\t  시작시간 : %d:00 | 종료시간 : %d:00 | 러닝타임 : %d분\n", ptr[movienum - 1][i].startTime, ptr[movienum - 1][i].exitTime, ptr[movienum - 1][i].runningTime);
		printf("\t\t  남은여석 : %d\n\n", ptr[movienum - 1][i].emptyseatcnt);
	}

	int selecttime; 
	printf("\t\t  원하시는 시간에 대한 번호를 입력하세요 : "); 
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
	printf("\t\t  시작시간 : %d:00 | 종료시간 : %d:00 | 러닝타임 : %d분\n", ptr.startTime, ptr.exitTime, ptr.runningTime);
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
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t "); printf("확인할 영화를 검색해주세요: ");
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
			printf("\t\t  시작시간 : %d:00 | 종료시간 : %d:00 | 러닝타임 : %d분\n", ptr[idx][i].startTime, ptr[idx][i].exitTime, ptr[idx][i].runningTime);
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