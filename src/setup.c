/*
** $Id: setup.c,v 1.23 2013-06-11 18:55:44 kahan Exp $
*/

#include "hypermail.h"

#ifdef __LCC__
#include "../lcc/defaults.h"
#else
#include "defaults.h"
#endif

#include "setup.h"
#include "struct.h"
#include "print.h"

char *set_fragment_prefix;
char *set_antispam_at;
char *set_htmlmessage_edited;
char *set_htmlmessage_deleted_other;
char *set_htmlmessage_deleted_spam;
char *set_language;
char *set_htmlsuffix;
char *set_mbox;
char *set_archives;
char *set_custom_archives;
char *set_about;
char *set_dir;
char *set_defaultindex;
char *set_default_top_index;
char *set_txtsuffix;
char *set_antispamdomain;

bool set_email_address_obfuscation;
bool set_i18n;
bool set_i18n_body;
bool set_overwrite;
bool set_inlinehtml;
bool set_increment;
bool set_readone;
bool set_reverse;
bool set_reverse_folders;
bool set_showprogress;
bool set_showheaders;
bool set_showbr;
bool set_showhr;
bool set_showreplies;
int set_show_msg_links;
int set_show_index_links;
bool set_usetable;
bool set_indextable;
bool set_iquotes;
bool set_eurodate;
bool set_gmtime;
bool set_isodate;
bool set_require_msgids;
bool set_discard_dup_msgids;
bool set_usemeta;
bool set_userobotmeta;
bool set_uselock;
bool set_ietf_mbox;
bool set_linkquotes;
bool set_monthly_index;
bool set_yearly_index;
bool set_spamprotect;
bool set_spamprotect_id;
bool set_attachmentsindex;
bool set_usegdbm;
bool set_writehaof;
bool set_append;
char *set_append_filename;
bool set_nonsequential;
bool set_warn_surpressions;
bool set_files_by_thread;
bool set_href_detection;
bool set_mbox_shortened;
bool set_report_new_file;
bool set_report_new_folder;
bool set_use_sender_date;
bool set_inline_addlink;

int set_showhtml;
int set_thrdlevels;
int set_dirmode;
int set_filemode;

int set_locktime;

int set_searchbackmsgnum;
int set_quote_hide_threshold;
int set_thread_file_depth;

int set_startmsgnum;

int set_save_alts;
char *set_alts_text;

char *set_mailcommand;
char *set_newmsg_command;
char *set_replymsg_command;
char *set_inreplyto_command;
char *set_mailto;
char *set_hmail;
char *set_domainaddr;
static char *set_htmlbody;

char *set_icss_url;
char *set_mcss_url;

char *set_label;

char *set_dateformat;
char *set_indexdateformat;
char *set_stripsubject;

char *set_link_to_replies;
char *set_quote_link_string;

struct hmlist *set_text_types = NULL;
struct hmlist *set_inline_types = NULL;
struct hmlist *set_prefered_types = NULL;
struct hmlist *set_ignore_types = NULL;
struct hmlist *set_show_headers = NULL;
struct hmlist *set_skip_headers = NULL;
struct hmlist *set_avoid_indices = NULL;
struct hmlist *set_avoid_top_indices = NULL;
struct hmlist *set_filter_out = NULL;
struct hmlist *set_filter_require = NULL;
struct hmlist *set_filter_out_full_body = NULL;
struct hmlist *set_filter_require_full_body = NULL;

bool set_format_flowed;
bool set_format_flowed_disable_quoted;

char *set_ihtmlheader;
char *set_ihtmlfooter;
char *set_ihtmlhead;
char *set_ihtmlhelpup;
char *set_ihtmlhelplow;
char *set_ihtmlnavbar2up;
char *set_mhtmlheader;
char *set_mhtmlfooter;
char *set_attachmentlink;
char *set_unsafe_chars;
char *set_filename_base;

char *set_folder_by_date;
char *set_latest_folder;
char *set_base_url;
char *set_describe_folder;
int set_msgsperfolder;

bool set_iso2022jp;

bool set_noindex_onindexes;
struct hmlist *set_annotated = NULL;
struct hmlist *set_deleted = NULL;
struct hmlist *set_expires = NULL;
struct hmlist *set_delete_msgnum = NULL;
char *set_delete_older;
char *set_delete_newer;
bool set_delete_incremental;
int set_delete_level;

struct Config cfg[] = {
    {"fragment_prefix", &set_fragment_prefix, "msg", CFG_STRING,
     "# put this string before the message number in each URI fragment.\n", FALSE},

    {"email_address_obfuscation", &set_email_address_obfuscation, BFALSE, CFG_SWITCH,
     "# set to On to enable email address obfuscation using numeric character references.\n",FALSE},

    {"i18n", &set_i18n, BTRUE, CFG_SWITCH,
     "# Enable I18N features, hypermail must be linked with libiconv.\n",FALSE},

    {"i18n_body", &set_i18n_body, BFALSE, CFG_SWITCH,
     "# Translate message body into UTF-8. \"i18n\" must be enabled.\n",FALSE},

    {"htmlmessage_edited",  &set_htmlmessage_edited, NULL, CFG_STRING,
     "# Set this to HTML markup you want to appear in the body of manually\n"
     "edited messages.\n",FALSE},

    {"htmlmessage_deleted_other",  &set_htmlmessage_deleted_other, NULL, CFG_STRING,
     "# Set this to HTML markup you want to appear in the body of deleted\n"
     "# messages (by reasons other than spam).\n",FALSE},

    {"htmlmessage_deleted_spam",  &set_htmlmessage_deleted_spam, NULL, CFG_STRING,
     "# Set this to HTML markup you want to appear in the body of deleted\n"
     "# messages (by spam reasons).\n",FALSE},

    {"antispam_at", &set_antispam_at, ANTISPAM_AT, CFG_STRING,
     "# replace any @ sign with this string, if spam flags enabled.\n", FALSE},
     
    {"language", &set_language, LANGUAGE, CFG_STRING,
     "# A two-letter string specifying the language to use!\n"
     "# For example 'en' for English.\n", FALSE},

    {"htmlsuffix", &set_htmlsuffix, HTMLSUFFIX, CFG_STRING,
     "# Use this to specify the html file suffix to be used\n"
     "# when Hypermail generates the html files. This is depen-\n"
     "# dent on local needs. Do not put a '.' in the value. It\n"
     "# would result in \"file..html\", probably not what you want.\n", FALSE},

    {"mbox", &set_mbox, NULL, CFG_STRING,
     "# This is the mailbox to read messages in from. Set this with \n"
     "# a value of NONE to read from standard input.\n", FALSE},

     {"ietf_mbox",  &set_ietf_mbox, BFALSE, CFG_SWITCH,
     "# Set this to On to read mboxes using the IETF convention.\n", FALSE},

    {"archives", &set_archives, NULL, CFG_STRING,
     "# This will create a link in the archived index pages\n"
     "# labeled 'Other mail archives' to the specified URL. Set\n"
     "# this to NONE to omit such a link.\n", FALSE},

    {"custom_archives", &set_custom_archives, NULL, CFG_STRING,
     "# If this variable is defined, a navigation entry will be\n"
     "# created below the sorted_by_x list entry, with the text\n"
     "# Other mail archives: followed by the value of this variable.\n"
     "# Set it to NONE to ommit such an entry.\n", FALSE},

    {"about", &set_about, NULL, CFG_STRING,
     "# This will create a link in the archived index pages\n"
     "# labeled 'About this archive' to the specified URL. Set\n"
     "# this to NONE to omit such a link.\n", FALSE},

    {"label", &set_label, NULL, CFG_STRING,
     "# Define this as the label to put in archives.\n", FALSE},

    {"dir", &set_dir, NULL, CFG_STRING,
     "# This is the directory that Hypermail will look for when\n"
     "# creating  and updating archives. If defined as NONE the\n"
     "# directory will have the same name as the input mailbox.\n", FALSE},

