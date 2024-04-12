#include "raylib.h"
#include <string.h>
#include "my_platform.h"

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

typedef struct Mouse {
	Texture2D body;
	Texture2D left;
	Texture2D right;
	Texture2D scroll;
	Rectangle rect;
	Vector2 pivot;
} Mouse;


Font font = { 0 };
Mouse mouseTex = { 0 };

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
	mouseTex.body = LoadTextureFromImage(mouseImg);
	UnloadImage(mouseImg);
	mouseImg = LoadImage("resources/mouse_left.png");
	mouseTex.left = LoadTextureFromImage(mouseImg);
	UnloadImage(mouseImg);
	mouseImg = LoadImage("resources/mouse_right.png");
	mouseTex.right = LoadTextureFromImage(mouseImg);
	UnloadImage(mouseImg);
	mouseImg = LoadImage("resources/mouse_middle.png");
	mouseTex.scroll = LoadTextureFromImage(mouseImg);
	UnloadImage(mouseImg);

	mouseTex.rect = { 0.0f, 0.0f, (float)mouseTex.body.width, (float)mouseTex.body.height };
	mouseTex.pivot = GetHalf({ (float)mouseTex.body.width, (float)mouseTex.body.height });
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
	Vector2I winMousePos = GetMouseMonitorPosition();
	bool focused = IsWindowFocused();

	BeginDrawing();
	ClearBackground(focused ? RAYWHITE : BLANK);

	float m_x = Remap(winMousePos.x, 0.0f, monitorSize.x, 0.0f, screenSize.x);
	float m_y = Remap(winMousePos.y, 0.0f, monitorSize.y, 0.0f, screenSize.y);
	Rectangle m_rect = { m_x, m_y, mouseTex.rect.width,mouseTex.rect.height };
	DrawTexturePro(mouseTex.body, mouseTex.rect, m_rect, mouseTex.pivot, 0.0f, WHITE);
	if (GetMouseKeyLeft())
		DrawTexturePro(mouseTex.left, mouseTex.rect, m_rect, mouseTex.pivot, 0.0f, WHITE);
	if (GetMouseKeyRight())
		DrawTexturePro(mouseTex.right, mouseTex.rect, m_rect, mouseTex.pivot, 0.0f, WHITE);
	if (GetMouseKeyMiddle())
		DrawTexturePro(mouseTex.scroll, mouseTex.rect, m_rect, mouseTex.pivot, 0.0f, WHITE);

	// DRAW HUD - Only if widows if focused
	if (focused)
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
	SetTargetFPS(targetFPS);
	while (!WindowShouldClose()) // Detect window close button or ESC key
		UpdateDrawFrame();
	Unload();
	return 0;
}