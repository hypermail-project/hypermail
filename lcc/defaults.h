#ifndef __DEFAULTS_H
#define __DEFAULTS_H

/* ALERT! ALERT! ALERT! ALERT! ALERT! ALERT! ALERT! ALERT!
 *
 * The #defines following here ARE NOT intended to be modified locally 
 * before you compile hypermail. They're here to set internal defaults 
 * and they are set during the configure process. To alter these settings
 * for use during runtime, modify your config file!
 */

#define CONFIGFILE "~/.hmrc"

#define MSG_FRAGMENT_PREFIX "msg"

#define INLINE_TYPES "image/gif image/jpeg image/png"

#define SHOW_HEADERS "From Subject Date Message-ID"

#define MAILCOMMAND "mailto:$TO?Subject=$SUBJECT"

#define PROGRESS 0

#define RECOGNIZE_SUPERCITE_QUOTES 1

#define LANGUAGE     "@language@"

#define HTMLSUFFIX   "@htmlsuffix@"

#define DEFAULTINDEX "@defaultindex@"

#define DEFAULT_TOP_INDEX "folders"

#define DOMAINADDR   "@domainaddr@"

#define ANTISPAM_AT   "_at_"

#define APPLE_MAIL_UA_HEADER "X-Mailer"

#define APPLE_MAIL_UA "Apple iPhone iPad"

#define DEFAULT_CHARSET "US-ASCII"

#define HM_ANNOTATION_HEADER "X-Hypermail-Annotated"

#define HM_DELETED_HEADERS "X-Hypermail-Deleted X-No-Archive"

#define EXPIRES_HEADER "Expires"

#define NEW_MSG_COMMAND "mailto:$TO"

#define REPLYMSG_COMMAND "not set"

#define DEFAULT_CSS_URL "hypermail.css"

#endif