    {"defaultindex", &set_defaultindex, DEFAULTINDEX, CFG_STRING,
     "# This specifies the default index that  users can view when\n"
     "# entering the archive. Valid types are date, thread, author,\n"
     "# subject, and attachment. When using the folder_by_date or\n"
     "# msgsperfolder options, this option applies to subdirectories.\n", FALSE},

    {"default_top_index", &set_default_top_index, "folders", CFG_STRING,
     "# This specifies the default index that  users can view when\n"
     "# entering the top level of an archive that uses the folder_by_date\n"
     "# or msgsperfolder option. Valid types are date, thread, author,\n"
     "# subject, attachment, and folders.\n", FALSE},

    {"avoid_indices", &set_avoid_indices, NULL, CFG_LIST,
     "# This is a list of index files to not generate. Valid types are\n"
     "# date, thread, author, and subject. When using the folder_by_date or\n"
     "# msgsperfolder options, this option applies to subdirectories.\n", FALSE},

    {"avoid_top_indices", &set_avoid_top_indices, NULL, CFG_LIST,
     "# This is a list of index files to not generate for the top\n"
     "# directory of an archive using the folder_by_date or\n"
     "# msgsperfolder option. Valid types are date, thread, author, \n"
     "# subject, folders, and attachment.\n", FALSE},

    {"overwrite", &set_overwrite, BFALSE, CFG_SWITCH,
     "# Set this to On to make Hypermail overwrite existing archives.\n", FALSE},

    {"inlinehtml", &set_inlinehtml, BTRUE, CFG_SWITCH,
     "# Define to On to make text/html parts to get inlined with the mails.\n"
     "# If set to Off, HTML-parts will be stored as separate files.\n"
     "# A \"Content-Disposition: attachment;\" line in the mail will\n"
     "# cause an HTML-part to be stored as a separate file even if this\n"
     "# option is On.\n", FALSE},

    {"increment", &set_increment, BFALSE, CFG_INTEGER,
     "# Set this to -1 to have hypermail figure out whether the input\n"
     "# is entirely new messages to be appended or whether it contains\n"
     "# messages that are already in the archive. A value of -1 cannot be\n"
     "# used with the mbox_shortened option or with the -i command line\n"
     "# option or with mbox = NONE. Set this to 0 to always treat\n"
     "# the input as an mbox that contains messages in the archive if the\n"
     "# existing archive is not empty.\n"
     "# Set this to 1 to append the input to existing archive.\n", FALSE},

    {"readone", &set_readone, BFALSE, CFG_SWITCH,
     "# Set this to On to specify there is only one message in the input.\n", FALSE},

    {"reverse", &set_reverse, BFALSE, CFG_SWITCH,
     "# Setting this variable to On will reverse-sort the article\n"
     "# entries in the date and thread index files by the\n"
     "# date they were received. That is, the most recent messages\n"
     "# will appear at the top of the index rather than the other\n"
     "# way around.\n", FALSE},

    {"reverse_folders", &set_reverse_folders, BFALSE, CFG_SWITCH,
     "# Setting this variable to On will reverse-sort the list of\n"
     "# folders. That is, the most recent folders will appear at\n"
     "# the top of the index rather than the other way around.\n", FALSE},

    {"progress", &set_showprogress, INT(PROGRESS), CFG_INTEGER,
     "# Set this to 1 or 2 to always show a progress report  as\n"
     "# Hypermail works. With a setting of 1, hypermail\n"
     "# overwrites the progress information relating to attachment\n"
     "# creation. With a setting of 2, attachment creation information\n"
     "# is listed individually with the number of the message the\n"
     "# attachments relate to. This is written to stdout.\n", FALSE},

    {"showheaders", &set_showheaders, BFALSE, CFG_SWITCH,
     "# Set this to On to show the article header lines in the archived HTML\n"
     "# files.  These lines typically include the To:, From:, and Subject:\n"
     "# information found in most email messages.\n", FALSE},

    {"showhtml", &set_showhtml, INT(1), CFG_INTEGER,
     "# Set this to 1 to show the articles in a proportionally-spaced\n"
     "# font rather than a fixed-width (monospace) font.\n"
     "# Set this to 2 for more complex conversion to html\n"
     "# similar to that in <a href=\"http://www.cs.wustl.edu/~seth/txt2html/\">txt2html.pl</a>.\n"
     "# Showhtml = 2 will normally produce nicer looking results than\n"
     "# showhtml = 1, and showhtml = 0 will look pretty dull, but\n"
     "# 1 and 2 run risks of altering the appearance in undesired ways.\n", FALSE},

    {"showbr", &set_showbr, BTRUE, CFG_SWITCH,
     "# Set this to On to place <br> tags at the end of article lines.\n"
     "# Otherwise, all non-quoted article lines will word wrap. This\n"
     "# only takes effect if hm_showhtml is 1.\n", FALSE},

    {"showhr", &set_showhr, BFALSE, CFG_SWITCH,
     "# Set this to On to place horizontal rules before and after articles.\n"
     "NOTE: THIS OPTION HAS BEEN DEPRECATED BY THE WAI CHANGES. IT WILL BE\n"
     "IGNORED.\n", FALSE},

    {"showreplies", &set_showreplies, BTRUE, CFG_SWITCH,
     "# Set this to On to show all replies to a message as links\n"
     "# in article files.\n", FALSE},

    {"show_msg_links", &set_show_msg_links, INT(1), CFG_INTEGER,
     "# Set this to 1 to put the individual message links at the top\n"
     "# of the individual message pages. Set this to 0 to produce pages\n"
     "# without the Next, Previous, Reply, In-Reply-To, etc. links. Set\n"
     "# it to 3 to produce those links only at the top of the message\n"
     "# pages, or 4 to produce those links only at the bottom of the\n"
     "# message.\n", FALSE},

    {"show_index_links", &set_show_index_links, INT(1), CFG_INTEGER,
     "# Set this to 1 to show links to index pages from the top and\n"
     "# bottom of each message file. Set it to 0 to avoid those links.\n"
     "# Set it to 3 to show the links only at the top of the message\n"
     "# pages, or 4 to produce those links only at the bottom of the\n"
     "# message.\n", FALSE},

    {"usetable", &set_usetable, BFALSE, CFG_SWITCH,
     "# Setting this variable to On will tell Hypermail to generate\n"
     "# an index menu at the top and bottom of each page in a table\n"
     "# format. Set to Off if you want the standard Hypermail page look\n"
     "# and feel\n"
     "NOTE: THIS OPTION HAS BEEN DEPRECATED BY THE WAI CHANGES. IT WILL BE\n"
     "IGNORED.\n", FALSE},

    {"indextable", &set_indextable, BFALSE, CFG_SWITCH,
     "# Setting this variable to On will tell Hypermail to generate\n"
     "# message index Subject/Author/Date listings using a nice table\n"
     "# format. Set to Off if you want the original Hypermail index look.\n", FALSE},

    {"iquotes", &set_iquotes, BTRUE, CFG_SWITCH,
     "# Set this to On to italicize quoted lines.\n", FALSE},

    {"eurodate", &set_eurodate, BFALSE, CFG_SWITCH,
     "# Set this to On to display article received dates with\n"
     "# days before months instead of months before days.\n", FALSE},

    {"isodate", &set_isodate, BFALSE, CFG_SWITCH,
     "# Set this to On to display article received dates in\n"
     "# YYYY-MM-DD HH:MM:SS format. If used with the gmtime option, a\n"
     "# Z will be inserted between the DD and HH.\n", FALSE},

    {"gmtime", &set_gmtime, BFALSE, CFG_SWITCH,
     "# Set this to On to display article received dates using\n"
     "# Greenwich Mean Time (UTC) rather than local time.\n", FALSE},

    {"discard_dup_msgids", &set_discard_dup_msgids, BTRUE, CFG_SWITCH,
     "# Set this to Off to accept messages with a Message-ID matching\n"
     "# that of a message already  in this archive.\n"
     "# By default such messages are discarded.\n", FALSE},

    {"require_msgids", &set_require_msgids, BTRUE, CFG_SWITCH,
     "# Set this to Off to accept messages without a Message-ID header.\n"
     "# By default such messages are discarded.\n", FALSE},

