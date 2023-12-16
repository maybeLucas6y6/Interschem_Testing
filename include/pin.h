#pragma once

#ifndef PIN_RADIUS
#define PIN_RADIUS 4.0f
#endif // !PIN_RADIUS

struct Pin {
	int id; // TODO: int or unsigned?
	float x, y;
	float radius;
};