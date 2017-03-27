#pragma once
class Entity
{
public:
	Entity();
	~Entity();

	virtual int attack() = 0;
	virtual void takeDamage(int damage) = 0;
	bool isAlive();

protected:
	int health;
	int maxHealth = 50;
};

