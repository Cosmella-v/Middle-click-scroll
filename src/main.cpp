#include <Geode/Geode.hpp>
#include <Geode/modify/TableView.hpp>
#include <Geode/modify/CCScrollLayerExt.hpp>
#include "middleClick.hpp"

#ifdef GEODE_IS_WINDOWS
#include <windows.h>
#endif

using namespace geode::prelude;

#ifdef GEODE_IS_WINDOWS

enum Direction
{
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

void setCursorForDirection(Direction dir)
{
	HCURSOR cursor = NULL;

	switch (dir)
	{
	case UP:
		cursor = LoadCursor(NULL, MAKEINTRESOURCE(32655));
		break;
	case DOWN:
		cursor = LoadCursor(NULL, MAKEINTRESOURCE(32656));
		break;
	case RIGHT:
		cursor = LoadCursor(NULL, MAKEINTRESOURCE(32657));
		break;
	case LEFT:
		cursor = LoadCursor(NULL, MAKEINTRESOURCE(32658));
		break;
	case NONE:
	default:
		cursor = LoadCursor(NULL, MAKEINTRESOURCE(32654));
		break;
	}

	if (cursor)
	{
		SetCursor(cursor);
	}
}

void resetCursor()
{
	SetCursor(LoadCursor(NULL, IDC_ARROW));
}
#endif

float scrollSpeedFactor = 0.1f;
float maxSpeed = 50.0f;

class $modify(TableViewMouseScroll, TableView)
{
	struct Fields
	{
		bool isScrolling = false;
		bool m_movement = false;
		bool m_Scheduled = false;
		CCPoint initialClickPos;
		CCPoint scrollVelocity;
		bool currentInput = false;
	};
#ifdef GEODE_IS_WINDOWS
	void OnExit()
	{
		resetCursor();
		return TableView::onExit();
	};
#endif
	void registerWithTouchDispatcher()
	{
		if (m_fields->m_Scheduled == false)
		{
			m_fields->m_Scheduled = true;
			if (m_unused2)
			{
				this->schedule(schedule_selector(TableViewMouseScroll::ForceScrollUpdate));
			}
		}
		return TableView::registerWithTouchDispatcher(); // HOLY SHIT A ALMOST INIT FUNCTION!
	}
	bool PositionOverlap(CCPoint Provided)
	{
		CCNode *Parent = this->getParent();
		if (Parent == nullptr)
			Parent = this;
		return this->boundingBox().containsPoint(Parent->convertToNodeSpace(Provided));
	}

