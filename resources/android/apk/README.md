Marshmallow Game Engine
=======================

Executable
----------

You will need to build your project as a library instead of an executable, this
is a very simple procedure. **It's automatically done for the included demos**.

Layout
------

### Demo

Move the demo binary of your choice into the following path
**libs/armeabi/libmarshmallow_demo.so**.

### Custom

Copy your built library into the **libs/armeabi/** directory, then update both
**AndroidManifest.xml** and **res/values/strings.xml**.

Packaging
---------

### Using Eclipse

If you have the full ADT Bundle, you can load this project directly in Eclipse
and build it from there.

### Using Ant (console)

You will need to have the SDK Tools installed on your system.

We will need to get the project directory ready for Apache Ant. Run the
following command from the SDK directory:

    $ tools/android package update --path ${YOUR_PROJECT_FOLDER}

Now from your project directory run:

    $ ant debug

This will create a **bin** directory containing the **debug** apk package;
once you're ready to release you can run:

    $ ant release

