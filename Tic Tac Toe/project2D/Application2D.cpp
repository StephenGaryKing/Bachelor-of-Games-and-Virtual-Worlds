#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <string>
#include <algorithm>
#include <ctime>

Application2D::Application2D() {
	srand((size_t)time(NULL));
}

Application2D::~Application2D() {

}

void Application2D::WaitForSeconds(float seconds) // used to control the "pace" of the game (so that the computer doesn't move immediately every time).
{
	float clockEndTime = seconds * 1000 + clock();
	while (clock() < clockEndTime);
}

bool Application2D::startup() {

	m_2dRenderer = new aie::Renderer2D();
	// set the backround colour to teal
	setBackgroundColour(0, 0.1f, 0.2f, 1);

	// initialize the textures
	m_XTexture = new aie::Texture("./textures/X.png");
	m_OTexture= new aie::Texture("./textures/O.png");
	m_GridTexture = new aie::Texture("./textures/Grid.png");
	m_blankTexture = new aie::Texture("./textures/Blank.png");
	m_CurrentPlayerFont = new aie::Font("./font/consolas.ttf", 30);
	m_titleFont = new aie::Font("./font/consolas_bold.ttf", 110);

	
	for (int i = 0; i < 9; i++)
	{
		m_boardLayout[i] = m_blankTexture; // populate the board with blank spaces
	}

	Colour buttonOutline {.4f,.4f,.4f,1}; // gray
	Colour buttonInnerColour{ 0,1,1,1 }; // teal

	// create each button at the main menu screen
	m_easy = { buttonOutline, buttonInnerColour, "EASY",{80, 40}, {100, 400} };
	m_medium = { buttonOutline, buttonInnerColour, "MEDIUM",{ 110, 40 },{ 100, 300 } };
	m_hard = { buttonOutline, buttonInnerColour, "HARD",{ 80, 40 },{ 100, 200 } };
	m_MainMenuButtons[0] = m_easy;
	m_MainMenuButtons[1] = m_medium;
	m_MainMenuButtons[2] = m_hard;

	m_cameraX = 0;
	m_cameraY = 0;

	draw();

	return true;
}

void Application2D::shutdown() {
	
	delete m_XTexture;
	delete m_OTexture;
	delete m_2dRenderer;
	delete m_CurrentPlayerFont;
	delete m_blankTexture;
}

void Application2D::update(float deltaTime) {

	aie::Input* input = aie::Input::getInstance();

	if (m_gameState == -1)
	{
		// check if the Main menu buttons are pressed and set the difficulty accordingly
		if (input->wasMouseButtonPressed(0))
		{
			m_mouseX = (float)input->getMouseX();
			m_mouseY = (float)input->getMouseY();
			for (int i = 0; i < 3; i++)
			{
				if ((m_mouseX - m_MainMenuButtons[i].position[0]) < m_MainMenuButtons[i].size[0] &&
					(m_mouseX - m_MainMenuButtons[i].position[0]) > 0 &&
					(m_mouseY - m_MainMenuButtons[i].position[1]) < m_MainMenuButtons[i].size[1] - m_MainMenuButtons[i].size[1]/2 &&
					(m_mouseY - m_MainMenuButtons[i].position[1]) > -m_MainMenuButtons[i].size[1]/2)
				{
					for (int n = 0; n < 10; n++)
					{
						m_difficulty[n] = m_MainMenuButtons[i].text[n];
					}
					m_gameState = 5;
					m_mouseX = 0;
					m_mouseY = 0;
					// change the backround colour to purple
					setBackgroundColour(0.2f, 0, 0.3f, 1);
					break;
				}
			}
		}
	}
	// while the game is being played
	if (m_gameState == 0)
	{
		if (m_currentTurn == 1)
		{
			// player's turn
			// check whether the player chooses a square and place an 'X' if possible
			if (input->wasMouseButtonPressed(0))
			{
				m_mouseX = (float)input->getMouseX();
				m_mouseY = (float)input->getMouseY();
				for (int i = 0; i < 9; i++)
				{
					if (m_boardLayout[i] == m_blankTexture && 
						(m_mouseX - m_boardPositions[i][0]) < 50 && 
						(m_mouseX - m_boardPositions[i][0]) > -50 && 
						(m_mouseY - m_boardPositions[i][1]) < 50 && 
						(m_mouseY - m_boardPositions[i][1]) > -50)
					{
						m_boardLayout[i] = m_XTexture;
						m_currentTurn = 2;
					}
				}
				// check whether the game is over
				if (CheckIfWinner(m_XTexture) == 1)
					m_gameState = 1;
				if (CheckIfWinner(m_XTexture) == 2 && CheckIfWinner(m_OTexture) == 2)
					m_gameState = 3;
			}
		}
		else
		{
			//computer's turn
			WaitForSeconds(rand()%2 * 1.0f);
			if (strcmp(m_difficulty, "EASY") == 0)
			{
				m_boardLayout[FindRandomMove()] = m_OTexture; // move randomly if the difficulty is EASY
				m_currentTurn = 1;
			}
			else
			{
				int positionToMove = FindBestMove();
				m_boardLayout[positionToMove] = m_OTexture; // place an 'O' in the position determined to be the best by FindBestMove()
				m_currentTurn = 1;
			}
			// check whether the game is over
			if (CheckIfWinner(m_OTexture) == 1)
				m_gameState = 2;
			if (CheckIfWinner(m_OTexture) == 2 && CheckIfWinner(m_XTexture) == 2)
				m_gameState = 3;
		}
	}

	if (m_gameState == 5)
	{
		m_gameState = 0;
	}

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	// restart the game
	if (input->wasKeyPressed(aie::INPUT_KEY_R) && (m_gameState > -1))
	{
		m_gameState = 0;
		m_currentTurn = 1;
		for (int i = 0; i < 9; i++)
		{
			m_boardLayout[i] = m_blankTexture;
		}
	}

	// change difficulty (return to main menu and clear the board)
	if (input->wasKeyPressed(aie::INPUT_KEY_SPACE) && (m_gameState > -1))
	{
		m_currentTurn = 1;
		for (int i = 0; i < 9; i++)
		{
			m_boardLayout[i] = m_blankTexture;
		}
		m_gameState = -1;
		// change the backround colour to teal
		setBackgroundColour(0, 0.1f, 0.2f, 1);
	}
}

