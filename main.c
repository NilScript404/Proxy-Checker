#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#define DARKISH (Color) { 19, 20, 21, 255 }
#define DARKMODERN (Color) { 37, 38, 44, 255 }
#define StartButton (Rectangle) { 0, 0, 120, 50 }
#define screenWidth 500
#define screenHeight 400

Vector2 GetCenteredTextPosition(Rectangle rect, const char *text, int fontSize);
void DrawStartButton(int mouseX, int mouseY, char *debug);
static int scrollMax = 20;

int main(void)
{
	InitWindow(screenWidth, screenHeight, "Raylib Window");
	SetTargetFPS(60);
	
	char Debug[20];
	char *StartButtonLabel = "START";
	int scrollOffset = 0;
	Rectangle scrollBarRec = {475, 0, 25, 400};
	Rectangle listBox = {250, 0, 250, 400};
	
	FILE *file = fopen("proxies.txt" , "r");
	char line[128];
	int linenumber = 1;
	if (file == NULL){
		printf("Proxies.txt Not Found");
		return 1;
	}
	
	while (!WindowShouldClose())
	{
		// Update mouse position
		int mousePosX = GetMouseX();
		int mousePosY = GetMouseY();
		sprintf(Debug, "%d", mousePosX);
		
		printf("Current Scroll Offset: %d\n", scrollOffset);
		printf("MaxScrollOffset: %d \n", scrollMax);
		BeginDrawing();
		ClearBackground(DARKISH);
			
		// Create the scrollbar and the listbox
		// Todo => increment scrollMax by 1 everytime we get a good proxy =>
		// thats all we need for a dynamic Listbox
		scrollOffset = GuiScrollBar(scrollBarRec, scrollOffset, 0, scrollMax);
		DrawRectangle(225, 0, 250, 400 , DARKMODERN);
		
		DrawStartButton(mousePosX, mousePosY, Debug);
		
		// Draw a centered text for StartButton => we can use the same function for other stuff
		Vector2 position = GetCenteredTextPosition(StartButton, StartButtonLabel, 20);
		DrawText(StartButtonLabel, position.x, position.y, 20, YELLOW);
		
		DrawText("Proxy" , 300 , 200 , 20 , YELLOW);
		
		
		// TODO
		//  im stupid , fgets works with a FILE pointer , this thing works dynamicly
		//  this is only going to run about 20 time then the condition is not met anymore
		//  which is why we need an array or a way to store this garbo
		//  but the problem is that an array isnt dynamic , so how do we handle the read and write
		//  to the file ? while also updating the array ?
		//  need to learn how to write a dynamic array in C => dynamic memory allocation :D
		// TODO
		while (fgets(line , sizeof(line) , file) != NULL){
			DrawText(line , 200 , 0 , 20 , YELLOW);
		}
		
		EndDrawing();
	}
	
	CloseWindow();
	return 0;
}

Vector2 GetCenteredTextPosition(Rectangle rect, const char *text, int fontSize)
{
	int textWidth = MeasureText(text, fontSize); 			// Measure the width and height of the text
	int textHeight = fontSize; 									// Set height to the font size
	
	Vector2 position;													// Calculate the centered position
	position.x = rect.x + (rect.width - textWidth) / 2;	// Center horizontally
	position.y = rect.y + (rect.height - textHeight) / 2; // Center vertically
	
	return position; 													// Return the calculated position
}

// We also handle the mouse click , thats why this function needs mousex and mousey
void DrawStartButton(int mouseX, int mouseY, char *debug)
{
	Vector2 mouseVector = {mouseX, mouseY};
	
	DrawRectangleRounded(StartButton, 0.2, 16, DARKMODERN);
	
	if (CheckCollisionPointRec(mouseVector, StartButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		DrawText(debug, 200, 200, 40, DARKBROWN);
		scrollMax++;
	}
}
