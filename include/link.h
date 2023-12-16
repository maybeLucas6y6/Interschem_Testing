#pragma once

#include "pin.h"
#include "nodeGeneral.h"

struct Link {
	Pin* fromPin;
	NodeInfo fromNode;
	Pin* toPin;
	NodeInfo toNode;
};

struct NodeLinks {
	unsigned nLinks = 0;
	Link vec[32] = { 0 };
};

void NewLink(NodeLinks& links, Pin& fromPin, NodeInfo fromNode, Pin& toPin, NodeInfo toNode);
void DrawLink(Link link);