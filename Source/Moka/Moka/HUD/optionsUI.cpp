#include "optionsUI.h"
#include "../Resources/resourceIdentifiers.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <cassert>
#include <functional>


OptionsUI::OptionsUI(Fonts::ID font, trmb::FontHolder &fonts, SoundEffects::ID soundEffect, trmb::SoundPlayer &soundPlayer,
	EventGuid leftClickPress, EventGuid leftClickRelease)
: mFrameBuffer(sf::Vector2f(7.0f, 7.0f))
, mOutLineThickness(1.0f)
, mTabSize(sf::Vector2f(55.0f, 20.0f))
, mTabs(leftClickPress)
, mIncDecUI(font, fonts, soundEffect, soundPlayer, leftClickPress, leftClickRelease, sf::Vector2f(20.0f, 20.0f))
, mHorizontalBuffer(1.0f)
, mVerticalBuffer(1.0f)
, mDrawIncDecUI(false)
, mDisableIncrementButtonOfLHSTab(false)
, mDisableDecrementButtonOfLHSTab(false)
, mDisableIncrementButtonOfRHSTab(false)
, mDisableDecrementButtonOfRHSTab(false)
, mDisable(false)
, mHide(false)
, mRestoreBackgroundSize()
{
	mLHSTab = std::make_shared<trmb::GameTab>(Fonts::ID::Main, fonts, soundEffect, soundPlayer, mTabSize);
	mLHSTab->setCallback(std::bind(&OptionsUI::lhsTab, this));

	mRHSTab = std::make_shared<trmb::GameTab>(Fonts::ID::Main, fonts, soundEffect, soundPlayer, mTabSize);
	mRHSTab->setCallback(std::bind(&OptionsUI::rhsTab, this));

	mTabs.pack(mLHSTab);
	mTabs.pack(mRHSTab);
	mTabCount = mTabs.getSize();
	mTabs.setPosition(0.0f, 0.0f);
	mTabs.setVisualScheme(sf::Color(112u, 128u, 144u, 255u), sf::Color(255u, 255u, 255u, 255u), sf::Color(0u, 0u, 0u, 255u)
		, sf::Color(95u, 158u, 160u, 255u), sf::Color(255u, 255u, 255u, 255u), sf::Color(0u, 0u, 0u, 255u)        // Depress
		, sf::Color(160u, 160u, 160u, 255u), sf::Color(224u, 224u, 224u, 255u), sf::Color(128u, 128u, 128u, 255u) // Disable
		, mOutLineThickness);

	mBackground.setPosition(0.0f, 0.0f);
	mBackground.setFillColor(sf::Color(0u, 0u, 0u, 200u));
	mBackground.setOutlineColor(sf::Color(0u, 0u, 0u, 255u));
	mBackground.setOutlineThickness(mOutLineThickness);

	trmb::centerOrigin(mIncDecUI, true, false);
	mIncDecUI.setVisualScheme(sf::Color(95u, 158u, 160u, 255u), sf::Color(255u, 255u, 255u, 255u), sf::Color(0u, 0u, 0u, 255u)
		, sf::Color(162u, 181u, 205u, 255u), sf::Color(255u, 255u, 255u, 255u), sf::Color(0u, 0u, 0u, 255u) // Hover
		, sf::Color(88u, 146u, 148u, 255u), sf::Color(255u, 255u, 255u, 255u), sf::Color(0u, 0u, 0u, 255u)  // Depress
		, sf::Color(0u, 0u, 0u, 0u), sf::Color(224u, 224u, 224u, 75u), sf::Color(0u, 0u, 0u, 255u)	        // Disable
		, mOutLineThickness);

	buildUI();
}

bool OptionsUI::isHidden() const
{
	return mHide;
}

sf::Vector2f OptionsUI::getSize() const
{
	return mBackground.getSize();
}

