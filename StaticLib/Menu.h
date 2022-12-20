#ifndef MENU_H

    #define MENU_H

    #include <stdint.h>
    #include <stdbool.h>

    typedef uint8_t MenuElementID;

    typedef enum{
        Up,
        Down
    } Direction;
    
    typedef struct Menu
    {
        uint8_t menuSize;
        uint8_t currentMenuElement;
    } Menu;
    
    bool initMenu(Menu* menu, uint8_t menuSize);
    MenuElementID getPressedMenuElement(Menu* menu);
    MenuElementID move(Menu* menu, Direction dir);

#endif