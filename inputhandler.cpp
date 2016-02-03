#include "inputhandler.h"
#include <list>
#include <iostream>

InputHandler::InputHandler(){
    mouse_move_position_ = std::make_pair(0, 0);
    mouse_relative_motion_ = std::make_pair(0, 0);

    mouse_wheel_movement_ = std::make_pair(0, 0);
}

InputHandler::~InputHandler(){
}

void InputHandler::frame(){
    resetKeys();

    while(true){
        SDL_Event event;
        while(SDL_PollEvent(&event) != 0){
            //remove this first condition when not testing anymore
            if(event.type == SDL_QUIT){
                exit(0);
            }
            else if(event.type == SDL_KEYUP || event.type == SDL_KEYDOWN){
                checkKeyEvent(event);
            }
        }
    }
}

void InputHandler::checkKeyEvent(const SDL_Event& event){
    if(event.type == SDL_KEYDOWN){
        SDL_Keycode key = event.key.keysym.sym;
        key_states_[key] = event.key.repeat > 0 ? InputKeyState::KEYPRESSED : InputKeyState::KEYDOWN;
    }
    else if(event.type == SDL_KEYUP){
        SDL_Keycode key = event.key.keysym.sym;
        key_states_[key] = InputKeyState::KEYUP;
    }
    else{
        //should never reach here
        assert(false);
    }
}

void InputHandler::resetKeys(){
    std::list<SDL_Keycode> keys_to_reset;

    for(auto key : key_states_){
        if(key.second == InputKeyState::KEYUP){
            keys_to_reset.push_back(key.first);
        }
    }

    for(auto key_code : keys_to_reset){
        key_states_[key_code] = InputKeyState::KEYNONE;
    }
}

bool InputHandler::isKeyStatus(const SDL_Keycode& key_code, const InputKeyState& key_state){
    if(key_states_.find(key_code) != key_states_.end()){
        return key_states_[key_code] == key_state;
    }

    return false;
}

bool InputHandler::isKeyDown(const SDL_Keycode& key_code){
    return isKeyStatus(key_code, InputKeyState::KEYDOWN);
}

bool InputHandler::isKeyUp(const SDL_Keycode& key_code){
    return isKeyStatus(key_code, InputKeyState::KEYUP);
}

bool InputHandler::isKey(const SDL_Keycode& key_code){
    return isKeyStatus(key_code, InputKeyState::KEYPRESSED) || isKeyStatus(key_code, InputKeyState::KEYDOWN);
}

void InputHandler::resetMouseInfo(){
    std::list<Uint8> mousebtns_to_reset;

    for(auto btn : mouse_button_states_){
        if(std::get<2>(btn.second) == MouseClickedState::MOUSEBTNUP){
            mousebtns_to_reset.push_back(btn.first);
        }
    }

    for(auto btn_info : mousebtns_to_reset){
        MouseClickedInfo new_mousebtn_info = std::make_tuple(0, 0, MouseClickedState::MOUSEBTNNONE, 0);
        mouse_button_states_[btn_info] = new_mousebtn_info;
    }

    //mouse motion resets
    mouse_relative_motion_ = std::make_pair(0, 0);

    //mouse wheel resets
    mouse_wheel_movement_ = std::make_pair(0, 0);
}

void InputHandler::checkMouseBtnEvent(const SDL_Event& event){
    if(event.type == SDL_MOUSEBUTTONDOWN){
        Uint8 btn = event.button.button;

        bool previous_down = false;
        if(mouse_button_states_.find(btn) != mouse_button_states_.end()){
            MouseClickedInfo info = mouse_button_states_[btn];
            MouseClickedState prev_state = std::get<2>(info);

            previous_down = prev_state == MouseClickedState::MOUSEBTNDOWN;
        }

        int clicks = (int) event.button.clicks;
        int x = event.button.x;
        int y = event.button.y;
        MouseClickedState curr_state = previous_down ? MouseClickedState::MOUSEBTNPRESSED : MouseClickedState::MOUSEBTNDOWN;

        mouse_button_states_[btn] = std::make_tuple(x, y, curr_state, clicks);
    }
    else if(event.type == SDL_KEYUP){
        Uint8 btn = event.button.button;

        int x = event.button.x;
        int y = event.button.y;
        MouseClickedState curr_state = MouseClickedState::MOUSEBTNUP;

        mouse_button_states_[btn] = std::make_tuple(x, y, curr_state, 0);

        key_states_[btn] = InputKeyState::KEYUP;
    }
    else{
        //should never reach here
        assert(false);
    }
}

