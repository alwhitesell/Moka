#ifndef RESIDENT_NODE_H
#define RESIDENT_NODE_H

#include "Trambo/Events/eventStr.h"
#include "Trambo/Events/eventHandler.h"
#include "Trambo/SceneNodes/sceneNode.h"

#include <SFML/Graphics/Rect.hpp>
// #include <SFML/Graphics/CircleShape.hpp>  // ALW - mDebugShape
// #include <SFML/Graphics/RenderStates.hpp> // ALW - mDebugShape
// #include <SFML/Graphics/RenderTarget.hpp> // ALW - mDebugShape


namespace sf
{
	class Time;
}

namespace trambo
{
	class Event;
}

class HouseNode;

class ResidentNode : public trmb::SceneNode, trmb::EventHandler
{
public:
							ResidentNode(int residentID, bool hasMalaria, const HouseNode * const houseNode);
							ResidentNode(const ResidentNode &) = delete;
	ResidentNode &			operator=(const ResidentNode &) = delete;

	bool					hasMalaria() const;
	bool					isBitten(int totalNewNets, int totalOldNets) const;
	bool					isCured(int RDTs, int ACTs) const;

	virtual sf::FloatRect	getBoundingRect() const override;

	void					contractMalaria();

	virtual void			handleEvent(const trmb::Event &gameEvent) override final;


private:
	enum ResidentCount
	{
		Resident0 = 0,
		Resident1 = 1,
		Resident2 = 2,
		Resident3 = 3,
		Resident4 = 4,
		Resident5 = 5,
		Resident6 = 6,
	};

	enum Stock
	{
		None   = 0,
		Small  = 1,
		Medium = 2,
		Large  = 3,
	};


private:
	using					EventGuid = unsigned long;


private:
//	virtual void			drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override; // ALW - mDebugShape

	bool					isNetDamaged(int totalDamagedNets) const;
	bool					isNetMint(int totalMintNets, int totalDamagedNets) const;

	bool					isDiagnosed(int RDTs) const;
	bool					isTreated(int ACTs) const;

	void					sendCureMsg();
	void					sendMalariaMsg();
	void					generateSpawnPosition(sf::FloatRect houseBoundingRect);


private:
	const EventGuid         mBeginSimulationEvent;	// ALW - Matches the GUID in the DaylightUI class.

	const trmb::EventStr	mCured0;				// ALW - Is sent from here.
	const trmb::EventStr	mCured1;				// ALW - Is sent from here.
	const trmb::EventStr	mCured2;				// ALW - Is sent from here.
	const trmb::EventStr	mCured3;				// ALW - Is sent from here.
	const trmb::EventStr	mCured4;				// ALW - Is sent from here.
	const trmb::EventStr	mCured5;				// ALW - Is sent from here.
	const trmb::EventStr	mCured6;				// ALW - Is sent from here.

	const trmb::EventStr	mHasMalaria0;			// ALW - Is sent from here.
	const trmb::EventStr	mHasMalaria1;			// ALW - Is sent from here.
	const trmb::EventStr	mHasMalaria2;			// ALW - Is sent from here.
	const trmb::EventStr	mHasMalaria3;			// ALW - Is sent from here.
	const trmb::EventStr	mHasMalaria4;			// ALW - Is sent from here.
	const trmb::EventStr	mHasMalaria5;			// ALW - Is sent from here.
	const trmb::EventStr	mHasMalaria6;			// ALW - Is sent from here.

	const int				mResidentID;
	const HouseNode * const mHouseNode;
	bool					mHasMalaria;
//	sf::CircleShape			mDebugShape;
};

#endif
