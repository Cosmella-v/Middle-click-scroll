#include <Geode/Geode.hpp>
#include <Geode/modify/TableView.hpp>
#include <Geode/modify/CCScrollLayerExt.hpp>
#include "Mouse.hpp"

Mouse* UserMouse = Mouse::get();
using namespace geode::prelude;

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
	void OnExit()
	{
		UserMouse->resetCursor();
		return TableView::onExit();
	};
	void registerWithTouchDispatcher()
	{
		if (m_unused2)
		{
			this->schedule(schedule_selector(TableViewMouseScroll::ForceScrollUpdate));
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
		bool CurrentStagedInput = UserMouse->isMiddleClickPressed();
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
				UserMouse->resetCursor();
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

			if (m_fields->m_movement)
			{
				if (m_fields->scrollVelocity.y > 0)
				{
					UserMouse->setCursorForDirection(MouseDrag::DOWN);
				}
				else if (m_fields->scrollVelocity.y < 0)
				{
					UserMouse->setCursorForDirection(MouseDrag::UP);
				}
			}
			else
			{
				UserMouse->setCursorForDirection(MouseDrag::NONE);
			};
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
		this->schedule(schedule_selector(ScrollLayerMouseScroll::ForceScrollUpdate));
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
		if (m_disableMovement) return;
		bool CurrentStagedInput = UserMouse->isMiddleClickPressed();
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
				UserMouse->resetCursor();
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
			if (m_fields->m_movement)
			{
				if (m_disableVertical)
				{
					UserMouse->setCursorForDirection(MouseDrag::NONE);
				}
				else
				{
					if (m_fields->scrollVelocity.y > 0)
					{
						UserMouse->setCursorForDirection(MouseDrag::DOWN);
					}
					else if (m_fields->scrollVelocity.y < 0)
					{
						UserMouse->setCursorForDirection(MouseDrag::UP);
					}
				}
			}
			else
			{
				UserMouse->setCursorForDirection(MouseDrag::NONE);
			};
			scrollLayer(m_fields->scrollVelocity.y);
		};
		return;
	}
};
