#!/bin/sh
# This script must run without sudo

sudo apt-get -y update
sudo apt-get -y upgrade
sudo apt-get install -y git libopencv-dev meson libcamera-dev

# Modify /boot/config.txt and /boot/cmdline.txt files
if grep -Fq "dtoverlay=dwc2" /boot/config.txt
then
    echo "Already processed /boot/config.txt"
else
    echo "dtoverlay=dwc2" | sudo tee -a /boot/config.txt
    echo "start_x=1" | sudo tee -a /boot/config.txt
fi

if grep -Fq "modules-load=dwc2,libcomposite" /boot/cmdline.txt
then
    echo "Already processed /boot/cmdline.txt"
else
    sudo sed -i -e 's/rootwait/rootwait modules-load=dwc2,libcomposite/g' /boot/cmdline.txt
fi

# Clone project
cd /home/$USER
git clone https://github.com/jianhanlim/uvc-gadget.git
cd /home/$USER/uvc-gadget
make uvc-gadget && cd build && sudo meson install && sudo ldconfig

chmod +x /home/$USER/uvc-gadget/scripts/multi-gadget.sh
sudo cp /home/$USER/uvc-gadget/scripts/video_formats.txt /etc/video_formats.txt

# Adding everything to /etc/rc.local
if grep -Fq "multi-gadget.sh" /etc/rc.local
then
    echo "Already processed /etc/rc.local"
    sudo chmod +x /etc/rc.local
else
    head -n-1 /etc/rc.local > /tmp/rc.local ;
    echo "sudo bash /home/$USER/uvc-gadget/scripts/multi-gadget.sh" >> /tmp/rc.local ;
    echo "sudo uvc-gadget -c 0 uvc.0 &" >> /tmp/rc.local ;  # must run with sudo to control the led light
    tail -n 1 /etc/rc.local >> /tmp/rc.local
    sudo mv /tmp/rc.local /etc/rc.local
    sudo chmod +x /etc/rc.local
fi

sudo reboot