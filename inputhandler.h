#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <SDL.h>
#include <memory>
#include <unordered_map>
#include <tuple>

class InputHandler{
private:
    enum InputKeyState{KEYDOWN, KEYUP, KEYPRESSED, KEYNONE};
    std::unordered_map<SDL_Keycode, InputKeyState> key_states_;

    //x, y
    std::pair<int, int> mouse_wheel_movement_;

    enum MouseClickedState{MOUSEBTNDOWN, MOUSEBTNUP, MOUSEBTNPRESSED, MOUSEBTNNONE};
    //x, y, state, clicks
    typedef std::tuple<int, int, MouseClickedState, int> MouseClickedInfo;
    std::unordered_map<Uint8, MouseClickedInfo> mouse_button_states_;

    //x, y
    std::pair<int, int> mouse_move_position_;
    std::pair<int, int> mouse_relative_motion_;

private:
    //resets all keys that are in KEYUP state to none. As this is called at the beginning of frame, it essentially ensures
    //that keyups last 1 frame, before being reset to the default state.
    void resetKeys();

    //checks for the key pressed, and updates the key_states_ accordingly
    void checkKeyEvent(const SDL_Event& event);

    //helper for keychecks
    bool isKeyStatus(const SDL_Keycode& key_code, const InputKeyState& key_state);

    //resets various mouse info such as relative movement, wheel movement, and button ups to ensure that the latest information
    //is correct, and that keyups last 1 frame.
    void resetMouseInfo();

    //checks and updates mouse button state
    void checkMouseBtnEvent(const SDL_Event& event);

    //checks and updates mouse movement state
    void checkMouseMoveEvent(const SDL_Event& event);

    //checks and updates mouse wheel state
    void checkMouseWheelEvent(const SDL_Event& event);

    //helper for mouse button states
    std::tuple<bool, int, int, int> isMouseBtnStatus(const Uint8& btn,const MouseClickedState& state);

public:
    InputHandler();
    //not allowed to copy
    InputHandler(const InputHandler&) = delete;
    ~InputHandler();

    //change this to private later on, only to be accessed by friends
    void frame();

    /**
     * @brief Checks if \p key_code has just (within the last frame) been pressed
     * @param key_code The key to check for
     * @return true if the key has just been pressed, otherwise false
     */
    bool isKeyDown(const SDL_Keycode& key_code);

    /**
     * @brief Checks if \p key_code has just (within the last frame) been released
     * @param key_code The key to check for
     * @return true if the key has been released, otherwise false
     */
    bool isKeyUp(const SDL_Keycode& key_code);

    /**
     * @brief Checks if \p key_code is currently being pressed
     * @param key_code The key to check for
     * @return true if the key is currently being pressed, otherwise false
     */
    bool isKey(const SDL_Keycode& key_code);

    std::tuple<bool, int, int, int> isMouseBtnDown(const Uint8& btn);

    std::tuple<bool, int, int> isMouseBtnUp(const Uint8& btn);

    std::tuple<bool, int, int, int> isMouseBtn(const Uint8& btn);

    std::pair<int, int> mouseWheelMovement();

    std::pair<int, int> mousePosition();

    std::pair<int, int> mouseRelativeMotion();

    bool mouseMoved();

    bool mouseWheelMoved();
};

#endif // INPUTHANDLER_H

