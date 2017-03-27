#pragma once

#include "Application.h"
#include "Renderer2D.h"

class Testing2DThingsApp : public aie::Application {
public:

	Testing2DThingsApp();
	virtual ~Testing2DThingsApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
};