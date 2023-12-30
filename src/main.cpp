#include <raylib.h>

#include "nodeComponents.h"
#include "uiComponents.h"
#include "execFunc.h"

#include <iostream>
using namespace std;

enum EditorState {
	EditorStateNormal,
	EditorStateSelectedNode,
	EditorStateAddingNode,
	EditorStateEditingNode,
	EditorStateAddingLink,
	EditorStateEditingLink,
	EditorStateLinkingVariable
};

int main() {
	InitWindow(1020, 800, "Interschem");

	AnyNodeType dragNode{ nullptr, noType };
	AnyNodeType selectedNode{ nullptr, noType };
	Pin* selectedPin = nullptr;

	EditorState edState = EditorStateNormal;

	Button* del = NewButton();
	SetButtonColors(del, RED, WHITE);
	SetButtonLabel(del, "Delete", 16, 5);
	Button* edit = NewButton();
	SetButtonColors(edit, BLUE, WHITE);
	SetButtonLabel(edit, "Edit", 16, 5);
	Button* linkVar = NewButton();
	SetButtonColors(linkVar, PURPLE, WHITE);
	SetButtonLabel(linkVar, "Link", 16, 5);

	Button* exec = NewButton();
	SetButtonColors(exec, GREEN, BLACK);
	SetButtonPosition(exec, 5, 5);
	SetButtonLabel(exec, "Execute", 20, 5);
	string inBuffer = "";
	string outputString = "";

	NodeArrays nodes;

	ExecutionState state = notExecuting;
	AnyNodeType currentNode{ nullptr, noType };

	Button* createStartNode = NewButton();
	SetButtonColors(createStartNode, { 0, 100, 32, 255 }, WHITE);
	SetButtonLabel(createStartNode, "New Start Node", 20, 5);
	Button* createReadNode = NewButton();
	SetButtonColors(createReadNode, { 210,100,100,255 }, WHITE);
	SetButtonLabel(createReadNode, "New Read Node", 20, 5);
	Button* createWriteNode = NewButton();
	SetButtonColors(createWriteNode, { 0,121,255,255 }, WHITE);
	SetButtonLabel(createWriteNode, "New Write Node", 20, 5);
	Button* createAssignNode = NewButton();
	SetButtonColors(createAssignNode, { 20,200,200,255 }, WHITE);
	SetButtonLabel(createAssignNode, "New Assign Node", 20, 5);
	Button* createDecisionNode = NewButton();
	SetButtonColors(createDecisionNode, { 80,20,200,255 }, WHITE);
	SetButtonLabel(createDecisionNode, "New Decision Node", 20, 5);
	Button* createStopNode = NewButton();
	SetButtonColors(createStopNode, { 155, 40, 60, 255 }, WHITE);
	SetButtonLabel(createStopNode, "New Stop Node", 20, 5);
	Window* createNodes = NewWindow();
	SetWindowColor(createNodes, { 66, 66, 66, 255 });
	SetWindowPosition(createNodes, 5, 40);
	SetWindowSpacing(createNodes, 5.0f);
	SetWindowPadding(createNodes, 5.0f);
	AddElementToWindow(createNodes, { createStartNode, WindowElementTypeButton });
	AddElementToWindow(createNodes, { createReadNode, WindowElementTypeButton });
	AddElementToWindow(createNodes, { createWriteNode, WindowElementTypeButton });
	AddElementToWindow(createNodes, { createAssignNode, WindowElementTypeButton });
	AddElementToWindow(createNodes, { createDecisionNode, WindowElementTypeButton });
	AddElementToWindow(createNodes, { createStopNode, WindowElementTypeButton });

	Dictionary* dict = NewDictionary();
	SetDictionaryPosition(dict, 100, 100);
	DictionaryRow* r1 = NewDictionaryRow();
	SetDictionaryRowData(r1, "abc", 1, 20, 5);
	DictionaryRow* r2 = NewDictionaryRow();
	SetDictionaryRowData(r2, "ab", 44, 20, 5);
	DictionaryRow* r3 = NewDictionaryRow();
	SetDictionaryRowData(r3, "abd", 3, 20, 5);
	DictionaryRow* r4 = NewDictionaryRow();
	SetDictionaryRowData(r4, "bruh", 420, 20, 5);
	AddDictionaryRow(dict, r1);
	AddDictionaryRow(dict, r2);
	AddDictionaryRow(dict, r3);
	AddDictionaryRow(dict, r4);
	SetDictionaryPadding(dict, 5);
	SetDictionarySpacing(dict, 5);
	Window* variables = NewWindow();
	SetWindowColor(variables, { 66, 66, 66, 255 });
	SetWindowPosition(variables, 5, 300);
	SetWindowSpacing(variables, 5.0f);
	SetWindowPadding(variables, 5.0f);
	AddElementToWindow(variables, { dict, WindowElementTypeDictionary });
	DictionaryRowHalf rhalf = { DictionaryRowHalfType_None, nullptr };
	DictionaryRow* selectedRow = nullptr;

	SingleLineText* inputLine = NewSingleLineText();
	SetSingleLineTextColors(inputLine, RAYWHITE, DARKGRAY);
	SetSingleLineTextFontSize(inputLine, 20);
	SetSingleLineTextPadding(inputLine, 5);
	SetSingleLineTextPosition(inputLine, 300, 5);

	/*string testName = "Test";
	int testVal = 5;
	NewNode(nodes, assign, 5, 20, 400, 400);*/
	//LinkAssignNodeVar(nodes.assignNodes[0], &testName, &testVal);

	SetTargetFPS(120);
	while (!WindowShouldClose()) {
		//double t = GetTime();

		if (IsKeyPressed(KEY_Q)) {
			EvaluateAssignNode(nodes.assignNodes[0], dict);
			//ResizeDictionaryRow(selectedRow);
			//selectedRow = nullptr;
		}
		for (AssignNode* p : nodes.assignNodes) {
			GetInputAssignNode(p);
		}

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			if (IsSingleLineTextClicked(inputLine)) {
				// TODO: should also have an 'active' variable
				inputLine->focused = true;
			}
			else {
				inputLine->focused = false;
			}
		}
		if (IsSingleLineTextFocused(inputLine)) {
			char c = GetCharPressed();
			if (rhalf.type == DictionaryRowHalfType_Key && ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
				InsertCharSingleLineText(inputLine, c);
			}
			else if (rhalf.type == DictionaryRowHalfType_Value && (c >= '0' && c <= '9')) {
				InsertCharSingleLineText(inputLine, c);
			}
			if (IsKeyPressed(KEY_ENTER)) {
				if (rhalf.type == DictionaryRowHalfType_Key) {
					int find = FindKeyInDictionary(dict, inputLine->str); // TODO: do it internally?
					if (find == -1) {
						SetDictionaryRowKey(selectedRow, inputLine->str);
						rhalf = { DictionaryRowHalfType_None, nullptr };
						selectedRow = nullptr;
						inputLine->focused = false;
					}
				}
				else if (rhalf.type == DictionaryRowHalfType_Value) {
					SetDictionaryRowValue(selectedRow, stoi(inputLine->str));
					rhalf = { DictionaryRowHalfType_None, nullptr };
					selectedRow = nullptr;
					inputLine->focused = false;
				}
				ClearStrSingleLineText(inputLine);
			}
			if (IsKeyPressed(KEY_BACKSPACE)) {
				EraseCharSingleLineText(inputLine);
			}
		}

		// update data
		int mx = GetMouseX(), my = GetMouseY();

		if (IsDictionaryRowClicked(dict)) {
			selectedRow = GetClickedDictionaryRow(dict);
			if (edState == EditorStateLinkingVariable) { // link read or write nodes var 
				if (selectedNode.type == read) {
					LinkReadNodeVar((ReadNode*)selectedNode.address, &selectedRow->key, &selectedRow->value);
				}
				else if (selectedNode.type == write) {
					LinkWriteNodeVar((WriteNode*)selectedNode.address, &selectedRow->value);
				}
				else if (selectedNode.type == assign) {
					LinkAssignNodeVar((AssignNode*)selectedNode.address, &selectedRow->key, &selectedRow->value);
					SetAssignNodeExpression(nodes.assignNodes[0], "sin(3.1415/2)"); //TODO: move this
				}
			}
			else {
				rhalf = GetClickedDictionaryRowHalf(selectedRow);
				if (rhalf.type != DictionaryRowHalfType_None) {
					SetSingleLineTextPosition(inputLine, selectedRow->dict->window->x + selectedRow->dict->window->width + 5, selectedRow->y);
					inputLine->focused = true;
				}
			}
		}

		DragWindow(createNodes);
		DragWindow(variables);

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			if (edState == EditorStateNormal) {
				GetClickedNode(dragNode, mx, my, nodes);
			}

			if (edState == EditorStateEditingNode) {
				GetClickedPin(selectedPin, mx, my, nodes);
				if (selectedPin != nullptr && selectedPin->type == output) {
					selectedNode.address = selectedPin->owner; //TODO: this should not be necessary
					selectedNode.type = selectedPin->ownerType;

					edState = EditorStateAddingLink;
				}
			}
			else if(edState == EditorStateAddingLink){
				Pin* secondPin = nullptr;
				GetClickedPin(secondPin, mx, my, nodes);
				if (secondPin != nullptr && secondPin->type == input && selectedPin != nullptr) {
					switch (selectedPin->ownerType) {
					case start: NewLink(((StartNode*)selectedPin->owner)->toPin, *secondPin); break;
					case read: NewLink(((ReadNode*)selectedPin->owner)->toPin, *secondPin); break;
					case write: NewLink(((WriteNode*)selectedPin->owner)->toPin, *secondPin); break;
					case assign: NewLink(((AssignNode*)selectedPin->owner)->toPin, *secondPin); break;
					case decision:
						if(selectedPin == &((DecisionNode*)selectedPin->owner)->outPinTrue)
							NewLink(((DecisionNode*)selectedPin->owner)->toPinTrue, *secondPin);
						else 
							NewLink(((DecisionNode*)selectedPin->owner)->toPinFalse, *secondPin);
						break;
					default: break;
					}
					selectedPin = nullptr;
					edState = EditorStateEditingNode;
				}
			}
		}
		else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			DragNode(dragNode, mx, my);
		}
		else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
			dragNode = { nullptr, noType }; // TODO: performance?
		}

		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
			GetClickedNode(selectedNode, mx, my, nodes);
			if (selectedNode.address != nullptr) {
				edState = EditorStateSelectedNode;
			}
			else {
				edState = EditorStateNormal;
			}
		}

		if (selectedNode.address != nullptr) {
			if (IsButtonClicked(edit)) {
				edState = EditorStateEditingNode;
			}
			if (IsButtonClicked(del)) {
				EraseNode(nodes, selectedNode);
				selectedNode = { nullptr, noType };
				edState = EditorStateNormal;
			}
			if (IsButtonClicked(linkVar)) {
				edState = EditorStateLinkingVariable;
			}
		}

		if (IsButtonClicked(exec)) {
			GetNextNodeInExecution(currentNode, state);
		}
		if (IsButtonClicked(createStartNode)) {
			currentNode = dragNode = NewNode(nodes, start, 5, 20, mx, my);
		}
		if (IsButtonClicked(createReadNode)) {
			dragNode = NewNode(nodes, read, 5, 20, mx, my);
		}
		if (IsButtonClicked(createWriteNode)) {
			dragNode = NewNode(nodes, write, 5, 20, mx, my);
		}
		if (IsButtonClicked(createAssignNode)) {
			dragNode = NewNode(nodes, assign, 5, 20, mx, my);
		}
		if (IsButtonClicked(createDecisionNode)) {
			dragNode = NewNode(nodes, decision, 5, 20, mx, my);
		}
		if (IsButtonClicked(createStopNode)) {
			dragNode = NewNode(nodes, stop, 5, 20, mx, my);
		}

		if (state == notExecuting) {

		}
		else if (state == waitingForInput) {
			cout << "Waiting for input\n";
			char c = GetCharPressed();
			if (c != 0 && (c >= '0' && c <= '9')) {
				inBuffer.insert(inBuffer.end(), c);
			}

			if (IsKeyPressed(KEY_ENTER)) {
				int x = stoi(inBuffer);
				ReadNode* p = (ReadNode*)currentNode.address;
				SetReadNodeVarValue(p, x);
				ResizeDictionaryRow(GetDictionaryRow(dict, *p->myVarName));
				GetNextNodeInExecution(currentNode, state);
			}
		}
		else if (state == processing) {
			cout << "Processing\n";
			if (currentNode.type == write) {
				cout << GetWriteNodeVarValue((WriteNode*)currentNode.address) << "\n";
			}
			GetNextNodeInExecution(currentNode, state);
		}
		else {
			cout << "Done\n";
			state = notExecuting;
			outputString = "Result: " + to_string(*nodes.writeNodes[0]->myVarValue);
			currentNode = { nodes.startNode, start };
		}

		BeginDrawing();
		// render on screen
		ClearBackground({ 33, 33, 33 });

		DrawWindow(createNodes);
		DrawWindow(variables);

		if (edState == EditorStateAddingLink) {
			DrawGhostLink(selectedPin, mx, my);
		}
		if (edState != EditorStateNormal) {
			DrawSelectedNodeOptions(selectedNode, del, edit, linkVar);
		}

		DrawNodes(nodes);

		DrawButton(exec);

		if (state == waitingForInput) {
			DrawText(inBuffer.c_str(), 100, 5, 20, WHITE);
		}
		else if (!outputString.empty()) {
			DrawText(outputString.c_str(), 100, 5, 20, WHITE);
		}

		if (inputLine->focused) {
			DrawSingleLineText(inputLine);
		}

		EndDrawing();
		//t = GetTime() - t;
		//cout << t << "\n";
	}

	CleanupNodes(nodes);
	CloseWindow();
	return 0;
}