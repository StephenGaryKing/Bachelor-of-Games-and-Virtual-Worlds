// A Debugging Exercise by Marc Chee (marcc@aie.edu.au)
// 18/12/2016

// Marines are trying to "de-bug" an area (haha . . . haha)
// However something's up with this code . . . figure out what's going wrong
// When you're finished, there should be no compiler errors or warnings
// The encounter should also run and finish correctly as per the rules in the comments

// In many cases there are differences between what's in otherwise identical sections
// for Marines and Zerglings. It's good to be able to tell what the differences are
// and why they might be important.

// What's efficient and inefficient? Why?
// What uses more memory and what doesn't? Why?

#include <iostream>
#include <vector>
#include <algorithm>
#include "Marine.h"
#include "Zergling.h"

using std::vector;
using std::cout;
using std::endl;

//function prototypes
bool marineAlive(vector<Marine> squad);
bool zerglingAlive(vector<Zergling> swarm);

int main()
{
	vector<Marine> squad;
	vector<Zergling> swarm;

	int squadSize = 10;
	int swarmSize = 20;

	// Set up the Squad and the Swarm at their initial sizes listed above

	Marine m;
	for (size_t i = 0; i < squadSize; i++)
	{
		squad.push_back(m);
	}

	Zergling z;
	for (size_t i = 0; i < swarmSize; i++)
	{
		swarm.push_back(z);
	}

	cout << "A squad of marines approaches a swarm of Zerglings and opens fire! The Zerglings charge!" << endl;
	// Attack each other until only one team is left alive
	while (marineAlive(squad) && zerglingAlive(swarm)) // If anyone is left alive to fight . . .
	{
		if (marineAlive(squad)) // if there's at least one marine alive
		{
			for (int a = 0; a < squad.size(); a ++) // loop through marines
			{
				// each marine will attack the first living zergling in the swarm
				for (int b = 0; b < swarm.size(); b++) // loop through zerglings
				{
					if (swarm[b].isAlive()) // if the zergling in question is still alive
					{
						cout << "A marine fires for " << squad[a].attack() << " damage. " << endl;
						swarm[b].takeDamage(squad[a].attack()); // deal damage to the zergling
						if (!(swarm[b].isAlive())) // if the zergling dies, it is removed from the swarm (all dead zerglings will be removed after for loop)
						{
							cout << "The zergling target dies" << endl;
						}
						break;
					}
				}
			}
			swarm.erase(std::remove_if(swarm.begin(), swarm.end(), [](Zergling& z) { return !z.isAlive(); }), swarm.end());
			
		}
		if (zerglingAlive(swarm)) // if there's at least one zergling alive
		{
			for (int a = 0; a < swarm.size(); a++) // loop through zerglings
			{
				// each zergling will attack the first living marine in the swarm
				for (int b = 0; b < squad.size(); b++) // loop through marines
				{
					if (squad[b].isAlive()) // if the marine in question is still alive
					{
						cout << "A zergling attacks for " << swarm[a].attack() << " damage." << endl;
						squad[b].takeDamage(swarm[a].attack()); // deal damage to the marine
						if (!(squad[b].isAlive())) // if the marine dies, it is removed from the swarm (all dead marines will be removed after for loop)
						{
							cout << "The marine succumbs to his wounds." << endl;
						}
						break;
					}
				}
			}
			squad.erase(std::remove_if(squad.begin(), squad.end(), [](Marine& m) { return !m.isAlive(); }), squad.end());
		}
	}

	// Once one team is completely eliminated, the fight ends and one team wins
	cout << "The fight is over. ";
	if (marineAlive(squad))
	{
		cout << "The Marines win." << endl;
	}
	else
	{
		cout << "The Zerg win." << endl;
	}
	system("PAUSE");
}

// Is there a Marine Alive?
bool marineAlive(vector<Marine> squad)
{
	return squad.size() > 1;
}

// Is there a zergling Alive
bool zerglingAlive(vector<Zergling> swarm)
{
	return swarm.size() > 1;
}
