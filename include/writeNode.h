#pragma once

#include "pin.h"

struct WriteNode {
	int id, index;
	char* label;
	int fontSize, padding;
	float x, y, width, height;
	Pin inPin;
	Pin outPin;
};

WriteNode* NewWriteNode(int padding, int fontSize, float x, float y);
void SetWriteNodeSize(WriteNode* node, int padding, int fontSize);
void SetWriteNodePosition(WriteNode* node, float x, float y);
void DrawWriteNode(WriteNode* node);