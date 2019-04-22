TEMPLATE = subdirs

SUBDIRS += QT_VS_Dock \
       DRE_Master \
       DRE_Master_Loader

# plugins
SUBDIRS += Plugins_src/User_plugins \
           Plugins_src/Control_Plugins \
           Plugins_src/Measure_plugins \
           Plugins_src/DRE2008OS_plugins \
           Plugins_src/DoubleControl_plugins \
           Plugins_src/Scara_Control_plugins
