#include "interfaceElement.hpp"

CInterfaceElement::CInterfaceElement(WINDOW * w) : m_Window(w) {
    getmaxyx(m_Window, m_Height, m_Width);
}

CInterfaceElement::~CInterfaceElement() {
}
