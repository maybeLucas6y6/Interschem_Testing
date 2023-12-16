#include "readNode.h"

#include "raylib.h"
#include <cstring>
#include <iostream>

ReadNode* NewReadNode(int padding, int fontSize, float x, float y) {
	ReadNode* p = new ReadNode;
	p->id = -1;
	p->index = -1;
	char* temp = new char[5];
	strcpy(temp, "Read");
	p->label = temp;
	p->fontSize = 0;
	p->padding = 0;

	p->x = 0.0f;
	p->y = 0.0f;
	p->width = 0.0f;
	p->height = 0.0f;

	p->inPin.id = 0;
	p->inPin.x = 0.0f;
	p->inPin.y = 0.0f;
	p->inPin.radius = PIN_RADIUS;
	p->inPin.owner = p;
	p->inPin.ownerType = read;

	p->outPin.id = 0;
	p->outPin.x = 0.0f;
	p->outPin.y = 0.0f;
	p->outPin.radius = PIN_RADIUS;
	p->outPin.owner = p;
	p->outPin.ownerType = read;

	p->toPin = nullptr;

	SetReadNodePosition(p, x, y);
	SetReadNodeSize(p, padding, fontSize);

	return p;
}
void SetReadNodeSize(ReadNode* node, int padding, int fontSize) {
	node->padding = padding;
	node->fontSize = fontSize;
	node->width = MeasureText(node->label, fontSize) + 2 * padding;
	node->height = fontSize + 2 * padding;

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;

	node->outPin.x = node->x + node->width / 2.0f;
	node->outPin.y = node->y + node->height;
}
void SetReadNodePosition(ReadNode* node, float x, float y) {
	node->x = x;
	node->y = y;

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;

	node->outPin.x = node->x + node->width / 2.0f;
	node->outPin.y = node->y + node->height;
}
void DrawReadNode(ReadNode* node) {
	DrawRectangle(node->x, node->y, node->width, node->height, YELLOW);
	DrawText(node->label, node->x + node->padding, node->y + node->padding, node->fontSize, BLACK);
	DrawCircle(node->inPin.x, node->inPin.y, node->inPin.radius, GRAY);
	DrawCircle(node->outPin.x, node->outPin.y, node->outPin.radius, GRAY);
	DrawLink(node->outPin, node->toPin);
}
void ReadValue(ReadNode* node) {
	std::cin >> *(node->myVar);
}