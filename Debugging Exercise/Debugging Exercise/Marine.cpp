#include "Marine.h"



Marine::Marine()
{
	health = maxHealth;
}


Marine::~Marine()
{
}

int Marine::attack()
{
	return 10;
}

void Marine::takeDamage(int damage)
{
	health -= damage;
	if (health < 0)
		health = 0;
}
