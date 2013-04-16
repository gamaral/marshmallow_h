Marshmallow Game Engine
=======================

**“The only game engine that tastes better slightly burned!”**

Coding standard
---------------

The coding standard is still a work in progress, I'm currently working on
normalizing the codebase. So you're best bet is to checkout recent commits, I
usually normalize files I work on.

I'll write up a proper document once I settle on the style, but for now, reading
the [FreeBSD style guidelines][style9] will give you and idea of how I write
code (albeit with a few OOP tweaks)

Things to keep in mind while working on Marshmallow:

  * Engine is primarily targeted to Unix systems, but any help with keeping it
    working on other system like Windows is appreciated.

  * Follow the coding style, copy what you see in other files:

    - Indentation: Tab indentation, 4 space soft-tabs and no spaces before tabs.

FAQ
---

### Why are some subsystems not classes/objects (OOP)?

In Marshmallow, any subsystems that won't need to be swapped or instantiated
more than once at runtime may use a simple *namespaced* function interface.

This is specially true for platform-specific subsystems like Graphics::Viewport,
since not all platforms require the use of windows, surfaces, etc.

How the subsystem is implemented behind the interface is not important, it may
indeed decide to use objects, that's up to the implementor.

[style9]: http://www.freebsd.org/cgi/man.cgi?query=style&sektion=9 "Style guide for FreeBSD"
