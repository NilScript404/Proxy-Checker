#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used services from windows headers
#define NOGDI                // Exclude GDI functions and definitions
#define NODRAWTEXT           // Exclude DrawText() and DT_* constants
#define NOCLOSE              // Exclude CloseWindow()
#define NOSHOWWINDOW         // Exclude ShowWindow() and related functions
#define NOUSER               // Exclude all USER specific functions

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <curl/curl.h>

#define DARKISH (Color) { 19, 20, 21, 255 }
#define DARKMODERN (Color) { 37, 38, 44, 255 }
#define StartButton (Rectangle) { 0, 0, 120, 50 }
#define screenWidth 500
#define screenHeight 400



// ------------------Curl requests------------------
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
void HandleResponse(char *proxy, CURL *curl, CURLcode curlCode);
void PerformProxyRequest(char *proxy, const char *URL);
// ------------------Curl requests------------------

// ------------------Raylib-------------------------
Vector2 GetCenteredTextPosition(Rectangle rect, const char *text, int fontSize);
void DrawStartButton(int mouseX, int mouseY, char *debug);
static int scrollMax = 20;
// ------------------Raylib-------------------------

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


// stops curl from printing the the response , which always happens when we perform the request
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
   (void)contents; // ignore unused parameter warning
   (void)userp;    // ignore unused parameter warning
	// the actual size of the data chunk , this is the only thing libcurl needs to confirm
	// that WriteCallback has been successfull
	return size * nmemb; 
}

// it makes save to have two dynamic arrays , one for good proxies , another for bad proxies
// save each array into a seperate text file , but only render the good ones xd
// need a global variables for counting the number of good proxies
// same as bad proxies
void Responsehandler(char *proxy , CURL *curl , CURLcode curlCode){
	
	int intstatusCode;
	char stringstatusCode[20];
	const char *Err = curl_easy_strerror(curlCode);
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &intstatusCode);
	snprintf(stringstatusCode, sizeof(stringstatusCode), "%d", intstatusCode);
	
	if (curlCode == CURLE_OK) {
		// append the proxy : DELAY to the Good array
		// good++
	} else if  (intstatusCode > 400) {
		// append the PROXY : StatusCode to the Bad array
		// bad++
		proxy = (char *)malloc(strlen(proxy) + strlen(stringstatusCode) + 1);
		strcat(proxy , stringstatusCode);
	} else {
		// append the PROXY : ERROR to the Bad array
		// bad++
		proxy = (char *)malloc(strlen(proxy) + strlen(Err) + 1);
		strcat(proxy, Err);
	}
	// for debugging purposes
	printf("%s", proxy);
}

// performs the request and sends the response to the ResponeHandler function 
void ProxyRequest(char *proxy , char *URL){
	CURL *curl;
	curl = curl_easy_init();
	if (!curl) {
		// append to the Bad Array PROXY : HANDLER Creation failed
		printf("Failed at creating the handler");
		return;
	}
	
	CURLcode curlCode;
	curl_easy_setopt(curl, CURLOPT_URL, URL);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 10000L);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	
	curlCode = curl_easy_perform(curl);
	Responsehandler("Proxy" , curl , curlCode);
	curl_easy_cleanup(curl);
	
	// ------------------TODO------------------
	// curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);
	// curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 2L);
	// curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 1000000000L);
	// ------------------TODO------------------
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
		
		// TODO
		// multithreading => so far we are only using a single thread , and even that thread
		// still doesnt have proper capturing and saving functionality.
		// i need to store the good and bad proxies with their capture in a seperate file
		// then i should focus on the multithreading part , im probably going to use same
		// simple algorithm that i used in the C# code , since its minimal and easy to maintain
		// but it should probably be done better and i should consider edge cases . 
		// such as when we load 706 proxies and the user wants 10 proxy , then each proxy
		// is going to check 100 proxies and the last 6 is a leftover , so we need another
		// extra thread or one of the exisiting threads to finish the job by checking those 
		// 6 proxies as well
		// TODO
		// we need to implement multiple profile , some of them with heavy capturing and with 
		// the focus on quality of proxy , another one could be super lightweight to just
		// find alive proxies in a very short time , without any capture 
		// some capturing ideas : country , speed(kb , libcurl as functions) , response time
		// TODO 
		// the gui can be can be easily be improved , raylib is super flexible and we can do
		// some insane stuff , make the app looks very modern while the backend is super
		// lightweight , cool stuff overall
		// TODO
		// extra buttons on the gui => loading proxies , saving proxies , opening results ( good and bad)
		// need some labels for goods and bads
		// also a controlbar so that the user can select profiles , edit profiles , and his own
		// checking config , and capable saving of his config
		// the config setting should probably be in a seperate tab , which you can click on
		// and then raylib will render that tab as the main tab , and the setting tab should
		// let the user customize lots of stuff , selecting profiles and editing them or removing them
		// creating profile with many options , such as timeout , checking website
		// TODO 
		// we can implement multiple checking config in a same config , idk maybe checking the proxies
		// twice , each on different website , each with its own config , for example one of them
		// could check if the proxy bypasses something , another one can just check if its alive
		
		// TODO TODO TODO TODO
		// we can do extensive research and experiment on proxy checking and finding
		// edge cases , fixing them , improving them , abusing them , but probably needs lots of
		// testing on different stuff
		// we could also add a proxy grabber => a seperate app maybe? xd 
		// TODO TODO TODO TODO 
		
		
		
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
