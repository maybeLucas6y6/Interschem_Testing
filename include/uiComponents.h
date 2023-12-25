#pragma once

#include <cstring>
#include <raylib.h>
#include <string>
#include <vector>

struct Button {
	std::string label;
	int fontSize, padding;
	Color bgColor, textColor;
	float x, y, width, height;
};

enum WindowElementType {
	WindowElementTypeButton,
	WindowElementTypeDictionary, // only <string, int> for now
};

struct WindowElement {
	void* ptr;
	WindowElementType type;
};

struct Window {
	int x, y, width, height; // TODO: int/float?
	float padding, spacing;
	std::vector<WindowElement> elements;
	Color bgColor;
	bool focused;
	int dx, dy;
	bool dragging;
};

struct Popup {

};

struct SingleLineText {

};

struct MultiLinetext {

};

struct DictionaryRow {
	std::string key;
	int value;
	int x, y, width, height;
	int fontSize, padding;
	Color bgColor, fontColor;
};

struct Dictionary {
	int x, y, width, height;
	int padding, spacing;
	std::vector<DictionaryRow*> rows; //TODO: or list, or map
};

Button* NewButton() {
	Button* p = new Button;
	p->label = "";
	p->fontSize = -1;
	p->padding = -1;
	p->x = -1;
	p->y = -1;
	p->width = -1;
	p->height = -1;
	return p;
}
void SetButtonColors(Button* p, Color bgColor, Color textColor) {
	p->bgColor = bgColor;
	p->textColor = textColor;
}
void SetButtonLabel(Button* p, const char* label, int fontSize, int padding) { // TODO: replace const char* with string& ?? 
	p->label = label;
	p->fontSize = fontSize;
	p->padding = padding;
	p->width = MeasureText(label, fontSize) + 2 * padding;
	p->height = fontSize + 2 * padding;
}
void SetButtonPosition(Button* p, float x, float y) {
	p->x = x;
	p->y = y;
}
bool IsButtonClicked(Button* p) {
	int mx = GetMouseX(), my = GetMouseY();
	return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mx >= p->x && mx <= p->x + p->width && my >= p->y && my <= p->y + p->height;
}
void DrawButton(Button* p) {
	DrawRectangle(p->x, p->y, p->width, p->height, p->bgColor);
	DrawText(p->label.c_str(), p->x + p->padding, p->y + p->padding, p->fontSize, p->textColor);
}

DictionaryRow* NewDictionaryRow() {
	DictionaryRow* p = new DictionaryRow;
	p->key = "";
	p->value = 0;
	p->x = 0;
	p->y = 0;
	p->width = 0;
	p->height = 0;
	p->padding = 0;
	p->bgColor = BLACK;
	p->fontColor = WHITE;
	return p;
}
void SetDictionaryRowData(DictionaryRow* drow, std::string key, int value, int fontSize, int padding) {
	drow->key = key;
	drow->value = value;
	drow->fontSize = fontSize;
	drow->padding = padding;
	std::string strval = std::to_string(value);
	drow->width = 2 * padding + 10 + MeasureText(key.c_str(), fontSize) + MeasureText(strval.c_str(), fontSize);
	drow->height = 2 * padding + fontSize;
}
void SetDictionaryRowPosition(DictionaryRow* drow, int x, int y) {
	drow->x = x;
	drow->y = y;
}
void SetDictionaryRowColors(DictionaryRow* drow, Color bgColor, Color fontColor) {
	drow->bgColor = bgColor;
	drow->fontColor = fontColor;
}
bool IsDictionaryRowHovered(DictionaryRow* drow) {
	int mx = GetMouseX(), my = GetMouseY();
	return mx >= drow->x && mx <= drow->x + drow->width && my >= drow->y && my <= drow->y + drow->height;
}
bool IsDictionaryRowClicked(DictionaryRow* drow) {
	return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsDictionaryRowHovered(drow);
}
void DrawDictionaryRow(DictionaryRow* drow) {
	std::string strval = std::to_string(drow->value);
	int keyWidth = MeasureText(drow->key.c_str(), drow->fontSize);
	DrawRectangle(drow->x, drow->y, drow->width, drow->height, drow->bgColor);
	DrawText(drow->key.c_str(), drow->x + drow->padding, drow->y + drow->padding, drow->fontSize, drow->fontColor);
	DrawText(strval.c_str(), drow->x + drow->padding + keyWidth + 10, drow->y + drow->padding, drow->fontSize, drow->fontColor);
}

