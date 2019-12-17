#win32{
#    win32:DEFINES+=_CRT_SECURE_NO_WARNING

    INCLUDEPATH += $$PWD/../SOEM-src/oshw/win32/wpcap/Include/
    INCLUDEPATH += \
       $$PWD/../SOEM-src/osal/win32/ \
       $$PWD/../SOEM-src/osal/ \
       $$PWD/../SOEM-src/oshw/win32/ \
       $$PWD/../SOEM-src/soem/

    # wpcap libs
    LIBS += $$PWD/../SOEM-src/oshw/win32/wpcap/Lib/Packet.lib \
            $$PWD/../SOEM-src/oshw/win32/wpcap/Lib/wpcap.lib
    LIBS += Ws2_32.lib \
            Winmm.lib

    HEADERS += \
        $$PWD/../SOEM-src/oshw/win32/wpcap/Include/pcap/bluetooth.h \
        $$PWD/../SOEM-src/oshw/win32/wpcap/Include/pcap/bpf.h \
        $$PWD/../SOEM-src/oshw/win32/wpcap/Include/pcap/namedb.h \
        $$PWD/../SOEM-src/oshw/win32/wpcap/Include/pcap/pcap.h \
        $$PWD/../SOEM-src/oshw/win32/wpcap/Include/pcap/sll.h \
        $$PWD/../SOEM-src/oshw/win32/wpcap/Include/pcap/usb.h \
        $$PWD/../SOEM-src/oshw/win32/wpcap/Include/pcap/vlan.h \
        $$PWD/../SOEM-src/oshw/win32/wpcap/Include/bittypes.h \
        $$PWD/../OEM-src/oshw/win32/wpcap/Include/ip6_misc.h \
        $$PWD/../SOEM-src/oshw/win32/wpcap/Include/Packet32.h \
        $$PWD/../SOEM-src/oshw/win32/wpcap/Include/pcap-bpf.h \
        $$PWD/../SOEM-src/oshw/win32/wpcap/Include/pcap-namedb.h \
        $$PWD/../SOEM-src/oshw/win32/wpcap/Include/pcap-stdinc.h \
        $$PWD/../SOEM-src/oshw/win32/wpcap/Include/pcap.h \
        $$PWD/../SOEM-src/oshw/win32/wpcap/Include/remote-ext.h \
        $$PWD/../SOEM-src/oshw/win32/wpcap/Include/Win32-Extensions.h
    HEADERS += \
        $$PWD/../SOEM-src/osal/win32/inttypes.h \
        $$PWD/../SOEM-src/osal/win32/osal_defs.h \
        $$PWD/../SOEM-src/osal/win32/osal_win32.h \
        $$PWD/../SOEM-src/osal/win32/stdint.h \
        $$PWD/../SOEM-src/osal/osal.h \
        $$PWD/../SOEM-src/oshw/win32/nicdrv.h \
        $$PWD/../SOEM-src/oshw/win32/oshw.h \
        $$PWD/../SOEM-src/soem/ethercat.h \
        $$PWD/../SOEM-src/soem/ethercatbase.h \
        $$PWD/../SOEM-src/soem/ethercatcoe.h \
        $$PWD/../SOEM-src/soem/ethercatconfig.h \
        $$PWD/../SOEM-src/soem/ethercatconfiglist.h \
        $$PWD/../SOEM-src/soem/ethercatdc.h \
        $$PWD/../SOEM-src/soem/ethercatfoe.h \
        $$PWD/../SOEM-src/soem/ethercatmain.h \
        $$PWD/../SOEM-src/soem/ethercatprint.h \
        $$PWD/../SOEM-src/soem/ethercatsoe.h \
        $$PWD/../SOEM-src/soem/ethercattype.h

    SOURCES += \
        $$PWD/../SOEM-src/osal/win32/osal.c \
        $$PWD/../SOEM-src/oshw/win32/nicdrv.c \
        $$PWD/../SOEM-src/oshw/win32/oshw.c \
        $$PWD/../SOEM-src/soem/ethercatbase.c \
        $$PWD/../SOEM-src/soem/ethercatcoe.c \
        $$PWD/../SOEM-src/soem/ethercatconfig.c \
        $$PWD/../SOEM-src/soem/ethercatdc.c \
        $$PWD/../SOEM-src/soem/ethercatfoe.c \
        $$PWD/../SOEM-src/soem/ethercatmain.c \
        $$PWD/../SOEM-src/soem/ethercatprint.c \
        $$PWD/../SOEM-src/soem/ethercatsoe.c
#}
