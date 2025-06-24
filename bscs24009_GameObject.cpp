#include "bscs24009_GameObject.h"

GameObject::GameObject(float x, float y) :
	x(x), y(y), texture({ 0 }), hitbox{}
{
}

Rectangle GameObject::GetHitbox() const {
	return hitbox;
}

float GameObject::GetX() const {
	return x;
}

float GameObject::GetY() const {
	return y;
}