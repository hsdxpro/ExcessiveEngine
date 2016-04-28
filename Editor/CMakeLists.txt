INCLUDE_EXTERNAL_MSPROJECT( Editor Editor.vcxproj )

target_include_directories( Editor "./GeneratedFiles")
target_include_directories( Editor "$(QTDIR)/include")
target_include_directories( Editor "./GeneratedFiles/${CMAKE_BUILD_TYPE}")
target_include_directories( Editor "$(QTDIR)/include/QtCore")
target_include_directories( Editor "$(QTDIR)/include/QtGui")
target_include_directories( Editor "$(QTDIR)/include/QtWidgets")