void InputHandler::checkMouseMoveEvent(const SDL_Event& event){
    assert(event.type == SDL_MOUSEMOTION);

    //ignore touch events, as they will get dealt separately.
    if(event.motion.which == SDL_TOUCH_MOUSEID){
        return;
    }

    mouse_move_position_ = std::make_pair(event.motion.x, event.motion.y);
    mouse_relative_motion_ = std::make_pair(event.motion.xrel, event.motion.yrel);
}

void InputHandler::checkMouseWheelEvent(const SDL_Event& event){
    assert(event.type == SDL_MOUSEWHEEL);

    //ignore touch events, as they will get dealt separately.
    if(event.wheel.which == SDL_TOUCH_MOUSEID){
        return;
    }

    mouse_wheel_movement_ = std::make_pair(event.wheel.x, event.wheel.y);
}

std::tuple<bool, int, int, int> InputHandler::isMouseBtnStatus(const Uint8& btn, const MouseClickedState& state){
    std::tuple<bool, int, int, int> out = std::make_tuple(false, 0, 0, 0);

    if(mouse_button_states_.find(btn) != mouse_button_states_.end()){
        MouseClickedInfo inf = mouse_button_states_[btn];
        if(std::get<2>(inf) == state){
            int x = std::get<0>(inf);
            int y = std::get<1>(inf);
            int clicks = std::get<3>(inf);

            out = std::make_tuple(true, x, y, clicks);
        }
    }

    return out;
}

std::tuple<bool, int, int, int> InputHandler::isMouseBtnDown(const Uint8& btn){
    return isMouseBtnStatus(btn, MouseClickedState::MOUSEBTNDOWN);
}

std::tuple<bool, int, int> InputHandler::isMouseBtnUp(const Uint8& btn){
    auto btn_up_status = isMouseBtnStatus(btn, MouseClickedState::MOUSEBTNDOWN);

    bool is_active = std::get<0>(btn_up_status);
    int x = std::get<1>(btn_up_status);
    int y = std::get<2>(btn_up_status);

    return std::make_tuple(is_active, x, y);
}

std::tuple<bool, int, int, int> InputHandler::isMouseBtn(const Uint8& btn){
    auto mouse_down_inf = isMouseBtnStatus(btn, MouseClickedState::MOUSEBTNDOWN);
    auto mouse_pressed_inf = isMouseBtnStatus(btn, MouseClickedState::MOUSEBTNPRESSED);

    if(std::get<0>(mouse_down_inf)){
        return mouse_down_inf;
    }
    else if(std::get<0>(mouse_pressed_inf)){
        return mouse_pressed_inf;
    }

    //doesnt matter which is returned, both are false and empty;
    return mouse_down_inf;
}

std::pair<int, int> InputHandler::mouseWheelMovement(){
    return mouse_wheel_movement_;
}

std::pair<int, int> InputHandler::mousePosition(){
    return mouse_move_position_;
}

std::pair<int, int> InputHandler::mouseRelativeMotion(){
    return mouse_relative_motion_;
}

bool InputHandler::mouseMoved(){
    return mouse_relative_motion_.first == 0 && mouse_relative_motion_.second == 0;
}

bool InputHandler::mouseWheelMoved(){
    return mouse_wheel_movement_.first == 0 && mouse_wheel_movement_.second == 0;
}
