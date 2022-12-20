#include "Menu.h"

bool initMenu(Menu* menu, uint8_t menuSize)
{
    if ( !menuSize )
        return false;

    menu->menuSize = menuSize;
    menu->currentMenuElement = 0;
    return true;
}

MenuElementID getPressedMenuElement(Menu* menu)
{
    if (!menu->menuSize)
        return 0;
    return menu->currentMenuElement;
}

MenuElementID move(Menu* menu, Direction dir)
{
    if (!menu->menuSize)
        return 0;

    dir == Up ? menu->currentMenuElement-- : menu->currentMenuElement++;

    if (menu->currentMenuElement == menu->menuSize)
    {
        menu->currentMenuElement = 0;
    }

    else if (menu->currentMenuElement == 255)
    {
        menu->currentMenuElement = menu->menuSize - 1;
    }

    return menu->currentMenuElement;
}
