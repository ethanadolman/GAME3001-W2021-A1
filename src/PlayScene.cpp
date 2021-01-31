#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	Util::DrawLine(m_pSpaceShip->getTransform()->position, (m_pSpaceShip->getTransform()->position + m_pSpaceShip->getOrientation() * 100.0f));
	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	updateDisplayList();
	CollisionManager::AABBCheck(m_pSpaceShip, m_pTarget);
	if (m_pObstacle->isEnabled()) {
		//CollisionManager::AABBCheck(m_pSpaceShip, m_pObstacle);
		//CollisionManager::circleAABBCheck(m_pSpaceShip, m_pObstacle);
		if (CollisionManager::lineAABBCheck(m_pSpaceShip, m_pObstacle))
		{
			std::cout << "line collision with obstacle!" << std::endl;
			m_pSpaceShip->setAI(AI::AVOIDING);
			//m_pSpaceShip->setRotation(m_pSpaceShip->getRotation() + m_pSpaceShip->getTurnRate());
		}
		else if(m_pSpaceShip->getAI() != AI::IDLE)
		{
			m_pSpaceShip->setAI(AI::SEEKING);
		}
	}
}

void PlayScene::clean()
{
	removeAllChildren();
	SoundManager::Instance().stopMusic();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_BACKSPACE))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_SPACE))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_4) && (m_pSpaceShip->getAI() != AI::IDLE || m_pSpaceShip->getAI() != AI::AVOIDING))
	{
		m_pObstacle->setEnabled(true);
		m_pSpaceShip->setAI(AI::IDLE);
		std::cout << "Avoiding\n";
		m_pSpaceShip->getTransform()->position = glm::vec2(100.0f, 400.0f);
		m_pSpaceShip->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pSpaceShip->setRotation(0.0f); // set Angle to 0 degrees
		m_pTarget->getTransform()->position = glm::vec2(400.0f, 400.0f);
		m_pSpaceShip->setDestination(m_pTarget->getTransform()->position);
		m_pSpaceShip->setTurnRate(5.0f);
		m_pSpaceShip->setMaxSpeed(1.0f);
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_3) && m_pSpaceShip->getAI() != AI::ARRIVING)
	{
		m_pObstacle->setEnabled(false);
		m_pSpaceShip->setAI(AI::ARRIVING);
		std::cout << "Arriving\n";
		m_pSpaceShip->getTransform()->position = glm::vec2(50.0f, 50.0f);
		m_pSpaceShip->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pSpaceShip->setRotation(0.0f); // set Angle to 0 degrees
		m_pTarget->getTransform()->position = glm::vec2(400.0f, 400.0f);
		m_pSpaceShip->setDestination(m_pTarget->getTransform()->position);
		m_pSpaceShip->setTurnRate(10.0f);
		m_pSpaceShip->setMaxSpeed(5.0f);
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2) && m_pSpaceShip->getAI() != AI::FLEEING)
	{
		m_pObstacle->setEnabled(false);
		m_pSpaceShip->setAI(AI::FLEEING);
		std::cout << "Fleeing\n";
		m_pSpaceShip->getTransform()->position = glm::vec2(300.0f, 250.0f);
		m_pSpaceShip->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pSpaceShip->setRotation(0.0f); // set Angle to 0 degrees
		m_pTarget->getTransform()->position = glm::vec2(500.0f, 350.0f);
		m_pSpaceShip->setDestination(m_pTarget->getTransform()->position);
		m_pSpaceShip->setTurnRate(5.0f);
		m_pSpaceShip->setMaxSpeed(5.0f);
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1) && m_pSpaceShip->getAI() != AI::SEEKING)
	{
		m_pObstacle->setEnabled(false);
		m_pSpaceShip->setAI(AI::SEEKING);
		std::cout << "Seeking\n";
		m_pSpaceShip->getTransform()->position = glm::vec2(100.0f, 100.0f);
		m_pSpaceShip->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pSpaceShip->setRotation(0.0f); // set Angle to 0 degrees
		m_pTarget->getTransform()->position = glm::vec2(550.0f, 450.0f);
		m_pSpaceShip->setDestination(m_pTarget->getTransform()->position);
		m_pSpaceShip->setTurnRate(10.0f);
		m_pSpaceShip->setMaxSpeed(5.0f);
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	m_pTarget = new Target();
	m_pTarget->getTransform()->position = glm::vec2(550.0f, 450.0f);
	addChild(m_pTarget);
	//m_pTarget->setEnabled(false);

	

	// instantiating spaceship
	m_pSpaceShip = new SpaceShip();
	m_pSpaceShip->getTransform()->position = glm::vec2(100.0f, 100.0f);
	m_pSpaceShip->setEnabled(false);
	m_pSpaceShip->setDestination(m_pTarget->getTransform()->position);
	addChild(m_pSpaceShip);

	m_pObstacle = new Obstacle();
	m_pObstacle->getTransform()->position = glm::vec2(50.0f, 100.0f);
	addChild(m_pObstacle);
	if (m_pSpaceShip->getAI() != AI::IDLE) { m_pObstacle->setEnabled(false); }
	else { m_pObstacle->setEnabled(true); };

	SoundManager::Instance().load("../Assets/audio/racetomars.mp3", "rtm", SOUND_MUSIC);
	SoundManager::Instance().setMusicVolume(64);
	SoundManager::Instance().playMusic("rtm", -1);
}

