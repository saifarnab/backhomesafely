#include "iGraphics.h"
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <shellapi.h>
#include <Windows.h>
#include <MMSystem.h>

#define lifetag 3
#define wintag 45
#define secondtag 121
#define speedtag 3

int wb = 0, winflag, walk = 1, lossflag, boatride = 0, i, j, w;
int start = 0, speed = speedtag, second = secondtag, score = 0, life = lifetag, win = wintag, wincount = 0, level = 1, scoreflag = 0, helpflag = 0, aboutusflag = 0, pauseflag = 0, wellcome = 1;
int cartimer, boattimer, timetimer;
int homecount = 0, homex[10];//homex[10] is boy's safe position value to the x-axis//
int randbgm, safebgm, randcar[5] = { 0, 1, 2, 3, 4 }, randcarr[5] = { 0, 1, 2, 3, 4 }; //randbgm=generates random number for background music.
int home[10], filled[10];//home[10] and filled[10] is to count the number of how many boys has back to home, home[10] is to print boysafe.bmp and 
//filled[10] is to keep an eye not to give place a boy in an allready filled home, there have a problem if we use one variable instead of different two it restrict boy's moving

double boyx = 490, boyy = 17, walkx = 1050;//boy's starting position

char *backgroundmusic[3] = { "bgm1.mp3", "bgm2.mp3", "bgm3.mp3" };//starting menu background music
char *boysafemusic[3] = { "shish1.mp3", "shish2.mp3", "lalala.mp3" };//when boy reach into home

char car[5][10] = { "car1.bmp", "car2.bmp", "car3.bmp", "car4.bmp", "car5.bmp" };//car names
char carrev[5][10] = { "car1r.bmp", "car2r.bmp", "car3r.bmp", "car4r.bmp", "car5r.bmp" };//reverse car names

int carx1 = 0, carx2 = 400, carx3 = 700, carx4 = 200, carx5 = 500, carx6 = 800, cary1 = 50, cary2 = 118;//car position

int carx1r = 1024, carx2r = 850, carx3r = 600, carx4r = 400, carx5r = 220, carx6r = 330, cary1r = 83, cary2r = 152;//reverse car position

int fastcarx1 = -270, fastcarx2 = 350, fastcarx3 = -600, fastcary = 185;

char boat[3][10] = { "boat1.bmp", "boat2.bmp", "boat3.bmp" }, boatrev[3][10] = { "boat4.bmp", "boat5.bmp", "boat6.bmp" };

int boatx1 = 56, boatx2 = 700, boatx3 = 400, randboat[6] = { 0, 1, 2, 0, 1, 2 };//1st line boat position
int boatx4 = 0, boatx5 = 500, boatx6 = 900;//3rd line boat position
int boatx1r = 1024, boatx2r = 850, boatx3r = 600, randboatr[6] = { 0, 1, 2, 0, 1, 2 };//reverse 2nd line boat position
int boatx4r = 400, boatx5r = 220, boatx6r = 330;//reverse 4th line boat position
double boatyl1 = 251.5, boatyl2 = 285, boatyl3 = 318.5, boatyl4 = 352;
char str[15], timer[15];

//show scores in integer
void showscore()
{
	int n = 0, x = 925, y = 475;//820, 460
	if (winflag == 1)
		x = 875, y = 35;

	if (lossflag == 1)
		x = 545, y = 35;

	_itoa_s(score, str, 10);
	while (str[n] != '\0'){
		iSetColor(240, 15, 140);
		iText(x, y, str, GLUT_BITMAP_TIMES_ROMAN_24);
		str[0] = 0;
		n++;
	}
}

struct game
{
	char name[20];
	int score;
}save[5];

void savescore()
{
	FILE *fp;
	fp = fopen("score.txt", "r");
	fscanf(fp, "%s%d", save[0].name, &save[0].score);


	if (score > save[0].score)
	{
		fp = fopen("score.txt", "w");
		fputs("Depto ", fp);
		fprintf(fp, "%d", score);
		fputc('\n', fp);
	}
}

//boy walking on home screen 
void walking()
{
	int s;
	walkx-=15;

	if (walkx < -130)
		walkx = 1040;

	if (w % 4 == 0)
		for (s = 0; s < 250; s++)
			iShowBMP(walkx, 50, "bw1.bmp");
	if (w % 4 == 1)
		for (s = 0; s < 250; s++)
			iShowBMP(walkx, 50, "bw2.bmp");
	if (w % 4 == 2)
		for (s = 0; s<250; s++)
			iShowBMP(walkx, 50, "bw3.bmp");
	if (w % 4 == 3)
		for (s = 0; s<250; s++)
			iShowBMP(walkx, 50, "bw4.bmp");

	iSetColor(0, 0, 250);
	iText(420, 250, "Back to Home Safely", GLUT_BITMAP_TIMES_ROMAN_24);

	w++;
	if (w>5000)
		w = 1000;

}

//show menu buttons
void menu()
{
	iSetColor(255, 255, 255);
	iFilledRectangle(0, 0, 1024, 512);
	
	iShowBMP(80, 375, "play.bmp");
	iShowBMP(258, 375, "scores.bmp");
	iShowBMP(436, 375, "help.bmp");
	iShowBMP(614, 375, "about us.bmp");
	iShowBMP(792, 375, "exit.bmp");

}

