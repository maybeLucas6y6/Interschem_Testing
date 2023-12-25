#pragma once

#include "nodeComponents.h"
#include "uiComponents.h"

enum ExecutionState {
	notExecuting, // TODO: find a better name?
	processing,
	waitingForInput,
	done
};

void GetClickedNode(AnyNodeType& clickedNode, int mx, int my, NodeArrays& nodes) {
	if (nodes.startNode != nullptr) {
		if (mx >= nodes.startNode->x && mx <= nodes.startNode->x + nodes.startNode->width && my >= nodes.startNode->y && my <= nodes.startNode->y + nodes.startNode->height) {
			clickedNode.address = nodes.startNode;
			clickedNode.type = start;
			return;
		}
	}
	for (unsigned i = 0; i < nodes.readNodes.size(); i++) {
		if (mx >= nodes.readNodes[i]->x && mx <= nodes.readNodes[i]->x + nodes.readNodes[i]->width && my >= nodes.readNodes[i]->y && my <= nodes.readNodes[i]->y + nodes.readNodes[i]->height) {
			clickedNode.address = nodes.readNodes[i];
			clickedNode.type = read;
			return;
		}
	}
	for (unsigned i = 0; i < nodes.writeNodes.size(); i++) {
		if (mx >= nodes.writeNodes[i]->x && mx <= nodes.writeNodes[i]->x + nodes.writeNodes[i]->width && my >= nodes.writeNodes[i]->y && my <= nodes.writeNodes[i]->y + nodes.writeNodes[i]->height) {
			clickedNode.address = nodes.writeNodes[i];
			clickedNode.type = write;
			return;
		}
	}
	for (unsigned i = 0; i < nodes.assignNodes.size(); i++) {
		if (mx >= nodes.assignNodes[i]->x && mx <= nodes.assignNodes[i]->x + nodes.assignNodes[i]->width && my >= nodes.assignNodes[i]->y && my <= nodes.assignNodes[i]->y + nodes.assignNodes[i]->height) {
			clickedNode.address = nodes.assignNodes[i];
			clickedNode.type = assign;
			return;
		}
	}
	for (unsigned i = 0; i < nodes.decisionNodes.size(); i++) {
		if (mx >= nodes.decisionNodes[i]->x && mx <= nodes.decisionNodes[i]->x + nodes.decisionNodes[i]->width && my >= nodes.decisionNodes[i]->y && my <= nodes.decisionNodes[i]->y + nodes.decisionNodes[i]->height) {
			clickedNode.address = nodes.decisionNodes[i];
			clickedNode.type = decision;
			return;
		}
	}
	for (unsigned i = 0; i < nodes.stopNodes.size(); i++) {
		if (mx >= nodes.stopNodes[i]->x && mx <= nodes.stopNodes[i]->x + nodes.stopNodes[i]->width && my >= nodes.stopNodes[i]->y && my <= nodes.stopNodes[i]->y + nodes.stopNodes[i]->height) {
			clickedNode.address = nodes.stopNodes[i];
			clickedNode.type = stop;
			return;
		}
	}

	clickedNode = { nullptr, noType };
}

void GetNextNodeInExecution(AnyNodeType& currentNode, ExecutionState& state) {
	if (currentNode.address == nullptr || currentNode.type == noType) {
		state = done; // TODO: add error variant
		return;
	}

	switch (currentNode.type) {
	case start:
		currentNode.type = ((StartNode*)currentNode.address)->toPin->ownerType;
		currentNode.address = ((StartNode*)currentNode.address)->toPin->owner;
		break;
	case read:
		currentNode.type = ((ReadNode*)currentNode.address)->toPin->ownerType;
		currentNode.address = ((ReadNode*)currentNode.address)->toPin->owner;
		break;
	case write:
		currentNode.type = ((WriteNode*)currentNode.address)->toPin->ownerType;
		currentNode.address = ((WriteNode*)currentNode.address)->toPin->owner;
		break;
	case assign:
		currentNode.type = ((AssignNode*)currentNode.address)->toPin->ownerType;
		currentNode.address = ((AssignNode*)currentNode.address)->toPin->owner;
		break;
	case decision:
		// TODO: 
		// TODO: choose branch according to condition

		//currentNode.type = ((DecisionNode*)currentNode.address)->toPinTrue->ownerType;
		//currentNode.address = ((DecisionNode*)currentNode.address)->toPinTrue->owner;

		//currentNode.type = ((DecisionNode*)currentNode.address)->toPinFalse->ownerType;
		//currentNode.address = ((DecisionNode*)currentNode.address)->toPinFalse->owner;
		break;
	case stop: break;
	default: break;
	}

	if (currentNode.type == read) {
		state = waitingForInput;
	}
	else if (currentNode.type == stop) {
		state = done;
	}
	else {
		state = processing;
	}
}

