#pragma once
#include "Operation.h"

class BinaryOperation : public Operation
{
public:
	BinaryOperation(const Napis& opr);

	TreeNode* clone() const override;
	TreeNode* nextFreeNode() override;

	bool simplify(AbstractSyntaxTree& ast) override;

	bool evaluate(AbstractSyntaxTree& ast, UnaryOperation* opr) override;

	bool evaluate_part1(AbstractSyntaxTree& ast, BinaryOperation* opr) override;
	bool evaluate_part2(AbstractSyntaxTree& ast, BinaryOperation* opr, Constant* left) override;
	bool evaluate_part2(AbstractSyntaxTree& ast, BinaryOperation* opr, Matrix* left) override;
	bool evaluate_part2(AbstractSyntaxTree& ast, BinaryOperation* opr, Variable* left) override;
	bool evaluate_part2(AbstractSyntaxTree& ast, BinaryOperation* opr, BinaryOperation* left) override;
	bool evaluate_part2(AbstractSyntaxTree& ast, BinaryOperation* opr, UnaryOperation* left) override;

	Napis toNapis() const override;

	bool lowerPrecedenceThan(const Napis& opr2) const override;
	bool equalPrecedenceAs(const Napis& opr2) const override;
	bool isNegationSignificant() const override;
};