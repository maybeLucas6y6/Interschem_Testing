#pragma once

#include "pin.h"

struct ReadNode {
	int id, index;
	char* label;
	int fontSize, padding;
	float x, y, width, height;
	Pin inPin;
	Pin outPin;

	int* myVar;
};

ReadNode* NewReadNode(int padding, int fontSize, float x, float y);
void SetReadNodeSize(ReadNode* node, int padding, int fontSize);
void SetReadNodePosition(ReadNode* node, float x, float y);
void DrawReadNode(ReadNode* node);
void ReadValue(ReadNode* node);