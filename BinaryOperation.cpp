#include "BinaryOperation.h"

#include "Parser.h"
#include "AbstractSyntaxTree.h"
#include "UnaryOperation.h"
#include "Constant.h"
#include "Matrix.h"
#include "Variable.h"

BinaryOperation::BinaryOperation(const Napis& opr) : Operation(opr) {}

TreeNode* BinaryOperation::clone() const
{
	return new BinaryOperation(*this);
}

TreeNode* BinaryOperation::shift()
{
	return new BinaryOperation(std::move(*this));
}

TreeNode* BinaryOperation::nextFreeNode()
{
	if (!m_leftChild || !m_rightChild) return this;
	if (m_parent) return m_parent->nextFreeNode();
	return nullptr;
}

void BinaryOperation::arrange(AbstractSyntaxTree& ast)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this);
	// x + (-y) = x - y;   x - (-y) = x + y;   reduction
	if ((m_opr == '+' || m_opr == '-') && m_rightChild->isNegation()) {
		ast.removeOneChildNode(m_rightChild);
		m_opr = (m_opr == '+' ? '-' : '+');
		ast.printTree(this);
	}

	TreeNode* nodeAtThisPosition = this;

	// x + (y + z) = x + y + z;   reduction
	if (m_rightChild->equalPrecedenceAs(m_opr, true)) {
		nodeAtThisPosition = ast.reverseRelationship(this, RIGHT_CHILD_RIGHT_SUBTREE);
		ast.printTree(nodeAtThisPosition);
	}

	nodeAtThisPosition->m_rightChild->arrange(ast);
	ast.printTree(nodeAtThisPosition);

	nodeAtThisPosition->m_leftChild->arrange(ast);
	ast.printTree(nodeAtThisPosition);

	// rearrange operands in order: expressions, constants, matrices, variables (in alphabetic order)
	TreeNode* unorderedNodeParent = nodeAtThisPosition;

	while (unorderedNodeParent->m_leftChild->equalPrecedenceAs(m_opr) &&
		unorderedNodeParent->m_rightChild->getOrderRank() <
		unorderedNodeParent->m_leftChild->m_rightChild->getOrderRank())
		unorderedNodeParent = ast.reverseRelationship(unorderedNodeParent, LEFT_CHILD_RIGHT_SUBTREE)->m_leftChild;

	if (unorderedNodeParent->m_leftChild->getOrderRank() > unorderedNodeParent->m_rightChild->getOrderRank())
		ast.swapChildren(unorderedNodeParent);
#else
	// x + (-y) = x - y;   x - (-y) = x + y;   reduction
	if ((m_opr == '+' || m_opr == '-') && m_rightChild->isNegation()) {
		ast.removeOneChildNode(m_rightChild);
		m_opr = (m_opr == '+' ? '-' : '+');
	}
	TreeNode* nodeAtThisPosition = this;

	// x + (y + z) = x + y + z;   reduction
	if (m_rightChild->equalPrecedenceAs(m_opr, true))
		nodeAtThisPosition = ast.reverseRelationship(this, RIGHT_CHILD_RIGHT_SUBTREE);

	nodeAtThisPosition->m_rightChild->arrange(ast);
	nodeAtThisPosition->m_leftChild->arrange(ast);

	// rearrange operands in order: expressions, constants, matrices, variables (in alphabetic order)
	TreeNode* unorderedNodeParent = nodeAtThisPosition;

	while (unorderedNodeParent->m_leftChild->equalPrecedenceAs(m_opr) &&
		unorderedNodeParent->m_rightChild->getOrderRank() <
		unorderedNodeParent->m_leftChild->m_rightChild->getOrderRank())
		unorderedNodeParent = ast.reverseRelationship(unorderedNodeParent, LEFT_CHILD_RIGHT_SUBTREE)->m_leftChild;

	if (unorderedNodeParent->m_leftChild->getOrderRank() > unorderedNodeParent->m_rightChild->getOrderRank())
		ast.swapChildren(unorderedNodeParent);
#endif
}

void BinaryOperation::evaluate(AbstractSyntaxTree& ast)
{
#ifdef AST_MARK_VISITED_NODE
	ast.printTree(this, TREE_NODE_COLOR2);
	m_leftChild->evaluate(ast);
	ast.printTree(this, TREE_NODE_COLOR2);
	m_rightChild->evaluate(ast);
	ast.printTree(this, TREE_NODE_COLOR2);
	// x + (-y) = x - y;   x - (-y) = x + y;   reduction
	if ((m_opr == '+' || m_opr == '-') && m_rightChild->isNegation()) {
		ast.removeOneChildNode(m_rightChild);
		m_opr = (m_opr == '+' ? '-' : '+');
		return;
	}
	ast.printTree(this, TREE_NODE_COLOR2);
	ast.replaceSubtree(this, m_leftChild->compute1(m_opr, m_rightChild));
#else
	m_leftChild->evaluate(ast);
	m_rightChild->evaluate(ast);
	ast.replaceSubtree(this, m_leftChild->compute1(m_opr, m_rightChild));
#endif
}

AbstractSyntaxTree BinaryOperation::compute1(const Napis& opr, TreeNode* right) const
{
	return right->compute2(*this, opr);
}

Napis BinaryOperation::toNapis() const
{
	bool left_parentheses = m_leftChild->lowerPrecedenceThan(m_opr);
	bool right_parentheses = (m_rightChild->lowerPrecedenceThan(m_opr) ||
		(m_opr == '-' && m_rightChild->equalPrecedenceAs(m_opr)) ||
		(m_opr == '/' && m_rightChild->equalPrecedenceAs(m_opr)));
	return
		(left_parentheses ? "(" : "") +
		m_leftChild->toNapis() +
		(left_parentheses ? ")" : "") +
		m_opr +
		(right_parentheses ? "(" : "") +
		m_rightChild->toNapis() +
		(right_parentheses ? ")" : "");
}

Napis BinaryOperation::toNapisExpand() const
{
	bool left_parentheses = m_leftChild->lowerPrecedenceThan(m_opr);
	bool right_parentheses = (m_rightChild->lowerPrecedenceThan(m_opr) ||
		(m_opr == '-' && m_rightChild->equalPrecedenceAs(m_opr)) ||
		(m_opr == '/' && m_rightChild->equalPrecedenceAs(m_opr)));
	return
		(left_parentheses ? "(" : "") +
		m_leftChild->toNapisExpand() +
		(left_parentheses ? ")" : "") +
		m_opr +
		(right_parentheses ? "(" : "") +
		m_rightChild->toNapisExpand() +
		(right_parentheses ? ")" : "");
}

bool BinaryOperation::lowerPrecedenceThan(const Napis& opr2) const
{
	return precedence(m_opr) < precedence(opr2);
}

bool BinaryOperation::equalPrecedenceAs(const Napis& opr1, bool replace)
{
	if (precedence(m_opr) == precedence(opr1)) {
		if (replace) {
			if (opr1 == '-')
				m_opr = (m_opr == '+' ? '-' : '+');
			else if (opr1 == '/')
				m_opr = (m_opr == '*' ? '/' : '*');
		}
		return true;
	}
	return false;
}

bool BinaryOperation::isNegationSignificant() const
{
	return m_opr == '+' || m_opr == '-';
}
