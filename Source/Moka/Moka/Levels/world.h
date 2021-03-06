#ifndef WORLD_H
#define WORLD_H

#include "../EventDialog/didYouKnow.h"
#include "../HUD/chatBoxUI.h"
#include "../HUD/daylightUI.h"
#include "../HUD/mainTrackerUI.h"
#include "../HUD/optionsUI.h"
#include "../HUD/scoreboardUI.h"
#include "../HUD/uiBundle.h"
#include "../HUD/undoUI.h"
#include "../EventDialog/eventDialogManager.h"
#include "../GameObjects/objectGroups.h"

#include "Trambo/Camera/camera.h"
#include "Trambo/Events/event.h"
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
#include <string>
#include <vector>


namespace sf
{
	class RenderTarget;
	class RenderWindow;
}

namespace trmb
{
	class MusicPlayer;
	class SoundPlayer;
}

class HeroNode;
class BarrelNode;
class ClinicNode;
class DoorNode;
class HouseNode;
class ResidentNode;
class WindowNode;

class World : public trmb::EventHandler
{
public:
										World(sf::RenderWindow &window, trmb::FontHolder &fonts, trmb::SoundPlayer &soundPlayer
											, trmb::MusicPlayer &MusicPlayer);
										World(const World &) = delete;
	World &								operator=(const World &) = delete;

	bool								isScoreboardFinished() const;

	void								update(sf::Time dt);
	virtual void						handleEvent(const trmb::Event &gameEvent);
	void								draw();


private:
	enum Direction
	{
		Up    = 0,
		Down  = 1,
		Left  = 2,
		Right = 3,
		Count
	};


private:
	typedef								unsigned long EventGuid;


private:
	bool								isFirstTransmission() const;
	bool								hasMosquitoPopulationDoubled();
	bool								existsClinicEventDialog() const;
	bool								existsDoorEventDialog() const;
	bool								existsHouseEventDialog() const;
	bool								existsWindowEventDialog() const;

	void								updateScheduledEventDialog(sf::Time dt);
	void								calculateTotalScheduledEventDialogs();
	void								displayClinicEventDialog();
	void								displayDoorEventDialog();
	void								displayHouseEventDialog();
	void								displayWindowEventDialog();

	void								initializeDoorToHouseMap();
	void								initializeWindowToHouseMap();
	void								initializeResidentToHouseMap();

	void								updateCollisions(sf::Time dt);
	void								mosquitoDoorCollisions();
	void								mosquitoWindowCollisions();
	void								mosquitoResidentCollisions();

	void								spawnBarrelMosquitoes();
	void								spawnBarrelMosquito(std::size_t barrelID);

	void								updateSoundPlayer();
	void								configureUIs();
	void								buildScene();
	std::vector<sf::FloatRect>			buildAttachedRects(const InteractiveObject &interactiveObj);
	void								generateSpawnPositions();
	sf::Vector2f						getRandomSpawnPosition() const;
	sf::Vector2f						getRandomSpawnPositionNearBarrel(std::size_t barrelID) const;
	std::string							getRandomHouseName(int exlusiveMax) const;
	int									getHouseCount() const;


private:
	enum Layer
	{
		Background,
		Update,
		ClinicSelection,
		DoorSelection,
		HouseSelection,
		WindowSelection,
		Selection,
		Residents,
		Mosquitoes,
		Sky,
		UI,
		Camera,
		LayerCount
	};


private:
	const trmb::Event							mBeginScoreboardEvent;   // ALW - Is sent from here.
	const EventGuid								mFullscreen;			 // ALW - Matches the GUID in the ToggleFullscreen class.
	const EventGuid								mWindowed;				 // ALW - Matches the GUID in the ToggleFullscreen class.
	const EventGuid								mCreateTextPrompt;		 // ALW - Matches the GUID in the ChatBoxUI class.
	const EventGuid								mClearTextPrompt;		 // ALW - Matches the GUID in the ChatBoxUI class.
	const EventGuid								mBeginSimulationEvent;   // ALW - Matches the GUID in the DaylightUI class.
	const EventGuid								mSpawnMosquitoEvent;	 // ALW - matches the GUID in the BarrelNode class.

	const sf::RenderWindow						&mWindow;
	sf::RenderTarget							&mTarget;
	trmb::FontHolder							&mFonts;
	trmb::SoundPlayer							&mSoundPlayer;
	trmb::MusicPlayer							&mMusicPlayer;
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
	MainTrackerUI								mMainTrackerUI;
	UndoUI										mBarrelUI;
	UndoUI										mDoorUI;
	UndoUI										mWindowUI;
	OptionsUI									mClinicUI;
	OptionsUI									mHouseUI;

	std::vector<sf::Vector2f>					mSpawnPositions;

	int											mMosquitoCount;
	int											mResidentCount;

	bool										mDisableInput;
	bool										mSimulationMode;
	sf::Time									mTotalCollisionTime;
	sf::Time									mUpdateCollisionTime;
	std::vector<std::size_t>					mBarrelIDsToSpawnMosquito;
	std::vector<BarrelNode *>					mBarrels;
	int											mClinicCount;					// ALW - Only allow one clinic
	ClinicNode									*mClinic;
	std::map<DoorNode *, HouseNode *>			mDoorToHouse;
	std::map<WindowNode *, HouseNode *>			mWindowToHouse;
	std::map<ResidentNode *, HouseNode *>		mResidentToHouse;

	// ALW - Event Dialog
	EventDialogManager							mEventDialogManager;
	DidYouKnow									mDidYouKnow;

	// ALW - Scheduled Event Dialog
	std::vector<DoorNode *>						mDoors;
	std::vector<HouseNode *>					mHouses;
	std::vector<WindowNode *>					mWindows;
	bool										mDisplayClinicEventDialog;
	bool										mDisplayDoorEventDialog;
	bool										mDisplayHouseEventDialog;
	bool										mDisplayWindowEventDialog;
	bool										mDisplaySimulationFinishedEventDialog;

	// ALW - Unscheduled Event Dialog
	int											mTransmissionCount;
	bool										mDisableMosquitoPopulationCheck;

	// ALW - Scoreboard
	ScoreboardUI								mScoreboardUI;
};

#endif