    {"usemeta", &set_usemeta, BFALSE, CFG_SWITCH,
     "# Set this to On to store the content type of a MIME attachment in\n"
     "# a metadata file.\n", FALSE},

    {"userobotmeta", &set_userobotmeta, BFALSE, CFG_SWITCH,
     "# Set this to On to apply a robot annotation to a MIME attachment in\n"
     "# a metadata file, using the experimental X-Robots-Tag HTTP header.\n", FALSE},

    {"uselock", &set_uselock, BTRUE, CFG_SWITCH,
     "# Set this to On to use hypermail's internal locking mechanism.\n", FALSE},

    {"usegdbm",  &set_usegdbm,  BFALSE,    CFG_SWITCH,
     "# Set this to On to use gdbm to implement a header cache.\n"
     "# This will speed up hypermail, especially if your filesystem is slow.\n"
     "# It will not provide any speedup with the linkquotes option.\n"
#ifndef GDBM
     "# (This particular binary has been build with GDBM disabled.)\n"
#endif
    , FALSE},

    {"writehaof", &set_writehaof, BFALSE, CFG_SWITCH,
     "# Set this to On to let hypermail write an XML archive overview file\n"
     "# in each directory. The filename is " HAOF_NAME ".\n", FALSE},

    {"append",  &set_append,  BFALSE,    CFG_SWITCH,
     "# Set this to On to maintain a parallel mbox archive. The file\n"
     "# name defaults to mbox in the directory specified by -d or dir.\n", FALSE},

    {"append_filename", &set_append_filename, NULL, CFG_STRING,
     "# Specifies the filename to be used by the append option.\n"
     "# $DIR may be used to specify a name relative to the directory\n"
     "# specified in the -d or dir option.\n", FALSE},

    {"nonsequential",  &set_nonsequential,  BFALSE,    CFG_SWITCH,
     "# Set this to On to generate filenames that are not sequential, but\n"
     "# rather a hash of the message properties.\n"
#ifndef HAVE_LIBFNV
     "# (This particular binary has been build with LIBFNV disabled.\n"
     "# This option won't work)\n"
#endif
     , FALSE},

    {"thrdlevels", &set_thrdlevels, INT(4), CFG_INTEGER,
     "# This specifies the number of thread levels to outline\n"
     "# in the thread index.\n", FALSE},

    {"dirmode", &set_dirmode, INT(0755), CFG_OCTAL,
     "# This is an octal number  representing  the  permissions\n"
     "# that new directories are set to when they are created.\n", FALSE},

    {"filemode", &set_filemode, INT(0644), CFG_OCTAL,
     "# This is an octal number representing the file permissions\n"
     "# that new files are set to when they are created.\n", FALSE},

    {"mailcommand", &set_mailcommand, MAILCOMMAND, CFG_STRING,
     "# This specifies the mail command to use when converting\n"
     "# email addresses to links. The variables $TO, $SUBJECT,\n"
     "# and $ID can be used in constructing the command string.\n", FALSE},

    {"mailto", &set_mailto, NULL, CFG_STRING,
     "# The address of the contact point that is put in the HTML header\n"
     "# line <link rev=made href=\"mailto:MAILTO\">. Setting this to NONE\n"
     "# disables <link...> header generation.\n", FALSE},

    {"hmail", &set_hmail, NULL, CFG_STRING,
     "# Set this to the list's submission address.\n", FALSE},

    {"newmsg_command", &set_newmsg_command, "mailto:$TO", CFG_STRING,
     "# This specifies the mail command to use when converting the\n"
     "# set_hmail address to links in replies. The variables $TO, $SUBJECT,\n"
     "# and $ID can be used in constructing the command string.\n", FALSE},

    {"replymsg_command", &set_replymsg_command, "not set", CFG_STRING,
     "# This specifies the mail command to use when converting the\n"
     "# set_hmail address to links in replies. The variables $TO, $SUBJECT,\n"
     "# and $ID can be used in constructing the command string. The value\n"
     "# from mailcommand will be used if this option is not specified.\n", FALSE},

    {"inreplyto_command", &set_inreplyto_command, NULL, CFG_STRING,
     "# This gives a URI template to a script that hypermail will link to\n"
     "# if it's unable to find in the archive's messages the MID corresponding\n"
     "# to an In-Reply-To header. The variable $ID is used to specify where the\n"
     "# Message-Identifier value will appear in the link.\n", FALSE},

    {"domainaddr", &set_domainaddr, DOMAINADDR, CFG_STRING,
     "# Set this to the domainname you want added to a mail address\n"
     "# appearing in the RFC822 field which lack a hostname.\n", FALSE},

    {"body", &set_htmlbody, NULL, CFG_STRING,
     "# This obsolete entry kept around to help warn users with old config files.\n", FALSE},

    {"icss_url", &set_icss_url, NULL, CFG_STRING,
     "# Specifies a URL to an external CSS stylesheet for the index pages.\n"
     "# The CSS will be associated to the indexes thru an HTML LINK element.\n"
    , FALSE},

    {"mcss_url", &set_mcss_url, NULL, CFG_STRING,
     "# Specifies a URL to an external CSS stylesheet for the message pages.\n"
     "# The CSS will be associated to the indexes thru an HTML LINK element.\n"
    , FALSE},

    {"text_types", &set_text_types, NULL, CFG_LIST,
     "# This is a list of MIME types that you want hypermail to treat\n"
     "# exactly as if they were text/plain.\n", FALSE},

    {"inline_types", &set_inline_types, INLINE_TYPES, CFG_LIST,
     "# This is the list of MIME types that you want <img> tags to as\n"
     "# opposed to simply <a href> to from the message.\n", FALSE},

    {"inline_addlink", &set_inline_addlink, BTRUE, CFG_SWITCH,
     "# Set to On to add inline links to content that is stored in the\n"
     "# attachments subdirectory.  'inline_types' must be enabled.\n", FALSE},

    {"prefered_types", &set_prefered_types, NULL, CFG_LIST,
     "# When mails using multipart/mixed types are scanned, this list of\n"
     "# MIME types defines which part you want presented in the result.\n"
     "# See the save_alts option for how non prefered types are treated.\n", FALSE},

    {"ignore_types", &set_ignore_types, NULL, CFG_LIST,
     "# This is the list of MIME attachment types that you do not want\n"
     "# to do anything with. Two special types may be used here:\n"
     "# $BINARY - ignore all types that would be stored as separate files.\n"
     "# $NONPLAIN - ignore all types not treated as text/plain, and all $BINARY types.\n"
     "# Note: the behavior of these may be affected by the inlinehtml option.\n", FALSE},

    {"show_headers", &set_show_headers, NULL, CFG_LIST,
     "# This is the list of headers to be displayed if 'showheaders'\n"
     "# is set to On). They can be listed comma or space separated\n"
     "# all on a single line.\n", FALSE},

    {"format_flowed", &set_format_flowed, BFALSE, CFG_SWITCH,
     "# Enable support for RFC3676 format=flowed (EXPERIMENTAL)\n", FALSE},

    {"format_flowed_disable_quoted", &set_format_flowed_disable_quoted, BFALSE, CFG_SWITCH,
     "# If format_flowed is enabled, this option allows you to disable\n"
     "# format=flowed inside quoted text\n", FALSE},

    {"ihtmlheaderfile", &set_ihtmlheader, NULL, CFG_STRING,
     "# Define path as the path to a template  file  containing\n"
     "# valid  HTML  formatting  statements  that  you  wish to\n"
     "# included at the top of every index page.\n", FALSE},

    {"ihtmlfooterfile", &set_ihtmlfooter, NULL, CFG_STRING,
     "# Define path as the path to a template  file  containing\n"
     "# valid  HTML  formatting  statements  that  you  wish to\n"
     "# included at the bottom of every index  page.\n", FALSE},

    {"ihtmlheadfile", &set_ihtmlhead, NULL, CFG_STRING,
     "# Define path as the path to a template  file  containing\n"
     "# valid  HTML  formatting  statements  that  you  wish to\n"
     "# included inside the HTML HEAD element of every index page.\n", FALSE},

