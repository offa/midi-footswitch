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
}

TEST_CASE("Led setup sets pin mode and initial on", "[MidiButtonTest]")
{
    constexpr std::uint8_t pin{3};
    REQUIRE_CALL(mock::arduino, pinMode(pin, OUTPUT));
    REQUIRE_CALL(mock::arduino, digitalWrite(pin, HIGH));
    Led<pin> led;
    led.setup();
}
