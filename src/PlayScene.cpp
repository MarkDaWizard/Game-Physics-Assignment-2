#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "Util.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	TextureManager::Instance()->draw("background", 500, 40, 0, 255, true);

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

}

void PlayScene::update()
{
	updateDisplayList();
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}

void PlayScene::start()
{
	TextureManager::Instance()->load("../Assets/textures/Overwatch.png", "background");

	
	// Set GUI Title
	m_guiTitle = "Play Scene";

	// Player Sprite
	/*m_pPlayer = new Player();
	addChild(m_pPlayer);
	
	//Enemy Sprite
	m_pPlane = new Plane();
	addChild(m_pPlane);*/

	// Thermal Detonator
	m_pBall = new Target();
	addChild(m_pBall);

	// Back Button
	m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	m_pBackButton->getTransform()->position = glm::vec2(300.0f, 400.0f);
	m_pBackButton->addEventListener(CLICK, [&]()-> void
	{
		m_pBackButton->setActive(false);
		TheGame::Instance()->changeSceneState(START_SCENE);
	});

	m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pBackButton->setAlpha(128);
	});

	m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pBackButton->setAlpha(255);
	});
	addChild(m_pBackButton);

	// Next Button
	m_pNextButton = new Button("../Assets/textures/nextButton.png", "nextButton", NEXT_BUTTON);
	m_pNextButton->getTransform()->position = glm::vec2(500.0f, 400.0f);
	m_pNextButton->addEventListener(CLICK, [&]()-> void
	{
		m_pNextButton->setActive(false);
		TheGame::Instance()->changeSceneState(END_SCENE);
	});

	m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pNextButton->setAlpha(128);
	});

	m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pNextButton->setAlpha(255);
	});

	addChild(m_pNextButton);

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 500.0f);

	addChild(m_pInstructionsLabel);
}

void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Physics Controls", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	if(ImGui::Button("Slide"))
	{
		m_pBall->doThrow();
	}
	
	ImGui::Separator();

	static bool isGravityEnabled = false;
	if (ImGui::Checkbox("Gravity", &isGravityEnabled))
	{
		m_pBall->isGravityEnabled = isGravityEnabled;
	}
	
	static int xPlayerPos = 300;
	if (ImGui::SliderInt("Player Position X", &xPlayerPos, 16, 784)) {
		m_pPlayer->getTransform()->position.x = xPlayerPos;
		m_pBall->throwPosition = glm::vec2(xPlayerPos, 300);
	}
	static int xPlanePos = 300;
	if (ImGui::SliderInt("Stormtrooper's Position X", &xPlanePos, 16, 784)) {
		m_pPlane->getTransform()->position.x = xPlanePos;
	}
	if (m_pBall->getTransform()->position.y > 300.0f)
	{
		m_pBall->getRigidBody()->velocity = glm::vec2(0, 0);
	}
	
	static float velocity[2] = { 0,0 };
	if (ImGui::SliderFloat2("Throw Speed", velocity, 0, 500))
	{
		m_pBall->throwSpeed = glm::vec2(velocity[0], -velocity[1]);
	}

	static int rampWidth = 150;
	glm::vec2 point = glm::vec2(150, 400);
	ImGui::SliderInt("Ramp Width", &rampWidth, 0, Config::SCREEN_WIDTH - 200);

	static int rampHeight = 150;
	ImGui::SliderInt("Ramp Height", &rampHeight, 0, Config::SCREEN_HEIGHT - 250);

	Util::DrawLine(point, glm::vec2(point.x, point.y - rampHeight), glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	Util::DrawLine(point, glm::vec2(point.x + rampWidth, point.y), glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	Util::DrawLine(glm::vec2(point.x + rampWidth, point.y), glm::vec2(point.x, point.y - rampHeight), glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	
	ImGui::End();
	ImGui::EndFrame();
	
	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