void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("GAME3001 - Assignment 1", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	static float speed = 5.0f;
	if(ImGui::SliderFloat("MaxSpeed", &speed, 0.0f, 50.0f))
	{
		m_pSpaceShip->setMaxSpeed(speed);
	}

	static float acceleration_rate = 10.0f;
	if(ImGui::SliderFloat("Acceleration Rate", &acceleration_rate, 0.0f, 25.0f))
	{
		m_pSpaceShip->setAccelerationRate(acceleration_rate);
	}

	static float angleInRadians = m_pSpaceShip->getRotation();
	if(ImGui::SliderAngle("Orientation Angle", &angleInRadians))
	{
		m_pSpaceShip->setRotation(angleInRadians * Util::Rad2Deg);
	}

	static float turn_rate = 5.0f;
	if(ImGui::SliderFloat("Turn Rate", &turn_rate, 0.0f, 10.0f))
	{
		m_pSpaceShip->setTurnRate(turn_rate);
	}
	
	if(ImGui::Button("Start"))
	{
		m_pSpaceShip->setEnabled(true);
		m_pSpaceShip->setMaxSpeed(speed);
		m_pSpaceShip->setAccelerationRate(acceleration_rate);
		m_pSpaceShip->setRotation(angleInRadians * Util::Rad2Deg);
		m_pSpaceShip->setTurnRate(turn_rate);
	}

	ImGui::SameLine();
	
	if (ImGui::Button("Reset"))
	{
		
		m_pSpaceShip->setEnabled(false);
		m_pObstacle->setEnabled(false);
		m_pSpaceShip->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pSpaceShip->setRotation(0.0f); // set Angle to 0 degrees
		switch (m_pSpaceShip->getAI())
		{
		case AI::FLEEING:
			m_pSpaceShip->getTransform()->position = glm::vec2(300.0f, 100.0f);
			turn_rate = 5.0f;
			acceleration_rate = 10.0f;
			speed = 5.0f;
			angleInRadians = m_pSpaceShip->getRotation();
			break;
		case AI::SEEKING:
			m_pSpaceShip->getTransform()->position = glm::vec2(100.0f, 100.0f);
			turn_rate = 10.0f;
			acceleration_rate = 10.0f;
			speed = 5.0f;
			angleInRadians = m_pSpaceShip->getRotation();
			break;
		case AI::ARRIVING:
			m_pSpaceShip->getTransform()->position = glm::vec2(50.0f, 50.0f);
			turn_rate = 10.0f;
			acceleration_rate = 10.0f;
			speed = 5.0f;
			angleInRadians = m_pSpaceShip->getRotation();
		case AI::IDLE:
		case AI::AVOIDING:
			m_pObstacle->setEnabled(true);
			m_pSpaceShip->getTransform()->position = glm::vec2(50.0f, 50.0f);
			turn_rate = 5.0f;
			acceleration_rate = 10.0f;
			speed = 1.0f;
			angleInRadians = m_pSpaceShip->getRotation();
			break;
		}
	}

	ImGui::Separator();

	static float targetPosition[2] = { m_pTarget->getTransform()->position.x, m_pTarget->getTransform()->position.y};
	if(ImGui::SliderFloat2("Target", targetPosition, 0.0f, 800.0f))
	{
		m_pTarget->getTransform()->position = glm::vec2(targetPosition[0], targetPosition[1]);
		m_pSpaceShip->setDestination(m_pTarget->getTransform()->position);
	}

	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