    {"ihtmlhelpupfile", &set_ihtmlhelpup, NULL, CFG_STRING,
     "# Define path as the path to a template  file  containing\n"
     "# valid  HTML  formatting  statements  that  you  wish to\n"
     "# included as information giving help to your archive users,"
     "# in the upper navigation bar.\n", FALSE},

    {"ihtmlhelplowfile", &set_ihtmlhelplow, NULL, CFG_STRING,
     "# Define path as the path to a template  file  containing\n"
     "# valid  HTML  formatting  statements  that  you  wish to\n"
     "# included as information giving help to your archive users,"
     "# in the lower navigation bar.\n", FALSE},

    {"ihtmlnavbar2upfile", &set_ihtmlnavbar2up, NULL, CFG_STRING,
     "# Define path as the path to a template  file  containing\n"
     "# valid  HTML  formatting  statements  that  you  wish to\n"
     "# included as information giving links to the hierarchin your archive.\n", FALSE},

    {"mhtmlheaderfile", &set_mhtmlheader, NULL, CFG_STRING,
     "# Define path as the path to a template  file  containing\n"
     "# valid  HTML  formatting statements that you wish to use\n"
     "# at the top of every message page.\n", FALSE},

    {"mhtmlfooterfile", &set_mhtmlfooter, NULL, CFG_STRING,
     "# Define path as the path to a template  file  containing\n"
     "# valid HTML formatting statements you wish to use at the\n"
     "# bottom of every message page.\n", FALSE},

    {"locktime", &set_locktime, INT(3600), CFG_INTEGER,
     "# Specify number of seconds to wait for a lock before we\n"
     "# override it! .\n", FALSE},

    {"dateformat", &set_dateformat, NULL, CFG_STRING,
     "# Format (see strftime(3)) for displaying dates.\n", FALSE},

    {"indexdateformat", &set_indexdateformat, NULL, CFG_STRING,
     "# Format (see strftime(3)) for displaying dates in the index pages.\n"
     "# Will use dateformat if not specified.\n", FALSE},

    {"stripsubject", &set_stripsubject, NULL, CFG_STRING,
     "# A word to be stripped from all subject lines.  Helps unclutter\n"
     "# mailing lists which add tags to subject lines.\n", FALSE},

    {"attachmentlink", &set_attachmentlink, NULL, CFG_STRING,
     "# Format of the attachment links.\n"
     "# %p for the full path to the attachment\n"
     "# %f for the file name part only\n"
     "# %d for the directory name only\n"
     "# %n for the message number\n" "# %c for the content type string\n", FALSE},

    {"spamprotect", &set_spamprotect, BTRUE, CFG_SWITCH,
     "# Set this to On to make hypermail not output real email addresses\n"
     "# in the output HTML but instead it will obfuscate them a little.\n"
     "# You can control the obfuscation with set_antispamdomain.\n", FALSE},
  
    {"antispamdomain", &set_antispamdomain, NULL, CFG_STRING,
     "# By default hypermail only does a small amount of massaging\n"
     "# of email addresses. Use this to completely replace the domain\n"
     "# from which a message originates (everything after the @)\n"
     "# with some string to confuse screen-scraping programs.\n"
     "# It is probably wise to make this an invalid mail domain.\n", FALSE}, 

    {"spamprotect_id", &set_spamprotect_id, BTRUE, CFG_SWITCH,
     "# Set this to On to make hypermail not output real email message\n"
     "# ids in HTML comments (sometimes used internally by hypermail) but\n"
     "# instead it will obfuscate them a little so they don't look like\n"
     "# email addresses to spammers.\n", FALSE},

    {"attachmentsindex", &set_attachmentsindex, BTRUE, CFG_SWITCH,
     "# Set this to  Off to make hypermail not output an index of\n"
     "# messages with attachments.\n", FALSE},

    {"linkquotes", &set_linkquotes, BFALSE, CFG_SWITCH, 
     "# Set this to On to create fine-grained links from quoted\n"
     "# text to the text where the quote originated. It also improves\n"
     "# the threads index file by more accurately matching messages\n"
     "# with replies. Note that this may be rather cpu intensive (see\n"
     "# the searchbackmsgnum option to alter the performance).\n" , FALSE},

    {"searchbackmsgnum", &set_searchbackmsgnum, INT(500), CFG_INTEGER,
     "# If the linkquotes option is on and an incremental update is being"
     "# done (-u option), this controls the tradeoff between speed and\n"
     "# the reliability of finding the right source for quoted text.\n"
     "# Try to set it to the largest number of messages between a\n"
     "# message and the final direct reply to that message.\n", FALSE},

    {"link_to_replies", &set_link_to_replies, NULL, CFG_STRING,
     "# If the linkquotes option is on, specifying a string here\n"
     "# causes it to generate links from original quoted text to the\n"
     "# location(s) in replies which quote them. The string\n"
     "# is used to display the link.\n", FALSE},

    {"quote_hide_threshold", &set_quote_hide_threshold, INT(100), CFG_INTEGER,
     "# If the linkquotes option is on, setting this to an\n"
     "# integer less than 100 will cause it to replace quoted\n"
     "# text with one-line links if the percent of lines in the\n"
     "# message body (exluding the signature) consisting of\n"
     "# quoted text exceeds the number indicated by this option.\n", FALSE},

    {"quote_link_string", &set_quote_link_string, NULL, CFG_STRING,
     "# If the quote_hide_threshold option is being used, the\n"
     "# quote_link_string will be used if available to display the\n"
     "# link that replaces the quoted text. If no string is specified\n"
     "# here, the first line of each section of quoted text will used.\n", FALSE},

    {"monthly_index", &set_monthly_index, BFALSE, CFG_SWITCH,
     "# Set this to On to create additional index files broken up\n"
     "# by month. A summary.html file will provide links to all the\n"
     "# monthly indices.\n", FALSE},

    {"yearly_index", &set_yearly_index, BFALSE, CFG_SWITCH,
     "# Set this to On to create additional index files broken up\n"
     "# by year. A summary.html file will provide links to all the\n"
     "# yearly indices.\n", FALSE},

    {"thread_file_depth", &set_thread_file_depth, INT(0), CFG_INTEGER,
     "# If nonzero, break the threads index file into multiple files,\n"
     "# with the initial message of each thread in the main index file\n"
     "# along with links to files containing the replies. Setting this\n"
     "# to 1 creates one file for each thread that has replies, and is\n"
     "# recommended for archives with over a few hundred messages.\n"
     "# Setting this greater than 1 will produce multiple levels of files\n"
     "# for each thread whose replies are nested by more than 1 level,\n"
     "# but that is rarely useful. This option is currently disabled\n"
     "# if the indextable option is turned on, and probably needs to\n"
     "# be less than thrdlevels.\n", FALSE},

    {"startmsgnum", &set_startmsgnum, INT(0), CFG_INTEGER,
     "# Sets the number of the first message of an archive. This option is\n"
     "# only active when adding new messages to brand new archive.\n"
     "# If not set, the default number will be 0000.\n"
     "# Note that if you change this setting, you are stuck with it. If you\n"
     "# rebuild your archive, you must use the same value or you'll break any\n"
     "# link pointing to your archive.\n", FALSE},

    {"folder_by_date", &set_folder_by_date, NULL, CFG_STRING,
     "# This string causes the messages to be put in subdirectories\n"
     "# by date. The string will be passed to strftime(3) to generate\n"
     "# subdirectory names based on message dates. Suggested values are\n"
     "# \"%y%m\" or \"%b%y\" for monthly subdirectories, \"%Y\" for\n"
     "# yearly, \"%G/%V\" for weekly. Do not alter this for an existing\n"
     "# archive without removing the old html files. If you use this\n"
     "# and update the archive incrementally (e.g. with -u), you must\n"
     "# use the usegdbm option.\n", FALSE},

    {"msgsperfolder", &set_msgsperfolder, INT(0), CFG_INTEGER,
     "# Put messages in subdirectories with this many messages per\n"
     "# directory. Do not use this and folder_by_date on the same archive.\n"
     "# Do not alter this for an existing archive without removing the old\n"
     "# html files. Deleted/expired messages ARE COUNTED for the purpose\n"
     "# of deciding how many messages to put in a subdirectory.\n", FALSE},

