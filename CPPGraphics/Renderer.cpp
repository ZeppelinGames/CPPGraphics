#include <math.h>
#include <list>
#include <iostream>
#include "Color.h"

Color GetColor(Vector2 pos) {
	unsigned int* pixel = (unsigned int*)render_state.memory + (int)pos.x + (int)pos.y * render_state.width;
	return Color::UINTToColor(*pixel);
}

void Plot(Vector2 pos, Color color) {
	if (Bounds::pointInBounds(pos, render_state.screenBounds)) {
		unsigned int* pixel = (unsigned int*)render_state.memory + (int)pos.x + (int)pos.y * render_state.width;

		Color bgColor = GetColor(pos);
		color.alpha = 255;
		Color alphaCol = (color + bgColor) * color.alpha;

		unsigned int alphaColor = Color::ColorToUINT(alphaCol);
		*pixel++ = alphaColor;
	}
}

void ClearScreen(Color col) {
	unsigned int uintCol = Color::ColorToUINT(col);
	unsigned int* pixel = (unsigned int*)render_state.memory;

	for (int y = 0; y < render_state.height; y++) {
		for (int x = 0; x < render_state.width; x++) {
			*pixel++ = uintCol;
		}
	}
}

void PlotLineLow(Vector2 p1, Vector2 p2, Color col) {
	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;
	int yi = 1;
	if (dy < 0) {
		yi = -1;
		dy = -dy;
	}
	float D = (2 * dy) - dx;
	float y = p1.y;

	for (int x = p1.x; x < p2.x; x++) {
		Plot(Vector2(x, y), col);
		if (D > 0) {
			y += yi;
			D += (2 * (dy - dx));
		}
		else {

			D += 2 * dy;
		}
	}
}
void PlotLineHigh(Vector2 p1, Vector2 p2, Color col) {
	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;
	int xi = 1;
	if (dx < 0) {
		xi = -1;
		dx = -dx;
	}
	float D = (2 * dx) - dy;
	float x = p1.x;

	for (int y = p1.y; y < p2.y; y++) {
		Plot(Vector2(x, y), col);
		if (D > 0) {
			x += xi;
			D += (2 * (dx - dy));
		}
		else
		{
			D += 2 * dx;
		}
	}
}

void PlotLine(Vector2 p1, Vector2 p2, Color col) {
	if (abs(p2.y - p1.y) < abs(p2.x - p1.x)) {
		if (p1.x > p2.x) {
			PlotLineLow(p2, p1, col);
		}
		else {
			PlotLineLow(p1, p2, col);
		}
	}
	else {
		if (p1.y > p2.y) {
			PlotLineHigh(p2, p1, col);
		}
		else {
			PlotLineHigh(p1, p2, col);
		}
	}
}