int Application2D::CheckIfWinner(aie::Texture* currentPlayer)
{
	/*this is the format of the board for reference
	1|2|3
	-+-+-
	4|5|6
	-+-+-
	7|8|9
	*/

	int count = 0;
	bool gameState;
	//check horizontals
	while (count < 9)
	{
		gameState = true;
		for (int i = count; i < count + 3; i++)
		{
			if (m_boardLayout[i] != currentPlayer)
			{
				gameState = false;
			}
		}
		if (gameState)
			return 1;
		count += 3;
	}
	count = 0;

	//check verticals
	while (count < 3)
	{
		gameState = true;
		for (int i = count; i < 9; i += 3)
		{
			if (m_boardLayout[i] != currentPlayer)
			{
				gameState = false;
			}
		}
		if (gameState)
			return 1;
		count++;
	}

	//check diagonals
	if (m_boardLayout[0] == currentPlayer && m_boardLayout[4] == currentPlayer && m_boardLayout[8] == currentPlayer)
		return 1;

	if (m_boardLayout[2] == currentPlayer && m_boardLayout[4] == currentPlayer && m_boardLayout[6] == currentPlayer)
		return 1;

	//check if it is a draw
	gameState = true;
	for (int i = 0; i < 9; i++)
	{
		if (m_boardLayout[i] == m_blankTexture)
		{
			gameState = false;
			break;
		}
	}
	if (gameState)
	{
		return 2;
	}

	return 0;

}

int Application2D::FindRandomMove() // generate a random move for the computer to use
{
	int blankSpots[9];
	int count = 0;
	for (int i = 0; i < 9; i++)
	{
		if (m_boardLayout[i] == m_blankTexture)
		{
			blankSpots[count] = i;
			count++;
		}
	}
	return blankSpots[rand() % count];
}

int Application2D::FindBestMove() // find the best move given the current board using a MiniMax algorithm
{
	std::vector<Move> AllMoves; // this vector holds all of the possible moves so far
	m_currentTurn = 1;

	// find best score and return it
	for (int position = 0; position < 9; position++)
	{
		if (m_boardLayout[position] == m_blankTexture) // check all possible positions to start looking
		{
			m_boardLayout[position] = m_OTexture;
			Move newMove;
			newMove.Score = MiniMax(m_XTexture, &AllMoves, position); // use a MiniMax algorithm with the new board state to find all possible moves
			newMove.position = position;
			AllMoves.push_back(newMove);
			m_boardLayout[position] = m_blankTexture;
		}
	}
	m_currentDepth = 1;
	for (int position = 0; position < 9; position++) // look through all possible positions again inpersonating the player to try to block the player
	{
		if (m_boardLayout[position] == m_blankTexture)
		{
			m_boardLayout[position] = m_XTexture;
			if (CheckIfWinner(m_XTexture) == 1)
			{
				Move newMove;
				newMove.position = position;
				newMove.Score = 1;
				AllMoves.push_back(newMove);
			}
			m_boardLayout[position] = m_blankTexture;
		}
	}

	Move bestSpotToMove = { 10, -10 };
	for (size_t i = 0; i < AllMoves.size(); i++)
	{
		if (AllMoves[i].Score == 0 && bestSpotToMove.position == -10) // if there is no best spot to move set it to the first spot that is a draw (draws can only be ditermined ahead of time if set to HARD difficulty)
		{
			bestSpotToMove = AllMoves[i];
		}
		if ((bestSpotToMove.Score == 0 || AllMoves[i].Score < bestSpotToMove.Score) && AllMoves[i].Score > 0) // find the move that will win at the lowest depth in the computer's search. lower scores are better and 0's are draws
		{
			bestSpotToMove = AllMoves[i];
		}
		if (AllMoves[i].Score == bestSpotToMove.Score && rand()%100 <= 10) // randomly decide wether to place the 'O' here or in another position where the score is the same (adds variety to multiple play throughs)
		{
			bestSpotToMove = AllMoves[i];
		}
	}
	return bestSpotToMove.position;
}

