// Todo => cleaning up the main loop
//      => single threaded proxy checking D:
//      => multithreaded proxy checking => phthreads ?

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

typedef struct {
	char **data;
	int count;
	int capacity;
} StringArray;

void init_array(StringArray *array, int initial_capacity) {
	array->data = (char **)malloc(initial_capacity * sizeof(char *));
	array->count = 0;
	array->capacity = initial_capacity;
	
	if (array->data == NULL) {
		printf("Memory Allocation Failed\n");
		exit(1);
	}
}
// since we needed a dynamic array
void add_string(StringArray *array, const char *str) {
	if (array->count == array->capacity) {
		array->capacity *= 2;
		array->data = (char **)realloc(array->data, array->capacity * sizeof(char *));
		if (array->data == NULL) {
			printf("Memory Reallocation Failed\n");
			exit(1);
		}
	}
	
	array->data[array->count] = (char *)malloc(strlen(str) + 1);
	if (array->data[array->count] == NULL) {
		printf("String Memory Allocation Failed\n");
		exit(1);
	}
	strcpy(array->data[array->count], str);
	array->count++;
}
// i dont think we even need this
void free_array(StringArray *array) {
	for (int i = 0; i < array->count; i++) {
		free(array->data[i]);
	}
	free(array->data);
	array->data = NULL;
	array->count = 0;
	array->capacity = 0;
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

int main(void)
{
	InitWindow(screenWidth, screenHeight, "Raylib Window");
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
	
	while (!WindowShouldClose())
	{
		// Update mouse position
		int mousePosX = GetMouseX();
		int mousePosY = GetMouseY();
		sprintf(Debug, "%d", mousePosX);
		
		// Debug prints		
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
		
		while (fgets(line , sizeof(line) , file) != NULL){
			add_string(&proxiesBuffer , line);
		}
		printf("%d", proxiesBuffer.count);
		int minrender = scrollOffset; 
		int maxrender = scrollOffset + 20;
		int Ypos = 0;
		while (minrender < maxrender && proxiesBuffer.count > minrender){
			int currenty = 20 * minrender;
			DrawText(proxiesBuffer.data[minrender] , 225 , Ypos * 20 , 20 , YELLOW);
			minrender++;
			Ypos++;
		}
			
		EndDrawing();
	}
	
	CloseWindow();
	return 0;
}
