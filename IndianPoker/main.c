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
//1. ī��� 1~10���� ���� ī��� ���� 2��Ʈ�� �־�����
//2. �� �÷��̾�� ī�带 �ޱ��� �켱 1���� Ĩ�� �켱 �����ؾ��Ѵ�.
//3. �� �÷��̾�� �ڽ��� ī�带 �� �� ������, ����� ī�常���� ������ �ؾ��Ѵ�.
//4. ������ �켱 ������ ������ �ױ�(0������), �º���, �� �̻����� ������ �����ϴ�.
//5. ������� ī���� ����� ������� ������ Ĩ�� ��ΰ� ��뿡�� ���޵ȴ�.
//6. ���� ���� ������ �� ��� ���� ī�带 Ȯ���Ͽ� �� ���� ���ڸ� ������ �ִ� �÷��̾ ������ Ĩ�� ��������.
//7. All-in�� ��� ��밡 ������ �ִ� �ִ밳������ �� ���� ���� Ĩ�� ������ �� ����.
//8. ī���� ��� Ȯ�� �� Ĩ�� 0����� �ش� �÷��̾�� �й��Ѵ�.
//9. ����, ù ���� ���� Ĩ�� 0���� ���̻� ������ �� ���� ����� ī��� ��ð����ȴ�.

//COMPUTER
//1. ī�忡 ���� 1~10������ �켱������ ���Ѵ�.
//2. �켱������ �ٰ��� Ȯ���� ���� Ĩ�� 1�� ���� 2���� �����Ѵ�.

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
		"��������������������",
		"��������������������",
		"����������ᡡ����",
		"�����ᡡ�������ᡡ��",
		"���������������ᡡ��",
		"���������������ᡡ��",
		"������������ᡡ����",
		"���������ᡡ��������",
		"���������ᡡ��������",
		"��������������������",
		"���������ᡡ��������",
		"��������������������",
		"��������������������"
	},
	{
		"��������������������",
		"��������������������",
		"�������������� 1����",
		"��������������������",
		"��������������������",
		"��������������������",
		"�����������¡�������",
		"��������������������",
		"��������������������",
		"��������������������",
		"����1 ��������������",
		"��������������������",
		"��������������������"
	},
	{
		"��������������������",
		"��������������������",
		"�������������� 2����",
		"��������������������",
		"�����������¡�������",
		"��������������������",
		"��������������������",
		"��������������������",
		"�����������¡�������",
		"��������������������",
		"����2 ��������������",
		"��������������������",
		"��������������������"
	},
	{
		"��������������������",
		"��������������������",
		"�������������� 3����",
		"��������������������",
		"�����������¡�������",
		"��������������������",
		"�����������¡�������",
		"��������������������",
		"�����������¡�������",
		"��������������������",
		"����3 ��������������",
		"��������������������",
		"��������������������"
	},
	{
		"��������������������",
		"��������������������",
		"�������������� 4����",
		"��������������������",
		"�����¡��������¡���",
		"��������������������",
		"��������������������",
		"��������������������",
		"�����¡��������¡���",
		"��������������������",
		"����4 ��������������",
		"��������������������",
		"��������������������"
	},
	{
		"��������������������",
		"��������������������",
		"�������������� 5����",
		"�����¡��������¡���",
		"��������������������",
		"��������������������",
		"�������� �� ��������",
		"��������������������",
		"��������������������",
		"�����¡��������¡���",
		"����5 ��������������",
		"��������������������",
		"��������������������"
	},
	{
		"��������������������",
		"��������������������",
		"�������������� 6����",
		"�����¡��������¡���",
		"��������������������",
		"��������������������",
		"������ ������ �¡���",
		"��������������������",
		"��������������������",
		"�����¡��������¡���",
		"����6 ��������������",
		"��������������������",
		"��������������������"
	},
	{
		"��������������������",
		"��������������������",
		"�������������� 7����",
		"�����¡��������¡���",
		"��������������������",
		"��������������������",
		"��  �¡� �� ���¡���",
		"����  ����  ��������",
		"��������������������",
		"�����¡���    �¡���",
		"����7 ����    ������",
		"������������    ����",
		"��������������������"
	},
	{
		"��������������������",
		"��������������������",
		"�������������� 8����",
		"�����¡��������¡���",
		"��������������������",
		"�����¡��������¡���",
		"������������ ���� ��",
		"�����¡��������¡���",
		"�������� �������� ��",
		"�����¡��������¡���",
		"����8 ������ ���� ��",
		"�������������� �� ��",
		"��������������������"
	},
	{
		"��������������������",
		"��������������������",
		"���������� ����9����",
		"�����¡��������¡���",
		"�����¡��������¡���",
		"��������������������",
		"������ ���¡� ������",
		"��������������������",
		"�����¡��������¡���",
		"�����¡��������¡���",
		"����9 ��������������",
		"��������������������",
		"��������������������"
	},
	{
		"��������������������",
		"��������������������",
		"��������������10����",
		"��������������������",
		"�����¡��������¡���",
		"�����¡��������¡���",
		"�����¡��������¡���",
		"�����¡��������¡���",
		"�����¡��������¡���",
		"��������������������",
		"����10��������������",
		"��������������������",
		"��������������������"
	}

};

int use_card[2][10] = { -1 }; // -1�̸� ���� ī��, 1�̸� ���ȵ� ī��

