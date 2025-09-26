#define _CRT_SECURE_NO_WARNINGS
#include "iGraphics.h"
#include <math.h>
#include <stdlib.h>
#include <Windows.h>
int screenWidth = 1300;
int screenHeight = 700;
int gameState = 0;
int startWidth = 200, startHeight = 40;
int aboutWidth = 200, aboutHeight = 40;
int exitWidth = 200, exitHeight = 40;
int startX = screenWidth / 2 - startWidth / 2;
int startY = screenHeight / 2 ;
float sharkX = 200, sharkY = 300; 

int aboutX = screenWidth / 2 - aboutWidth / 2;
int aboutY = screenHeight / 2;

int exitX = screenWidth / 2 - exitWidth / 2;
int exitY = screenHeight / 2 - 100;

bool hoverStart = false;
bool hoverExit = false;
bool hoverAbout = false;
int bgImage, sharkImage, ugImage, fishImage,bombImage;
int enemyImage[5];
int benemyImage[3];
int sharkWidth = 120, sharkHeight = 80;
int fishWidth = 80, fishHeight = 40;

float camX = 0.0f, camY = 0.0f;
int score = 0;
int joyCenterX, joyCenterY;
int joyRadius = 50;
bool joyActive = false;
float joyAngle = 0;

int sharkMaxHealth = 100;
int sharkHealth = 100;

const int NUM_FISH = 30;


float fishX[NUM_FISH];
float fishY[NUM_FISH];
float fishSpeedX[NUM_FISH];
float fishSpeedY[NUM_FISH];
const int NUM_ENEMIES = 5;

int enemyWidth = 100, enemyHeight = 60;
float enemyX[NUM_ENEMIES];
float enemyY[NUM_ENEMIES];
float enemySpeed[NUM_ENEMIES];
const int BIG_fish = 3;
int bWidth = 220, bHeight = 160;
float benemyX[BIG_fish];
float benemyY[BIG_fish];
float benemySpeed[BIG_fish];
bool sharkPr = true;
bool sharkPup = true;
bool  bombActive = false;
float bombX = 0.0f, bombY = 0.0f;
float bombSpeedY = 4.0f;
int   bombWidth = 90, bombHeight = 70;
int   secondsCounter = 0;
const int bombIntervalSec = 10;
const int NUM_BUBBLES = 30;
float bubbleX[NUM_BUBBLES];
float bubbleY[NUM_BUBBLES];
int bubbleDir[NUM_BUBBLES]; // 0=down,1=up
int highScore = 0;
bool sharkup = true;

