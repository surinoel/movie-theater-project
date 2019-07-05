// Header file
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Private function
static void init_movielist(void);								// ��ȭ ����Ʈ �ʱ�ȭ �Լ�
static void init_userlist(void);								// ����� ����Ʈ �ʱ�ȭ �Լ� 
static void check_admin(int);									// ������ ���� Ȯ�� �Լ�
static void change_price(int, int);								// �����ڰ� ������ �ٲٴ� �Լ�
static int display_menu(void);									// �ʱ�ȭ���� �����ִ� �Լ�
static int display_admin_menu(void);							// �����ڸ��� �������� �� ȭ���� �����ִ� �Լ�
static void error_handler(long long, char [], char []);			// ������ �߻����� �� ó�����ִ� �Լ�
static int display_movie_list(void);							// ��ȭ����� ǥ�����ִ� �Լ�
static void display_movietime_list(int);						// �󿵽ð�ǥ�� �����ִ� �Լ�
static int display_seat(int, int);								// �󿵰��� �¼������� �����ִ� �Լ�
static void dipslay_seat_admin(void);							// ������ ��忡�� ��ȭ�̸��� �˻��ؼ� �����ִ� �Լ�
static void display_bookingticket_info(Movie, User);			// ��ȭ���Ÿ� �Ϸ����� �� ���� ������ �����ִ� �Լ�				
static void cancle_movie_ticket(void);							// ��ȭƼ���� ����ϴ� �Լ�
static void show_userinfo(User);								// ����� ���� ������ �����ִ� �Լ�

typedef struct Moviedata	// ��ȭ ����
{
	char movieTitle[50];
	char movieDirector[50];
	int yearReleased; int startTime; int exitTime; int runningTime;
	int **seat; int emptyseatcnt;
	int ticketprice;
} Movie;

struct Moviedetaildata		// ��ȭ �ʱ�ȭ�� �� ���ϰ� �ϱ� ���ؼ�
{
	char movieTitle[50];
	char movieDirector[50];
	int yearReleased;
	int runningTime;
	int ticketprice;
};

struct MovieTicket			// ����ڰ� ������ �ִ� Ƽ������
{
	char movieTitle[50];
	int startTime; int exitTime; int runningTime;
	int ticketprice;
	int seatcnt; int *seatnum;
};

typedef struct user			// ����� ����
{
	char name[20];
	int password; int money;
	struct MovieTicket *mt;
} User;

Movie **movielist;						// ũ�Դ� ��ȭ, �۰Դ� �ð����� ������ �߱� ������ 2���� �迭 ����
struct Moviedetaildata *movieinfo;		// �ʱ�ȭ �������� ����ϴ� ����
User *user;								// ����� ����

