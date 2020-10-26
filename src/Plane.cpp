#include "Plane.h"
#include "TextureManager.h"

Plane::Plane()
{
	TextureManager::Instance()->load("../Assets/textures/Stormtroopers.png", "Storm");

	// set frame width
	setWidth(65);

	// set frame height
	setHeight(65);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PLANE);

}

Plane::~Plane()
= default;

void Plane::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	TextureManager::Instance()->draw("Storm", x, y, 0, 255, true);
}

void Plane::update()
{
}

void Plane::clean()
{
}