Dictionary* NewDictionary() {
	Dictionary* p = new Dictionary;
	p->x = 0;
	p->y = 0;
	p->width = 0;
	p->height = 0;
	p->padding = 0;
	p->spacing = 0;
	return p;
}
void SetDictionaryPosition(Dictionary* dict, int x, int y) {
	int dx = x - dict->x, dy = y - dict->y;
	dict->x = x;
	dict->y = y;
	for (DictionaryRow* r : dict->rows) {
		SetDictionaryRowPosition(r, r->x + dx, r->y + dy);
	}
}
int AddDictionaryRow(Dictionary* dict, DictionaryRow* drow) {
	if (dict->rows.empty()) {
		dict->height += drow->height;
		SetDictionaryRowPosition(drow, dict->x + dict->padding, dict->y + dict->padding);
		dict->rows.push_back(drow);
		return 0;
	}

	size_t n = dict->rows.size();
	int left = 0, right = n - 1;
	int pos = 0;
	while (left <= right) {
		int middle = (left + right) / 2;
		pos = middle;
		if (drow->key.compare(dict->rows[middle]->key) < 0) {
			right = middle - 1;
		}
		else if (drow->key.compare(dict->rows[middle]->key) > 0) {
			left = middle + 1;
		}
		else {
			return - 1; // TODO: throw error or smth, can t have two identical keys
			/*
			or replace the values
			delete dict->data[middle];
			dict->data[middle] = drow;
			*/
		}
	}
	// 1 4 5 6 8 10
	// 0 1 2 3 4  5
	// l   m      r
	//       l m  r
	//            b
	if (drow->key.compare(dict->rows[pos]->key) < 0) {
		SetDictionaryRowPosition(drow, dict->rows[pos]->x, dict->rows[pos]->y);
		for (int i = pos; i < n; i++) {
			SetDictionaryRowPosition(dict->rows[i], dict->rows[i]->x, dict->rows[i]->y + drow->height + dict->spacing);
		}
		dict->rows.insert(dict->rows.begin() + pos, drow);
	}
	else {
		for (int i = pos + 1; i < n; i++) {
			SetDictionaryRowPosition(dict->rows[i], dict->rows[i]->x, dict->rows[i]->y + drow->height + dict->spacing);
		}
		SetDictionaryRowPosition(drow, dict->x + dict->padding, dict->y + dict->height - dict->padding + dict->spacing);
		dict->rows.insert(dict->rows.begin() + pos + 1, drow);
	}
	if (drow->width + 2 * dict->padding > dict->width) {
		dict->width = drow->width + 2 * dict->padding;
	}
	dict->height += drow->height + dict->spacing;
	return pos;
}
int FindKeyInDictionary(Dictionary* dict, std::string key) {
	size_t n = dict->rows.size();
	int left = 0, right = n - 1;
	while (left <= right) {
		int middle = (left + right) / 2;
		if (key.compare(dict->rows[middle]->key) < 0) {
			right = middle - 1;
		}
		else if (key.compare(dict->rows[middle]->key) > 0) {
			left = middle + 1;
		}
		else {
			return middle;
		}
	}
	return -1;
}
int GetValueFromDictionary(Dictionary* dict, int index) {
	if (index != -1) {
		return dict->rows[index]->value;
	}
}
bool IsDictionaryHovered(Dictionary* dict) {
	int mx = GetMouseX(), my = GetMouseY();
	return mx >= dict->x && mx <= dict->x + dict->width && my >= dict->y && my <= dict->y + dict->height;
}
bool IsDictionaryClicked(Dictionary* dict) {
	for (DictionaryRow* r : dict->rows) {
		if (IsDictionaryRowClicked(r)) {
			return true;
		}
	}
	return false;
}
void SetDictionaryPadding(Dictionary* dict, int padding) {
	dict->width -= 2 * dict->padding;
	dict->width += 2 * padding;
	int height = padding;
	for (DictionaryRow* r : dict->rows) {
		SetDictionaryRowPosition(r, r->x - dict->padding + padding, dict->y + height);
		height += r->height + dict->spacing;
	}
	if (!dict->rows.empty()) {
		height -= dict->spacing;
	}
	height += padding;
	dict->height = height;
	dict->padding = padding;
}
void SetDictionarySpacing(Dictionary* dict, int spacing) {
	float height = dict->padding;
	for (DictionaryRow* r : dict->rows) {
			SetDictionaryRowPosition(r, r->x, dict->y + height);
			height += r->height + spacing;
	}
	if (!dict->rows.empty()) {
		height -= spacing;
	}
	height += dict->padding;
	dict->height = height;
	dict->spacing = spacing;
}
void DrawDictionary(Dictionary* dict) {
	DrawRectangle(dict->x, dict->y, dict->width, dict->height, RAYWHITE); // TODO:
	for (DictionaryRow* r : dict->rows) {
		DrawDictionaryRow(r);
	}
}

