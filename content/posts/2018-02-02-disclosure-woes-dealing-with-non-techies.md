---
categories: ["articles"]
tags: ["responsible-disclosure", "websec"]
date: 2018-02-02T00:00:00Z
title: "Disclosure Woes: Dealing With Non-Techies"
url: /2018/02/02/disclosure-woes-dealing-with-non-techies/
---

Written by [Rob](/authors/rob/)

What I Am Writing About
=======================
Unlike most things I publish, this isn't going to focus on how I came to find a vulnerability, but rather the problems I had trying to disclose it, and things that I could have approached better, in the hope it may help other people in the future if faced with being a small fish in a big pond which is trying to approach a larger organisation, who have few technical staff.

The Findings & Impact
=====================
Towards the end of 2017, I identified a flaw within a tenancy deposit scheme system, which resulted in a PII leakage. The vulnerability was able to be abused by unauthenticated users, and also resulted in PII being cached by numerous search engines, and had been doing so for a number of years.

The data made available via this flaw, included:

* The full name and address of the landlord and their tenants
* Account numbers of landlords
* Deposit policy numbers

For those of you outside the UK, or those simply not aware of what a tenancy deposit scheme is - it is a scheme created by the UK government, which requires landlords to register the security deposit of their tenants with a government approved organisation. As of February, 2018, there are only three companies within the UK that the government have licensed to implement and conduct business around the scheme.

