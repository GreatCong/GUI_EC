TEMPLATE = subdirs

#master
SUBDIRS += QT_VS_Dock

win32{
#windows
include(Component_Win.pri)
}
#unix{
##unix
#include(Component_Unix.pri)
#}
