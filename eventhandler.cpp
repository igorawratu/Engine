#include "eventhandler.h"
#include <list>
#include <iostream>

EventHandler::EventHandler(){
    mouse_position_ = std::make_pair(0, 0);
    mouse_relative_motion_ = std::make_pair(0, 0);

    mouse_moved_ = false;

    mouse_wheel_movement_ = std::make_pair(0, 0);

    quit_ = false;
}

EventHandler::~EventHandler(){
}

void EventHandler::frame(){
    resetKeys();
    resetMouseInfo();
    resetWindowEvents();

    SDL_Event event;
    while(SDL_PollEvent(&event) != 0){
        switch(event.type){
            case SDL_QUIT:
                quit_ = true;
                break;
            case SDL_KEYUP:
            case SDL_KEYDOWN:
                checkKeyEvent(event);
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                checkMouseBtnEvent(event);
                break;
            case SDL_MOUSEMOTION:
                checkMouseMoveEvent(event);
                break;
            case SDL_MOUSEWHEEL:
                checkMouseWheelEvent(event);
                break;
            default:
                break;
        }
    }
}

void EventHandler::resetWindowEvents(){
    window_events_.clear();
}

void EventHandler::checkWindowEvent(const SDL_Event& event){
    assert(event.type == SDL_WINDOWEVENT);

    Uint32 windowID = event.window.windowID;
    Uint8 event_type = event.window.event;

    switch(event_type){
        case SDL_WINDOWEVENT_SHOWN:
        case SDL_WINDOWEVENT_HIDDEN:
        case SDL_WINDOWEVENT_EXPOSED:
        case SDL_WINDOWEVENT_MINIMIZED:
        case SDL_WINDOWEVENT_MAXIMIZED:
        case SDL_WINDOWEVENT_RESTORED:
        case SDL_WINDOWEVENT_ENTER:
        case SDL_WINDOWEVENT_LEAVE:
        case SDL_WINDOWEVENT_FOCUS_GAINED:
        case SDL_WINDOWEVENT_FOCUS_LOST:
        case SDL_WINDOWEVENT_CLOSE:
            window_events_[windowID][event_type] = std::make_pair(0, 0);
            break;
        case SDL_WINDOWEVENT_MOVED:
            window_events_[windowID][SDL_WINDOWEVENT_MOVED] = std::make_pair(event.window.data1, event.window.data1);
        case SDL_WINDOWEVENT_RESIZED:
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            window_events_[windowID][SDL_WINDOWEVENT_RESIZED] = std::make_pair(event.window.data1, event.window.data1);
            break;
        default:
            break;
    }
}

bool EventHandler::windowEventExists(const Uint32& windowID, const Uint8& event_type){
    if(window_events_.find(windowID) == window_events_.end()){
        return false;
    }

    if(window_events_[windowID].find(event_type) == window_events_[windowID].end()){
        return false;
    }

    return true;
}

bool EventHandler::windowShown(const Uint32& windowID){
    return windowEventExists(windowID, SDL_WINDOWEVENT_SHOWN);
}

bool EventHandler::windowHidden(const Uint32& windowID){
    return windowEventExists(windowID, SDL_WINDOWEVENT_HIDDEN);
}

bool EventHandler::windowExposed(const Uint32& windowID){
    return windowEventExists(windowID, SDL_WINDOWEVENT_EXPOSED);
}

bool EventHandler::windowMinimized(const Uint32& windowID){
    return windowEventExists(windowID, SDL_WINDOWEVENT_MINIMIZED);
}

bool EventHandler::windowMaximized(const Uint32& windowID){
    return windowEventExists(windowID, SDL_WINDOWEVENT_MAXIMIZED);
}

bool EventHandler::windowRestored(const Uint32& windowID){
    return windowEventExists(windowID, SDL_WINDOWEVENT_RESTORED);
}

bool EventHandler::mouseEnteredWindow(const Uint32& windowID){
    return windowEventExists(windowID, SDL_WINDOWEVENT_ENTER);
}

bool EventHandler::mouseLeftWindow(const Uint32& windowID){
    return windowEventExists(windowID, SDL_WINDOWEVENT_LEAVE);
}

bool EventHandler::windowFocusGained(const Uint32& windowID){
    return windowEventExists(windowID, SDL_WINDOWEVENT_FOCUS_GAINED);
}

bool EventHandler::windowFocusLost(const Uint32& windowID){
    return windowEventExists(windowID, SDL_WINDOWEVENT_FOCUS_LOST);
}

bool EventHandler::windowClosed(const Uint32& windowID){
    return windowEventExists(windowID, SDL_WINDOWEVENT_CLOSE);
}

std::tuple<bool, int, int> EventHandler::windowMoved(const Uint32& windowID){
    auto out = std::make_tuple(false, 0, 0);
    if(!windowEventExists(windowID, SDL_WINDOWEVENT_MOVED)){
        return out;
    }

    auto coords = window_events_[windowID][SDL_WINDOWEVENT_MOVED];

    out = std::make_tuple(true, coords.first, coords.second);

    return out;
}

std::tuple<bool, int, int> EventHandler::windowResized(const Uint32& windowID){
    auto out = std::make_tuple(false, 0, 0);
    if(!windowEventExists(windowID, SDL_WINDOWEVENT_RESIZED)){
        return out;
    }

    auto coords = window_events_[windowID][SDL_WINDOWEVENT_RESIZED];

    out = std::make_tuple(true, coords.first, coords.second);

    return out;
}

