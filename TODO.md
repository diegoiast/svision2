# TODO

Random list of things to be implemented on this library

1. Widgets
    1. ~~List view~~
    2. ~~Combo box~~
    3. ~~StackWidget~~
    4. ~~TabView/TabWidget~~
        1. ~~North~~
        2. West
        3. East
        4. South
        5. Small triangles (only south?)
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
    7. Layout system
        1. ~~Horizontal/Vertical layout~~
        2. <https://www.qt.io/blog/responsive-layouts-in-qt>?
        3. Other layouts?
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
    10. Scrolling inside widget
        1. Need to have an offset - from which the widget is dispalayed
        2. All drawing will reduce from this offset
        3. New widget with a scroll bar - to have a scroll area. (Horizontal
           layout, with a main sub widget, and a scroll bar).
    11. Textbox
        1. Fix cursor movement, by measuing on screen text
        2. Fix selection
           1. We need to be able to select part of the text
           2. When part of the text is selected - work uppon it (delete/replace text)
           3. Properly display the selected area.
    12. Buttons
        1. Bitmap on buttons
        1. Open menu on click
    13. Color picker
    14. Font picker
    15. Directory chooser
    16. File choosed

2. Infrastructure
    1. Mouse drag
    1. Mouse wheel support
    1. keyboard 
        1. Fix Unicode support (adding several char* on input, per unicode point)
        1. Propagate keyboard events
    1. ~~`widget::add<Widget>.. (constructor)` (is this possible at all?)~~
    1. All *getters* should return a reference
    1. All *callbacks* should have a reference to the widget as first argument
    1. Override functions for the library, callbacks for the consumers of the API
    1. Theme:
        1. `Widget::on_theme_changed()` - modify implicit widget sizes, color
        1. ~~Rip of color form theme, into color style.~~
        1. ~~Make the frame part of a widget theme - so you can choose frame per widget~~
        1. Add GTK based theme
        1. Add Aqua (old OSX) theme
        1. Add OSX theme
        1. Add Material (Andorid) theme
        1. Add support for a 'transparent color' (currently 0 paints black)
        1. Finish dark color support
        1. Missing API: when to use dark themes
    1. MacOS support
    1. MSDOS support (for gaming!?)
    1. Multitouch (?)
    1. High DPI: drawing functions will be on a wrapper - that will
        multiply values before caling good-old-raster-code
    1. Rounded corner rectangle drawing
    1. Better bitmap support
        1. Alpha blending drawing (blend pixel done).
        2. At least transparent color - non-drawing.
    1. ~~Use freetype for better text~~
    1. Use Harfbuzz for better rendering
    1. Wayland native support (see <https://gaultier.github.io/blog/wayland_from_scratch.html>)
    1. Connan recipe - for consumers of this library
    1. VCPKG - both internally, and for consumers of this library
    1. Rust integration? How can I use this in Rust?
    1. Clipboard integration
    1. Printing support
    1. Move all code into a namespace. `namespace svision;`, `auto a = std::make_shared<svision::Widget>(pos,size);`
    1. Move all public includes into `/inc/svision`, and
        in the cmake - make it public api (will trigger
        a refactor in all code - `#include <svision/widget>`)
    1. i18n
        1. We need to  have a `_()`/`tr()` macro which will ask for the translated string  at runtime
        2. Default implementation will return the same string
        3. We need a way to extract messages into a translation templeate
        4. We eed to be able to merge a translation template, into a translation list
        5. Do we have avialable formats?
    1. Minimum size
        1. Widgets need to have a minimum size
        2. Layouts need to have a minimum size (computing all sub items)
        3. Windows need to have a minimum size
           * problem: setting minimum size is trivial. Getting it on windows, is
             not. Might need to cache the value, and the function return might
             not be always read from the OS.
        4. Do we need a maximum?
    1. Relayout on first display on X11
3. 3rd party porting
    1. Code to load images: I would like to be able to choose  which decoders to use:
            (libpng, libjpg) vs, <https://libspng.org/>, and stb (<https://github.com/nothings/stb>)
    2. HTML widget - <https://github.com/litehtml/litehtml>
    3. TextEditor - <https://www.scintilla.org/>
4. Demos
    0. Demo/reference app
    1. Freetype rendering demo
    2. CopyPasta - clipboard manager, ala CopyQ
    3. SVComm - Nortoma/Midnight/Total Commander clone - dual panel file manager
5. Bugs
    1. Mouse hover with subwidgets is borked.
    2. TabWidget changing tab flickers
    3. Changing theme on the fly does not work
    4. TabWidget/TabHeader on Redmond theme - height is wrong.