void showwellcome()
{
	iSetColor(255, 255, 255);
	iFilledRectangle(0, 0, 1024, 512);
	
	iSetColor(0, 0, 250);
	if (second < 121)
		iText(420, 430, "Back to Home Safely", GLUT_BITMAP_TIMES_ROMAN_24);
	if (second < 120)
		iText(460, 80, "Instructed by:", GLUT_BITMAP_9_BY_15);
	if (second < 119)
		iText(415, 50, "Naheed Anjum Arafat", GLUT_BITMAP_TIMES_ROMAN_24);

	int s;
	if (second < 118)
	{
		if (w % 5 == 0)
			for (s = 0; s < 150; s++)
				iShowBMP(400, 130, "naheedsir5.bmp");
		if (w % 5 == 1)
			for (s = 0; s < 150; s++)
				iShowBMP(400, 130, "naheedsir4.bmp");
		if (w % 5 == 2)
			for (s = 0; s< 150; s++)
				iShowBMP(400, 130, "naheedsir3.bmp");
		if (w % 5 == 3)
			for (s = 0; s< 150; s++)
				iShowBMP(400, 130, "naheedsir2.bmp");
		if (w % 5 == 4)
		{
			iShowBMP(400, 130, "naheedsir.bmp");
		}
		else
			w++;
	}
	

	if (second < 112)
	{
		iSetColor(255, 255, 255);
		iFilledRectangle(0, 0, 1024, 512);
		wellcome = 0;
	}
}
void gameover()
{
	iClear();
	start = 0;
	menu();
	//savescore();

	iShowBMP(270, 80, "gameover.bmp");
	iSetColor(0, 0, 255);
	iText(100, 375, "Play Again!!", GLUT_BITMAP_HELVETICA_18);
	iShowBMP(440, 20, "score.bmp");
	showscore();

}

void boydead()
{
	life--;
	wb = 0;
	boyx = 490, boyy = 17;
	ShellExecute(NULL, "open", "ouch.mp3", 0, 0, 0);
	if (life <= 0)
	{
		ShellExecute(NULL, "open", "gameover.mp3", 0, 0, 0);
		lossflag = 1;
	}

}

//show remaining time when you are playing
void showtimer()
{
	int k = 0;
	iSetColor(0, 255, 0);
	if (second>20)
		iText(380, 230, "You have Left         seconds!!", GLUT_BITMAP_TIMES_ROMAN_24);
	else if (second < 20 && second % 2 != 0)
		iText(380, 230, "You have Left         seconds!!", GLUT_BITMAP_TIMES_ROMAN_24);
	else
	{
		iSetColor(0, 0, 255);
		iText(380, 230, "You have Left         seconds!!", GLUT_BITMAP_TIMES_ROMAN_24);
	}

	_itoa_s(second, timer, 10);
	while (timer[k] != '\0')
	{
		iSetColor(255, 0, 0);
		iText(530, 230, timer, GLUT_BITMAP_TIMES_ROMAN_24);
		timer[0] = 0;
		k++;
	}
}

//count remaining time when you are playing
void counttimer()
{
	second--;

	if (second == 0 && start == 1)
	{
		ShellExecute(NULL, "open", "gameover.mp3", 0, 0, 0);
		lossflag = 1;
	}
}

//inform you about bonus when you got it
void bonus()
{
	int s;

	iSetColor(0, 0, 0);
	iFilledRectangle(305, 465, 150, 35);
	iSetColor(0, 0, 255);

	iText(310, 475, "You got bonus!!", GLUT_BITMAP_HELVETICA_18);

}

//when a boy can reach into a box
void boysafe()
{
	score += 100;
	boatride = 0;
	wb++;//count that how many times you can take the box without losing any life

	if (wb == 10)
		wb = 1;

	if (wb == 3)
	{
		score += 300;
	}
	else if (wb == 6)
	{
		score += 600;
		life++;
	}
	else if (wb == 9)
	{
		score += 1000;
		life += 2;
	}

	if (boyy > 352 && (boyx > 25 && boyx < 76))
	{
		home[1] = 1;
		homex[1] = boyx;
		filled[1] = 1;
	}
	else if (boyy > 352 && (boyx > 135 && boyx < 192))
	{
		home[2] = 1;
		homex[2] = boyx;
		filled[2] = 1;
	}
	else if (boyy > 352 && (boyx > 248 && boyx < 298))
	{
		home[3] = 1;
		homex[3] = boyx;
		filled[3] = 1;
	}
	else if (boyy > 352 && (boyx > 359 && boyx < 408))
	{
		home[4] = 1;
		homex[4] = boyx;
		filled[4] = 1;
	}
	else if (boyy > 352 && (boyx > 465 && boyx < 520))
	{
		home[5] = 1;
		homex[5] = boyx;
		filled[5] = 1;
	}
	else if (boyy > 352 && (boyx > 575 && boyx < 630))
	{
		home[6] = 1;
		homex[6] = boyx;
		filled[6] = 1;
	}
	else if (boyy > 352 && (boyx > 685 && boyx < 740))
	{
		home[7] = 1;
		homex[7] = boyx;
		filled[7] = 1;
	}
	else if (boyy > 352 && (boyx > 790 && boyx < 860))
	{
		home[8] = 1;
		homex[8] = boyx;
		filled[8] = 1;
	}
	else if (boyy > 352 && (boyx > 900 && boyx < 960))
	{
		home[9] = 1;
		homex[9] = boyx;
		filled[9] = 1;
	}

	boyx = 490, boyy = 17;
	homecount++;
	wincount++;
	safebgm = rand() % 3;
	ShellExecute(NULL, "open", boysafemusic[safebgm % 3], 0, 0, 0);

	if (homecount == 9 || homecount == 18 || homecount == 27 || homecount == 36 || homecount == 45)
	{
		speed += 2;
		level++;
		life++;
		second += secondtag;

		for (i = 0; i < 10; i++)
		{
			home[i] = 0;
			filled[i] = 0;
		}

		if (level == 2)
			ShellExecute(NULL, "open", "level2.mp3", 0, 0, 0);
		if (level == 3)
			ShellExecute(NULL, "open", "level3.mp3", 0, 0, 0);
		if (level == 4)
			ShellExecute(NULL, "open", "level4.mp3", 0, 0, 0);
		if (level == 5)
			ShellExecute(NULL, "open", "level5.mp3", 0, 0, 0);
	}

	if (wincount == win)//check when you win
	{
		score += life * 1000;//score increase according to your remainig life
		score += second * 100;//score increase according to your remainig seconds
		ShellExecute(NULL, "open", "youwin.mp3", 0, 0, 0);
		winflag = 1;
	}

}

