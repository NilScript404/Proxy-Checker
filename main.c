#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define Background (Color) { 245, 245, 245, 255 }
#define DisplayBackground (Color) { 242, 242, 242, 255 }
#define ButtonNumber (Color) { 208, 231, 249, 255 }
#define ButtonFunction (Color) { 243, 156, 18, 255 }
#define ButtonClear (Color) { 247, 140, 107, 255 }
#define ButtonEquals (Color) { 52, 199, 89, 255 }

typedef enum
{
	BUTTON_TYPE_NUMBER,
	BUTTON_TYPE_FUNCTION,
	BUTTON_TYPE_CLEAR,
	BUTTON_TYPE_EQUALS
} ButtonType;

ButtonType buttonTypes[16] = 
{
	BUTTON_TYPE_NUMBER, BUTTON_TYPE_NUMBER, BUTTON_TYPE_NUMBER, BUTTON_TYPE_FUNCTION,
	BUTTON_TYPE_NUMBER, BUTTON_TYPE_NUMBER, BUTTON_TYPE_NUMBER, BUTTON_TYPE_FUNCTION,
	BUTTON_TYPE_NUMBER, BUTTON_TYPE_NUMBER, BUTTON_TYPE_NUMBER, BUTTON_TYPE_FUNCTION,
	BUTTON_TYPE_EQUALS, BUTTON_TYPE_NUMBER, BUTTON_TYPE_CLEAR, BUTTON_TYPE_FUNCTION
};

Rectangle recs[16];

// why ? because if i use something like char labels[16] , then i would have to convert each
// character to an string later on ( when i want to use string functions such as strcat)
// but , if i have an array of string and if each string is a single character , i can just
// do the same thing but without having to convert ( by accessing the characters via labels[index][0])
const char *labels[16] = 
{
	"1", "2", "3", "/",
	"4", "5", "6", "X",
	"7", "8", "9", "-",
	"=", "0", "C", "+"
};

Color colors[16] =
{
	ButtonNumber, ButtonNumber, ButtonNumber, ButtonFunction,
	ButtonNumber, ButtonNumber, ButtonNumber, ButtonFunction,
	ButtonNumber, ButtonNumber, ButtonNumber, ButtonFunction,
	ButtonEquals, ButtonNumber, ButtonClear, ButtonFunction
};

char num1[5];
char operation;
char num2[5];
char Display[40];
int counter = 0;

void calculate(char num1[], char num2[], char operation) {
	char Str[10];
	int n1 = atoi(num1);
	int n2 = atoi(num2);
	int result = 0;
	
	if (operation == '+') {
		result = n1 + n2;
	} else if (operation == '-') {
		result = n1 - n2;
	} else if (operation == 'X') {
		result = n1 * n2;
	} else if (operation == '/') {
		if (n2 != 0) {
			result = n1 / n2;
		} else {
			strcpy(Str, "Error");
			memset(Display, 0, sizeof(Display));
			strcat(Display, Str);
			return;
		}
	}
	
	sprintf(Str, "%d", result);
	strcat(Display, Str);
}

int main(void) {
	const int screenWidth = 350;
	const int screenHeight = 450;
	int margin = 10;
	
	InitWindow(screenWidth, screenHeight, "Calculator GUI");
	Font customFont = LoadFontEx("FiraCode-Regular.ttf", 200, 0, 0);
	int displayHeight = 100;
	
	int index = 0;
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			recs[index] = (Rectangle){(float)((j - 1) * 75) + (margin * j), (float)((i - 1) * 75) + displayHeight + (margin * i), 75, 75};
			index++;
		}
	}
	
	SetTargetFPS(60);
	
	while (!WindowShouldClose()) {
		for (int i = 0; i < 16; i++) {
			if (CheckCollisionPointRec(GetMousePosition(), recs[i]) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
				if (i != 14) strcat(Display, labels[i]);
				if (counter == 0 && (buttonTypes[i] == BUTTON_TYPE_NUMBER))
				{
					strcat(num1, labels[i]);
				} else if (counter == 0 && (buttonTypes[i] == BUTTON_TYPE_FUNCTION))
				{
					operation = labels[i][0];
					counter++;
				} else if (counter == 1 && (buttonTypes[i] == BUTTON_TYPE_NUMBER))
				{
					strcat(num2, labels[i]);
				} else if (counter == 1 && (buttonTypes[i] == BUTTON_TYPE_EQUALS))
				{
					//strcat(Display, labels[i]);
					calculate(num1, num2, operation);
					counter++;
				} else if (buttonTypes[i] == BUTTON_TYPE_CLEAR)
				{
					counter = 0;
					memset(Display, 0, sizeof(Display));
					memset(num1, 0, sizeof(num1));
					memset(num2, 0, sizeof(num2));
				}
			}
		}
		
		BeginDrawing();
		ClearBackground(Background);
		
		for (int i = 0; i < 16; i++) {
			DrawRectangleRounded(recs[i], 0.0, 24, colors[i]);
			DrawTextEx(customFont, labels[i], (Vector2){(recs[i].x + recs[i].width / 2) - 10, (recs[i].y + recs[i].height / 2) - 25}, 60, 0, DARKGRAY);
		}
		
		DrawRectangle(0, 0, 350, 100, DisplayBackground);
		DrawLineEx((Vector2){0, 100}, (Vector2){350, 100}, 3.0f, DARKGRAY);
		DrawText(Display, 0, 30, 60, DARKGRAY);
		
		EndDrawing();
	}
	
	UnloadFont(customFont);
	CloseWindow();
	return 0;
}
