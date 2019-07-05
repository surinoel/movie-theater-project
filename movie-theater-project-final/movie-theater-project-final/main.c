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
static void show_userinfo(User);								// 사용자 예매 정보를 보여주는 함수

typedef struct Moviedata	// 영화 정보
{
	char movieTitle[50];
	char movieDirector[50];
	int yearReleased; int startTime; int exitTime; int runningTime;
	int **seat; int emptyseatcnt;
	int ticketprice;
} Movie;

struct Moviedetaildata		// 영화 초기화할 때 편하게 하기 위해서
{
	char movieTitle[50];
	char movieDirector[50];
	int yearReleased;
	int runningTime;
	int ticketprice;
};

struct MovieTicket			// 사용자가 가지고 있는 티켓정보
{
	char movieTitle[50];
	int startTime; int exitTime; int runningTime;
	int ticketprice;
	int seatcnt; int *seatnum;
};

typedef struct user			// 사용자 정보
{
	char name[20];
	int password; int money;
	struct MovieTicket *mt;
} User;

Movie **movielist;						// 크게는 영화, 작게는 시간별로 구분을 했기 때문에 2차원 배열 선언
struct Moviedetaildata *movieinfo;		// 초기화 과정에서 사용하는 정보
User *user;								// 사용자 정보

