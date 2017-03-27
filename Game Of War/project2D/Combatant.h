#pragma once
#include <vector>

struct Point2D // point in 2D space
{
	float x, y;
};
struct Colour // colour using red, green, blue, alpha
{
	float R, G, B, A;
};
class Combatant
{
public:
	Combatant();
	Combatant(float X);
	Point2D GetPosition();
	int GetHealth();
	bool GetSorted();
	Combatant* GetCurrentTarget();
	Colour GetColour();
	void SetSorted(bool isSorted);
	void SetHealth(int newHealth);
	void TakeDamage(int amount, Combatant* other);
	void MoveCombatant();
	void TryToAttack();
	void LookForNewTarget(std::vector<Combatant*> combatants);
	bool CheckifDead();
	void UpdatePhysics();
	~Combatant();

private:
	Combatant* currentTarget;
	Point2D position;
	Point2D momentum;
	Point2D pointToMoveTo;
	int health = 100;
	int reachDistance = 10;
	int attackStrength = 10;
	int movementDelay = 10;
	float acceleration = 0.1f;
	float regularAcceleration = acceleration;
	float reverseAcceleration = -acceleration;
	float maxMomentum = 2;
	bool isDead = false;
	bool runningAway = false;
	bool sorted = false;
	Colour combatantColour;
};

