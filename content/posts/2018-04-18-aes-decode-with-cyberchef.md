---
categories: ["articles"]
tags: ["Mobile", "CTF", "Android", "CyberChef", "timw"]
date: 2018-03-20T23:50:00Z
title: "AES decode with Cyberchef"
url: /2018/04/18/aes-decode-with-cyberchef/
---

Presented by [Tim](authors/timwilkes)

[Follow me on Twitter](https://twitter.com/timmehwimmy)

Introduction
============

This is a follow up article to [Paul W](authors/paulw)'s write-up about [m1con's mobile ctf](/2018/03/28/m1con-ctf-writeup.md). In the article, Paul mentioned [Cyberchef](https://gchq.github.com/CyberChef), and it was a quick way of solving the challenge.

Since Cyberchef wasn't covered, and may provide a quicker solution for future ctf challenges, I decided to describe it here.

What is CyberChef?
==================

CyberChef was developed by GCHQ to perform quickly certain operations on inputs. This includes decoding certain types of encryption and well as standard text encoding. It's a great tool to use on something like HackTheBox to quickly solve certain challenges.

What's the scenario?
====================

In this case, we have 4 bits of text that we want to decode. We know from the challenge, these are encrypted with AES. We also know the IV (initalization vector) and the key. From this, we have all the ingredients to decode the get the actual text.

As a reminder, the password was "kiwi0349", which gave a key of "kiwi034900000000". The IV was "itsasecret00000000". The 4 pieces of text to decode were found in the SQLite database.

Setup
=====

Click on this link to take you to [Cyberchef](https://gchq.github.com/CyberChef). On the left are different "ingredients" and they can be dragged and dropped in to the middle to make a recipe. On the top right, is the input box and below it is the output text.

To make the recipe to solve the puzzle, we are going to need the three ingredients. The ingredients can be reordered, by dragging and dropping, if required.

The first ingredient we know we are going to need is AES-decode. Drag this in to the center and populate the kay and IV. Don't forget to select utf-8 rather than hex. The mode for this challenge is CBC, and the input and output are to be set to raw.

Populating the input box with all four strings of text, shows us our first issue, as the decoding hasn't worked at all. Looking at the both the strings (which end in =) and from the code, we can deduce the input is base64 encoded. For the ingredients, select base64 decode, and place it above the AES decoder.

This should show the first line decoded, but not the subsequent ones. This is because when we decode using AES, we aren't decoding 4 strings, we are decoding 1 block. To sort this out, CyberChef has an ingredient called fork. This splits the actions and repeats them, based on a selector. In our case, if we make this the carriage return, and place this ingredient at the top of the list the decoder text should become obvious.

As a reference, the recipe should look like this:

![image](images/aes-and-cyberchef/safepass.png)
