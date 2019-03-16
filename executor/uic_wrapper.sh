#!/bin/sh
LD_LIBRARY_PATH=/media/msys/20a682fc-f67a-418e-840c-707510ddc9be/qt-5.6/qt-everywhere-opensource-src-5.6.0/qtbase/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}
export LD_LIBRARY_PATH
QT_PLUGIN_PATH=/media/msys/20a682fc-f67a-418e-840c-707510ddc9be/qt-5.6/qt-everywhere-opensource-src-5.6.0/qtbase/plugins${QT_PLUGIN_PATH:+:$QT_PLUGIN_PATH}
export QT_PLUGIN_PATH
exec /media/msys/20a682fc-f67a-418e-840c-707510ddc9be/qt-5.6/qt-everywhere-opensource-src-5.6.0/qtbase/bin/uic "$@"
