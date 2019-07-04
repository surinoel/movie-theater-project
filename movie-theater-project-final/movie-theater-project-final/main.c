// Header file
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Private function
static void init_movielist(void);								// 영화 리스트 초기화 함수
static void init_userlist(void);								// 사용자 리스트 초기화 함수 
static void check_admin(int);									// 관리자 접속 확인 함수
static void change_price(int, int);								// 관리자가 가격을 바꾸는 함수
static int display_menu(void);									// 초기화면을 보여주는 함수
static int display_admin_menu(void);							// 관리자모드로 접속했을 때 화면을 보여주는 함수
static void error_handler(long long, char [], char []);			// 에러가 발생했을 때 처리해주는 함수
static int display_movie_list(void);							// 영화목록을 표시해주는 함수
static void display_movietime_list(int);						// 상영시간표를 보여주는 함수
static int display_seat(int, int);								// 상영관의 좌석정보를 보여주는 함수
static void dipslay_seat_admin(void);							// 관리자 모드에서 영화이름을 검색해서 보여주는 함수
static void display_bookingticket_info(Movie, User);			// 영화예매를 완료했을 때 예매 정보를 보여주는 함수				
static void cancle_movie_ticket(void);							// 영화티켓을 취소하는 함수
static void show_userinfo(User);

typedef struct Moviedata
{
	char movieTitle[50];
	char movieDirector[50];
	int yearReleased; int startTime; int exitTime; int runningTime;
	int **seat; int emptyseatcnt;
	int ticketprice;
} Movie;

struct Moviedetaildata
{
	char movieTitle[50];
	char movieDirector[50];
	int yearReleased;
	int runningTime;
	int ticketprice;
};

struct MovieTicket
{
	char movieTitle[50];
	int startTime; int exitTime; int runningTime;
	int ticketprice;
	int seatcnt; int *seatnum;
};

typedef struct user
{
	char name[20];
	int password; int money;
	struct MovieTicket *mt;
} User;

Movie **movielist;
struct Moviedetaildata *movieinfo;
User *user;

int main(void) {
	srand((unsigned)time(NULL));
	int adminpswd = 1234;
	init_movielist();
	init_userlist();

	do {
		system("cls");
		int num = display_menu();
		int choicenum;
		rewind(stdin);
		switch (num) {
		case 1:
			check_admin(adminpswd);
			break;
		case 2:
			choicenum = display_movie_list();
			switch (choicenum) {
			case 0:
				break;
			default:
				display_movietime_list(choicenum);
				break;
			}
			break;
		case 3:
			cancle_movie_ticket();
			break;
		case 4:
			error_handler(5, "안녕히 가세요", "프로그램이 종료됩니다");
			goto end;
			break;
		default:
			error_handler(5, "사용자 입력을 다시 확인해주세요", "재입력으로 넘어갑니다");
			break;
		}
	} while (1);
end:
	return 0;
}

static void init_movielist(void)
{
	// 영화 정보를 초기화
	movielist = (struct Moviedata **)malloc(sizeof(struct Moviedata *) * 4);
	movieinfo = (struct Moviedetaildata *)malloc(sizeof(struct Moviedetaildata) * 4);
	strcpy(movieinfo[0].movieTitle, "알라딘"); strcpy(movieinfo[0].movieDirector, "가이 리치"); movieinfo[0].yearReleased = 2019, movieinfo[0].runningTime = 130; movieinfo[0].ticketprice = 8000;
	strcpy(movieinfo[1].movieTitle, "스파이더맨"); strcpy(movieinfo[1].movieDirector, "존 왓츠"); movieinfo[1].yearReleased = 2019, movieinfo[1].runningTime = 130; movieinfo[1].ticketprice = 9000;
	strcpy(movieinfo[2].movieTitle, "토이스토리"); strcpy(movieinfo[2].movieDirector, "조시 쿨리"); movieinfo[2].yearReleased = 2019, movieinfo[2].runningTime = 130; movieinfo[2].ticketprice = 10000;
	strcpy(movieinfo[3].movieTitle, "기생충"); strcpy(movieinfo[3].movieDirector, "봉준호"); movieinfo[3].yearReleased = 2019, movieinfo[3].runningTime = 130; movieinfo[3].ticketprice = 11000;

	for (int i = 0; i < 4; i++) {
		movielist[i] = (struct Moviedata *)malloc(sizeof(struct Moviedata) * 4);
		for (int j = 0; j < 4; j++) {
			strcpy(movielist[i][j].movieTitle, movieinfo[i].movieTitle);
			strcpy(movielist[i][j].movieDirector, movieinfo[i].movieDirector);
			movielist[i][j].yearReleased = movieinfo[i].yearReleased;
			movielist[i][j].runningTime = movieinfo[i].runningTime;
			movielist[i][j].ticketprice = movieinfo[i].ticketprice;

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
		}
	}
}

