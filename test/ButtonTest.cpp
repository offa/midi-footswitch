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
#include <catch2/catch_test_macros.hpp>
#include <catch2/trompeloeil.hpp>


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
