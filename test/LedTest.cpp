/*
 * MIDI Footswitch
 *
 * Copyright (C) 2019-2022  offa
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
#include "mock/Mocks.h"
#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>


namespace mock
{
    ArduinoMock arduino;
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
