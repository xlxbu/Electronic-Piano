#!/bin/sh

ln -s /media/mmcblk0/lib/libQtCore.so.4 /usr/lib/libQtCore.so.4

ln -s /media/mmcblk0/lib/libQtGui.so.4 /usr/lib/libQtGui.so.4

ln -s /media/mmcblk0/lib/libQtNetwork.so.4 /usr/lib/libQtNetwork.so.4

ln -s /media/mmcblk0/lib/libstdc\+\+.so.6 /usr/lib/libstdc\+\+.so.6

ln -s /media/mmcblk0/lib/libm.so.0  /usr/lib/libm.so.0

ln -s /media/mmcblk0/lib/libc.so.0 /usr/lib/libc.so.0

ln -s /media/mmcblk0/lib/ld-uClibc.so.0 /usr/lib/ld-uClibc.so.0

ln -s /media/mmcblk0/lib/libQtSvg.so.4 /usr/lib/libQtSvg.so.4

ln -s /media/mmcblk0/lib/libQtXml.so.4 /usr/lib/libQtXml.so.4

echo -ne "\033[9;0]\033[14;0]" > /dev/ttyo

export QWS_MOUSE_PROTO='tslib:/dev/input/touchscreen0'

export TSLIB_CONFFILE=/etc/ts.conf

export TSLIB_PLUGINDIR=/usr/lib

export TSLIB_TSDEVICE=/dev/input/event0

export TSLIB_FBDEVICE=/dev/fb0

export TSLIB_CONSOLEDEVICE=/dev/tty

export QT_QWS_FONTDIR=/media/mmcblk0/lib/fonts



export TSLIB_PLUGINDIR=/usr/lib/ts

./Final_Project -qws