The system which contained the vulnerability, is of particular popularity amongst landlords, but, if we were to naively assume that each scheme authorised by the government has a 33% share of the market, based on figures for the year 2015 published by [HomeLet](https://homelet.co.uk/letting-agents/news/article/how-many-landlords-and-tenants-are-there-in-the-uk), that would _*theoretically*_ put over 1,782,000 UK residents at risk of a PII leakage.

In reality - the impact will be nowhere near this number, as the weakness was "secured" by a key with a significantly high level of entropy; making any potential abuse of the vulnerability a very noisy task, that would *hopefully* be picked up by the organisation's internal monitoring protocols.

How I [Naively] Approached The Disclosure
=========================================
As most of the research I have done up to this point, and vulnerabilities I've found, have been directly reportable to developers, I naively dismissed the fact that I was contacting non-technical people, about a very technical issue.

The risk I later realised I was running by doing this, is that they could misinterpret what I am sending as some form of threat, or view it as untrustworthy; as I was an individual entity contacting them about their system security, it may raise suspicion.

After attempting to find a technical support contact, and only being able to find a generic customer service e-mail address, I proceeded to e-mail them, on December 28th, to explain that I had found a security flaw within the website, and that I'd like them to provide me with contact information of someone that I can report the full findings to.

A day later, I received a response, indicating that my e-mail would be passed on to the marketing team. At this point, I felt that the initial e-mail may have gone misunderstood - as this wasn't really an issue for the marketing team; unless the development was being outsourced and headed up by someone in marketing to relay customer requirements.

Requesting Assistance
=====================
Despite the odd referral to the marketing team, I continued to wait for a response, for a few more days. After not hearing anything for a few more days, and the realisation that approaching the organisation as an individual may have not been a great idea, I proceeded to request [Disclosure Assistance via HackerOne](https://hackerone.com/disclosure-assistance), in the hope they may be able to provide a more trustworthy appearance. Unfortunately, on 15th January, HackerOne responded to let me know the issue was not severe enough for them to take on (they must receive **a lot** of requests for assistance, so this was completely understandable).

Although the request for assistance had been declined, it did also make me feel a bit more confident in continuing to pursue it on my own; as if they considered it to be of low risk, it may not be as severe as I was interpreting it to be, and thus my worries of repercussion for trying to help may just be a bit over the top.

Another Attempt at Disclosure
=============================
After 17 days with no further correspondence from the organisation, I chased them again to request an update:

```html
Hi *redacted*,

I’ve not had any correspondence about the below yet.  It’d probably be best discussed with a manager of the development team, as the issue is a technical one with the website.

To give a bit more information as to what the problem is - *redacted information as to what the flaw was*.

I have put together a report to explain exactly where the issue exists, how it can be replicated and how it can be remedied, but I’m not sure who to provide the information to.

Although the likelihood of it being abused may be low, it’s still a security flaw which is putting users personal information at risk.

Kind regards,

Rob
```

At the time, I felt sending this e-mail was a good idea, but in hindsight - it was a bad idea. Originally, I had set out to not disclose any technical information to the initial points of contact on support, and only provide it to someone I can have *some* confidence is trusted with such information; but in an attempt to try and get them to take it more seriously, disclosed a higher level overview of it, to a complete stranger.

Had the support representative not understood what was being explained to them and not understood the sensitivity of the situation, they could have [innocently] relayed the information passively to other parties, that may have understood how to maliciously use it. Alternatively, it could be possible the support representative understood it and could have used that information for their own personal gain, in some form; **TLDR: doing this was not a good idea**.

Downtime & Some Progress
=======================
On 16th January, the day after chasing the support team for a contact in the IT / development department, an e-mail was sent out to their users to let them know the system would be being taken down for an extended period of maintenance on the evening of 18th January.

As this had *seemingly* been the first downtime being incurred for a few years, it seemed as if they may be taking action; I still had not had any direct response from them, however.

During the night of the maintenance, I checked to see if the problematic resource was still available, to find it was now resulting in a 404; suggesting it had been removed. I continued to wait until the maintenance window was finished, and once the system was back up - the data no longer seemed to be available - **partial success!**

![](/images/disclosure-woes-dealing-with-non-techies/yes.gif)

Although the problem had been resolved, there was still the case of PII that had been cached in search engine results. So, there was still more to be done, and thus, I still needed to get more information to the team dealing with the situation.

A Request for My Report & More Chasing
======================================
The morning after the downtime, I received a response to the e-mail I had sent on January 15th, this time, requesting a copy of the report I had produced:

```html
Thank you for your email.

Can you please provide us with your report so we can pass this to the development team so we can look in to this issue for you.

Kind Regards,
```

Although I was aware at this point that I had already made the mistake once, of providing technical information to someone who is potentially sat outside the development department, I felt it was worth the risk to send my report for them to pass on, given the main issue had now been patched and the clean-up was the last step.

I proceeded to send the report and point out the information of the report that needed particular attention:

```html
Hi *redacted*,

From the looks of it, last night's maintenance may have resolved the issue, as the *redacted detail* no longer available. However, there are still some remnants of exposed *redacted detail* that are cached on public search engines such as Google.

In the report I have attached, there is information on how the "big 3" search engines can be contacted to request the removal of said data and how the data was originally accessible by the public.

If you could let me know if the issue has indeed been resolved, that would be great.

Kind regards,

Rob
```

The report contained quite a verbose explanation of the problem, what coding practises cause it, as well as reading material for the developers to aid in resolving the issue and to help with the data clean-up.

A Resolution
============
More time passed, and there was still no response, but after another request for an update, I received a response 12 days later:

```html
Dear Rob,

Thank you for your email.

The report you provided has been passed on to our IT department for further investigation.

Should you have any further queries, or if we can be of any further assistance, please do not hesitate to contact us.

Kind regards,

*redacted*
```

Following this update, I reviewed the dorks I had included in my report, and verified that the organisation had seemingly followed the instructions I provided to have the PII that had been cached removed from all search engines that seemed to contain copies of it. After 36 days, the issue was fully resolved!

![](/images/disclosure-woes-dealing-with-non-techies/yay.gif)

TLDR / The Things I Did Wrong
=============================
Although I have touched upon the things I did wrong in this rather verbose post, below is a bulleted list of summarised points that could have been handled better:

* I set out on my own as an individual to contact a government backed organisation that deal with a lot of sensitive information. This was a risky move, given the opportunity for misinterpretation. I should have first contacted an organisation akin to [HackerOne](https://www.hackerone.com/) to act on my behalf.
* I left it until 2-3 weeks before consulting more experienced researchers for advice. If you're unsure, find trustworthy and experienced people, and ask them what they'd do in your shoes.
* I gave information of the vulnerability to a person of lower trust within the company. Although this was unlikely to have an impact in this instance, it was a bad practice, and one I shall not be repeating again any time soon.
* I didn't point out the sensitivity of the situation in a way which would mean something to the recipients of my e-mails. A better approach would probably to have explained the legal implications the company was exposing itself to, should they not address the issues at hand; as although an organisation may not care about the privacy of their users, they all certainly care about their profit margins.
