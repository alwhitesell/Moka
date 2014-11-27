#ifndef WINDOW_NODE_H
#define WINDOW_NODE_H

#include "PreventionNode.h"
#include "../HUD/undoUI.h"

#include <deque>
#include <vector>


namespace sf
{
	class RenderStates;
	class RenderTarget;
	class RenderWindow;
	class Time;
	class View;
}

namespace trmb
{
	class Event;
	class SoundPlayer;
}

class ChatBox;
class InteractiveObject;
class UIBundle;

class WindowNode : public PreventionNode
{
private:
	using CallbackPair = UndoUI::CallbackPair;


public:
								WindowNode(const InteractiveObject &interactiveObject, const sf::RenderWindow &window
									, const sf::View &view, UIBundle &uiBundle, trmb::SoundPlayer &soundPlayer, ChatBox &chatBox);
								WindowNode(const WindowNode &) = delete;
	WindowNode &				operator=(const WindowNode &) = delete;

	virtual void				handleEvent(const trmb::Event &gameEvent) override final;


private:
	virtual void				drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override final;
	virtual void				updateCurrent(sf::Time dt) override final;
	virtual void				activate() override final;
	void						updateUndoUI();

	void						addScreen();
	void						undoScreen();
	void						closeWindow();
	void						openWindow();


private:
	const trmb::Event			mWindowUIActivated;
	const EventGuid				mLeftClickPress;   // ALW - Matches the GUID in the Controller class.
	trmb::SoundPlayer			&mSoundPlayer;
	ChatBox						&mChatBox;
	bool						mWindowUIActive;

	// ALW - STL containers are used here, so the interface of the UndoUI class is the same for an instance of UndoUI with 
	// ALW - one or multiple UI elements.
	std::vector<CallbackPair>	mCallbackPairs;
	std::deque<bool>			mUIElemStates;		// ALW - Tracks which button is to be displayed and hidden.
};

#endif