void DrawLine(Vector2 p1, Vector2 p2, Color col, int thickness = 1) {
	bool steep = abs(p2.y - p1.y) > abs(p2.x - p1.x);

	if (steep) {
		p1 = Vector2::SwapPoints(p1.x, p1.y);
		p2 = Vector2::SwapPoints(p2.x, p2.y);
	}

	if (p1.x > p2.x) {
		Vector2 tempP1 = p1;
		p1 = p2;
		p2 = tempP1;
	}

	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;
	float gradient = dy / dx;

	if (dx == 0) {
		gradient = 1;
	}

	float xEnd = round(p1.x);
	float yEnd = p1.y + gradient * (xEnd - p1.x);
	float xGap = (p1.x + 0.5f) - floor(p1.x + 0.5f);

	float xpxl1 = xEnd;
	float ypxl1 = floor(yEnd);

	if (steep) {
		Plot(Vector2(ypxl1, xpxl1), Color(col.red, col.green, col.blue, (1 - (yEnd - floor(yEnd))) * xGap));
		Plot(Vector2(ypxl1 + 1, xpxl1), Color(col.red, col.green, col.blue, (yEnd - floor(yEnd)) * xGap));
	}
	else {
		Plot(Vector2(xpxl1, ypxl1), Color(col.red, col.green, col.blue, (1 - (yEnd - floor(yEnd))) * xGap));
		Plot(Vector2(xpxl1, ypxl1 + 1), Color(col.red, col.green, col.blue, (yEnd - floor(yEnd)) * xGap));
	}

	float yInter = yEnd + gradient;

	xEnd = round(p2.x);
	yEnd = p2.y + gradient * (xEnd - p2.x);
	xGap = (p2.x + 0.5f) - floor(p2.x + 0.5f);

	float xpxl2 = xEnd;
	float ypxl2 = floor(yEnd);

	if (steep) {
		Plot(Vector2(ypxl2, xpxl2), Color(col.red, col.green, col.blue, (1 - (yEnd - floor(yEnd))) * xGap));
		Plot(Vector2(ypxl2 + 1, xpxl2), Color(col.red, col.green, col.blue, (yEnd - floor(yEnd)) * xGap));
	}
	else {
		Plot(Vector2(xpxl2, ypxl2), Color(col.red, col.green, col.blue, (1 - (yEnd - floor(yEnd))) * xGap));
		Plot(Vector2(xpxl2, ypxl2 + 1), Color(col.red, col.green, col.blue, (yEnd - floor(yEnd)) * xGap));
	}

	if (steep) {
		for (int x = xpxl1; x < xpxl2; x++) {
			Plot(Vector2(floor(yInter), x), Color(col.red, col.green, col.blue, (1 - (yInter - floor(yInter)))));
			for (int n = 0; n < thickness; n++) {
				Plot(Vector2(floor(yInter) + n, x), Color(col.red, col.green, col.blue, (yInter - floor(yInter))));
			}
			yInter = yInter + gradient;
		}
	}
	else {
		for (int x = xpxl1; x < xpxl2; x++) {
			Plot(Vector2(x, floor(yInter)), Color(col.red, col.green, col.blue, (1 - (yInter - floor(yInter)))));
			for (int n = 0; n < thickness; n++) {
				Plot(Vector2(x, floor(yInter) + n), Color(col.red, col.green, col.blue, (yInter - floor(yInter))));
			}

			yInter = yInter + gradient;
		}
	}
}

void DrawRect(Vector2 position, Vector2 size, Color color, bool filled=false) {
	int x0 = Mathf::Clamp(position.x - size.x, 1, render_state.width - 1);
	int x1 = Mathf::Clamp(position.x + size.x, 1, render_state.width - 1);

	int y0 = Mathf::Clamp(position.y - size.y, 1, render_state.height - 1);
	int y1 = Mathf::Clamp(position.y + size.y, 1, render_state.height - 1);

	Vector2 TL = Vector2(x0, y0);
	Vector2 TR = Vector2(x1, y0);
	Vector2 BL = Vector2(x0, y1);
	Vector2 BR = Vector2(x1, y1);

	unsigned int pixelColor = Color::ColorToUINT(color);

	DrawLine(TL, TR, color);
	DrawLine(TR, BR, color);
	DrawLine(BR, BL, color);
	DrawLine(BL, TL, color);

	if (filled) {
		for (int y = y0; y < y1; y++) {
			unsigned int* pixel = (unsigned int*)render_state.memory + x0 + y * render_state.width;

			for (int x = x0; x < x1; x++) {
				*pixel++ = pixelColor;
			}
		}
	}
}

void DrawPoly(std::list<Vector2> points, Color color, int thickness = 1, bool connect = false) {
	Vector2 prevpoint = points.front();
	for (Vector2 p : points) {
		DrawLine(prevpoint, p, color, thickness);
		prevpoint = p;
	}
	if (connect) {
		DrawLine(points.front(), points.back(), color, thickness);
	}
}

void DrawCircle(Vector2 position, float radius, Color color,bool filled=false, int edgeRadius = 1) {
	Vector2 previousPoint = Vector2(position.x + cos(0) * radius, position.y + sin(0) * radius);

	for (float r = 0; r < (M_PI * 2); r += (M_PI / radius)) {
		int x = cos(r) * radius;
		int y = sin(r) * radius;

		int drawX = position.x + x;
		int drawY = position.y + y;

		DrawLine(Vector2(drawX, drawY), previousPoint, color,edgeRadius);
		previousPoint = Vector2(drawX, drawY);
	}

	if (filled) {

	}
}