void startgame()
{
	iClear();
	iShowBMP(0, 0, "field.bmp");

	iShowBMP(465, 460, "menu.bmp");
	iShowBMP(600, 460, "pause.bmp");

	if (boyy < 50)
		iShowBMP(boyx, boyy, "boystart.bmp");
	else if (boyy >= 50 && boyy <= 180)
		iShowBMP(boyx, boyy, "boyroad.bmp");// when boy is in the road
	else if (boyy >= 180 && boyy <= 218)
		iShowBMP(boyx, boyy, "boywhite.bmp");// when boy is on the boat
	else if (boyy > 352)
		boysafe();// when boy is in the box/home

	if (home[1] == 1)
	{
		iShowBMP(50, 395, "boysafe.bmp");
	}
	if (home[2] == 1)
	{
		iShowBMP(160, 395, "boysafe.bmp");
	}
	if (home[3] == 1)
	{
		iShowBMP(270, 395, "boysafe.bmp");
	}
	if (home[4] == 1)
	{
		iShowBMP(380, 395, "boysafe.bmp");
	}
	if (home[5] == 1)
	{
		iShowBMP(490, 395, "boysafe.bmp");
	}
	if (home[6] == 1)
	{
		iShowBMP(600, 395, "boysafe.bmp");
	}
	if (home[7] == 1)
	{
		iShowBMP(710, 395, "boysafe.bmp");
	}
	if (home[8] == 1)
	{
		iShowBMP(820, 395, "boysafe.bmp");
	}
	if (home[9] == 1)
	{
		iShowBMP(930, 395, "boysafe.bmp");
	}

	if (wb == 6)
	{
		iSetColor(255, 0, 0);
		iText(330, 195, "Congratulations! You have got 1 Life Bonus!!!", GLUT_BITMAP_HELVETICA_18);
	}
	
	if (level > 1 && homecount % 9 == 0 && homecount != 0 && wb != 9)
	{
		iSetColor(255, 0, 0);
		iText(310, 195, "Congratulations! You have got 1 Life Bonus for level up!!!", GLUT_BITMAP_HELVETICA_18);
	}

	else if (wb == 9 && homecount % 9 != 0)
	{
		iSetColor(0, 255, 0);
		iText(330, 195, "Congratulations! You have got 2 Life Bonus!!!", GLUT_BITMAP_HELVETICA_18);
	}

	if (wb == 9 && homecount % 9 == 0)
	{
		iSetColor(0, 255, 0);
		iText(330, 195, "Congratulations! You have got 3 Life Bonus!!!", GLUT_BITMAP_HELVETICA_18);
	}


	iShowBMP(820, 460, "score.bmp");

	//showing your remainig life
	iSetColor(255, 0, 0);
	iText(5, 475, "Life Left ", GLUT_BITMAP_TIMES_ROMAN_24);
	if (life >= 7)
	{
		iShowBMP(100, 465, "life.bmp");
		iShowBMP(150, 465, "life.bmp");
		iShowBMP(200, 465, "life.bmp");
		iShowBMP(250, 465, "life.bmp");
		iShowBMP(300, 465, "life.bmp");
		iShowBMP(350, 465, "life.bmp");
		iShowBMP(400, 465, "life.bmp");
	}
	else if (life == 6)
	{
		iShowBMP(100, 465, "life.bmp");
		iShowBMP(150, 465, "life.bmp");
		iShowBMP(200, 465, "life.bmp");
		iShowBMP(250, 465, "life.bmp");
		iShowBMP(300, 465, "life.bmp");
		iShowBMP(350, 465, "life.bmp");
	}
	else if (life == 5)
	{
		iShowBMP(100, 465, "life.bmp");
		iShowBMP(150, 465, "life.bmp");
		iShowBMP(200, 465, "life.bmp");
		iShowBMP(250, 465, "life.bmp");
		iShowBMP(300, 465, "life.bmp");
	}
	else if (life == 4)
	{
		iShowBMP(100, 465, "life.bmp");
		iShowBMP(150, 465, "life.bmp");
		iShowBMP(200, 465, "life.bmp");
		iShowBMP(250, 465, "life.bmp");
	}
	else if (life == 3)
	{
		iShowBMP(100, 465, "life.bmp");
		iShowBMP(150, 465, "life.bmp");
		iShowBMP(200, 465, "life.bmp");
	}
	else if (life == 2)
	{
		iShowBMP(100, 465, "life.bmp");
		iShowBMP(150, 465, "life.bmp");
	}
	else if (life == 1)
	{
		iShowBMP(100, 465, "life.bmp");
		
		if (second % 2 == 0)
			iText(385, 195, "Alert!!! You have only 1 life left!!!", GLUT_BITMAP_HELVETICA_18);//alerting you about last life
		
	}

	if (wb == 3 || wb == 6 || wb == 9)
		bonus();

	//showing car
	iShowBMP(carx1, cary1, car[randcar[0]]);
	iShowBMP(carx2, cary1, car[randcar[1]]);
	iShowBMP(carx3, cary1, car[randcar[2]]);
	iShowBMP(carx4, cary2, car[randcar[3]]);
	iShowBMP(carx5, cary2, car[randcar[4]]);
	iShowBMP(carx6, cary2, car[randcar[4]]);

	//showing car reverse
	iShowBMP(carx1r, cary1r, carrev[randcarr[0]]);
	iShowBMP(carx2r, cary2r, carrev[randcarr[1]]);
	iShowBMP(carx3r, cary1r, carrev[randcarr[2]]);
	iShowBMP(carx4r, cary2r, carrev[randcarr[3]]);
	iShowBMP(carx5r, cary1r, carrev[randcarr[4]]);
	iShowBMP(carx6r, cary2r, carrev[randcarr[4]]);
	
	//showing fast car
	iShowBMP(fastcarx1, fastcary, "car6.bmp");
	iShowBMP(fastcarx2, fastcary, "car7.bmp");
	iShowBMP(fastcarx3, fastcary, "car8.bmp");

	//showing boat
	iShowBMP(boatx1, boatyl1, boat[0]);
	iShowBMP(boatx2, boatyl1, boat[1]);
	iShowBMP(boatx3, boatyl1, boat[2]);
	iShowBMP(boatx4, boatyl3, boat[0]);
	iShowBMP(boatx5, boatyl3, boat[1]);
	iShowBMP(boatx6, boatyl3, boat[2]);

	//showing boat reverse
	iShowBMP(boatx1r, boatyl2, boatrev[0]);
	iShowBMP(boatx2r, boatyl4, boatrev[1]);
	iShowBMP(boatx3r, boatyl2, boatrev[2]);
	iShowBMP(boatx4r, boatyl4, boatrev[0]);
	iShowBMP(boatx5r, boatyl2, boatrev[1]);
	iShowBMP(boatx6r, boatyl4, boatrev[2]);

	showscore();

	if (boatride == 1)
		iShowBMP(boyx, boyy, "boywhite.bmp");

}

