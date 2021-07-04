#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include <functional>
#include <map>

/**
 * Captures and handles user input. Singleton class.
 */
class CControls {
public:
    /**
     * Getter for singleton class.
     * @return CControls instance
     */
    static CControls & getInstance();
    /**
     * Makes a keymap key->func, calling func each time key is pressed.
     * @param key Keycode of the key
     * @param func Function to be called
     */
    void AssignKey(int key, std::function<void()> func);
    /**
     * Removes one key and his function from keymap.
     * @param key Keycode of the key
     */
    void UnassignKey(int key);
    /**
     * Removes all assigned key mappings.
     */
    void ClearAssignedKeys();
    /**
     * Checks if user input is available and handles it.
     */
    void ReadInput() const;
private:
    CControls();
    CControls(const CControls & c) = delete;
    CControls(CControls && c) = delete;
    CControls & operator = (const CControls & c) = delete;
    CControls & operator = (CControls && c) = delete;
    
    /**
     * Map for mapping keycode to function.
     */
    std::map<int, std::function<void()>> m_KeyMap;
};

#endif /* CONTROLS_HPP */