	void ForceScrollUpdate(float dt)
	{
		bool CurrentStagedInput = Mouse::isMiddleClickPressed();
		auto mousePos = getMousePos();
		if (CurrentStagedInput == m_fields->currentInput)
			goto Updated;
		m_fields->currentInput = CurrentStagedInput;

		if (CurrentStagedInput || m_fields->m_movement)
		{
			if (!m_fields->isScrolling)
			{
				if (!PositionOverlap(mousePos))
					return;
				m_fields->isScrolling = true;
				m_fields->initialClickPos = mousePos;
				m_fields->scrollVelocity = CCPointZero;
			}
			else
			{
				m_fields->m_movement = false;
				m_fields->isScrolling = false;
#ifdef GEODE_IS_WINDOWS
				resetCursor();
#endif
			}
		}
		goto Updated;
	Updated:
		if (m_fields->isScrolling)
		{

			CCPoint offset = ccpSub(m_fields->initialClickPos, mousePos);
			m_fields->scrollVelocity = ccpMult(offset, scrollSpeedFactor);

			// Clamp
			m_fields->scrollVelocity.x = std::clamp(m_fields->scrollVelocity.x, -maxSpeed, maxSpeed);
			m_fields->scrollVelocity.y = std::clamp(m_fields->scrollVelocity.y, -maxSpeed, maxSpeed);
			m_fields->m_movement = m_fields->scrollVelocity.y != 0 || m_fields->scrollVelocity.x != 0;
#ifdef GEODE_IS_WINDOWS
			// windows only supports the mouse icon thing since i have no idea who didn't care lol, i didn't care enough to make some lol
			if (m_fields->m_movement)
			{
				if (m_fields->scrollVelocity.y > 0)
				{
					setCursorForDirection(DOWN);
				}
				else if (m_fields->scrollVelocity.y < 0)
				{
					setCursorForDirection(UP);
				} /*else if ((m_fields->scrollVelocity.x > 0)) {
					setCursorForDirection(RIGHT);
				} else {
					setCursorForDirection(LEFT);
				};*/
			}
			else
			{
				setCursorForDirection(NONE);
			};
#endif
			scrollWheel(m_fields->scrollVelocity.y, m_fields->scrollVelocity.x);
		};
		return;
	}
};

class $modify(ScrollLayerMouseScroll, CCScrollLayerExt)
{
	struct Fields
	{
		bool isScrolling = false;
		bool m_movement = false;
		bool m_Scheduled = false;
		CCPoint initialClickPos;
		CCPoint scrollVelocity;
		bool currentInput = false;
	};
	void registerWithTouchDispatcher()
	{
		if (m_fields->m_Scheduled == false)
		{
			m_fields->m_Scheduled = true;
			if (!m_disableMovement)
			{
				this->schedule(schedule_selector(ScrollLayerMouseScroll::ForceScrollUpdate));
			}
		}
		return CCScrollLayerExt::registerWithTouchDispatcher(); // HOLY SHIT A ALMOST INIT FUNCTION!
	}
	bool PositionOverlap(CCPoint Provided)
	{
		CCNode *Parent = this->getParent();
		if (Parent == nullptr)
			Parent = this;
		return this->boundingBox().containsPoint(Parent->convertToNodeSpace(Provided));
	}

	void ForceScrollUpdate(float dt)
	{
		bool CurrentStagedInput = Mouse::isMiddleClickPressed();
		auto mousePos = getMousePos();
		if (CurrentStagedInput == m_fields->currentInput)
			goto Updated;
		m_fields->currentInput = CurrentStagedInput;

		if (CurrentStagedInput || m_fields->m_movement)
		{
			if (!m_fields->isScrolling)
			{
				if (!PositionOverlap(mousePos))
					return;
				m_fields->isScrolling = true;
				m_fields->initialClickPos = mousePos;
				m_fields->scrollVelocity = CCPointZero;
			}
			else
			{
				m_fields->m_movement = false;
				m_fields->isScrolling = false;
#ifdef GEODE_IS_WINDOWS
				resetCursor();
#endif
			}
		}
		goto Updated;
	Updated:
		if (m_fields->isScrolling)
		{

			CCPoint offset = ccpSub(m_fields->initialClickPos, mousePos);
			m_fields->scrollVelocity = ccpMult(offset, scrollSpeedFactor);

			// Clamp
			m_fields->scrollVelocity.x = std::clamp(m_fields->scrollVelocity.x, -maxSpeed, maxSpeed);
			m_fields->scrollVelocity.y = std::clamp(m_fields->scrollVelocity.y, -maxSpeed, maxSpeed);
			m_fields->m_movement = m_fields->scrollVelocity.y != 0 || m_fields->scrollVelocity.x != 0;
#ifdef GEODE_IS_WINDOWS
			// windows only supports the mouse icon thing since i have no idea who didn't care lol, i didn't care enough to make some lol
			if (m_fields->m_movement)
			{
				if (m_disableVertical)
				{
					setCursorForDirection(NONE);
				}
				else
				{
					if (m_fields->scrollVelocity.y > 0)
					{
						setCursorForDirection(DOWN);
					}
					else if (m_fields->scrollVelocity.y < 0)
					{
						setCursorForDirection(UP);
					}
				}
			}
			else
			{
				setCursorForDirection(NONE);
			};
#endif
			scrollLayer(m_fields->scrollVelocity.y);
		};
		return;
	}
};