typedef struct inGamePlayer
{
	char name[10];
	int card, chip;
	int betting_chip; //������ Ĩ
	int status; //����, ���� ����
	int outcome; //���п���
	int turn; //���� �ڽ��� ������ �ƴ��� ����
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

//Ŀ�� ��ǥ�� ����
void gotoxy(int x, int y) //���� ���ϴ� ��ġ�� Ŀ�� �̵�
{
	COORD pos; // Windows.h �� ����
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//�۾� ������ ����
void textcolor(int fg_color, int bg_color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}

//ȭ���ʱ�ȭ
void cls(int bg_color, int text_color) // ȭ�� �����
{
	char cmd[100];
	int cols = 120, lines = 30;
	sprintf(cmd, "COLOR %x%x", bg_color, text_color);
	system(cmd);
	sprintf(cmd, "mode con: cols=%d lines=%d", cols, lines);
	system(cmd);
}

void removeCursor(void)
{ // Ŀ���� �Ⱥ��̰� �Ѵ�
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

//ī��迭 �ʱ�ȭ
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

//ī�����
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
		printf("����������ᢺ");
	}
	else
	{
		printf("�������������");
	}
}

//chip �������̽�
void chip_interface(int x, int y)
{
	char pcChip[13][14] = {
		"������chip����",
		"��������������",
		"��������������",
		"��������������",
		"��������������",
		"��������������",
		"��������������",
		"��������������",
		"��������������",
		"��������������",
		"��������������",
		"��������������",
		"��������������",
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

//���� �������̽�
void bet_interface(int x, int y)
{
	char betting[16][30] = {
		"            BETTING           ",
		"������������������������������",
		"������������������������������",
		"������������������������������",
		"������������������������������",
		"������������������������������",
		"������������������������������",
		"������������������������������",
		"������������������������������",
		"������������������������������",
		"������������������������������",
		"������������������������������",
		"������������������������������",
		"������������������������������",
		"������������������������������",
		"������������������������������",
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

//�������� ����
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

//������ �ϴܺθ� ����
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

//���α׷� ���� �� ���� ���� ������ ȭ��
void main_page(int x, int y)
{
	int i = 0;
	textcolor(GREEN2, BLACK);
	gotoxy(x, y + 5);
	printf("���ᡡ�ᡡ�������ᡡ���ᡡ�����ᡡ�����ᡡ�����ᡡ��������\n");
	gotoxy(x, y + 6);
	printf("���ᡡ����ᡡ�����ᡡ�ᡡ���ᡡ���ᡡ�����ᡡ�ᡡ����ᡡ������\n");
	gotoxy(x, y + 7);
	printf("���ᡡ���ᡡ�ᡡ���ᡡ�ᡡ���ᡡ���ᡡ�����ᡡ�ᡡ���ᡡ�ᡡ����\n");
	gotoxy(x, y + 8);
	printf("���ᡡ���ᡡ���ᡡ�ᡡ�ᡡ���ᡡ���ᡡ�������ᡡ�ᡡ���ᡡ��\n");
	gotoxy(x, y + 9);
	printf("���ᡡ���ᡡ������ᡡ�ᡡ���ᡡ���ᡡ���ᡡ�����ᡡ�ᡡ�������\n");
	gotoxy(x, y + 10);
	printf("���ᡡ�ᡡ�������ᡡ���ᡡ�����ᡡ�ᡡ�����ᡡ�ᡡ��������\n");
	gotoxy(x, y + 12); textcolor(MAGENTA2, BLACK);
	printf("���������ᡡ�������ᡡ���ᡡ���ᡡ����ᡡ����\n");
	gotoxy(x, y + 13);
	printf("�������ᡡ���ᡡ�ᡡ�����ᡡ�ᡡ�ᡡ���ᡡ�������ᡡ����\n");
	gotoxy(x, y + 14);
	printf("�������ᡡ���ᡡ�ᡡ�����ᡡ��ᡡ�����ᡡ�������ᡡ����\n");
	gotoxy(x, y + 15);
	printf("���������ᡡ���ᡡ�����ᡡ��ᡡ��������ᡡ����\n");
	gotoxy(x, y + 16);
	printf("�������ᡡ�������ᡡ�����ᡡ�ᡡ�ᡡ���ᡡ�������ᡡ����\n");
	gotoxy(x, y + 17);
	printf("�������ᡡ�����������ᡡ���ᡡ���ᡡ����ᡡ�ᡡ����");

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

//���Ӽ���
void select_Game()
{
	while (1)
	{
		cls(BLACK, WHITE);
		textcolor(GREEN1, BLACK);
		gotoxy(20, 3);
		printf("���ᡡ�������ᡡ������  ���ᡡ�����ᡡ���ᡡ�ᡡ����");
		gotoxy(20, 4);
		printf("��ᡡ�������ᡡ�����ᡡ�ᡡ�������ᡡ�����ᡡ�ᡡ�ᡡ����");
		gotoxy(20, 5);
		printf("���ᡡ�������ᡡ�����ᡡ���ᡡ���ᡡ�������ᡡ�ᡡ����");
		gotoxy(20, 6);
		printf("���ᡡ���������ᡡ�ᡡ�������ᡡ���ᡡ�����ᡡ�����ᡡ����");
		gotoxy(20, 7);
		printf("���ᡡ�ᡡ�����ᡡ�������ᡡ�����ᡡ�ᡡ���������");

		textcolor(BLUE1, BLACK);
		gotoxy(20, 10);
		printf("���ᡡ�����ᡡ������  ���ᡡ�����ᡡ�ᡡ���������ᡡ�����ᡡ�����ᡡ����ᡡ����");
		gotoxy(20, 11);
		printf("�����ᡡ�����ᡡ�����ᡡ�ᡡ�������ᡡ�ᡡ�ᡡ�������ᡡ�ᡡ���ᡡ�����ᡡ�ᡡ�������ᡡ����");
		gotoxy(20, 12);
		printf("���ᡡ�����ᡡ�����ᡡ���ᡡ�����ᡡ�ᡡ���������ᡡ�����ᡡ�ᡡ������ᡡ����");
		gotoxy(20, 13);
		printf("�ᡡ�����������ᡡ�ᡡ�������ᡡ���ᡡ�����ᡡ�����ᡡ�����ᡡ�����ᡡ�����ᡡ�������ᡡ����");
		gotoxy(20, 14);
		printf("���ᡡ�ᡡ�����ᡡ�������ᡡ���ᡡ�������ᡡ�ᡡ�����ᡡ�����ᡡ��������ᡡ�ᡡ����");

		textcolor(RED1, BLACK);
		gotoxy(20, 17);
		printf("���ᡡ��������ᡡ�ᡡ�����ᡡ���ᡡ������");
		gotoxy(20, 18);
		printf("�����ᡡ�����ᡡ���������ᡡ�ᡡ�����ᡡ��������");
		gotoxy(20, 19);
		printf("���ᡡ��������ᡡ�����ᡡ�������ᡡ��������");
		gotoxy(20, 20);
		printf("�����ᡡ�����ᡡ���������ᡡ�ᡡ�����ᡡ��������");
		gotoxy(20, 21);
		printf("���ᡡ�ᡡ����ᡡ�ᡡ�����ᡡ���ᡡ������");

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
			TypingMessage(40, 25, "���α׷��� �����մϴ�.");
			gotoxy(40, 26);
			exit(1);
		case '9':
			break;
		}
	}
}

//�Ͽ� ���� ���ø޴� ����
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
			TypingMessage(30, 23, "�÷��̾��� ���Դϴ�. ������ �� �ּ���.");
			Player_Betting(p1, p2, turn);
			gameset2(2);
		}
		else
		{
			TypingMessage(30, 23, "��ǻ���� ��, �����ϴ� ���Դϴ�.");
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

	TypingMessage(43, 23, "�� �÷��̾ �����մϴ�.");
	TypingMessage(43, 24, "�� �÷��̾�� �ֻ����� �����ϰڽ��ϴ�.");
	Sleep(1000);
	//�ֻ���
	while (1)
	{
		p1 = (rand() % 6) + 1;
		p2 = (rand() % 6) + 1;
		gotoxy(43, 25);
		printf("�ֻ��� ��� �÷��̾�1 : %d, �÷��̾�2 : %d", p1, p2);
		if (p1 > p2)
		{
			TypingMessage(43, 26, "�÷��̾��� ������ ���۵˴ϴ�.");
			return 1;
		}
		else if (p1 < p2)
		{
			TypingMessage(43, 26, "��ǻ���� ������ ���۵˴ϴ�.");
			return 0;
		}
	}
}

//�ʱ� ������ ������ �Լ��� ������
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

//Ĩ�� ������ ���� �� ����
void Chip_setting(inGamePlayer* p, int chip)
{
	p->chip = chip;
	p->betting_chip = 0;
}

//������ ����Ʈ�� Ĩ�� �⺻������ ������ ���̸� 10�������� �Ķ��� �÷� ����
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
			printf("��");
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
			printf("��");
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
			printf("��");
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
			printf("��");
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

//vsCPU�ɼ�
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
			TypingMessage(40, 24, "�����մϴ�.");
			TypingMessage2(40, 25," �� �¸��Դϴ�.",p2->name);
			free(p1, p2);
			Sleep(2000);
			return;
		}
		else if (p2->chip <= 0)
		{
			TypingMessage(40, 24, "�����մϴ�.");
			TypingMessage2(40,25," �� �¸��Դϴ�.",p1->name);
			free(p1, p2);
			Sleep(2000);
			return;
		}
		gameset2(3);
		TypingMessage(30, 23, "�� �÷��̾�� �� ���� �����ϱ� �� Ĩ �Ѱ����� �����ؾ��մϴ�.");
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

//��������
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

	printf("���Ӷ��̺귯���ʱ�ȭ\n");

	//�������ϻ���

	hServSock = socket(PF_INET, SOCK_STREAM, 0);

	if (hServSock == INVALID_SOCKET)
		return ;

	printf("�������ϻ���\n");

	memset(&servAddr, 0, sizeof(servAddr));

	servAddr.sin_family = AF_INET;

	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	servAddr.sin_port = htons(atoi(port));

	//���������ּ��Ҵ�

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		return ;

	printf("���������ּ��Ҵ�\n");

	//���������û����������

	if (listen(hServSock, 5) == SOCKET_ERROR)
		return ;

	printf("�����û������\n");

	// �����û����

	szClntAddr = sizeof(clntAddr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
	printf("�����û����\n");

	print_gameset();
	print_name(*p1, *p2);
	TypingMessage(35, 24, "�� �÷��̾ �����մϴ�.");
	Sleep(200);
	gameset2(2);
	turn = rand() % 2;
	if (turn == 1)
	{
		p1->turn = 1;
		p2->turn = 0;
		TypingMessage2(30, 25, "���� �����մϴ�.", p1->name);
	}
	else
	{
		p1->turn = 0;
		p2->turn = 1;
		TypingMessage2(30, 25, "���� �����մϴ�.", p2->name);
	}
	strLen = send(hClntSock, (char*)p1, sizeof(inGamePlayer), 0); // ����������
	if (strLen == SOCKET_ERROR)
	{
		printf("���Ͽ���!\n");
		return;
	}

	strLen = send(hClntSock, (char*)p2, sizeof(inGamePlayer), 0);
	if (strLen == SOCKET_ERROR)
	{
		printf("���� ����\n");
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

		//�¸�����
		if (p1->chip == 0 && p1->betting_chip == 0)
		{
			gameset2(2);
			TypingMessage2(30, 24, "�� �й��Դϴ�.", p1->name);
			TypingMessage(30, 25, "������ �����մϴ�.");
			Sleep(500);
			return;
		}
		else if (p2->chip == 0 && p2->betting_chip == 0)
		{
			gameset2(2);
			TypingMessage(30, 24, "�����մϴ�. ����� �¸��Դϴ�.");
			TypingMessage(30, 25, "������ �����մϴ�.");
			Sleep(500);
			return;
		}

		gameset2(2);
		if (res == 0)
		{
			TypingMessage(30, 25, "�� �÷��̾�� �����ϱ� �� Ĩ 1������ �����ؾ� �մϴ�.");
			p1->chip--; p2->chip--;
			p1->betting_chip++; p2->betting_chip++;
			res = 1;
		}
		print_chip(p1, 1); print_chip(p2, 0);
		print_Betting(p1, 1); print_Betting(p2, 0);
		Sleep(200);
		strLen = send(hClntSock, (char*)p1, sizeof(inGamePlayer), 0); // ����������
		if (strLen == SOCKET_ERROR)
		{
			printf("���Ͽ���!\n");
			return;
		}

		strLen = send(hClntSock, (char*)p2, sizeof(inGamePlayer), 0);
		if (strLen == SOCKET_ERROR)
		{
			printf("���� ����\n");
			return;
		}

		//������ ��
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

			strLen = send(hClntSock, (char*)p1, sizeof(inGamePlayer), 0); // ����������
			if (strLen == SOCKET_ERROR)
			{
				printf("���Ͽ���!\n");
				return;
			}

			strLen = send(hClntSock, (char*)p2, sizeof(inGamePlayer), 0);
			if (strLen == SOCKET_ERROR)
			{
				printf("���� ����\n");
				return;
			}

			if (bet_chip == 0)
			{
				gameset2(3);
				TypingMessage(30, 24, "�̹� ���ÿ� ���⸦ �ϼ̽��ϴ�. ī�带 �����ϰڽ��ϴ�.");
				Sleep(300);
				gameset2(3);
				p1->status = 0;
				print_card(p1->card, PLAYER1_X, CARDSET_Y, 0);
				TypingMessage(30, 25, "���ÿ� �����ϼ����Ƿ� ����÷��̾�� Ĩ�� ���ư��ϴ�.");
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
					TypingMessage(30, 24, "�� �÷��̾ ���� ���� Ĩ�� �����Ͽ����Ƿ� ī�带 �����մϴ�.");
					print_card(p1->card, PLAYER1_X, CARDSET_Y, 0);
					if (p1->card > p2->card)
					{
						gameset2(2);
						TypingMessage2(30, 24, "�� �¸� Ĩ�� �������ϴ�.", p1->name);
						p1->chip = p1->chip + p1->betting_chip + p2->betting_chip;
						p1->betting_chip = p2->betting_chip = 0;
						p1->turn = 1; p2->turn = 0;
					}
					else if (p2->card > p1->card)
					{
						gameset2(2);
						TypingMessage2(30, 24, "�� �¸� Ĩ�� �������ϴ�.", p2->name);
						p2->chip = p2->chip + p1->betting_chip + p2->betting_chip;
						p1->betting_chip = p2->betting_chip = 0;
						p1->turn = 0; p2->turn = 1;
					}
					else
					{
						gameset2(2);
						TypingMessage(30, 24, "���º��̹Ƿ� ���õ� Ĩ�� �״�� �ΰ� �ٽ� ������ �����մϴ�.");
					}
					Sleep(300);
				}
			}
		}
		//����� ��
		else
		{
			gameset2(2);
			TypingMessage2(35, 24, "�� ���� ��ٸ��� ���Դϴ�...", p2->name);
			strLen = recv(hClntSock, (char*)p1, sizeof(inGamePlayer), 0);
			if (strLen == SOCKET_ERROR)
			{
				printf("���Ͽ���!\n");
				return;
			}
			strLen = recv(hClntSock, (char*)p2, sizeof(inGamePlayer), 0);
			if (strLen == SOCKET_ERROR)
			{
				printf("���Ͽ���!\n");
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
					TypingMessage(30, 24, "��� �÷��̾ ���ÿ� ���߽��ϴ�. ī�带 �����մϴ�.");
					print_card(p1->card, PLAYER1_X, CARDSET_Y, 0);
					gameset2(2);
					Sleep(200);
				}
				else if (p2->res == -1)
				{
					TypingMessage(30, 24, "��� �÷��̾ ������ �����߽��ϴ�. ī�带 �����մϴ�.");
					print_card(p1->card, PLAYER1_X, CARDSET_Y, 0);
					Sleep(200);
				}
				if (p1->card > p2->card)
				{
					TypingMessage2(30, 24, "�� �¸� Ĩ�� �̵��մϴ�.", p1->name);
					p1->chip = p1->chip + p1->betting_chip + p2->betting_chip;
					p1->betting_chip = p2->betting_chip = 0;
					p1->turn = 1; p2->turn = 0;
				}
				else if (p1->card < p2->card) 
				{
					TypingMessage2(30, 24, "�� �¸�! Ĩ�� �̵��մϴ�.", p2->name); 
					p2->chip = p2->chip + p1->betting_chip + p2->betting_chip;
					p1->betting_chip = p2->betting_chip = 0;
					p1->turn = 0; p2->turn = 1;
				}
				else TypingMessage(30, 24, "ī�尡 �����ϴ�. ���� Ĩ�� �״�� �� ä ������ ����մϴ�.");
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
			if (strLen == SOCKET_ERROR) printf("���� ����");
			strLen = send(hClntSock, (char*)p2, sizeof(inGamePlayer), 0);
			if (strLen == SOCKET_ERROR) printf("���� ����");
			res = 0;
		}

		strLen = send(hClntSock, (char*)p1, sizeof(inGamePlayer), 0);
		if (strLen == SOCKET_ERROR) printf("���� ����");
		strLen = send(hClntSock, (char*)p2, sizeof(inGamePlayer), 0);
		if (strLen == SOCKET_ERROR) printf("���� ����");

		print_Betting(p1, 1);
		print_Betting(p2, 0);
		Sleep(200);
	}
	closesocket(hClntSock); // ��������
	printf("��������\n");
	WSACleanup(); // �Ҵ�ȸ��ҽ����������ش�
	printf("���ҽ�����\n");
	return;
}

//Ŭ���̾�Ʈ
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

	printf("���Ӷ��̺귯���ʱ�ȭ\n");

	//Ŭ���̾�Ʈ���ϻ���

	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (hSocket == INVALID_SOCKET)
		return ;

	printf("Ŭ���̾�Ʈ���ϻ���\n");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(ip);
	servAddr.sin_port = htons(atoi(port));

	//�����ο����û

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		return ;

	printf("�����������û\n");

	print_gameset();

	strLen = recv(hSocket, (char*)p2, sizeof(inGamePlayer), 0);
	if (strLen == SOCKET_ERROR)
	{
		printf("���Ͽ���\n");
		return;
	}

	strLen = recv(hSocket, (char*)p1, sizeof(inGamePlayer), 0);
	if (strLen == SOCKET_ERROR)
	{
		printf("���� ����");
		return;
	}

	print_name(*p1, *p2);

	TypingMessage(30, 25, "�� �÷��̾ �����մϴ�.");

	if(p1->turn == 1)	TypingMessage2(30, 26, "���Դϴ�.",p1->name);
	else TypingMessage2(30, 26, "���Դϴ�.", p2->name);

	while (1)
	{
		gameset2(2);

		strLen = recv(hSocket, (char*)p2, sizeof(inGamePlayer), 0);
		if (strLen == SOCKET_ERROR)
		{
			printf("���Ͽ���\n");
			return;
		}

		strLen = recv(hSocket, (char*)p1, sizeof(inGamePlayer), 0);
		if (strLen == SOCKET_ERROR)
		{
			printf("���� ����");
			return;
		}

		if (res == 0)
		{
			TypingMessage(30, 25, "�� �÷��̾�� �����ϱ� �� Ĩ 1������ �����ؾ� �մϴ�.");
			res = 1;
		}

		print_turn(p1->turn);
		print_card(0, PLAYER1_X, CARDSET_Y, 0);
		print_card(p2->card, PLAYER2_X, CARDSET_Y, 0);

		print_chip(p1, 1); print_chip(p2, 0);
		print_Betting(p1, 1); print_Betting(p2, 0);
		Sleep(200);

		//�¸�����
		if (p1->chip == 0 && p1->betting_chip == 0)
		{
			gameset2(2);
			TypingMessage2(30, 24, "�� �й��Դϴ�.", p1->name);
			TypingMessage(30, 25, "������ �����մϴ�.");
			Sleep(500);
			return;
		}
		else if (p2->chip == 0 && p2->betting_chip == 0)
		{
			gameset2(2);
			TypingMessage(30, 24, "�����մϴ�. ����� �¸��Դϴ�.");
			TypingMessage(30, 25, "������ �����մϴ�.");
			Sleep(500);
			return;
		}

		//������ ��
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
				printf("���Ͽ���\n");
				break;
			}
			strLen = send(hSocket, (char*)p1, sizeof(inGamePlayer), 0);
			if (strLen == SOCKET_ERROR)
			{
				printf("���Ͽ���\n");
				break;
			}
		
			if (bet_chip == 0)
			{
				gameset2(3);
				TypingMessage(30, 24, "�̹� ���ÿ� ���⸦ �ϼ̽��ϴ�. ī�带 �����ϰڽ��ϴ�.");
				Sleep(300);
				gameset2(3);
				p1->status = 0;
				print_card(p1->card, PLAYER1_X, CARDSET_Y, 0);
				TypingMessage(30, 25, "���ÿ� �����ϼ����Ƿ� ����÷��̾�� Ĩ�� ���ư��ϴ�.");
				p2->chip = p2->chip + p1->betting_chip + p2->betting_chip;
				p1->betting_chip = p2->betting_chip = 0;
			}
			else
			{
				if (p1->betting_chip == p2->betting_chip)
				{
					gameset2(3);
					TypingMessage(30, 24, "�� �÷��̾ ���� ���� Ĩ�� �����Ͽ����Ƿ� ī�带 �����մϴ�.");
					print_card(p1->card, PLAYER1_X, CARDSET_Y, 0);
					if (p1->card > p2->card)
					{
						gameset2(2);
						TypingMessage2(30, 24, "�� �¸� Ĩ�� �������ϴ�.", p1->name);
						p1->chip = p1->chip + p1->betting_chip + p2->betting_chip;
						p1->betting_chip = p2->betting_chip = 0;
					}
					else if (p2->card > p1->card)
					{
						gameset2(2);
						TypingMessage2(30, 24, "�� �¸� Ĩ�� �������ϴ�.", p2->name);
						p2->chip = p2->chip + p1->betting_chip + p2->betting_chip;
						p1->betting_chip = p2->betting_chip = 0;
					}
					else
					{
						gameset2(2);
						TypingMessage(30, 24, "���º��̹Ƿ� ���õ� Ĩ�� �״�� �ΰ� �ٽ� ������ �����մϴ�.");
					}
					Sleep(300);
				}
			}
		}
		//����� ��
		else
		{
			gameset2(2);
			TypingMessage2(35, 24, "�� ���� ��ٸ��� ���Դϴ�...", p2->name);
			strLen = recv(hSocket, (char*)p2, sizeof(inGamePlayer), 0);
			if (strLen == SOCKET_ERROR)
			{
				printf("���Ͽ���!\n");
				return;
			}
			strLen = recv(hSocket, (char*)p1, sizeof(inGamePlayer), 0);
			if (strLen == SOCKET_ERROR)
			{
				printf("���Ͽ���!\n");
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
					TypingMessage(30, 24, "��� �÷��̾ ���ÿ� ���߽��ϴ�. ī�带 �����մϴ�.");
					print_card(p1->card, PLAYER1_X, CARDSET_Y, 0);
					Sleep(200);
				}
				else if (p2->res == -1)
				{
					TypingMessage(30, 24, "��� �÷��̾ ������ �����߽��ϴ�. ī�带 �����մϴ�.");
					print_card(p1->card, PLAYER1_X, CARDSET_Y, 0);
					Sleep(200);
				}
				gameset2(2);
				if (p1->card > p2->card)
				{
					TypingMessage2(30, 24, "�� �¸�! Ĩ�� �̵��մϴ�.", p1->name);
					p1->chip = p1->chip + p1->betting_chip + p2->betting_chip;
					p1->betting_chip = p2->betting_chip = 0;

				}
				else if (p1->card < p2->card)
				{
					TypingMessage2(30, 24, "�� �¸�! Ĩ�� �̵��մϴ�.", p2->name);
					p2->chip = p2->chip + p1->betting_chip + p2->betting_chip;
					p1->betting_chip = p2->betting_chip = 0;
				}
				else TypingMessage(30, 24, "ī�尡 �����ϴ�. ���� Ĩ�� �״�� �� ä ������ ����մϴ�.");
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
			if (strLen == SOCKET_ERROR) printf("���� ����");
			strLen = recv(hSocket, (char*)p1, sizeof(inGamePlayer), 0);
			if (strLen == SOCKET_ERROR) printf("���� ����");
			res = 0;
		}

		strLen = recv(hSocket, (char*)p2, sizeof(inGamePlayer), 0);
		if (strLen == SOCKET_ERROR) printf("���� ����");
		strLen = recv(hSocket, (char*)p1, sizeof(inGamePlayer), 0);
		if (strLen == SOCKET_ERROR) printf("���� ����");
		print_Betting(p1, 1);
		print_Betting(p2, 0);
		Sleep(200);
	}

	closesocket(hSocket); // ��������
	printf("��������\n");
	WSACleanup();
	printf("���ҽ�����\n");
	return;
}