//run all cars
void carmove()
{
	//according to +x axis
	carx1 += speed;
	carx2 += speed;
	carx3 += speed;
	carx4 += speed;
	carx5 += speed;
	carx6 += speed;
	if (carx1 == 1100)
	{
		srand((unsigned)time(NULL));
		randcar[0] = rand() % 5;
		carx1 = -64;
	}
	else if (carx2 > 1600)
	{
		srand((unsigned)time(NULL));
		randcar[1] = rand() % 5;
		carx2 = -65;
	}
	else if (carx3 > 1300)
	{
		srand((unsigned)time(NULL));
		randcar[2] = rand() % 5;
		carx3 = -65;
	}
	else if (carx4 > 1200)
	{
		srand((unsigned)time(NULL));
		randcar[3] = rand() % 5;
		carx4 = -65;
	}
	else if (carx5 > 1500)
	{
		srand((unsigned)time(NULL));
		randcar[4] = rand() % 5;
		carx5 = -65;
	}
	else if (carx5 > 1450)
	{
		srand((unsigned)time(NULL));
		randcar[4] = rand() % 5;
		carx6 = -65;
	}

	//according to -x axis
	carx1r -= speed + 3;
	carx2r -= speed + 3;
	carx3r -= speed + 3;
	carx4r -= speed + 3;
	carx5r -= speed + 3;
	carx6r -= speed + 3;
	if (carx1r < -200)
	{
		srand((unsigned)time(NULL));
		randcarr[0] = rand() % 5;
		carx1r = 1055;
	}
	else if (carx2r < -450)
	{
		srand((unsigned)time(NULL));
		randcarr[1] = rand() % 5;
		carx2r = 1055;
	}
	else if (carx3r < -500)
	{
		srand((unsigned)time(NULL));
		randcarr[2] = rand() % 5;
		carx3r = 1055;
	}
	else if (carx4r < -250)
	{
		srand((unsigned)time(NULL));
		randcarr[3] = rand() % 5;
		carx4r = 1055;
	}
	else if (carx5r < -350)
	{
		srand((unsigned)time(NULL));
		randcarr[4] = rand() % 5;
		carx5r = 1055;
	}
	else if (carx6r < -150)
	{
		srand((unsigned)time(NULL));
		randcarr[4] = rand() % 5;
		carx5r = 1055;
	}

	if (carx1 + 64 >= boyx && carx1 - 28 <= boyx && cary1 >= boyy - 32 && cary1 <= boyy + 32 || carx2 + 64 >= boyx && carx2 - 28 <= boyx && cary1 >= boyy - 32 && cary1 <= boyy + 32 || carx3 + 64 >= boyx && carx3 - 28 <= boyx && cary1 >= boyy - 32 && cary1 <= boyy + 32 || carx4 + 64 >= boyx && carx4 - 28 <= boyx && cary2 >= boyy - 32 && cary2 <= boyy + 32 || carx5 + 64 >= boyx && carx5 - 28 <= boyx && cary2 >= boyy - 32 && cary2 <= boyy + 32 || carx6 + 64 >= boyx && carx6 - 28 <= boyx && cary2 >= boyy - 32 && cary2 <= boyy + 32)
	{
		boydead();//boy will die if he get touch of car
	}

	if (carx1r + 64 >= boyx && carx1r - 28 <= boyx && cary1r >= boyy - 32 && cary1r <= boyy + 32 || carx2r + 64 >= boyx && carx2r - 28 <= boyx && cary2r >= boyy - 32 && cary2r <= boyy + 32 || carx3r + 64 >= boyx && carx3r - 28 <= boyx && cary1r >= boyy - 32 && cary1r <= boyy + 32 || carx4r + 64 >= boyx && carx4r - 28 <= boyx && cary2r >= boyy - 32 && cary2r <= boyy + 32 || carx5r + 64 >= boyx && carx5r - 28 <= boyx && cary1r >= boyy - 32 && cary1r <= boyy + 32 || carx6r + 64 >= boyx && carx6r - 28 <= boyx && cary2r >= boyy - 32 && cary2r <= boyy + 32)
	{
		boydead();//boy will die if he get touch of car reverse
	}

	//fast car
	fastcarx1 += speed + 5;
	fastcarx2 += speed + 5;
	fastcarx3 += speed + 5;

	if (fastcarx1 >1250)
	{
		fastcarx1 = 0;
	}
	else if (fastcarx2 > 1530)
	{
		fastcarx2 = 0;
	}
	else if (fastcarx3 > 1770)
	{
		fastcarx3 = 0;
	}

	if (fastcarx1 + 128 >= boyx && fastcarx1 - 56 <= boyx && fastcary >= boyy - 32 && fastcary <= boyy + 32 || fastcarx2 + 128 >= boyx && fastcarx2 - 56 <= boyx && fastcary >= boyy - 32 && fastcary <= boyy + 32 || fastcarx3 + 128 >= boyx && fastcarx3 - 56 <= boyx && fastcary >= boyy - 32 && fastcary <= boyy + 32)
	{
		boydead();//boy will die if he get touch of fastcar
	}
}

