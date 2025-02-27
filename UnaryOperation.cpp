#include "UnaryOperation.h"

#include "AbstractSyntaxTree.h"
#include "BinaryOperation.h"
#include "Constant.h"
#include "Matrix.h"
#include "Variable.h"

UnaryOperation::UnaryOperation(const Napis& opr) : Operation(opr) {}

TreeNode* UnaryOperation::clone() const
{
	return new UnaryOperation(*this);
}

TreeNode* UnaryOperation::nextFreeNode()
{
	if (!m_leftChild) return this;
	if (m_parent) return m_parent->nextFreeNode();
	return nullptr;
}

bool UnaryOperation::simplify(AbstractSyntaxTree& ast)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this, TREE_NODE_COLOR2);
	if (m_leftChild->simplify(ast)) return true;
	ast.printTree(this, TREE_NODE_COLOR2);
	return m_leftChild->evaluate(ast, this);
#else
	if (m_leftChild->simplify(ast)) return true;
#endif
}

bool UnaryOperation::evaluate(AbstractSyntaxTree& ast, UnaryOperation* opr)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	return false;
}

bool UnaryOperation::evaluate_part1(AbstractSyntaxTree& ast, BinaryOperation* opr)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	if (m_opr == '-') return m_leftChild->evaluate_part1(ast, opr);
	return false;
}

bool UnaryOperation::evaluate_part2(AbstractSyntaxTree& ast, BinaryOperation* opr, Constant* left)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	if (m_opr == '-') return m_leftChild->evaluate_part2(ast, opr, left);
	return false;
}

bool UnaryOperation::evaluate_part2(AbstractSyntaxTree& ast, BinaryOperation* opr, Matrix* left)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	if (m_opr == '-') return m_leftChild->evaluate_part2(ast, opr, left);
	return false;
}

bool UnaryOperation::evaluate_part2(AbstractSyntaxTree& ast, BinaryOperation* opr, Variable* left)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	if (m_opr == '-') return m_leftChild->evaluate_part2(ast, opr, left);
	return false;
}

bool UnaryOperation::evaluate_part2(AbstractSyntaxTree& ast, BinaryOperation* opr, BinaryOperation* left)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	if (m_opr == '-') return m_leftChild->evaluate_part2(ast, opr, left);
	return false;
}

bool UnaryOperation::evaluate_part2(AbstractSyntaxTree& ast, BinaryOperation* opr, UnaryOperation* left)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
#endif
	if (m_opr == '-') return m_leftChild->evaluate_part2(ast, opr, left);
	return false;
}

bool UnaryOperation::isNegation() const
{
	return m_opr == '-';
}

Napis UnaryOperation::toNapis() const
{
	bool inner_parentheses = m_leftChild->isNegationSignificant();
	bool outer_parentheses = (m_parent && m_parent->isNegationSignificant() &&
		this == m_parent->m_rightChild);
	return
		(outer_parentheses ? "(" : "") +
		m_opr +
		(inner_parentheses ? "(" : "") +
		m_leftChild->toNapis() +
		(inner_parentheses ? ")" : "") +
		(outer_parentheses ? ")" : "");
}
