#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <random>
#include <ctime>
#include <algorithm>

Application2D::Application2D() {
	srand(time(NULL));
}

Application2D::~Application2D() {

}

bool Application2D::startup() {

	m_2dRenderer = new aie::Renderer2D();

	m_playerFont = new aie::Font("./font/consolas.ttf", 20);
	m_winnerFont = new aie::Font("./font/consolas_bold.ttf", 120);

	for (int i = 0; i < numberOfCombatants; i++)
	{
		// using vectors
		redCombatants.push_back(new Combatant());
		blueCombatants.push_back(new Combatant());
		// using arrays
		//redCombatants[i] = new Combatant();
		//blueCombatants[i] = new Combatant();
	}
	for (int i = 0; i < numberOfCombatants; i++)
	{
		redCombatants[i]->LookForNewTarget(blueCombatants);
		blueCombatants[i]->LookForNewTarget(redCombatants);
	}
	
	m_cameraX = 0;
	m_cameraY = 0;

	return true;
}

bool highToLow(Combatant* i, Combatant* j) { return i->GetHealth() > j->GetHealth(); };

void Application2D::sortByHealth(std::vector<Combatant*> combatantList)
{
	std::sort(combatantList.begin(), combatantList.end(), highToLow);
}

void Application2D::shutdown() {
	
	delete m_2dRenderer;
	//delete[] redCombatants;
	//delete[] blueCombatants;

}

void Application2D::WaitForSeconds(float seconds)
{
	float clockEndTime = seconds * 1000 + clock();
	while (clock() < clockEndTime);
}

void Application2D::update(float deltaTime) {
	// input example
	aie::Input* input = aie::Input::getInstance();

	WaitForSeconds(0.01);

	if (input->wasKeyPressed(32)) // pause the simulation if space is pressed
	{
		gameOver = !gameOver;
	}

	if (!gameOver)
	{
		//update Combatant logic
		for (int i = 0; i < numberOfCombatants; i++)
		{
			if (rand() % 99 + 1 <= 50)
			{
				if (redCombatants[i]->GetCurrentTarget()->CheckifDead())
					redCombatants[i]->LookForNewTarget(blueCombatants);
				redCombatants[i]->MoveCombatant();
				if (blueCombatants[i]->GetCurrentTarget()->CheckifDead())
					blueCombatants[i]->LookForNewTarget(redCombatants);
				blueCombatants[i]->MoveCombatant();
			}
			else
			{
				if (blueCombatants[i]->GetCurrentTarget()->CheckifDead())
					blueCombatants[i]->LookForNewTarget(redCombatants);
				blueCombatants[i]->MoveCombatant();
				if (redCombatants[i]->GetCurrentTarget()->CheckifDead())
					redCombatants[i]->LookForNewTarget(blueCombatants);
				redCombatants[i]->MoveCombatant();
			}
		}
		if (CheckIfGameOver(redCombatants) || CheckIfGameOver(blueCombatants))
			gameOver = true;

		sortByHealth(redCombatants);
		sortByHealth(blueCombatants);
	}

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
	if (input->isKeyDown(aie::INPUT_KEY_R) && gameOver)
		ResetGame();
}

void Application2D::ResetGame()
{
	for (int i = 0; i < numberOfCombatants; i++)
	{
		delete redCombatants[i];
		delete blueCombatants[i];
		redCombatants[i] = new Combatant();
		blueCombatants[i] = new Combatant();
	}
	for (int i = 0; i < numberOfCombatants; i++)
	{
		redCombatants[i]->LookForNewTarget(blueCombatants);
		blueCombatants[i]->LookForNewTarget(redCombatants);
	}
	gameOver = false;
}

bool Application2D::CheckIfGameOver(std::vector<Combatant*> combatantList)
{
	for (int i = 0; i < numberOfCombatants; i++)
	{
		if (!combatantList[i]->CheckifDead())
		{
			return false;
		}
	}
	return true;
}

char* Application2D::ConvertNumToCharArray(int num)
{
	combatantNumber[0] = num+48;
	combatantNumber[1] = 0;
	return combatantNumber;
}