void boatmove()
{
	boatx1 += speed;
	boatx2 += speed;
	boatx3 += speed;
	boatx4 += speed;
	boatx5 += speed;
	boatx6 += speed;

	if (boatx1 > 1150)
	{		boatx1 = 0;
	}
	else if (boatx2 > 1200)
	{
		boatx2 = 0;
	}
	else if (boatx3 > 1550)
	{
		boatx3 = 0;
	}
	else if (boatx4 > 1450)
	{
		boatx4 = -65;
	}
	else if (boatx5 > 1400)
	{
		boatx5 = -65;
	}
	else if (boatx6 > 1300)
	{
		boatx6 = -65;
	}

	boatx1r -= speed + 1;
	boatx2r -= speed + 1;
	boatx3r -= speed + 1;
	boatx4r -= speed + 1;
	boatx5r -= speed + 1;
	boatx6r -= speed + 1;

	if (boatx1r < -200)
	{
		boatx1r = 1055;
	}
	else if (boatx2r < -450)
	{
		boatx2r = 1055;
	}
	else if (boatx3r < -500)
	{
		boatx3r = 1055;
	}
	else if (boatx4r < -250)
	{
		boatx4r = 1055;
	}
	else if (boatx5r < -350)
	{
		boatx5r = 1055;
	}
	else if (boatx6r < -150)
	{
		boatx6r = 1055;
	}

	if (boatx1 + 256 >= boyx && boatx1 - 28 <= boyx && boatyl1 >= boyy - 32 && boatyl1 <= boyy + 32 || boatx2 + 256 >= boyx && boatx2 - 28 <= boyx && boatyl1 >= boyy - 32 && boatyl1 <= boyy + 32 || boatx3 + 256 >= boyx && boatx3 - 28 <= boyx && boatyl1 >= boyy - 32 && boatyl1 <= boyy + 32 || boatx4 + 256 >= boyx && boatx4 - 28 <= boyx && boatyl3 >= boyy - 32 && boatyl3 <= boyy + 32 || boatx5 + 256 >= boyx && boatx5 - 28 <= boyx && boatyl3 >= boyy - 32 && boatyl3 <= boyy + 32 || boatx6 + 256 >= boyx && boatx6 - 28 <= boyx && boatyl3 >= boyy - 32 && boatyl3 <= boyy + 32)
	{
		boatride = 1;
		boyx += speed;//if boy ride on boat, boy will move as boat speeed
	}

	else if (boatx1r + 256 >= boyx && boatx1r - 28 <= boyx && boatyl2 >= boyy - 32 && boatyl2 <= boyy + 32 || boatx2r + 256 >= boyx && boatx2r - 28 <= boyx && boatyl4 >= boyy - 32 && boatyl4 <= boyy + 32 || boatx3r + 256 >= boyx && boatx3r - 28 <= boyx && boatyl2 >= boyy - 32 && boatyl2 <= boyy + 32 || boatx4r + 256 >= boyx && boatx4r - 28 <= boyx && boatyl4 >= boyy - 32 && boatyl4 <= boyy + 32 || boatx5r + 256 >= boyx && boatx5r - 28 <= boyx && boatyl2 >= boyy - 32 && boatyl2 <= boyy + 32 || boatx6r + 256 >= boyx && boatx6r - 28 <= boyx && boatyl4 >= boyy - 32 && boatyl4 <= boyy + 32)
	{
		boatride = 1;
		boyx -= speed + 1;//if boy ride on boat, boy will move as boat speeed
	}

	else if (boyy > 251 && boyy<353)
	{
		boatride = 0;
		boydead();
	}

	if (boyx < 0 || boyx>1015)
	{
		boatride = 0;
		boydead();
	}

}

