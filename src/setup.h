#ifndef __SETUP_H
#define __SETUP_H

typedef int bool;

struct Config {
    char *label;		/* the real-name of what to set */
    void *value;		/* pointer to the real storage */
    void *def;			/* default value */
    char flags;			/* specifies the kind of config setting, 
                                   see defines below */
    char *verbose;		/* detailed description of the label */
    bool changed;		/* read from .config or .set file */
};

#define CFG_INTEGER 0		/* 0 - 2G */
#define CFG_SWITCH  1		/* TRUE / FALSE */
#define CFG_STRING  2		/* regular string, pointing to a static 
                                   string area */
#define CFG_LIST    3		/* comma seperated list, uses itemlist */
#define CFG_FUNCTION 4		/* function */
#define CFG_OCTAL   5		/* numerical, octal based */
#define CFG_STRINGLIST    6	/* uses itemlist, whole lines used as items */

#define BTRUE (void *)1
#define BFALSE (void *)0

#define INT(x) (void *)x

int ConfigInit(char *filename);
void ConfigCleanup(void);
int ConfigAddItem(char *line);
void MakeConfig(bool);		/* set to TRUE to include comments */
void PreConfig(void);
void PostConfig(void);

extern char *set_antispam_at;
extern char *set_htmlmessage_edited;
extern char *set_htmlmessage_deleted_other;
extern char *set_htmlmessage_deleted_spam;
extern char *set_fragment_prefix;
extern char *set_language;
extern char *set_htmlsuffix;
extern char *set_mbox;
extern char *set_archives;
extern char *set_custom_archives;
extern char *set_about;
extern char *set_dir;
extern char *set_defaultindex;
extern char *set_default_top_index;
extern char *set_txtsuffix;

extern bool set_email_address_obfuscation;
extern bool set_i18n;
extern bool set_i18n_body;
extern bool set_overwrite;
extern bool set_inlinehtml;
extern bool set_increment;
extern bool set_readone;
extern bool set_reverse;
extern bool set_reverse_folders;
extern bool set_showprogress;
extern bool set_showheaders;
extern bool set_showbr;
extern bool set_showhr;
extern bool set_showreplies;
extern int set_show_msg_links;
extern int set_show_index_links;
extern bool set_usetable;
extern bool set_indextable;
extern bool set_iquotes;
extern bool set_eurodate;
extern bool set_gmtime;
extern bool set_isodate;
extern bool set_require_msgids;
extern bool set_discard_dup_msgids;
extern bool set_usemeta;
extern bool set_uselock;
extern bool set_ietf_mbox;
extern bool set_linkquotes;
extern bool set_monthly_index;
extern bool set_yearly_index;
extern bool set_spamprotect;
extern bool set_spamprotect_id;
extern bool set_attachmentsindex;
extern bool set_usegdbm;
extern bool set_writehaof;
extern bool set_append;
extern char *set_append_filename;
extern bool set_nonsequential;
extern bool set_warn_surpressions;
extern bool set_files_by_thread;
extern bool set_href_detection;
extern bool set_mbox_shortened;
extern bool set_report_new_file;
extern bool set_report_new_folder;
extern bool set_use_sender_date;
extern bool set_inline_addlink;

extern int set_showhtml;
extern int set_thrdlevels;
extern int set_dirmode;
extern int set_filemode;
extern int set_locktime;
extern int set_searchbackmsgnum;
extern int set_quote_hide_threshold;
extern int set_thread_file_depth;
extern int set_startmsgnum;

extern int set_save_alts;
extern char *set_alts_text;

extern char *set_label;

extern char *set_mailcommand;
extern char *set_newmsg_command;
extern char *set_replymsg_command;
extern char *set_inreplyto_command;
extern char *set_mailto;
extern char *set_hmail;
extern char *set_domainaddr;

extern char *set_icss_url;
extern char *set_mcss_url;

extern char *set_dateformat;
extern char *set_indexdateformat;

extern char *set_stripsubject;
extern char *set_link_to_replies;
extern char *set_quote_link_string;

extern struct hmlist *set_text_types;
extern struct hmlist *set_inline_types;
extern struct hmlist *set_prefered_types;
extern struct hmlist *set_ignore_types;
extern struct hmlist *set_show_headers;
extern struct hmlist *set_skip_headers;
extern struct hmlist *set_avoid_indices;
extern struct hmlist *set_avoid_top_indices;
extern struct hmlist *set_filter_out;
extern struct hmlist *set_filter_require;
extern struct hmlist *set_filter_out_full_body;
extern struct hmlist *set_filter_require_full_body;

extern char *set_ihtmlheader;
extern char *set_ihtmlfooter;
extern char *set_ihtmlhead;
extern char *set_ihtmlhelpup;
extern char *set_ihtmlhelplow;
extern char *set_ihtmlnavbar2up;
extern char *set_mhtmlheader;
extern char *set_mhtmlfooter;

extern char *set_attachmentlink;
extern char *set_unsafe_chars;
extern char *set_filename_base;
extern bool set_linkquotes;

extern char *set_antispamdomain;

extern char *set_folder_by_date;
extern char *set_latest_folder;
extern char *set_base_url;
extern int set_msgsperfolder;
extern char *set_describe_folder;

extern bool set_iso2022jp;

extern struct hmlist *set_annotated;
extern struct hmlist *set_deleted;
extern struct hmlist *set_expires;
extern struct hmlist *set_delete_msgnum;
extern char *set_delete_older;
extern char *set_delete_newer;
extern bool set_delete_incremental;
extern int set_delete_level;
/* values for delete_level: */
enum { DELETE_REMOVES_FILES,
       DELETE_LEAVES_STUBS, /* always replace msg body with lang[MSG_DELETED/MSG_EXPIRED] */
       DELETE_LEAVES_EXPIRED_TEXT, /* stubs for deleted msgs, full msg for expired */
       DELETE_LEAVES_TEXT /* leave full message */
};

#endif
