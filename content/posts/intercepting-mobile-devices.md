---
title: "Man in the Middle attacks on mobile apps"
categories: ["Articles"]
tags: ["MITM","ARP","mobile","Iot"]
date: 2018-02-01T17:17:29Z
---
Written by [Jay](authors/jayharris)

During most mobile application or IoT pen test, it's often a requirement to perform a man in the middle attack to view network traffic. Below are some approaches which work when the it's not possible to set a system proxy (or when a mobile application does not honour it).

# ARP Poisioning

## Pros/Cons

Pro | cons
--- | ---
Easy |\|Active attack; can be detected
Redirects all traffic |\|Can be risky
 |\|Device and laptop need to be on the same network.

## Tools

* Ettercap

## Launching the attack

Starting the attack is simple. Using ettercap and the IP address of the gateway and the mobile device we can run the following command

```
ettercap -T -M arp:remote //<ip address of gw>/ //<ip address of device>/
```


# DNS Spoofing

DNS Spoofing works by configuring the device to use a rouge DNS server. When the application attempts to connect to a domain, it will query the rouge DNS server and the IP address of our machine will be returned instead of the legitimate one. As the connection then comes to us, we can intercept it and send it on therefore allowing us to perform a Man in the Middle attack.

## Pros/Cons

Pros | Cons
--- | ---
Can easily target specific domains |\|Only works with requests to domains
Reliable |\| 
Can be on separate networks |\| 


## Tools
* DNS Server (e.g. metasploit fakedns module)

## Launching the attack

First, we need to start the malicious DNS server. It should be configured to return our IP address of for any domain we're interested in intercepting.

```
msf > use auxiliary/server/fakedns
msf auxiliary(fakedns) > show options
Module options (auxiliary/server/fakedns):

Name Current Setting Required Description
---- --------------- -------- -----------
DOMAINBYPASS www.google.com yes The list of domain names we want to fully resolve
SRVHOST 0.0.0.0 yes The local host to listen on.
SRVPORT 53 yes The local port to listen on.
TARGETHOST no The address that all names should resolve to

msf auxiliary(fakedns) > exploit
```

To redirect traffic to a proxy to intercept/modify traffic see the section on Intercepting traffic below.

## USB Tethering

As android allows us to create a network/teather via USB, we can use this to intercept the connection to the Internet and perform a Man in the Middle account.

Pros | Cons
--- | ---
Doesn't require wireless access points |\|Requires a rooted device
 |\|Limited to devices with this capability

The below script sets up a reverse USB tether (where the Android device connects to the Internet via the computer's Internet connection).

```
#!/bin/bash

WAN="wlp2s0"       # interface providing internet connection on the PC
LAN="enp0s20u1"        # usb interface on the PC
AND="rndis0"      # usb interface on Android

LAN_IP="10.0.0.1"
AND_IP="10.0.0.10"
NETMASK="255.255.255.0"

DNS1="8.8.8.8" #replace with malicious DNS server to use DNS spoofing
DNS2="8.8.4.4" 

sudo su -c "echo 1 > /proc/sys/net/ipv4/ip_forward"
sudo iptables -F -t nat
sudo iptables -F FORWARD
sudo iptables -t nat -A POSTROUTING -o $WAN -j MASQUERADE
sudo iptables -A FORWARD -i $WAN -o $LAN -m state --state RELATED,ESTABLISHED -j ACCEPT
sudo iptables -A FORWARD -i $LAN -o $WAN -j ACCEPT

sudo ifconfig $LAN $LAN_IP netmask $NETMASK up

adb shell su -c busybox ifconfig $AND $AND_IP netmask $NETMASK up
adb shell su -c busybox route add default gw $LAN_IP
adb shell su -c ndc resolver setifdns $AND "" $DNS1 $DNS2
adb shell su -c ndc resolver setdefaultif $AND

adb shell ping -c4 $DNS1
adb shell ping -c4 google.com
```
 
With reverse tethering in use, unencrypted traffic can be sniffed with something like Wireshark on the host, or traffic can be redirected to burp (see the Intercepting traffic section below).

## Malicious Access Point

Pros | Cons
--- | ---
Easy to setup |\|Requires additional network card/hardware
No device configuration |\|AP needs to be protected
Works for any device that supports WiFi |\|Some sites limit ability to create additional wireless networks 

This approach works very well as nearly all devices support wireless networking and so is often the first thing to try, especially since this functionality is now in Kali by default.

If using Kali in a VM, the VM should be configured with a USB Wi-Fi adapter as well as access to the Internet (NAT or Bridged network adapter).

In the Wi-Fi settings in Kali, Wi-Fi hotspot can be enabled as follows:

![image](images/intercepting-mobile-devices/turn-on-wifi-hotspot.png)

To send HTTP traffic to a proxy, see the below section on proxy configuration.

## Intercepting traffic

### Intercepting SSL

There are two ways to intercept SSL/TLS connections (i.e. https). The first (and preferred way) is to install a new root CA on the device. The other approach is to try and downgrade the connection from https to http which is unencrypted.

#### Install a CA

This depends on the device we're trying to intercept. Check out the documentation but for Android/iOS, the cert can be installed by tapping on a valid cert.

```
wget 127.0.0.1:8080/cert -O cert.crt
adb push cert.crt /sdcard/
```

With the certificate on the device, on Android it can be installed by tapping "Settings->Security & Lock Screen->Credential storage->Install from Storage" then finding the certificate.

#### Downgrading the connection

SSLStrip can be used to remove SSL and downgrade the connect to HTTP. This is occasionally useful although dated. 

Note: You may need to uncomment the following from etter.conf

ec_uid = 65534

ec_gid = 65535

If you're having issues with a security control called HSTS, you may have more luck with SSLStrip2

```
echo "1" > /proc/sys/net/ipv4/ip_forward
iptables -t nat -A PREROUTING -p tcp --dport 80 -j REDIRECT --to-ports <port>
sslstrip -a -f -l 8080
```

### Burp/proxy

With a proxy running (here we're assuming it's running on port 8080) and all traffic running though our machine, we can use some iptable rules to redirect all traffic coming in on port 80 and 443 (http and https) to the proxy (port 80)

```
iptables -t nat -A PREROUTING -p tcp --dport 80 -j REDIRECT --to-ports 8080
iptables -t nat -A PREROUTING -p tcp --dport 443 -j REDIRECT --to-ports 8080
```

If the proxy is on another IP address, use the following, where <lan> is the interface the connected to the device and <IP>:<port> is the IP address/port of the proxy.

```
iptables -t nat -A PREROUTING -i wlan0 -p tcp --dport 80 -j DNAT --to-destination 10.0.2.2:8080
iptables -t nat -A PREROUTING -i wlan0 -p tcp --dport 443 -j DNAT --to-destination 10.0.2.2:8080
```

The proxy (Burpsuite in this case) should be configured to listen on the external interface and should be set to invisible mode. These options can be found in the Proxy->Options->Proxy Listeners section.

![image](images/intercepting-mobile-devices/burp-proxy-options.png)

![image](images/intercepting-mobile-devices/burp-binding.png)

![image](images/intercepting-mobile-devices/burp-request-handling.png)



