#pragma once

struct StopNode {
	int id, index;
	char* label;
	int fontSize, padding;
	float x, y, width, height;
	Pin inPin;
};

void SetStopNodeSize(StopNode* node, int padding, int fontSize);
void SetStopNodePosition(StopNode* node, float x, float y);
void DrawStopNode(StopNode* node);
StopNode* NewStopNode(int padding, int fontSize, float x, float y);