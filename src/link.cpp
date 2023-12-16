#include "link.h"
#include "raylib.h"

void NewLink(NodeLinks& links, Pin& fromPin, NodeInfo fromNode, Pin& toPin, NodeInfo toNode) {
	Link link;
	link.fromPin = &fromPin;
	link.fromNode = fromNode;
	link.toPin = &toPin;
	link.toNode = toNode;
	links.vec[links.nLinks] = link;
	links.nLinks++;
}
void DrawLink(Link link) {
	DrawLineEx({ link.fromPin->x, link.fromPin->y }, { link.toPin->x, link.toPin->y }, 2.0f, GRAY);
}