#include "Combatant.h"
#include <random>



Combatant::Combatant() // initialize the combatant with the default settings (random)
{
	position.x = (float)(rand() % 718 + 1);
	position.y = (float)(rand() % 718 + 1);
	momentum = { (float)(rand() % (int)maxMomentum),(float)(rand() % (int)maxMomentum) };
	combatantColour = { (rand() % 10000) / 1000.0f, (rand() % 1000) / 1000.0f , (rand() % 1000) / 1000.0f , 1 };
}

Combatant::Combatant(float X) // initialize the combatant with a specified X coardinate
{
	position.x = X;
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

void Combatant::SetSorted(bool isSorted) // used when sorting combatants by their health
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

void Combatant::UpdatePhysics() // update the acceleration and direction that the combatant is moving
{
	static int count = 0;
	if (count >= movementDelay)
	{
		acceleration = regularAcceleration; // stop dashing backward
		count = 0;
		pointToMoveTo.x = currentTarget->GetPosition().x;
		pointToMoveTo.y = currentTarget->GetPosition().y;
		if (rand() % 100 < 10)
		{
			acceleration = reverseAcceleration; // manipulate acceleration to dash backward
			count = -movementDelay;
		}
	}
	else
	{
		count++; // increment count (count keeps track of when the last phisics update was called)

		// this section smoothly moves the combatants towards their pointToMoveTo
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

void Combatant::TryToAttack() // attempt to damage the currentTarget
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
		UpdatePhysics();
		// move combatant
		position.x += (momentum.x);
		position.y += (momentum.y);
		// If the combatant leaves the screen make them appear on the opposite side. (like pacman or snake)
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

void Combatant::LookForNewTarget(std::vector<Combatant*> combatants) // this is used when the currentTarget is dead
{
	currentTarget = combatants[rand()%combatants.size()]; // pick a random target from the other team to try to attack
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
