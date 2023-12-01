# TODO

Random list of things to be implemented on this library

1. Widgets
    1. ~~List view~~
    2. ~~Combo box~~
    3. StackWidget
    4. TabView
    5. Toolbar
        1. Actions
        2. Clickacble actions
        3. Toggle actions
        4. Actions with submenus
    6. ScrollBar
        1. Finish drawing barrel
        2. Click on empty areas - step up/down
        3. Drag the barrel - change value (needs mouse drag support?)
        4. Scroll wheel support
    7. Layout system (<https://www.qt.io/blog/responsive-layouts-in-qt>?)
    8. Menus
        1. sub-menu, popup-menu
        2. Window menu
            1. Initial implementation will be a "local menu bar" 
            2. Second implementation will be a proxy so we can have nice
               OSX global menus.
    9. Dialog
        1. Normal dialog
        2. Inline dialog (no window created)
        3. Message boxes
        4. Save/Open dir dialogs
        5. Choose dir dialog
2. Infrastructure
    1. Mouse drag
    2. Mouse wheel support
    3. DONE: ~~`widget::add<Widget>.. (constructor )` (is this possible at all?)~~
    4. All *getters* should return a reference
    5. All *callbacks* should have a reference to the widget as first argument
    6. Override functions for the library, callbacks for the consumers of the API
    7. Theme:
        1. `Widget::on_theme_changed()` - modify implicit widget sizes, color
        2. ~~Rip of color form theme, into color style.~~
        3. Make the frame part of a widget theme - so you can choose frame per widget
        3. Add GTK based theme
        4. Add Aqua (old OSX) theme
        5. Add OSX theme
        6. Add support for a 'transparent color' (currently 0 paints black)
    8. OSX support
    9. MSDOS support (for gaming!?)
    10. Multitouch (?)
    11. High DPI: drawing functions will be on a wrapper - that will
        multiply values before caling good-old-raster-code
    12. Rounded corner rectangle drawing
    13. Better bitmap support
        1. Alpha blending drawing
        2. At least transparent color - non-drawing.
    14. Use freetype for better text
    15. Use Harfbuzz for better rendering
    16. Wayland native support (see <https://gaultier.github.io/blog/wayland_from_scratch.html>)
    17. Connan recipe - for consumers of this library
    18. VCPKG - both internally, and for consumers of this library
    19. Rust integration? How can I use this in Rust?
    20. D integration?  How can I use this in D?
    21. GoLang integration?  How can I use this in GoLang?
    22. Clipboard integration
    23. Printing support
    24. Move all code into a namespace. `namespace svision;`, `auto a = std::make_shared<svision::Widget>(pos,size);`
    25. Move all public includes into `/inc/svision`, and
        in the cmake - make it public api (will trigger
        a refactor in all code - `#include <svision/widget>`)
    26. i18n
        1. We need to  have a `_()`/`tr()` macro which will ask for the translated string  at runtime
        2. Default implementation will return the same string
        3. We need a way to extract messages into a translation templeate
        4. We eed to be able to merge a translation template, into a translation list
        5. Do we have avialable formats? 
    27. Actions with shortcuts support
3. 3rd party porting
    1. Code to load images: I would like to be able to choose  which decoders to use:
            (libpng, libjpg) vs, <https://libspng.org/>, and stb (<https://github.com/nothings/stb>)
    2. HTML widget - <https://github.com/litehtml/litehtml>
    3. TextEditor - <https://www.scintilla.org/>
