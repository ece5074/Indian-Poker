#include <stdio.h>
#include <conio.h>
#include<string.h>

#pragma comment(lib,"ws2_32.lib") 
#include<WinSock2.h>
#include<Windows.h>

#define ABS(x) x >=0 ? x : x*-1
#define CARD 10
#define DECK 2
#define PLAYER1_X 20
#define PLAYER2_X 80
#define CARDSET_Y 7
#define PORT 12345

//HOW TO GAME
//1. 카드는 1~10까지 적힌 카드로 각각 2세트가 주어진다
//2. 양 플레이어는 카드를 받기전 우선 1개의 칩을 우선 베팅해야한다.
//3. 양 플레이어는 자신의 카드를 볼 수 없으며, 상대의 카드만으로 베팅을 해야한다.
//4. 선공이 우선 베팅을 했을때 죽기(0개베팅), 맞베팅, 그 이상으로 베팅이 가능하다.
//5. 죽은경우 카드의 결과와 상관없이 베팅한 칩의 모두가 상대에게 전달된다.
//6. 같은 수의 베팅을 한 경우 서로 카드를 확인하여 더 높은 숫자를 가지고 있는 플레이어가 베팅한 칩을 가져간다.
//7. All-in의 경우 상대가 가지고 있는 최대개수보다 더 많은 수의 칩을 베팅할 수 없다.
//8. 카드의 결과 확인 후 칩이 0개라면 해당 플레이어는 패배한다.
//9. 만일, 첫 베팅 이후 칩이 0개라서 더이상 베팅할 수 없는 경우라면 카드는 즉시공개된다.

//COMPUTER
//1. 카드에 따라 1~10까지의 우선순위를 지닌다.
//2. 우선순위를 근거한 확률에 의해 칩을 1개 내지 2개씩 베팅한다.

typedef enum COLOR
{
	BLACK,
	BLUE1,
	GREEN1,
	CYAN1,
	RED1,
	MAGENTA1,
	YELLOW1,
	GRAY1,
	GRAY2,
	BLUE2,
	GREEN2,
	CYAN2,
	RED2,
	MAGENTA2,
	YELLOW2,
	WHITE
}COLOR;

char cardIMG[11][13][20] = {
	{
		"┌────────┐",
		"│　　　　　　　　│",
		"│　　■■■■　　│",
		"│　■　　　　■　│",
		"│　　　　　　■　│",
		"│　　　　　　■　│",
		"│　　　　■■　　│",
		"│　　　■　　　　│",
		"│　　　■　　　　│",
		"│　　　　　　　　│",
		"│　　　■　　　　│",
		"│　　　　　　　　│",
		"└────────┘"
	},
	{
		"┌────────┐",
		"│　　　　　　　　│",
		"│　　　　　　 1　│",
		"│　　　　　　　　│",
		"│　　　　　　　　│",
		"│　　　　　　　　│",
		"│　　　　◈　　　│",
		"│　　　　　　　　│",
		"│　　　　　　　　│",
		"│　　　　　　　　│",
		"│　1 　　　　　　│",
		"│　　　　　　　　│",
		"└────────┘"
	},
	{
		"┌────────┐",
		"│　　　　　　　　│",
		"│　　　　　　 2　│",
		"│　　　　　　　　│",
		"│　　　　◈　　　│",
		"│　　　　　　　　│",
		"│　　　　　　　　│",
		"│　　　　　　　　│",
		"│　　　　◈　　　│",
		"│　　　　　　　　│",
		"│　2 　　　　　　│",
		"│　　　　　　　　│",
		"└────────┘"
	},
	{
		"┌────────┐",
		"│　　　　　　　　│",
		"│　　　　　　 3　│",
		"│　　　　　　　　│",
		"│　　　　◈　　　│",
		"│　　　　　　　　│",
		"│　　　　◈　　　│",
		"│　　　　　　　　│",
		"│　　　　◈　　　│",
		"│　　　　　　　　│",
		"│　3 　　　　　　│",
		"│　　　　　　　　│",
		"└────────┘"
	},
	{
		"┌────────┐",
		"│　　　　　　　　│",
		"│　　　　　　 4　│",
		"│　　　　　　　　│",
		"│　◈　　　　◈　│",
		"│　　　　　　　　│",
		"│　　　　　　　　│",
		"│　　　　　　　　│",
		"│　◈　　　　◈　│",
		"│　　　　　　　　│",
		"│　4 　　　　　　│",
		"│　　　　　　　　│",
		"└────────┘"
	},
	{
		"┌────────┐",
		"│　　　　　　　　│",
		"│　　　　　　 5　│",
		"│　◈　　　　◈　│",
		"│　　　　　　　　│",
		"│　　　　　　　　│",
		"│　　　 ◈ 　　　│",
		"│　　　　　　　　│",
		"│　　　　　　　　│",
		"│　◈　　　　◈　│",
		"│　5 　　　　　　│",
		"│　　　　　　　　│",
		"└────────┘"
	},
	{
		"┌────────┐",
		"│　　　　　　　　│",
		"│　　　　　　 6　│",
		"│　◈　　　　◈　│",
		"│　　　　　　　　│",
		"│　　　　　　　　│",
		"│　◈ 　　　 ◈　│",
		"│　　　　　　　　│",
		"│　　　　　　　　│",
		"│　◈　　　　◈　│",
		"│　6 　　　　　　│",
		"│　　　　　　　　│",
		"└────────┘"
	},
	{
		"┌────────┐",
		"│　　　　　　　　│",
		"│　　　　　　 7　│",
		"│　◈　　　　◈　│",
		"│　　　　　　　　│",
		"│　　　　　　　　│",
		"│  ◈　 ◈ 　◈　│",
		"│　  　　  　　　│",
		"│　　　　　　　　│",
		"│　◈　　    ◈　│",
		"│　7 　　    　　│",
		"│　　　　　    　│",
		"└────────┘"
	},
	{
		"┌────────┐",
		"│　　　　　　　　│",
		"│　　　　　　 8　│",
		"│　◈　　　　◈　│",
		"│　　　　　　　　│",
		"│　◈　　　　◈　│",
		"│　　　　　 　　 │",
		"│　◈　　　　◈　│",
		"│　　　 　　　　 │",
		"│　◈　　　　◈　│",
		"│　8 　　　 　　 │",
		"│　　　　　　 　 │",
		"└────────┘"
	},
	{
		"┌────────┐",
		"│　　　　　　　　│",
		"│　　　　 　　9　│",
		"│　◈　　　　◈　│",
		"│　◈　　　　◈　│",
		"│　　　　　　　　│",
		"│　　 　◈　 　　│",
		"│　　　　　　　　│",
		"│　◈　　　　◈　│",
		"│　◈　　　　◈　│",
		"│　9 　　　　　　│",
		"│　　　　　　　　│",
		"└────────┘"
	},
	{
		"┌────────┐",
		"│　　　　　　　　│",
		"│　　　　　　10　│",
		"│　　　　　　　　│",
		"│　◈　　　　◈　│",
		"│　◈　　　　◈　│",
		"│　◈　　　　◈　│",
		"│　◈　　　　◈　│",
		"│　◈　　　　◈　│",
		"│　　　　　　　　│",
		"│　10　　　　　　│",
		"│　　　　　　　　│",
		"└────────┘"
	}

};

