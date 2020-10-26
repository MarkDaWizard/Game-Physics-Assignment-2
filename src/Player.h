#pragma once
#ifndef __PLAYER__
#define __PLAYER__

#include "Sprite.h"

class Player final : public Sprite
{
public:
	Player();
	~Player();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

};

#endif /* defined (__PLAYER__) */
