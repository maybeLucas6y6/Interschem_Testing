#include "assignNode.h"

#include "raylib.h"
#include <string>
#include "evalExpression.h"

#include <iostream>

AssignNode* NewAssignNode(int padding, int fontSize, float x, float y) {
	AssignNode* p = new AssignNode;
	p->id = -1;
	p->fontSize = 0;
	p->padding = 0;

	p->varName = NewSingleLineText();
	SetSingleLineTextColors(p->varName, { 100, 100, 100, 70 }, WHITE);
	SetSingleLineTextPadding(p->varName, 5);
	SetSingleLineTextFontSize(p->varName, 20);
	SetSingleLineTextPosition(p->varName, 0, 0);

	p->expression = NewSingleLineText();
	SetSingleLineTextColors(p->expression, { 100, 100, 100, 70 }, WHITE);
	SetSingleLineTextPadding(p->expression, 5);
	SetSingleLineTextFontSize(p->expression, 20);
	SetSingleLineTextPosition(p->expression, 0, 0);

	p->x = 0.0f;
	p->y = 0.0f;
	p->width = 0.0f;
	p->height = 0.0f;

	p->inPin.id = 0;
	p->inPin.type = input;
	p->inPin.x = 0.0f;
	p->inPin.y = 0.0f;
	p->inPin.radius = PIN_RADIUS;
	p->inPin.owner = p;
	p->inPin.ownerType = assign;

	p->outPin.id = 0;
	p->outPin.type = output;
	p->outPin.x = 0.0f;
	p->outPin.y = 0.0f;
	p->outPin.radius = PIN_RADIUS;
	p->outPin.owner = p;
	p->outPin.ownerType = assign;

	p->toPin = nullptr;

	SetAssignNodePosition(p, x, y);
	SetAssignNodeSize(p, padding, fontSize);

	return p;
}
void SetAssignNodeSize(AssignNode* node, int padding, int fontSize) {
	node->padding = padding;
	node->fontSize = fontSize;
	node->width = MeasureText(node->varName->str.c_str(), fontSize) + MeasureText(node->expression->str.c_str(), fontSize) + MeasureText(" = ", node->fontSize) + 2 * padding + 4 * 5;
	node->height = fontSize + 2 * padding + 2 * 5;

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;

	node->outPin.x = node->x + node->width / 2.0f;
	node->outPin.y = node->y + node->height;

	SetSingleLineTextPosition(node->varName, node->x + node->padding, node->y + node->padding);
	int varNameWidth = MeasureText(node->varName->str.c_str(), node->fontSize);
	SetSingleLineTextPosition(node->expression, node->x + varNameWidth + node->padding + MeasureText(" = ", node->fontSize) + 2 * 5, node->y + node->padding);
}
void SetAssignNodePosition(AssignNode* node, int x, int y) {
	node->x = x;
	node->y = y;

	node->inPin.x = node->x + node->width / 2.0f;
	node->inPin.y = node->y;

	node->outPin.x = node->x + node->width / 2.0f;
	node->outPin.y = node->y + node->height;

	SetSingleLineTextPosition(node->varName, node->x + node->padding, node->y + node->padding);
	int varNameWidth = MeasureText(node->varName->str.c_str(), node->fontSize);
	SetSingleLineTextPosition(node->expression, node->x + varNameWidth + node->padding + MeasureText(" = ", node->fontSize) + 2 * 5, node->y + node->padding);
}
void DrawAssignNode(AssignNode* node) {
	DrawRectangle(node->x, node->y, node->width, node->height, ORANGE);
	DrawSingleLineText(node->varName);
	DrawText(" = ", node->x + node->padding + node->varName->width, node->y + node->padding + 5, node->fontSize, WHITE); // s ar putea sa nu mai mearga atunci cand schimbi padding ul de la varname
	DrawSingleLineText(node->expression);
	DrawCircle(node->inPin.x, node->inPin.y, node->inPin.radius, GRAY);
	DrawCircle(node->outPin.x, node->outPin.y, node->outPin.radius, GRAY);
	DrawLink(node->outPin, node->toPin);

	// +----+
	// |    |
	// +----+

	// +------------------+
	// | a = sin(6) + 5*x |
	// +------------------+

	// +-------+
	// | a = 2 |
	// +-------+
}
void LinkAssignNodeVar(AssignNode* node, std::string* name, int* val) {
	
}
void SetAssignNodeExpression(AssignNode* node, std::string expression) {
	
}
void EvaluateAssignNode(AssignNode* node, Dictionary* dict) {
	int err = 0;
	float result = evaluate(node->expression->str, err);
	if (err != 0) {
		// TODO: popup
	}
	else {
		auto drow = GetDictionaryRow(dict, node->varName->str);
		if (drow != nullptr) {
			SetDictionaryRowValue(drow, result);
		}
	}
}
void ResizeAssignNode(AssignNode* node) {
	int varNameWidth = MeasureText(node->varName->str.c_str(), node->fontSize);
	int expressionWidth = MeasureText(node->expression->str.c_str(), node->fontSize);
	int eqWidth = MeasureText(" = ", node->fontSize);
	node->width = 2 * node->padding + eqWidth + varNameWidth + expressionWidth + 4 * 5;
	SetSingleLineTextPosition(node->expression, node->x + varNameWidth + node->padding + eqWidth + 2 * 5, node->y + node->padding);

	node->inPin.x = node->x + node->width / 2.0f;

	node->outPin.x = node->x + node->width / 2.0f;
}
void GetInputAssignNode(AssignNode* node) {
	//std::cout << node->varName << " " << node->expression << "\n";
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		node->varName->focused = IsSingleLineTextClicked(node->varName);
		node->expression->focused = IsSingleLineTextClicked(node->expression);
	}

	if (node->varName->focused) {
		char c = GetCharPressed();
		if (c != 0) {
			InsertCharSingleLineText(node->varName, c);
			ResizeAssignNode(node);
		}
	}
	if (node->expression->focused) {
		char c = GetCharPressed();
		if (c != 0) {
			InsertCharSingleLineText(node->expression, c);
			ResizeAssignNode(node);
		}
	}

	if (IsKeyPressed(KEY_BACKSPACE)) {
		if (node->varName->focused) {
			EraseCharSingleLineText(node->varName);
		}
		if (node->expression->focused) {
			EraseCharSingleLineText(node->expression);
		}
		ResizeAssignNode(node);
	}
}