void wining()
{
	int s;
	start = 0;
	if (w % 4 == 0)
		for (s = 0; s < 25; s++)
			iShowBMP(256, 0, "hi1.bmp");
	if (w % 4 == 1)
		for (s = 0; s < 25; s++)
			iShowBMP(256, 0, "hi2.bmp");
	if (w % 4 == 2)
		for (s = 0; s<25; s++)
			iShowBMP(256, 0, "hi3.bmp");
	if (w % 4 == 3)
		for (s = 0; s<25; s++)
			iShowBMP(256, 0, "hi4.bmp");

	w++;
	if (w>5000)
		w = 1000;

	iShowBMP(80, 375, "play.bmp");
	iSetColor(0, 0, 255);
	iText(100, 375, "Play Again!!", GLUT_BITMAP_HELVETICA_18);
	iShowBMP(792, 375, "exit.bmp");
	iShowBMP(80, 20, "home.bmp");
	iShowBMP(770, 20, "score.bmp");
	showscore();
	savescore();
}

void scoremenu()
{
	menu();
	int n = 0, c = 1;
	char ch;
	char s1[100], s2[100], s3[100], s4[100], s5[100];
	FILE*fp;
	fp = fopen("score.txt", "r");

	for (;;)
	{
		ch = fgetc(fp);
		if (ch == EOF)
			break;
		
		s1[n++] = ch;
	}


	n = 0;
	iSetColor(0, 0, 0);
	while (s1[n] != '\0')
	{
		iText(450, 260, s1, GLUT_BITMAP_TIMES_ROMAN_24);
		s1[0] = 0;
		n++;
	}
	
}

void help()
{
	menu();
	iShowBMP(0, 0, "how to play.bmp");

	iShowBMP(80, 375, "play.bmp");
	iShowBMP(258, 375, "scores.bmp");
	iShowBMP(436, 375, "help.bmp");
	iShowBMP(614, 375, "about us.bmp");
	iShowBMP(792, 375, "exit.bmp");

}

void aboutus()
{
	iClear();
	iSetColor(255, 255, 255);
	iShowBMP(0, 0, "aboutus.bmp");

	iShowBMP(80, 375, "play.bmp");
	iShowBMP(258, 375, "scores.bmp");
	iShowBMP(436, 375, "help.bmp");
	iShowBMP(614, 375, "about us.bmp");
	iShowBMP(792, 375, "exit.bmp");

}

void iDraw()
{
	menu();

	if (wellcome == 1)
		showwellcome();

	if (start == 0 && walk == 1 && wellcome == 0)
		walking();

	if (start == 2)
	{
		menu();
		start = 0;
		walk = 1;
	}

	if (start == 1)
	{
		walk = 0;
		startgame();
		showtimer();
	}

	if (scoreflag == 1 && start == 0)
		scoremenu();

	if (helpflag == 1 && start == 0)
		help();

	if (aboutusflag == 1 && start == 0)
		aboutus();

	if (winflag == 1)
	{
		iClear();
		iSetColor(255, 255, 255);
		iFilledRectangle(0, 0, 1024, 512);
		iSetColor(0, 255, 255);

		wining();
	}

	else if (lossflag == 1)
	{
		gameover();
	}

	if (scoreflag == 1 && start == 0)
		iShowBMP(258, 375, "scores press.bmp");

	if (helpflag == 1 && start == 0)
		iShowBMP(436, 375, "help press.bmp");

	if (aboutusflag == 1 && start == 0)
		iShowBMP(614, 375, "about us press.bmp");

	if (pauseflag == 1)
	{
		iShowBMP(448, 192, "resume.bmp");
	}

	if (start == 1)
	{
		iSetColor(0, 0, 0);
		iFilledRectangle(730, 470, 80, 35);
	}

	if (level == 1 && start != 0)
	{
		iSetColor(250, 250, 250);
		iText(740, 480, "Level 1", GLUT_BITMAP_HELVETICA_18);
	}
	else if (level == 2 && start != 0)
	{
		iSetColor(250, 250, 250);
		iText(740, 480, "Level 2", GLUT_BITMAP_HELVETICA_18);
	}
	else if (level == 3 && start != 0)
	{
		iSetColor(250, 250, 250);
		iText(740, 480, "Level 3", GLUT_BITMAP_HELVETICA_18);
	}
	else if (level == 4 && start != 0)
	{
		iSetColor(250, 250, 250);
		iText(740, 480, "Level 4", GLUT_BITMAP_HELVETICA_18);
	}
	else if (level == 5 && start != 0)
	{
		iSetColor(250, 250, 250);
		iText(740, 480, "Level 5", GLUT_BITMAP_HELVETICA_18);
	}

}

/*
function iMouseMove() is called when the user presses and drags the mouse.
(mx, my) is the position where the mouse pointer is.
*/

void iMouseMove(int mx, int my)
{
	//place your codes here
}

