#include "link.h"
#include "raylib.h"

void NewLink(NodeLinks& links, Pin& fromPin, Pin& toPin) {
	Link link;
	link.fromPin = &fromPin;
	link.toPin = &toPin;
	links.vec[links.nLinks] = link;
	links.nLinks++;
}
void NewLink(Link& from, Pin& to) {
	from.toPin = &to;
}
void NewLink(Link& from, Pin& to, void* toNodeAddr, NodeType toNodeType) {
	from.toPin = &to;
	//from.toNode.address = toNodeAddr;
	//from.toNode.type = toNodeType;
}
void NewLink(Pin*& destination, Pin& source) {
	destination = &source;
}
void DrawLink(Link link) {
	DrawLineEx({ link.fromPin->x, link.fromPin->y }, { link.toPin->x, link.toPin->y }, 2.0f, GRAY);
}