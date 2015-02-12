#ifndef WORLD_H
#define WORLD_H

#include "../HUD/chatBoxUI.h"
#include "../HUD/daylightUI.h"
#include "../HUD/optionsUI.h"
#include "../HUD/uiBundle.h"
#include "../HUD/undoUI.h"
#include "../GameObjects/objectGroups.h"

#include "Trambo/Camera/camera.h"
#include "Trambo/Events/eventHandler.h"
#include "Trambo/Resources/resourceHolder.h"
#include "Trambo/SceneNodes/sceneNode.h"
#include "Trambo/Tiles/map.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <array>
#include <random>
#include <vector>


namespace sf
{
	class RenderTarget;
	class RenderWindow;
}

namespace trmb
{
	class Event;
	class SoundPlayer;
}

class HeroNode;
class DoorNode;
class HouseNode;
class WindowNode;

class World : public trmb::EventHandler
{
public:
										World(sf::RenderWindow &window, trmb::FontHolder &fonts, trmb::SoundPlayer &soundPlayer);
										World(const World &) = delete;
	World &								operator=(const World &) = delete;

	void								update(sf::Time dt);
	virtual void						handleEvent(const trmb::Event &gameEvent);
	void								draw();


private:
	typedef								unsigned long EventGuid;


private:
	void								initializeDoorToHouseMap();
	void								initializeWindowToHouseMap();

	void								updateCollisions(sf::Time dt);
	void								mosquitoDoorCollisions();
	void								mosquitoWindowCollisions();

	void								updateSoundPlayer();
	void								configureUIs();
	void								buildScene();
	std::vector<sf::FloatRect>			buildAttachedRects(const InteractiveObject &interactiveObj);
	void								generateSpawnPositions();
	sf::Vector2f						getRandomSpawnPosition();


private:
	enum Layer
	{
		Background,
		Update,
		DoorSelection,
		HouseSelection,
		WindowSelection,
		Selection,
		Mosquitoes,
		Sky,
		UI,
		Camera,
		LayerCount
	};


private:
	const EventGuid								mBeginSimulationEvent;   // ALW - Matches the GUID in the DaylightUI class.
	const EventGuid								mFullscreen;			 // ALW - Matches the GUID in the ToggleFullscreen class.
	const EventGuid								mWindowed;				 // ALW - Matches the GUID in the ToggleFullscreen class.

	const sf::RenderWindow						&mWindow;
	sf::RenderTarget							&mTarget;
	trmb::FontHolder							&mFonts;
	trmb::SoundPlayer							&mSoundPlayer;
	trmb::TextureHolder							mTextures;
	
	trmb::SceneNode								mSceneGraph;
	std::array<trmb::SceneNode *, LayerCount>	mSceneLayers;
	ObjectGroups								mObjectGroups;

	trmb::Map									mMap;
	sf::FloatRect								mWorldBounds;
	trmb::Camera								mCamera;
	sf::Vector2f								mHeroPosition;
	HeroNode									*mHero;

	UIBundle									mUIBundle;
	ChatBoxUI									mChatBoxUI;
	DaylightUI                                  mDaylightUI;
	UndoUI										mBarrelUI;
	UndoUI										mDoorUI;
	UndoUI										mWindowUI;
	OptionsUI									mClinicUI;
	OptionsUI									mHouseUI;

	std::vector<sf::Vector2f>					mSpawnPositions;
	std::random_device							mRandomDevice;
	std::mt19937								mGenerator;
	std::uniform_int_distribution<>				mDistribution;

	bool										mBeginSimulationMode;
	sf::Time									mTotalCollisionTime;
	sf::Time									mUpdateCollisionTime;
	std::map<DoorNode *, HouseNode *>			mDoorToHouse;
	std::map<WindowNode *, HouseNode *>			mWindowToHouse;
};

#endif