void vsNETWORK()
{
	//gotoxy(25, 20);
	cls(BLACK, WHITE);
	TypingMessage(25, 20, "1. �游���");
	TypingMessage(25, 21, "2. �����ϱ�");
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

//��Ʈ��ũ �÷��̾� ������Ȳ p1�� ����, ������ ������ p1�̰� Ŭ���̾�Ʈ�� p2
//�ݴ�� Ŭ���̾�Ʈ��� Ŭ���̾�Ʈ�� p1�̰� ������ p2
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
			TypingMessage2(30, 24, "�� �Ͽ� ������ Ĩ�� �����Ƿ� ī�带 �����մϴ�.", p1);
			card_open(p1, p2, turn);
		}
		max_chip = (p1->chip + p1->betting_chip > p2->chip + p2->betting_chip) ? p2->chip : p1->chip;
		min_chip = ABS((p1->betting_chip - p2->betting_chip));
		min_chip = (min_chip <= 0) ? 1 : min_chip;
		gotoxy(30, 24);
		textcolor(WHITE, BLACK);
		printf("���� ������ Ĩ�� ���� [%02d] ~ [%02d] : [    ]", min_chip, max_chip);
		gotoxy(68, 24);
		scanf("\n%d", &bet_chip);
		Sleep(200);

		if (bet_chip > max_chip || bet_chip < min_chip)
		{
			gameset2(3);
			TypingMessage(30, 24, "�߸��� �Է��Դϴ�. �ٽ� Ĩ�� ���� �� �ּ���.");
			while (getchar() != '\n');
			Sleep(300);
			gameset2(3);
		}
		return bet_chip;
	}
}

