#pragma once

enum NodeType {
	none,
	start,
	read,
	write,
	assign,
	decision,
	stop
};
struct NodeInfo {
	int id, index;
	NodeType type;
};