void EventHandler::checkKeyEvent(const SDL_Event& event){
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

void EventHandler::resetKeys(){
    std::list<SDL_Keycode> keys_to_reset;
    std::list<SDL_Keycode> keys_down;

    for(auto key : key_states_){
        if(key.second == InputKeyState::KEYUP){
            keys_to_reset.push_back(key.first);
        }
        else if(key.second == InputKeyState::KEYDOWN){
            keys_down.push_back(key.first);
        }
    }

    for(auto key_code : keys_to_reset){
        key_states_[key_code] = InputKeyState::KEYNONE;
    }

    for(auto key_code : keys_down){
        key_states_[key_code] = InputKeyState::KEYPRESSED;
    }
}

bool EventHandler::isKeyStatus(const SDL_Keycode& key_code, const InputKeyState& key_state){
    if(key_states_.find(key_code) != key_states_.end()){
        return key_states_[key_code] == key_state;
    }

    return false;
}

bool EventHandler::isKeyDown(const SDL_Keycode& key_code){
    return isKeyStatus(key_code, InputKeyState::KEYDOWN);
}

bool EventHandler::isKeyUp(const SDL_Keycode& key_code){
    return isKeyStatus(key_code, InputKeyState::KEYUP);
}

bool EventHandler::isKey(const SDL_Keycode& key_code){
    return isKeyStatus(key_code, InputKeyState::KEYPRESSED) || isKeyStatus(key_code, InputKeyState::KEYDOWN);
}

void EventHandler::resetMouseInfo(){
    mouse_moved_ = false;

    std::list<Uint8> mousebtns_to_reset;
    std::list<Uint8> mousebtns_to_down;

    for(auto btn : mouse_button_states_){
        if(std::get<2>(btn.second) == MouseClickedState::MOUSEBTNUP){
            mousebtns_to_reset.push_back(btn.first);
        }
        else if(std::get<2>(btn.second) == MouseClickedState::MOUSEBTNDOWN){
            mousebtns_to_down.push_back(btn.first);
        }
    }

    for(auto btn_info : mousebtns_to_reset){
        MouseClickedInfo new_mousebtn_info = std::make_tuple(0, 0, MouseClickedState::MOUSEBTNNONE, 0);
        mouse_button_states_[btn_info] = new_mousebtn_info;
    }

    for(auto btn_info : mousebtns_to_down){
        MouseClickedInfo btn = mouse_button_states_[btn_info];
        MouseClickedInfo new_mousebtn_info = std::make_tuple(std::get<0>(btn), std::get<1>(btn), MouseClickedState::MOUSEBTNPRESSED, std::get<3>(btn));
        mouse_button_states_[btn_info] = new_mousebtn_info;
    }

    //mouse wheel resets
    mouse_wheel_movement_ = std::make_pair(0, 0);

    int mouse_x, mouse_y, mouse_xrel, mouse_yrel;
    Uint8 btntmp;

    btntmp = SDL_GetMouseState(&mouse_x, &mouse_y);
    btntmp = SDL_GetRelativeMouseState(&mouse_xrel, &mouse_yrel);

    mouse_position_ = std::make_pair(mouse_x, mouse_y);
    mouse_relative_motion_ = std::make_pair(mouse_xrel, mouse_yrel);
}

void EventHandler::checkMouseBtnEvent(const SDL_Event& event){
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
    else if(event.type == SDL_MOUSEBUTTONUP){
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

void EventHandler::checkMouseMoveEvent(const SDL_Event& event){
    assert(event.type == SDL_MOUSEMOTION);

    //ignore touch events, as they will get dealt separately.
    if(event.motion.which == SDL_TOUCH_MOUSEID){
        return;
    }

    //mouse positions are updated in reset function, as they need to happen every frame rather than
    //when event gets called
    mouse_moved_ = true;
}

void EventHandler::checkMouseWheelEvent(const SDL_Event& event){
    assert(event.type == SDL_MOUSEWHEEL);

    //ignore touch events, as they will get dealt separately.
    if(event.wheel.which == SDL_TOUCH_MOUSEID){
        return;
    }

    mouse_wheel_movement_ = std::make_pair(event.wheel.x, event.wheel.y);
}

std::tuple<bool, int, int, int> EventHandler::isMouseBtnStatus(const Uint8& btn, const MouseClickedState& state){
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

std::tuple<bool, int, int, int> EventHandler::isMouseBtnDown(const Uint8& btn){
    return isMouseBtnStatus(btn, MouseClickedState::MOUSEBTNDOWN);
}

std::tuple<bool, int, int> EventHandler::isMouseBtnUp(const Uint8& btn){
    auto btn_up_status = isMouseBtnStatus(btn, MouseClickedState::MOUSEBTNUP);

    bool is_active = std::get<0>(btn_up_status);
    int x = std::get<1>(btn_up_status);
    int y = std::get<2>(btn_up_status);

    return std::make_tuple(is_active, x, y);
}

std::tuple<bool, int, int, int> EventHandler::isMouseBtn(const Uint8& btn){
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

std::pair<int, int> EventHandler::mouseWheelMovement(){
    return mouse_wheel_movement_;
}

std::pair<int, int> EventHandler::mousePosition(){
    return mouse_position_;
}

std::pair<int, int> EventHandler::mouseRelativeMotion(){
    return mouse_relative_motion_;
}

bool EventHandler::mouseMoved(){
    return mouse_moved_;
}

bool EventHandler::mouseWheelMoved(){
    return mouse_wheel_movement_.first == 0 && mouse_wheel_movement_.second == 0;
}