static void init_userlist(void)
{
	user = (User *)malloc(sizeof(User) * 3);
	memset(user, 0, sizeof(user));
	for (int i = 0; i < 3; i++) {
		user[i].mt = (struct MovieTicket *)malloc(sizeof(struct MovieTicket));
		memset(user[i].mt, 0, sizeof(user[i].mt));
	}
	strcpy(user[0].name, "유영재"); user[0].password = 1234; user[0].money = 20000; user[0].mt->seatcnt = 0;
	strcpy(user[1].name, "김재혁"); user[1].password = 3456; user[1].money = 20000; user[0].mt->seatcnt = 0;
	strcpy(user[2].name, "김륜영"); user[2].password = 5678; user[2].money = 20000; user[0].mt->seatcnt = 0;
}

static void check_admin(int adminpswd)
{
	Movie **ptr;
	int pswd, choicenum, price, idx;
	long long curtimetick, flagtick;
	char moviename[20];
	system("cls");
	printf("\n\n\n\n\n\n\n\n\n\n");printf("\t\t\t    "); 
	printf("관리자 비밀번호를 입력하세요 : "); scanf("%d", &pswd);
	if (pswd == adminpswd) {
		choicenum = display_admin_menu();
		system("cls");
		switch (choicenum) {
		case 1:
			printf("\n\n\n\n\n\n\n\n\n\n"); printf("\t\t\t    "); 
			printf("수정할 영화를 입력하세요 : "); scanf("%s", moviename);

			idx = -1;
			ptr = movielist;
			for (int i = 0; i < _msize(ptr) / sizeof(*ptr); i++) {
				if (!strcmp(ptr[i]->movieTitle, moviename)) {
					idx = i;
				}
			}
			if (idx == -1) goto err;

			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n"); printf("\t\t\t    "); 
			printf("수정할 가격을 입력하세요 : "); scanf("%d", &price);
			if (price >= 5000 && price <= 15000) {
				change_price(idx, price);
			}
			else {
				goto err;
			}
			break;
		case 2:
			dipslay_seat_admin();
			break;
		default:
			break;
		}
	}
	else {
	err:
		error_handler(5, "사용자 입력을 다시 확인해주세요", "메뉴로 넘어갑니다");
	}
}

static void change_price(int idx, int price)
{
	Movie *ptr = movielist[idx];
	for (int i = 0; i < _msize(ptr) / sizeof(*ptr); i++) {
		ptr[i].ticketprice = price;
	}
}

static int display_menu(void)
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
static int display_admin_menu(void)
{
	int num;
	while (1) {
		system("cls");
		printf("\n\n\n\n\n\n\n\n\n\n");
		printf("\t\t\t    "); printf("            충남 인력개발원 영화관에 오신 것을 환영합니다   \n");
		printf("\t\t\t    "); printf(" ==================================================================\n");
		printf("\t\t\t    "); printf("||             1- 가격 수정하기:                                  ||\n");
		printf("\t\t\t    "); printf("||             2- 좌석 현황보기:                                  ||\n");
		printf("\t\t\t    "); printf("||================================================================||\n");
		printf("\t\t\t    "); printf("	뒤로 가려면 0번을 눌러주세요 \n");
		printf("\t\t\t    "); printf("	번호를 선택해주세요: ");
		scanf("%d", &num);
		if (num == 1 || num == 2 || num == 0) {
			return num;
		}
		error_handler(5, "사용자 입력을 다시 확인해주세요", "재입력으로 넘어갑니다");
	}
}

