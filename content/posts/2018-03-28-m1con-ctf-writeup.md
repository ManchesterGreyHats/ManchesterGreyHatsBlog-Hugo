---
categories: ["articles"]
tags: ["Mobile", "CTF", "Android", "Frida", "PaulW"]
date: 2018-03-20T23:50:00Z
title: "M1Con CTF Write up"
url: /2018/03/28/m1con-ctf-writeup/
---

Presented by [Paul w](authors/PaulW/)

(Follow me)(https://twitter.com/phyushin)

Introduction
===========

Last night I went to the meetup group ["M1 Con"](https://twitter.com/M1Con_) hosted by [Digital Interruption](https://twitter.com/DI_Security) and [Outsource UK Ltd.](https://twitter.com/outsourceuk)

[Jay Harris](authors/jayharris) gave a talk about mobile security - it wasn't _super_ technical, but it highlighted the fact that clearly security is still a bit of an after thought especially when it comes to mobile development - he cited examples of issues that crop up in mobile devices that were fixed in web apps (XSS vulnerabilities for example) long ago.

He also gave a shout out to the [Manchester Grey Hats](https://twitter.com/mcrgreyhats) - but you're already here so ... you're already checking us out ... moving on

The CTF
========

So the CTF brief we were given was that it was a bit of OSINT (link to [Brett's OSINT Workshop](/2017/10/25/osint-workshop/)) and a bit of mobile security involved.
There were a couple of USB pen drives going around with a live image (Jay assured us the USB sticks were _legit_) for us to boot to with most of the environment set up this included an android virtual device (running Android 7 - _Nougat_) the CTF APK - an application called _safepass_ and a couple of tools - ADB and a tool called [JADX](https://github.com/skylot/jadx) which is used to decompile the APK into readable android code.

After unsuccessfully trying to run the APK in the android emulator in the live environment for a while I decided I was going to look at the APK code and see if I could rebuild the APK in Android Studio with some helpful breakpoints. First I put the APK on a pen drive and rebooted back into my regular environment, created a virtual device in android studio as I was going to do this anyway and installed the APK and ran it. I was greeted with a login screen what a _handsome_ devil!
![image](images/m1conctf/login_screen.png)

As you can see the app lists a user name but asks for a password. Since I heard there was going to be some OSINT in this CTF I started searching for _handsomerob_ some of the results were ... interesting
but on twitter I found a user that had the same image and user name - so that probably wasn't coincidence.

![image](images/m1conctf/handsome_twitter.png)

Seeing as this is OSINT we should poke around this user's twitter account and see what info may be relevant.

![image](images/m1conctf/twitter_posts.png)

Looking at the user's posts they have posts about kiwis and complains about having to use numbers in passwords. A couple of likely passwords:

```
kiwi1
k1w1
Kiwi1
K1w1
Kiwi0379
kiwi0379
```

![image](images/m1conctf/safepass_password.png)

Entering the correct password gives us a a 2fa prompt ...

![image](images/m1conctf/safepass_2fa.png)

But we don't have any info on the 2fa ... we'll try 0379... nope, let's bust the apk with Jadx.

Jadx
=====
Jadx is a tool that allows you to export apk into a gradle project so you can review the code.

Select the apk to export

![image](images/m1conctf/jadx_selecting_apk.png)

as you can see by looking at the apk in jadx, it shows the code in a tree including the package name.

![image](images/m1conctf/jadx_structure.png)

you can also export into a gradle project to open in android studio.

![image](images/m1conctf/export_to_gradle.png)

Examining the project structure in jadx we can also see there is an SQLite DB - we can look at this in jadx but it's a little garbled ... instead what we're going to to is open the apk with an archive manager [winrar](https://www.rarlab.com/download.htm) will work if you're in Windows, in Linux you'll be able to just pop it open from the file explorer
and in the assets folder there will be a database file _safepass.db_. Let's examine it using SQLiteBrowser.

SQLiteBrowser
=============

[SQLiteBrowser](http://sqlitebrowser.org) is a tool that allows you to browse sqlite databases.


_settings table_

![image](images/m1conctf/settings_table.png)

_credentials table_

![image](images/m1conctf/credentials_table.png)

So the string we need to decrypt is the notes field in the credentials_table : _Qqb1yxdZYPpO7IkgcwgY8Viv4lmNw/MQlb128tpcC1n+05vNWKRZrypzDWE3rtuG_

The Android Code
================
Looking at the code we can see the following functions that seem interesting

_LoginActivity.java_
```java
public class LoginActivity extends AppCompatActivity implements OnClickListener {
  private String getAuthenticationToken() {
       Cursor cursor = this.mDatabase.query("settings", new String[]{"key", "value"}, "key = ?", new String[]{"aes_token"}, null, null, null);
       String retval = null;
       if (cursor.moveToFirst()) {
           retval = cursor.getString(1);
       }
       cursor.close();
       return retval;
   }

   public boolean checkPassword(String password) {
       if (password.trim().equals("")) {
           return false;
       }
       return new CryptoHandler(password).decrypt(getAuthenticationToken()).equals("DI{Th15_u53r_15_l0gg3d_1n}");
   }

   private void login() {
      if (checkPassword(this.mPasswordEditText.getText().toString())) {
          this.mHasValidPassword = true;
          toggleControls(false);
          return;
      }
      Toast.makeText(this, "The password entered was incorrect", 1).show();
  }

  //...Snip

  public void onClick(View view) {
      if (view != this.mLoginButton) {
          return;
      }
      if (this.mHasValidPassword) {
          this.mHasValidPassword = false;
          toggleControls(true);
          Toast.makeText(this, "The token entered was incorrect", 1).show();
          return;
      }
      login();
  }
}

```

_CryptoHandler.java_
```java
public class CryptoHandler {
    private static CryptoHandler instance = null;
    private byte[] mIV;
    private byte[] mKey;

    public CryptoHandler(String secretKey) {
        try {
            this.mKey = (secretKey + "0000000000000000").substring(0, 16).getBytes("UTF8");
            this.mIV = "itsasecret000000".getBytes("UTF8");
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }
    }

    public String encrypt(String message) {
        try {
            byte[] srcBuff = message.getBytes("UTF8");
            SecretKeySpec keySpec = new SecretKeySpec(this.mKey, "AES");
            IvParameterSpec ivSpec = new IvParameterSpec(this.mIV);
            Cipher ecipher = Cipher.getInstance("AES/CBC/PKCS7Padding");
            ecipher.init(1, keySpec, ivSpec);
            return Base64.encodeToString(ecipher.doFinal(srcBuff), 0);
        } catch (Exception ex) {
            ex.printStackTrace();
            return "";
        }
    }

    public String decrypt(String encrypted) {
        try {
            SecretKeySpec keySpec = new SecretKeySpec(this.mKey, "AES");
            IvParameterSpec ivSpec = new IvParameterSpec(this.mIV);
            Cipher ecipher = Cipher.getInstance("AES/CBC/PKCS7Padding");
            ecipher.init(2, keySpec, ivSpec);
            return new String(ecipher.doFinal(Base64.decode(encrypted, 0)), "UTF8");
        } catch (Exception ex) {
            ex.printStackTrace();
            return "";
        }
    }
}

```
looking at the _checkPassword_ function, it creates a new instance of CryptoHandler passing the password
and then checks that the value returned by the _decrypt_ function equals __"DI{Th15_u53r_15_l0gg3d_1n}"__
```java
  return new CryptoHandler(password).decrypt(getAuthenticationToken()).equals("DI{Th15_u53r_15_l0gg3d_1n}");
```
From both the decrypt and encrypt functions we can see that the cipher is "AES" and we know the password is `kiwi0379`

```java
  this.mKey = (secretKey + "0000000000000000").substring(0, 16).getBytes("UTF8");
  this.mIV = "itsasecret000000".getBytes("UTF8");
```
We know we have all we need to decrypt the notes as we have everything to build the key so we _could_ take those things and use something like [CyberChef](https://gchq.github.io/CyberChef/#recipe=AES_Decrypt) but where's the fun in that? ;-)

We're going to use the app against itself with frida!


Frida
=====
Seeing as I wanted to play around with [Frida](https://www.frida.re) a little bit I thought I'd use it to solve this challenge.
I created an android virtual device and install the CTF app on it,

First you need we need to push the frida server to the phone or the virtual device
```
phyu@Balamb:~/Android/Sdk/platform-tools$ ./adb push ~/Dev/M1Con/frida-server /data/local/tmp/frida-server
/home/phyu/Dev/M1Con/frida-server: 1 file pushed. 168.5 MB/s (25126612 bytes in 0.142s)
```

And then connect to the shell.

```
generic_x86:/ $ su
generic_x86:/ # cd /data/local/tmp                                                                                                                
generic_x86:/data/local/tmp # chmod 775 frida-server                                                                                            
generic_x86:/data/local/tmp # ./frida-server &
```

Once we have the server running on the device we need to connect to it.

```
phyu@Balamb:~$ frida -U -f com.digitalinterruption.safepass.safepass --no-pause
     ____
    / _  |   Frida 10.7.7 - A world-class dynamic instrumentation toolkit
   | (_| |
    > _  |   Commands:
   /_/ |_|       help      -> Displays the help system
   . . . .       object?   -> Display information about 'object'
   . . . .       exit/quit -> Exit
   . . . .
   . . . .   More info at http://www.frida.re/docs/home/
Spawned `com.digitalinterruption.safepass.safepass`. Resuming main thread!
[Android Emulator 5554::com.digitalinterruption.safepass.safepass]->

```
We get the above window and then we know it's running. Now we can create a script file that we inject into the running program, which will allow us to overwrite functionality of the program.

```
phyu@Balamb:~$ frida -U -l ~/Dev/M1Con/ctf.js -f com.digitalinterruption.safepass.safepass --no-pause
```
the current contents of ctf.js is

__CTF.js (initial)__
```javascript
setImmediate(function() {
	Java.perform(function () {
		console.log("[*] Starting script");
	});
});
```

What currently happens is the user clicks the login button and it calls the decrypt function like so:

```java
  decrypt(getAuthenticationToken()).equals("DI{Th15_u53r_15_l0gg3d_1n}");
```

What we want to do is call the decrypt function to decrypt the notes field. What's really cool is we can actually use the current implementation to decrypt it.

With frida we can use the [Java.use](https://www.frida.re/docs/javascript-api/#java) function to create a wrapper for the CryptoHandler class

```java
  var crypto = Java.use("com.digitalinterruption.safepass.safepass.CryptoHandler");
```
It's important to note that `crypto` will now refer to the CryptoHandler that's instantiated when the user clicks the login button so we need to provide the correct passwords.

What we do then is instead of checking if the password matches, log the output of the decrypt function after passing in the notes field

__CTF.js (final)__
```javascript
setImmediate(function() {
	Java.perform(function () {
		console.log("[*] Starting script");
		var crypto = Java.use("com.digitalinterruption.safepass.safepass.CryptoHandler");
    console.log("[*] Overriding implementation of decrypt function in : com.digitalinterruption.safepass.safepass.CryptoHandler")
		crypto.decrypt.implementation = function(){//we change the decrypt function to decrypt the notes field instead of the password
			console.log("called decrypt");
			console.log(this.decrypt("Qqb1yxdZYPpO7IkgcwgY8Viv4lmNw/MQlb128tpcC1n+05vNWKRZrypzDWE3rtuG")); //passing the string we want to decrypt instead of the token
			return "Solved"; //we need to return a string as the decrypt function we're overriding returns one and if we don't the app will throw an error
		}
	});
});
```

Success!!
=========
```
phyu@Balamb:~$ frida -U -l ~/Dev/M1Con/ctf.js -f com.digitalinterruption.safepass.safepass --no-pause
     ____
    / _  |   Frida 10.7.7 - A world-class dynamic instrumentation toolkit
   | (_| |
    > _  |   Commands:
   /_/ |_|       help      -> Displays the help system
   . . . .       object?   -> Display information about 'object'
   . . . .       exit/quit -> Exit
   . . . .
   . . . .   More info at http://www.frida.re/docs/home/
Spawned `com.digitalinterruption.safepass.safepass`. Resuming main thread!
[Android Emulator 5554::com.digitalinterruption.safepass.safepass]-> [*] Starting script
[*] Overriding implementation of decrypt function in : com.digitalinterruption.safepass.safepass.CryptoHandler
called decrypt
Congratulations on finishing the challenge! :)

```

Thanks for reading and thanks for the CTF :)  - Phyu
