---
title: "Blogging With Mgh"
categories: ["Articles"]
tags: ["mgh","blog","how to"]
date: 2017-12-17T17:09:50Z
---
Manchester Grey Hats is about bringing the community together and because of that, we wanted to create a blogging platform for anyone wanting to use it.

Blogging with MGH is not like bloging with some of the other platforms. It might seem quite complicated and roundabout to start with but it gives us a few benefits:
- Security. As we using static files so we have a smaller attack surface.
- Text files are easy to version, update and edit.
- Markdown is easy to convert should we change our solution later.
- Any user can submit changes and updates. This can be to individual posts but also to the whole site! If you're not happy with the look of the blog, it's easy for the community to update it.

With that out the way, here is how we can create a post.

As a prerequisite, the following software needs to be installed (although if you're really can't or if you dislike this idea, it is possible to do everything from the github webpage):
- git
- hugo

Depending on your OS, there may be a few different way to do this. On Linux it should be possible to install both packages though whatever package manager is in use:

`# apt-get install git hugo`

You'll also need a free [github account](https://github.com).

After logging into git, you can head over to the Manchester Grey Hats Blog git repository, and view the code of the blog. To make an edit or to add a post, this first needs to be forked which will copy the MGH blog repo to a repository you own.

To fork a repository, click the "Fork" button on the page of the repository - in this case https://github.com/ManchesterGreyHats/ManchesterGreyHatsBlog-Hugo

![Image](images/bloggingwithmgh/github-fork-repo.png)

You should now have a copy of the ManchesterGreyHatsBlog-Hugo repository. This needs to be cloned i.e. making a copy locally so that files can be updated. With git, this can be done from the commandline as follows:

`$ git clone ManchesterGreyHatsBlog-Hugo
cd ManchesterGreyHatsBlog-Hugo
`

Woo! We now have a local copy to work on. Here we can make any changes to the website we want but don't worry. Changes can be viewed and tested before they get deployed. We also have the ability to easily roll back to earlier versions thanks to git.

To add a post run the following command:

`hugo new posts/<post_name>.md`

Replace <post_name> with the name of your post. Hugo will then create a new markdown file for you. This file can then be opened in your favourite text editor and your post can be written. If you're not familiar with markdown, it's a way to describe how text should be displayed. Wikipedia has some good [examples](https://en.wikipedia.org/wiki/Markdown) and there is a good cheatsheet [here](https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet). I'd recommend looking at how other posts have been created. If markdown seems complicated, it isn't really. Mostly, you're free to just type your post as you wish with a few extra things if you want lists, links, images etc.

At the top of the post, you will see some of the post metadata such as title, date etc. Some of this needs updating. First, remove the _draft: true_ line. I'd also recommend at least adding the relevant categories and tags. The category should either be workshop or article but the tags can by anything you feel is relevant. 

This can be done as follows:

`categories: ["article"]
tags: ["whatever", "is", "relevant"]`

If you're a new author, you might want to have a page describing who you are. This can be done with:

`hugo new authors/<your_name>.md`

Add some information about yourself and set the category to _Authors_ and remove the _draft: true_ line:

`categories: Authors`

If you'd like to view the posts as you're writing it, you can run a local instance of hugo and opening a webbrowser to localhost:1313 (by default)

`$ hugo serve -D
`
When you're happy with the post, it's time to commit it and push it to your repo:

`git add .
git commit -m <commit message>
git push
`

Make sure you replace _<commit message>_ with something meaningful such as "Post on xyz added" or "information updated for 2018".

Now it's time to create a pull request from the github page.

Navigate to your fork of the MGH blog repo at _https://github.com/<yourusername>/ManchesterGreyHatsBlog-Hugo_ and click the "New Pull Request" button.

![image](images/bloggingwithmgh/new-pull-request-1.png)

Check your changes and when you're happy click "Create pull request".


![image](images/bloggingwithmgh/new-pull-request-2.png)

Finally, add a bit of information and click "Create pull request"

![image](images/bloggingwithmgh/new-pull-request-3.png)

An admin will then review and merge your new post. Once merged, the post will appear on the website almost instantly!
