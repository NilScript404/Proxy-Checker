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

// 
// Todo => dynamic ListBox Height => based on the amount of good proxies + default height
//	
// Todo => dyanmic text lining in the ListBox => width should probably be bigger =>
//			  probably have to mess with textMeasure for finding the width and height
//			  of the texts => the size can also be used for resizing the the ListBox
//			  imagine the default height is 400 , but a single line of proxy has a height of 10px
// 		  we can add the value to the ListBox height value => must not forget about dynamicly
//			  changing ScrollBar Offset and its properties to give a smooth feeling to the ListBox

Vector2 GetCenteredTextPosition(Rectangle rect, const char *text, int fontSize);
void DrawStartButton(int mouseX, int mouseY, char *debug);

int main(void)
{
	InitWindow(screenWidth, screenHeight, "Raylib Window");
	SetTargetFPS(60);
	
	char Debug[20];
	char *StartButtonLabel = "START";
	int scrollOffset = 0;
	int scrollMax = 400;
	Rectangle scrollBarRec = {475, 0, 25, 400};
	Rectangle listBox = {250, 0, 250, 400};
	
	char proxies[100] = "1 - 187.19.127.246:8011";
	int textWidth = MeasureText(proxies , 20);
	int i = 0;
	
	while (!WindowShouldClose())
	{
		// Update mouse position
		int mousePosX = GetMouseX();
		int mousePosY = GetMouseY();
		sprintf(Debug, "%d", mousePosX);
		
		BeginDrawing();
		ClearBackground(DARKISH);
				
		// Create the scrollbar and the listbox
		scrollOffset = GuiScrollBar(scrollBarRec, scrollOffset, 0, scrollMax);
		DrawRectangle(225, 0, 250, 10000 - scrollOffset , DARKMODERN);
		
		// Draw the start Button 
		DrawStartButton(mousePosX, mousePosY, Debug);
		
		// Draw centered text
		Vector2 position = GetCenteredTextPosition(StartButton, StartButtonLabel, 20);
		DrawText(StartButtonLabel, position.x, position.y, 20, YELLOW);
		
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
	}
}