int use_card[2][10] = { -1 }; // -1이면 사용된 카드, 1이면 사용안된 카드

typedef struct inGamePlayer
{
	char name[10];
	int card, chip;
	int betting_chip; //베팅한 칩
	int status; //포기, 베팅 여부
	int outcome; //승패여부
	int turn; //현재 자신의 턴인지 아닌지 여부
	int res;
}inGamePlayer;

void gotoxy(int x, int y);
void textcolor(int fg_color, int bg_color);
void cls(int bg_color, int text_color);
void removeCursor(void);
void suffle();
void print_card(int cardNum, int x, int y, int Delay);
void chip_interface(int x, int y);
void bet_interface(int x, int y);
void gameset1(int x, int y);
void gameset2(int delay);
void main_page(int x, int y);
void select_Game();
void vsCPU();
void print_gameset();
void Player_Betting(inGamePlayer* p1, inGamePlayer* p2, int turn);
int card_select();
void Chip_setting(inGamePlayer* p, int chip);
void betting_menu(inGamePlayer* p1, inGamePlayer* p2, int turn);
int card_select();
void Tutorial();
void TypingMessage(int x, int y, char* message);
void TypingMessage2(int x, int y, char* message, char* name);
void card_open(inGamePlayer * p1, inGamePlayer* p2, int turn);
void chip_move(inGamePlayer* p1, inGamePlayer* p2);
void print_Betting(inGamePlayer* p1, int turn);
void print_chip(inGamePlayer* p1, int turn);
void CPU_Betting(inGamePlayer* p1, inGamePlayer* p2, int turn);
void print_name(inGamePlayer p1, inGamePlayer p2);
void print_turn(int turn);
void vsNETWORK();
void use_Server();
void use_Client();

void main()
{
	srand(time(NULL));
	removeCursor();
	main_page(20, 0);
	gotoxy(0, 0);
	while(1)
		select_Game();
}