    {"describe_folder", &set_describe_folder, NULL, CFG_STRING,
     "# Controls the labels used in folders.html to describe the\n"
     "# directories created by the folder_by_date or msgsperfolder\n"
     "# options. For folder_by_date labels, the describe_folder string\n"
     "# will be passed to strftime(3) the same as the folder_by_date string.\n"
     "# For msgsperfolder:\n"
     "# %d for the directory number (starts with 0)\n"
     "# %D for the directory number (starts with 1)\n"
     "# %m for the number of the first message in the directory\n"
     "# %M for the number of the last message that can be put in the\n"
     "# directory.\n", FALSE},

    {"latest_folder", &set_latest_folder, NULL, CFG_STRING,
     "# If folder_by_date or msgsperfolder are in use, create\n"
     "# a symbolic link by this name to the most recently created\n"
     "# subdirectory. Note that many web servers are configured to\n"
     "# not follow symbolic links for security reasons. The link will\n"
     "# be created in the directory specified by the 'dir' or '-d' option.\n",
     FALSE},

    {"base_url", &set_base_url, NULL, CFG_STRING,
     "# The url of the archive's main directory. This is needed when\n"
     "# the latest_folder option is used and the folder_by_date makes\n"
     "# directories more than one level deep (e.g. with '%y/%m').\n", FALSE},

    {"iso2022jp", &set_iso2022jp, BFALSE, CFG_SWITCH,
     "# Set this to On to support ISO-2022-JP messages.\n", FALSE},

    {"noindex_onindexes", &set_noindex_onindexes, BFALSE, CFG_SWITCH,
     "# Set to On to inform search engines that you don't want to index\n"
     "# the hypermail generated indexes. See the \"annotated\" configuration"
     "# option for a more detailed description.\n", FALSE},

    {"annotated", &set_annotated, "X-Hypermail-Annotated", CFG_LIST,
     "# This is the list of headers that indicate that a message was annotated.\n"
     "# When a message contains such a header, the header may have one more comma\n"
     "# separated values indicatating the annotation type. Order and case are\n"
     "# not important.\n"
     "# The possible values of this header are: content and robot annotations.\n"
     "# Content annotations can have only one of the following values:\n"
     "#      spam : message deleted because it is spam;\n"
     "#   deleted : message deleted, other reasons;\n"
     "#    edited : original received message was manually edited.\n"
     "# You can customize the markup that\'s shown for content annotations\n"
     "# by means of the htmlmessage_deleted_other, htmlmessage_deleted_spam\n,"
     "# htmlmessage_edited directives.\n\n"
     "# robot annotations can have either one or both of the following values:\n"
     "#  nofollow : do not follow the links on this page;\n"
     "#   noindex : prevent search engines from indexing the contents of this message.\n"
     "# Robot annotations instruct a visiting web  robot agent if a message contents\n"
     "# should be indexed and/or if the outgoing links from the message\n"
     "# should be followed, doing so thru a specific HTML meta tag. You can use one or\n"
     "# both values and combine them with the edited content annotation.\n"
     "# NOTE: Spam or deleted annotation values have an implicit robot \"noindex\"\n"
     "# annotation In such case, user supplied robot annotations values will be silently\n"
     "# ignored.\n", FALSE},

    {"deleted", &set_deleted, "X-Hypermail-Deleted X-No-Archive", CFG_LIST,
     "# NOTE: this option has been deprecated by annotated, but it will continue\n"
     "# being parsed and honored for legacy reasons.\n"
     "# This is the list of headers that indicate the message should\n"
     "# not be displayed if the value of this header is 'yes'.\n", FALSE},

    {"expires", &set_expires, "Expires", CFG_LIST,
     "# This is the list of headers that indicate the message should\n"
     "# not be displayed if the value of this header is a date in the past.\n", FALSE},

    {"delete_older", &set_delete_older, NULL, CFG_STRING,
     "# Any message older than this date should not be displayed.\n", FALSE},

    {"delete_newer", &set_delete_newer, NULL, CFG_STRING,
     "# Any message newer than this date should not be displayed.\n", FALSE},

    {"delete_msgnum", &set_delete_msgnum, NULL, CFG_LIST,
     "# This is the list of message numbers that should be deleted from the\n"
     "# html archive. The mbox is not changed.\n", FALSE},

    {"delete_incremental", &set_delete_incremental, BTRUE, CFG_SWITCH,
     "# If this option is enabled, hypermail will perform deletions on old\n"
     "# messages when run in incremental mode (according to the other delete\n"
     "# configuring option). Note that depending on your hypermail setup,\n"
     "# the size of the archive, and the complexity of the markup,\n"
     "# there may be memory and parsing issues, specifically when there are\n"
     "# non-deleted replies to a deleted message.\n"
     "# If this option is disabled, deleted messages will only be removed\n"
     "# when rebuilding the whole archive.\n", FALSE},

    {"delete_level", &set_delete_level, INT(DELETE_LEAVES_STUBS), CFG_INTEGER,
     "# 0 - remove deleted and expired files. Note that with this choice\n"
     "#     threading may be screwed up if there are replies to deleted or\n"
     "#     expired options and the archive is updated incrementally\n"
     "# 1 - remove message body\n"
     "# 2 - remove message body for deleted messages, leave expired messages\n"
     "# 3 - leave all messages\n"
     "# Deleted and expired messages are removed from the index files\n"
     "# regardless of the delete_level selection.\n", FALSE},

    {"txtsuffix", &set_txtsuffix, NULL, CFG_STRING,
     "# If you want the original mail messages archived in individual files,\n"
     "# set this to the extension that you want these messages to have\n"
     "# (recommended value: txt).\n", FALSE},

    {"filter_out", &set_filter_out, NULL, CFG_STRINGLIST,
     "# Delete from the html archives any message having a header line\n"
     "# which matches any of these expressions. Uses the same rules for\n"
     "# deletion as the expires option. The expressions use the same\n"
     "# syntax as Perl regular expressions.\n", FALSE},

    {"filter_require", &set_filter_require, NULL, CFG_STRINGLIST,
     "# Delete from the html archives any message not having header lines\n"
     "# which match each of these expressions. Uses the same rules for\n"
     "# deletion as the expires option. The expressions use the same\n"
     "# syntax as Perl regular expressions.\n", FALSE},

    {"filter_out_full_body", &set_filter_out_full_body, NULL, CFG_STRINGLIST,
     "# Delete from the html archives any message having a line\n"
     "# which matches any of these expressions. Uses the same rules for\n"
     "# deletion as the expires option. The expressions use the same\n"
     "# syntax as Perl regular expressions.\n", FALSE},

    {"filter_require_full_body", &set_filter_require_full_body, NULL, CFG_STRINGLIST,
     "# Delete from the html archives any message not having lines\n"
     "# which match each of these expressions. Uses the same rules for\n"
     "# deletion as the expires option. The expressions use the same\n"
     "# syntax as Perl regular expressions.\n", FALSE},

    {"save_alts", &set_save_alts, INT(0), CFG_INTEGER,
     "# This controls what happens to alternatives (other than the prefered\n"
     "# alternative) for multipart/alternative messages.\n"
     "# 0 - discard non-prefered alternatives\n"
     "# 1 - show all alternatives inline\n"
     "# 2 - put non-prefered alternatives in a separate file.\n", FALSE},

    {"alts_text", &set_alts_text, NULL, CFG_STRING,
     "# If save_alts is 1, this text is put between the alternatives.\n"
     "# If save_alts is 2, this text is used to describe the link to each\n"
     "# alternative file.\n", FALSE},

    {"warn_surpressions", &set_warn_surpressions, BTRUE, CFG_SWITCH,
     "# Set this to On to get warnings (on stdout) about messages that\n"
     "# are not converted because of they are missing a msgid (if\n"
     "# require_msgids is On) or because one of the following options\n"
     "# surpressed it: deleted expires delete_msgnum filter_out\n"
     "# filter_require filter_out_full_body filter_require_full_body.\n", FALSE},

