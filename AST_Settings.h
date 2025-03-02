#pragma once

#define AST_SIMPLIFY

#define AST_VISUALISE

#define AST_MARK_VISITED_NODE

const static char* TREE_COLOR		= "\033[32m";
const static char* TREE_NODE_COLOR1	= "\033[44m";
const static char* TREE_NODE_COLOR2	= "\033[41m";
const static char* TREE_RESET_COLOR	= "\033[0m";

const char TREE_PIPE	= 179;
const char TREE_BPARENT	= 180;
const char TREE_UPARENT	= 196;
const char TREE_RIGHT	= 218;
const char TREE_LEFT	= 192;

enum reverseRelationshipMode {
	LEFT_CHILD_LEFT_SUBTREE,
	LEFT_CHILD_RIGHT_SUBTREE,
	RIGHT_CHILD_LEFT_SUBTREE,
	RIGHT_CHILD_RIGHT_SUBTREE
};