######################################################################
#Script to run application in ubuntu
######################################################################
#QT library path

export QT_DIR=$HOME/Qt/5.6.3/gcc_64
echo $QT_DIR
export QT_XCB_GL_INTEGRATION=xcb_egl
export PATH=$QT_DIR/bin:$PATH
export LD_LIBRARY_PATH=$QT_DIR/lib:$LD_LIBRARY_PATH

#Executable
/usr/local/bin/LoTo
