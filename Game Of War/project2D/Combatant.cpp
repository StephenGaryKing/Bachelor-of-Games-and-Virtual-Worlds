#include "Combatant.h"
#include <random>



Combatant::Combatant()
{
	position.x = (float)(rand() % 718 + 1);
	position.y = (float)(rand() % 718 + 1);
	momentum = { (float)(rand() % (int)maxMomentum),(float)(rand() % (int)maxMomentum) };
	combatantColour = { (rand() % 10000) / 1000.0f, (rand() % 1000) / 1000.0f , (rand() % 1000) / 1000.0f , 1 };
}

Combatant::Combatant(float X)
{
	position.x = X + 180;
	position.y = (float)(rand() % 718 +1);
	momentum = { (float)(rand() % (int)maxMomentum),(float)(rand() % (int)maxMomentum) };
}

Point2D Combatant::GetPosition()
{
	return position;
}

int Combatant::GetHealth()
{
	return health;
}

Colour Combatant::GetColour()
{
	return combatantColour;
}

bool Combatant::GetSorted()
{
	return sorted;
}

void Combatant::SetSorted(bool isSorted)
{
	sorted = isSorted;
}

void Combatant::TakeDamage(int amount, Combatant* other)
{
	health -= amount;
	currentTarget = other;
	if (CheckifDead())
		isDead = true;
}

void Combatant::SetHealth(int newHealth)
{
	health = newHealth;
}

void Combatant::UpdatePhysics()
{
	static int count = 0;
	if (count >= movementDelay)
	{
		acceleration = regularAcceleration;
		count = 0;
		pointToMoveTo.x = currentTarget->GetPosition().x;
		pointToMoveTo.y = currentTarget->GetPosition().y;
		if (rand() % 100 < 10)
		{
			acceleration = reverseAcceleration; //manipulate acceleration to dash backward
			count = -movementDelay;
		}
	}
	else
	{
		count++;
		if (position.x < pointToMoveTo.x + reachDistance)
		{
			momentum.x += acceleration;
			if (momentum.x > maxMomentum)
			{
				momentum.x = maxMomentum;
			}
		}
		if (position.x > pointToMoveTo.x - reachDistance)
		{
			momentum.x += -acceleration;
			if (momentum.x < -maxMomentum)
			{
				momentum.x = -maxMomentum;
			}
		}
		if (position.y < pointToMoveTo.y + reachDistance)
		{
			momentum.y += acceleration;
			if (momentum.y > maxMomentum)
			{
				momentum.y = maxMomentum;
			}
		}
		if (position.y > pointToMoveTo.y - reachDistance)
		{
			momentum.y += -acceleration;
			if (momentum.y < -maxMomentum)
			{
				momentum.y = -maxMomentum;
			}
		}
	}
}

void Combatant::TryToAttack()
{
	float displacementX = currentTarget->GetPosition().x - position.x, displacementY = currentTarget->GetPosition().y - position.y;
	if (displacementX <= reachDistance+5 && displacementX >= -reachDistance-5 && displacementY <= reachDistance+5 && displacementY >= -reachDistance-5)
	{
		currentTarget->TakeDamage(attackStrength, this);
		momentum.x = 0;
		momentum.y = 0;
	}
}

void Combatant::MoveCombatant()
{
	if (!isDead)
	{
		TryToAttack();
		//TO DO: use momentum,
		UpdatePhysics();
		position.x += (momentum.x);
		position.y += (momentum.y);
		//keep them on the screen
		if (position.x < 0)
			position.x = 720;
		if (position.y < 0)
			position.y = 720;
		if (position.x > 720)
			position.x = 0;
		if (position.y > 720)
			position.y = 0;
	}
}

Combatant* Combatant::GetCurrentTarget()
{
	return currentTarget;
}

//using vectors
void Combatant::LookForNewTarget(std::vector<Combatant*> combatants)
{
	currentTarget = combatants[rand()%combatants.size()];
	pointToMoveTo.x = currentTarget->GetPosition().x;
	pointToMoveTo.y = currentTarget->GetPosition().y;
}

//using arrays
void Combatant::LookForNewTarget(Combatant* combatants[])
{
	currentTarget = combatants[rand() % 10];
	pointToMoveTo.x = currentTarget->GetPosition().x;
	pointToMoveTo.y = currentTarget->GetPosition().y;
}

bool Combatant::CheckifDead()
{
	if (health <= 0)
		isDead = true;
	return isDead;
}

Combatant::~Combatant()
{
}