void DrawSelectedNodeOptions(AnyNodeType& node, Button* del, Button* edit) { //TODO: asta n ar tb sa fie aici
	int x = 0, y = 0, width = 0, height = 0;

	if (node.type == start) {
		StartNode* n = (StartNode*)node.address;
		x = n->x;
		y = n->y;
		width = n->width;
		height = n->height;
	}
	else if (node.type == read) {
		ReadNode* n = (ReadNode*)node.address;
		x = n->x;
		y = n->y;
		width = n->width;
		height = n->height;
	}
	else if (node.type == write) {
		WriteNode* n = (WriteNode*)node.address;
		x = n->x;
		y = n->y;
		width = n->width;
		height = n->height;
	}
	else if (node.type == assign) {
		AssignNode* n = (AssignNode*)node.address;
		x = n->x;
		y = n->y;
		width = n->width;
		height = n->height;
	}
	else if (node.type == decision) {
		DecisionNode* n = (DecisionNode*)node.address;
		x = n->x;
		y = n->y;
		width = n->width;
		height = n->height;
	}
	else {
		StopNode* n = (StopNode*)node.address;
		x = n->x;
		y = n->y;
		width = n->width;
		height = n->height;
	}

	DrawRectangle(x - 2, y - 2, width + 4, height + 4, WHITE);

	SetButtonPosition(del, x + width + 7, y + (height - del->height) / 2);
	DrawButton(del);

	SetButtonPosition(edit, x + width + del->width + 12, y + (height - edit->height) / 2);
	DrawButton(edit);
}

void DragNode(AnyNodeType& node, int mx, int my) {//TODO: asta n ar tb sa fie aici
	if (node.address == nullptr || node.type == noType) { // TODO: nu cred ca e necesar sa verifici
		return;
	}

	if (node.type == start) {
		SetStartNodePosition((StartNode*)node.address, mx, my);
		return;
	}
	if (node.type == read) {
		SetReadNodePosition((ReadNode*)node.address, mx, my);
		return;
	}
	if (node.type == write) {
		SetWriteNodePosition((WriteNode*)node.address, mx, my);
		return;
	}
	if (node.type == assign) {
		SetAssignNodePosition((AssignNode*)node.address, mx, my);
		return;
	}
	if (node.type == decision) {
		SetDecisionNodePosition((DecisionNode*)node.address, mx, my);
		return;
	}
	else {
		SetStopNodePosition((StopNode*)node.address, mx, my);
		return;
	}
}

void GetClickedPin(Pin*& pin, int mx, int my, NodeArrays& nodes) {
	Pin* p = &nodes.startNode->outPin;
	if (mx >= p->x && mx <= p->x + p->radius && my >= p->y && my <= p->y + p->radius) {
		pin = p;
		return;
	}
	for (ReadNode* n : nodes.readNodes) {
		p = &n->inPin;
		if (mx >= p->x && mx <= p->x + p->radius && my >= p->y && my <= p->y + p->radius) {
			pin = p;
			return;
		}
		p = &n->outPin;
		if (mx >= p->x && mx <= p->x + p->radius && my >= p->y && my <= p->y + p->radius) {
			pin = p;
			return;
		}
	}
	for (WriteNode* n : nodes.writeNodes) {
		p = &n->inPin;
		if (mx >= p->x && mx <= p->x + p->radius && my >= p->y && my <= p->y + p->radius) {
			pin = p;
			return;
		}
		p = &n->outPin;
		if (mx >= p->x && mx <= p->x + p->radius && my >= p->y && my <= p->y + p->radius) {
			pin = p;
			return;
		}
	}
	for (AssignNode* n : nodes.assignNodes) {
		p = &n->inPin;
		if (mx >= p->x && mx <= p->x + p->radius && my >= p->y && my <= p->y + p->radius) {
			pin = p;
			return;
		}
		p = &n->outPin;
		if (mx >= p->x && mx <= p->x + p->radius && my >= p->y && my <= p->y + p->radius) {
			pin = p;
			return;
		}
	}
	for (DecisionNode* n : nodes.decisionNodes) {
		p = &n->inPin;
		if (mx >= p->x && mx <= p->x + p->radius && my >= p->y && my <= p->y + p->radius) {
			pin = p;
			return;
		}
		p = &n->outPinTrue;
		if (mx >= p->x && mx <= p->x + p->radius && my >= p->y && my <= p->y + p->radius) {
			pin = p;
			return;
		}
		p = &n->outPinFalse;
		if (mx >= p->x && mx <= p->x + p->radius && my >= p->y && my <= p->y + p->radius) {
			pin = p;
			return;
		}
	}
	for (StopNode* n : nodes.stopNodes) {
		p = &n->inPin;
		if (mx >= p->x && mx <= p->x + p->radius && my >= p->y && my <= p->y + p->radius) {
			pin = p;
			return;
		}
	}
	
	pin = nullptr;
}