void drawMenu() {
iShowImage(0, 0, screenWidth, screenHeight, bgImage);
	
	// Start button
	if (hoverStart) iSetColor(0, 100, 255);
	else iSetColor(0, 0, 255);
	iFilledRectangle(startX, startY, startWidth, startHeight);
	iSetColor(0, 0, 0);
	iText(startX + 70, startY + 12, "Start", GLUT_BITMAP_HELVETICA_18);

	
	if (hoverAbout) iSetColor(0, 100,255);
	else iSetColor(0, 0, 255);
	iFilledRectangle(aboutX, aboutY, aboutWidth, aboutHeight);
	iSetColor(0, 0, 0);
	iText(aboutX + 70, aboutY + 12, "About", GLUT_BITMAP_HELVETICA_18);

	if (hoverExit) iSetColor(255, 100, 100);
	else iSetColor(255, 0, 0);
	iFilledRectangle(exitX, exitY, exitWidth, exitHeight);
	iSetColor(0, 0, 0);
	iText(exitX + 90, exitY + 12, "Exit", GLUT_BITMAP_HELVETICA_18);
	for (int i = 0; i < NUM_BUBBLES; i++) {
		iSetColor(200, 200, 255);
		iFilledCircle(bubbleX[i], bubbleY[i], 8);
	}
}
void drawAbout() {
	iClear();
	// Black background
	iSetColor(0, 0, 0);
	iFilledRectangle(0, 0, screenWidth, screenHeight);

	iSetColor(255, 255, 255);
	iText(620, 420, "HUNGRY SHARK GAME v1 !", GLUT_BITMAP_HELVETICA_18);
	iText(630, 380, "(About This Game)", GLUT_BITMAP_TIMES_ROMAN_24);
	iText(500, 350, "You are a hungry shark. Avoid bombs.Eat fish to grow!", GLUT_BITMAP_HELVETICA_18);
	iText(520, 320, "GAME Is Developed By : NAZIFA & SHAHARIAR ", GLUT_BITMAP_HELVETICA_18);
	iText(690, 290, " ID : 00724105101061 ", GLUT_BITMAP_HELVETICA_18);
	iText(733, 270, "00724105101065 ", GLUT_BITMAP_HELVETICA_18);


	iSetColor(255, 0, 0);
	iText(630, 200, "Press M to return to Main Menu", GLUT_BITMAP_HELVETICA_18);
}
void drawJoystick() {
	iSetColor(200, 200, 200);
	iCircle(joyCenterX, joyCenterY, joyRadius);

	double x[3], y[3];
	x[0] = joyCenterX; x[1] = joyCenterX - 5; x[2] = joyCenterX + 5;
	y[0] = joyCenterY + 25; y[1] = joyCenterY + 20; y[2] = joyCenterY + 20;
	iFilledPolygon(x, y, 3);
	y[0] = joyCenterY - 25; y[1] = joyCenterY - 20; y[2] = joyCenterY - 20;
	iFilledPolygon(x, y, 3);
	x[0] = joyCenterX - 25; x[1] = joyCenterX - 20; x[2] = joyCenterX - 20;
	y[0] = joyCenterY; y[1] = joyCenterY + 5; y[2] = joyCenterY - 5;
	iFilledPolygon(x, y, 3);
	x[0] = joyCenterX + 25; x[1] = joyCenterX + 20; x[2] = joyCenterX + 20;
	iFilledPolygon(x, y, 3);

	if (joyActive) {
		int fx = joyCenterX + cos(joyAngle) * (joyRadius - 10);
		int fy = joyCenterY + sin(joyAngle) * (joyRadius - 10);
		iSetColor(255, 165, 0);
		iFilledCircle(fx, fy, 8);
	}
}
float scale = (float)screenHeight / 700; // 700 / 600
int scaledWidth = (int)(1300 * scale);   // ≈ 1166
int scaledHeight = screenHeight;         // 700
int offsetX = (screenWidth - scaledWidth) / 2; // ≈ 6
void drawGame() {
iShowImage(-camX, -camY, screenWidth * 8, screenHeight * 4, ugImage);
	//iShowImage(offsetX, 0, scaledWidth, scaledHeight, ugImage);
	// Camera centers on shark
	camX = sharkX - screenWidth / 2 + sharkWidth / 2;
	camY = sharkY - screenHeight / 2 + sharkHeight / 2;

	
	if (camX < 0) camX = 0;
	if (camY < 0) camY = 0;
	float maxCamX = screenWidth * 8 - screenWidth;
	float maxCamY = screenHeight * 4 -screenHeight;
	if (camX > maxCamX) camX = maxCamX;
	if (camY > maxCamY) camY = maxCamY;

	if (sharkPr)
		iShowImage(sharkX - camX, sharkY - camY, sharkWidth, sharkHeight, sharkImage);
	else
		iShowImage(sharkX - camX + sharkWidth, sharkY - camY, -sharkWidth, sharkHeight, sharkImage);

	//if (sharkup)
	//iShowImage(sharkX - camX, sharkY - camY - sharkHeight/4, sharkWidth, sharkHeight, sharkImage);

	// Fish
	for (int i = 0; i < NUM_FISH; i++)
		iShowImage(fishX[i] - camX, fishY[i] - camY, fishWidth, fishHeight, fishImage);

	// Enemies
	for (int i = 0; i < NUM_ENEMIES; i++)
		iShowImage(enemyX[i] - camX, enemyY[i] - camY, enemyWidth, enemyHeight, enemyImage[i]);
	for (int i = 0; i < BIG_fish; i++) {
		iShowImage(benemyX[i] - camX, benemyY[i] - camY, bWidth, bHeight, benemyImage[i]);
		for (int i = 0; i < NUM_BUBBLES; i++) {
			iSetColor(200, 200, 255); // light blue
			iFilledCircle(bubbleX[i] - camX, bubbleY[i] - camY, 8);
		}


	}

	// Joystick
	drawJoystick();

	// Health bar
	int barX = 20, barY = screenHeight - 40, barWidth = 200, barHeight = 20;
	float ratio = (float)sharkHealth / sharkMaxHealth;
	iSetColor(255, 0, 0);
	iFilledRectangle(barX, barY, barWidth, barHeight);
	iSetColor(0, 255, 0);
	iFilledRectangle(barX, barY, (int)(barWidth * ratio), barHeight);
	iSetColor(0, 0, 0);
	iRectangle(barX, barY, barWidth, barHeight);
	char str[20];
	sprintf(str, "Score: %d", score);
	iSetColor(255, 255, 255);  
	iText(20, 600, str, GLUT_BITMAP_HELVETICA_18);
	if (bombActive) {
		iShowImage((int)(bombX - camX), (int)(bombY - camY), bombWidth, bombHeight, bombImage);
	}
	char hs[50];
	sprintf(hs, "High Score: %d", highScore);
	iText(20, 570, hs, GLUT_BITMAP_HELVETICA_18);

}