/*
function iMouse() is called when the user presses/releases the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		printf("mx=%d	my=%d\n", mx, my);
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && start == 1 && mx > 465 && mx < 465 + 128 && my>470 && my < 470 + 64)
	{
		start = 2, pauseflag = 0, walk = 1;
		menu();
	}
	
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && winflag == 1 && mx > 80 && mx < 80 + 128 && my>20 && my < 20 + 128)
	{
		start = 2, pauseflag = 0, winflag = 0, walk = 1;
		menu();
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && start == 1 && mx > 600 && mx < 600 + 128 && my>470 && my < 470 + 64)
	{
		pauseflag = 1;
		iPauseTimer(cartimer);
		iPauseTimer(boattimer);
		iPauseTimer(timetimer);
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && pauseflag == 1 && mx > 448 && mx < 448 + 128 && my>192 && my < 192 + 128)
	{
		pauseflag = 0;
		iResumeTimer(cartimer);
		iResumeTimer(boattimer);
		iResumeTimer(timetimer);
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx > 0 && mx < 64 && my>465 && my < 465 + 32)
	{
		life++;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && start == 0 && mx > 80 && mx < 128 + 80 && my>375 && my < 375 + 128)
	{
		for (i = 0; i < 10; i++)
		{
			home[i] = 0;
			filled[i] = 0;
		}

		helpflag = 0, scoreflag = 0, aboutusflag = 0;
		ShellExecute(NULL, "open", "letsstart.mp3", 0, 0, 0);
		start = 1, boyx = 490, boyy = 17, second = secondtag, score = 0, life = lifetag, win = wintag, homecount = 0, wincount = 0, scoreflag = 0, helpflag = 0, aboutusflag = 0, level = 1, speed = speedtag, winflag = 0, lossflag = 0, pauseflag = 0, wb = 0, boatride = 0, walk = 0;
		counttimer();
		iResumeTimer(cartimer);
		iResumeTimer(boattimer);
		iResumeTimer(timetimer);
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && start == 0 && mx > 258 && mx < 128 + 258 && my>375 && my < 375 + 128)
	{
		helpflag = 0, aboutusflag = 0, winflag = 0, lossflag = 0;
		scoreflag = 1;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && start == 0 && mx > 436 && mx < 128 + 436 && my>375 && my < 375 + 128)
	{
		scoreflag = 0, aboutusflag = 0, winflag = 0, lossflag = 0;
		helpflag = 1;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && start == 0 && mx > 614 && mx < 128 + 614 && my>375 && my < 375 + 128)
	{
		helpflag = 0, scoreflag = 0, winflag = 0, lossflag = 0;
		aboutusflag = 1;
	}

	if (button == GLUT_LEFT_BUTTON && start == 0 && state == GLUT_DOWN && mx > 792 && mx < 128 + 792 && my>375 && my < 375 + 128)
	{
		exit(0);
	}

}

/*
function iKeyboard() is called whenever the user hits a key in keyboard.
key- holds the ASCII value of the key pressed.
*/

void iKeyboard(unsigned char key)
{
	if (key == 'q' || key == 'Q')
	{
		exit(0);
	}

	if (key == 'd' || key == 'D')
	{
		for (i = 2; i < 10; i++)
		{
			home[i] = 1;
			filled[i] = 1;
		}
		wincount += 8;
		homecount += 8;
		wb += 8;
		score += 1950;
	}

	if (key == ' ' && pauseflag == 0)
	{
		pauseflag = 1;
		iPauseTimer(cartimer);
		iPauseTimer(boattimer);
		iPauseTimer(timetimer);
	}


	else if (key == ' ' && pauseflag == 1)
	{
		pauseflag = 0;
		iResumeTimer(cartimer);
		iResumeTimer(boattimer);
		iResumeTimer(timetimer);

	}

}

/*
function iSpecialKeyboard() is called whenver user hits special keys like-
function keys, home, end, pg up, pg down, arraows etc. you have to use
appropriate constants to detect them. A list is:
GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
*/