int main(void) {
	srand((unsigned)time(NULL));				// ���� �õ� �߻�, �¼� ���� ��ġ�� ���� ���
	int adminpswd = 1234;						// ������ ��й�ȣ
	init_movielist();							// �ʱ�ȭ
	init_userlist();

	do {
		system("cls");							// ȭ�� �ʱ�ȭ
		rewind(stdin);							// �Է� ���۸� ����ִ� �Լ� 
		int num = display_menu();				// ù ȭ���� �����ְ� �ȴ�
		int choicenum;
		rewind(stdin);
		switch (num) {
		case 1:
			check_admin(adminpswd);					// ������ ���
			break;
		case 2:
			choicenum = display_movie_list();		// ��ȭ ����Ʈ�� �����ְ� �����ϴ� �Լ�
			switch (choicenum) {
			case 0:
				break;
			default:
				display_movietime_list(choicenum);	// �ð��� �����ϴ� �Լ�
				break;
			}
			break;
		case 3:
			cancle_movie_ticket();					// ��ȭ Ƽ���� ����ϴ� �Լ�
			break;
		case 4:
			error_handler(5, "�ȳ��� ������", "���α׷��� ����˴ϴ�");
			goto end;
			break;
		default:
			error_handler(5, "����� �Է��� �ٽ� Ȯ�����ּ���", "���Է����� �Ѿ�ϴ�");
			break;
		}
	} while (1);

end:	
	for (int i = 0; i < _msize(movielist) / sizeof(*movielist); i++) {									// �޸� ����
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
	// ��ȭ ������ �ʱ�ȭ
	movielist = (struct Moviedata **)malloc(sizeof(struct Moviedata *) * 4);				// ��ȭ ����
	movieinfo = (struct Moviedetaildata *)malloc(sizeof(struct Moviedetaildata) * 4);		// �ݺ��Ǵ� ������ ���� ���
	strcpy(movieinfo[0].movieTitle, "�˶��"); strcpy(movieinfo[0].movieDirector, "���� ��ġ"); movieinfo[0].yearReleased = 2019, movieinfo[0].runningTime = 130; movieinfo[0].ticketprice = 8000;
	strcpy(movieinfo[1].movieTitle, "�����̴���"); strcpy(movieinfo[1].movieDirector, "�� ����"); movieinfo[1].yearReleased = 2019, movieinfo[1].runningTime = 130; movieinfo[1].ticketprice = 9000;
	strcpy(movieinfo[2].movieTitle, "���̽��丮"); strcpy(movieinfo[2].movieDirector, "���� ��"); movieinfo[2].yearReleased = 2019, movieinfo[2].runningTime = 130; movieinfo[2].ticketprice = 10000;
	strcpy(movieinfo[3].movieTitle, "�����"); strcpy(movieinfo[3].movieDirector, "����ȣ"); movieinfo[3].yearReleased = 2019, movieinfo[3].runningTime = 130; movieinfo[3].ticketprice = 11000;

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

			// �¼� �ʱ�ȭ, 10 x 10
			movielist[i][j].seat = (int **)malloc(10 * sizeof(int *));
			for (int k = 0; k < 10; k++) {
				*(movielist[i][j].seat + k) = (int *)malloc(10 * sizeof(int));
			}
			// �����Լ��� �̿��ؼ� ���� ���� �ʱ�ȭ
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
	// ����� ���� �ʱ�ȭ
	user = (User *)malloc(sizeof(User) * 4);
	memset(user, 0, sizeof(user));
	for (int i = 0; i < 4; i++) {
		user[i].mt = (struct MovieTicket *)malloc(sizeof(struct MovieTicket) * 3);
		memset(user[i].mt, 0, sizeof(user[i].mt) * 3);
	}
	strcpy(user[0].name, "������"); user[0].password = 1234; user[0].money = 20000; user[0].mt->seatcnt = 0;
	strcpy(user[1].name, "������"); user[1].password = 3456; user[1].money = 20000; user[1].mt->seatcnt = 0;
	strcpy(user[2].name, "��ݿ�"); user[2].password = 5678; user[2].money = 20000; user[2].mt->seatcnt = 0;
	strcpy(user[3].name, "�̰���"); user[3].password = 9999; user[3].money = 20000; user[3].mt->seatcnt = 0;
}

static void check_admin(int adminpswd)
{
	Movie **ptr;
	int pswd, choicenum, price, idx;
	long long curtimetick, flagtick;
	char moviename[20];
	system("cls");
	printf("\n\n\n\n\n\n\n\n\n\n");printf("\t\t\t    "); 
	printf("������ ��й�ȣ�� �Է��ϼ��� : "); scanf("%d", &pswd);
	if (pswd == adminpswd) {												// ��й�ȣ�� ��ġ�ϴٸ�
		choicenum = display_admin_menu();							
		system("cls");
		switch (choicenum) {
		case 1:
			printf("\n\n\n\n\n\n\n\n\n\n"); printf("\t\t\t    "); 
			printf("������ ��ȭ�� �Է��ϼ��� : "); scanf("%s", moviename);

			idx = -1;														// ��ȭ �̸��� �߸��ƴٸ� idx�� -1�� ���´�
			ptr = movielist;
			for (int i = 0; i < _msize(ptr) / sizeof(*ptr); i++) {
				if (!strcmp(ptr[i]->movieTitle, moviename)) {		// ��ȭ �̸��� �´ٸ�
					idx = i;										// idx �ʱ�ȭ
				}
			}
			if (idx == -1) goto err;								// ���� �ʾ��� ��

			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n"); printf("\t\t\t    "); 
			printf("������ ������ �Է��ϼ��� : "); scanf("%d", &price);
			if (price >= 5000 && price <= 15000) {					// ���� ���� ����
				change_price(idx, price);
			}
			else {
				goto err;
			}
			break;
		case 2:
			dipslay_seat_admin();									// ������ ���ѿ��� �ڸ� ��Ȳ Ȯ��
			break;
		default:
			break;
		}
	}
	else {
	err:
		error_handler(5, "����� �Է��� �ٽ� Ȯ�����ּ���", "�޴��� �Ѿ�ϴ�");
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
static int display_admin_menu(void)
{
	int num;
	while (1) {
		system("cls");
		printf("\n\n\n\n\n\n\n\n\n\n");
		printf("\t\t\t    "); printf("            �泲 �η°��߿� ��ȭ���� ���� ���� ȯ���մϴ�   \n");
		printf("\t\t\t    "); printf(" ==================================================================\n");
		printf("\t\t\t    "); printf("||             1- ���� �����ϱ�:                                  ||\n");
		printf("\t\t\t    "); printf("||             2- �¼� ��Ȳ����:                                  ||\n");
		printf("\t\t\t    "); printf("||================================================================||\n");
		printf("\t\t\t    "); printf("	�ڷ� ������ 0���� �����ּ��� \n");
		printf("\t\t\t    "); printf("	��ȣ�� �������ּ���: ");
		scanf("%d", &num);
		if (num == 1 || num == 2 || num == 0) {			
			return num;
		}	// �� �Է��� �ƴ϶�� ��� ���Է� ���� �޼����� �����ش�
		error_handler(5, "����� �Է��� �ٽ� Ȯ�����ּ���", "���Է����� �Ѿ�ϴ�");
	}
}

static int display_movie_list(void)
{
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
		error_handler(5, "����� �Է��� �ٽ� Ȯ�����ּ���", "���Է����� �Ѿ�ϴ�");
	}
}

static void display_movietime_list(int movienum)	// ��ȭ ����Ʈ �Լ����� ��ȯ�� ���� �Ű������� ���޵Ǵ� �Լ�
{
	int ret;
	Movie **ptr = movielist;

	system("cls");
	printf("\n\n\n\n\n\n");
	printf("\t\t  %s�� �� �ð�ǥ�Դϴ�\n\n", ptr[movienum - 1]->movieTitle);

	for (int i = 0; i < _msize(ptr[movienum - 1]) / sizeof(*ptr[movienum - 1]); i++) {
		printf("\t\t  [%d]\n", i + 1);
		printf("\t\t  ���۽ð� : %d:00 | ����ð� : %d:00 | ����Ÿ�� : %d��\n", ptr[movienum - 1][i].startTime, ptr[movienum - 1][i].exitTime, ptr[movienum - 1][i].runningTime);
		printf("\t\t  �������� : %d\n\n", ptr[movienum - 1][i].emptyseatcnt);
	}

	int selecttime;
	printf("\t\t  ���Ͻô� �ð��� ���� ��ȣ�� �Է��ϼ��� : ");
	scanf("%d", &selecttime);
	if (!(selecttime - 1 >= 0 && selecttime - 1 < _msize(ptr) / sizeof(*ptr))) goto err; // �ð� ������ �߸� �Է��ߴٸ�
	ret = display_seat(movienum - 1, selecttime - 1);									 // �ش� ��ȭ�� �ð��뿡������ �¼� ǥ��
	if (ret == -1) goto err;		// �߸��� �Է°��� �־��� ��
	else if (ret == 1) goto err2;	// ���� ���� ��

	system("pause");
	return;

err:
	error_handler(5, "����� �Է��� �ٽ� Ȯ�����ּ���", "�޴��� �Ѿ�ϴ�");
	return;
err2:
	error_handler(5, "���� �����ϴ� �Ф�", "���α׷��� ����˴ϴ�");
	return;
}

static int display_seat(int movienum, int stime)
{
	Movie ptr = movielist[movienum][stime];
	system("cls");
	printf("\n\n");
	printf("\t\t  ���۽ð� : %d:00 | ����ð� : %d:00 | ����Ÿ�� : %d��\n", ptr.startTime, ptr.exitTime, ptr.runningTime);
	printf("\t    =================================================================  \n");
	printf("\t\t\t\t\t  SCREEN\t\t\t\t\n");
	printf("\t    =================================================================  \n\n");

	for (int j = 0; j < _msize(ptr.seat) / sizeof(*ptr.seat); j++) {					// �Ҵ�� �ڸ���ŭ
		for (int k = 0; k < _msize(ptr.seat[j]) / sizeof(*ptr.seat[j]); k++) {			// ��ȣ �Է�
			printf("\t%d", j * 10 + k + 1);
		}
		printf("\n");
		for (int k = 0; k < _msize(ptr.seat[j]) / sizeof(*ptr.seat[j]); k++) {		
			if (ptr.seat[j][k] == 1) printf("\to");										// 1�̶�� o, -1�̶�� xǥ��
			else printf("\tx");
		}
		printf("\n\n");
	}

	int hnum;
	printf("\t\t\t\tx�� ������ �� �ִ� �ڸ��Դϴ�\n\n");
	printf("        �ο� ���� �Է����ּ��� (�ִ� 4��)                                   : "); scanf("%d", &hnum);
	if (hnum > 4 || hnum + ptr.emptyseatcnt > 100) return -1;							// 100���� �ʰ��Ǿ��ų� Ȥ�� 4�� �̻��� �Է����� ��

	printf("        ���Ͻô� �¼� ��ȣ ������ �ΰ� �Է����ּ��� (������ ���� ��ŭ �Է�) : ");
	int *seatbuf = (int *)malloc(sizeof(int) * hnum);
	int ssize = _msize(seatbuf) / sizeof(*seatbuf);										// �ο�����ŭ seatbuf �Ҵ�

	for (int i = 0; i < _msize(seatbuf) / sizeof(*seatbuf); i++) {
		scanf("%d", &seatbuf[i]);
		if (!(seatbuf[i] - 1> 0 && seatbuf[i] - 1 < 100)) {								// �ڸ� ��ȣ�� �Ѿ�� ��
			free(seatbuf);
			return -1;
		}
		int x, y;
		x = (seatbuf[i] - 1) / 10, y = (seatbuf[i] - 1) % 10;
		if (ptr.seat[x][y] == 1) {														// ���ŵ� �ڸ����
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
	printf("        ȸ�� �̸��� �Է��ϼ���                                              : "); scanf("%s", name);
	for (int i = 0; i < _msize(user) / sizeof(*user); i++) {
		if (!strcmp(user[i].name, name)) {
			idx = i;
			int pswd;
			printf("        %s��! ��й�ȣ�� �Է��ϼ���                                     : ", user[i].name); scanf("%d", &pswd);
			if (user[i].password == pswd) {
				if (user[i].money > ssize * ptr.ticketprice) {
					strcpy(user[i].mt->movieTitle, ptr.movieTitle);
					user[i].mt->seatcnt = ssize;
					user[i].mt->runningTime = ptr.runningTime; user[i].mt->startTime = ptr.startTime; user[i].mt->exitTime = ptr.exitTime;
					user[i].mt->ticketprice = ptr.ticketprice;
					user[i].mt->seatnum = (int *)malloc(sizeof(int) * ssize);
					memcpy(user[i].mt->seatnum, seatbuf, sizeof(seatbuf) * ssize);
					display_bookingticket_info(ptr, user[i]);							// ���� �Ϸ�� ǥ�� �����ְ� �ȴ�
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
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t "); printf("Ȯ���� ��ȭ�� �˻����ּ���: ");
		scanf("%s", moviebuf);
		idx = -1;

		for (int i = 0; i < _msize(ptr) / sizeof(*ptr); i++) {
			if (!strcmp(ptr[i]->movieTitle, moviebuf)) {
				idx = i;
			}
		}

		if (idx == -1) {
			error_handler(3, "����� �Է��� �ٽ� Ȯ�����ּ���", "���Է����� �Ѿ�ϴ�");
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
			printf("\t                         ���Ű� ���������� �Ϸ�ƽ��ϴ� !                    \n\n\n");
			printf("\t------------------------------��ȭ ���� Ƽ�� ����----------------------------\n");
			printf("\t=============================================================================\n");
			printf("\t ��ȭ      : %s \t\t\t      ���� : %s\n", ptr.movieTitle, ptr.movieDirector);
			printf("\t ȸ������  : %s\n", us.name);
			printf("\t                                              ��¥             : 29-07-2019\n");
			printf("\t                                              �󿵽ð�         : %d:00 ~ %d:00\n", ptr.startTime, ptr.exitTime);
			printf("\t                                              �ѻ󿵽ð�       : %d��\n", ptr.runningTime);
			for (int i = 0; i < us.mt->seatcnt; i++) {
				printf("\t                                              �¼����� No.     : %d  \n", us.mt->seatnum[i]);
			}
			printf("\t                                              ����             : %d  \n\n", us.mt->ticketprice * us.mt->seatcnt);

			printf("\t=============================================================================\n\n");
			printf("\t                         %lld�� �ȿ� ���θ޴��� ���ư��ϴ� !                 \n", 5 - (clock() - curtimetick) / CLOCKS_PER_SEC);
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
	printf("\t\t\t\t\t#   "); printf("%lld�� �ȿ� %s\n", sec - (clock() - curtimetick) / CLOCKS_PER_SEC, msg2);
	while ((clock() - curtimetick) / CLOCKS_PER_SEC < sec) {
		if (flagtick != (clock() - curtimetick) / CLOCKS_PER_SEC) {
			flagtick = (clock() - curtimetick) / CLOCKS_PER_SEC;
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n"); printf("\t\t\t\t\t#   "); printf("%s\n", msg1);
			printf("\t\t\t\t\t#   "); printf("%lld�� �ȿ� %s\n", sec - (clock() - curtimetick) / CLOCKS_PER_SEC, msg2);
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
			printf("\t------------------------------��ȭ ���� Ƽ�� ����----------------------------\n");
			printf("\t=============================================================================\n");
			printf("\t ��ȭ      : %s \t\t\t", us.mt->movieTitle);
			printf("\t ȸ������  : %s\n", us.name);
			printf("\t                                              ��¥             : 29-07-2019\n");
			printf("\t                                              �󿵽ð�         : %d:00 ~ %d:00\n", us.mt->startTime, us.mt->exitTime);
			printf("\t                                              �ѻ󿵽ð�       : %d��\n", us.mt->runningTime);
			for (int i = 0; i < us.mt->seatcnt; i++) {
				printf("\t                                              �¼����� No.     : %d  \n", us.mt->seatnum[i]);
			}
			printf("\t                                              ����             : %d  \n\n", us.mt->ticketprice * us.mt->seatcnt);

			printf("\t=============================================================================\n\n");
			printf("\t                         %lld�� �ȿ� ���θ޴��� ���ư��ϴ� !                 \n", 5 - (clock() - curtimetick) / CLOCKS_PER_SEC);
		}
	}
}

static void cancle_movie_ticket(void)
{
	system("cls");
	char buf[20], moviebuf[20]; memset(buf, 0, sizeof(buf)); memset(moviebuf, 0, sizeof(buf));
	bool ok = false;
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t "); printf("ȸ�� �̸��� �Է����ּ���: "); scanf("%s", buf);
	for (int i = 0; i < _msize(user) / sizeof(*user); i++) {
		ok = true;
		if (!strcmp(buf, user[i].name)) {
			int pswd;
			printf("\t\t  %s��! ��й�ȣ�� �Է��ϼ���: ", user[i].name); scanf("%d", &pswd);
			if (pswd == user[i].password) {
				if (user[i].mt->seatcnt != 0) show_userinfo(user[i]);
				else goto err;
			}
			else {
				goto err2;
			}
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t "); printf("����� ��ȭ�̸��� �Է����ּ���: "); scanf("%s", moviebuf);
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
	error_handler(5, "���ų����� �����ϴ�", "���α׷��� ����˴ϴ�");
	return;

err2:
	error_handler(5, "����� �Է��� �ٽ� Ȯ�����ּ���", "�޴��� �Ѿ�ϴ�");
	return;
}