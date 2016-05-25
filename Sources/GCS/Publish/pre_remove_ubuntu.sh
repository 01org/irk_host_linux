#!/bin/bash

#Close running instances
killall -s SIGQUIT RemoteKeyboard

#Remove group needed by fallback input
groupdel RemoteKeyboard

#Remove UInput rules
rm -f /etc/udev/rules.d/10-local-RemoteKeyboard.rules

#uninstall driver
sed -e '/vhost_controller/d' -i /etc/modules
depmod -a
rm -f /lib/modules/$(uname -r)/kernel/drivers/input/vhost_controller.ko

#remove driver for this run
modprobe -r vhost_controller

#remove run at boot
cd /home
directories="$(ls -d */)"
for directory in $directories
do
	trimmed_directory=${directory%/}
	echo $trimmed_directory
	if [ -f /home/$trimmed_directory/.config/autostart/RemoteKeyboard.desktop ]
	then
		rm /home/$trimmed_directory/.config/autostart/RemoteKeyboard.desktop
	fi
done

exit 0