void Application2D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// set the camera position before we begin rendering
	m_2dRenderer->setCameraPos(m_cameraX, m_cameraY);

	// begin drawing sprites
	m_2dRenderer->begin();

	//draw general text
	m_2dRenderer->setRenderColour(1, 1, 1, .4);
	m_2dRenderer->drawText(m_playerFont, "Press 'Esc' to close simulation", 5, 5);

	// draw redCombatants
	for (int i = 0; i < numberOfCombatants; i++)
	{
		if (!redCombatants[i]->CheckifDead())
		{
			m_2dRenderer->setRenderColour(redCombatants[i]->GetCurrentTarget()->GetColour().R, redCombatants[i]->GetCurrentTarget()->GetColour().G, redCombatants[i]->GetCurrentTarget()->GetColour().B, 1);
			Point2D pointToDraw = redCombatants[i]->GetPosition();
			m_2dRenderer->drawCircle(pointToDraw.x + 18, pointToDraw.y + 18, 4, 4);

			m_2dRenderer->setRenderColour(redCombatants[i]->GetColour().R, redCombatants[i]->GetColour().G, redCombatants[i]->GetColour().B, 1);
			pointToDraw = redCombatants[i]->GetPosition();
			m_2dRenderer->drawCircle(pointToDraw.x, pointToDraw.y, 20, 20);

			m_2dRenderer->setRenderColour(1, 0, 0, 1);
			pointToDraw = redCombatants[i]->GetPosition();
			m_2dRenderer->drawCircle(pointToDraw.x, pointToDraw.y, 16, 16);
			m_2dRenderer->setRenderColour(1, 1, 1, 1);
			//ConvertNumToCharArray(i);
			//m_2dRenderer->drawText(m_playerFont, combatantNumber, pointToDraw.x-10, pointToDraw.y-10);
		}
		else
		{
			m_2dRenderer->setRenderColour(.1, .1, .1, 1);
			Point2D pointToDraw = redCombatants[i]->GetPosition();
			m_2dRenderer->drawCircle(pointToDraw.x, pointToDraw.y, 20, 20);
			m_2dRenderer->setRenderColour(.3, .3, .3, 1);
			//ConvertNumToCharArray(i);
			//m_2dRenderer->drawText(m_playerFont, combatantNumber, pointToDraw.x - 10, pointToDraw.y - 10);
		}
	}
	// draw blueCombatants
	for (int i = 0; i < numberOfCombatants; i++)
	{
		if (!blueCombatants[i]->CheckifDead())
		{
			m_2dRenderer->setRenderColour(blueCombatants[i]->GetCurrentTarget()->GetColour().R, blueCombatants[i]->GetCurrentTarget()->GetColour().G, blueCombatants[i]->GetCurrentTarget()->GetColour().B, 1);
			Point2D pointToDraw = blueCombatants[i]->GetPosition();
			m_2dRenderer->drawCircle(pointToDraw.x + 18, pointToDraw.y + 18, 4, 4);

			m_2dRenderer->setRenderColour(blueCombatants[i]->GetColour().R, blueCombatants[i]->GetColour().G, blueCombatants[i]->GetColour().B, 1);
			pointToDraw = blueCombatants[i]->GetPosition();
			m_2dRenderer->drawCircle(pointToDraw.x, pointToDraw.y, 20, 20);

			m_2dRenderer->setRenderColour(0, 0, 1, 1);
			pointToDraw = blueCombatants[i]->GetPosition();
			m_2dRenderer->drawCircle(pointToDraw.x, pointToDraw.y, 16, 16);
			m_2dRenderer->setRenderColour(1, 1, 1, 1);
			//ConvertNumToCharArray(i);
			//m_2dRenderer->drawText(m_playerFont, combatantNumber, pointToDraw.x - 10, pointToDraw.y - 10);
		}
		else
		{
			m_2dRenderer->setRenderColour(.1, .1, .1, 1);
			Point2D pointToDraw = blueCombatants[i]->GetPosition();
			m_2dRenderer->drawCircle(pointToDraw.x, pointToDraw.y, 20, 20);
			m_2dRenderer->setRenderColour(.3, .3, .3, 1);
			//ConvertNumToCharArray(i);
			//m_2dRenderer->drawText(m_playerFont, combatantNumber, pointToDraw.x - 10, pointToDraw.y - 10);
		}
	}

	// draw game over text
	if (CheckIfGameOver(redCombatants))
	{
		m_2dRenderer->setRenderColour(0, 0, 1, 1);
		m_2dRenderer->drawText(m_winnerFont, "BLUE WINS!", +20, 300);
		m_2dRenderer->setRenderColour(1, 1, 1, .8);
		m_2dRenderer->drawText(m_playerFont, "Press 'R' to retsart simulation", 5, 30);
	}
	if (CheckIfGameOver(blueCombatants))
	{
		m_2dRenderer->setRenderColour(1, 0, 0, 1);
		m_2dRenderer->drawText(m_winnerFont, "RED WINS!", +50, 300);
		m_2dRenderer->setRenderColour(1, 1, 1, .8);
		m_2dRenderer->drawText(m_playerFont, "Press 'R' to retsart simulation", 5, 30);
	}

	// done drawing sprites
	m_2dRenderer->end();
}