#include <math.h>

void RenderBackground() {
	unsigned int* pixel = (unsigned int*)render_state.memory;
	for (int y = 0; y < render_state.height; y++) {
		for (int x = 0; x < render_state.width; x++) {
			*pixel++ = x * y;
		}
	}
}

void ClearScreen(unsigned int col) {
	unsigned int* pixel = (unsigned int*)render_state.memory;
	for (int y = 0; y < render_state.height; y++) {
		for (int x = 0; x < render_state.width; x++) {
			*pixel++ = col;
		}
	}
}

void DrawThickLine(Vector2 p1, Vector2 p2,int thickness, unsigned int color) {
	int x0 = Mathf::Clamp(p1.x, 1, render_state.width - 1);
	int x1 = Mathf::Clamp(p2.x, 1, render_state.width - 1);

	int y0 = Mathf::Clamp(p1.y, 1, render_state.height - 1);
	int y1 = Mathf::Clamp(p2.y, 1, render_state.height - 1);

	if (x0 == x1) {
		//Straight up (no gradient)
		int from = (y0 <= y1) ? y0 : y1;
		int to = (y0 < y1) ? y1 : y0;

		for (int y = from; y < to; y++) {
			for (int x = -thickness; x < thickness; x++) {
				unsigned int* pixel = (unsigned int*)render_state.memory + (x0 + x) + y * render_state.width;
				*pixel++ = color;
			}
		}
	}
	else if (y0 == y1) {
		//Horizontal line (m=0)
		int from = (x0 <= x1) ? x0 : x1;
		int to = (x0 < x1) ? x1 : x0;

		for (int x = from; x < to; x++) {
			for (int y = -thickness; y < thickness; y++) {
				unsigned int* pixel = (unsigned int*)render_state.memory + x + (y0 + y) * render_state.width;
				*pixel++ = color;
			}
		}
	}
	else {
		int dx = x1 - x0;
		int dy = y1 - y0;

		int from = (x0 <= x1) ? x0 : x1;
		int to = (x0 < x1) ? x1 : x0;

		for (int x = from; x < to; x++) {
			int y = y1 + dy * (x - x1) / dx;

			unsigned int* pixel = (unsigned int*)render_state.memory + (x+thickness)+(y+thickness)*render_state.width;
			for (int c = -thickness; c < thickness; c++) {
				*pixel++ = color;
			}
		}
	}
}

void DrawLine(Vector2 p1, Vector2 p2, unsigned int color) {
	int x0 = p1.x;
	int x1 = p2.x;

	int y0 = p1.y;
	int y1 = p2.y;

	if (x0 == x1) {
		//Straight up (no gradient)
		int from = (y0 <= y1) ? y0 : y1;
		int to = (y0 < y1) ? y1 : y0;

		for (int y = from; y < to; y++) {

			int drawX = x0;
			int drawY = y;
			if (Bounds::pointInBounds(Vector2(drawX, drawY), Bounds(Vector2(halfWidth, halfHeight), Vector2(halfWidth - 1, halfHeight - 1)))) {
				unsigned int* pixel = (unsigned int*)render_state.memory + drawX + drawY * render_state.width;
				*pixel++ = color;
			}
		}
	}
	else if (y0 == y1) {
		//Horizontal line (m=0)
		int from = (x0 <= x1) ? x0 : x1;
		int to = (x0 < x1) ? x1 : x0;

		for (int x = from; x < to; x++) {
			int drawX = x;
			int drawY = y0;
			if (Bounds::pointInBounds(Vector2(drawX, drawY), Bounds(Vector2(halfWidth, halfHeight), Vector2(halfWidth - 1, halfHeight - 1)))) {
				unsigned int* pixel = (unsigned int*)render_state.memory + drawX + drawY * render_state.width;
				*pixel++ = color;
			}
		}
	}
	else {
		int dx = x1 - x0;
		int dy = y1 - y0;

		int from = (x0 <= x1) ? x0 : x1;
		int to = (x0 < x1) ? x1 : x0;

		for (int x = from; x < to; x++) {
			int y = y1 + dy * (x - x1) / dx;

			int drawX = x;
			int drawY = y;
			if (Bounds::pointInBounds(Vector2(drawX, drawY), Bounds(Vector2(halfWidth, halfHeight), Vector2(halfWidth - 1, halfHeight - 1)))) {
				unsigned int* pixel = (unsigned int*)render_state.memory + drawX + drawY * render_state.width;
				*pixel++ = color;
			}
		}
	}
}

void DrawRect(Vector2 position, Vector2 size, unsigned int color) {
	int x0 = Mathf::Clamp(position.x - size.x, 1, render_state.width-1);
	int x1 = Mathf::Clamp(position.x + size.x, 1, render_state.width-1);

	int y0 = Mathf::Clamp(position.y - size.y, 1, render_state.height-1);
	int y1 = Mathf::Clamp(position.y + size.y, 1, render_state.height-1);

	for (int y = y0; y < y1; y++) {
		unsigned int* pixel = (unsigned int*)render_state.memory + x0 + y * render_state.width;

		for (int x = x0; x < x1; x++) {
			*pixel++ = color;
		}
	}
}

void DrawRotatedRect(Vector2 position, Vector2 size, float rotation, unsigned int color) {
	int x0 = position.x - size.x;
	int x1 = position.x + size.x;

	int y0 = position.y - size.y;
	int y1 = position.y + size.y;

	int cx = position.x;
	int cy = position.y;

	Vector2 center = Vector2(cx, cy);

	//Get distance from each point to center (cx,cy)
	Vector2 TL = Vector2(x0, y0);
	Vector2 TR = Vector2(x1, y0);
	Vector2 BL = Vector2(x0, y1);
	Vector2 BR = Vector2(x1, y1);

	int tlD = Vector2::Distance(TL, center);
	int trD = Vector2::Distance(TR, center);
	int blD = Vector2::Distance(BL, center);
	int brD = Vector2::Distance(BR, center);

	rotation *= (M_PI / 180);

	//Rotate point by rotation
	Vector2 newTL = Vector2::RotatePoint(center, TL, rotation);
	Vector2 newTR = Vector2::RotatePoint(center, TR, rotation);
	Vector2 newBR = Vector2::RotatePoint(center, BR, rotation);
	Vector2 newBL = Vector2::RotatePoint(center, BL, rotation);

	//Redraw lines
	DrawLine(newTL, newTR, color);
	DrawLine(newTR, newBR, color);
	DrawLine(newBR, newBL, color);
	DrawLine(newBL, newTL, color);
}

void DrawCircle(Vector2 position, float radius, unsigned int color) {
	for (float r = 0; r < 360; r += (M_PI / radius)) {
		int x = cos(r) * radius;
		int y = sin(r) * radius;

		int drawX = position.x + x;
		int drawY = position.y + y;
		if (Bounds::pointInBounds(Vector2(drawX,drawY), Bounds(Vector2(halfWidth, halfHeight),Vector2(halfWidth-1,halfHeight-1)))) {
			unsigned int* pixel = (unsigned int*)render_state.memory + drawX + drawY * render_state.width;
			*pixel++ = color;
		}
	}
}
