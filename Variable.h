#pragma once
#include "TreeNode.h"

class Variable : public TreeNode
{
	char m_name;
public:
	Variable(char name);

	TreeNode* clone() const override;

	bool simplify_L(AbstractSyntaxTree& ast, UnaryOperation* opr) override;
	bool simplify_L(AbstractSyntaxTree& ast, BinaryOperation* opr) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, Constant* left) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, Matrix* left) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, Variable* left) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, BinaryOperation* left) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, UnaryOperation* left) override;

	Napis toNapis() const override;
};