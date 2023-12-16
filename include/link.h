#pragma once

#include "pin.h"
#include "nodeGeneral.h"

struct AnyNodeType {
	void* address;
	NodeType type;
};

//struct Link {
//	Pin* fromPin;
//	Pin* toPin;
//	AnyNodeType toNode;
//};

struct Link {
	Pin* fromPin;
	Pin* toPin;
};

struct NodeLinks {
	unsigned nLinks = 0;
	Link vec[32] = { 0 };
};

void NewLink(NodeLinks& links, Pin& fromPin, Pin& toPin);
void NewLink(Link& from, Pin& to);
void NewLink(Link& from, Pin& to, void* toNodeAddr, NodeType toNodeType);
void NewLink(Pin*& destination, Pin& source);
void DrawLink(Link link);