//커서 좌표를 변경
void gotoxy(int x, int y) //내가 원하는 위치로 커서 이동
{
	COORD pos; // Windows.h 에 정의
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//글씨 색상을 변경
void textcolor(int fg_color, int bg_color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}

//화면초기화
void cls(int bg_color, int text_color) // 화면 지우기
{
	char cmd[100];
	int cols = 120, lines = 30;
	sprintf(cmd, "COLOR %x%x", bg_color, text_color);
	system(cmd);
	sprintf(cmd, "mode con: cols=%d lines=%d", cols, lines);
	system(cmd);
}

void removeCursor(void)
{ // 커서를 안보이게 한다
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

//카드배열 초기화
void suffle()
{
	int i, j, chk_card = 0;
	for (i = 0; i < DECK; i++)
	{
		for (j = 0; j < CARD; j++)
		{
			if (use_card[i][j] == 1)
				chk_card++;
		}
	}
	if(chk_card == 20)
		for (i = 0; i < DECK; i++)
			for (j = 0; j < CARD; j++)
				use_card[i][j] = 1;
}

//카드출력
void print_card(int cardNum, int x, int y, int Delay)
{
	int i, j;
	textcolor(BLACK, WHITE);
	for (i = 0; i < 13; i++)
	{
		gotoxy(x, y + i);
		for (j = 0; j < 20; j++)
		{
			printf("%c", cardIMG[cardNum][i][j]);
		}
		Sleep(Delay);
	}
}

void print_turn(int turn)
{
	int i, j;
	gotoxy(50, 3);
	textcolor(BLACK, GRAY2);
	if (turn == 0)
	{
		printf("■■■■■■■■■■▶");
	}
	else
	{
		printf("◀■■■■■■■■■■");
	}
}

//chip 인터페이스
void chip_interface(int x, int y)
{
	char pcChip[13][14] = {
		"┌──chip─┐",
		"│　　　　　│",
		"│　　　　　│",
		"│　　　　　│",
		"│　　　　　│",
		"│　　　　　│",
		"│　　　　　│",
		"│　　　　　│",
		"│　　　　　│",
		"│　　　　　│",
		"│　　　　　│",
		"│　　　　　│",
		"└─────┘",
	};
	int i, j;
	textcolor(BLACK, WHITE);
	for (i = 0; i < 13; i++)
	{
		gotoxy(x, y + i);
		for (j = 0; j < 14; j++)
		{
			printf("%c", pcChip[i][j]);
		}
	}
}

//베팅 인터페이스
void bet_interface(int x, int y)
{
	char betting[16][30] = {
		"            BETTING           ",
		"　　　　　　　　　　　　　　　",
		"　　　　　　　　　　　　　　　",
		"　　　　　　　　　　　　　　　",
		"　　　　　　　　　　　　　　　",
		"　　　　　　　　　　　　　　　",
		"　　　　　　　　　　　　　　　",
		"　　　　　　　　　　　　　　　",
		"　　　　　　　　　　　　　　　",
		"　　　　　　　　　　　　　　　",
		"　　　　　　　　　　　　　　　",
		"　　　　　　　　　　　　　　　",
		"　　　　　　　　　　　　　　　",
		"　　　　　　　　　　　　　　　",
		"　　　　　　　　　　　　　　　",
		"　　　　　　　　　　　　　　　",
	};
	int i, j;
	textcolor(BLACK, WHITE);
	for (i = 0; i < 16; i++)
	{
		gotoxy(x, y + i);
		for (j = 0; j < 30; j++)
		{
			printf("%c", betting[i][j]);
		}
	}
}

//게임판을 세팅
void gameset1(int x, int y)
{
	int i, j, k;
	cls(GREEN1, BLACK);
	textcolor(GRAY2, GRAY2);
	for (i = 0; i < 20; i++)
	{
		gotoxy(x, y + i);
		for (j = 0; j < 112; j++)
		{
			printf(" ");
		}
	}
}

//게임판 하단부를 세팅
void gameset2(int delay)
{
	int i, j;
	int x = 4, y = 2;
	textcolor(BLACK, BLACK);
	for (i = 25; i >= 20; i--)
	{
		for (j = 111; j >= 0; j--)
		{
			gotoxy(x + j, y + i);
			printf(" ");
			Sleep(delay);
		}
	}
}

//프로그램 시작 시 가장 먼저 보여줄 화면
void main_page(int x, int y)
{
	int i = 0;
	textcolor(GREEN2, BLACK);
	gotoxy(x, y + 5);
	printf("■■■　■　　　　■　■■■　　■■■　　　■　　　■　　　　■\n");
	gotoxy(x, y + 6);
	printf("　■　　■■　　　■　■　　■　　■　　　■　■　　■■　　　■\n");
	gotoxy(x, y + 7);
	printf("　■　　■　■　　■　■　　■　　■　　　■　■　　■　■　　■\n");
	gotoxy(x, y + 8);
	printf("　■　　■　　■　■　■　　■　　■　　■■■■■　■　　■　■\n");
	gotoxy(x, y + 9);
	printf("　■　　■　　　■■　■　　■　　■　　■　　　■　■　　　■■\n");
	gotoxy(x, y + 10);
	printf("■■■　■　　　　■　■■■　　■■■　■　　　■　■　　　　■\n");
	gotoxy(x, y + 12); textcolor(MAGENTA2, BLACK);
	printf("　　　■■■　　　■■■　　■　　■　■■■■　■■■\n");
	gotoxy(x, y + 13);
	printf("　　　■　　■　■　　　■　■　■　　■　　　　■　　■\n");
	gotoxy(x, y + 14);
	printf("　　　■　　■　■　　　■　■■　　　■　　　　■　　■\n");
	gotoxy(x, y + 15);
	printf("　　　■■■　　■　　　■　■■　　　■■■■　■■■\n");
	gotoxy(x, y + 16);
	printf("　　　■　　　　■　　　■　■　■　　■　　　　■　　■\n");
	gotoxy(x, y + 17);
	printf("　　　■　　　　　■■■　　■　　■　■■■■　■　　■");

	while (1)
	{
		textcolor(i % 16 + 1, BLACK);
		gotoxy(40, y + 20);
		printf("Please press the any key");
		Sleep(100);
		i++;
		if (_kbhit())
		{
			getch();
			break;
		}
	}
}

//게임선택
void select_Game()
{
	while (1)
	{
		cls(BLACK, WHITE);
		textcolor(GREEN1, BLACK);
		gotoxy(20, 3);
		printf("　■　　　　■　　　■  ■■■　　■■■　■■■　■　　■");
		gotoxy(20, 4);
		printf("■■　　　　■　　　■　■　　　　■　　　■　■　■　　■");
		gotoxy(20, 5);
		printf("　■　　　　■　　　■　■■■　　■　　　■■■　■　　■");
		gotoxy(20, 6);
		printf("　■　　　　　■　■　　　　■　　■　　　■　　　■　　■");
		gotoxy(20, 7);
		printf("■■■　■　　　■　　　■■■　　■■■　■　　　■■■■");

		textcolor(BLUE1, BLACK);
		gotoxy(20, 10);
		printf("■■■　　　■　　　■  ■■■　　■■■　■　　　　　■　　　■　　　■　■■■■　■■■");
		gotoxy(20, 11);
		printf("　　■　　　■　　　■　■　　　　■　■　■　　　　■　■　　■　　　■　■　　　　■　　■");
		gotoxy(20, 12);
		printf("■■■　　　■　　　■　■■■　　■■■　■　　　　■■■　　　■　■　　■■■■　■■■");
		gotoxy(20, 13);
		printf("■　　　　　　■　■　　　　■　　■　　　■　　　■　　　■　　　■　　　■　　　　■　　■");
		gotoxy(20, 14);
		printf("■■■　■　　　■　　　■■■　　■　　　■■■　■　　　■　　　■　　　■■■■　■　　■");

		textcolor(RED1, BLACK);
		gotoxy(20, 17);
		printf("■■■　　　■■■■　■　　　■　■■■　■■■■■");
		gotoxy(20, 18);
		printf("　　■　　　■　　　　　■　■　　　■　　　　■");
		gotoxy(20, 19);
		printf("■■■　　　■■■■　　　■　　　　■　　　　■");
		gotoxy(20, 20);
		printf("　　■　　　■　　　　　■　■　　　■　　　　■");
		gotoxy(20, 21);
		printf("■■■　■　■■■■　■　　　■　■■■　　　■");

		switch (getch())
		{
		case '1':
			Sleep(500);
			vsCPU();
			break;
		case '2':
			vsNETWORK();
			break;
		case '3':
			TypingMessage(40, 25, "프로그램을 종료합니다.");
			gotoxy(40, 26);
			exit(1);
		case '9':
			break;
		}
	}
}

//턴에 따른 베팅메뉴 생성
void betting_menu(inGamePlayer* p1, inGamePlayer* p2, int turn)
{
	p1->card = card_select();
	p2->card = card_select();
	textcolor(BLACK, WHITE);
	gotoxy(1, 1);
	printf("%d %d", p1->card, p2->card);
	print_card(0, PLAYER1_X, CARDSET_Y, 0);
	print_card(p2->card, PLAYER2_X, CARDSET_Y, 0);
	gameset2(3);
	while (1)
	{
		print_turn(turn);
		if (turn == 1)
		{
			TypingMessage(30, 23, "플레이어의 턴입니다. 베팅을 해 주세요.");
			Player_Betting(p1, p2, turn);
			gameset2(2);
		}
		else
		{
			TypingMessage(30, 23, "컴퓨터의 턴, 베팅하는 중입니다.");
			CPU_Betting(p2, p1, turn);
			gameset2(2);
		}
		turn = (turn + 1) % 2;
		if (p1->outcome == 1)
		{
			turn = 1;
			break;
		}
		else if (p2->outcome == 1)
		{
			turn = 0;
			break;
		}
	}
}

int dice()
{
	int p1, p2;

	TypingMessage(43, 23, "선 플레이어를 결정합니다.");
	TypingMessage(43, 24, "선 플레이어는 주사위로 결정하겠습니다.");
	Sleep(1000);
	//주사위
	while (1)
	{
		p1 = (rand() % 6) + 1;
		p2 = (rand() % 6) + 1;
		gotoxy(43, 25);
		printf("주사위 결과 플레이어1 : %d, 플레이어2 : %d", p1, p2);
		if (p1 > p2)
		{
			TypingMessage(43, 26, "플레이어의 턴으로 시작됩니다.");
			return 1;
		}
		else if (p1 < p2)
		{
			TypingMessage(43, 26, "컴퓨터의 턴으로 시작됩니다.");
			return 0;
		}
	}
}

//초기 게임판 구현된 함수를 꺼내옴
void print_gameset()
{
	gameset1(4, 2);
	gameset2(0);
	chip_interface(4, 7);
	chip_interface(102, 7);
	bet_interface(45, 5);
	print_card(0, PLAYER1_X, CARDSET_Y, 0);
	print_card(0, PLAYER2_X, CARDSET_Y, 0);
}

//칩의 개수를 설정 및 변동
void Chip_setting(inGamePlayer* p, int chip)
{
	p->chip = chip;
	p->betting_chip = 0;
}

//베팅을 프린트함 칩은 기본적으로 빨간색 ▣이며 10개단위로 파란색 ▣로 변경
void print_Betting(inGamePlayer* p1, int turn)
{
	int x, y;
	int i, j;
	int chip = p1->betting_chip;
	if (turn == 1)
	{
		x = 46;
		y = 9;
	}
	else
	{
		x = 69;
		y = 9;
	}

	textcolor(WHITE, WHITE);
	for (i = 0; i < 5; i++)
	{
		gotoxy(x, y + i);
		for (j = 0; j < 2; j++)
		{
			printf("　");
		}
	}

	for (i = 0; chip != 0; i++)
	{
		gotoxy(x, y + i);
		for (j = 0; j < 2; j++)
		{
			if (chip >= 10)
			{
				textcolor(BLUE2, WHITE);
				chip -= 10;
			}
			else
			{
				textcolor(RED1, WHITE);
				chip--;
			}
			printf("▣");
			if (chip <= 0)
				break;
		}
	}
}

void print_chip(inGamePlayer* p1, int turn)
{
	int x, y;
	int i, j;
	int chip = p1->chip;
	if (turn == 1)
	{
		x = 6;
		y = 8;
	}
	else
	{
		x = 104;
		y = 8;
	}

	textcolor(WHITE, WHITE);
	for (i = 0; i < 8; i++)
	{
		gotoxy(x, y + i);
		for (j = 0; j < 3; j++)
		{
			printf("　");
		}
	}

	for (i = 0; chip != 0; i++)
	{
		gotoxy(x, y + i);
		for (j = 0; j < 2; j++)
		{
			if (chip >= 10)
			{
				textcolor(BLUE2, WHITE);
				chip -= 10;
			}
			else
			{
				textcolor(RED1, WHITE);
				chip--;
			}
			printf("▣");
			if (chip <= 0)
				break;
		}
	}
}

void print_name(inGamePlayer p1, inGamePlayer p2)
{
	textcolor(YELLOW2, BLACK);
	gotoxy(26, 4);
	printf("%s", p1.name);
	gotoxy(86, 4);
	printf("%s", p2.name);
}

//vsCPU옵션
void vsCPU()
{
	int turn;
	inGamePlayer *p1, *p2;
	p1 = (inGamePlayer*)malloc(sizeof(inGamePlayer));
	p2 = (inGamePlayer*)malloc(sizeof(inGamePlayer));
	strcpy(p1->name, "Player1");
	strcpy(p2->name, "Player2");
	Chip_setting(p1, 30);
	Chip_setting(p2, 30);
	print_gameset();
	print_name(*p1, *p2);
	Tutorial();
	print_chip(p1, 1); print_chip(p2, 0);
	turn = dice();
	while (1)
	{
		print_Betting(p1, 1); print_Betting(p2, 0);
		print_chip(p1, 1); print_chip(p2, 0);
		if (p1->chip <= 0)
		{
			TypingMessage(40, 24, "축하합니다.");
			TypingMessage2(40, 25," 의 승리입니다.",p2->name);
			free(p1, p2);
			Sleep(2000);
			return;
		}
		else if (p2->chip <= 0)
		{
			TypingMessage(40, 24, "축하합니다.");
			TypingMessage2(40,25," 의 승리입니다.",p1->name);
			free(p1, p2);
			Sleep(2000);
			return;
		}
		gameset2(3);
		TypingMessage(30, 23, "각 플레이어는 매 게임 시작하기 전 칩 한개씩을 베팅해야합니다.");
		gameset2(4);
		p1->chip--; p1->betting_chip++;
		p2->chip--; p2->betting_chip++;
		print_Betting(p1, 1); print_Betting(p2, 0);
		print_chip(p1, 1); print_chip(p2, 0);

		p1->outcome = 0;
		p2->outcome = 0;
		suffle();
		betting_menu(p1, p2, turn);
	}
}

//서버연결
void use_Server()
{
	WSADATA wsaData;
	SOCKET hServSock;
	SOCKET hClntSock;
	SOCKADDR_IN servAddr;
	SOCKADDR_IN clntAddr;
	int szClntAddr;
	int strLen;

	char message[256] = { 0, };
	char port[] = "9888";
	char ip[] = "127.0.0.1";

	inGamePlayer *p1, *p2;
	int turn;
	int res = 0;
	int bet_chip;

	p1 = (inGamePlayer*)malloc(sizeof(inGamePlayer));
	p2 = (inGamePlayer*)malloc(sizeof(inGamePlayer));
	p1->card = 0; p1->betting_chip = 0; p1->chip = 30; p1->outcome = 0; p1->status = 0;
	*p2 = *p1;
	strcpy(p1->name, "Player1");
	strcpy(p2->name, "Player2");

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup() error!");
		return ;
	}

	printf("윈속라이브러리초기화\n");

	//서버소켓생성

	hServSock = socket(PF_INET, SOCK_STREAM, 0);

	if (hServSock == INVALID_SOCKET)
		return ;

	printf("서버소켓생성\n");

	memset(&servAddr, 0, sizeof(servAddr));

	servAddr.sin_family = AF_INET;

	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	servAddr.sin_port = htons(atoi(port));

	//서버소켓주소할당

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		return ;

	printf("서버소켓주소할당\n");

	//서버연결요청대기상태진입

	if (listen(hServSock, 5) == SOCKET_ERROR)
		return ;

	printf("연결요청대기상태\n");

	// 연결요청수락

	szClntAddr = sizeof(clntAddr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
	printf("연결요청수락\n");

	print_gameset();
	print_name(*p1, *p2);
	TypingMessage(35, 24, "선 플레이어를 결정합니다.");
	Sleep(200);
	gameset2(2);
	turn = rand() % 2;
	if (turn == 1)
	{
		p1->turn = 1;
		p2->turn = 0;
		TypingMessage2(30, 25, "부터 시작합니다.", p1->name);
	}
	else
	{
		p1->turn = 0;
		p2->turn = 1;
		TypingMessage2(30, 25, "부터 시작합니다.", p2->name);
	}
	strLen = send(hClntSock, (char*)p1, sizeof(inGamePlayer), 0); // 데이터전송
	if (strLen == SOCKET_ERROR)
	{
		printf("소켓에러!\n");
		return;
	}

	strLen = send(hClntSock, (char*)p2, sizeof(inGamePlayer), 0);
	if (strLen == SOCKET_ERROR)
	{
		printf("소켓 에러\n");
		return;
	}
	Sleep(200);
	gameset2(2);

	p1->card = card_select();
	p2->card = card_select();

	while (1)
	{
		p1->res = 0; bet_chip = 0; p2->res = 0;
		print_turn(p1->turn);
		print_chip(p1, 1);
		print_chip(p2, 2);
		print_card(0, PLAYER1_X, CARDSET_Y, 0);
		print_card(p2->card, PLAYER2_X, CARDSET_Y, 0);

		//승리조건
		if (p1->chip == 0 && p1->betting_chip == 0)
		{
			gameset2(2);
			TypingMessage2(30, 24, "의 패배입니다.", p1->name);
			TypingMessage(30, 25, "게임을 종료합니다.");
			Sleep(500);
			return;
		}
		else if (p2->chip == 0 && p2->betting_chip == 0)
		{
			gameset2(2);
			TypingMessage(30, 24, "축하합니다. 당신의 승리입니다.");
			TypingMessage(30, 25, "게임을 종료합니다.");
			Sleep(500);
			return;
		}

		gameset2(2);
		if (res == 0)
		{
			TypingMessage(30, 25, "각 플레이어는 시작하기 전 칩 1개씩을 베팅해야 합니다.");
			p1->chip--; p2->chip--;
			p1->betting_chip++; p2->betting_chip++;
			res = 1;
		}
		print_chip(p1, 1); print_chip(p2, 0);
		print_Betting(p1, 1); print_Betting(p2, 0);
		Sleep(200);
		strLen = send(hClntSock, (char*)p1, sizeof(inGamePlayer), 0); // 데이터전송
		if (strLen == SOCKET_ERROR)
		{
			printf("소켓에러!\n");
			return;
		}

		strLen = send(hClntSock, (char*)p2, sizeof(inGamePlayer), 0);
		if (strLen == SOCKET_ERROR)
		{
			printf("소켓 에러\n");
			return;
		}

		//본인의 턴
		if (p1->turn == 1)
		{
			gameset2(2);
			bet_chip = network_Betting(p1, p2, turn);
			p1->turn = 0; p2->turn = 1;

			p1->chip -= bet_chip;
			p1->betting_chip += bet_chip;

			print_Betting(p1, 1);
			print_Betting(p2, 0);

			if (bet_chip == 0) p1->res = -1;
			else if (bet_chip > 0 && p1->betting_chip == p2->betting_chip) p1->res = 1;
			else p1->res = 0;

			strLen = send(hClntSock, (char*)p1, sizeof(inGamePlayer), 0); // 데이터전송
			if (strLen == SOCKET_ERROR)
			{
				printf("소켓에러!\n");
				return;
			}

			strLen = send(hClntSock, (char*)p2, sizeof(inGamePlayer), 0);
			if (strLen == SOCKET_ERROR)
			{
				printf("소켓 에러\n");
				return;
			}

			if (bet_chip == 0)
			{
				gameset2(3);
				TypingMessage(30, 24, "이번 베팅에 포기를 하셨습니다. 카드를 공개하겠습니다.");
				Sleep(300);
				gameset2(3);
				p1->status = 0;
				print_card(p1->card, PLAYER1_X, CARDSET_Y, 0);
				TypingMessage(30, 25, "베팅에 포기하셨으므로 상대플레이어에게 칩이 돌아갑니다.");
				p2->chip = p2->chip + p1->betting_chip + p2->betting_chip;
				p1->betting_chip = p2->betting_chip = 0;
				p1->turn = 0; p2->turn = 1;
			}
			else
			{				
				print_Betting(p1, p1->turn);
				print_chip(p1, p1->turn);

				if (p1->betting_chip == p2->betting_chip)
				{
					gameset2(3);
					TypingMessage(30, 24, "두 플레이어가 같은 수의 칩을 베팅하였으므로 카드를 공개합니다.");
					print_card(p1->card, PLAYER1_X, CARDSET_Y, 0);
					if (p1->card > p2->card)
					{
						gameset2(2);
						TypingMessage2(30, 24, "의 승리 칩을 가져갑니다.", p1->name);
						p1->chip = p1->chip + p1->betting_chip + p2->betting_chip;
						p1->betting_chip = p2->betting_chip = 0;
						p1->turn = 1; p2->turn = 0;
					}
					else if (p2->card > p1->card)
					{
						gameset2(2);
						TypingMessage2(30, 24, "의 승리 칩을 가져갑니다.", p2->name);
						p2->chip = p2->chip + p1->betting_chip + p2->betting_chip;
						p1->betting_chip = p2->betting_chip = 0;
						p1->turn = 0; p2->turn = 1;
					}
					else
					{
						gameset2(2);
						TypingMessage(30, 24, "무승부이므로 베팅된 칩을 그대로 두고 다시 게임을 진행합니다.");
					}
					Sleep(300);
				}
			}
		}
		//상대의 턴
		else
		{
			gameset2(2);
			TypingMessage2(35, 24, "의 턴을 기다리는 중입니다...", p2->name);
			strLen = recv(hClntSock, (char*)p1, sizeof(inGamePlayer), 0);
			if (strLen == SOCKET_ERROR)
			{
				printf("소켓에러!\n");
				return;
			}
			strLen = recv(hClntSock, (char*)p2, sizeof(inGamePlayer), 0);
			if (strLen == SOCKET_ERROR)
			{
				printf("소켓에러!\n");
				return;
			}
			
			print_chip(p2, 0); print_Betting(p2, 0);
			print_chip(p1, 1); print_Betting(p1, 1);

			Sleep(500);
			gameset2(1);
			if (p2->res == 1 || p2->res == -1)
			{
				gameset2(2);
				if (p2->res == 1 && p1->betting_chip == p2->betting_chip)
				{
					TypingMessage(30, 24, "상대 플레이어가 베팅에 응했습니다. 카드를 공개합니다.");
					print_card(p1->card, PLAYER1_X, CARDSET_Y, 0);
					gameset2(2);
					Sleep(200);
				}
				else if (p2->res == -1)
				{
					TypingMessage(30, 24, "상대 플레이어가 베팅을 포기했습니다. 카드를 공개합니다.");
					print_card(p1->card, PLAYER1_X, CARDSET_Y, 0);
					Sleep(200);
				}
				if (p1->card > p2->card)
				{
					TypingMessage2(30, 24, "의 승리 칩을 이동합니다.", p1->name);
					p1->chip = p1->chip + p1->betting_chip + p2->betting_chip;
					p1->betting_chip = p2->betting_chip = 0;
					p1->turn = 1; p2->turn = 0;
				}
				else if (p1->card < p2->card) 
				{
					TypingMessage2(30, 24, "의 승리! 칩을 이동합니다.", p2->name); 
					p2->chip = p2->chip + p1->betting_chip + p2->betting_chip;
					p1->betting_chip = p2->betting_chip = 0;
					p1->turn = 0; p2->turn = 1;
				}
				else TypingMessage(30, 24, "카드가 같습니다. 베팅 칩을 그대로 둔 채 게임을 계속합니다.");
				print_chip(p2, 0); print_Betting(p2, 0);
				print_chip(p1, 1); print_Betting(p1, 1);
				p2->res = 0;
				Sleep(200);
			}
		}


		if (p1->res != 0 || p2->res != 0)
		{
			p1->card = card_select();
			p2->card = card_select();
			p1->res = 0;
			p2->res = 0;
			strLen = send(hClntSock, (char*)p1, sizeof(inGamePlayer), 0);
			if (strLen == SOCKET_ERROR) printf("소켓 에러");
			strLen = send(hClntSock, (char*)p2, sizeof(inGamePlayer), 0);
			if (strLen == SOCKET_ERROR) printf("소켓 에러");
			res = 0;
		}

		strLen = send(hClntSock, (char*)p1, sizeof(inGamePlayer), 0);
		if (strLen == SOCKET_ERROR) printf("소켓 에러");
		strLen = send(hClntSock, (char*)p2, sizeof(inGamePlayer), 0);
		if (strLen == SOCKET_ERROR) printf("소켓 에러");

		print_Betting(p1, 1);
		print_Betting(p2, 0);
		Sleep(200);
	}
	closesocket(hClntSock); // 연결종료
	printf("연결종료\n");
	WSACleanup(); // 할당된리소스를해제해준다
	printf("리소스해제\n");
	return;
}

//클라이언트
void use_Client()
{
	WSADATA wsaData;
	SOCKET hSocket;
	char message[256];
	int strLen;
	SOCKADDR_IN servAddr;
	char port[] = "9888";
	char ip[] = "127.0.0.1";
	inGamePlayer *p1, *p2;
	int res = 0;
	p1 = (inGamePlayer*)malloc(sizeof(inGamePlayer));
	p2 = (inGamePlayer*)malloc(sizeof(inGamePlayer));
	int bet_chip;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup() error!");
		return ;
	}

	printf("윈속라이브러리초기화\n");

	//클라이언트소켓생성

	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (hSocket == INVALID_SOCKET)
		return ;

	printf("클라이언트소켓생성\n");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(ip);
	servAddr.sin_port = htons(atoi(port));

	//서버로연결요청

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		return ;

	printf("서버에연결요청\n");

	print_gameset();

	strLen = recv(hSocket, (char*)p2, sizeof(inGamePlayer), 0);
	if (strLen == SOCKET_ERROR)
	{
		printf("소켓에러\n");
		return;
	}

	strLen = recv(hSocket, (char*)p1, sizeof(inGamePlayer), 0);
	if (strLen == SOCKET_ERROR)
	{
		printf("소켓 에러");
		return;
	}

	print_name(*p1, *p2);

	TypingMessage(30, 25, "선 플레이어를 결정합니다.");

	if(p1->turn == 1)	TypingMessage2(30, 26, "선입니다.",p1->name);
	else TypingMessage2(30, 26, "선입니다.", p2->name);

	while (1)
	{
		gameset2(2);

		strLen = recv(hSocket, (char*)p2, sizeof(inGamePlayer), 0);
		if (strLen == SOCKET_ERROR)
		{
			printf("소켓에러\n");
			return;
		}

		strLen = recv(hSocket, (char*)p1, sizeof(inGamePlayer), 0);
		if (strLen == SOCKET_ERROR)
		{
			printf("소켓 에러");
			return;
		}

		if (res == 0)
		{
			TypingMessage(30, 25, "각 플레이어는 시작하기 전 칩 1개씩을 베팅해야 합니다.");
			res = 1;
		}

		print_turn(p1->turn);
		print_card(0, PLAYER1_X, CARDSET_Y, 0);
		print_card(p2->card, PLAYER2_X, CARDSET_Y, 0);

		print_chip(p1, 1); print_chip(p2, 0);
		print_Betting(p1, 1); print_Betting(p2, 0);
		Sleep(200);

		//승리조건
		if (p1->chip == 0 && p1->betting_chip == 0)
		{
			gameset2(2);
			TypingMessage2(30, 24, "의 패배입니다.", p1->name);
			TypingMessage(30, 25, "게임을 종료합니다.");
			Sleep(500);
			return;
		}
		else if (p2->chip == 0 && p2->betting_chip == 0)
		{
			gameset2(2);
			TypingMessage(30, 24, "축하합니다. 당신의 승리입니다.");
			TypingMessage(30, 25, "게임을 종료합니다.");
			Sleep(500);
			return;
		}

		//본인의 턴
		if (p1->turn == 1)
		{
			p1->res = 0; bet_chip = 0;
			gameset2(2);
			bet_chip = network_Betting(p1, p2, p1->turn);
			p1->turn = 0; p2->turn = 1;

			p1->chip -= bet_chip;
			p1->betting_chip += bet_chip;
			print_Betting(p1, 1);
			print_Betting(p2, 0);

			if (bet_chip == 0) p1->res = -1;
			else if (bet_chip > 0 && p1->betting_chip == p2->betting_chip) p1->res = 1;
			else p1->res = 0;

			strLen = send(hSocket, (char*)p2, sizeof(inGamePlayer), 0);
			if (strLen == SOCKET_ERROR)
			{
				printf("소켓에러\n");
				break;
			}
			strLen = send(hSocket, (char*)p1, sizeof(inGamePlayer), 0);
			if (strLen == SOCKET_ERROR)
			{
				printf("소켓에러\n");
				break;
			}
		
			if (bet_chip == 0)
			{
				gameset2(3);
				TypingMessage(30, 24, "이번 베팅에 포기를 하셨습니다. 카드를 공개하겠습니다.");
				Sleep(300);
				gameset2(3);
				p1->status = 0;
				print_card(p1->card, PLAYER1_X, CARDSET_Y, 0);
				TypingMessage(30, 25, "베팅에 포기하셨으므로 상대플레이어에게 칩이 돌아갑니다.");
				p2->chip = p2->chip + p1->betting_chip + p2->betting_chip;
				p1->betting_chip = p2->betting_chip = 0;
			}
			else
			{
				if (p1->betting_chip == p2->betting_chip)
				{
					gameset2(3);
					TypingMessage(30, 24, "두 플레이어가 같은 수의 칩을 베팅하였으므로 카드를 공개합니다.");
					print_card(p1->card, PLAYER1_X, CARDSET_Y, 0);
					if (p1->card > p2->card)
					{
						gameset2(2);
						TypingMessage2(30, 24, "의 승리 칩을 가져갑니다.", p1->name);
						p1->chip = p1->chip + p1->betting_chip + p2->betting_chip;
						p1->betting_chip = p2->betting_chip = 0;
					}
					else if (p2->card > p1->card)
					{
						gameset2(2);
						TypingMessage2(30, 24, "의 승리 칩을 가져갑니다.", p2->name);
						p2->chip = p2->chip + p1->betting_chip + p2->betting_chip;
						p1->betting_chip = p2->betting_chip = 0;
					}
					else
					{
						gameset2(2);
						TypingMessage(30, 24, "무승부이므로 베팅된 칩을 그대로 두고 다시 게임을 진행합니다.");
					}
					Sleep(300);
				}
			}
		}
		//상대의 턴
		else
		{
			gameset2(2);
			TypingMessage2(35, 24, "의 턴을 기다리는 중입니다...", p2->name);
			strLen = recv(hSocket, (char*)p2, sizeof(inGamePlayer), 0);
			if (strLen == SOCKET_ERROR)
			{
				printf("소켓에러!\n");
				return;
			}
			strLen = recv(hSocket, (char*)p1, sizeof(inGamePlayer), 0);
			if (strLen == SOCKET_ERROR)
			{
				printf("소켓에러!\n");
				return;
			}
			
			print_chip(p2, 0); print_Betting(p2, 0);
			print_chip(p1, 1); print_Betting(p1, 1);

			Sleep(500);

			if (p2->res == 1 || p2->res == -1)
			{
				gameset2(2);
				if (p2->res == 1 && p1->betting_chip == p2->betting_chip)
				{
					TypingMessage(30, 24, "상대 플레이어가 베팅에 응했습니다. 카드를 공개합니다.");
					print_card(p1->card, PLAYER1_X, CARDSET_Y, 0);
					Sleep(200);
				}
				else if (p2->res == -1)
				{
					TypingMessage(30, 24, "상대 플레이어가 베팅을 포기했습니다. 카드를 공개합니다.");
					print_card(p1->card, PLAYER1_X, CARDSET_Y, 0);
					Sleep(200);
				}
				gameset2(2);
				if (p1->card > p2->card)
				{
					TypingMessage2(30, 24, "의 승리! 칩을 이동합니다.", p1->name);
					p1->chip = p1->chip + p1->betting_chip + p2->betting_chip;
					p1->betting_chip = p2->betting_chip = 0;

				}
				else if (p1->card < p2->card)
				{
					TypingMessage2(30, 24, "의 승리! 칩을 이동합니다.", p2->name);
					p2->chip = p2->chip + p1->betting_chip + p2->betting_chip;
					p1->betting_chip = p2->betting_chip = 0;
				}
				else TypingMessage(30, 24, "카드가 같습니다. 베팅 칩을 그대로 둔 채 게임을 계속합니다.");
				print_chip(p2, 0); print_Betting(p2, 0);
				print_chip(p1, 1); print_Betting(p1, 1);
				p2->res = 0;
				Sleep(200);
			}
		}

		if (p1->res != 0 || p2->res != 0)
		{
			p1->res = 0; p2->res = 0;
			strLen = recv(hSocket, (char*)p2, sizeof(inGamePlayer), 0);
			if (strLen == SOCKET_ERROR) printf("소켓 에러");
			strLen = recv(hSocket, (char*)p1, sizeof(inGamePlayer), 0);
			if (strLen == SOCKET_ERROR) printf("소켓 에러");
			res = 0;
		}

		strLen = recv(hSocket, (char*)p2, sizeof(inGamePlayer), 0);
		if (strLen == SOCKET_ERROR) printf("소켓 에러");
		strLen = recv(hSocket, (char*)p1, sizeof(inGamePlayer), 0);
		if (strLen == SOCKET_ERROR) printf("소켓 에러");
		print_Betting(p1, 1);
		print_Betting(p2, 0);
		Sleep(200);
	}

	closesocket(hSocket); // 연결종료
	printf("연결종료\n");
	WSACleanup();
	printf("리소스해제\n");
	return;
}

void vsNETWORK()
{
	//gotoxy(25, 20);
	cls(BLACK, WHITE);
	TypingMessage(25, 20, "1. 방만들기");
	TypingMessage(25, 21, "2. 접속하기");
	switch (getch())
	{
	case '1':
		use_Server();
		break;
	case '2':
		use_Client();
		break;
	}
}

//네트워크 플레이어 베팅현황 p1은 본인, 서버면 서버가 p1이고 클라이언트가 p2
//반대로 클라이언트라면 클라이언트가 p1이고 서버가 p2
int network_Betting(inGamePlayer* p1, inGamePlayer* p2, int turn)
{
	int max_chip, min_chip;
	int bet_chip;
	int res = 0;
	
	while (1)
	{
		if (p1->chip == 0)
		{
			gameset2(3);
			TypingMessage2(30, 24, "의 턴에 베팅할 칩이 없으므로 카드를 공개합니다.", p1);
			card_open(p1, p2, turn);
		}
		max_chip = (p1->chip + p1->betting_chip > p2->chip + p2->betting_chip) ? p2->chip : p1->chip;
		min_chip = ABS((p1->betting_chip - p2->betting_chip));
		min_chip = (min_chip <= 0) ? 1 : min_chip;
		gotoxy(30, 24);
		textcolor(WHITE, BLACK);
		printf("베팅 가능한 칩의 개수 [%02d] ~ [%02d] : [    ]", min_chip, max_chip);
		gotoxy(68, 24);
		scanf("\n%d", &bet_chip);
		Sleep(200);

		if (bet_chip > max_chip || bet_chip < min_chip)
		{
			gameset2(3);
			TypingMessage(30, 24, "잘못된 입력입니다. 다시 칩을 베팅 해 주세요.");
			while (getchar() != '\n');
			Sleep(300);
			gameset2(3);
		}
		return bet_chip;
	}
}

//CPU베팅옵션
//CPU는 한개 내지 두개로만 베팅하게되며, 올인은 지양
void CPU_Betting(inGamePlayer* p1, inGamePlayer* p2, int turn)
{
	int max_chip, min_chip;
	int bet_chip;
	int priority;
	while (1)
	{
		if (p1->chip == 0)
		{
			gameset2(3);
			TypingMessage2(30, 24, "의 턴에 베팅할 칩이 없으므로 카드를 공개합니다.", p1);
			card_open(p1, p2, turn);
		}
		max_chip = (p1->chip + p1->betting_chip > p2->chip + p2->betting_chip) ? p2->chip : p1->chip;
		min_chip = ABS((p1->betting_chip - p2->betting_chip));
		min_chip = (min_chip <= 0) ? 1 : min_chip;

		//우선순위 설정 (카드 10 = 1 , 9 = 2, 8 = 3, 7 = 4, 6 = 5 ......)
		priority = p2->card % 10 + 1;

		while (1)
		{
			if (priority + (rand()%10) >= p2->card)
			{
				bet_chip = min_chip + (rand() % 3);
				if (bet_chip > max_chip);
				else break;
			}
			else
			{
				bet_chip = 0;
				break;
			}
		}
		
		Sleep(500);

		if (bet_chip == 0)
		{
			gameset2(3);
			TypingMessage2(30, 24, "가 이번 베팅에 포기를 하셨습니다. 카드를 공개하겠습니다.",p1->name);
			Sleep(500);
			gameset2(3);
			p1->status = 0;
			card_open(p1, p2, turn);
			return;
		}
		else if (bet_chip > max_chip || bet_chip < min_chip)
		{
			
		}
		else
		{
			p1->chip -= bet_chip;
			p1->betting_chip += bet_chip;

			print_Betting(p1, turn);
			print_chip(p1, turn);

			if (p1->betting_chip == p2->betting_chip)
			{
				gameset2(3);
				TypingMessage(30, 24, "두 플레이어가 같은 수의 칩을 베팅하였으므로 카드를 공개합니다.");
				card_open(p1, p2, turn);
			}
			return;
		}
	}
}

//플레이어 베팅현황 p1 = 현재플레이어, p2 = 상대플레이어
void Player_Betting(inGamePlayer* p1, inGamePlayer* p2, int turn)
{
	int max_chip, min_chip;
	int bet_chip;
	while(1)
	{	
		if (p1->chip == 0)
		{
			gameset2(3);
			TypingMessage2(30, 24, "의 턴에 베팅할 칩이 없으므로 카드를 공개합니다.",p1);
			card_open(p1, p2, turn);
		}
		max_chip = (p1->chip + p1->betting_chip > p2->chip + p2->betting_chip) ? p2->chip : p1->chip;
		min_chip = ABS((p1->betting_chip - p2->betting_chip));
		min_chip = (min_chip <= 0) ? 1 : min_chip;
		gotoxy(30, 24);
		textcolor(WHITE, BLACK);
		printf("베팅 가능한 칩의 개수 [%02d] ~ [%02d] : [    ]", min_chip, max_chip);
		gotoxy(68, 24);
		scanf("\n%d", &bet_chip);
		Sleep(500);

		if (bet_chip == 0)
		{
			gameset2(3);
			TypingMessage(30, 24, "이번 베팅에 포기를 하셨습니다. 카드를 공개하겠습니다.");
			Sleep(300);
			gameset2(3);
			p1->status = 0;
			card_open(p1, p2, turn);
			return;
		}
		else if (bet_chip > max_chip || bet_chip < min_chip)
		{
			gameset2(3);
			TypingMessage(30, 24, "잘못된 입력입니다. 다시 칩을 베팅 해 주세요.");
			while (getchar() != '\n');
			Sleep(300);
			gameset2(3);
		}
		else
		{
			p1->chip -= bet_chip;
			p1->betting_chip += bet_chip;

			print_Betting(p1, turn);
			print_chip(p1, turn);
			
			if (p1->betting_chip == p2->betting_chip)
			{
				gameset2(3);
				TypingMessage(30, 24, "두 플레이어가 같은 수의 칩을 베팅하였으므로 카드를 공개합니다.");
				card_open(p1, p2, turn);
			}
			return;
		}
	}
}

//카드를 공개한다. -> 결과에 따라 베팅된 칩을 이동
void card_open(inGamePlayer *p1, inGamePlayer* p2, int turn)
{
	if (turn == 1)
		print_card(p1->card, PLAYER1_X, CARDSET_Y, 0);
	else
		print_card(p2->card, PLAYER1_X, CARDSET_Y, 0);
	Sleep(100);
	chip_move(p1, p2);
}

//베팅결과에따른 칩이동
void chip_move(inGamePlayer* p1, inGamePlayer* p2)
{
	gameset2(10);
	if (p1->status != 0 && p2->status != 0)
	{
		if (p1->card > p2->card)
		{
			TypingMessage2(40, 24, "의 승리! 칩을 획득합니다.", p1->name);
			Sleep(300);
			p1->chip = p1->chip + p1->betting_chip + p2->betting_chip;
			p1->betting_chip = 0;
			p2->betting_chip = 0;
			p1->outcome = 1;
		}
		else if (p1->card < p2->card)
		{
			TypingMessage2(40, 24, "의 승리! 칩을 획득합니다.",p2->name);
			Sleep(300);
			p2->chip = p2->chip + p1->betting_chip + p2->betting_chip;
			p1->betting_chip = 0;
			p2->betting_chip = 0;
			p2->outcome = 1;
		}
		else
		{
			TypingMessage(40, 24, "두 플레이어의 카드가 같습니다.");
			TypingMessage(40, 25, "베팅한 칩을 그대로 두고 다시 베팅을 진행합니다.");
			p1->outcome = 1;
			p2->outcome = 1;
		}
	}
	else if (p1->status == 0)
	{
		TypingMessage2(40, 24, "이 포기했습니다.",p1->name);
		TypingMessage2(40, 25, "에게 베팅된 칩 모두가 돌아갑니다.",p2->name);
		p2->chip = p2->chip + p1->betting_chip + p2->betting_chip;
		p1->betting_chip = 0;
		p2->betting_chip = 0;
		p2->outcome = 1;
	}
	
	Sleep(500);
	print_card(0, PLAYER1_X, CARDSET_Y, 0);
	print_card(0, PLAYER2_X, CARDSET_Y, 0);
	gotoxy(10, 0);
	printf("%02d %02d", p1->chip, p2->chip);
	p1->status = 1;
	p2->status = 1;
}

//카드를 고른다.
//중복체크를 해줌 1~10까지의 카드는 각각 두 장씩밖에없음.
int card_select()
{
	int card;
	int i,j;
	int chk=0;

	//모든 카드를 다 사용하였는지 확인
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if (use_card[i][j] == 1)
			{
				chk = 1;
				break;
			}
		}
		if (chk == 1) break;
	}

	while(1)
	{
		card = rand() % 10 + 1;
		if (use_card[0][card - 1] == -1 && use_card[1][card - 1] == -1);
		else if (use_card[0][card - 1] == -1)
		{
			use_card[1][card - 1] = -1;
			break;
		}
		else
		{
			use_card[0][card - 1] = -1;
			break;
		}
	}
	return card;
}

