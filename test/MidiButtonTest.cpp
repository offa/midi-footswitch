/*
 * MIDI Footswitch
 *
 * Copyright (C) 2019-2021  offa
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
