#include <Geode/Geode.hpp>
#include <Geode/modify/TableView.hpp>
#include <Geode/modify/CCScrollLayerExt.hpp>
#include "Mouse.hpp"

using namespace geode::prelude;

Mouse* UserMouse = Mouse::get();

// both classes use this :troll:
template <typename Derived, typename Base>
struct MouseScrollClass : Modify<Derived, Base> {
    struct Fields {
        bool m_scrolling = false;
        bool m_movement = false;
        CCPoint m_clickPos;
        CCPoint m_velocity;
        bool m_currentInput = false;
    };

    void forceScrollUpdate(float) {
		if (!UserMouse->m_windowActive) {
			this->m_fields->m_scrolling = false;
			return;
		}; // no scrolling when tabbed out

        if constexpr (requires { this->m_disableMovement; }) {
            if (this->m_disableMovement) return;
        }

        bool MiddleClickInput = UserMouse->isMiddleClickPressed();
        CCPoint mousePos = getMousePos();

        if (MiddleClickInput != this->m_fields->m_currentInput) {
            this->m_fields->m_currentInput = MiddleClickInput;

            if (MiddleClickInput || this->m_fields->m_movement) {
                if (!this->m_fields->m_scrolling) {
                    if (!positionOverlaps(mousePos)) return;

                    this->m_fields->m_scrolling = true;
                    this->m_fields->m_clickPos = mousePos;
                    this->m_fields->m_velocity = CCPointZero;
                } else {
                    this->m_fields->m_movement = false;
                    this->m_fields->m_scrolling = false;
                    UserMouse->resetCursor();
                }
            }
        }

        if (this->m_fields->m_scrolling) {
            CCPoint offset = ccpSub(this->m_fields->m_clickPos, mousePos);
            this->m_fields->m_velocity = ccpMult(offset, UserMouse->m_scrollSpeedFactor);

            CCPoint vel = this->m_fields->m_velocity;
            vel.x = geode::utils::clamp(vel.x, -UserMouse->m_maxSpeed, UserMouse->m_maxSpeed);
            vel.y = geode::utils::clamp(vel.y, -UserMouse->m_maxSpeed, UserMouse->m_maxSpeed);
            this->m_fields->m_movement =  vel.y != 0;

            if (this->m_fields->m_movement) {
                if constexpr (requires { this->m_disableVertical; }) {
                    if (this->m_disableVertical) {
                        UserMouse->setCursorForDirection(MouseDrag::ALL);
                    } else {
                        UserMouse->setCursorForDirection(vel.y > 0 ? MouseDrag::DOWN :
                                                          vel.y < 0 ? MouseDrag::UP :
                                                                      MouseDrag::ALL);
                    }
                } else {
                    UserMouse->setCursorForDirection(vel.y > 0 ? MouseDrag::DOWN :
                                                      vel.y < 0 ? MouseDrag::UP :
                                                                  MouseDrag::ALL);
                }
            } else {
                UserMouse->setCursorForDirection(MouseDrag::ALL);
            }

            if constexpr (std::is_base_of_v<TableView, Base>) {
                this->scrollWheel(vel.y, vel.x);
            } else if constexpr (std::is_base_of_v<CCScrollLayerExt, Base>) {
                this->scrollLayer(vel.y);
            }
        }
    }

    void registerWheel() {
        this->schedule(schedule_selector(Derived::forceScrollUpdate));
    }

    bool positionOverlaps(CCPoint Provided) {
        CCNode *Parent = this->getParent();
		if (Parent == nullptr)
			Parent = this;
		return this->boundingBox().containsPoint(Parent->convertToNodeSpace(Provided));
    }
	
};

struct ScrollLayerMouseScroll : MouseScrollClass<ScrollLayerMouseScroll, CCScrollLayerExt> {
    using MouseScrollClass::MouseScrollClass;
	void registerWithTouchDispatcher() // it's inlined and hates me
	{
		registerWheel();
		return CCScrollLayerExt::registerWithTouchDispatcher();
	}
};

struct TableViewMouseScroll : MouseScrollClass<TableViewMouseScroll, TableView> {
	void registerWithTouchDispatcher() // it's inlined and hates me
	{
		if (m_unused2)
		{
			registerWheel();
		}
		return TableView::registerWithTouchDispatcher(); 
	}
    using MouseScrollClass::MouseScrollClass;
};