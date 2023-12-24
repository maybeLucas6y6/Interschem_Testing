#include <raylib.h>

#include "nodeComponents.h"
#include "uiComponents.h"
#include "execFunc.h"

#include <iostream>
using namespace std;

enum EditorState {
	EditorStateNormal,
	EditorStateSelectedNode,
	EditorStateAddingNode, // TODO:
	EditorStateEditingNode,
	EditorStateAddingLink,
	EditorStateEditingLink // TODO:
};

int main() {
	InitWindow(1020, 800, "Interschem");

	AnyNodeType dragNode{ nullptr, noType };
	AnyNodeType selectedNode{ nullptr, noType };
	Pin* selectedPin = nullptr;

	EditorState edState = EditorStateNormal;

	Button* createStartNode = NewButton();
	SetButtonColors(createStartNode, DARKGREEN, WHITE);
	SetButtonLabel(createStartNode, "New Start Node", 20, 5);
	Button* createReadNode = NewButton();
	SetButtonColors(createReadNode, YELLOW, BLACK);
	SetButtonLabel(createReadNode, "New Read Node", 20, 5);
	Button* createWriteNode = NewButton();
	SetButtonColors(createWriteNode, BLUE, WHITE);
	SetButtonLabel(createWriteNode, "New Write Node", 20, 5);
	Button* createAssignNode = NewButton();
	SetButtonColors(createAssignNode, ORANGE, BLACK);
	SetButtonLabel(createAssignNode, "New Assign Node", 20, 5);
	Button* createDecisionNode = NewButton();
	SetButtonColors(createDecisionNode, PURPLE, WHITE);
	SetButtonLabel(createDecisionNode, "New Decision Node", 20, 5);
	Button* createStopNode = NewButton();
	SetButtonColors(createStopNode, RED, WHITE);
	SetButtonLabel(createStopNode, "New Stop Node", 20, 5);

	Button* del = NewButton();
	SetButtonColors(del, RED, WHITE);
	SetButtonLabel(del, "Delete", 16, 5);
	Button* edit = NewButton();
	SetButtonColors(edit, BLUE, WHITE);
	SetButtonLabel(edit, "Edit", 16, 5);

	Button* exec = NewButton();
	SetButtonColors(exec, GREEN, BLACK);
	SetButtonPosition(exec, 5, 5);
	SetButtonLabel(exec, "Execute", 20, 5);
	string inputString = "";
	string outputString = "";

	NodeArrays nodes;

	ExecutionState state = notExecuting;
	AnyNodeType currentNode{ nodes.startNode, start };

	Window* createNodes = NewWindow();
	SetWindowColor(createNodes, DARKGRAY);
	SetWindowPosition(createNodes, 300, 300);
	AddElementToWindow(createNodes, { (void*)createStartNode, WindowElementTypeButton });
	AddElementToWindow(createNodes, { (void*)createReadNode, WindowElementTypeButton });
	AddElementToWindow(createNodes, { (void*)createWriteNode, WindowElementTypeButton });
	AddElementToWindow(createNodes, { (void*)createAssignNode, WindowElementTypeButton });
	AddElementToWindow(createNodes, { (void*)createDecisionNode, WindowElementTypeButton });
	AddElementToWindow(createNodes, { (void*)createStopNode, WindowElementTypeButton });
	SetWindowSpacing(createNodes, 5.0f);
	SetWindowPadding(createNodes, 5.0f);

	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		// update data
		int mx = GetMouseX(), my = GetMouseY();

		if (IsKeyPressed(KEY_F)) {
			SetWindowPosition(createNodes, mx, my);
		}

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
						if(secondPin == &((DecisionNode*)selectedPin->owner)->outPinTrue)
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

		if (selectedNode.address != nullptr && IsButtonClicked(edit)) {
			edState = EditorStateEditingNode;
		}

		if (IsButtonClicked(exec)) {
			GetNextNodeInExecution(currentNode, state);
		}
		if (IsButtonClicked(createStartNode)) {
			NewNode(nodes, start, 5, 20, mx, my);
			dragNode = { nodes.startNode, start };
		}
		if (IsButtonClicked(createReadNode)) {
			NewNode(nodes, read, 5, 20, mx, my);
			dragNode = { nodes.readNodes[nodes.readNodes.size() - 1], read };
		}
		if (IsButtonClicked(createWriteNode)) {
			NewNode(nodes, write, 5, 20, mx, my);
			dragNode = { nodes.writeNodes[nodes.writeNodes.size() - 1], write };
		}
		if (IsButtonClicked(createAssignNode)) {
			NewNode(nodes, assign, 5, 20, mx, my);
			dragNode = { nodes.assignNodes[nodes.assignNodes.size() - 1], assign };
		}
		if (IsButtonClicked(createDecisionNode)) {
			NewNode(nodes, decision, 5, 20, mx, my);
			dragNode = { nodes.decisionNodes[nodes.decisionNodes.size() - 1], decision };
		}
		if (IsButtonClicked(createStopNode)) {
			NewNode(nodes, stop, 5, 20, mx, my);
			dragNode = { nodes.stopNodes[nodes.stopNodes.size() - 1], stop };
		}

		if (state == notExecuting) {

		}
		else if (state == waitingForInput) {
			cout << "Waiting for input\n";
			char c = GetCharPressed();
			if (c != 0 && (c >= '0' && c <= '9')) {
				inputString.insert(inputString.end(), c);
			}

			if (IsKeyPressed(KEY_ENTER)) {
				//x = stoi(inputString);
				//SetValue((ReadNode*)currentNode.address, x);
				GetNextNodeInExecution(currentNode, state);
			}
		}
		else if (state == processing) {
			cout << "Processing\n";
			if (currentNode.type == write) {
				WriteValue((WriteNode*)currentNode.address);
			}
			GetNextNodeInExecution(currentNode, state);
		}
		else {
			cout << "Done\n";
			state = notExecuting;
			outputString = "Result: " + to_string(*nodes.writeNodes[0]->myVar);
		}

		BeginDrawing();
		// render on screen
		ClearBackground(BLACK);

		DrawWindow(createNodes);

		if (edState == EditorStateAddingLink) {
			DrawGhostLink(selectedPin, mx, my);
		}
		if (edState == EditorStateSelectedNode || edState == EditorStateEditingNode|| edState == EditorStateAddingLink) {
			DrawSelectedNodeOptions(selectedNode, del, edit);
		}

		DrawNodes(nodes);

		DrawButton(exec);

		if (state == waitingForInput) {
			DrawText(inputString.c_str(), 100, 5, 20, WHITE);
		}
		else if (!outputString.empty()) {
			DrawText(outputString.c_str(), 100, 5, 20, WHITE);
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}