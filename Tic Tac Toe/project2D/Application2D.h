#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "Audio.h"
#include <vector>

struct Move
{
	int Score, position;
};

struct Colour
{
	float R, G, B, A;
};

struct Button
{
	Colour outerColour;
	Colour innerColour;
	char text[10];
	int size[2];
	int position[2];
};

class Application2D : public aie::Application {
public:

	Application2D();
	virtual ~Application2D();

	virtual bool startup();
	virtual void shutdown();
	void WaitForSeconds(float seconds);
	int CheckIfWinner(aie::Texture* currentPlayer);
	int FindRandomMove();
	int FindBestMove();
	int MiniMax(aie::Texture* currentPlayer, std::vector<Move>* AllMoves, int position);
	virtual void update(float deltaTime);
	void DrawButton(Button button);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;

	//textures
	aie::Texture*		m_XTexture;
	aie::Texture*		m_OTexture;
	aie::Texture*		m_GridTexture;
	aie::Texture*		m_boardLayout[9];
	aie::Texture*		m_HighlightTexture;
	aie::Texture*		m_blankTexture;

	//fonts
	aie::Font*			m_titleFont;
	aie::Font*			m_CurrentPlayerFont;

	//variables
	float m_cameraX, m_cameraY;
	float m_mouseX, m_mouseY;
	bool m_mouseIsPressed = false;
	float m_boardPositions[9][2] = { { 180, 540 },{ 360, 540 },{ 540, 540 },{ 180, 360 },{ 360,360 },{ 540, 360 },{ 180, 180 },{ 360, 180 },{ 540, 180} };
	int m_currentTurn = rand()%2+1;
	int m_maxDepth = 1;
	int m_currentDepth = 1;
	int m_gameState = -1;
	char m_difficulty[10];

	//buttons
	Button m_MainMenuButtons[3];
	Button m_easy;
	Button m_medium;
	Button m_hard;

};