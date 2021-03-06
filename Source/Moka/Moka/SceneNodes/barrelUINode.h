#ifndef BARREL_UI_NODE_H
#define BARREL_UI_NODE_H

#include "Trambo/Events/eventHandler.h"
#include "Trambo/SceneNodes/sceneNode.h"


namespace sf
{
	class RenderStates;
	class RenderTarget;
}

namespace trambo
{
	class Event;
}

class UndoUI;

class BarrelUINode : public trmb::SceneNode, trmb::EventHandler
{
// ALW - This motivation for this class is to allow the UI to be attached to
// ALW - the foreground layer. This way the UI is drawn on top of everything.
public:
	explicit			BarrelUINode(const UndoUI &barrelUI);
						BarrelUINode(const BarrelUINode &) = delete;
	BarrelUINode &		operator=(const BarrelUINode &) = delete;

	virtual void		handleEvent(const trmb::Event &gameEvent) override final;


private:
	typedef unsigned long EventGuid;


private:
	virtual void		drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;


private:
	const EventGuid     mDrawUI;	  // ALW - Matches the GUID in the BarrelNode class.
	const EventGuid     mDoNotDrawUI; // ALW - Matches the GUID in the BarrelNode class.
	const UndoUI		&mUI;
	bool				mIsDrawn;
};

#endif