void drawGameOver() {
	iClear();
	iSetColor(255, 0, 0);
	iText(screenWidth / 2 , screenHeight / 2, " GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);
	iText(screenWidth / 2 , screenHeight / 2 - 50, "Press R to Restart", GLUT_BITMAP_HELVETICA_18);
	for (int i = 0; i < NUM_BUBBLES; i++) {
		iSetColor(200, 200, 255);
		iFilledCircle(bubbleX[i], bubbleY[i], 8);
	}
}

void iDraw() {
	iClear();
	if (gameState == 0) drawMenu();
	else if (gameState == 1) drawGame();
	else if (gameState == 2) drawGameOver();
	else if (gameState == 3) drawAbout();
}

void iMouseMove(int mx, int my) {
	if (gameState == 1) {
		int dx = mx - joyCenterX;
		int dy = my - joyCenterY;
		float dist = sqrt((float)(dx * dx + dy * dy));
		joyActive = dist < joyRadius;
		if (joyActive) joyAngle = atan2((double)dy, (double)dx);
	}
}

void iPassiveMouseMove(int mx, int my) {
	if (gameState == 0) {
		hoverStart = (mx >= startX && mx <= startX + startWidth && my >= startY && my <= startY + startHeight);
		hoverAbout = (mx >= aboutX && mx <= aboutX + aboutWidth && my >= aboutY && my <= aboutY + startHeight);
		hoverExit = (mx >= exitX && mx <= exitX + exitWidth && my >= exitY && my <= exitY + exitHeight);
	}
	iMouseMove(mx, my);
}

void iMouse(int button, int state, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && gameState == 0) {
		if (mx >= startX && mx <= startX + startWidth && my >= startY && my <= startY + startHeight) {
			gameState = 1;
			PlaySound("song.wav", NULL, SND_ASYNC |SND_LOOP); 
		}
		//if (mx >= aboutX && mx <= aboutX + aboutWidth && my >= aboutY && my <= aboutY + aboutHeight)
		//	;
		if (mx >= aboutX && mx <= aboutX + aboutWidth && my >= aboutY && my <= aboutY + aboutHeight) {
			gameState = 3;
		}
		if (mx >= exitX && mx <= exitX + exitWidth && my >= exitY && my <= exitY + exitHeight)
			exit(0);
	}
}
void saveHighScore() {
	FILE* f = fopen("highscore.txt", "w");
	if (f != NULL) {
		fprintf(f, "%d", highScore);
		fclose(f); 
	}
}

