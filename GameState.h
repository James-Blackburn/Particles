#pragma once

class GameState
{
public:
	GameState() = default;
	~GameState() = default;
	
	virtual void update() {};
	virtual void draw() {};
};

