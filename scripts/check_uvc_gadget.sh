#!/bin/bash

# Write the cron job to the file
#CRON_JOB="* * * * * root bash /home/pulses/uvc-gadget/scripts/check_uvc_gadget.sh"
#echo "$CRON_JOB" | sudo tee /etc/cron.d/check_uvc_gadget > /dev/null

if ps -eaf | grep -v grep | grep 'uvc-gadget' > /dev/null
then
    logger "uvc-gadget is running"
else
    logger "uvc-gadget is NOT running"
fi