void loadHighScore() {
	FILE* f = fopen("highscore.txt", "r");
	if (f != NULL) {
		fscanf(f, "%d", &highScore);
		fclose(f);
	}
}

void moveShark() {

	if (sharkX >= screenWidth * 8 - sharkWidth) {
		sharkX = 0;   
	}

	if (joyActive && gameState == 1) {
		float dx = cos(joyAngle) * 9;
		float dy = sin(joyAngle) * 9;

		sharkX += dx;
		sharkY += dy;

		if (dx > 0) sharkPr = true;
		if (dx < 0) sharkPr = false;
		if (dx>0 &&dy>0) sharkup=true;
		if (dx<0 && dy<0) sharkup = false;

	

			if (sharkX < 0) sharkX = 0;
			if (sharkY < 0) sharkY = 0;

			float maxX = screenWidth * 8 - sharkWidth;
			float maxY = screenHeight * 4 - sharkHeight;

			if (sharkX > maxX) sharkX = maxX;
			if (sharkY > maxY) sharkY = maxY;
		}
		
		// ---- Collision with fish ----
		for (int i = 0; i < NUM_FISH; i++) {
			if (sharkX < fishX[i] + fishWidth && sharkX + sharkWidth > fishX[i] &&
				sharkY < fishY[i] + fishHeight && sharkY + sharkHeight > fishY[i]) {

				fishX[i] = screenWidth * 2 + rand() % 700;
				fishY[i] = rand() % (screenHeight * 2 - fishHeight);

				sharkWidth += 4;
				sharkHeight += 3;
				sharkHealth += 10;
				score += 10;
				if (score > highScore) {
					highScore = score;
					saveHighScore();
				}
				if (sharkHealth > sharkMaxHealth) sharkHealth = sharkMaxHealth;
			}
		}
	for (int i = 0; i < NUM_ENEMIES; i++) {
			if (sharkX < enemyX[i] + enemyWidth && sharkX + sharkWidth > enemyX[i] &&
				sharkY < enemyY[i] + enemyHeight && sharkY + sharkHeight > enemyY[i]) {
				if (sharkHeight >80 & sharkWidth >120){


				enemyX[i] = screenWidth * 2 + rand() % 400;
				enemyY[i] = rand() % (screenHeight * 2 - enemyHeight);

				sharkWidth += 4;
				sharkHeight += 3;
				sharkHealth += 10;
				score += 10;
				if (score > highScore) {
					highScore = score;
					saveHighScore();
				}
				if (sharkHealth > sharkMaxHealth) 
					sharkHealth = sharkMaxHealth;
			}
				else{
					sharkHealth -= 10;


					enemyX[i] = screenWidth * 2 + rand() % 800;
					enemyY[i] = rand() % (screenHeight * 3 - enemyHeight);
					enemySpeed[i] = 2.0f + rand() % 3;
				}
		
				
				if (sharkHealth <= 0) {
					sharkHealth = 0;
			
					gameState = 2;

				}


			}
		}
	

	for (int i = 0; i < BIG_fish; i++) {
		if (sharkX < benemyX[i] + bWidth && sharkX + sharkWidth > benemyX[i] &&
			sharkY < benemyY[i] + bHeight && sharkY + sharkHeight > benemyY[i]) {
			if (sharkHeight >220 & sharkWidth >160){


				benemyX[i] = screenWidth * 2 + rand() % 400;
				benemyY[i] = rand() % (screenHeight * 2 - bHeight);

				sharkWidth += 4;
				sharkHeight += 3;
				sharkHealth += 10;
				score += 10;
				if (score > highScore) {
					highScore = score;
					saveHighScore();
				}
				if (sharkHealth > sharkMaxHealth) sharkHealth = sharkMaxHealth;
			}
			else{
				sharkHealth -= 40;
				benemyX[i] = screenWidth * 2 + rand() % 800;
				benemyY[i] = rand() % (screenHeight * 4 - bHeight);
				benemySpeed[i] = 2.5f + rand() % 3;
			}


			if (sharkHealth <= 0) {
				sharkHealth = 0;

				gameState = 2;

			}


		}
	}
	
		if (sharkX + sharkWidth >= 1300) {
		
			 joyActive = TRUE;
		}

		if (bombActive) {
			if (sharkX < bombX + bombWidth && sharkX + sharkWidth > bombX &&
				sharkY < bombY + bombHeight && sharkY + sharkHeight > bombY) {
				bombActive = false;
				sharkHealth -= 35;
				sharkWidth -= 5;
				sharkHeight -= 4;
				if (sharkHealth <= 0) {
					sharkHealth = 0;
					gameState = 2;
				}
			}
		}

	
}