sf::FloatRect OptionsUI::getRect() const
{
	const sf::Vector2f backgroundPosition = mBackground.getPosition();
	const sf::Vector2f backgroundSize = mBackground.getSize();
	sf::FloatRect rect = sf::FloatRect(backgroundPosition.x, backgroundPosition.y, backgroundSize.x, backgroundSize.y);
	rect = getTransform().transformRect(rect);

	return rect;
}

void OptionsUI::setTabSize(sf::Vector2f size)
{
	mTabSize = size;
	buildUI();
	standardizeCharacterSize();
}

void OptionsUI::setLHSTabText(std::string string)
{
	mLHSTab->setText(string);
	standardizeCharacterSize();
}

void OptionsUI::setRHSTabText(std::string string)
{
	mRHSTab->setText(string);
	standardizeCharacterSize();
}

void OptionsUI::setCharacterSize(unsigned int size)
{
	mLHSTab->setCharacterSize(size);
	mRHSTab->setCharacterSize(size);
}

void OptionsUI::setDisableIncrementButtonOfLHSTab(bool flag, bool useDisableColorScheme)
{
	mDisableIncrementButtonOfLHSTab = flag;
	mIncDecUI.setDisableIncrementButton(flag, useDisableColorScheme);
}

void OptionsUI::setDisableDecrementButtonOfLHSTab(bool flag, bool useDisableColorScheme)
{
	mDisableDecrementButtonOfLHSTab = flag;
	mIncDecUI.setDisableDecrementButton(flag, useDisableColorScheme);
}

void OptionsUI::setDisableIncrementButtonOfRHSTab(bool flag, bool useDisableColorScheme)
{
	mDisableIncrementButtonOfRHSTab = flag;
	mIncDecUI.setDisableIncrementButton(flag, useDisableColorScheme);
}

void OptionsUI::setDisableDecrementButtonOfRHSTab(bool flag, bool useDisableColorScheme)
{
	mDisableDecrementButtonOfRHSTab = flag;
	mIncDecUI.setDisableDecrementButton(flag, useDisableColorScheme);
}

void OptionsUI::handler(const sf::RenderWindow &window, const sf::View &view, const sf::Transform &transform)
{
	if (!mDisable)
	{
		sf::Transform combinedTransform = getTransform() * transform;

		mTabs.handler(window, view, combinedTransform);
		mIncDecUI.handler(window, view, combinedTransform);
	}
}

void OptionsUI::updateIncDecCallbacks(Callback incPurchaseCallback, Callback decPurchaseCallback
	, Callback incRepairCallback, Callback decRepairCallback)
{
	mIncPurchaseCallback = incPurchaseCallback;
	mDecPurchaseCallback = decPurchaseCallback;
	mIncRepairCallback = incRepairCallback;
	mDecRepairCallback = decRepairCallback;
}

void OptionsUI::reset()
{
	// ALW - Resize background
	mBackground.setSize(mBackgroundDefaultSize);

	// ALW - Deactivate tabs
	mTabs.deactivate();

	// ALW - Deactivate IncDec UI
	mDrawIncDecUI = false;
	mIncDecUI.setCallbacks(nullptr, nullptr);
}

void OptionsUI::enable()
{
	mDisable = false;
	mTabs.enable();
	mIncDecUI.enable();
}

void OptionsUI::disable(bool useDisableColorScheme)
{
	mDisable = true;
	mTabs.disable(useDisableColorScheme);
	mIncDecUI.disable(useDisableColorScheme);
}

void OptionsUI::unhide()
{
	mHide = false;
	mBackground.setSize(mRestoreBackgroundSize);

	mTabs.unhide();
	mIncDecUI.unhide();

	enable();
}

void OptionsUI::hide()
{
	// ALW - hide() must be called before unhide(), so the restore values are assigned.
	mHide = true;
	disable(false);

	mRestoreBackgroundSize = mBackground.getSize();
	const sf::Vector2f hideBackground = sf::Vector2f(0.0f, 0.0f);
	mBackground.setSize(hideBackground);

	mTabs.hide();
	mIncDecUI.hide();
}