//메세지를 타이핑 한 것 처럼 보이게 만들기
void TypingMessage(int x, int y, char* message)
{
	int i;
	textcolor(WHITE, BLACK);
	gotoxy(x, y);
	for (i = 0; i < strlen(message); i++)
	{
		printf("%c", message[i]);
		if (message[i] & 0x80) Sleep(20); //입력된 문자가 2byte의 문자(ex 한글)일경우
		else Sleep(40);
	}
}
void TypingMessage2(int x, int y, char* message, char* name)
{
	int i;
	textcolor(WHITE, BLACK);
	gotoxy(x, y);
	printf("%s", name);
	for (i = 0; i < strlen(message); i++)
	{
		printf("%c", message[i]);
		if (message[i] & 0x80) Sleep(20); //입력된 문자가 2byte의 문자(ex 한글)일경우
		else Sleep(40);
	}
}
//
//튜토리얼 구현
void Tutorial()
{
	char select;
	textcolor(WHITE, BLACK);
	gotoxy(43, 24);
	TypingMessage(43, 24, "Indian Poker에 오신것을 환영합니다.");
	gotoxy(30, 25);
	TypingMessage(30, 25, "게임을 처음하신다면 1을 이미 해보신적이 있으시면 2를 눌러주세요");
	select = getch();

	switch (select)
	{
	case '1':
		gameset2(4);
		TypingMessage(9, 23, "인디언 포커는 포커게임의 일종으로, 더 큰 숫자카드를 가진 플레이어가");
		TypingMessage(9, 24, "베팅된 칩을 모두 가져가는 매우 간단한 룰을 가지고 있습니다.");
		Sleep(500);
		TypingMessage(9, 25, "인디언 포커는 1부터 10까지의 숫자 카드가 2세트씩 들어있는 총 20장의 카드로 진행됩니다.");
		Sleep(500);
		gameset2(4);
		TypingMessage(9, 23, "각 플레이어에게 카드가 1장씩 주어지게되면 플레이어는 자신의 카드를 볼 수 없습니다.");
		TypingMessage(9, 24, "다시말해 자신의 카드는 상대플레이어만 확인할 수 있습니다.");
		TypingMessage(9, 25, "카드를 받게되면 각 플레이어는 기본적으로 칩을 한 개씩 제시 한 후 선 플레이어부터 베팅을 시작합니다.");
		gameset2(4);
		TypingMessage(9, 23, "두 플레이어는 상대의 카드를 단서로 원하는 베팅을 할 수 있습니다.");
		TypingMessage(9, 24, "칩의 개수 역시 제한이 없습니다.");
		gameset2(4);
		TypingMessage(9, 23, "베팅에는 세가지 선택이 있습니다.");
		TypingMessage(9, 24, "첫째, 상대가 베팅한 개수와 같은 칩을 베팅");
		Sleep(500);
		TypingMessage(9, 25, "이때는 베팅이 그 즉시 종료되며 두사람의 카드가 공개됩니다.");
		gameset2(4);
		TypingMessage(9, 23, "둘째, 앞서 베팅한 칩보다 더 많은 칩을 베팅");
		Sleep(500);
		TypingMessage(9, 24, "이 경우에는 카드를 공개하지 않은 채 계속 베팅을 이어나갑니다.");
		gameset2(4);
		TypingMessage(9, 23, "셋째, 베팅을 하지않고 포기");
		Sleep(500);
		TypingMessage(9, 24, "이때는 카드의 결과와 상관없이 무조건 상대의 승리로 해당 판이 종료됩니다.");
		TypingMessage(9, 25, "하지만, 포기한 플레이어의 카드가 10카드였다면 패널티로 상대방에게 칩 10개를 주어야합니다.");
		gameset2(4);
		TypingMessage(9, 23, "만약 카드 공개 시 두 플레이어가 같은 수의 카드를 가지고 있다면,");
		TypingMessage(9, 24, "칩의 개수는 그대로 둔 채 다음 판으로 넘어갑니다.");
		gameset2(4);
		TypingMessage(9, 23, "한명의 플레이어가 칩을 모두 잃게되면 게임이 종료됩니다.");
		gameset2(4);
		TypingMessage(9, 23, "자, 이제 게임을 시작하겠습니다.");
		TypingMessage(9, 24, "즐거운 게임 되시기 바랍니다.");
		gameset2(4);
		break;
	case '2':
		gameset2(5);
		break;
	}
}