static int display_movie_list(void)
{
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
		error_handler(5, "사용자 입력을 다시 확인해주세요", "재입력으로 넘어갑니다");
	}
}

static void display_movietime_list(int movienum)
{
	int ret;
	Movie **ptr = movielist;

	system("cls");
	printf("\n\n\n\n\n\n");
	printf("\t\t  %s의 상영 시간표입니다\n\n", ptr[movienum - 1]->movieTitle);

	for (int i = 0; i < _msize(ptr[movienum - 1]) / sizeof(*ptr[movienum - 1]); i++) {
		printf("\t\t  [%d]\n", i + 1);
		printf("\t\t  시작시간 : %d:00 | 종료시간 : %d:00 | 러닝타임 : %d분\n", ptr[movienum - 1][i].startTime, ptr[movienum - 1][i].exitTime, ptr[movienum - 1][i].runningTime);
		printf("\t\t  남은여석 : %d\n\n", ptr[movienum - 1][i].emptyseatcnt);
	}

	int selecttime;
	printf("\t\t  원하시는 시간에 대한 번호를 입력하세요 : ");
	scanf("%d", &selecttime);
	if (!(selecttime - 1 >= 0 && selecttime - 1 < _msize(ptr) / sizeof(*ptr))) goto err;
	ret = display_seat(movienum - 1, selecttime - 1);
	if (ret == -1) goto err;
	else if (ret == 1) goto err2;

	system("pause");
	return;

err:
	error_handler(5, "사용자 입력을 다시 확인해주세요", "메뉴로 넘어갑니다");
	return;
err2:
	error_handler(5, "돈이 없습니다 ㅠㅠ", "프로그램이 종료됩니다");
	return;
}

static int display_seat(int movienum, int stime)
{
	Movie ptr = movielist[movienum][stime];
	system("cls");
	printf("\n\n");
	printf("\t\t  시작시간 : %d:00 | 종료시간 : %d:00 | 러닝타임 : %d분\n", ptr.startTime, ptr.exitTime, ptr.runningTime);
	printf("\t    =================================================================  \n");
	printf("\t\t\t\t\t  SCREEN\t\t\t\t\n");
	printf("\t    =================================================================  \n\n");

	for (int j = 0; j < _msize(ptr.seat) / sizeof(*ptr.seat); j++) {
		for (int k = 0; k < _msize(ptr.seat[j]) / sizeof(*ptr.seat[j]); k++) {
			printf("\t%d", j * 10 + k + 1);
		}
		printf("\n");
		for (int k = 0; k < _msize(ptr.seat[j]) / sizeof(*ptr.seat[j]); k++) {
			if (ptr.seat[j][k] == 1) printf("\to");
			else printf("\tx");
		}
		printf("\n\n");
	}

	int hnum;
	printf("\t\t\t\tx가 예매할 수 있는 자리입니다\n\n");
	printf("        인원 수를 입력해주세요 (최대 4인)                                   : "); scanf("%d", &hnum);
	if (hnum > 4 || hnum + ptr.emptyseatcnt > 100) return -1;

	printf("        원하시는 좌석 번호 공백을 두고 입력해주세요 (선택한 갯수 만큼 입력) : ");
	int *seatbuf = (int *)malloc(sizeof(int) * hnum);
	int ssize = _msize(seatbuf) / sizeof(*seatbuf);

	for (int i = 0; i < _msize(seatbuf) / sizeof(*seatbuf); i++) {
		scanf("%d", &seatbuf[i]);
		if (!(seatbuf[i] - 1> 0 && seatbuf[i] - 1 < 100)) {
			return -1;
		}
		int x, y;
		x = (seatbuf[i] - 1) / 10, y = (seatbuf[i] - 1) % 10;
		if (ptr.seat[x][y] == 1) {
			return -1;
		}
		else {
			ptr.seat[x][y] = 1;
		}
	}

	int idx = -1;
	char name[20];
	printf("        회원 이름을 입력하세요                                              : "); scanf("%s", name);
	for (int i = 0; i < _msize(user) / sizeof(*user); i++) {
		if (!strcmp(user[i].name, name)) {
			idx = i;
			int pswd;
			printf("        %s님! 비밀번호를 입력하세요                                     : ", user[i].name); scanf("%d", &pswd);
			if (user[i].password == pswd) {
				if (user[i].money > ssize * ptr.ticketprice) {
					strcpy(user[i].mt->movieTitle, ptr.movieTitle);
					user[i].mt->seatcnt = ssize;
					user[i].mt->runningTime = ptr.runningTime; user[i].mt->startTime = ptr.startTime; user[i].mt->exitTime = ptr.exitTime;
					user[i].mt->ticketprice = ptr.ticketprice;
					user[i].mt->seatnum = (int *)malloc(sizeof(int) * ssize);
					memcpy(user[i].mt->seatnum, seatbuf, sizeof(seatbuf) * ssize);
					display_bookingticket_info(ptr, user[i]);
				}
				else {
					return 1;
				}
			}
			else {
				return -1;
			}
		}
	}

	if (idx == -1) return -1;
	return 0;
}

