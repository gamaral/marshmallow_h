Marshmallow Game Engine
=======================

Deployment
----------

You will need to build your project as a library instead of an executable,
this is a very simple procedure and **it's automatically done for the included
demos**.

### Demo

Move the demo binary of your choice into the following path
**libs/armeabi/libmarshmallow_demo.so**.

Then simply, import this directory into eclipse or build the apk by hand and
deploy.

### Custom

Copy your built library into the libs/armeabi/ directory and update both
AndroidManifest.xml and res/values/strings.xml.

Then simply, import this directory into eclipse or build the apk by hand and
deploy.

