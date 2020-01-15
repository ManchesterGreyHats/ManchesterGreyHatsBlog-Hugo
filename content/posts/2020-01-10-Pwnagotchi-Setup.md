---
 Title: Pwnagotchi Set Up
 Date: 2020-01-13T23:30:00Z
 Author : John W
 Categories: ["Misc"]
 
 Tags: ["John W","Wi-Fi", "Raspberry Pi", "Weekend Projects"]
---
# Basic Setup

First things first you will need the following items

- Raspbery pi (with wireless it's better to use the pi0W but you can use a pi3)
- MicroSD card (at least 8 Gb)
- SD card reader(Duh!)
- A little patience (some times things won't got to plan accept this and take a breath ;))

## Step 1 - Installation of the Pwnagotchi .img
Head over to [the releases page](https://github.com/evilsocket/pwnagotchi/releases) and grab a copy of the the .img file, once the file has downloaded you will need write it to the SD card. there are a few ways to do this.

The way I will be using a tool called balenaEctcher (`https://www.balena.io/etcher/`) now that we have everything we need lets get started.

Open up the program and select the img and the device you want to write to. (make sure you dont write over a pendrive you forgot was plugged in ... I have a friend who did this) and it should look something like this:

![installer image](images/pwnagotchi/write_image.PNG)

After writing is complete, eject your sd card and then put it back in, the SD card should pop up in the file explorer menu.
Once we have that then right click on the the root of the boot partition and create a new text document and name it `config.yml` then open it up and paste in the following:
  ```
main:
  name: 'pwnagotchi' # change this to what ever you want to call the device
  whitelist:
    - 'YourHomeNetworkMaybe'
  plugins:
    grid:
      enabled: true
      report: true
      exclude:
        - 'YourHomeNetworkMaybe'

ui:
    display:
      enabled: false
      type: 'waveshare_2'
     
web:
    address: 0.0.0.0
    enabled: true
    on_frame: ''
    origin: ''
    port: 8080
    username: '' #your username 
    password: '' #your password
```

Once you have done this then you can eject the sd card and put it in your pi; the first time it boots it will take some time so just wait.
Once the green LED stops blinking you should then be able to connect to the pi via usb to your PC you should hear the _new hardware detected_ sound; this is the rndis gadget being installed (to connect via usb)

## Step 2 - Connecting to the RPI

So on the pi0w there are 2 mirco usb ports the one that is closer to the hdmi port it the data one and the one at the top is the power one.

![Pi connections](images/pwnagotchi/connections.PNG)

Now to connect to the pi we will need to use ssh, I normally use [putty](https://www.chiark.greenend.org.uk/~sgtatham/putty/) to SSH in windows.
In the hostname field type in the following `pi@10.0.0.2`, make sure the port number is 22, and type the name of your pwnagotchi into the `saved sessions` box and click save.
Now we need to set the address of the usb network. Go to the control panel and open the `network and sharing centre`, there will be another connection on my system it is called `Ethernet 2` it may be somthing else on yours.

Open the properties section, go to IP 4 and properties to set the IP adress to a manual IP:
  ```
  IP address: 10.0.0.1
  Subnet: 255.0.0.0
  Gateway: leave this blank

  Prefered DNS: 1.1.1.1
  ```
![configuration](images/pwnagotchi/network_config.PNG)  
And save this. Now go back to putty and you should now be able to connect to the pi with the following creds
``` 
  user: pi
  password: raspberry
```

Congratulations! You are now connected to the pi :)

Once you are logged in run the following commands:

```
passwd #(this will allow you to change your password from raspberry to what every you like)
sudo su #(this will switch you to the root user)
systemctl status pwnagotchi #(this will show you what the pwnagotchi is currently upto)
```
If you see a message in the log that says `web ui available at http://0.0.0.0:8080` That means that your Pwnagotchi is now set up as a headless device.

Navigate to the URL in your chosen web browser (mine's chrome); you will be prompted for a username and password (these are set by default to changeme:changeme).
Don't worry about that now that you have access to the web interface wou can go into the web config menu and change them. 

![web_config image](images/pwnagotchi/webcfg.PNG)

From here you can search for the settings and change them search for ui.web.username and enter it ui.web.password and enter it, then save it and go back to 10.0.0.1:8080

Restart in auto mode and voila you are done :)