int Application2D::MiniMax(aie::Texture* currentPlayer, std::vector<Move>* AllMoves, int position)
{
	// check the state of the board
	if (CheckIfWinner(m_OTexture) == 1)
	{
		// 'O' wins
		return m_currentDepth;
	}
	if (CheckIfWinner(m_OTexture) == 2)
	{
		// it's a draw
		return 0;
	}
	if (CheckIfWinner(m_OTexture) == 0)
	{
		if (strcmp(m_difficulty, "MEDIUM") == 0)
		{
			// dont look more than 1 move ahead if the difficulty is on medium
			return 0;
		}
		for (int i = 0; i < 9; i++)
		{
			// look a deap as possible for the best moves
			if (m_boardLayout[i] == m_blankTexture)
			{
				m_currentDepth++;
				m_boardLayout[i] = m_OTexture;
				Move newMove;
				newMove.Score = MiniMax((currentPlayer == m_OTexture) ? m_XTexture : m_OTexture, AllMoves, position); // flip flop between the two players as the next MiniMax is called to generate a logical game
				newMove.position = position;
				AllMoves->push_back(newMove);
				m_boardLayout[i] = m_blankTexture;
				m_currentDepth--;
			}
		}
	}
	return 0;
}

// function for drawing buttons
void Application2D::DrawButton(Button button)
{
	m_2dRenderer->setRenderColour(button.outerColour.R, button.outerColour.G, button.outerColour.B, button.outerColour.A);
	m_2dRenderer->drawBox(button.position[0]+button.size[0]/2.0f, button.position[1], button.size[0] + button.size[0]/10.0f, button.size[1] + button.size[0] / 10.0f);
	m_2dRenderer->setRenderColour(button.innerColour.R, button.innerColour.G, button.innerColour.B, button.innerColour.A);
	m_2dRenderer->drawBox(button.position[0] + button.size[0] / 2, button.position[1], button.size[0], button.size[1]);
	m_2dRenderer->setRenderColour(button.outerColour.R, button.outerColour.G, button.outerColour.B, button.outerColour.A);
	m_2dRenderer->drawText(m_CurrentPlayerFont, button.text, button.position[0]+5, button.position[1] - 10);
}

void Application2D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// set the camera position before we begin rendering
	m_2dRenderer->setCameraPos(m_cameraX, m_cameraY);

	// begin drawing sprites
	m_2dRenderer->begin();
	
	// draw the Main Menu
	if (m_gameState == -1)
	{
		m_2dRenderer->setRenderColour(0.3f, 1, 0.3f, 1);
		m_2dRenderer->drawText(m_titleFont, "Choose", 15, 700);
		m_2dRenderer->drawText(m_titleFont, "A", 15, 600);
		m_2dRenderer->drawText(m_titleFont, "difficulty", 15, 500);

		DrawButton(m_easy);
		DrawButton(m_medium);
		DrawButton(m_hard);
	}

	if (m_gameState >= 0)
	{
		// Draw the Game Board
		m_2dRenderer->drawSprite(m_GridTexture, 360, 360, 500, 500);

		m_2dRenderer->setRenderColour(0.3f, 0.3f, 0.3f, 1);
		m_2dRenderer->drawText(m_CurrentPlayerFont, "press 'SPACE' to change difficulty", 5, 5);
		m_2dRenderer->drawText(m_CurrentPlayerFont, "press 'R' to restart", 5, 40);

		for (int i = 0; i < 9; i++)
		{
			m_2dRenderer->drawSprite(m_boardLayout[i], m_boardPositions[i][0], m_boardPositions[i][1], 100, 100);
		}

		// Draw on screen text
		if (m_gameState == 0)
		{
			m_2dRenderer->setRenderColour(0.3f, 0, 1, 1);
			m_2dRenderer->drawText(m_titleFont, "Tic Tac Toe", 15, 720);
		}

		// if the game is over display who won
		else
		{
			if (m_gameState == 1) // if player wins
			{
				m_2dRenderer->setRenderColour(0.3f, 0, 1, 1);
				m_2dRenderer->drawText(m_CurrentPlayerFont, "WOU WIN!", 15, 720);
			}

			if (m_gameState == 2) // if computer wins
			{
				m_2dRenderer->setRenderColour(0.3f, 0, 1, 1);
				m_2dRenderer->drawText(m_CurrentPlayerFont, "THE COMPUTER WON!", 15, 720);
			}

			if (m_gameState == 3) // draw
			{
				m_2dRenderer->setRenderColour(0.3f, 0, 1, 1);
				m_2dRenderer->drawText(m_CurrentPlayerFont, "IT'S A DRAW!", 15, 720);
			}
		}
	}

	// done drawing sprites
	m_2dRenderer->end();
}