void moveFish() {
	for (int i = 0; i < NUM_FISH; i++) {
		fishX[i] += fishSpeedX[i];
		fishY[i] += fishSpeedY[i];
		if (fishY[i] <= 0 || fishY[i] + fishHeight >= screenHeight * 2)
			fishSpeedY[i] = -fishSpeedY[i]  ;
		if (fishX[i] + fishWidth < 0) {
			fishX[i] = screenWidth * 2;
			fishY[i] = rand() % (screenHeight * 4 - fishHeight);
		}
	}
}


void moveEnemies() {
	for (int i = 0; i < NUM_ENEMIES; i++) {
		enemyX[i] -= enemySpeed[i];
		if (enemyX[i] + enemyWidth < 0) {
			enemyX[i] = screenWidth * 2 + rand() % 600;
			enemyY[i] = rand() % (screenHeight * 2 - enemyHeight);
			enemySpeed[i] = 2.0f + rand() % 3;
		} 
		
	}
}
void moveBIG_fish() {
	for (int i = 0; i < BIG_fish; i++) {
		benemyX[i] -= benemySpeed[i];
		if (benemyX[i] + bWidth < 0) {
			benemyX[i] = screenWidth * 2 + rand() % 600;
			benemyY[i] = rand() % (screenHeight * 2 - bHeight);
			benemySpeed[i] = 2.0f + rand() % 3;
		}
	}
}


	void iKeyboard(unsigned char key) {
		if (gameState == 3 && (key == 'm' || key == 'M')) {
			gameState = 0; 
		}
		if (gameState == 2 && (key == 'r' || key == 'R')) {
			sharkHealth = sharkMaxHealth;
			sharkWidth = 120;
			sharkHeight = 80;
			camX = camY = 0;
			score = 0;
			for (int i = 0; i < NUM_FISH; i++) {
		fishX[i] = screenWidth + rand() % 500 ;
		fishY[i] = rand() % (screenHeight * 2 - fishHeight);
		fishSpeedX[i] = 2.0f +rand() % 3;
		if (rand() % 2 == 0)
			fishSpeedY[i] = 0.03f;
		else
			fishSpeedY[i] = 0.3f;

	}
			gameState = 1;
		}
	}

	void moveBubbles() {
		for (int i = 0; i < NUM_BUBBLES; i++) {
			if (bubbleDir[i] == 1) {
				bubbleY[i] += 2;
				if (bubbleY[i] > screenHeight * 2) {
					bubbleY[i] = 0;
					bubbleX[i] = rand() % (screenWidth * 4);
				}
			}
			else {
				bubbleY[i] -= 2;
				if (bubbleY[i] < 0) {
					bubbleY[i] = screenHeight * 2;
					bubbleX[i] = rand() % (screenWidth * 4);
				}
			}
		}
	}


	void spawnBombIfReady() {
		if (gameState != 1) return;
		secondsCounter++;
		if (secondsCounter >= bombIntervalSec) {
			secondsCounter = 0;
			bombActive = true;
			// spawn bomb somewhere ahead in world X relative to camera
			float spawnXMin = camX + screenWidth;
			float spawnXMax = camX + screenWidth * 2;
			if (spawnXMax > screenWidth * 8 - bombWidth) spawnXMax = screenWidth * 8 - bombWidth;
			if (spawnXMin > spawnXMax) spawnXMin = spawnXMax - 100;
			bombX = spawnXMin + rand() % (int)(spawnXMax - spawnXMin + 1);
			bombY = camY + (float)screenHeight + (rand() % (screenHeight)); // above view
			bombSpeedY = 4.0f + (float)(rand() % 3);
		}
	}

	void updateBomb() {
		if (gameState != 1 || !bombActive) return;
		bombY -= bombSpeedY;
		if (bombY < camY - 200.0f) {
			bombActive = false; 
		}
	}


	