int main(void) {
	srand((unsigned)time(NULL));				// 랜덤 시드 발생, 좌석 랜덤 배치로 인해 사용
	int adminpswd = 1234;						// 관리자 비밀번호
	init_movielist();							// 초기화
	init_userlist();

	do {
		system("cls");							// 화면 초기화
		rewind(stdin);							// 입력 버퍼를 비워주는 함수 
		int num = display_menu();				// 첫 화면을 보여주게 된다
		int choicenum;
		rewind(stdin);
		switch (num) {
		case 1:
			check_admin(adminpswd);					// 관리자 모드
			break;
		case 2:
			choicenum = display_movie_list();		// 영화 리스트를 보여주고 선택하는 함수
			switch (choicenum) {
			case 0:
				break;
			default:
				display_movietime_list(choicenum);	// 시간을 선택하는 함수
				break;
			}
			break;
		case 3:
			cancle_movie_ticket();					// 영화 티켓을 취소하는 함수
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
	for (int i = 0; i < _msize(movielist) / sizeof(*movielist); i++) {									// 메모리 해제
		for (int j = 0; j < _msize(movielist[i]) / sizeof(*movielist[i]); j++) {
			for (int k = 0; k < _msize(movielist[i][j].seat) / sizeof(*movielist[i][j].seat); k++) {
				free(movielist[i][j].seat[k]);
			}
			free(movielist[i][j].seat);
		}
		free(movielist[i]);
	}
	free(movielist);
	free(movieinfo);
	for (int i = 0; i < _msize(user) / sizeof(*user); i++) {
		free(user[i].mt);
	}
	free(user);
	return 0;
}

static void init_movielist(void)
{
	// 영화 정보를 초기화
	movielist = (struct Moviedata **)malloc(sizeof(struct Moviedata *) * 4);				// 영화 정보
	movieinfo = (struct Moviedetaildata *)malloc(sizeof(struct Moviedetaildata) * 4);		// 반복되는 정보를 위해 사용
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

			// 좌석 초기화, 10 x 10
			movielist[i][j].seat = (int **)malloc(10 * sizeof(int *));
			for (int k = 0; k < 10; k++) {
				*(movielist[i][j].seat + k) = (int *)malloc(10 * sizeof(int));
			}
			// 랜덤함수를 이용해서 여석 정보 초기화
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
	// 사용자 정보 초기화
	user = (User *)malloc(sizeof(User) * 4);
	memset(user, 0, sizeof(user));
	for (int i = 0; i < 4; i++) {
		user[i].mt = (struct MovieTicket *)malloc(sizeof(struct MovieTicket) * 3);
		memset(user[i].mt, 0, sizeof(user[i].mt) * 3);
	}
	strcpy(user[0].name, "유영재"); user[0].password = 1234; user[0].money = 20000; user[0].mt->seatcnt = 0;
	strcpy(user[1].name, "임정희"); user[1].password = 3456; user[1].money = 20000; user[1].mt->seatcnt = 0;
	strcpy(user[2].name, "양금영"); user[2].password = 5678; user[2].money = 20000; user[2].mt->seatcnt = 0;
	strcpy(user[3].name, "이경희"); user[3].password = 9999; user[3].money = 20000; user[3].mt->seatcnt = 0;
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
	if (pswd == adminpswd) {												// 비밀번호와 일치하다면
		choicenum = display_admin_menu();							
		system("cls");
		switch (choicenum) {
		case 1:
			printf("\n\n\n\n\n\n\n\n\n\n"); printf("\t\t\t    "); 
			printf("수정할 영화를 입력하세요 : "); scanf("%s", moviename);

			idx = -1;														// 영화 이름이 잘못됐다면 idx는 -1로 남는다
			ptr = movielist;
			for (int i = 0; i < _msize(ptr) / sizeof(*ptr); i++) {
				if (!strcmp(ptr[i]->movieTitle, moviename)) {		// 영화 이름과 맞다면
					idx = i;										// idx 초기화
				}
			}
			if (idx == -1) goto err;								// 맞지 않았을 때

			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n"); printf("\t\t\t    "); 
			printf("수정할 가격을 입력하세요 : "); scanf("%d", &price);
			if (price >= 5000 && price <= 15000) {					// 가격 범위 지정
				change_price(idx, price);
			}
			else {
				goto err;
			}
			break;
		case 2:
			dipslay_seat_admin();									// 관리자 권한에서 자리 현황 확인
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
		}	// 위 입력이 아니라면 계속 재입력 에러 메세지를 던져준다
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

static void display_movietime_list(int movienum)	// 영화 리스트 함수에서 반환한 값을 매개변수로 전달되는 함수
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
	if (!(selecttime - 1 >= 0 && selecttime - 1 < _msize(ptr) / sizeof(*ptr))) goto err; // 시간 정보를 잘못 입력했다면
	ret = display_seat(movienum - 1, selecttime - 1);									 // 해당 영화와 시간대에서만의 좌석 표시
	if (ret == -1) goto err;		// 잘못된 입력값을 넣었을 때
	else if (ret == 1) goto err2;	// 돈이 없을 때

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

	for (int j = 0; j < _msize(ptr.seat) / sizeof(*ptr.seat); j++) {					// 할당된 자리만큼
		for (int k = 0; k < _msize(ptr.seat[j]) / sizeof(*ptr.seat[j]); k++) {			// 번호 입력
			printf("\t%d", j * 10 + k + 1);
		}
		printf("\n");
		for (int k = 0; k < _msize(ptr.seat[j]) / sizeof(*ptr.seat[j]); k++) {		
			if (ptr.seat[j][k] == 1) printf("\to");										// 1이라면 o, -1이라면 x표시
			else printf("\tx");
		}
		printf("\n\n");
	}

	int hnum;
	printf("\t\t\t\tx가 예매할 수 있는 자리입니다\n\n");
	printf("        인원 수를 입력해주세요 (최대 4인)                                   : "); scanf("%d", &hnum);
	if (hnum > 4 || hnum + ptr.emptyseatcnt > 100) return -1;							// 100석이 초과되었거나 혹은 4명 이상을 입력했을 때

	printf("        원하시는 좌석 번호 공백을 두고 입력해주세요 (선택한 갯수 만큼 입력) : ");
	int *seatbuf = (int *)malloc(sizeof(int) * hnum);
	int ssize = _msize(seatbuf) / sizeof(*seatbuf);										// 인원수만큼 seatbuf 할당

	for (int i = 0; i < _msize(seatbuf) / sizeof(*seatbuf); i++) {
		scanf("%d", &seatbuf[i]);
		if (!(seatbuf[i] - 1> 0 && seatbuf[i] - 1 < 100)) {								// 자리 번호가 넘어갔을 때
			free(seatbuf);
			return -1;
		}
		int x, y;
		x = (seatbuf[i] - 1) / 10, y = (seatbuf[i] - 1) % 10;
		if (ptr.seat[x][y] == 1) {														// 예매된 자리라면
			free(seatbuf);
			return -1;
		}
		else {
			ptr.seat[x][y] = 1;
			ptr.emptyseatcnt -= 1;														// 
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
					display_bookingticket_info(ptr, user[i]);							// 예약 완료된 표를 보여주게 된다
				}
				else {
					free(seatbuf);
					return 1;
				}
			}
			else {
				free(seatbuf);
				return -1;
			}
		}
	}

	free(seatbuf);
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
					printf("\t%d", j * 10 + k + 1);
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