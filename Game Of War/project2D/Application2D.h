#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "Combatant.h"
#include "Audio.h"
#include <vector>

class Application2D : public aie::Application {
public:

	Application2D();
	virtual ~Application2D();

	virtual bool startup();
	virtual void shutdown();
	void WaitForSeconds(float seconds);
	void ResetGame();
	void sortByHealth(std::vector<Combatant*> combatantList);
	bool CheckIfGameOver(std::vector<Combatant*> combatantList);
	char* ConvertNumToCharArray(int num);
	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;

	aie::Font*			m_playerFont;
	aie::Font*			m_winnerFont;
	std::vector<Combatant*> redCombatants;
	std::vector<Combatant*> blueCombatants;
	//Combatant* redCombatants[10];
	//Combatant* blueCombatants[10];

	float m_cameraX, m_cameraY;
	int numberOfCombatants = 10;
	bool gameOver = false;
	char combatantNumber[2];
};