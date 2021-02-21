#include <math.h>
#include <list>
#include <iostream>
#include "Color.h"

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

Color GetColor(Vector2 pos) {
	unsigned int* pixel = (unsigned int*)render_state.memory + (int)pos.x + (int)pos.y * render_state.width;
	return Color::UINTToColor(*pixel);
}

void Point(Vector2 pos, Color color) {
	unsigned int* pixel = (unsigned int*)render_state.memory + (int)pos.x + (int)pos.y * render_state.width;

	Color bgColor = GetColor(pos);
	Color alphaCol = (color + bgColor) * color.alpha;

	unsigned int alphaColor = Color::ColorToUINT(alphaCol);
	*pixel++ = alphaColor;
}

void DrawLine(Vector2 p1, Vector2 p2, Color color, bool antialias = false) {
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
				Point(Vector2(drawX, drawY), color);
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
				Point(Vector2(drawX, drawY), color);
			}
		}
	}
	else {
		int dx = x1 - x0;
		int dy = y1 - y0;

		int from = (x0 <= x1) ? x0 : x1;
		int to = (x0 < x1) ? x1 : x0;

		for (float x = from; x < to; x += 0.1f) {
			float y = y1 + dy * (x - x1) / dx;

			int drawX = x;
			int drawY = y;
			if (Bounds::pointInBounds(Vector2(drawX, drawY), Bounds(Vector2(halfWidth, halfHeight), Vector2(halfWidth - 1, halfHeight - 1)))) {
				Point(Vector2(drawX, drawY), color);
			}
		}
	}
}

void DrawThickLine(Vector2 p1, Vector2 p2,int thickness, Color color) {
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
				Point(Vector2(x0 + x, y), color);
			}
		}
	}
	else if (y0 == y1) {
		//Horizontal line (m=0)
		int from = (x0 <= x1) ? x0 : x1;
		int to = (x0 < x1) ? x1 : x0;

		for (int x = from; x < to; x++) {
			for (int y = -thickness; y < thickness; y++) {
				Point(Vector2(x, y0 + y), color);
			}
		}
	}
	else {
		int dx = x1 - x0;
		int dy = y1 - y0;

		int from = (x0 <= x1) ? x0 : x1;
		int to = (x0 < x1) ? x1 : x0;

		DrawLine(Vector2(x0, y0), Vector2(x1, y1), color);

		int m = (dy/dx);
		if(m != 0)
			int c = (y1/m)-x1;

		/*for (int x = from; x < to; x+=1) {
			int y = y1 + dy * (x - x1) / dx;

			unsigned int* pixel = (unsigned int*)render_state.memory + ((int)(x+(thickness/2)))+((y+(thickness/2)))*render_state.width;
			for (float c = -thickness; c < thickness; c++) {
				*pixel++ = color;
			}
		}*/
	}
}

void DrawRect(Vector2 position, Vector2 size, Color color) {
	int x0 = Mathf::Clamp(position.x - size.x, 1, render_state.width-1);
	int x1 = Mathf::Clamp(position.x + size.x, 1, render_state.width-1);

	int y0 = Mathf::Clamp(position.y - size.y, 1, render_state.height-1);
	int y1 = Mathf::Clamp(position.y + size.y, 1, render_state.height-1);

	unsigned int pixelColor = Color::ColorToUINT(color);

	for (int y = y0; y < y1; y++) {
		unsigned int* pixel = (unsigned int*)render_state.memory + x0 + y * render_state.width;

		for (int x = x0; x < x1; x++) {
			*pixel++ = pixelColor;
		}
	}
}

void DrawRotatedRect(Vector2 position, Vector2 size, float rotation, Color color) {
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

void DrawPoly(std::list<Vector2> points, Color color) {
	Vector2 prevPoint = points.front();
	for (Vector2 p : points) {
		DrawLine(prevPoint, p, color);
		prevPoint = p;
	}
}

void DrawPoly(std::list<Vector2> points,bool connect,Color color) {
	Vector2 prevPoint = points.front();
	for (Vector2 p : points) {
		DrawLine(prevPoint, p, color);
		prevPoint = p;
	}
	DrawLine(points.front(), points.back(),color);
}

void DrawThickPoly(std::list<Vector2> points,float thickness, Color color) {
	Vector2 prevPoint = points.front();
	for (Vector2 p : points) {
		DrawThickLine(prevPoint, p,thickness, color);
		prevPoint = p;
	}
}

void DrawThickPoly(std::list<Vector2> points,float thickness, bool connect, Color color) {
	Vector2 prevPoint = points.front();
	for (Vector2 p : points) {
		DrawThickLine(prevPoint, p,thickness, color);
		prevPoint = p;
	}
	DrawThickLine(points.front(), points.back(),thickness, color);
}


void DrawCircle(Vector2 position, float radius, bool filled, Color color) {
	Vector2 previousPoint = Vector2(position.x + cos(0) * radius, position.y + sin(0) * radius);
	
	for (float r = 0; r < (M_PI * 2); r += (M_PI / radius)) {
		int x = cos(r) * radius;
		int y = sin(r) * radius;

		int drawX = position.x + x;
		int drawY = position.y + y;

		DrawLine(Vector2(drawX, drawY), previousPoint, color);
		previousPoint = Vector2(drawX, drawY);
	}

	if (filled) {
	}
}

void DrawCircle(Vector2 position, float radius, int edgeRadius, bool filled, Color color) {
	Vector2 previousPoint = Vector2(position.x + cos(0) * radius, position.y + sin(0) * radius);

	for (float r = 0; r < (M_PI * 2); r += (M_PI / radius)) {
		int x = cos(r) * radius;
		int y = sin(r) * radius;

		int drawX = position.x + x;
		int drawY = position.y + y;

		DrawThickLine(Vector2(drawX, drawY), previousPoint, edgeRadius, color);
		previousPoint = Vector2(drawX, drawY);
	}

	if (filled) {
	}
}

void DrawCircle(Vector2 position, float radius, int edgeRadius,Color  color) {
	Vector2 previousPoint = Vector2(position.x + cos(0) * radius, position.y + sin(0) * radius);

	for (float r = 0; r < (M_PI * 2); r += (M_PI / radius)) {
		int x = cos(r) * radius;
		int y = sin(r) * radius;

		int drawX = position.x + x;
		int drawY = position.y + y;

		DrawThickLine(Vector2(drawX, drawY), previousPoint, edgeRadius, color);
		previousPoint = Vector2(drawX, drawY);
	}
}

void DrawCircle(Vector2 position, float radius, Color color) {
	Vector2 previousPoint = Vector2(position.x + cos(0) * radius, position.y + sin(0) * radius);

	for (float r = 0; r < (M_PI * 2); r += (M_PI / radius)) {
		int x = cos(r) * radius;
		int y = sin(r) * radius;

		int drawX = position.x + x;
		int drawY = position.y + y;

		DrawLine(Vector2(drawX, drawY), previousPoint, color);
		previousPoint = Vector2(drawX, drawY);
	}
}