Window* NewWindow() {
	Window* p = new Window;
	p->x = 0;
	p->y = 0;
	p->width = 0;
	p->height = 0;
	p->padding = 0.0f;
	p->spacing = 0.0f;
	p->bgColor = WHITE;
	p->focused = false;
	p->dx = 0;
	p->dy = 0;
	p->dragging = false;
	return p;
}
void SetWindowColor(Window* win, Color bgColor) {
	win->bgColor = bgColor;
}
void SetWindowPosition(Window* win, int x, int y) {
	int dx = x - win->x, dy = y - win->y;
	for (WindowElement& e : win->elements) {
		if (e.type == WindowElementTypeButton) {
			Button* b = (Button*)e.ptr;
			SetButtonPosition(b, b->x + dx, b->y + dy);
		}
		else if (e.type = WindowElementTypeDictionary) {
			Dictionary* d = (Dictionary*)e.ptr;
			SetDictionaryPosition(d, d->x + dx, d->y + dy);
		}
	}
	win->x = x;
	win->y = y;
}
void SetWindowPadding(Window* win, float padding) {
	win->width -= 2 * win->padding;
	win->width += 2 * padding;
	float height = padding;
	for (WindowElement& e : win->elements) {
		if (e.type == WindowElementTypeButton) {
			Button* b = (Button*)e.ptr;
			SetButtonPosition(b, b->x - win->padding + padding, win->y + height);
			height += b->height + win->spacing;
		}
		else if (e.type == WindowElementTypeDictionary) {
			Dictionary* d = (Dictionary*)e.ptr;
			SetDictionaryPosition(d, d->x - win->padding + padding, win->y + height);
			height += d->height + win->spacing;
		}
	}
	if (!win->elements.empty()) {
		height -= win->spacing;
	}
	height += padding;
	win->height = height;
	win->padding = padding;
}
void SetWindowSpacing(Window* win, float spacing) {
	float height = win->padding;
	for (WindowElement& e : win->elements) {
		if (e.type == WindowElementTypeButton) {
			Button* b = (Button*)e.ptr;
			SetButtonPosition(b, b->x, win->y + height);
			height += b->height + spacing;
		}
		else if (e.type == WindowElementTypeDictionary) {
			Dictionary* d = (Dictionary*)e.ptr;
			SetDictionaryPosition(d, d->x, win->y + height);
			height += d->height + spacing;
		}
	}
	if (!win->elements.empty()) {
		height -= spacing;
	}
	height += win->padding;
	win->height = height;
	win->spacing = spacing;
}
void AddElementToWindow(Window* win, WindowElement el) {
	if (el.type == WindowElementTypeButton) {
		Button* b = (Button*)el.ptr;
		if (!win->elements.empty()) {
			SetButtonPosition(b, win->x + win->padding, win->y + win->height - win->padding + win->spacing);
		}
		else {
			SetButtonPosition(b, win->x + win->padding, win->y + win->height - win->padding);
		}
		if (b->width + 2 * win->padding > win->width) {
			win->width = b->width + 2 * win->padding;
		}
		if (!win->elements.empty()) {
			win->height += win->spacing;
		}
		win->height += b->height;
	}
	else if (el.type == WindowElementTypeDictionary) {
		Dictionary* d = (Dictionary*)el.ptr;
		if (!win->elements.empty()) {
			SetDictionaryPosition(d, win->x + win->padding, win->y + win->height - win->padding + win->spacing);
		}
		else {
			SetDictionaryPosition(d, win->x + win->padding, win->y + win->height - win->padding);
		}
		if (d->width + 2 * win->padding > win->width) {
			win->width = d->width + 2 * win->padding;
		}
		if (!win->elements.empty()) {
			win->height += win->spacing;
		}
		win->height += d->height;
	}
	win->elements.push_back(el); //TODO: validate
}
void DrawWindow(Window* win) {
	DrawRectangle(win->x, win->y, win->width, win->height, win->bgColor);
	for (WindowElement& e : win->elements) {
		if (e.type == WindowElementTypeButton) {
			DrawButton((Button*)e.ptr);
		}
		else if (e.type == WindowElementTypeDictionary) {
			DrawDictionary((Dictionary*)e.ptr);
		}
	}
}
bool IsWindowFocused(Window* win) {
	return win->focused;
}
bool IsWindowHovered(Window* win) {
	int mx = GetMouseX(), my = GetMouseY();
	return mx >= win->x && mx <= win->x + win->width && my >= win->y && my <= win->y + win->height;
}
bool IsWindowClicked(Window* win) {
	return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsWindowHovered(win);
}
bool IsWindowElementClicked(Window* win) {
	for (WindowElement& e : win->elements) {
		if (e.type == WindowElementTypeButton) {
			if (IsButtonClicked((Button*)e.ptr)) {
				return true;
			}
		}
		else if (e.type == WindowElementTypeDictionary) {
			if (IsDictionaryClicked((Dictionary*)e.ptr)) {
				return true;
			}
		}
	}
	return false;
}
void DragWindow(Window* win) {
	int mx = GetMouseX(), my = GetMouseY();
	if (IsWindowClicked(win) && !IsWindowElementClicked(win)) {
		win->dx = mx - win->x;
		win->dy = my - win->y;
		win->dragging = true;
	}
	else if (win->dragging && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		SetWindowPosition(win, mx - win->dx, my - win->dy);
	}
	else if (win->dragging && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
		win->dragging = false;
	}
}