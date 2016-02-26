# Hypermail

Version `2.3.x`

* [Hypermail background](#hypermail-background)
  * [License evolution](#license-evolution)
  * [EIT's net.disappearance](#eits-netdisappearance)
  * [Kevin and ongoing Hypermail development](#kevin-and-ongoing-hypermail-development)
* [General](#general)
* [Warning](#warning-warning)
* [Git repository](#git-repository)
* [CVS server](#cvs-server)
* [Getting help](#getting-help)
* [Additionally](#additionally)

This is a release of the `2.3.x` version of hypermail.

Hypermail is a program that takes a file of mail messages in UNIX mailbox
format and generates a set of cross-referenced HTML documents.

:warning: **Security warning:**
Do not put hypermail's output anyplace where a web server might have
server side includes (SSI) enabled unless you are sure you know what
you are doing.

:warning: **Warning:**
There once existed a program called `mail` that came with hypermail.
`mail` utility has not installed by default for the last two years.
This program has been disabled because it was probably easy for spammers
to use as an open relay. It also had problems with enabling malicious
use of JavaScript and CRLF Injection. The `mail` utility is a historic
reclic and will not be supplied in future versions. Its functionality
has been replaced with a warning that anyone using it should remove it
immediately.

See the [`INSTALL`](INSTALL) file to get started. For a description of how to use it,
see the [`hypermail.html`](docs/hypermail.html), [`hmrc.html`](docs/hmrc.html), and [`hypermail-faq.html`](docs/hypermail-faq.html) files that come
in the [`docs/`](docs/) directory.
Please refer to [`Changelog`](Changelog) for the list of recent changes.

Hypermail is distributed under the GNU GPL license (see the file [`COPYING`](COPYING) for
details). Some programs that are distributed with it in the archive and
contrib directories have different licenses - check the individual files for
details.

## Hypermail background

Hypermail was originally designed and developed by Tom Gruber for
Enterprise Integration Technologies (EIT) using Common Lisp.

It was later rewritten in C by Kevin Hughes in 1994 while Kevin
worked at EIT.

### License evolution

In the spring of 1997, Kevin Hughes heavily pressed Hewlett-Packard
(who was now the legal owner of Hypermail, since EIT was bought by
VeriFone, which was bought by Hewlett-Packard) into placing a free
software license onto his old EIT software. They officially put the
GNU GPL license on *all* of Kevin's old EIT software, opening it up to
more open methods of development and distribution. So now Hypermail
is under GPL.

### EIT's net.disappearance
A very old and established government contractor company called
Electronic Instrumentation and Technology Inc. made legal moves to
obtain the eit.com domain. Since VeriFone/HP had no interest in keeping
EIT, dissolved it completely. As this company had a trademark on EIT,
the domain name was given to them. Elizabeth Batson of EIT/VeriFone/HP
informed Kevin he could maintain all his old software himself wherever
he wished to put it.

### Kevin and ongoing Hypermail development

Kevin left Hewlett-Packard in 1997 and helped form a new company
called [Veo Systems](http://www.veosystems.com/) with his old boss Marty
Tenenbaum, who founded EIT. Kevin did not have the time to maintain
any of his old software, so he was looking for different parties to
help maintain it and take over different pieces. For instance, Getstats
has been obsoleted by [Analog](http://www.statslab.cam.ac.uk/~sret1/analog/).
SWISH has been taken over by the [SWISH-E project](http://sunsite.berkeley.edu/SWISH-E/)
and many people have been doing
good things with it. April 17, 1998 Kevin passed hypermail development
to [Kent Landfield](http://www.hypermail.org/). Kent had been
supporting an enhanced version of hypermail he had been using for the
last few years. It has gradually become a group effort, with [Peter McCluskey](mailto:pcm@rahul.net)
often acting as the leader during its heydays.
Since a couple of years, it is stable and mostly unmaintained. W3C continues
to contribute to this repository, although infrequently, sharing with the
community enhancements and bug fixes originating from its use of
hypermail.

## General

This version has been tested on the platforms listed below. If you would
like to send portability patches or confirmation that it works on a certain
platform, please do. There should not be toooo many changes that need to be
made.

This version of hypermail has substantial support for attachments and for
splitting archives into subdirectories. It also allows an administrator to
customize the header and footers to match their local needs. This allows
you to have hypermail facilities better integrated into your web site.

This version is an integration of patches that Kevin had received through
the years, and new features through the individual efforts many people.
This has been run through lint, Insight and Purify and has been cleaned up
accordingly.

* `archive`: general archive utilites that are useful in managing
  list archives. Much of the functionality here has been
  rendered obsolete by the `folder_by_date` option.
* `contrib`: contributed hypermail relate utilities
* `configs`: sample hypermail configuration files,
* `docs`:    documentation and documentation support files,
* `libcgi`:  support library for the mail utility,
* `src`:     here's the beef,
* `tests`:   directory for supporting local testing,

## :warning: Warning

Take the time to read the [`KNOWN_BUGS`](KNOWN_BUGS) file so that you are aware of
things that might affect your use of hypermail.

## GIT repository

Hypermail's code base has migrated from CVS to git and after a
residence at sourceforge, now resides at GitHub:

[`https://github.com/hypermail-project/hypermail/`](https://github.com/hypermail-project/hypermail/)

## CVS server

The hypermail CVS repository has been retired. Hypermail's code base
now resides at github (see above).

Thanks to Ashley [M. Kirchner](mailto:ashley@pcraft.com) for having
hosted and keep alive the CVS repository during the years.

## Getting help

There is no active dedicated mailing list for hypermail support or
development.

Please use hypermail's github repository issue tracker for all bug
reports, feature requests, patches, and other program-related
things. This project is under low-maintenance priority.

You can browse the old hypermail development mailing list archives
from the historical hypermail project homepage:

[`http://hypermail-project.org/`](http://hypermail-project.org/)

## Additionally

You'll find the image `hypermail.gif` included with the source;
this icon is for your use in your Hypermail-related pages and links
to them. If you are talented with graphics and would like to donate
new icons and images to the hypermail effort, please feel free.

Regular expression support is provided by the PCRE library package,
which is open source software, written by Philip Hazel, and copyright
by the University of Cambridge, England. See [`http://www.pcre.org/`](http://www.pcre.org/).
