---
title: "Reddit Notification Router Mod"
date: 2012-07-07T09:29:33Z
categories: ["articles"]
tags: ["hardware", "router", "reddit", "shell script","Jay"]
---
Written by [Jay](authors/jayharris)

I wanted to be notified of reddit PMs when all computers were off. Unfortuantly I don't have a phone which can subscribe to RSS feeds so the solution was to create a couple of (quick and dirty) shell scripts which will blink a LED on a modified Buffalo WHR-G125 router running Tomato firmware.

Soldering is required to attach an LED to an unused output pin. If hardware hacking isn't your strong point (god knows it's not mine) you should be able to hijack one of the existing LEDs, such as DIAG or ROUTER.

After installation/modification you should have a LED which will flash at one second intervals when a reddit PM arrives and continue flashing until the message is read.

First, get and install Tomato firmware on the router. Even if you don't want to hack your router, Tomato is an excellent replacement firmware.

Create a JFFS partition on the router (this can be done via the web interface) and telnet into the router. The default username/password is root:admin

```
cd /jffs 
//find a way to copy files. Busybox wget will not work with https. 
//Either put them on a http server and wget them or start vi and copy/paste files
```

Add your json new message url to the checkNewRedditMessage.sh [script](https://github.com/jahmelharris/reddit-notification-shell-script) (tomato comes with vi)

Add the following Init Script (in the web interface)

```
cd /jffs sh checkNewRedditMessage.sh
Hardware
```

I used an unused output pin to wire up an external LED. If there is no unused pins it should be possible to hijack a used one. To see the output ports do

```
gpio enable <port number> gpio disable <port number>
```

which turns the LED on and off.

Note: Enable will turn the LED off, disable will turn the LED on

I found that setting port 4 on/off would restart and reset all router setting. Be warned.

![image](images/reddit-notification-router-mod/resize_1.jpg)

Shows inside the case and where the spare output pin is located

![image](images/reddit-notification-router-mod/resize_2.jpg)

Close up of my terrible soldering job

![image](images/reddit-notification-router-mod/resize_3.jpg)

Back together