    {"unsafe_chars", &set_unsafe_chars, NULL, CFG_STRING,
     "# Any characters listed in this string are removed from user-specified\n"
     "# attachment filenames. Those characters will be replaced by a \"_\"\n"
     "# (which means that specifying \"_\" here won't have any effect).\n"
     "# Note that many characters (including / and \\) are removed by the\n"
     "# safe_filename in parse.c regardless of what this option says. There\n"
     "# might be some security problems that can be prevented if you specify\n"
     "# \".\" here (e.g. if a web server is configured to enable server side\n"
     "# includes on filenames ending in something other than .shtml), but\n"
     "# that will prevent browsers from recognizing many file types.\n", FALSE},

    {"files_by_thread", &set_files_by_thread, BFALSE, CFG_SWITCH,
     "# Set this to On to generate (in addition to the usual files),\n"
     "# a file for each thread that contains all the messages in that\n"
     "# thread.\n", FALSE},

    {"href_detection", &set_href_detection, BTRUE, CFG_SWITCH,
     "# Set this to On to assume that any string on the body of the message\n"
     "# that says <A HREF=\" ... </A> is a URL, together with its markup\n"
     "# and treat it as such.\n", TRUE},

    {"mbox_shortened", &set_mbox_shortened, BFALSE, CFG_SWITCH,
     "# Set this to On to enable use of mbox that has had some of its\n"
     "# initial messages deleted. Requires usegdbm = 1 and increment = 0.\n"
     "# The first message in the shortened mbox must have a Message-Id header.\n"
     "# If discard_dup_msgids is 0, the first message in the shortened mbox\n"
     "# may not have the same Message-Id as a message that was deleted.\n"
     "# The mbox may not be altered in any way other than deleting from\n"
     "# beginning of the mbox or appending new messages to the end (unless\n"
     "# you rebuild the archive from scratch using a complete mbox).\n", FALSE},

    {"report_new_folder", &set_report_new_folder, BFALSE, CFG_SWITCH,
     "# Set this to On to have it print (on stdout) the names of any\n"
     "# new directories created pursuant to the folder_by_date or\n"
     "# msgsperfolder option, or the initial creation of the archive.\n"
     "# It will print the full path if that is what you use to specify\n"
     "# the archive directory. Does not print anything when attachment\n"
     "# or metadata directories are created.\n", FALSE},

    {"report_new_file", &set_report_new_file, BFALSE, CFG_SWITCH,
     "# Set this to On to have it print (on stdout) the names of any\n"
     "# new files created for new messages. It will print the full path\n"
     "# if that is what you use to specify the archive directory.\n", FALSE},

    {"use_sender_date", &set_use_sender_date, BFALSE, CFG_SWITCH,
     "# Set this to On to have it use the Date: header (created by the\n"
     "# the system that sent the message) rather than the date/time the\n"
     "# message was received, for purposes such as putting in folders\n"
     "# or sorting. Details of which purposes this affects may change\n"
     "# in the future.\n", FALSE},

    {"filename_base", &set_filename_base, NULL, CFG_STRING,
     "# This option overrides the normal rules for creating attachment\n"
     "# file names, and creates file names from the string that this\n"
     "# option is set to plus a file name extension if one can be found\n"
     "# in the name supplied by the message. This option is mainly for\n"
     "# languages that use different character sets from English.\n", FALSE},
};

/* ---------------------------------------------------------------- */

void MakeConfig(bool comments)
{
    int i;
    time_t now = time(NULL);
    if (comments) {
	fprintf(stdout,
		"#################################################################\n"
		"# This is an automatically generated config file done by\n"
		"# Hypermail " VERSION " patchlevel " PATCHLEVEL " - %s",
		ctime(&now));
    }
    for (i = 0; i < sizeof(cfg) / sizeof(cfg[0]); i++) {
	if (cfg[i].verbose) {
	    if (comments) {
		fputs("\n", stdout);
		fputs(cfg[i].verbose, stdout);
	    }
	    switch (cfg[i].flags) {
	    case CFG_STRING:
		if (cfg[i].changed && *(char **)cfg[i].value)
		    /* they differ, show the actual contents */
		    fprintf(stdout, "%s = %s\n", cfg[i].label,
			    *(char **)cfg[i].value);
		else if (comments)
		    /* show the default value, but commented */
/*
		    fprintf(stdout, "#%s = %s\n", cfg[i].label,
*/
		    fprintf(stdout, "%s = %s\n", cfg[i].label,
			    cfg[i].def ? (char *)cfg[i].def : "");
		break;
	    case CFG_LIST:
	    case CFG_STRINGLIST:
		if (cfg[i].changed) {
		    print_list(cfg[i].label,
			       *(struct hmlist **)cfg[i].value);
		}
		else if (comments) {
		    /* show the default value, but commented */
/*
		    fprintf(stdout, "#%s = %s\n", cfg[i].label,
*/
		    fprintf(stdout, "%s = %s\n", cfg[i].label,
			    cfg[i].def ? (char *)cfg[i].def : "");
		}
		break;
	    case CFG_OCTAL:
		if (cfg[i].changed)
		    /* they differ, show the actual contents */
		    fprintf(stdout, "%s = %lo\n", cfg[i].label,
			    *(long *)(cfg[i].value));
		else if (comments)
		    /* show the default value, but commented */
/*
		    fprintf(stdout, "#%s = %lo\n", cfg[i].label,
*/
		    fprintf(stdout, "%s = %lo\n", cfg[i].label,
			    (long)cfg[i].def);
		break;
	    case CFG_INTEGER:
		if (cfg[i].changed)
		    /* they differ, show the actual contents */
		    fprintf(stdout, "%s = %ld\n", cfg[i].label,
			    *(long *)(cfg[i].value));
		else if (comments)
		    /* show the default value, but commented */
/*
		    fprintf(stdout, "#%s = %ld\n", cfg[i].label,
*/
		    fprintf(stdout, "%s = %ld\n", cfg[i].label,
			    (long)cfg[i].def);
		break;
	    case CFG_SWITCH:
		if (cfg[i].changed)
		    /* they differ, show the actual contents */
		    fprintf(stdout, "%s = %s\n",
			    cfg[i].label,
			    *(int *)(cfg[i].value) ? "On" : "Off");
		else if (comments)
/*
		    fprintf(stdout, "#%s = %s\n", cfg[i].label,
*/
		    fprintf(stdout, "%s = %s\n", cfg[i].label,
			    cfg[i].def ? "On" : "Off");
		break;
	    default:
		break;
	    }			/* end of switch() */
	}			/* end of if() */
    }				/* end of for() */
}				/* end of function */

static bool inited = FALSE;

void PreConfig(void)
{
    int i;
    char name[128] = "HM_";
    char *labp;
    int ind;
    void *defval;

    for (i = 0; i < sizeof(cfg) / sizeof(cfg[0]); i++) {
	cfg[i].changed = FALSE;

	/* build environment variable to read from */
	for (ind = 3, labp = cfg[i].label; *labp; labp++) {
	    name[ind++] = toupper(*labp);
	}
	name[ind] = 0;

	/* we try to get a default value from an environment variable */
	defval = getenv(name);
	if (!defval)
	    /* get the built-in default instead */
	    defval = cfg[i].def;

	switch (cfg[i].flags) {
	case CFG_STRING:
	    if (defval)
		*(char **)cfg[i].value = strsav(defval);
	    else
		*(char **)cfg[i].value = NULL;
	    break;
	case CFG_SWITCH:
	case CFG_INTEGER:
	case CFG_OCTAL:
	    if (defval == cfg[i].def)
	        *(int *)cfg[i].value = (int)defval;
	    else
	        *(int *)cfg[i].value = atoi(defval);
	    break;
	case CFG_LIST:
	    if (defval) {
                char *tpstr;
                tpstr = strsav(defval);
		*(struct hmlist **)cfg[i].value = NULL;
		*(struct hmlist **)cfg[i].value =
		    (void *)add_list(*(struct hmlist **)cfg[i].value, tpstr);
                free(tpstr);
            }
	    else
		*(struct hmlist **)cfg[i].value = NULL;
	    break;
	case CFG_STRINGLIST:
	    if (defval) {
                char *tpstr;
                tpstr = strsav(defval);
		*(struct hmlist **)cfg[i].value = NULL;
		*(struct hmlist **)cfg[i].value =
		    (void *)add_2_list(*(struct hmlist **)cfg[i].value, tpstr);
                free(tpstr);
            }
	    else
		*(struct hmlist **)cfg[i].value = NULL;
	    break;
	default:
	    break;
	}
    }
    inited = TRUE;
}

