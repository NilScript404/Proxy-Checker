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
	
	char Debug[20];
	char *StartButtonLabel = "START";
	
	// Listbox and its scrolloffset
	int scrollOffset = 0;
	Rectangle scrollBarRec = {475, 0, 25, 400};
	Rectangle listBox = {250, 0, 250, 400};
	
	// reading the file and the buffer that we use for storing the data
	FILE *file = fopen("proxies.txt" , "r");
	char line[128];
	int linenumber = 1;
	if (file == NULL){
		printf("Proxies.txt Not Found");
		return 1;
	}
	
	// proxies array for storing the data from fgets
	StringArray proxiesBuffer;
	init_array(&proxiesBuffer, 40);
	
	// good array for storing good proxies
	StringArray goodBuffer;
	init_array(&goodBuffer, 40);
	int totalproxies = 2;
	int j = 1;
	while (!WindowShouldClose())
	{
		// performing requests in a loop sounds like insanity so theres gotta be a better way
		// our main function is ran once , maybe if we use multiple threads before entering
		// the render loop , we can check the proxies , but the issue is that we need to
		// keep updating variables , and also render the updated array
		// lets say we create two threads
		// 1 => gets one good proxy => update the array
		// 2 => gets a bad proxy => update the bad array
		// since we had a dynamic array , updating the array and rendering the proxy shouldnt be
		// any issue
		// so in theory , we can spawn many threads , but managing many threads is an issue on
		// its own
		// so lets focus on a single thread
		// the thread must perform the request , when the request is done , regardless of the result
		// it should start a new request with the next proxy
		// so we need a thread which exectues the Request function on different proxy , one by one
		// its easy to know which proxy it should request
		// but running the thread once again or performing the request for the next proxy with
		// a single thread might be a lil hard
		// like how do you know when to stop the thread from executing the request function ? =>
		// based on how many proxies we had ofc , if we have 40 , we need the loop to run 40 times
		// and each time its going to exectute the request , with a different proxy untill all of the
		// proxies are checked.
		// tl;dr => spwan a thread , let it run a loop untill all proxies are checked
		while (j <= totalproxies){
			
			j++;
		}

		// Update mouse position
		int mousePosX = GetMouseX();
		int mousePosY = GetMouseY();
		sprintf(Debug, "%d", mousePosX);
		
		// Debug prints		
		printf("Current Scroll Offset: %d\n", scrollOffset);
		printf("MaxScrollOffset: %d \n", scrollMax);
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
