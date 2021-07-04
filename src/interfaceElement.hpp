#ifndef INTERFACEELEMENT_HPP
#define INTERFACEELEMENT_HPP

#include <ncurses.h>

/**
 * Base class for any interface element.
 */
class CInterfaceElement{
public:
    /**
     * Constructor.
     * @param w Window which will contain the element.
     */
    CInterfaceElement(WINDOW * w);
    virtual ~CInterfaceElement();
    
    /**
     * Renders the element.
     */
    virtual void Show() = 0;
    /**
     * Registers keycodes for input handling.
     */
    virtual void RegisterKeys() = 0;
   
protected:
    /*! Window which will contain the element. */
    WINDOW * m_Window;
    /*! Width of the given window. */
    size_t m_Width;
    /*! Height of the given window. */
    size_t m_Height;
};


#endif /* INTERFACEELEMENT_HPP */

