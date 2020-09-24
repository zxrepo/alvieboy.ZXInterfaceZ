#ifndef __WSYS_BIN_H__
#define __WSYS_BIN_H__

#include "widgetgroup.h"

class Bin: public WidgetGroup
{
public:
    Bin(Widget *parent=NULL);
    virtual ~Bin();
    virtual void draw(bool force=false) override;
    void removeChild(Widget *c);
//    virtual void resize(uint8_t x, uint8_t y, uint8_t w, uint8_t h) override;
//    virtual void move(uint8_t x, uint8_t y) override;
    virtual void handleEvent(uint8_t type, u16_8_t code) override;
protected:

    virtual void setChild(Widget *);
    Widget *m_child;
};



#endif
