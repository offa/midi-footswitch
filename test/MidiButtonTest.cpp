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

namespace
{
    auto midiEventEq(const midiEventPacket_t& event)
    {
        return trompeloeil::make_matcher<midiEventPacket_t>(
            [](const midiEventPacket_t& expected, const midiEventPacket_t& actual) {
                return std::tie(expected.header, expected.byte1, expected.byte2, expected.byte3) ==
                       std::tie(actual.header, actual.byte1, actual.byte2, actual.byte3);
            },
            [](std::ostream& os, const midiEventPacket_t& event) {
                os << std::hex << std::showbase << std::internal << std::setfill('0')
                   << " midi event {" << std::setw(4) << std::uint32_t{event.header}
                   << ", " << std::setw(4) << std::uint32_t(event.byte1)
                   << ", " << std::setw(4) << std::uint32_t(event.byte2)
                   << ", " << std::setw(4) << std::uint32_t(event.byte3) << "}";
            },
            event);
    };
}

MIDIMock MidiUSB;

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

TEST_CASE("Send event message", "[MidiUSBTest]")
{
    REQUIRE_CALL(MidiUSB, sendMIDI(midiEventEq({(0x1a & 0xf0) >> 4, (0x1a | 0x2b), 0xbb, 0xcc})));
    REQUIRE_CALL(MidiUSB, flush());
    detail::sendEventMessage<0x1a, 0x2b>(0xbb, 0xcc);
}

TEST_CASE("Program change action sends packet", "[ProgramChangeActionTest]")
{
    constexpr std::uint8_t channel{0x02};
    constexpr std::uint8_t program{0x11};
    REQUIRE_CALL(MidiUSB, sendMIDI(midiEventEq({0x0c, (0xc0 | channel), program, 0x00})));
    REQUIRE_CALL(MidiUSB, flush());

    ProgramChangeAction<channel, program>::onPressed();
}

TEST_CASE("Control change action sends packet", "[ProgramChangeActionTest]")
{
    constexpr std::uint8_t channel{0x05};
    constexpr std::uint8_t control{0x22};
    constexpr std::uint8_t data{0x33};
    REQUIRE_CALL(MidiUSB, sendMIDI(midiEventEq({0x0b, (0xb0 | channel), control, data})));
    REQUIRE_CALL(MidiUSB, flush());

    ControlChangeAction<channel, control, data>::onPressed();
}

TEST_CASE("Control change toggle action sends packet with toggled values", "[ProgramChangeActionTest]")
{
    constexpr std::uint8_t channel{0x03};
    constexpr std::uint8_t control{0x17};
    constexpr std::uint8_t dataA{0x01};
    constexpr std::uint8_t dataB{0x02};

    REQUIRE_CALL(MidiUSB, sendMIDI(midiEventEq({0x0b, (0xb0 | channel), control, dataA})));
    REQUIRE_CALL(MidiUSB, flush());
    REQUIRE_CALL(MidiUSB, sendMIDI(midiEventEq({0x0b, (0xb0 | channel), control, dataB})));
    REQUIRE_CALL(MidiUSB, flush());

    ControlChangeToggleAction<channel, control, dataA, dataB>::onPressed();
    ControlChangeToggleAction<channel, control, dataA, dataB>::onPressed();
}