//CPU���ÿɼ�
//CPU�� �Ѱ� ���� �ΰ��θ� �����ϰԵǸ�, ������ ����
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
			TypingMessage2(30, 24, "�� �Ͽ� ������ Ĩ�� �����Ƿ� ī�带 �����մϴ�.", p1);
			card_open(p1, p2, turn);
		}
		max_chip = (p1->chip + p1->betting_chip > p2->chip + p2->betting_chip) ? p2->chip : p1->chip;
		min_chip = ABS((p1->betting_chip - p2->betting_chip));
		min_chip = (min_chip <= 0) ? 1 : min_chip;

		//�켱���� ���� (ī�� 10 = 1 , 9 = 2, 8 = 3, 7 = 4, 6 = 5 ......)
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
			TypingMessage2(30, 24, "�� �̹� ���ÿ� ���⸦ �ϼ̽��ϴ�. ī�带 �����ϰڽ��ϴ�.",p1->name);
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
				TypingMessage(30, 24, "�� �÷��̾ ���� ���� Ĩ�� �����Ͽ����Ƿ� ī�带 �����մϴ�.");
				card_open(p1, p2, turn);
			}
			return;
		}
	}
}

//�÷��̾� ������Ȳ p1 = �����÷��̾�, p2 = ����÷��̾�
void Player_Betting(inGamePlayer* p1, inGamePlayer* p2, int turn)
{
	int max_chip, min_chip;
	int bet_chip;
	while(1)
	{	
		if (p1->chip == 0)
		{
			gameset2(3);
			TypingMessage2(30, 24, "�� �Ͽ� ������ Ĩ�� �����Ƿ� ī�带 �����մϴ�.",p1);
			card_open(p1, p2, turn);
		}
		max_chip = (p1->chip + p1->betting_chip > p2->chip + p2->betting_chip) ? p2->chip : p1->chip;
		min_chip = ABS((p1->betting_chip - p2->betting_chip));
		min_chip = (min_chip <= 0) ? 1 : min_chip;
		gotoxy(30, 24);
		textcolor(WHITE, BLACK);
		printf("���� ������ Ĩ�� ���� [%02d] ~ [%02d] : [    ]", min_chip, max_chip);
		gotoxy(68, 24);
		scanf("\n%d", &bet_chip);
		Sleep(500);

		if (bet_chip == 0)
		{
			gameset2(3);
			TypingMessage(30, 24, "�̹� ���ÿ� ���⸦ �ϼ̽��ϴ�. ī�带 �����ϰڽ��ϴ�.");
			Sleep(300);
			gameset2(3);
			p1->status = 0;
			card_open(p1, p2, turn);
			return;
		}
		else if (bet_chip > max_chip || bet_chip < min_chip)
		{
			gameset2(3);
			TypingMessage(30, 24, "�߸��� �Է��Դϴ�. �ٽ� Ĩ�� ���� �� �ּ���.");
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
				TypingMessage(30, 24, "�� �÷��̾ ���� ���� Ĩ�� �����Ͽ����Ƿ� ī�带 �����մϴ�.");
				card_open(p1, p2, turn);
			}
			return;
		}
	}
}

