# Using direct paths. Edit here if you want to install in different locations

build:
	sudo apt-get install gcc make libudev-dev libusb-1.0-0-dev libhidapi-hidraw0
	gcc -Wall -o victor86d src/victor86d.c -lusb-1.0 -lpthread -Isrc -Lsrc -lhidapi-hidraw

install:
	sudo cp src/99-hidraw-permissions.rules /etc/udev/rules.d/
	sudo /etc/init.d/udev restart
	sudo cp victor86d /usr/local/bin
