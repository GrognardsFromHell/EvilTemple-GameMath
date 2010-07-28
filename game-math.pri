
HEADERS += $${PWD}/include/gamemath.h \
        $${PWD}/include/gamemath_constants.h \
        $${PWD}/include/matrix4.h \
        $${PWD}/include/vector4.h

INCLUDEPATH += $${PWD}/include

linux-g++:QMAKE_CXXFLAGS += -msse -msse2 -mmmx -mfpmath=sse
win32-g++:QMAKE_CXXFLAGS += -msse -msse2 -mmmx -mfpmath=sse
win32-g++:QMAKE_CFLAGS += -msse -msse2 -mmmx -mfpmath=sse
