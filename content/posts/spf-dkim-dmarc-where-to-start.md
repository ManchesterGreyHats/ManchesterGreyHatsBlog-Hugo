---
title: "SPF / DKIM / DMARC - Where to start"
date: 2018-01-20T20:16:23Z
draft: true
categories: ["Articles"]
tags: ["blog","how to", "mail servers", "DNS", "SPF", "DKIM", "DMARC"]
---

Written by [Tim](authors/timwilkes)

# SPF / DKIM / DMARC - Where to start

The alphabet soup from the title of this blog post are the DNS records you should have in place to help keep your domain from being abused by a spammer. Or worse, someone spreading malware. If you already have them in place, congratulations. Why are you reading this article? Move on to DNSSEC. Still here? Good.

Since it's creation, email has never had security in mind. The whole system really needs an overhaul. In the meantime, this will at least help protect your brand from being misused.

# What do these terms mean?

SPF - Sender Policy Framework
This is basically a list of valid senders for the domain. Setting this up has the added benefit of speeding up mail delivery when greylisting is in place. 

DKIM - DomainKeys Identified Mail
This is a signature to prove the message came from a valid server.

DMARC - Domainbased Message Authentication, Reporting and Conformance
This is used to tie SPF and DKIM records together in a policy and report on it.

For more information about these records, and how to set up each one individually, I recommend Scott Helme's blog posts on [SPF](https://scotthelme.co.uk/email-security-spf/), [DKIM](https://scotthelme.co.uk/email-security-dkim/) and [DMARC](https://scotthelme.co.uk/email-security-dmarc/). 

# What order should I put these in to DNS?

Well, this depends. What is your focus? Do you want to be secure quickly, or do you want to make sure you don't block your email? If you are talking to a manager, the answer is usually "both".

If you want to be secure quickly, follow the standard advice of SPF, DKIM then DMARC. This approach does have it's drawbacks. Mainly, that as soon as you deploy SPF, valid mail can be blocked if you have missed a sender.

# So, if I don't want to have any of my email blocked, what's the best way to actually deploy this?

To start off, deploy DMARC. DMARC can be deployed in a reporting-only mode by setting the policy to none (p=none). Once set up, you will start to recieve email about your messages. They should, at this stage, say fail a lot. Don't worry, this is because we haven't set up any policies yet. Also, don't set up forensics reports, you want aggregate reports at this stage. This means that you won't get an email for every sent email. This may be what you want for debugging an issu, but not for a general setup.

Next, deploy DKIM on all known mail servers. Use a selector which will help you identify the service. The mailservers which are setup with DKIM will start showing a pass in the DMARC reports. The reports which show fail should then be looked at to see if the mail is legitimate or not. If it is, you have missed a mail server. Find the server, and set up DKIM.

Once DKIM is deployed, you now have a list of servers which are sending email for you. You can now use this list to create your SPF records. When creating your SPF records, avoid weak records (ie, ones which do not end with -all).

Lastly, when you are happy that all your email is being marked correctly with SPF and DKIM, change the DKIM policy from "None" to "reject" or "quarentine". Congratulations, you now have deployed all the DNS records and the domain should have better protection. This isn't foolproof, because not everyone is looking for these records, acting upon them. Also, things change, so keep checking the DMARC reports regularly.


