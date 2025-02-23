#pragma once
#include "Napis.h"

class AbstractSyntaxTree;

class TreeNode
{
public:
	TreeNode* m_parent;
	TreeNode* m_leftChild;
	TreeNode* m_rightChild;

	TreeNode();
	virtual ~TreeNode();

	virtual TreeNode* clone() const = 0;
	virtual TreeNode* nextFreeNode();

	virtual bool simplify(AbstractSyntaxTree& ast);

	virtual Napis toNapis() const = 0;
	virtual Napis toNapisExpand() const;

	virtual bool lowerPrecedenceThan(const Napis& opr2) const;
	virtual bool isNegationSignificant() const;
};