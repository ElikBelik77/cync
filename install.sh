#!/bin/sh

make all
if [ ! -d /usr/local/etc/cync ];
then
	mkdir -p /usr/local/etc/cync
fi
cp configuration.txt /usr/local/etc/cync/configuration.txt
cp ./build/cync /usr/local/bin/cync
cp cyncservice /etc/init.d/cyncservice

