#!/bin/bash

#install rules, driver, etc

#Create group needed by fallback input
groupadd RemoteKeyboard

#UInput rules
if [ ! -e "/etc/udev/rules.d/10-local-RemoteKeyboard.rules" ]
then
	cp /usr/bin/RemoteKeyboard/tmp/10-local-RemoteKeyboard.rules /etc/udev/rules.d/
fi

#config folders and permissions
if [ ! -e /var/lib/RemoteKeyboard ]
then
	mkdir /var/lib/RemoteKeyboard/
fi

chmod -R 777 /var/lib/RemoteKeyboard/

#build driver
cd /usr/bin/RemoteKeyboard/tmp/Driver
make

#install driver
cp /usr/bin/RemoteKeyboard/tmp/Driver/vhost_controller.ko /lib/modules/$(uname -r)/kernel/drivers/input
depmod -a
if [ ! `grep -q vhost_controller "/etc/modules"` ] ;
then
	sed -i '$a vhost_controller' /etc/modules
fi

#load driver for this run
insmod /usr/bin/RemoteKeyboard/tmp/Driver/vhost_controller.ko

#run at boot
cd /home
directories="$(ls -d */)"
for directory in $directories
do
	trimmed_directory=${directory%/}
	if [ ! -d /home/$trimmed_directory/.config/autostart ]
	then
		mkdir /home/$trimmed_directory/.config/autostart
	fi
	cp /usr/bin/RemoteKeyboard/tmp/RemoteKeyboard.desktop "/home/$trimmed_directory/.config/autostart/"
done

#clean up temp files
#rm -rf /usr/bin/RemoteKeyboard/tmp

#Set permissions
chown :RemoteKeyboard /usr/bin/RemoteKeyboard/InputHandler
chmod g+s /usr/bin/RemoteKeyboard/InputHandler
chmod -R 755 /usr/bin/RemoteKeyboard/Resources

##run
#DISPLAY=:0.0
#export DISPLAY
#echo $DISPLAY
#
#export $(/usr/bin/dbus-launch)
#
#xhost +
#cd /usr/bin/RemoteKeyboard
#user_names="$(exec users)"
#echo $user_names
#unique_users="$(exec users | awk '!($0 in a) { a[$0]; print }')"
#echo $unique_users
#for user in $unique_users
#do
#	echo $user
#	#sudo -u $user /usr/bin/at now + 1 minutes -f /usr/bin/RemoteKeyboard/run-remote-keyboard
#	sudo -u $user /usr/bin/RemoteKeyboard/RemoteKeyboard &&
#	disown
#done
#xhost -

exit 0

