#ifndef INFOPANEL_HPP
#define INFOPANEL_HPP

#include <vector>
#include "interfaceElement.hpp"

/**
 * Base class for panels displaying various info messages.
 */
class CInfoPanel : public CInterfaceElement {
public:
    /**
     * Constructor.
     * @param w Window which will contain the panel.
     */
    CInfoPanel(WINDOW * w = stdscr);
    virtual ~CInfoPanel();
    virtual void RegisterKeys() override;

protected:
};

/**
 * Panel for displaying a simple message, scrollable.
 */
class CInfoMessagePanel : public CInfoPanel {
public:
    /**
     * New Message panel.
     * @param w Window which will contain the panel.
     * @param message Message to be displayed.
     * @param center Should the message be centered.
     */
    CInfoMessagePanel(WINDOW * w = stdscr, std::string message = "", bool center = true);
    /**
     * New Message panel.
     * @param w Window which will contain the panel.
     * @param message Message to be displayed. All messages are displayed at once if screen size allows it.
     * @param center Should the message be centered.
     */
    CInfoMessagePanel(WINDOW * w = stdscr, std::vector<std::string> message = {}, bool center = true);
    virtual ~CInfoMessagePanel();
    void Show() override;
protected:
    void RegisterKeys() override;
    /*! Called when user presses Enter. */
    void onEnter();

    /*! Current row displayed at the top. */
    short unsigned int m_Row = 0;
    /*! Whether the message should be centered. */
    bool m_Centered;
    /*! The message to be displayed. */
    std::vector<std::string> m_Message;

};

/**
 * Warning panel to resize terminal to 80x24
 */
class CInfoResizeTerminalPanel : public CInfoPanel {
public:
    /**
     * Constructor.
     * @param w Window which will contain the panel
     */
    CInfoResizeTerminalPanel(WINDOW * w = stdscr);
    virtual ~CInfoResizeTerminalPanel();
    void Show() override;
protected:
};

#endif /* INFOPANEL_HPP */

