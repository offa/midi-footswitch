/*
 * MIDI Footswitch
 *
 * Copyright (C) 2019-2020  offa
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "MidiButton.h"
#include <catch2/catch.hpp>
#include <trompeloeil.hpp>

namespace mock
{
    ArduinoMock arduino;

    namespace
    {
        template <uint8_t pin>
        struct ButtonSpy : public Button<pin>
        {
            EasyButton& spy()
            {
                return this->button;
            }
        };

        class ButtonMock
        {
        public:
            MAKE_MOCK0(setup, void());
            MAKE_MOCK0(pressed, bool());
        };

        class LedMock
        {
        public:
            MAKE_MOCK0(setup, void());
            MAKE_MOCK0(toggle, void());
        };

        class ActionImpl
        {
        public:
            MAKE_MOCK0(onPressed, void());
        };

        class ActionMock
        {
        public:
            static void onPressed()
            {
                impl.onPressed();
            }

            inline static ActionImpl impl;
        };
    }
}

TEST_CASE("Setup sets pin mode and initial on", "[LedTest]")
{
    constexpr std::uint8_t pin{3};
    REQUIRE_CALL(mock::arduino, pinMode(pin, OUTPUT));
    REQUIRE_CALL(mock::arduino, digitalWrite(pin, HIGH));

    Led<pin> led;
    led.setup();
}

TEST_CASE("Toggle toggles led", "[LedTest]")
{
    constexpr std::uint8_t pin{8};
    REQUIRE_CALL(mock::arduino, digitalRead(pin)).RETURN(LOW);
    REQUIRE_CALL(mock::arduino, digitalWrite(pin, HIGH));

    Led<pin> led;
    led.toggle();
}

TEST_CASE("Setup initializes button", "[ButtonTest]")
{
    mock::ButtonSpy<3> buttonSpy;
    REQUIRE_CALL(buttonSpy.spy(), begin());
    buttonSpy.setup();
}

TEST_CASE("Pressed returns pressed button state", "[ButtonTest]")
{
    mock::ButtonSpy<3> buttonSpy;
    REQUIRE_CALL(buttonSpy.spy(), read()).RETURN(true);
    REQUIRE_CALL(buttonSpy.spy(), wasReleased()).RETURN(true);
    REQUIRE(buttonSpy.pressed() == true);
}

TEST_CASE("Pressed returns not pressed button state", "[ButtonTest]")
{
    mock::ButtonSpy<3> buttonSpy;
    REQUIRE_CALL(buttonSpy.spy(), read()).RETURN(true);
    REQUIRE_CALL(buttonSpy.spy(), wasReleased()).RETURN(false);
    REQUIRE(buttonSpy.pressed() == false);
}

TEST_CASE("Setup initializes button and led", "[MidiButtonTest]")
{
    MidiButton<mock::ButtonMock, mock::LedMock, mock::ActionMock> button;
    REQUIRE_CALL(button.getButton(), setup());
    REQUIRE_CALL(button.getLed(), setup());
    button.setup();
}

TEST_CASE("Read does nothing if not pressed", "[MidiButtonTest]")
{
    MidiButton<mock::ButtonMock, mock::LedMock, mock::ActionMock> button;
    REQUIRE_CALL(button.getButton(), pressed()).RETURN(false);
    button.read();
}

TEST_CASE("Read executes action if pressed", "[MidiButtonTest]")
{
    MidiButton<mock::ButtonMock, mock::LedMock, mock::ActionMock> button;
    REQUIRE_CALL(button.getButton(), pressed()).RETURN(true);
    REQUIRE_CALL(button.getLed(), toggle());
    REQUIRE_CALL(mock::ActionMock::impl, onPressed());
    button.read();
}
