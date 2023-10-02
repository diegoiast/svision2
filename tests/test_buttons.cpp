/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <buttonstates.h>
#include <catch2/catch_test_macros.hpp>
#include <vector>

TEST_CASE("Button states", "[button]") {
    EventMouse event_in;
    EventMouse event_out;
    EventMouse event_click;
    EventMouse event_release_inside;
    EventMouse event_release_outside;

    event_in.type = MouseEvents::MouseMove;
    event_in.is_local = true;
    event_in.x = 10;
    event_in.y = 10;

    event_out.type = MouseEvents::MouseMove;
    event_out.is_local = false;
    event_out.x = 1000;
    event_out.y = 1000;

    event_click.type = MouseEvents::Press;
    event_click.pressed = true;
    event_click.button = 1;
    event_click.is_local = true;
    event_click.x = 10;
    event_click.y = 10;

    event_release_inside.type = MouseEvents::Release;
    event_release_inside.pressed = false;
    event_release_inside.button = 1;
    event_release_inside.is_local = true;
    event_release_inside.x = 10;
    event_release_inside.y = 10;

    event_release_outside.type = MouseEvents::Press;
    event_release_outside.pressed = false;
    event_release_outside.button = 1;
    event_release_outside.is_local = false;
    event_release_outside.x = 1000;
    event_release_outside.y = 1000;

    SECTION("Testing hover in and out") {
        bool clicked = false;
        AbstractButtonState buttonState;
        auto on_button_click = [&clicked]() { clicked = true; };
        REQUIRE(buttonState.state == ButtonStates::Normal);

        buttonState.on_mouse_enter();
        REQUIRE(buttonState.state == ButtonStates::Hovered);
        buttonState.on_mouse_leave();
        REQUIRE(buttonState.state == ButtonStates::Normal);

        buttonState.on_mouse_enter();
        buttonState.on_mouse_enter();
        buttonState.on_mouse_enter();
        buttonState.on_mouse_enter();
        REQUIRE(buttonState.state == ButtonStates::Hovered);

        buttonState.on_mouse_leave();
        REQUIRE(buttonState.state == ButtonStates::Normal);
    }

    SECTION("Normal click inside") {
        AbstractButtonState buttonState;
        EventPropagation result;
        REQUIRE(buttonState.state == ButtonStates::Normal);

        buttonState.on_mouse_enter();
        REQUIRE(buttonState.state == ButtonStates::Hovered);

        result = buttonState.on_mouse_click(event_click);
        REQUIRE(buttonState.state == ButtonStates::ClickedInside);
        REQUIRE(result == EventPropagation::handled);

        result = buttonState.on_mouse_click(event_release_inside);
        REQUIRE(buttonState.state == ButtonStates::Hovered);
        REQUIRE(result == EventPropagation::handled);

        buttonState.on_mouse_leave();
        REQUIRE(buttonState.state == ButtonStates::Normal);
    }

    SECTION("Normal click abort") {
        bool clicked = false;
        AbstractButtonState buttonState;
        REQUIRE(buttonState.state == ButtonStates::Normal);

        buttonState.on_mouse_enter();
        REQUIRE(buttonState.state == ButtonStates::Hovered);
        REQUIRE(clicked == false);

        buttonState.on_mouse_click(event_click);
        REQUIRE(buttonState.state == ButtonStates::ClickedInside);
        REQUIRE(clicked == false);

        buttonState.on_mouse_leave();
        REQUIRE(buttonState.state == ButtonStates::ClickedOutside);

        buttonState.on_mouse_click(event_release_outside);
        REQUIRE(buttonState.state == ButtonStates::Normal);
        REQUIRE(clicked == false);
    }
}