void iSpecialKeyboard(unsigned char key)
{

	if (key == GLUT_KEY_LEFT && boyx > 100 && start == 1 &&  pauseflag == 0)
	{
		boyx -= 55;
		ShellExecute(NULL, "open", "walk.mp3", 0, 0, 0);
	}
	if (key == GLUT_KEY_RIGHT && boyx<950 && start == 1 && pauseflag == 0)
	{
		boyx += 55;
		ShellExecute(NULL, "open", "walk.mp3", 0, 0, 0);
	}
	if (key == GLUT_KEY_UP && boyy < 500 && start == 1 && pauseflag == 0)
	{
		boyy += 33.5;
		ShellExecute(NULL, "open", "walk.mp3", 0, 0, 0);
	}
	if (key == GLUT_KEY_DOWN && boyy > 32 && start == 1 && pauseflag == 0)
	{
		boyy -= 33.5;
		ShellExecute(NULL, "open", "walk.mp3", 0, 0, 0);
	}


	if (boyy > 352 && filled[1] == 1 && (boyx > 40 && boyx < 86) || (boyy > 352 && filled[2] == 1 && (boyx > 150 && boyx < 202)) || (boyy > 352 && filled[3] == 1 && (boyx > 263 && boyx < 308)) || (boyy > 352 && filled[4] == 1 && (boyx > 372 && boyx < 418)) || (boyy > 352 && filled[5] == 1 && (boyx > 480 && boyx < 530)) || (boyy > 352 && filled[6] == 1 && (boyx > 590 && boyx < 640)) || (boyy > 352 && filled[7] == 1 && (boyx > 700 && boyx < 750)) || (boyy > 352 && filled[8] == 1 && (boyx > 810 && boyx < 860)) || (boyy > 352 && filled[9] == 1 && (boyx > 920 && boyx > 970)))
		boydead();
	if ((boyy > 352 && (boyx > 970)) || (boyy > 352 && (boyx > 0 && boyx < 40)) || (boyy > 352 && (boyx > 86 && boyx < 150)) || (boyy > 352 && (boyx > 202 && boyx < 263)) || (boyy > 352 && (boyx > 308 && boyx < 372)) || (boyy > 352 && (boyx > 418 && boyx < 480)) || (boyy > 352 && (boyx > 530 && boyx < 590)) || (boyy > 352 && (boyx > 640 && boyx < 700)) || (boyy > 352 && (boyx > 750 && boyx < 810)) || (boyy > 352 && (boyx > 860 && boyx < 920)))
		boydead();

	if (carx1 + 64 >= boyx && carx1 - 28 <= boyx && cary1 >= boyy - 32 && cary1 <= boyy + 32 || carx2 + 64 >= boyx && carx2 - 28 <= boyx && cary1 >= boyy - 32 && cary1 <= boyy + 32 || carx3 + 64 >= boyx && carx3 - 28 <= boyx && cary1 >= boyy - 32 && cary1 <= boyy + 32 || carx4 + 64 >= boyx && carx4 - 28 <= boyx && cary2 >= boyy - 32 && cary2 <= boyy + 32 || carx5 + 64 >= boyx && carx5 - 28 <= boyx && cary2 >= boyy - 32 && cary2 <= boyy + 32 || carx6 + 64 >= boyx && carx6 - 28 <= boyx && cary2 >= boyy - 32 && cary2 <= boyy + 32)
	{
		boydead();
	}

	if (carx1r + 64 >= boyx && carx1r - 28 <= boyx && cary1r >= boyy - 32 && cary1r <= boyy + 32 || carx2r + 64 >= boyx && carx2r - 28 <= boyx && cary2r >= boyy - 32 && cary2r <= boyy + 32 || carx3r + 64 >= boyx && carx3r - 28 <= boyx && cary1r >= boyy - 32 && cary1r <= boyy + 32 || carx4r + 64 >= boyx && carx4r - 28 <= boyx && cary2r >= boyy - 32 && cary2r <= boyy + 32 || carx5r + 64 >= boyx && carx5r - 28 <= boyx && cary1r >= boyy - 32 && cary1r <= boyy + 32 || carx6r + 64 >= boyx && carx6r - 28 <= boyx && cary2r >= boyy - 32 && cary2r <= boyy + 32)
	{
		boydead();
	}
	if (fastcarx1 + 128 >= boyx && fastcarx1 - 56 <= boyx && fastcary >= boyy - 32 && fastcary <= boyy + 32 || fastcarx2 + 128 >= boyx && fastcarx2 - 56 <= boyx && fastcary >= boyy - 32 && fastcary <= boyy + 32 || fastcarx3 + 128 >= boyx && fastcarx3 - 56 <= boyx && fastcary >= boyy - 32 && fastcary <= boyy + 32)
	{
		boydead();
	}

	if (boatx1 + 256 >= boyx && boatx1 - 28 <= boyx && boatyl1 >= boyy - 32 && boatyl1 <= boyy + 32 || boatx2 + 256 >= boyx && boatx2 - 28 <= boyx && boatyl1 >= boyy - 32 && boatyl1 <= boyy + 32 || boatx3 + 256 >= boyx && boatx3 - 28 <= boyx && boatyl1 >= boyy - 32 && boatyl1 <= boyy + 32 || boatx4 + 256 >= boyx && boatx4 - 28 <= boyx && boatyl3 >= boyy - 32 && boatyl3 <= boyy + 32 || boatx5 + 256 >= boyx && boatx5 - 28 <= boyx && boatyl3 >= boyy - 32 && boatyl3 <= boyy + 32 || boatx6 + 256 >= boyx && boatx6 - 28 <= boyx && boatyl3 >= boyy - 32 && boatyl3 <= boyy + 32)
	{
		boatride = 1;
		boyx += speed;
	}

	else if (boatx1r + 256 >= boyx && boatx1r - 28 <= boyx && boatyl2 >= boyy - 32 && boatyl2 <= boyy + 32 || boatx2r + 256 >= boyx && boatx2r - 28 <= boyx && boatyl4 >= boyy - 32 && boatyl4 <= boyy + 32 || boatx3r + 256 >= boyx && boatx3r - 28 <= boyx && boatyl2 >= boyy - 32 && boatyl2 <= boyy + 32 || boatx4r + 256 >= boyx && boatx4r - 28 <= boyx && boatyl4 >= boyy - 32 && boatyl4 <= boyy + 32 || boatx5r + 256 >= boyx && boatx5r - 28 <= boyx && boatyl2 >= boyy - 32 && boatyl2 <= boyy + 32 || boatx6r + 256 >= boyx && boatx6r - 28 <= boyx && boatyl4 >= boyy - 32 && boatyl4 <= boyy + 32)
	{
		boatride = 1;
		boyx -= speed + 1;
	}

	else if (boyy > 251 && boyy<353)
	{
		boydead();
	}

	if (boyy < 251)
	{
		boatride = 0;
	}

}

int main()
{
	srand((unsigned)time(NULL));
	randbgm = rand() % 3;
	ShellExecute(NULL, "open", backgroundmusic[randbgm % 3], 0, 0, 0); //initializes background music.

	cartimer = iSetTimer(0, carmove);
	boattimer = iSetTimer(40, boatmove);
	timetimer = iSetTimer(1000, counttimer);

	iInitialize(1024, 512, "Back to Home Safely");

	return 0;
}