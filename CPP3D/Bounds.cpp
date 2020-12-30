#include "Bounds.h"
Bounds::Bounds() {
	position = Vector2(0, 0);
	size = Vector2(100, 100);
}

Bounds::Bounds(Vector2 pos, Vector2 s) {
	position = pos;
	size = s;
}

bool Bounds::inBounds(Bounds check, Bounds in) {
	return false;
}

bool Bounds::pointInBounds(Vector2 point, Bounds in) {
	if (point.x <= (in.position.x + in.size.x) && point.x >= (in.position.x - in.size.x)) {
		if (point.y >= (in.position.y - in.size.y) && point.y <= (in.position.y + in.size.y)) {
			return true;
		}
	}

	return false;
}