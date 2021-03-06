/*****************************************************************************/
/*                             Spike SourceCode                              */
/*                                                                           */
/* File created by: Fahim Fuad                                               */
/* Other editors: None                                                       */
/*                                                                           */
/*   Licensed under the Apache License, Version 2.0 (the "License");         */
/*   you may not use this file except in compliance with the License.        */
/*   You may obtain a copy of the License at                                 */
/*                                                                           */
/*       http://www.apache.org/licenses/LICENSE-2.0                          */
/*                                                                           */
/*   Unless required by applicable law or agreed to in writing, software     */
/*   distributed under the License is distributed on an "AS IS" BASIS,       */
/*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*/
/*   See the License for the specific language governing permissions and     */
/*   limitations under the License.                                          */
/*****************************************************************************/
#pragma once
#include <functional>
#include <sstream>
#include "Spike/Core/Base.h"

namespace Spike
{
    // Events in Spike are currently blocking, meaning when an event occurs it
    // immediately gets dispatched and must be dealt with right then an there.
    // For the future, a better strategy might be to buffer events in an event
    // bus and process them during the "event" part of the update stage.

    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    enum EventCategory
    {
        None = 0,
        EventCategoryApplication  = BIT(0),
        EventCategoryInput        = BIT(1),
        EventCategoryKeyboard     = BIT(2),
        EventCategoryMouse        = BIT(3),
        EventCategoryMouseButton  = BIT(4)
    };

    #define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
        virtual EventType GetEventType() const override { return GetStaticType(); }\
        virtual const char* GetName() const override { return #type; }

    #define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }
//MAIN EVENT CLASS
    class  Event
    {
    public:
        virtual ~Event() = default;
        bool Handled = false;
        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetName(); }

        bool IsInCategory(EventCategory category) { return (GetCategoryFlags() & category);}
    };
//EVENT DISPATCHER
    class EventDispatcher
    {
    public:
        EventDispatcher(Event& event)
            : m_Event(event)
        {
        }

        template<typename T, typename F>
        bool Dispatch(const F& func)
        {
            if (m_Event.GetEventType() == T::GetStaticType())
            {
                m_Event.Handled = func(static_cast<T&>(m_Event));
                return true;
            }
            return false;
        }
    private:
        Event& m_Event;
    };
//OPERATOR OVERLOADING FOR THE EVENTS
    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }
}