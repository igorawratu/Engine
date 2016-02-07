#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <SDL_opengl.h>

#include <string>

const int CENTER_WINDOW_POS = -1;
const int UNDEFINED_WINDOW_POS = -2;

class Window
{
private:
    SDL_Window* window_;
    SDL_GLContext context_;

public:
    Window(std::string title = "Engine", int width = 0, int height = 0, int x_pos = UNDEFINED_WINDOW_POS, int y_pos = UNDEFINED_WINDOW_POS, bool maximized = true,
           bool fullscreen = true, bool resizable = true, bool focus = true);
    //do not allow copying windows
    Window(const Window& other) = delete;
    ~Window();

    //The various functions in the windows class merely checks for

    /**
     * @brief Makes the window's opengl context the current one, resulting in all rendering and such being performed onto this window.
     */
    void makeCurrent();

    /**
     * @brief Acquires SDL window ID for the window
     * @return the window ID
     */
    Uint32 getID();

    /**
     * @brief Calculates the resolution of the window
     * @return a pair containing the width and height of the window
     */
    std::pair<int, int> getResolution();

    /**
     * @brief Calculates the position of the window in screen space (top left is 0,0 typically)
     * @return a pair containing the x and y coordinates of the top-left position of the window
     */
    std::pair<int, int> getPosition();

    /**
     * @brief Checks if window is visible
     * @return true if window if visible, otherwise false
     */
    bool isVisible();

    /**
     * @brief Checks if window is in a minimized state
     * @return true if the window is currently minimized, otherwise false
     */
    bool isMinimized();

    /**
     * @brief Checks if the window has grabbed the input (this means that inputs such as mouse is locked within the window)
     * @return true if input is being grabbed, otherwise false;
     */
    bool inputGrabbed();

    /**
     * @brief Checks if input is focused on current window (ie. the window is receiving input)
     * @return true if input is focused, otherwise false;
     */
    bool isInputFocused();

    /**
     * @brief Checks if mouse is over window
     * @return true if mouse is over window, otherwise false;
     */
    bool isMouseOver();

    /**
     * @brief Checks if window can be resized
     * @return true if window is resizable, otherwise false
     */
    bool isResizable();

    /**
     * @brief Checks if window is in borderless mode
     * @return true if window is in borderless mode, otherwise false
     */
    bool isBorderless();

    /**
     * @brief Checks if window is in fullscreen mode
     * @return true if window is in fullscreen mode, otherwise false;
     */
    bool isFullScreen();

    /**
     * @brief Resizes the window to the specified \p width and \p height. This works for both windowed and fullscreen modes
     * @param width The new width of the window
     * @param height The new height of the window
     */
    void resizeWindow(int width, int height);

    //put private later
    void frame();
};

#endif // WINDOW_H