//ī�带 �����Ѵ�. -> ����� ���� ���õ� Ĩ�� �̵�
void card_open(inGamePlayer *p1, inGamePlayer* p2, int turn)
{
	if (turn == 1)
		print_card(p1->card, PLAYER1_X, CARDSET_Y, 0);
	else
		print_card(p2->card, PLAYER1_X, CARDSET_Y, 0);
	Sleep(100);
	chip_move(p1, p2);
}

//���ð�������� Ĩ�̵�
void chip_move(inGamePlayer* p1, inGamePlayer* p2)
{
	gameset2(10);
	if (p1->status != 0 && p2->status != 0)
	{
		if (p1->card > p2->card)
		{
			TypingMessage2(40, 24, "�� �¸�! Ĩ�� ȹ���մϴ�.", p1->name);
			Sleep(300);
			p1->chip = p1->chip + p1->betting_chip + p2->betting_chip;
			p1->betting_chip = 0;
			p2->betting_chip = 0;
			p1->outcome = 1;
		}
		else if (p1->card < p2->card)
		{
			TypingMessage2(40, 24, "�� �¸�! Ĩ�� ȹ���մϴ�.",p2->name);
			Sleep(300);
			p2->chip = p2->chip + p1->betting_chip + p2->betting_chip;
			p1->betting_chip = 0;
			p2->betting_chip = 0;
			p2->outcome = 1;
		}
		else
		{
			TypingMessage(40, 24, "�� �÷��̾��� ī�尡 �����ϴ�.");
			TypingMessage(40, 25, "������ Ĩ�� �״�� �ΰ� �ٽ� ������ �����մϴ�.");
			p1->outcome = 1;
			p2->outcome = 1;
		}
	}
	else if (p1->status == 0)
	{
		TypingMessage2(40, 24, "�� �����߽��ϴ�.",p1->name);
		TypingMessage2(40, 25, "���� ���õ� Ĩ ��ΰ� ���ư��ϴ�.",p2->name);
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

//ī�带 ����.
//�ߺ�üũ�� ���� 1~10������ ī��� ���� �� �徿�ۿ�����.
int card_select()
{
	int card;
	int i,j;
	int chk=0;

	//��� ī�带 �� ����Ͽ����� Ȯ��
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

//�޼����� Ÿ���� �� �� ó�� ���̰� �����
void TypingMessage(int x, int y, char* message)
{
	int i;
	textcolor(WHITE, BLACK);
	gotoxy(x, y);
	for (i = 0; i < strlen(message); i++)
	{
		printf("%c", message[i]);
		if (message[i] & 0x80) Sleep(20); //�Էµ� ���ڰ� 2byte�� ����(ex �ѱ�)�ϰ��
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
		if (message[i] & 0x80) Sleep(20); //�Էµ� ���ڰ� 2byte�� ����(ex �ѱ�)�ϰ��
		else Sleep(40);
	}
}
//
//Ʃ�丮�� ����
void Tutorial()
{
	char select;
	textcolor(WHITE, BLACK);
	gotoxy(43, 24);
	TypingMessage(43, 24, "Indian Poker�� ���Ű��� ȯ���մϴ�.");
	gotoxy(30, 25);
	TypingMessage(30, 25, "������ ó���ϽŴٸ� 1�� �̹� �غ������� �����ø� 2�� �����ּ���");
	select = getch();

	switch (select)
	{
	case '1':
		gameset2(4);
		TypingMessage(9, 23, "�ε�� ��Ŀ�� ��Ŀ������ ��������, �� ū ����ī�带 ���� �÷��̾");
		TypingMessage(9, 24, "���õ� Ĩ�� ��� �������� �ſ� ������ ���� ������ �ֽ��ϴ�.");
		Sleep(500);
		TypingMessage(9, 25, "�ε�� ��Ŀ�� 1���� 10������ ���� ī�尡 2��Ʈ�� ����ִ� �� 20���� ī��� ����˴ϴ�.");
		Sleep(500);
		gameset2(4);
		TypingMessage(9, 23, "�� �÷��̾�� ī�尡 1�徿 �־����ԵǸ� �÷��̾�� �ڽ��� ī�带 �� �� �����ϴ�.");
		TypingMessage(9, 24, "�ٽø��� �ڽ��� ī��� ����÷��̾ Ȯ���� �� �ֽ��ϴ�.");
		TypingMessage(9, 25, "ī�带 �ްԵǸ� �� �÷��̾�� �⺻������ Ĩ�� �� ���� ���� �� �� �� �÷��̾���� ������ �����մϴ�.");
		gameset2(4);
		TypingMessage(9, 23, "�� �÷��̾�� ����� ī�带 �ܼ��� ���ϴ� ������ �� �� �ֽ��ϴ�.");
		TypingMessage(9, 24, "Ĩ�� ���� ���� ������ �����ϴ�.");
		gameset2(4);
		TypingMessage(9, 23, "���ÿ��� ������ ������ �ֽ��ϴ�.");
		TypingMessage(9, 24, "ù°, ��밡 ������ ������ ���� Ĩ�� ����");
		Sleep(500);
		TypingMessage(9, 25, "�̶��� ������ �� ��� ����Ǹ� �λ���� ī�尡 �����˴ϴ�.");
		gameset2(4);
		TypingMessage(9, 23, "��°, �ռ� ������ Ĩ���� �� ���� Ĩ�� ����");
		Sleep(500);
		TypingMessage(9, 24, "�� ��쿡�� ī�带 �������� ���� ä ��� ������ �̾���ϴ�.");
		gameset2(4);
		TypingMessage(9, 23, "��°, ������ �����ʰ� ����");
		Sleep(500);
		TypingMessage(9, 24, "�̶��� ī���� ����� ������� ������ ����� �¸��� �ش� ���� ����˴ϴ�.");
		TypingMessage(9, 25, "������, ������ �÷��̾��� ī�尡 10ī�忴�ٸ� �г�Ƽ�� ���濡�� Ĩ 10���� �־���մϴ�.");
		gameset2(4);
		TypingMessage(9, 23, "���� ī�� ���� �� �� �÷��̾ ���� ���� ī�带 ������ �ִٸ�,");
		TypingMessage(9, 24, "Ĩ�� ������ �״�� �� ä ���� ������ �Ѿ�ϴ�.");
		gameset2(4);
		TypingMessage(9, 23, "�Ѹ��� �÷��̾ Ĩ�� ��� �ҰԵǸ� ������ ����˴ϴ�.");
		gameset2(4);
		TypingMessage(9, 23, "��, ���� ������ �����ϰڽ��ϴ�.");
		TypingMessage(9, 24, "��ſ� ���� �ǽñ� �ٶ��ϴ�.");
		gameset2(4);
		break;
	case '2':
		gameset2(5);
		break;
	}
}