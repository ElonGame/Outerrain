#include "terratree.h"

TerraTree::TerraTree(TerraNode* r) : root(r)
{

}

float TerraTree::Value(const Vector2& p) const
{
	return root->Value(p);
}

Vector2 TerraTree::Gradient(const Vector2& p) const
{
	return root->Gradient(p);
}