void iSpecialKeyboard(unsigned char key) {}

// ----------------------
int main() {
	startX = screenWidth / 2 - startWidth / 2;
	startY = screenHeight / 2 + 50;
	exitX = screenWidth / 2 - exitWidth / 2;
	exitY = screenHeight / 2 - 50;

	joyCenterX = screenWidth / 8;
	joyCenterY = screenHeight / 6;

	iInitialize(screenWidth, screenHeight, "Shark Game ");

	bgImage = iLoadImage("2.png");
	sharkImage = iLoadImage("1.png");
	ugImage = iLoadImage("3.png");
	fishImage = iLoadImage("Fish.png");
	bombImage = iLoadImage("Bomb.png");


	for (int i = 0; i < NUM_FISH; i++) {
		fishX[i] =  rand() % 600;
		fishY[i] = rand() % (screenHeight * 3- fishHeight);
		fishSpeedX[i] = 3.0f +rand() % 3;
		if (rand() % 2 == 0)
			fishSpeedY[i] = 0.03f;
		else
			fishSpeedY[i] = 0.3f;

	}

	enemyImage[0] = iLoadImage("e1.png");
	enemyImage[1] = iLoadImage("e2.png");
	enemyImage[2] = iLoadImage("e3.png");
	enemyImage[3] = iLoadImage("e4.png");
	enemyImage[4] = iLoadImage("e5.png");
	benemyImage[0] = iLoadImage("be1.png");
	benemyImage[1] = iLoadImage("be2.png");
	benemyImage[2] = iLoadImage("be3.png");
	for (int i = 0; i < NUM_ENEMIES; i++) {
		enemyX[i] = screenWidth * 2 + rand() % 500 ;
		enemyY[i] = rand() % (screenHeight  - enemyHeight);
		enemySpeed[i] = 3.0f + rand() % 3;
	}

	for (int i = 0; i < BIG_fish; i++) {
		benemyX[i] = screenWidth * 2 + rand() % 500 + i * 300;
		benemyY[i] = rand() % (screenHeight - bHeight);
		benemySpeed[i] = 3.0f + rand() % 3;
	}
	for (int i = 0; i < NUM_BUBBLES; i++) {
		bubbleX[i] = rand() % screenWidth;
		bubbleY[i] = rand() % screenHeight;
		bubbleDir[i] = rand() % 2;
	}
	iSetTimer(10, moveShark);
	iSetTimer(10, moveFish);
	iSetTimer(1, moveEnemies);
	iSetTimer(4, moveBIG_fish);
	iSetTimer(500, spawnBombIfReady); 
	iSetTimer(30, updateBomb);

	iSetTimer(10, moveBubbles);
	loadHighScore();

	iStart();
	return 0;
}