void dipslay_seat_admin(void)
{
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
			error_handler(3, "사용자 입력을 다시 확인해주세요", "재입력으로 넘어갑니다");
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

void display_bookingticket_info(Movie ptr, User us)
{
	system("cls");

	long long curtimetick, flagtick;
	curtimetick = clock();
	flagtick = curtimetick / CLOCKS_PER_SEC;
	while ((clock() - curtimetick) / CLOCKS_PER_SEC < 5) {
		if (flagtick != (clock() - curtimetick) / CLOCKS_PER_SEC) {
			flagtick = (clock() - curtimetick) / CLOCKS_PER_SEC;
			system("cls");
			printf("\n\n\n\n");
			printf("\t                         예매가 성공적으로 완료됐습니다 !                    \n\n\n");
			printf("\t------------------------------영화 예매 티켓 정보----------------------------\n");
			printf("\t=============================================================================\n");
			printf("\t 영화      : %s \t\t\t      감독 : %s\n", ptr.movieTitle, ptr.movieDirector);
			printf("\t 회원정보  : %s\n", us.name);
			printf("\t                                              날짜             : 29-07-2019\n");
			printf("\t                                              상영시간         : %d:00 ~ %d:00\n", ptr.startTime, ptr.exitTime);
			printf("\t                                              총상영시간       : %d분\n", ptr.runningTime);
			for (int i = 0; i < us.mt->seatcnt; i++) {
				printf("\t                                              좌석정보 No.     : %d  \n", us.mt->seatnum[i]);
			}
			printf("\t                                              가격             : %d  \n\n", us.mt->ticketprice * us.mt->seatcnt);

			printf("\t=============================================================================\n\n");
			printf("\t                         %lld초 안에 메인메뉴로 돌아갑니다 !                 \n", 5 - (clock() - curtimetick) / CLOCKS_PER_SEC);
		}
	}

	return;
}

static void error_handler(long long sec, char msg1[], char msg2[])
{
	long long curtimetick, flagtick;
	curtimetick = clock();
	flagtick = curtimetick / CLOCKS_PER_SEC;
	system("cls");
	printf("\n\n\n\n\n\n\n\n\n\n"); printf("\t\t\t\t\t#   ");  printf("%s\n", msg1);
	printf("\t\t\t\t\t#   "); printf("%lld초 안에 %s\n", sec - (clock() - curtimetick) / CLOCKS_PER_SEC, msg2);
	while ((clock() - curtimetick) / CLOCKS_PER_SEC < sec) {
		if (flagtick != (clock() - curtimetick) / CLOCKS_PER_SEC) {
			flagtick = (clock() - curtimetick) / CLOCKS_PER_SEC;
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n"); printf("\t\t\t\t\t#   "); printf("%s\n", msg1);
			printf("\t\t\t\t\t#   "); printf("%lld초 안에 %s\n", sec - (clock() - curtimetick) / CLOCKS_PER_SEC, msg2);
		}
	}
	system("cls");
	return;
}

static void show_userinfo(User us)
{
	system("cls");

	long long curtimetick, flagtick;
	curtimetick = clock();
	flagtick = curtimetick / CLOCKS_PER_SEC;
	while ((clock() - curtimetick) / CLOCKS_PER_SEC < 5) {
		if (flagtick != (clock() - curtimetick) / CLOCKS_PER_SEC) {
			flagtick = (clock() - curtimetick) / CLOCKS_PER_SEC;
			system("cls");
			printf("\n\n\n\n");
			printf("\t------------------------------영화 예매 티켓 정보----------------------------\n");
			printf("\t=============================================================================\n");
			printf("\t 영화      : %s \t\t\t", us.mt->movieTitle);
			printf("\t 회원정보  : %s\n", us.name);
			printf("\t                                              날짜             : 29-07-2019\n");
			printf("\t                                              상영시간         : %d:00 ~ %d:00\n", us.mt->startTime, us.mt->exitTime);
			printf("\t                                              총상영시간       : %d분\n", us.mt->runningTime);
			for (int i = 0; i < us.mt->seatcnt; i++) {
				printf("\t                                              좌석정보 No.     : %d  \n", us.mt->seatnum[i]);
			}
			printf("\t                                              가격             : %d  \n\n", us.mt->ticketprice * us.mt->seatcnt);

			printf("\t=============================================================================\n\n");
			printf("\t                         %lld초 안에 메인메뉴로 돌아갑니다 !                 \n", 5 - (clock() - curtimetick) / CLOCKS_PER_SEC);
		}
	}
}

static void cancle_movie_ticket(void)
{
	long long curtimetick, flagtick;
	system("cls");
	char buf[20], moviebuf[20]; memset(buf, 0, sizeof(buf)); memset(moviebuf, 0, sizeof(buf));
	bool ok = false;
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t "); printf("회원 이름을 입력해주세요: "); scanf("%s", buf);
	for (int i = 0; i < _msize(user) / sizeof(*user); i++) {
		ok = true;
		if (!strcmp(buf, user[i].name)) {
			int pswd;
			printf("\t\t  %s님! 비밀번호를 입력하세요: ", user[i].name); scanf("%d", &pswd);
			if (pswd == user[i].password) {
				if (user[i].mt->seatcnt != 0) show_userinfo(user[i]);
				else goto err;
			}
			else {
				goto err2;
			}
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t "); printf("취소할 영화이름을 입력해주세요: "); scanf("%s", moviebuf);
			if (!strcmp(moviebuf, user[i].mt->movieTitle)) {
				user[i].money += user[i].mt->ticketprice * user[i].mt->seatcnt;
				free(user[i].mt);
				user[i].mt = (struct MovieTicket *)malloc(sizeof(struct MovieTicket));
				memset(user[i].mt, 0, sizeof(user[i].mt));
				user[i].mt->seatcnt = 0;
			}
			else {
				goto err2;
			}
		}
	}
	if (!ok) goto err2;
	return;

err:
	error_handler(5, "예매내역이 없습니다", "프로그램이 종료됩니다");
	return;

err2:
	error_handler(5, "사용자 입력을 다시 확인해주세요", "메뉴로 넘어갑니다");
	return;
}