void PostConfig(void)
{
    int i;
    /* Keep default behavior the same as it was when mailcommand applied
    * to the cases now covered by replymsg_command. */
    if (!strcmp(set_replymsg_command, "not set"))
	set_replymsg_command = set_mailcommand;

    show_index[1][AUTHOR_INDEX]  = !inlist(set_avoid_indices, "author");
    show_index[1][DATE_INDEX]    = !inlist(set_avoid_indices, "date");
    show_index[1][SUBJECT_INDEX] = !inlist(set_avoid_indices, "subject");
    show_index[1][THREAD_INDEX]  = !inlist(set_avoid_indices, "thread");
    show_index[1][ATTACHMENT_INDEX] = !inlist(set_avoid_indices, "attachments")
        && set_attachmentsindex;
    if (set_folder_by_date || set_msgsperfolder) {
	show_index[0][AUTHOR_INDEX]  = !inlist(set_avoid_top_indices, "author");
	show_index[0][DATE_INDEX]    = !inlist(set_avoid_top_indices, "date");
	show_index[0][SUBJECT_INDEX] = !inlist(set_avoid_top_indices, "subject");
	show_index[0][THREAD_INDEX]  = !inlist(set_avoid_top_indices, "thread");
	show_index[0][ATTACHMENT_INDEX] = !inlist(set_avoid_top_indices, "attachments")
	    && set_attachmentsindex;
	show_index[0][FOLDERS_INDEX]  = !inlist(set_avoid_top_indices, "folders");
    }
    else {
	for (i = 0; i <= ATTACHMENT_INDEX; ++i)
	    show_index[0][i] = show_index[1][i];
    }
    if (set_htmlbody != NULL)
	printf("Warning: the body option has been disabled. See the\n"
	       "INSTALL file for instructions on replacing it with a style sheet.\n");
}

int ConfigAddItem(char *cfg_line)
{
    char keyword[256];
    char towhat[1001]; /* increased due to htmlmessage_deleted */
    char *keywp;
    int i;
    char *line = cfg_line;
 
    /* 
    ** This is here to support earlier config file formats. 
    ** The "set " is nolonger needed but has been added 
    ** for backwards compatability.
    */

    if (!strncmp (line, "set ", 4))
        line += 4;

    if (2 <= sscanf(line, " %255[a-zA-Z0-9._] %*[=: ] %500[^\n]",
		    keyword, towhat)) {

	if ('\"' == towhat[0]) {
	    /* we have a quote as the first character, then we assume we have a
	       "string" to parse instead of just string. */
	    sscanf(line, " %255[a-zA-Z0-9._] %*[=: ] \"%500[^\"]",
		   keyword, towhat);
	}
	else {
	    /* no quotes, we eat all trailing white spaces */
	    i = strlen(towhat);
	    if (i--) {
		while ((i >= 0) && isspace(towhat[i]))
		    towhat[i--] = 0;
	    }
	}

	if (!strncasecmp(keyword, "hm_", 3))
	    /* if there is a hm_ prefix, skip it! */
	    keywp = keyword + 3;
	else
	    keywp = keyword;

	for (i = 0; i < sizeof(cfg) / sizeof(cfg[0]); i++) {

	    if (*keywp != *(cfg[i].label)) 	/* minor speedup */
                continue;

	    if (!strcasecmp(keywp, cfg[i].label)) {	/* match */

		switch (cfg[i].flags) {
		case CFG_STRING:
		    *(char **)cfg[i].value = strsav(towhat);
		    break;
		case CFG_SWITCH:
		    if (atoi(towhat) ||
			!strcasecmp("ON", towhat) ||
			!strcasecmp("YES", towhat))
			    *(int *)cfg[i].value = (int)TRUE;
		    else
			*(int *)cfg[i].value = (int)FALSE;
		    break;
		case CFG_INTEGER:
		    /* get an integer using any base */
		    /* ON/YES support is for options that used to be 
		       CFG_SWITCH's */
		    if (!strcasecmp("ON", towhat) || !strcasecmp("YES", towhat))
			*(int *)cfg[i].value = 1;
		    else
		        *(int *)cfg[i].value = (int)strtol(towhat, NULL, 0);
		    break;
		case CFG_OCTAL:
		    /* get an octal number */
		    *(int *)cfg[i].value = (int)strtol(towhat, NULL, 8);
		    break;
		case CFG_LIST:

                    /* Is this the first time that it's been called
                     * for this list ? If so then there is a value
                     * being set for the list via a config file.
                     * In the case of lists we need to replace the
                     * current value with the new material completely.
                     */
                    if (cfg[i].changed == FALSE) { /* first time through ? */
                        if (cfg[i].value) {
                            if (*(struct hmlist **)cfg[i].value)
                                 free(*(struct hmlist **)cfg[i].value);
		            *(struct hmlist **)cfg[i].value = NULL;
                        }
                    }
		    *(struct hmlist **)cfg[i].value =
		       (void *)add_list(*(struct hmlist **)cfg[i].value,towhat);
		    break;

		case CFG_STRINGLIST:

                    /* Is this the first time that it's been called
                     * for this list ? If so then there is a value
                     * being set for the list via a config file.
                     * In the case of lists we need to replace the
                     * current value with the new material completely.
                     */
                    if (cfg[i].changed == FALSE) { /* first time through ? */
                        if (cfg[i].value) {
                            if (*(struct hmlist **)cfg[i].value)
                                 free(*(struct hmlist **)cfg[i].value);
		            *(struct hmlist **)cfg[i].value = NULL;
                        }
                    }
		    *(struct hmlist **)cfg[i].value =
		       (void *)add_2_list(*(struct hmlist **)cfg[i].value,towhat);
		    break;
		default:
		    break;
		}
		cfg[i].changed = TRUE;
		return 0;
	    }
	}
    }
    return 1;
}

int ConfigInit(char *filename)
{
    FILE *f;
    char line[MAXLINE];
    int r = TRUE;

    PreConfig();

    if (filename && *filename) {
	f = fopen(filename, "r");
	if (f) {
	    while (fgets(line, MAXLINE, f)) {
		if ((line[0] != '#') && (line[0] != '\n')) {
	            /* need to error check the format */
		    ConfigAddItem(line);
                }
            }
	    fclose(f);
	}
	else
	    r = FALSE;
    }

    PostConfig();

    return r;
}

void ConfigCleanup(void)
{
    int i;
    for (i = 0; i < sizeof(cfg) / sizeof(cfg[0]); i++) {
	switch (cfg[i].flags) {
	case CFG_LIST:
	case CFG_STRINGLIST:
	    if (cfg[i].value) {
		if (*(struct hmlist **)cfg[i].value)
		    free(*(struct hmlist **)cfg[i].value);
	    }
	    break;
	case CFG_STRING:
	    if (cfg[i].value) {
		if (*(char **)cfg[i].value)
		    free(*(char **)cfg[i].value);
	    }
	    break;
	default:
	    break;
	}
    }
}

#ifdef DEBUG

