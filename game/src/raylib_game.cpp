#include <raylib.h>
#include <string.h>
//#include "winuser.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

typedef struct Vector2I {
	int x;
	int y;
} Vector2I;

extern "C" {
	int GetCursorPos(Vector2I* point);
}

Vector2 GetHalf(Vector2 size) {
	return { (size.x / 2.0f), (size.y / 2.0f) };
}
Vector2I ToInt(Vector2 vector) {
	return { (int)vector.x, (int)vector.y };
}
Vector2 ToFloat(Vector2I vector) {
	return { (float)vector.x, (float)vector.y };
}

float Remap(float x, float in_min, float in_max, float out_min, float out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

typedef struct Button {
	Rectangle bounds;
	int state;
	int action;
} Button;

Font font = { 0 };
Texture2D mouseTex = { 0 };
Vector2 mouseTextureHalf;

int monitorNumber = 0;
Vector2I monitorSize = { 0, 0 };

Vector2I screenSize = { 800, 450 };
Vector2 screenHalf = { 0, 0 };

const int targetFPS = 30;

static void Init(void)
{
	SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenSize.x, screenSize.y, "raylib-mouse-keyboard");

	monitorSize = { GetMonitorWidth(monitorNumber), GetMonitorHeight(monitorNumber) };
	screenHalf = GetHalf(ToFloat(screenSize));

	//SetWindowPosition(monitorSize.x / 2.0f - screenCenter.x, monitorSize.y / 2.0f - screenCenter.y);
	//SetWindowState(FLAG_WINDOW_UNDECORATED);

	font = LoadFont("resources/mecha.png");
	Image mouseImg = LoadImage("resources/mouse.png");
	mouseTex = LoadTextureFromImage(mouseImg);
	mouseTextureHalf = GetHalf({ (float)mouseTex.width, (float)mouseTex.height });
	UnloadImage(mouseImg);
}
static void Unload(void)
{
	UnloadFont(font);
	CloseWindow(); // Close window and OpenGL context
}

Button btn = { { 5, 5, 75, 50}, 0, 0 };
const int baseFontSize = 20;
const int baseHalfFontSize = 10;
int counter = 0;

static void UpdateDrawFrame(void)
{
	Vector2 mousePos = GetMousePosition();
	Vector2I winMousePos;
	GetCursorPos(&winMousePos);

	BeginDrawing();
	ClearBackground(RAYWHITE);
	//ClearBackground(BLANK);

	float x = Remap(winMousePos.x, 0.0f, monitorSize.x, 0.0f, screenSize.x);
	float y = Remap(winMousePos.y, 0.0f, monitorSize.y, 0.0f, screenSize.y);
	DrawTexturePro(mouseTex,
		{ 0.0f, 0.0f, (float)mouseTex.width, (float)mouseTex.height },
		{ x, y, (float)mouseTex.width, (float)mouseTex.height },
		{ mouseTextureHalf.x, mouseTextureHalf.y } /*Pivot*/,
		0.0f, WHITE);


	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		DrawRectangle(400, 200, 100, 100, ORANGE);

	// DRAW HUD - Only if widows if focused
	if (IsWindowFocused()) 
	{
		DrawFPS(screenSize.x - 100, 10);
		
		// Debug Text
		const char* txt = TextFormat("%d %d %d", winMousePos.x, winMousePos.y, counter++);
		DrawText(txt, screenSize.x / 2 - strlen(txt) * baseHalfFontSize, 10, baseFontSize, DARKGRAY);

		// Debug Button
		if (CheckCollisionPointRec(mousePos, btn.bounds))
		{
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
				btn.state = 2;
			else
				btn.state = 1;

			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
				btn.action = true;
		}
		DrawRectangle(btn.bounds.x, btn.bounds.y, btn.bounds.width, btn.bounds.height, DARKGREEN);
		DrawText("Debug", btn.bounds.x + baseHalfFontSize, btn.bounds.y + btn.bounds.height / 2.0f - baseHalfFontSize, baseFontSize, BLACK);
	}

	EndDrawing();
}

int main(void)
{
	Init();
#if defined(PLATFORM_WEB)
	emscripten_set_main_loop(UpdateDrawFrame, targetFPS, 1);
#else
	SetTargetFPS(targetFPS);
	while (!WindowShouldClose()) // Detect window close button or ESC key
		UpdateDrawFrame();
#endif
	Unload();
	return 0;
}