void OptionsUI::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mBackground, states);
	target.draw(mTabs, states);
	if (mDrawIncDecUI)
		target.draw(mIncDecUI, states);
}

void OptionsUI::standardizeCharacterSize()
{
	mTabs.standardizeCharacterSize();
}

void OptionsUI::buildUI()
{
	// ALW - Set up tabs before background, because the background size depends on the number of tabs in mTabs.
	mLHSTab->setSize(mTabSize);
	mRHSTab->setSize(mTabSize);
	mLHSTab->setPosition(mFrameBuffer.x + 0.0f, mFrameBuffer.y + 0.0f);
	mRHSTab->setPosition(mFrameBuffer.x + mLHSTab->getSize().x + mHorizontalBuffer, mFrameBuffer.y + 0.0f);

	// ALW - Setup background before inc/dec UI, because the position of the in/dec UI depends on the size of the background.
	mBackgroundDefaultSize = sf::Vector2f(mTabSize.x * mTabCount + mHorizontalBuffer * (mTabCount - 1) + mFrameBuffer.x * 2.0f
		, mTabSize.y + mFrameBuffer.y * 2.0f);
	mBackgroundExpandedSize = sf::Vector2f(mBackgroundDefaultSize.x, mBackgroundDefaultSize.y + mVerticalBuffer + mIncDecUI.getSize().y);
	mBackground.setSize(mBackgroundDefaultSize);

	assert(("ALW - Logic Error: The increment button's height is not the same as the decrement button's height!"
		, mLHSTab->getSize().y == mRHSTab->getSize().y));
	mIncDecUI.setPosition(mBackground.getSize().x / 2.0f, mFrameBuffer.y + mLHSTab->getSize().y + mVerticalBuffer);
}

void OptionsUI::lhsTab()
{
	// ALW - Resize background
	mBackground.setSize(mBackgroundExpandedSize);

	// ALW - Activate IncDec UI
	mDrawIncDecUI = true;
	mIncDecUI.setCallbacks(mIncPurchaseCallback, mDecPurchaseCallback);

	// ALW - IncDec UI has been activated, restore enabled/disabled states of increment/decrement button
	if (mDisableIncrementButtonOfLHSTab)
	{
		mIncDecUI.setDisableIncrementButton(true, true);
	}
	else
	{
		mIncDecUI.setDisableIncrementButton(false, false);
	}

	if (mDisableDecrementButtonOfLHSTab)
	{
		mIncDecUI.setDisableDecrementButton(true, true);
	}
	else
	{
		mIncDecUI.setDisableDecrementButton(false, false);
	}
}

void OptionsUI::rhsTab()
{
	// ALW - Resize background
	mBackground.setSize(mBackgroundExpandedSize);

	// ALW - Activate IncDec UI
	mDrawIncDecUI = true;
	mIncDecUI.setCallbacks(mIncRepairCallback, mDecRepairCallback);

	// ALW - IncDec UI has been activated, restore enabled/disabled states of increment/decrement button
	if (mDisableIncrementButtonOfRHSTab)
	{
		mIncDecUI.setDisableIncrementButton(true, true);
	}
	else
	{
		mIncDecUI.setDisableIncrementButton(false, false);
	}

	if (mDisableDecrementButtonOfRHSTab)
	{
		mIncDecUI.setDisableDecrementButton(true, true);
	}
	else
	{
		mIncDecUI.setDisableDecrementButton(false, false);
	}
}

void centerOrigin(OptionsUI &button, bool centerXAxis, bool centerYAxis)
{
	sf::Vector2f size = button.getSize();
	float xAxis = 0.0f;
	float yAxis = 0.0f;

	if (centerXAxis)
		xAxis = std::floor(size.x / 2.0f);

	if (centerYAxis)
		yAxis = std::floor(size.y / 2.0f);

	button.setOrigin(xAxis, yAxis);
}
