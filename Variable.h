#pragma once
#include "TreeNode.h"

class Variable : public TreeNode
{
	char m_name;
public:
	Variable(char name);

	TreeNode* clone() const override;

	bool simplify_L(AbstractSyntaxTree& ast, UnaryOperation* opr, bool leftIsNegated = false) override;
	bool simplify_L(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated = false) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, Constant* left, bool rightIsNegated = false) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, Matrix* left, bool rightIsNegated = false) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, Variable* left, bool rightIsNegated = false) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, BinaryOperation* left, bool rightIsNegated = false) override;
	bool simplify_R(AbstractSyntaxTree& ast, BinaryOperation* opr, bool leftIsNegated, UnaryOperation* left, bool rightIsNegated = false) override;

	Napis toNapis() const override;
};