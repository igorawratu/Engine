#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <SDL.h>
#include <memory>
#include <unordered_map>
#include <tuple>

class EventHandler{
private:
    bool quit_;

    typedef std::unordered_map<Uint32, std::unordered_map<Uint8, std::pair<Sint32, Sint32> > > WindowEventDatabase;
    WindowEventDatabase window_events_;

    enum InputKeyState{KEYDOWN, KEYUP, KEYPRESSED, KEYNONE};
    std::unordered_map<SDL_Keycode, InputKeyState> key_states_;

    //x, y
    std::pair<int, int> mouse_wheel_movement_;

    enum MouseClickedState{MOUSEBTNDOWN, MOUSEBTNUP, MOUSEBTNPRESSED, MOUSEBTNNONE};
    //x, y, state, clicks
    typedef std::tuple<int, int, MouseClickedState, int> MouseClickedInfo;
    std::unordered_map<Uint8, MouseClickedInfo> mouse_button_states_;

    //x, y
    bool mouse_moved_;
    std::pair<int, int> mouse_position_;
    std::pair<int, int> mouse_relative_motion_;

private:
    //forwards event handler by 1 frame
    void frame();

    //helper for window event checking
    bool windowEventExists(const Uint32& windowID, const Uint8& event_type);

    //clears all window events
    void resetWindowEvents();

    //Updates the window events database with the valid window events from the current frame
    void checkWindowEvent(const SDL_Event& event);

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
    EventHandler();
    //not allowed to copy
    EventHandler(const EventHandler&) = delete;
    ~EventHandler();

    /**
     * @brief Checks if the quit event has been received
     * @return true if event has been received, otherwise false
     */
    bool isQuit(){
        return quit_;
    }

    /**
     * @brief Checks if the window has been shown (from an unshown state) over the course of the past frame
     * @param windowID ID of the window to check
     * @return true if window has been shown, otherwise false
     */
    bool windowShown(const Uint32& windowID);

    /**
     * @brief Checks if the window has been hidden in the past frame
     * @param windowID ID of the window to check
     * @return true if window has been hidden, otherwise false
     */
    bool windowHidden(const Uint32& windowID);

    /**
     * @brief Checks if the window has been exposed in the past frame
     * @param windowID ID of the window to check
     * @return true if window has been exposed, otherwise false
     */
    bool windowExposed(const Uint32& windowID);

    /**
     * @brief Checks if window has been minimized in the past frame
     * @param windowID ID of the window to check
     * @return true if window has been minimized, otherwise false
     */
    bool windowMinimized(const Uint32& windowID);

    /**
     * @brief Checks if window has been maximized in the past frame
     * @param windowID ID of the window to check
     * @return true if window has been maximized, otherwise false
     */
    bool windowMaximized(const Uint32& windowID);

    /**
     * @brief Checks if window has been restored from a minimized state in the past frame
     * @param windowID ID of the window to check
     * @return true if window has been restored, otherwise false
     */
    bool windowRestored(const Uint32& windowID);

    /**
     * @brief Checks if the mouse has entered the window frame in the past frame
     * @param windowID ID of the window to check
     * @return true if mouse has entered the window, otherwise false
     */
    bool mouseEnteredWindow(const Uint32& windowID);

    /**
     * @brief Checks if the mouse has left the window frame in the past frame
     * @param windowID ID of the window to check
     * @return true if mouse has left the window, otherwise false
     */
    bool mouseLeftWindow(const Uint32& windowID);

    /**
     * @brief Checks if the window has gained focus in the past frame
     * @param windowID ID of the window to check
     * @return true if the window has gained focus, otherwise false
     */
    bool windowFocusGained(const Uint32& windowID);

    /**
     * @brief Checks if the window has lost focus in the past frame
     * @param windowID ID of the window to check
     * @return true if the window has lost focus, otherwise false
     */
    bool windowFocusLost(const Uint32& windowID);

    /**
     * @brief Checks if the window has closed in the past frame (this event occurs in multi-window systems when window is x'd, as opposed to the quit event)
     * @param windowID ID of the window to check
     * @return true if window has been closed, otherwise false
     */
    bool windowClosed(const Uint32& windowID);

    /**
     * @brief Checks if the window has moved in the past frame
     * @param windowID ID of the window to check
     * @return a tuple consisting of a bool to specify if the window has moved, and 2 integers specifying the new window x and y positional coordinates
     */
    std::tuple<bool, int, int> windowMoved(const Uint32& windowID);

    /**
     * @brief Checks if the window has been resized in the past frame
     * @param windowID ID of the window to check
     * @return a tuple consisting of a bool to specify if the window has been resized, and 2 integers specifying the new window width and height (in that order)
     */
    std::tuple<bool, int, int> windowResized(const Uint32& windowID);

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

    /**
     * @brief checks if the passed button \p btn has just been clicked
     * @param btn The button to check for
     * @return a tuple with the first element indicating if the condition is true, the x position, the y position, and the number of clicks
     */
    std::tuple<bool, int, int, int> isMouseBtnDown(const Uint8& btn);

    /**
     * @brief checks if the passed button \p btn has just been released
     * @param btn The button to check for
     * @return a tuple with the first element indicating if the condition is true, the x position, and the y position
     */
    std::tuple<bool, int, int> isMouseBtnUp(const Uint8& btn);

    /**
     * @brief checks if the passed button \p btn is currently being clicked
     * @param btn The button to check for
     * @return a tuple with the first element indicating if the condition is true, the x and y coordinates of the last
     * clicked position (if you want the actual mouse position, then use mousePosition()), and the number of clicks
     */
    std::tuple<bool, int, int, int> isMouseBtn(const Uint8& btn);

    /**
     * @brief Provides the relative mouse movement over the past frame
     * @return a pair containing the x and y relative movements
     */\
    std::pair<int, int> mouseWheelMovement();

    /**
     * @brief Provides the position of the mouse. If the mouse has not yet been moved, this function returns 0,0 as the coordinates, as
     * it can only register mouse positions on mouse move.
     * @return a pair containing the x and y mouse positions
     */
    std::pair<int, int> mousePosition();

    /**
     * @brief Provides the relative movement of the mouse in the past frame
     * @return a pair contianing the x and y relative positions of the mouse
     */
    std::pair<int, int> mouseRelativeMotion();

    /**
     * @brief Checks if the mouse has moved in the last frame
     * @return True if mouse has moved, otherwise false
     */
    bool mouseMoved();

    /**
     * @brief Checks if the mouse wheel has been scrolled in the last frame
     * @return True if mouse wheel has moved, otherwise false
     */
    bool mouseWheelMoved();
};

#endif // INPUTHANDLER_H