void dump_config(void)
{
    register char *cp;

    printf("\nConfiguration Values:\n");

    printf("set_language = %s\n",set_language ? set_language : "Not Used");
    printf("set_htmlsuffix = %s\n",set_htmlsuffix ? set_htmlsuffix : "Not Used");
    printf("set_mbox = %s\n", set_mbox ? set_mbox : "Not Used");
    printf("set_archives = %s\n",set_archives ? set_archives : "Not set");
    printf("set_custom_archives = %s\n",set_custom_archives ? set_custom_archives : "Not set");
    printf("set_about = %s\n",set_about ? set_about : "Not set");
    printf("set_dir = %s\n",set_dir ? set_dir : "Not set");
    printf("set_defaultindex = %s\n",set_defaultindex ? set_defaultindex : "Not set");
    printf("set_default_top_index = %s\n",set_default_top_index ? set_default_top_index : "Not set");

    printf("set_label = %s\n",set_label ? set_label : "Not set");
    printf("set_append_filename = %s\n",set_append_filename ? set_append_filename : "Not set");
    printf("set_dateformat = %s\n",set_dateformat ? set_dateformat : "Not set");
    printf("set_stripsubject = %s\n",set_stripsubject ? set_stripsubject : "Not set");
    printf("set_mailcommand = %s\n",set_mailcommand ? set_mailcommand : "Not set");
    printf("set_replymsg_command = %s\n",set_replymsg_command ? set_replymsg_command : "Not set");
    printf("set_inreplyto_command = %s\n",set_inreplyto_command ? set_inreplyto_command : "Not set");
    printf("set_newmsg_command = %s\n",set_newmsg_command ? set_newmsg_command : "Not set");
    printf("set_mailto = %s\n",set_mailto ? set_mailto : "Not set");
    printf("set_hmail = %s\n",set_hmail ? set_hmail : "Not set");
    printf("set_domainaddr = %s\n",set_domainaddr ? set_domainaddr : "Not set");
    printf("set_icss_url = %s\n",set_icss_url ? set_icss_url : "Not set");
    printf("set_mcss_url = %s\n",set_mcss_url ? set_mcss_url : "Not set");

    printf("set_attachmentlink = %s\n",set_attachmentlink ? set_attachmentlink : "Not set");
    printf("set_link_to_replies = %s\n",set_link_to_replies ? set_link_to_replies : "Not set");
    printf("set_quote_link_string = %s\n",set_quote_link_string ? set_quote_link_string : "Not set");
    printf("set_folder_by_date = %s\n",set_folder_by_date ? set_folder_by_date : "Not set");
    printf("set_describe_folder = %s\n",set_describe_folder ? set_describe_folder : "Not set");
    printf("set_latest_folder = %s\n",set_latest_folder ? set_latest_folder : "Not set");
    printf("set_antispamdomain = %s\n",set_antispamdomain ? set_antispamdomain : "Not set");

    /* Boolean or integer */

    printf("set_overwrite = %d\n",set_overwrite);
    printf("set_inlinehtml = %d\n",set_inlinehtml);
    printf("set_increment = %d\n",set_increment);
    printf("set_readone = %d\n",set_readone);
    printf("set_reverse = %d\n",set_reverse);
    printf("set_showprogress = %d\n",set_showprogress);
    printf("set_showheaders = %d\n",set_showheaders);
    printf("set_showhtml = %d\n",set_showhtml);
    printf("set_showbr = %d\n",set_showbr);
    printf("set_showhr = %d\n",set_showhr);
    printf("set_showreplies = %d\n",set_showreplies);
    printf("set_show_msg_links = %d\n",set_show_msg_links);
    printf("set_usetable = %d\n",set_usetable);
    printf("set_indextable = %d\n",set_indextable);
    printf("set_iquotes = %d\n",set_iquotes);
    printf("set_eurodate = %d\n",set_eurodate);
    printf("set_isodate = %d\n",set_isodate);
    printf("set_gmtime = %d\n",set_gmtime);
    printf("set_require_msgids = %d\n",set_require_msgids);
    printf("set_discard_dup_msgids = %d\n",set_discard_dup_msgids);
    printf("set_usemeta = %d\n",set_usemeta);
    printf("set_userobotmeta = %d\n",set_userobotmeta);
    printf("set_uselock = %d\n",set_uselock);
    printf("set_locktime = %d\n",set_locktime);
    printf("set_ietf_mbox = %d\n",set_ietf_mbox);
    printf("set_usegdbm = %d\n",set_usegdbm);
    printf("set_writehaof = %d\n",set_writehaof);
    printf("set_append = %d\n",set_append);
    printf("set_nonsequential = %d\n",set_nonsequential);
    printf("set_thrdlevels = %d\n",set_thrdlevels);
    printf("set_dirmode = %04o\n",set_dirmode);
    printf("set_filemode = %04o\n",set_filemode);
    printf("set_spamprotect = %04o\n",set_spamprotect);
    printf("set_attachmentsindex = %04o\n",set_attachmentsindex);
    printf("set_linkquotes = %d\n",set_linkquotes);
    printf("set_searchbackmsgnum = %d\n",set_searchbackmsgnum);
    printf("set_quote_hide_threshold = %d\n",set_quote_hide_threshold);
    printf("set_thread_file_depth = %d\n",set_thread_file_depth);
    printf("set_monthly_index = %d\n",set_monthly_index);
    printf("set_yearly_index = %d\n",set_yearly_index);
    printf("set_msgsperfolder = %d\n",set_msgsperfolder);
    printf("set_iso2022jp = %d\n",set_iso2022jp);
    printf("set_delete_incremental = %d\n",set_delete_incremental);
    printf("set_delete_level = %d\n",set_delete_level);
    printf("set_delete_older = %d\n",set_delete_older);
    printf("set_delete_newer = %d\n",set_delete_newer);
    printf("set_noindex_onindexes = %d\n",set_noindex_onindexes);
    printf("set_format_flowed= %d\n",set_format_flowed);
    printf("set_format_flowed_disable_quoted= %d\n",set_format_flowed_disable_quoted);

    if (!set_ihtmlheader)
        printf("set_ihtmlheader = Not set\n");
    else {
        printf("set_ihtmlheader = ");
        for (cp = set_ihtmlheader; *cp; cp++)
             putchar(*cp);
    }

    if (!set_ihtmlfooter)
        printf("set_ihtmlfooter = Not set\n");
    else {
        printf("set_ihtmlfooter = ");
        for (cp = set_ihtmlfooter; *cp; cp++)
             putchar(*cp);
    }

    if (!set_ihtmlhead)
        printf("set_ihtmlhead = Not set\n");
    else {
        printf("set_ihtmlhead = ");
        for (cp = set_ihtmlhead; *cp; cp++)
             putchar(*cp);
    }

    if (!set_ihtmlhelpup)
        printf("set_ihtmlhelpup = Not set\n");
    else {
        printf("set_ihtmlhelpup = ");
        for (cp = set_ihtmlhelpup; *cp; cp++)
             putchar(*cp);
    }

    if (!set_ihtmlhelplow)
        printf("set_ihtmlhelplow = Not set\n");
    else {
        printf("set_ihtmlhelplow = ");
        for (cp = set_ihtmlhelplow; *cp; cp++)
             putchar(*cp);
    }

    if (!set_ihtmlnavbar2up)
        printf("set_ihtmlnavbar2up = Not set\n");
    else {
        printf("set_ihtmlnavbar2up = ");
        for (cp = set_ihtmlnavbar2up; *cp; cp++)
             putchar(*cp);
    }


    if (!set_mhtmlheader)
        printf("set_mhtmlheader = Not set\n");
    else {
        printf("set_mhtmlheader = ");
        for (cp = set_mhtmlheader; *cp; cp++)
             putchar(*cp);
    }

    if (!set_mhtmlfooter)
        printf("set_mhtmlfooter = Not set\n");
    else {
        printf("set_mhtmlfooter = ");
        for (cp = set_mhtmlfooter; *cp; cp++)
             putchar(*cp);
    }

    /* List values */

    print_list("set_text_types", set_text_types);
    print_list("set_inline_types", set_inline_types);
    print_list("set_prefered_types",set_prefered_types);
    print_list("set_ignore_types",set_ignore_types);
    print_list("set_show_headers", set_show_headers);
    print_list("set_avoid_top_indices", set_avoid_top_indices);
    print_list("set_avoid_indices", set_avoid_indices);
    print_list("set_annotated", set_annotated);
    print_list("set_deleted", set_deleted);
    print_list("set_expires", set_expires);
    print_list("set_delete_msgnum", set_delete_msgnum);

}

#endif
