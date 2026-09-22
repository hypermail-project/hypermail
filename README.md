    Hypermail 3.0.x
    ![hypermail logo](./docs//hypermail.png)

## Introduction

Hypermail is a free, portable program that takes a file of mail messages in
UNIX mailbox format and generates an archive of cross-referenced HTML
documents.

You can use hypermail to process existing mailboxes. You can also use it to
convert messages that arrive on-the-fly andq add them to an existing hypermail
message archive.

An hypermail archive is made of **indexes** and **messages**. Indexes sort the
messages in the archive by author, subject, date, and thread. Messages show a
configurable summary of a message's headers and the message's attachments, such
as images. A message will also include links to other messages, such as next
and previous received message or next and previous message in a thread.

## Features

* HTML5 markup reviewed for accessibility
* User customizable headers, footers
* Use of external CSS to customize look and feel
* Possiblity of adding javascript for further customization
* Generated archives can be browsed online (thru a web browser) or offline
  (thru your file system)

You can see an example of hypermail in action in any of W3C's public mailing
list archives, for example,
[here](https://lists.w3.org/Archives/Public/public-aria/2015Dec/).

## Project Status

Hypermail is still active and under development.

The main repository of hypermail is on github:

	https://github.com/hypermail-project/hypermail

We follow a trunk-based development model.

The main branch has the on-going cutting edge development; if you want a stable
release, please use the releases. In addition to the packaged releases, Each
stable release is tagged.

Please refer to [Release Notes](./RELEASE_NOTES) and the [Changelog](./Changelog) for
the list of recent changes.

There is no active dedicated mailing list for hypermail support or
development.

Please use hypermail's github repository issue tracker for all bug reports,
feature requests, patches, and other program-related things. This project is
under low-maintenance priority.
	
If you detect an issue with hypermail, please open a new issue given your
operating system version, hypermail version, and the steps allowing to
reproduce it. If the issue occurs when processing a specific message, please
include an anonymzed copy message that allows to reproduce the issue in your
report.

If you want to open a merge request that solves an issue, please first test
your issue against the main branch. If you still have the issue, fork the
repository, create a branch against main and then do a PR.

If you want to contribute a new feature, please open first a discussion item.

## Installing

Hypermail is written in C. 

Detailed instructions on dependencies, how to compile and install it are given
in the [INSTALL](./INSTALL) file.  For a description of how to use it, see the
[hypermail.html](./docs/hypermail.html), [hmrc.html](./docs/hmrc.html), and
[hypermail-faq.html](./docs/hypermail-faq.html) files that come in the [docs/](./docs)
directory. Take the time to read the [KNOWN_BUGS](./KNOWN_BUGS) file so that you
are aware of things that might affect your use of hypermail.

## Try it out

You can try out hypermail after compiling it, without needing to install it.
In the next instructions we're going to use hypermail to generate an archive
from the ``y2k.mbox`` sample mbox and to park the archive in the
``/tmp/hypermail-test`` directory (feel free to try it out with any mbox file
you have).

```
cd src/hypermail
./hypermail -m ../tests/mboxes/y2k.mbox -d /tmp/hypermail-test
```

You can browse the archive by opening one of the generated indexes, e.g., 
```
firefox /tmp/hypermail-test/index.html
```

## Repository organization

   * [archive](./archive) - general archive utilites that are useful in managing
               list archives. Much of the functionality here has been
               rendered obsolete by the folder_by_date option.
   * [contrib](./contrib) - contributed hypermail related utilities
   * [configs](./configs) - sample hypermail configuration files
   * [docs](./docs)    - documentation and documentation support files
   * [src](./src)     - here's the beef
   * [tests](./tests)   - directory for supporting local testing

Please note that other than src and docs, the content in some of this
directories is outdated.

## SECURITY WARNING

If you make a hypermail mailing list archive available behind a web server,
please take into account that messages can be crafted to bypass security
measures you have in place.

HTML attachment can be used to use server side includes (SSI) directives,
include javascript scripts to do XSS or other kinds of attacks. Attachments can
be PHP or other scripts that could be interpreted by your web server.

It is important to disable all those web server features in the directory where
an hypermail archive will be stored. You can also customize hypermail to only
allow some specific attachments. You can also associate a hypermail directory
with a CSP policy to disable or restrict javascript use from within an HTML
document.

We advise you to follow the the least-privilege principle: restrict / disable
features available to a hypermail archive and enable them one by one as
required, after evaluating they are safe.

Please read the (INSTALL)[./INSTALL] file for more info.

## Icon

You'll find the image [hypermail.png](./docs/hypermail.png) included with the
source; this icon is for your use in your Hypermail-related pages and links to
them. If you are talented with graphics and would like to donate new icons and
images to the hypermail effort, please feel free.
