/*
** WARNING!!! Don't muck with this file unless you know what you are
**            getting yourself into!!!!!!
*/

/* 
** Below are the messages used within hypermail that 
** are for user consumption. There are differences
** between user message strings and strings used in
** the HTML and SMTP protocols. Beware not to get
** confused with which is which.
** 
** What follows are a set of defines that act as indices 
** into the specific language array that is setup for
** this list archive. They need to point to the right
** messages in the language arrays or things get interesting.
**
** To add a new language, simply copy an existing language
** array and make the appropriate translations. For example,
** the Spanish language message array has the English message
** as the comment on the side of the message. (It is hoped
** this makes for easier translations... Also make sure and
** add the language to the supported_languages array.
**
** Currently this does not support wide-characters but it is
** a start in the right direction.  
*/

struct language_entry {
    char     *langcode;
    char     **mtable;
    char     *locale_code;
};

/* print.c */
#define MSG_NEW_MESSAGE            0
#define MSG_REPLY                  1
#define MSG_ABOUT_THIS_LIST        2
#define MSG_END_OF_MESSAGES        3
#define MSG_START_OF_MESSAGES      4
#define MSG_DATE_VIEW              5
#define MSG_THREAD_VIEW            6
#define MSG_SUBJECT_VIEW           7
#define MSG_AUTHOR_VIEW            8
#define MSG_ATTACHMENT_VIEW        9
#define MSG_MESSAGES              10
#define MSG_STARTING              11
#define MSG_ENDING                12
#define MSG_ABOUT_THIS_ARCHIVE    13
#define MSG_SORTED_BY             14
#define MSG_OTHER_MAIL_ARCHIVES   15
#define MSG_BY_DATE               16
#define MSG_MOST_RECENT_MESSAGES  17
#define MSG_AUTHOR                18
#define MSG_DATE                  19
#define MSG_THREAD                20
#define MSG_SUBJECT               21
#define MSG_FOR_OPTIONS           22
#define MSG_WRITING_ARTICLES      23
#define MSG_WRITING_DATE_INDEX    24
#define MSG_WRITING_THREAD_INDEX  25
#define MSG_WRITING_SUBJECT_INDEX 26
#define MSG_WRITING_AUTHOR_INDEX  27
#define MSG_LAST_MESSAGE_DATE     28
#define MSG_ARCHIVED_ON           29
#define MSG_CANNOT_CHMOD          30
#define MSG_COULD_NOT_WRITE       31
#define MSG_NEXT_MESSAGE          32
#define MSG_PREVIOUS_MESSAGE      33
#define MSG_MAYBE_IN_REPLY_TO     34
#define MSG_IN_REPLY_TO           35
#define MSG_NEXT_IN_THREAD        36
#define MSG_MAYBE_REPLY           37
#define MSG_BY_THREAD             38
#define MSG_BY_SUBJECT            39
#define MSG_BY_AUTHOR             40

/* file.c */
#define MSG_CANNOT_CREATE_DIRECTORY   41
#define MSG_CREATING_DIRECTORY        42
#define MSG_CONFIGURATION_VALUES      43
#define MSG_PATH                      44

/* mem.c */
#define MSG_RAN_OUT_OF_MEMORY         45

/* printfile.c */
#define MSG_ARCHIVE_GENERATED_BY      46

/* struct.c */
#define MSG_ELEMENTS                  47
#define MSG_NO_ELEMENTS               48

/* parse.c */
#define MSG_CANNOT_OPEN_MAIL_ARCHIVE  49
#define MSG_READING_NEW_HEADER        50
#define MSG_LOADING_MAILBOX           51
#define MSG_ENCODING_IS_NOT_SUPPORTED 52
#define MSG_ARTICLES                  53

/* hypermail.c */
#define MSG_VERSION                              54
#define MSG_PATCHLEVEL                           55
#define MSG_DOCS                                 56
#define MSG_COMMAND_AND_CONTROL_VARIABLES        57
#define MSG_YES                                  58
#define MSG_NO                                   59
#define MSG_ABOUT_THE_ARCHIVE_NOT_USED           60
#define MSG_OTHER_ARCHIVES_NOT_USED              61
#define MSG_ADDRESS_NOT_USED                     62
#define MSG_BUILTIN_BODY_STATEMENT_USED          63
#define MSG_CANNOT_READ_FROM_BOTH_FILE_AND_STDIN 64
#define MSG_OPTIONS                              65
#define MSG_OPTION_A                             66
#define MSG_OPTION_B                             67
#define MSG_OPTION_C                             68
#define MSG_OPTION_D                             69
#define MSG_OPTION_I                             70
#define MSG_OPTION_L                             71
#define MSG_OPTION_M                             72
#define MSG_OPTION_P                             73
#define MSG_OPTION_VERBOSE                       74
#define MSG_OPTION_VERSION                       75
#define MSG_OPTION_U                             76
#define MSG_OPTION_X                             77
#define MSG_OPTION_LANG                          78
#define MSG_USAGE                                79
#define MSG_LANGUAGE_NOT_SUPPORTED               80
#define MSG_NOT_SET                              81
#define MSG_NOT_USED                             82
#define MSG_CREATED_ATTACHMENT_FILE              83
#define MSG_ATTACHMENT                           84
#define MSG_MODE                                 85
#define MSG_READING_OLD_HEADERS                  86
#define MSG_OPTIONS_STRING2                      87
#define MSG_ERROR                                88
#define MSG_OPTION_N                             89
#define MSG_OPTION_1                             90
#define MSG_CAUTHOR                		 91
#define MSG_CDATE                  		 92
#define MSG_CSUBJECT               		 93

/* print.c This belong in the first list, but it's quite a lot of work
   to move all the numbers to add them there :-/ How can we simplify
   this work? Jose */
#define MSG_MAIL_ACTIONS                         94
#define MSG_MA_NEW_MESSAGE                       95
#define MSG_MA_REPLY                             96

#define MSG_MONTHLY_INDEX                        97
#define MSG_YEARLY_INDEX                         98

#define MSG_OPTION_G                             99
#define MSG_OPTION_G_NOT_BUILD_IN                100
#define MSG_CREATING_GDBM_INDEX                  101
#define MSG_CANT_CREATE_GDBM_INDEX               102
#define MSG_OPTION_ARCHIVE                       103
#define MSG_CANNOT_BOTH_READ_AND_WRITE_TO_MBOX   104

/* XXX Move these up with the others */
#define MSG_ATTACHMENT_VIEW_OBSOLETED		 105
#define MSG_BY_ATTACHMENT			 106
#define MSG_WRITING_ATTACHMENT_INDEX  		 107
#define MSG_BYTES                                108

#define MSG_CANNOT_CREATE_SYMLINK                109
#define MSG_CANNOT_UNLINK                        110
#define MSG_PREV_DIRECTORY                       111
#define MSG_NEXT_DIRECTORY                       112
#define MSG_FOLDERS_INDEX                        113
#define MSG_DELETED                              114
#define MSG_EXPIRED                              115
#define MSG_DEL_SHORT                            116
#define MSG_TXT_VERSION                          117
#define MSG_FILTERED_OUT                         118
#define MSG_FROM                                 119

#define MSG_OPTION_XML				 120
#define MSG_WRITING_HAOF			 121

/* JK: new messages added by WAI. When this is ready, it would
   be good to merge them into the above sections. The current
   use of localization in hypermail makes this quite complex to do. */

/* print.c */

#define MSG_THIS_MESSAGE                         122
#define MSG_MSG_BODY                             123
#define MSG_RESPOND                              124
#define MSG_MORE_OPTIONS                         125
#define MSG_RELATED_MESSAGES                     126
#define MSG_NEXT                                 127
#define MSG_PREVIOUS                             128
#define MSG_REPLIES                              129
#define MSG_CONTEMPORARY_MSGS_SORTED             130
#define MSG_HELP                                 131
#define MSG_LTITLE_IN_REPLY_TO                   132
#define MSG_LTITLE_NEXT_IN_THREAD                133
#define MSG_LTITLE_REPLIES                       134
#define MSG_LTITLE_BY_DATE                       135
#define MSG_LTITLE_BY_THREAD                     136
#define MSG_LTITLE_BY_SUBJECT                    137
#define MSG_LTITLE_BY_AUTHOR                     138
#define MSG_LTITLE_NEXT                          139
#define MSG_LTITLE_PREVIOUS                      140
#define MSG_LTITLE_BY_ATTACHMENT                 141
#define MSG_NAVBAR2UPPERLEVELS                   142
#define MSG_NAVBAR                               143
#define MSG_SORT_BY                              144
#define MSG_OTHER_PERIODS                        145
#define MSG_NEXTPERIOD                           146
#define MSG_LTITLE_NEXTPERIOD                    147
#define MSG_PREVPERIOD                           148
#define MSG_LTITLE_PREVPERIOD                    149
#define MSG_LTITLE_FOLDERS_INDEX                 150
#define MSG_THIS_PERIOD                          151
#define MSG_RECEIVED_ON                          152
#define MSG_NEARBY                               153
#define MSG_RESORTED                             154
#define MSG_LTITLE_LISTED_BY_DATE                155
#define MSG_LTITLE_DISCUSSION_THREADS            156
#define MSG_LTITLE_LISTED_BY_AUTHOR              157
#define MSG_LTITLE_LISTED_BY_SUBJECT             158
#define MSG_LTITLE_LISTED_BY_ATTACHMENT          159
#define MSG_PERIOD                               160
#define MSG_TO                                   161
#define MSG_FROM_TITLE                           162
#define MSG_ON_TITLE                             163
#define MSG_UNKNOWN_IN_REPLY_TO                  164
#define MSG_DELETED_OTHER                        165
#define MSG_EDITED                               166
#ifdef MAIN_FILE

/*
** German version of the language table
** Translation by Martin Schulze <joey@kuolema.Infodrom.North.DE> 
*/

char *de[] = {       /* German */
  "Neue Nachricht",                       /* New Message        -HTML*/
  "Antwort",                              /* Reply              -HTML*/
  "‹ber diese Liste",                  /* About this list    -HTML*/
  "Ende der Nachricht",                   /* End of Messages    -HTML*/
  "Anfang der Nachricht",                 /* Start of Messages  -HTML*/
  "Datumsansicht",                         /* Date view          -HTML*/
  "Themenansicht",                         /* Thread view        -HTML*/
  "Titelansicht",                        /* Subject view       -HTML*/
  "Autorenansicht",                         /* Author view        -HTML*/
  "Anh‰nge-‹bersicht",                    /* Attachment view       -HTML*/
  "Nachrichten",                          /* Messages           -HTML*/
  "Anfang",                               /* Starting           -HTML*/
  "Ende",                                 /* Ending             -HTML*/
  "‹ber dieses Archiv",                /* About this archive -HTML*/
  "sortiert nach",            /* sorted by -HTML*/
  "Weitere Mail-Archive",                 /* Other mail archives -HTML*/
  "Nach Datum",                                /* By Date             -HTML*/
  "Neueste Nachrichten",                  /* Most recent messages-HTML*/
  "Autor",                                /* author              -HTML*/
  "Datum",                                /* date                -HTML*/
  "Themenstrang",                               /* thread              -HTML*/
  "Titel",                              /* subject             -HTML*/
  "f¸r Optionen",                      /* for options       -STDOUT*/
  "Schreibe Nachrichten in",              /* Writing messages to-STDOUT*/
  "Schreibe Datums-Index in",             /* Writing date index to-STDOUT*/
  "Schreibe Themenstrang-Index in",       /* Writing thread index to -STDOUT*/
  "Schreibe Titel-Index in",            /* Writing subject index to-STDOUT*/
  "Schreibe Autoren-Index in",            /* Writing author index to-STDOUT*/
  "Letztes Datum",                        /* Last message date   -HTML*/
  "Archiviert am",                       /* Archived on         -HTML*/
  "Kann Dateimodus nicht ‰ndern",      /* Can not chmod     -STDERR*/
  "Kann nicht schreiben",                 /* Could not write   -STDERR*/
  "N‰chste Nachricht",                 /* Next message        -HTML*/
  "Vorherige Nachricht",                  /* Previous message    -HTML*/
  "Vielleicht als Antwort auf",           /* Maybe in reply to   -HTML*/
  "Als Antwort auf",                      /* In reply to         -HTML*/
  "N‰chste zum Thema",           	  /* Next in thread      -HTML*/
  "Eventuelle Antwort",                   /* Maybe reply         -HTML*/
  "Nach Strang",                          /* By Thread           -HTML*/
  "Nach Titel",                         /* By Subject          -HTML*/
  "Nach Autor",                           /* By Author           -HTML*/
  "Kann das Verzeichnis nicht erstellen", /* Can not create directory -STDERR*/
  "Erstelle Verzeichnis",                 /* Creating directory -STDOUT*/
  "Konfiguration",                        /* Configuration Values -STDOUT*/
  "Pfad",                                 /* path              -STDOUT*/
  "Kein Speicher mehr!",                  /* Ran out of memory!-STDERR*/
  "Dieses Archiv wurde generiert von",    /* This archive was generated by-HTML*/
  "Elemente",                             /* Elements          -STDOUT*/
  "Keine Elemente",                       /* No Elements       -STDOUT*/
  "Kann E-Mail-Archiv nicht ˆffnen",     /* Cannot open mail archive */
  "Lese neue Kopfzeilen...",                  /* Reading new header...-STDOUT*/
  "Lade Mailbox",                         /* Loading mailbox      -STDOUT*/
  "Kodierung wird nicht unterst¸tzt, speichere so", 
                        /* encoding is not supported, stored as-is -HTML*/
  "Nachrichten",                          /* messages             -HTML*/
  "Version",                              /* Version            -STDOUT*/
  "Patchlevel",                           /* Patchlevel         -STDOUT*/
  "Dokus",                                /* Docs               -STDOUT*/
  "Befehls- und Kontrollvariablen",       /* Command and Control Variables-STDOUT*/
  "Ja",                                   /* Yes                -STDOUT*/
  "Nein",                                 /* No                 -STDOUT*/
  "‹ber das Archiv: nicht benutzt",    /* About the archive: not used-STDOUT */
  "Andere Archive: nicht benutzt",        /* Other archives: not used-STDOUT*/
  "Adresse nicht benutzt",                /* address not used        -STDOUT*/
  "Eingebautes <BODY> benutzt",           /* Builtin <BODY> statement used-STDOUT*/
  "Kann nicht gleichzeitig von Datei und Standardeingabe lesen.", 
  			   /* Cannot read from both file and stdin. -STDERR*/
  "Optionen",                             /* Options                -STDOUT*/
  "URL f¸r weitere Archive",              /* URL to other archives  -STDOUT*/
  "URL f¸r Informationen ¸ber Archiv",  
                                      /* URL to archive information -STDOUT*/
  "Konfigurationsdatei zum Einlesen", 
                                    /* Configuration file to read in -STDOUT*/
  "Das Verzeichnis, um die HTML-Dateien zu speichern", 
                              /* The directory to save HTML files in -STDOUT*/
  "Lese Nachrichten von der Standardeingabe",
                                /* Read messages from standard input -STDOUT*/
  "Wie soll das Archiv genannt werden",   
                                  /* What to name the output archive -STDOUT*/
  "Mail-Archive zum Einlesen",            /* Mail archive to read in -STDOUT*/
  "Zeige Zwischenbericht",                /* Show progress           -STDOUT*/
  "Zeigt nur die Konfigurationsvariablen", /* Show configuration variables only -STDOUT*/
  "Zeigt Version und beende",             /* Show version information and exit -STDOUT*/
  "Jeweils einen Artikel hinzuf¸gen",  /* Update archive by one article -STDOUT*/
  "Vorherige Nachrichten ¸berschreiben", /* Overwrite previous messages -STDOUT*/
  "Sprache angeben",                      /* Specify language to use -STDOUT*/
  "Anleitung",                                /* Usage -STDOUT*/
  "Die Sprache wird nicht unterst¸tzt",/* Language not supported  -STDERR*/
  "Nicht gesetzt",                        /* Not set               -STDOUT*/
  "Nicht benutzt",                        /* Not used              -STDOUT*/
  "Anhang in Datei gespeichert",           /* Created attachment file -STDOUT*/
  "Anhang",                          /* attachment           -HTML*/
  "Modus",                                 /* mode                 -STDOUT*/
  "Lese alte Kopfzeilen",                     /* Reading old headers  -STDOUT*/
  "",       /* for alignment only -STDOUT*/
  "FEHLER",                       /* ERROR                       -STDERR*/
  "Einsendeadresse f¸r die Liste", 
                           /* The submission address of the list-STDERR*/
  "Lese nur eine E-Mail aus der Eingabe",
  "Autor",                                /* author              -HTML*/
  "Datum",                                /* date                -HTML*/
  "Titel",                                /* subject             -HTML*/
  "Aktionsmˆglichkeiten",                 /* Mail actions (MA) header -HTML*/
  "Sende E-Mail mit neuen Titel",         /* MA New Message      -HTML*/
  "Antworte auf die E-Mail",              /* MA Reply            -HTML*/
  "Zusammenfassung der monatlichen Index-Dateien", 
  					  /* monthly             -HTML*/
  "Zusammenfassung der j‰hrlichen Index-Dateien",        
  					  /* yearly              -HTML*/
  "Lege GDBM-Zwischenspeicher f¸r Kopfzeilen an", 
  				/* Build a GDBM header cache   -STDOUT*/
  "GDBM-Zwischenspeicher-Option nicht eingebaut",  
  		     /* GDBM header cache option not build in  -STDERR*/
  "Erstelle Gdbm Index... ",    /* Creating gdbm index         -STDOUT*/
  "Kann Gdbm Datei nicht erstellen... ",  
  				/* Can't create gdbm index     -STDOUT*/
  "Pflege ein zus‰tzliches mbox-Archiv", /* Maintain an mbox archive -STDOUT*/
  "Kann nicht gleichzeitig von mbox lesen und schreiben.", 
  	       /* "Can't both read from and write to an mbox." -STDOUT*/
  "Anh‰nge-‹bersicht",                      /* Attachment view       -HTML*/
  "Nach Nachrichten mit Anhang",/* By messages with attachments -HTML*/
  "Schreibe Anhang-Index in",	/* Writing attachment index to -STDOUT*/
  "Bytes",		    /* file size, so far only for attachments  -HTML */
  "Kann symbolischen Verweis nicht erstellen.", 
  			     /* MSG_CANNOT_CREATE_SYMLINK     -STDOUT */
  "Kann Datei nicht entfernen", /* MSG_CANNOT_UNLINK          -STDOUT */
  "Vorheriges Verzeichnis",              /* MSG_PREV_DIRECTORY  -HTML */
  "N‰chstes Verzeichnis",                 /* MSG_NEXT_DIRECTORY  -HTML */
  "Verzeichnisliste",  /* MSG_FOLDERS_INDEX                     -HTML */
  "Diese Nachricht wurde aus dem Archiv entfernt",/* MSG_DELETED -HTML */
  "Diese Nachricht ist abgelaufen",              /* MSG_EXPIRED -HTML */
  "(gelˆschte Nachricht)",          /* MSG_DEL_SHORT            -HTML */
  "Urspr¸nglicher Text dieser Nachricht", /* MSG_TXT_VERSION    -HTML */
  "Diese Nachricht wurde herausgefiltert",  /* MSG_FILTERED_OUT -HTML */
  "Autor",                              /* MSG_FROM              -HTML*/
  "Erstelle haof XML Dateien",  /* Write hoaf XML files       -STDOUT */
  "Schreibe Hoaf in",           /* Writing haof to               -HTML*/
  "Diese Nachricht",                 /* This message -HTML */
  "Inhalt",                 /* Message body -HTML */
  "Antworten",                      /* Respond -HTML */
  "Weitere Mˆglichkeiten",                 /* More options -HTML */
  "Zugehˆrige Nachrichten",             /* Related messages -HTML */
  "Next",                         /* Next -HTML */
  "Previous",                     /* Previous -HTML */
  "Antworten",                      /* Replies -HTML */
  "Nachrichten aus dem gleichen Zeitraum, sortiert", /* Contemporary messages sorted -HTML */
  "Hilfe",                         /* Help  -HTML */
  "Message to which this message replies", /* In Reply To - HTML link*/
  "Next message in this discussion thread", /* Next message in thread - HTML link*/
  "Message sent in reply to this message", /* Replies to this message - HTML link*/
  "Nachrichten im gleichen Zeitraum, nach Datum", /* Contemporary messages by date */
  "Themenstr‰nge im gleichen Zeitraum", /* Contemporary messages by threads -  HTML */
  "Nachrichten im gleichen Zeitraum, nach Betreff", /* Contemporary messages by subject - HTML */
  "Nachrichten im gleichen Zeitraum, nach Autor", /* Contemporary messages by author - HTML*/
  "Next message in the list", /* Next message - HTML */
  "Previous message in the list", /* Previous message - HTML */
  "Contemporary messages by attachment", /* Contemporary messages by attachments - HTML*/
  "Navigation bar to upper levels", /* Navigation bar, upper levels - HTML*/
  "Navigation bar", /* Navigation bar - HTML*/
  "sortieren nach", /* Sort by - HTML*/
  "Weitere Zeitr‰ume", /* Other periods - HTML */
  "Sp‰ter", /* Next folder - HTML */
  "Im n‰chsten Zeitraum gespeicherte Nachrichten, sortiert nach Datum", /* Next folder, by date - HTML link */
  "Fr¸her", /* Previous folder - HTML */
  "Im vorigen Zeitraum gespeicherte Nachrichten, sortiert nach Datum", /* Previous folder, by date - HTML Link */
  "Liste aller Zeitr‰ume", /* List of all periods - HTML*/
  "Dieser Zeitraum", /* This period - HTML*/
  "Empfangen am", /* Received on - HTML */
  "Dazu", /* Nearby - HTML */
  "sortiert", /* Resorted - HTML */
  "in zeitlicher Reihenfolge", /* listed by date - HTML */
  "nach Themenstrang" , /* discussion threads - HTML */
  "nach Autor geordnet", /* listed by author - HTML */
  "nach Titel", /* listed by subject - HTML */
  "nach Anh‰ngen", /* by attachment - HTML */
  "Zeitraum", /* period - HTML */
  " bis ", /* to - HTML */
  "from", /* from - HTML */
  "on", /* on - HTML */
  "message archived in another list or period", /* unknown in reply to - HTML */
  "Diese Nachricht wurde aus dem Archiv entfernt", /* MSG_DELETED_OTHER -HTML */
  "Note: this message has been edited and differs from the originally archived copy.", /* MSG_EDITED -HTML */
  NULL,                              /* End Of Message Table - NOWHERE*/
};

/*
** Polish version of the language table (Polska wersja tabeli jÍzykowej)
** Translation by Adam T. Tkocz <adam@storm.pl>
*/

char *pl[] = {                      /* English */
  "Nowa wiadomo∂Ê",                 /* New Message        -HTML*/
  "Odpowiedz",                      /* Reply              -HTML*/
  "O tej li∂cie",                   /* About this list    -HTML*/
  "DÛ≥ strony",                     /* End of Messages    -HTML*/
  "GÛra strony",                    /* Start of Messages  -HTML*/
  "Wg daty",                        /* Date view          -HTML*/
  "Wg w±tkÛw",                      /* Thread view        -HTML*/
  "Wg tematÛw",                     /* Subject view       -HTML*/
  "Wg autorÛw",                     /* Author view        -HTML*/
  "Wg za≥±cznikÛw",                 /* Attachment view    -HTML*/
  "Wiadomo∂ci",                     /* Messages           -HTML*/
  "Pocz±tek",                       /* Starting           -HTML*/
  "Koniec",                         /* Ending             -HTML*/
  "O tym archiwum",                 /* About this archive -HTML*/
  "sortowane wg",                   /* Messages sorted by -HTML*/
  "Inne archiwa maili",             /* Other mail archives -HTML*/
  "Wg daty",                        /* By Date             -HTML*/
  "Naj∂wieøsze wiadomo∂ci",         /* Most recent messages-HTML*/
  "autora",                         /* author              -HTML*/
  "datay",                          /* date                -HTML*/
  "w±tku",                          /* thread              -HTML*/
  "tematu",                         /* subject             -HTML*/
  "Dla opcji",                      /* for options       -STDOUT*/
  "Zapisywanie wiadomo∂ci do",      /* Writing messages to-STDOUT*/
  "Zapisywanie indeksu dat do",     /* Writing date index to-STDOUT*/
  "Zapisywanie indeksu w±tkÛw do",  /* Writing thread index to -STDOUT*/
  "Zapisywanie indeksu tematÛw do", /* Writing subject index to-STDOUT*/
  "Zapisywanie indeksu autorÛw do", /* Writing author index to-STDOUT*/
  "Data ostatniej wiadomo∂ci",      /* Last message date   -HTML*/
  "Zarchiwizowane w",               /* Archived on         -HTML*/
  "Niedozwolone chmod",             /* Can not chmod     -STDERR*/
  "Nie moøna zapisaÊ",              /* Could not write   -STDERR*/
  "NastÍpna wiadomo∂Ê",             /* Next message        -HTML*/
  "Poprzednia wiadomo∂Ê",           /* Previous message    -HTML*/
  "Przypuszczalnie w odpowiedzi na",/* Maybe in reply to   -HTML*/
  "W odpowiedzi na",                /* In reply to         -HTML*/
  "NastÍpna w w±tku",               /* Next in thread      -HTML*/
  "Prawdopodobna odpowiedº",        /* Maybe reply         -HTML*/
  "Wg w±tku",                       /* By Thread           -HTML*/
  "Wg tematu",                      /* By Subject          -HTML*/
  "Wg autora",                      /* By Author           -HTML*/
  "Nie moøna stworzyÊ katalogu",    /* Can not create directory -STDERR*/
  "Tworzenie katalogu",             /* Creating directory -STDOUT*/
  "Parametry konfiguracji",         /* Configuration Values -STDOUT*/
  "∂cieøka",                        /* path              -STDOUT*/
  "Przekroczona pamiÍÊ!",           /* Ran out of memory!-STDERR*/
  "To archiwum zosta≥o wygenerowane przez",  /* This archive was generated by -HTML*/
  "Elementy",                       /* Elements          -STDOUT*/
  "Øadnych elementÛw",              /* No Elements       -STDOUT*/
  "Nie moøna otworzyÊ archiwum",    /* Cannot open mail archive */
  "Czytanie nowego nag≥Ûwka",       /* Reading new header... -STDOUT   */
  "£adowanie mailboxa",             /* Loading mailbox      -STDOUT  */
  "kodowanie nie jest obs≥ugiwane", /* encoding is not supported, stored as-is -HTML*/
  "wiadomo∂ci",                     /* messages             -HTML*/
  "Wersja",                         /* Version            -STDOUT*/
  "Wersja ≥aty",                    /* Patchlevel         -STDOUT*/
  "Dokumentacja",                   /* Docs               -STDOUT*/
  "Polecenia i zmienne",            /* Command and Control Variables-STDOUT*/
  "Tak",                            /* Yes                -STDOUT*/
  "Nie",                            /* No                 -STDOUT*/
  "O tym archiwum: nie uøywane",    /* About the archive: not used-STDOUT */
  "Inne archiwa: nie uøywane",      /* Other archives: not used-STDOUT*/
  "adres nie zosta≥ uøyty",         /* address not used -STDOUT*/
  "Wbudowane <BODY> zosta≥o uøyte", /* Builtin <BODY> statement used-STDOUT*/
  "Nie moøna odczytaÊ pliku ani stdin.", /* Cannot read from both file and stdin. -STDERR*/
  "Opcje",                          /* Options                -STDOUT*/
  "URL do innych arciwÛw",          /* URL to other archives  -STDOUT*/
  "URL do informacji o archiwum",   /* URL to archive information -STDOUT*/
  "Plik konfiguracyjny do wczytania", /* Configuration file to read in -STDOUT*/
  "Katalog do zapisu plikÛw HTML",  /* The directory to save HTML files in -STDOUT*/
  "Czytaj wiadomo∂ci z stdin",      /* Read messages from standard input -STDOUT*/
  "Podaj nazwÍ archiwum",           /* What to name the output archive -STDOUT*/
  "Archiwum do wczytania",          /* Mail archive to read in -STDOUT*/
  "Pokaø postÍpy",                  /* Show progress           -STDOUT*/
  "Pokaø tylko zmienne konfiguracyjne", /* Show configuration variables only -STDOUT*/
  "Pokaø wersjÍ i wyjdº",           /* Show version information and exit -STDOUT*/
  "Uzupelnij archiwum o jedn± wiadomo∂Ê", /* Update archive by one article -STDOUT*/
  "Zast±p poprzednia wiadomo∂Ê",    /* Overwrite previous messages -STDOUT*/
  "Wybierz uøywany jÍzyk",          /* Specify language to use     -STDOUT*/
  "Uøycie",                         /* Usage                       -STDOUT*/
  "JÍzyk nie jest wspierany",       /* Language not supported  -STDERR*/
  "Nie ustawione",                  /* Not set     -STDOUT*/
  "Nie uøywane",                    /* Not used  -STDOUT*/
  "Stworzono za≥±cznik",            /* Created attachment file     -STDOUT*/
  "za≥±cznik",                      /* attachment                    -HTML*/
  "tryb",                           /* mode                        -STDOUT*/
  "Czytanie starych nag≥ÛwkÛw",     /* Reading old headers      -STDOUT*/
  "",                               /* tylko do wyrÛwnania       -STDOUT*/
  "B£°D",                           /* ERROR                       -STDERR*/
  "Adres wypisuj±cy z listy",       /* The submission address of the list-STDERR*/
  "Czytaj tylko jedn± wiadomo∂Ê",
  "Autor",                          /* author              -HTML*/
  "Data",                           /* date                -HTML*/
  "Temat",                          /* subject             -HTML*/
  "Wybierz",                        /* Mail actions (MA) header -HTML*/
  "wy∂lij nowy temat",              /* MA New Message      -HTML*/
  "odpowiedz na t± wiadomo∂Ê",      /* MA Reply            -HTML*/
  "Zestawienie miesiÍcy",           /* monthly             -HTML*/
  "Zestawienie lat",                /* yearly              -HTML*/
  "UtwÛrz cache nag≥owkÛw GDBM",    /* Build a GDBM header cache -STDOUT*/
  "GDBM header cache option not build in",
  		      /* GDBM header cache option not build in  -STDERR*/
  "Tworzenie indeksu GDBM... ",     /* Creating gdbm index -STDOUT*/
  "Nie moøna utworzyÊ pliku GDBM... ",  /* Can't create gdbm index     -STDOUT*/
  "Zachowaj archiwum mbox",         /* Maintain an mbox archive    -STDOUT*/
  "Nie moøna odczytaÊ ani zapisaÊ mailboxa.", /* "Can't both read from and write to an mbox." -STDOUT*/
  "Wg za≥±cznikÛw",                  /* Attachment view -HTML*/
  "Wg wiadomo∂ci z za≥±cznikami",    /* By messages with attachments -HTML*/
  "Zapisywanie indeksu za≥±cznikÛw", /* Writing attachment index to -STDOUT*/
  "bajtÛw",                          /* file size, so far only for attachments  -HTML */
  "Nie moøna stworzyÊ aliasa",       /* MSG_CANNOT_CREATE_SYMLINK -STDOUT */
  "Nie moøna usun±Ê pliku",          /* MSG_CANNOT_UNLINK      -STDOUT */
  "Poprzedni katalog",               /* MSG_PREV_DIRECTORY  -HTML */
  "NastÍpny katalog",                /* MSG_NEXT_DIRECTORY    -HTML */
  "Lista KatalogÛw",                 /* MSG_FOLDERS_INDEX   -HTML */
  "Ta wiadomo∂Ê zosta≥a usuniÍta z archiwum", /* MSG_DELETED  -HTML */
  "Ta wiadomo∂Ê jest przedawniona",  /*MSG_EXPIRED -HTML */
  "(usuniÍta wiadomo∂Ê)",            /* MSG_DEL_SHORT     -HTML */
  "Tekst tej zawarto∂ci",            /* MSG_TXT_VERSION     -HTML */
  "Ta wiadomo∂Ê zosta≥a odfiltrowana",        /* MSG_FILTERED_OUT -HTML */
  "Autor",                          /* MSG_FROM              -HTML*/
  "Write haof XML files",       /* Write hoaf XML files       -STDOUT */
  "Writing haof to",            /* Writing haof to               -HTML*/
  "This message",                 /* This message -HTML */
  "Message body",                 /* Message body -HTML */
  "Respond",                      /* Respond -HTML */
  "More options",                 /* More options -HTML */
  "Related messages",             /* Related messages -HTML */
  "Next",                         /* Next -HTML */
  "Previous",                     /* Previous -HTML */
  "Replies",                      /* Replies -HTML */
  "Contemporary messages sorted", /* Contemporary messages sorted -HTML */
  "Help",                         /* Help  -HTML */
  "Message to which this message replies", /* In Reply To - HTML link*/
  "Next message in this discussion thread", /* Next message in thread - HTML link*/
  "Message sent in reply to this message", /* Replies to this message - HTML link*/
  "Contemporary messages by date", /* Contemporary messages by date */
  "Contemporary discussion threads", /* Contemporary messages by threads -  HTML */
  "Contemporary messages by subject", /* Contemporary messages by subject - HTML */
  "Contemporary messages by author", /* Contemporary messages by author - HTML*/
  "Next message in the list", /* Next message - HTML */
  "Previous message in the list", /* Previous message - HTML */
  "Contemporary messages by attachment", /* Contemporary messages by attachments - HTML*/
  "Navigation bar to upper levels", /* Navigation bar, upper levels - HTML*/
  "Navigation bar", /* Navigation bar - HTML*/
  "sort by", /* Sort by - HTML*/
  "Other periods", /* Other periods - HTML */
  "Next", /* Next folder - HTML */
  "Messages archived in the next period, sorted by date", /* Next folder, by date - HTML link */
  "Previous", /* Previous folder - HTML */
  "Messages archived in the previous period, sorted by date", /* Previous folder, by date - HTML Link */
  "List of all periods", /* List of all periods - HTML*/
  "This period", /* This period - HTML*/
  "Received on", /* Received on - HTML */
  "Nearby", /* Nearby - HTML */
  "re-sorted", /* Resorted - HTML */
  "listed by date", /* listed by date - HTML */
  "by discussion threads" , /* discussion threads - HTML */
  "listed by author", /* listed by author - HTML */
  "by subject", /* listed by subject - HTML */
  "by attachment", /* by attachment - HTML */
  "period", /* period - HTML */
  " to ", /* to - HTML */
  "from", /* from - HTML */
  "on", /* on - HTML */
  "message archived in another list or period", /* unknown in reply to - HTML */
  "Ta wiadomo∂Ê zosta≥a usuniÍta z archiwum", /* MSG_DELETED_OTHER  -HTML */
  "Note: this message has been edited and differs from the originally archived copy.", /* MSG_EDITED -HTML */
  NULL,                              /* End Of Message Table      - NOWHERE*/
};


/*
** English version of the language table
*/

char *en[] = {       /* English */
  "New Message",                 /* New Message        -HTML*/
  "Reply",                       /* Reply              -HTML*/
  "About this list",             /* About this list    -HTML*/
  "End of Messages",             /* End of Messages    -HTML*/
  "Start of Messages",           /* Start of Messages  -HTML*/
  "Date view",                   /* Date view          -HTML*/
  "Thread view",                 /* Thread view        -HTML*/
  "Subject view",                /* Subject view       -HTML*/
  "Author view",                 /* Author view        -HTML*/
  "Attachment view",             /* Attachment view    -HTML*/
  "Messages",                    /* Messages           -HTML*/
  "Starting",                    /* Starting           -HTML*/
  "Ending",                      /* Ending             -HTML*/
  "About this archive",          /* About this archive -HTML*/
  "sorted by",                   /* Messages sorted by -HTML*/
  "Other mail archives",         /* Other mail archives -HTML*/
  "by date",                     /* By Date             -HTML*/
  "Most recent messages",        /* Most recent messages-HTML*/
  "author",                      /* author              -HTML*/
  "date",                        /* date                -HTML*/
  "thread",                      /* thread              -HTML*/
  "subject",                     /* subject             -HTML*/
  "for options",                 /* for options       -STDOUT*/
  "Writing messages to",         /* Writing messages to-STDOUT*/
  "Writing date index to",       /* Writing date index to-STDOUT*/
  "Writing thread index to",     /* Writing thread index to -STDOUT*/
  "Writing subject index to",    /* Writing subject index to-STDOUT*/
  "Writing author index to",     /* Writing author index to-STDOUT*/
  "Last message date",           /* Last message date   -HTML*/
  "Archived on",                 /* Archived on         -HTML*/
  "Can not chmod",               /* Can not chmod     -STDERR*/
  "Could not write",             /* Could not write   -STDERR*/
  "Next message",                /* Next message        -HTML*/
  "Previous message",            /* Previous message    -HTML*/
  "Maybe in reply to",           /* Maybe in reply to   -HTML*/
  "In reply to",                 /* In reply to         -HTML*/
  "Next in thread",              /* Next in thread      -HTML*/
  "Maybe reply",                 /* Maybe reply         -HTML*/
  "by thread",                   /* By Thread           -HTML*/
  "by subject",                  /* By Subject          -HTML*/
  "by author",                   /* By Author           -HTML*/
  "Can not create directory",    /* Can not create directory -STDERR*/
  "Creating directory",          /* Creating directory -STDOUT*/
  "Configuration Values",        /* Configuration Values -STDOUT*/
  "path",                        /* path              -STDOUT*/
  "Ran out of memory!",          /* Ran out of memory!-STDERR*/
  "This archive was generated by",  /* This archive was generated by-HTML*/
  "Elements",                    /* Elements          -STDOUT*/
  "No Elements",                 /* No Elements       -STDOUT*/
  "Cannot open mail archive",    /* Cannot open mail archive */
  "Reading new header...",       /* Reading new header...-STDOUT   */
  "Loading mailbox",             /* Loading mailbox      -STDOUT   */
  "encoding is not supported, stored as-is", /* encoding is not supported, stored as-is -HTML*/
  "messages",                    /* messages             -HTML*/
  "Version",                     /* Version            -STDOUT*/
  "Patchlevel",                  /* Patchlevel         -STDOUT*/
  "Docs",                        /* Docs               -STDOUT*/
  "Command and Control Variables",       /* Command and Control Variables-STDOUT*/
  "Yes",                         /* Yes                -STDOUT*/
  "No",                          /* No                 -STDOUT*/
  "About the archive: not used", /* About the archive: not used-STDOUT */
  "Other archives: not used",    /* Other archives: not used-STDOUT*/
  "address not used",            /* address not used        -STDOUT*/
  "Builtin <BODY> statement used", /* Builtin <BODY> statement used-STDOUT*/
  "Cannot read from both file and stdin.", /* Cannot read from both file and stdin. -STDERR*/
  "Options",                     /* Options                -STDOUT*/
  "URL to other archives",       /* URL to other archives  -STDOUT*/
  "URL to archive information",  /* URL to archive information -STDOUT*/
  "Configuration file to read in", /* Configuration file to read in -STDOUT*/
  "The directory to save HTML files in", /* The directory to save HTML files in -STDOUT*/
  "Read messages from standard input",/* Read messages from standard input -STDOUT*/
  "What to name the output archive", /* What to name the output archive -STDOUT*/
  "Mail archive to read in",     /* Mail archive to read in -STDOUT*/
  "Show progress",               /* Show progress           -STDOUT*/
  "Show configuration variables only", /* Show configuration variables only -STDOUT*/
  "Show version information and exit", /* Show version information and exit -STDOUT*/
  "Update archive by one article", /* Update archive by one article -STDOUT*/
  "Overwrite previous messages", /* Overwrite previous messages -STDOUT*/
  "Specify language to use",     /* Specify language to use     -STDOUT*/
  "Usage",                       /* Usage                       -STDOUT*/
  "Language not supported",      /* Language not supported      -STDERR*/
  "Not set",                     /* Not set                     -STDOUT*/
  "Not used",                    /* Not used                    -STDOUT*/
  "Created attachment file",     /* Created attachment file     -STDOUT*/
  "attachment",                  /* attachment                    -HTML*/
  "mode",                        /* mode                        -STDOUT*/
  "Reading old headers",         /* Reading old headers         -STDOUT*/
  "", /* for alignment only       -STDOUT*/
  "ERROR",                       /* ERROR                       -STDERR*/
  "The submission address of the list", 
                           /* The submission address of the list-STDERR*/
  "Read only one mail from input",
  "Author",                      /* author              -HTML*/
  "Date",                        /* date                -HTML*/
  "Subject",                     /* subject             -HTML*/
  "Mail actions",                /* Mail actions (MA) header -HTML*/
  "mail a new topic",            /* MA New Message      -HTML*/
  "respond to this message",     /* MA Reply            -HTML*/
  "Summary of Monthly Index Files",       /* monthly             -HTML*/
  "Summary of Yearly Index Files",        /* yearly              -HTML*/
  "Build a GDBM header cache",   /* Build a GDBM header cache   -STDOUT*/
  "GDBM header cache option not build in",
  		      /* GDBM header cache option not build in  -STDERR*/
  "Creating gdbm index... ",     /* Creating gdbm index         -STDOUT*/
  "Can't create gdbm file... ",  /* Can't create gdbm index     -STDOUT*/
  "Maintain an mbox archive",    /* Maintain an mbox archive    -STDOUT*/
  "Can't both read from and write to an mbox.", /* "Can't both read from and write to an mbox." -STDOUT*/
  "Attachment view",			  /* Attachment view       -HTML*/
  "by messages with attachments",	  /* By messages with attachments -HTML*/
  "Writing attachment index to",	  /* Writing attachment index to -STDOUT*/
  "bytes",		    /* file size, so far only for attachments  -HTML */
  "Cannot create symbolic link", /* MSG_CANNOT_CREATE_SYMLINK     -STDOUT */
  "Cannot remove file", /* MSG_CANNOT_UNLINK                       -STDOUT */
  "Previous Folder",  /* MSG_PREV_DIRECTORY                     -HTML */
  "Next Folder",      /* MSG_NEXT_DIRECTORY                     -HTML */
  "List of Folders",  /* MSG_FOLDERS_INDEX                      -HTML */
  "This message has been deleted from the archive", /* MSG_DELETED -HTML */
  "This message has expired",                       /* MSG_EXPIRED -HTML */
  "(deleted message)", /* MSG_DEL_SHORT                            -HTML */
  "Original text of this message", /* MSG_TXT_VERSION                          -HTML */
  "This message has been filtered out",        /* MSG_FILTERED_OUT -HTML */
  "From",                       /* MSG_FROM                    	 -HTML*/
  "Write haof XML files",       /* Write hoaf XML files       -STDOUT */
  "Writing haof to",            /* Writing haof to               -HTML*/
  "This message",                 /* This message -HTML */
  "Message body",                 /* Message body -HTML */
  "Respond",                      /* Respond -HTML */
  "More options",                 /* More options -HTML */
  "Related messages",             /* Related messages -HTML */
  "Next",                         /* Next -HTML */
  "Previous",                     /* Previous -HTML */
  "Replies",                      /* Replies -HTML */
  "Contemporary messages sorted", /* Contemporary messages sorted -HTML */
  "Help",                         /* Help  -HTML */
  "Message to which this message replies", /* In Reply To - HTML link*/
  "Next message in this discussion thread", /* Next message in thread - HTML link*/
  "Message sent in reply to this message", /* Replies to this message - HTML link*/
  "Contemporary messages by date", /* Contemporary messages by date */
  "Contemporary discussion threads", /* Contemporary messages by threads -  HTML */
  "Contemporary messages by subject", /* Contemporary messages by subject - HTML */
  "Contemporary messages by author", /* Contemporary messages by author - HTML*/
  "Next message in the list", /* Next message - HTML */
  "Previous message in the list", /* Previous message - HTML */
  "Contemporary messages by attachment", /* Contemporary messages by attachments - HTML*/
  "Navigation bar to upper levels", /* Navigation bar, upper levels - HTML*/
  "Navigation bar", /* Navigation bar - HTML*/
  "sort by", /* Sort by - HTML*/
  "Other periods", /* Other periods - HTML */
  "Next", /* Next folder - HTML */
  "Messages archived in the next period, sorted by date", /* Next folder, by date - HTML link */
  "Previous", /* Previous folder - HTML */
  "Messages archived in the previous period, sorted by date", /* Previous folder, by date - HTML Link */
  "List of all periods", /* List of all periods - HTML*/
  "This period", /* This period - HTML*/
  "Received on", /* Received on - HTML */
  "Nearby", /* Nearby - HTML */
  "re-sorted", /* Resorted - HTML */
  "listed by date", /* listed by date - HTML */
  "by discussion threads" , /* discussion threads - HTML */
  "listed by author", /* listed by author - HTML */
  "by subject", /* listed by subject - HTML */
  "by attachment", /* by attachment - HTML */
  "period", /* period - HTML */
  " to ", /* to - HTML */
  "from", /* from - HTML */
  "on", /* on - HTML */
  "message archived in another list or period", /* unknown in reply to - HTML */
  "This message has been deleted from the archive", /* MSG_DELETED_OTHER -HTML */
  "Note: this message has been edited and differs from the originally archived copy.", /* MSG_EDITED -HTML */
  NULL,                         /* End Of Message Table      - NOWHERE*/
};

/*
** Spanish version of the language table
** Translation by Francisco Iacobelli <fiacobelli@ibersis.cl>.
*/

char *es[] = {       /* Espanol/Spanish */
  "Nuevo mensaje",                    /* New Message 		      - HTML  */
  "Responder",                        /* Reply  		      - HTML  */
  "Acerca de la lista",               /* About this list  	      - HTML  */
  "Fin de los Mensajes",              /* End of Messages  	      - HTML  */
  "Inicio de mensajes",               /* Start of Messages  	      - HTML  */
  "Por fecha",                        /* Date view  		      - HTML  */
  "Por conversaciÛn",         	      /* Thread view  		      - HTML  */
  "Por tema",                         /* Subject view  		      - HTML  */
  "Por autor",                        /* Author view  		      - HTML  */
  "Por adjuntos",                     /* Attachment view	      - HTML  */
  "Mensajes",                         /* Messages  		      - HTML  */
  "Inicio",                           /* Starting		      - HTML  */
  "Fin",                              /* Ending  		      - HTML  */
  "Acerca de este archivo",           /* About this archive           - HTML  */
  "Ordenados por",                    /* Messages sorted by           - HTML  */
  "Otros archivos de correo",         /* Other mail archives  	      - HTML  */
  "Por fecha",                        /* By Date  		      - HTML  */
  "⁄ltimos mensajes",                 /* Most recent messages 	      - HTML  */
  "autor",                            /* author 		      - HTML  */
  "fecha",                            /* date 			      - HTML  */
  "Hilo de conversaciÛn",             /* thread 		      - HTML  */
  "tema",                             /* subject 		      - HTML  */
  "para las opciones",                /* for options 	 	      - STDOUT*/
  "Escribiendo mensajes en",          /* Writing articles to 	      - STDOUT*/
  "Escribiendo Ìndice de fechas en",  /* Writing date index to        - STDOUT*/
  "Escribiendo Ìndice de conversaciones en",
  				      /* Writing thread index to      - STDOUT*/
  "Escribiendo Ìndice de temas en",   /* Writing subject index to     - STDOUT*/
  "Escribiendo Ìndice de autores en", /* Writing author index to      - STDOUT*/
  "Fecha del ˙ltimo mensaje",         /* Last message date 	      - HTML  */
  "Archivado el ",                    /* Archived on  		      - HTML  */
  "No se puede ejecutar chmod",       /* Can not chmod 		      - STDERR*/
  "No se pudo escribir",              /* Could not write 	      - STDERR*/
  "Siguiente mensaje",                /* Next message		      - HTML  */
  "Mensaje anterior",                 /* Previous message             - HTML  */
  "Posiblemente en respuesta a ",     /* Maybe in reply to  	      - HTML  */
  "En Respuesta a",                   /* In reply to  		      - HTML  */
  "Siguiente en conversaciÛn",        /* Next in thread 	      - HTML  */
  "Posible respuesta",                /* Maybe reply  		      - HTML  */
  "Por conversaciÛn",                 /* By Thread  		      - HTML  */
  "Por tema",                         /* By Subject  		      - HTML  */
  "Por autor",                        /* By Author  		      - HTML  */
  "No se puede crear el directorio",  /* Can not create directory     - STDERR*/
  "Creando el directorio",            /* Creating directory 	      - STDOUT*/
  "Par·metros de configuracÛn",       /* Configuration Values         - STDOUT*/
  "ruta",                             /* path 			      - STDOUT*/
  "°No hay suficiente memoria!",      /* Ran out of memory! 	      - STDERR*/
  "Este archivo fue generado por",    /* This archive was generated by- HTML  */
  "Elementos",                        /* Elements 		      - STDOUT*/
  "No existen elementos",             /* No Elements 		      - STDOUT*/
  "No se puede abrir el archivo de correo",
  				      /* Cannot open mail archive     - STDERR*/
  "Leyendo el encabezado nuevo...",   /* Reading new header... 	      - STDOUT*/
  "Cargando la casilla",              /* Loading mailbox 	      - STDOUT*/
  "codificaciÛn no soportada, grabado -tal cual-", 
	                     /* encoding is not supported, stored as-is - HTML*/

  "mensajes",                         /* articles */
  "VersiÛn",                          /* Version 		      - STDOUT*/
  "Parche No.",                       /* Patchlevel 		      - STDOUT*/
  "Docs",                             /* Docs 			      - STDOUT*/
  "Variables de comandos y control",  /* Command and Control Variables- STDOUT*/
  "Si",                               /* Yes			      - STDOUT*/
  "No",                               /* No 			      - STDOUT*/
  "Acerca del archivo: no utilizado", /* About the archive: not used  - STDOUT*/
  "Otros archivos: no utilizado",     /* Other archives: not used     - STDOUT*/
  "direcciÛn no utilizada",           /* address not used 	      - STDOUT*/
  "Utilizado <BODY> predefinido",   /* Builtin <BODY> statement used  - STDOUT*/
  "No se puede leer de archivo y de stdin a la vez.", 
                            /* Cannot read from both file and stdin.  - STDERR*/
  "Opciones",                         /* Options 		      - STDOUT*/
  "URL de otros archivos",            /* URL to other archives 	      - STDOUT*/
  "URL de informaciÛn de archivos",   /* URL to archive information   - STDOUT*/
  "Archivo de conf. para leer",     /* Configuration file to read in  - STDOUT*/
  "Directorio donde grabar archivos HTML", 
                              /* The directory to save HTML files in  - STDOUT*/
  "Leer mensajes de la entrada por defecto",   
                                /* Read messages from standard input  - STDOUT*/
  "Nombre del archivo de salida", /* What to name the output archive  - STDOUT*/
  "Archivo de correo para leer",      /* Mail archive to read in      - STDOUT*/
  "Mostrar progreso",                 /* Show progress 		      - STDOUT*/
  "Mostrar solamente variables de conf.",   
                                 /* Show configuration variables only - STDOUT*/
  "Mostrar informaciÛn de versiÛn y salir",   
				 /* Show version information and exit - STDOUT*/
  "Actualizar archivo con un mensaje",/*Update archive by one article - STDOUT*/
  "Sobreescribir mensajes anteriores",/* Overwrite previous messages  - STDOUT*/
  "Especifique idioma utilizar",      /* Specify language to use      - STDOUT*/
  "Modo de uso",                      /* Usage                        - STDOUT*/
  "Idioma no soportado",              /* Language not supported       - STDERR*/
  "No establecido",                   /* Not set                      - STDOUT*/
  "No utilizado",                     /* Not used                     - STDOUT*/
  "Archivo adjunto creado",           /* Created attachment file      - STDOUT*/
  "adjunto",                          /* attachment                   - HTML  */
  "modo",                             /* mode                         - STDOUT*/
  "Leyendo encabezados antiguos",     /* Reading old headers          - STDOUT*/
  "",                                 /* (for alignment only)         - STDOUT*/
  "ERROR",                            /* ERROR                        - STDERR*/
  "DirecciÛn de correo de la lista",  
  				/* The submission address of the list - STDERR*/
  "Leer sÛlo un mensaje de la entrada",
  			       /* Read only one mail from input		      */
  "Autor",                            /* author	 		      - HTML  */
  "Fecha",                            /* date 			      - HTML  */
  "Tema",                             /* subject		      - HTML  */
  "Cabecera MA (Mail actions)",       /* Mail actions (MA) header     - HTML  */
  "Enviar un nuevo tema",             /* MA New Message     	      - HTML  */
  "responder a este mensaje",         /* MA Reply       	      - HTML  */
  "Resumen de Ìndices mensuales",     /* monthly            	      - HTML  */
  "Resumen de Ìndices anuales",       /* yearly              	      - HTML  */
  "Costruir cabecera para cachÈ GDBM",/* Build a GDBM header cache    - STDOUT*/
  "Creando Ìndice gdbm... ",          /* Creating gdbm index          - STDOUT*/
  "No pudo crearse fichero gdbm... ", /* Can't create gdbm index      - STDOUT*/
  "Mantener un archivo mbox",         /* Maintain an mbox archive     - STDOUT*/
  "No se puede leer y escribir al tiempo de un fichero mbox.",
		      /* "Can't both read from and write to an mbox." - STDOUT*/
  "Por adjuntos",                     /* Attachment view  	      - HTML  */
  "Por mensajes con adjuntos",	      /* By messages with attachments - HTML  */
  "Escribiendo Ìndice de adjuntos en",/* Writing attachment index to  - STDOUT*/
  "bytes",		    /* file size, so far only for attachments - HTML  */
  "No pudo crearse enlace simbÛlico", /* MSG_CANNOT_CREATE_SYMLINK    - STDOUT*/
  "No puede borrarse fichero",        /* MSG_CANNOT_UNLINK            - STDOUT*/
  "Carpeta anterior",  		      /* MSG_PREV_DIRECTORY           - HTML  */
  "Siguiente carpeta",                /* MSG_NEXT_DIRECTORY           - HTML  */
  "Lista de carpetas",  	      /* MSG_FOLDERS_INDEX            - HTML  */
  "El mensaje ha sido borrado del archivo",
  				      /* MSG_DELETED 		      - HTML  */
  "El mensaje ha caducado",           /* MSG_EXPIRED 		      - HTML  */
  "(mensaje borrado)",		      /* MSG_DEL_SHORT                - HTML  */
  "Texto original del mensaje",       /* MSG_TXT_VERSION   	      - HTML  */
  "El mensaje ha sido filtrado",      /* MSG_FILTERED_OUT 	      - HTML  */
  "Autor",                            /* MSG_FROM 		      - HTML  */
  "Write haof XML files",       /* Write hoaf XML files       -STDOUT */
  "Writing haof to",            /* Writing haof to               -HTML*/
  "This message",                 /* This message -HTML */
  "Message body",                 /* Message body -HTML */
  "Respond",                      /* Respond -HTML */
  "More options",                 /* More options -HTML */
  "Related messages",             /* Related messages -HTML */
  "Next",                         /* Next -HTML */
  "Previous",                     /* Previous -HTML */
  "Replies",                      /* Replies -HTML */
  "Contemporary messages sorted", /* Contemporary messages sorted -HTML */
  "Help",                         /* Help  -HTML */
  "Message to which this message replies", /* In Reply To - HTML link*/
  "Next message in this discussion thread", /* Next message in thread - HTML link*/
  "Message sent in reply to this message", /* Replies to this message - HTML link*/
  "Contemporary messages by date", /* Contemporary messages by date */
  "Contemporary discussion threads", /* Contemporary messages by threads -  HTML */
  "Contemporary messages by subject", /* Contemporary messages by subject - HTML */
  "Contemporary messages by author", /* Contemporary messages by author - HTML*/
  "Next message in the list", /* Next message - HTML */
  "Previous message in the list", /* Previous message - HTML */
  "Contemporary messages by attachment", /* Contemporary messages by attachments - HTML*/
  "Navigation bar to upper levels", /* Navigation bar, upper levels - HTML*/
  "Navigation bar", /* Navigation bar - HTML*/
  "sort by", /* Sort by - HTML*/
  "Other periods", /* Other periods - HTML */
  "Next", /* Next folder - HTML */
  "Messages archived in the next period, sorted by date", /* Next folder, by date - HTML link */
  "Previous", /* Previous folder - HTML */
  "Messages archived in the previous period, sorted by date", /* Previous folder, by date - HTML Link */
  "List of all periods", /* List of all periods - HTML*/
  "This period", /* This period - HTML*/
  "Received on", /* Received on - HTML */
  "Nearby", /* Nearby - HTML */
  "re-sorted", /* Resorted - HTML */
  "listed by date", /* listed by date - HTML */
  "by discussion threads" , /* discussion threads - HTML */
  "listed by author", /* listed by author - HTML */
  "by subject", /* listed by subject - HTML */
  "by attachment", /* by attachment - HTML */
  "period", /* period - HTML */
  " to ", /* to - HTML */
  "from", /* from - HTML */
  "on", /* on - HTML */
  "message archived in another list or period", /* unknown in reply to - HTML */
  "El mensaje ha sido borrado del archivo",
  				      /* MSG_DELETED_OTHER - HTML  */
  "Note: this message has been edited and differs from the originally archived copy.", /* MSG_EDITED -HTML */
  NULL,                               /* End Of Message Table */
};

/*
** Brazilian Portuguese Version of the language table
** Translation by Hugo Cisneiros <hugo@devin.com.br>.
*/

char *pt[] = {                     /* Brazilian Portuguese */
  "Nova Mensagem",                    /* New Message        -HTML*/
  "Responder",                        /* Reply              -HTML*/
  "Sobre esta lista",                 /* About this list    -HTML*/
  "Fim das Mensagens",                /* End of Messages    -HTML*/
  "InÌcio das Mensagens",             /* Start of Messages  -HTML*/
  "Por Data",                         /* Date view          -HTML*/
  "Por TÛpico",                       /* Thread view        -HTML*/
  "Por Assunto",                      /* Subject view       -HTML*/
  "Por Autor",                        /* Author view        -HTML*/
  "Por Anexo",                        /* Attachment view    -HTML*/
  "Mensagens",                        /* Messages           -HTML*/
  "Iniciando",                        /* Starting           -HTML*/
  "Finalizando",                      /* Ending             -HTML*/
  "Sobre este arquivo",               /* About this archive -HTML*/
  "ordenado por",                     /* Messages sorted by -HTML*/
  "Outros arquivos de lista",         /* Other mail archives -HTML*/
  "Por Data",                         /* By Date             -HTML*/
  "Mensagens mais recentes",          /* Most recent messages-HTML*/
  "autor",                            /* author              -HTML*/
  "data",                             /* date                -HTML*/
  "tÛpico",                           /* thread              -HTML*/
  "assunto",                          /* subject             -HTML*/
  "para opÁıes",                      /* for options       -STDOUT*/
  "Escrevendo mensagens para",        /* Writing messages to-STDOUT*/
  "Escrevendo Ìndice por data para",     /* Writing date index to-STDOUT*/
  "Escrevendo Ìndice por tÛpico para",   /* Writing thread index to -STDOUT*/
  "Escrevendo Ìndice por assunto para",  /* Writing subject index to-STDOUT*/
  "Escrevendo Ìndice por autor para",    /* Writing author index to-STDOUT*/
  "Data da ˙ltima mensagem",          /* Last message date   -HTML*/
  "Arquivado em",                     /* Archived on         -HTML*/
  "N„o foi possÌvel utilizar o chmod",   /* Can not chmod     -STDERR*/
  "N„o foi possivel escrever",        /* Could not write   -STDERR*/
  "PrÛxima mensagem",                 /* Next message        -HTML*/
  "Mensagem anterior",                /* Previous message    -HTML*/
  "Talvez em resposta ‡",             /* Maybe in reply to   -HTML*/
  "Em resposta ·",                    /* In reply to         -HTML*/
  "Pøoxima no tÛpico",                /* Next in thread      -HTML*/
  "Talvez resposta",                  /* Maybe reply         -HTML*/
  "Por TÛpico",                       /* By Thread           -HTML*/
  "Por Assunto",                      /* By Subject          -HTML*/
  "Por Autor",                        /* By Author           -HTML*/
  "N„o foi possÌvel criar diretÛrio", /* Can not create directory -STDERR*/
  "Criando diretÛrio",                /* Creating directory -STDOUT*/
  "Configurando valores",             /* Configuration Values -STDOUT*/
  "caminho",                          /* path              -STDOUT*/
  "Faltou memÛria!",                  /* Ran out of memory!-STDERR*/
  "Este arquivo foi gerado por",      /* This archive was generated by-HTML*/
  "Elementos",                        /* Elements          -STDOUT*/
  "Sem Elementos",                    /* No Elements       -STDOUT*/
  "N„o foi possÌvel abrir arquivo de lista", /* Cannot open mail archive */
  "Lendo novo cabeÁalho...",          /* Reading new header...-STDOUT   */
  "Carregando caixa de e-mail",       /* Loading mailbox      -STDOUT   */
  "CodificaÁ„o n„o suportada, armazenando como est·",
                                      /* encoding is not supported, stored as-is -HTML*/
  "mensagens",                        /* messages             -HTML*/
  "Vers„o",                           /* Version            -STDOUT*/
  "NÌvel de Patch",                   /* Patchlevel         -STDOUT*/
  "DocumentaÁ„o",                     /* Docs               -STDOUT*/
  "Vari·veis de Comando e Controle",  /* Command and Control Variables-STDOUT*/
  "Sim",                              /* Yes                -STDOUT*/
  "N„o",                              /* No                 -STDOUT*/
  "Sobre este arquivo: n„o usado",    /* About the archive: not used-STDOUT */
  "Outros arquivos: n„o usado",       /* Other archives: not used-STDOUT*/
  "endereÁo n„o usado",               /* address not used        -STDOUT*/
  "Tag <BODY> integrada usada",       /* Builtin <BODY> statement used-STDOUT*/
  "N„o foi possÌvel ler de ambos arquivos e entrada padr„o.",
                                      /* Cannot read from both file and stdin. -STDERR*/
  "OpÁıes",                           /* Options                -STDOUT*/
  "URL para outros arquivos",         /* URL to other archives  -STDOUT*/
  "URL para informaÁ„o do arquivo",   /* URL to archive information -STDOUT*/
  "Arquivo de configuraÁ„o para ler", /* Configuration file to read in -STDOUT*/
  "O diretÛrio onde ser„o salvos os arquivos HTML",
                                      /* The directory to save HTML files in -STDOUT*/
  "Ler mensagens da entrada padr„o",  /* Read messages from standard input -STDOUT*/
  "Qual o nome do arquivo de lista de saÌda", /* What to name the output archive -STDOUT*/
  "Arquivo de Lista ‡ ser lido",      /* Mail archive to read in -STDOUT*/
  "Mostrar progresso",                /* Show progress           -STDOUT*/
  "Mostrar apenas vari·veis de configuraÁ„o", /* Show configuration variables only -STDOUT*/
  "Mostrar informaÁ„o de vers„o e sair",      /* Show version information and exit -STDOUT*/
  "Atualizar arquivo por um artigo",          /* Update archive by one article -STDOUT*/
  "Sobrescrever mensagens anteriores",        /* Overwrite previous messages -STDOUT*/
  "Especifique a lÌngua ‡ se usar",           /* Specify language to use     -STDOUT*/
  "Uso",                              /* Usage                       -STDOUT*/
  "LÌngua n„o suportada",             /* Language not supported      -STDERR*/
  "N„o configurado",                  /* Not set                     -STDOUT*/
  "N„o usado",                        /* Not used                    -STDOUT*/
  "Criado arquivo anexo",             /* Created attachment file     -STDOUT*/
  "anexo",                            /* attachment                    -HTML*/
  "modo",                             /* mode                        -STDOUT*/
  "Lendo cabeÁalhos antigos",         /* Reading old headers         -STDOUT*/
  "",                                 /* for alignment only       -STDOUT*/
  "ERRO",                             /* ERROR                       -STDERR*/
  "O endereÁo de submiss„o da lista", /* The submission address of the list-STDERR*/
  "Ler apenas um e-mail da entrada",  /* Read only one e-mail from mbox */
  "Autor",                            /* author              -HTML*/
  "Data",                             /* date                -HTML*/
  "Assunto",                          /* subject             -HTML*/
  "AÁıes de E-Mail",                  /* Mail actions (MA) header -HTML*/
  "Novo tÛpico de E-Mail",            /* MA New Message      -HTML*/
  "Responder ‡ esta mensagem",        /* MA Reply            -HTML*/
  "Sum·rio dos Arquivos Mensais de Õndice", /* monthly             -HTML*/
  "Sum·rio dos Arquivos Anuais de Õndice",  /* yearly              -HTML*/
  "Compilar cache de cabeÁalho GDBM",       /* Build a GDBM header cache   -STDOUT*/
  "OpÁ„o de cabeÁalho GDBM n„o compilada",  /* GDBM header cache option not build in  -STDERR*/
  "Criando Ìndice gdbm... ",                /* Creating gdbm index         -STDOUT*/
  "N„o foi possÌvel criar o arquivo gdbm... ",      /* Can't create gdbm index     -STDOUT*/
  "Manter um arquivo do tipo mbox",   /* Maintain an mbox archive    -STDOUT*/
  "N„o foi possÌvel ler e escrever para uma mbox.",
                                       /* "Can't both read from and write to an mbox." -STDOUT*/
  "Por Anexo",			       /* Attachment view       -HTML*/
  "Por mensagens com anexo",	       /* By messages with attachments -HTML*/
  "Escrevendo Ìndice de anexos para ", /* Writing attachment index to -STDOUT*/
  "bytes",		               /* file size, so far only for attachments  -HTML */
  "N„o foi possÌvel criar link simbÛlico", /* MSG_CANNOT_CREATE_SYMLINK     -STDOUT */
  "N„o foi possÌvel remover o arquivo",    /* MSG_CANNOT_UNLINK                       -STDOUT */
  "DiretÛrio Anterior",                /* MSG_PREV_DIRECTORY                     -HTML */
  "PrÛximo DiretÛrio",                 /* MSG_NEXT_DIRECTORY                     -HTML */
  "Lista de DiretÛrios",               /* MSG_FOLDERS_INDEX                      -HTML */
  "Esta mensagem foi removida do arquivo", /* MSG_DELETED -HTML */
  "Esta mensagem expirou",             /* MSG_EXPIRED -HTML */
  "(mensagem removida)",               /* MSG_DEL_SHORT                            -HTML */
  "Texto original desta mensagem",     /* MSG_TXT_VERSION                          -HTML */
  "Esta mensagem foi filtrada",        /* MSG_FILTERED_OUT -HTML */
  "De",                                /* MSG_FROM                    	 -HTML*/
  "Escrever arquivos XML haof",        /* Write hoaf XML files       -STDOUT */
  "Escrevendo haof em",                /* Writing haof to               -HTML*/
  "Esta mensagem",                     /* This message -HTML */
  "Corpo da mensagem",                 /* Message body -HTML */
  "Responder",                         /* Respond -HTML */
  "Mais opÁıes",                       /* More options -HTML */
  "Mensagens relacionadas",            /* Related messages -HTML */
  "PrÛximo",                           /* Next -HTML */
  "Anterior",                          /* Previous -HTML */
  "Respostas",                         /* Replies -HTML */
  "Mensagens contempor‚neas ordenadas", /* Contemporary messages sorted -HTML */
  "Ajuda",                             /* Help  -HTML */
  "Mensagens que esta mensagem responde",         /* In Reply To - HTML link*/
  "PrÛxima mensagem neste tÛpico de discuss„o",   /* Next message in thread - HTML link*/
  "Mensagem mandada em resposta ‡ esta mensagem", /* Replies to this message - HTML link*/
  "Mensagens contempor‚neas por data",    /* Contemporary messages by date */
  "Mensagens contempor‚neas por tÛpico",  /* Contemporary messages by threads -  HTML */
  "Mensagens contempor‚neas por assunto", /* Contemporary messages by subject - HTML */
  "Mensagens contempor‚neas por autor",   /* Contemporary messages by author - HTML*/
  "PrÛxima mensagem na lista",  /* Next message - HTML */
  "Mensagem anterior na lista", /* Previous message - HTML */
  "Mensagens contempor‚neas por anexo", /* Contemporary messages by attachments - HTML*/
  "Barra de navegaÁ„o para nÌveis superiores", /* Navigation bar, upper levels - HTML*/
  "Barra de navegaÁ„o",                /* Navigation bar - HTML*/
  "ordenar por",                       /* Sort by - HTML*/
  "Outros perÌodos",                   /* Other periods - HTML */
  "PrÛximo",                           /* Next folder - HTML */
  "Mensagens arquivadas no prÛximo perÌodo, ordenadas por data",
                                       /* Next folder, by date - HTML link */
  "Anterior",                          /* Previous folder - HTML */
  "Mensagens arquivadas no perÌodo anterior, ordenadas por data",
                                       /* Previous folder, by date - HTML Link */
  "Lista de todos os perÌodos",        /* List of all periods - HTML*/
  "Este perÌodo",                      /* This period - HTML*/
  "Recebida em",                       /* Received on - HTML */
  "Perto",                             /* Nearby - HTML */
  "re-organizado",                     /* Resorted - HTML */
  "listado por data",                  /* listed by date - HTML */
  "por tÛpicos de discuss„o" ,         /* discussion threads - HTML */
  "listado por autor",                 /* listed by author - HTML */
  "por assunto",                       /* listed by subject - HTML */
  "por anexo",                         /* by attachment - HTML */
  "perÌodo",                           /* period - HTML */
  " para ",                            /* to - HTML */
  "message archived in another list or period", /* unknown in reply to - HTML */
  "Esta mensagem foi removida do arquivo", /* MSG_DELETE_OTHER -HTML */
  "Note: this message has been edited and differs from the originally archived copy.", /* MSG_EDITED -HTML */
  NULL,                                /* End Of Message Table      - NOWHERE*/
};

/*
** Finnish version of the language table
*/

char *fi[] = {       /* Finnish */
  "L‰het‰ uusi viesti",              /* New Message        -HTML*/
  "Kommentoi",                             /* Reply              -HTML*/
  "Tietoa listasta",                       /* About this list    -HTML*/
  "Viestien loppuun",                      /* End of Messages    -HTML*/
  "Viestien alkuun",                       /* Start of Messages  -HTML*/
  "P‰iv‰ysn‰kym‰",             /* Date view          -HTML*/
  "Ketjun‰kym‰",                     /* Thread view        -HTML*/
  "Aihen‰kym‰",                      /* Subject view       -HTML*/
  "Kirjoittajan‰kym‰",               /* Author view        -HTML*/
  "Attachment view",                       /* Attachment view    -HTML*/
  "Viestit",                               /* Messages           -HTML*/
  "alkaa",                                 /* Starting           -HTML*/
  "loppuu",                                /* Ending             -HTML*/
  "Tietoa arkistosta",                     /* About this archive -HTML*/
  "lajiteltuna",                           /* Messages sorted by -HTML*/
  "Muita arkistoja",                       /* Other mail archives -HTML*/
  "P‰iv‰yksitt‰in",               /* By Date             -HTML*/
  "Tuoreimmat viestit",                    /* Most recent messages-HTML*/
  "kirjoittajittain",                      /* author              -HTML*/
  "p‰iv‰yksitt‰in",               /* date                -HTML*/
  "ketjuittain",                           /* thread              -HTML*/
  "aiheittain",                            /* subject             -HTML*/
  "asetuksiin",                            /* for options       -STDOUT*/
  "Kirjoitan viestej‰",                 /* Writing messages to-STDOUT*/
  "Kirjoitan hakemistoa p‰iv‰yksitt‰in", /* Writing date index to-STDOUT*/
  "Kirjoitan hakemistoa ketjuittain",      /* Writing thread index to -STDOUT*/
  "Kirjoitan hakemistoa aiheittain",       /* Writing subject index to-STDOUT*/
  "Kirjoitan hakemistoa kirjoittajittain", /* Writing author index to-STDOUT*/
  "Viimeinen viesti p‰iv‰tty",       /* Last message date   -HTML*/
  "Arkistoitu",                            /* Archived on         -HTML*/
  "Ei voi muuttaa oikeuksia ",             /* Can not chmod     -STDERR*/
  "Kirjoitus ep‰onnistui",              /* Could not write   -STDERR*/
  "Seuraava viesti",                       /* Next message        -HTML*/
  "Edellinen viesti",                      /* Previous message    -HTML*/
  "Ehk‰ kommentti viestiin",            /* Maybe in reply to   -HTML*/
  "Kommentti viestiin",                    /* In reply to         -HTML*/
  "Seuraava ketjussa",                     /* Next in thread      -HTML*/
  "Ehk‰ kommentti",                     /* Maybe reply         -HTML*/
  "Ketjuittain",                           /* By Thread           -HTML*/
  "Aiheittain",                            /* By Subject          -HTML*/
  "Kirjoittajittain",                      /* By Author           -HTML*/
  "Ei voi luoda hakemistoa",               /* Can not create directory -STDERR*/
  "Luon hakemistoa",                       /* Creating directory -STDOUT*/
  "Asetukset",                             /* Configuration Values -STDOUT*/
  "polku",                                 /* path              -STDOUT*/
  "Muisti loppui!",                        /* Ran out of memory!-STDERR*/
  "T‰m‰n arkiston loi",            /* This archive was generated by-HTML*/
  "Osaset",                                /* Elements          -STDOUT*/
  "Ei osasia",                             /* No Elements       -STDOUT*/
  "Ei voi avata postiarkistoa",            /* Cannot open mail archive */
  "Luen uusia otsikkorivej‰...",        /* Reading new header...-STDOUT   */
  "Lataan postilaatikkoa",                 /* Loading mailbox      -STDOUT   */
  "koodaus ei tuettu, talletettu sellaisenaan", /* encoding is not supported, stored as-is -HTML*/
  "viesti‰",                            /* messages             -HTML*/
  "Versio",                                /* Version            -STDOUT*/
  "Patchlevel",                            /* Patchlevel         -STDOUT*/
  "Ohjeet",                                /* Docs               -STDOUT*/
  "Komento- ja ohjausmuuttujat",       /* Command and Control Variables-STDOUT*/
  "Kyll‰",                              /* Yes                -STDOUT*/
  "Ei",                                    /* No                 -STDOUT*/
  "Tietoa arkistosta: ei k‰ytˆss‰", /* About the archive: not used-STDOUT */
  "Muut arkistot: ei k‰ytˆss‰",   /* Other archives: not used-STDOUT*/
  "osoite ei k‰ytˆss‰",           /* address not used        -STDOUT*/
  "K‰ytet‰‰n oletus-<BODY>-rakennetta", /* Builtin <BODY> statement used-STDOUT*/
  "Ei voi lukea sek‰ tiedostosta ett‰ syˆttˆvirrasta (stdin).", /* Cannot read from both file and stdin. -STDERR*/
  "Asetukset",                             /* Options                -STDOUT*/
  "URL muihin arkistoihin",                /* URL to other archives  -STDOUT*/
  "URL arkiston tietoihin",              /* URL to archive information -STDOUT*/
  "Asetustiedosto",                   /* Configuration file to read in -STDOUT*/
  "HTML-tiedostojen talletushakemisto", /* The directory to save HTML files in -STDOUT*/
  "Lue viestit syˆttˆvirrasta (stdin)",/* Read messages from standard input -STDOUT*/
  "Talletettavan arkiston nimi",   /* What to name the output archive -STDOUT*/
  "Luettava postiarkisto",                /* Mail archive to read in -STDOUT*/
  "N‰yt‰ eteneminen",               /* Show progress           -STDOUT*/
  "N‰yt‰ vain asetusmuuttujat", /* Show configuration variables only -STDOUT*/
  "N‰yt‰ versiotieto ja lopeta ohjelma", /* Show version information and exit -STDOUT*/
  "P‰ivit‰ arkistoa yhdell‰ viestill‰", /* Update archive by one article -STDOUT*/
  "Korvaa aiemmat viestit", /* Overwrite previous messages -STDOUT*/
  "M‰‰rit‰ k‰ytett‰v‰ kieli",     /* Specify language to use     -STDOUT*/
  "K‰yttˆohje",                    /* Usage                   -STDOUT*/
  "Kieli ei tuettu",                     /* Language not supported  -STDERR*/
  "Ei asetettu",                         /* Not set                 -STDOUT*/
  "Ei k‰ytˆss‰",                /* Not used                -STDOUT*/
  "Luotu liitetiedosto",                 /* Created attachment file -STDOUT*/
  "liite",                               /* attachment              -HTML*/
  "suojaus",                             /* mode                    -STDOUT*/
  "Luen vanhoja otsikkorivej‰",       /* Reading old headers     -STDOUT*/
  "",                                    /* for alignment only      -STDOUT*/
  "VIRHE",                               /* ERROR                   -STDERR*/
  "Listan l‰hetysosoite",   /* The submission address of the list-STDERR*/
  "Lue vain yksi viesti",
  "Kirjoittajan mukaan",                 /* author                    -HTML*/
  "P‰iv‰yksen mukaan",             /* date                      -HTML*/
  "Aiheen mukaan",                       /* subject                   -HTML*/
  "Mail actions",                    /* Mail actions (MA) header -HTML*/
  "mail a new topic",                /* MA New Message      -HTML*/
  "respond to this message",         /* MA Reply            -HTML*/
  "Summary of Monthly Index Files",      /* monthly             -HTML*/
  "Summary of Yearly Index Files",       /* yearly              -HTML*/
  "Build a GDBM header cache",   /* Build a GDBM header cache   -STDOUT*/
  "GDBM header cache option not build in",
   		      /* GDBM header cache option not build in  -STDERR*/
  "Creating gdbm index... ",     /* Creating gdbm index         -STDOUT*/
  "Can't create gdbm file... ",  /* Can't create gdbm index     -STDOUT*/
  "Maintain an mbox archive",    /* Maintain an mbox archive    -STDOUT*/
  "Can't both read from and write to an mbox.", /* "Can't both read from and write to an mbox." -STDOUT*/
  "Attachment view",                      /* Attachment view  -HTML*/
  "By messages with attachments",	  /* By messages with attachments -HTML*/
  "Writing attachment index to",	  /* Writing attachment index to -STDOUT*/
  "bytes",		    /* file size, so far only for attachments  -HTML */
  "Cannot create symbolic link", /* MSG_CANNOT_CREATE_SYMLINK     -STDOUT */
  "Cannot remove file", /* MSG_CANNOT_UNLINK                       -STDOUT */
  "Previous Folder",  /* MSG_PREV_DIRECTORY                     -HTML */
  "Next Folder",      /* MSG_NEXT_DIRECTORY                     -HTML */
  "List of Folders",  /* MSG_FOLDERS_INDEX                      -HTML */
  "This message has been deleted from the archive", /* MSG_DELETED -HTML */
  "This message has expired",                       /* MSG_EXPIRED -HTML */
  "(deleted message)", /* MSG_DEL_SHORT                            -HTML */
  "Original text of this message", /* MSG_TXT_VERSION                          -HTML */
  "This message has been filtered out",        /* MSG_FILTERED_OUT -HTML */
  "Kirjoittajan mukaan",                 /* MSG_FROM                  -HTML*/
  "Write haof XML files",       /* Write hoaf XML files       -STDOUT */
  "Writing haof to",            /* Writing haof to               -HTML*/
  "This message",                 /* This message -HTML */
  "Message body",                 /* Message body -HTML */
  "Respond",                      /* Respond -HTML */
  "More options",                 /* More options -HTML */
  "Related messages",             /* Related messages -HTML */
  "Next",                         /* Next -HTML */
  "Previous",                     /* Previous -HTML */
  "Replies",                      /* Replies -HTML */
  "Contemporary messages sorted", /* Contemporary messages sorted -HTML */
  "Help",                         /* Help  -HTML */
  "Message to which this message replies", /* In Reply To - HTML link*/
  "Next message in this discussion thread", /* Next message in thread - HTML link*/
  "Message sent in reply to this message", /* Replies to this message - HTML link*/
  "Contemporary messages by date", /* Contemporary messages by date */
  "Contemporary discussion threads", /* Contemporary messages by threads -  HTML */
  "Contemporary messages by subject", /* Contemporary messages by subject - HTML */
  "Contemporary messages by author", /* Contemporary messages by author - HTML*/
  "Next message in the list", /* Next message - HTML */
  "Previous message in the list", /* Previous message - HTML */
  "Contemporary messages by attachment", /* Contemporary messages by attachments - HTML*/
  "Navigation bar to upper levels", /* Navigation bar, upper levels - HTML*/
  "Navigation bar", /* Navigation bar - HTML*/
  "sort by", /* Sort by - HTML*/
  "Other periods", /* Other periods - HTML */
  "Next", /* Next folder - HTML */
  "Messages archived in the next period, sorted by date", /* Next folder, by date - HTML link */
  "Previous", /* Previous folder - HTML */
  "Messages archived in the previous period, sorted by date", /* Previous folder, by date - HTML Link */
  "List of all periods", /* List of all periods - HTML*/
  "This period", /* This period - HTML*/
  "Received on", /* Received on - HTML */
  "Nearby", /* Nearby - HTML */
  "re-sorted", /* Resorted - HTML */
  "listed by date", /* listed by date - HTML */
  "by discussion threads" , /* discussion threads - HTML */
  "listed by author", /* listed by author - HTML */
  "by subject", /* listed by subject - HTML */
  "by attachment", /* by attachment - HTML */
  "period", /* period - HTML */
  " to ", /* to - HTML */
  "from", /* from - HTML */
  "on", /* on - HTML */
  "message archived in another list or period", /* unknown in reply to - HTML */
  "This message has been deleted from the archive", /* MSG_DELETED_OTHER -HTML */
  "Note: this message has been edited and differs from the originally archived copy.", /* MSG_EDITED -HTML */
  NULL,                              /* End Of Message Table      - NOWHERE*/
};


/*
** Italian version of the language table
** Gabriele Bartolini <gbartolini@prato.linux.it>, Prato Linux User Group, Italia
** Marco Nenciarini <mnencia@prato.linux.it>, Prato Linux User Group, Italia
**
*/

char *it[] = {       /* Italian */
  "Nuovo Messaggio",                 /* New Message        -HTML*/
  "Rispondi",                       /* Reply              -HTML*/
  "Riguardo questa lista",             /* About this list    -HTML*/
  "Fine dei messaggi",             /* End of Messages    -HTML*/
  "Inizio dei messaggi",           /* Start of Messages  -HTML*/
  "Visualizza per data",                   /* Date view          -HTML*/
  "Visualizza per discussione",                 /* Thread view        -HTML*/
  "Visualizza per oggetto",                /* Subject view       -HTML*/
  "Visualizza per autore",                 /* Author view        -HTML*/
  "Altri gruppi",                /* Other groups       -HTML*/
  "Messaggi",                    /* Messages           -HTML*/
  "Inizio",                    /* Starting           -HTML*/
  "Fine",                      /* Ending             -HTML*/
  "Riguardo questo archivio",          /* About this archive -HTML*/
  "ordinato per",                   /* Messages sorted by -HTML*/
  "Altri archivi di posta",         /* Other mail archives -HTML*/
  "Per data",                     /* By Date             -HTML*/
  "Messaggi pi˘ recenti",        /* Most recent messages-HTML*/
  "autore",                      /* author              -HTML*/
  "data",                        /* date                -HTML*/
  "discussione",                      /* thread              -HTML*/
  "oggetto",                     /* subject             -HTML*/
  "per opzioni",                 /* for options       -STDOUT*/
  "Scrittura messaggi su",         /* Writing messages to-STDOUT*/
  "Scrittura indice delle date su",       /* Writing date index to-STDOUT*/
  "Scrittura indice delle discussioni su",     /* Writing thread index to -STDOUT*/
  "Scrittura indice degli oggetti su",    /* Writing subject index to-STDOUT*/
  "Scrittura indice degli autori su",     /* Writing author index to-STDOUT*/
  "Data dell'ultimo messaggio",           /* Last message date   -HTML*/
  "Archiviato il",                 /* Archived on         -HTML*/
  "Impossibile eseguire 'chmod'",               /* Can not chmod     -STDERR*/
  "Impossibile scrivere",             /* Could not write   -STDERR*/
  "Prossimo messaggio",                /* Next message        -HTML*/
  "Messaggio precedente",            /* Previous message    -HTML*/
  "Forse in risposta a",           /* Maybe in reply to   -HTML*/
  "In risposta a",                 /* In reply to         -HTML*/
  "Prossimo nella discussione",              /* Next in thread      -HTML*/
  "Forse risposta",                 /* Maybe reply         -HTML*/
  "Per discussione",                   /* By Thread           -HTML*/
  "Per oggetto",                  /* By Subject          -HTML*/
  "Per autore",                   /* By Author           -HTML*/
  "Impossibile creare la directory",    /* Can not create directory -STDERR*/
  "Creazione directory",          /* Creating directory -STDOUT*/
  "Valori di configurazione",        /* Configuration Values -STDOUT*/
  "percorso",                        /* path              -STDOUT*/
  "Memoria insufficiente!",          /* Ran out of memory!-STDERR*/
  "Questo archivio Ë stato generato da",  /* This archive was generated by-HTML*/
  "Elementi",                    /* Elements          -STDOUT*/
  "Nessun elemento",                 /* No Elements       -STDOUT*/
  "Impossibile aprire archivio di posta",    /* Cannot open mail archive */
  "Lettura nuove intestazioni",       /* Reading new header...-STDOUT   */
  "Caricamento casella di posta",             /* Loading mailbox      -STDOUT   */
  "Codifica non supportata, memorizzato 'cosÏ come Ë'", /* encoding is not supported, stored as-is -HTML*/
  "messaggi",                    /* messages             -HTML*/
  "Versione",                     /* Version            -STDOUT*/
  "Patchlevel",                  /* Patchlevel         -STDOUT*/
  "Documenti",                        /* Docs               -STDOUT*/
  "Variabili di comando e controllo",       /* Command and Control Variables-STDOUT*/
  "SÏ",                         /* Yes                -STDOUT*/
  "No",                          /* No                 -STDOUT*/
  "Riguardo l'archivio: non usato", /* About the archive: not used-STDOUT */
  "Altri archivi: non usati",    /* Other archives: not used-STDOUT*/
  "indirizzo inutilizzato",            /* address not used        -STDOUT*/
  "Istruzione <BODY> di default utilizzata", /* Builtin <BODY> statement used-STDOUT*/
  "Impossibile leggere sia dal file che dal canale stdin", /* Cannot read from both file and stdin. -STDERR*/
  "Opzioni",                     /* Options                -STDOUT*/
  "URL per gli altri archivi",       /* URL to other archives  -STDOUT*/
  "URL per le informazioni sull'archivio",  /* URL to archive information -STDOUT*/
  "File di configurazione", /* Configuration file to read in -STDOUT*/
  "Directory in cui salvare i file HTML", /* The directory to save HTML files in -STDOUT*/
  "Lettura messaggi dal canale stdin",/* Read messages from standard input -STDOUT*/
  "Come nominare l'archivio in output", /* What to name the output archive -STDOUT*/
  "Archivio di posta in lettura",     /* Mail archive to read in -STDOUT*/
  "Mostra progresso",               /* Show progress           -STDOUT*/
  "Mostra solo le variabili di configurazione", /* Show configuration variables only -STDOUT*/
  "Mostra le informazioni sulla versione ed esci", /* Show version information and exit -STDOUT*/
  "Aggiorna l'archivio per un articolo", /* Update archive by one article -STDOUT*/
  "Sovrascrivi messaggi precedenti", /* Overwrite previous messages -STDOUT*/
  "Specifica il linguaggio da usare",     /* Specify language to use     -STDOUT*/
  "Utilizzo",                       /* Usage                       -STDOUT*/
  "Linguaggio non supportato",      /* Language not supported      -STDERR*/
  "Non impostato",                     /* Not set                     -STDOUT*/
  "Non utilizzato",                    /* Not used                    -STDOUT*/
  "Creato il file per l'allegato",     /* Created attachment file     -STDOUT*/
  "allegato",                  /* attachment                    -HTML*/
  "modalit‡",                        /* mode                        -STDOUT*/
  "Lettura intestazioni vecchie",         /* Reading old headers         -STDOUT*/
  "", /* for alignment only       -STDOUT*/
  "ERRORE",                       /* ERROR                       -STDERR*/
  "Indirizzo per l'iscrizione alla lista",
                           /* The submission address of the list-STDERR*/
  "Legge una mail soltanto dall'input",
  "Autore",                      /* author              -HTML*/
  "Data",                        /* date                -HTML*/
  "Oggetto",                     /* subject             -HTML*/
  "Azioni di posta",                /* Mail actions (MA) header -HTML*/
  "spedisci un nuovo argomento",            /* MA New Message      -HTML*/
  "rispondi a questo messaggio",     /* MA Reply            -HTML*/
  "Riepilogo dei file di indice mensili",       /* monthly             -HTML*/
  "Riepilogo dei file di indice annuali",        /* yearly              -HTML*/
  "Costruisci una cache degli header in GDBM",   /* Build a GDBM header cache   -STDOUT*/
  "Creazione dell'indice gdbm ... ",     /* Creating gdbm index         -STDOUT*/
  "Impossibile creare l'indice gdbm ... ",  /* Can't create gdbm index     -STDOUT*/
  "Mantieni un archivio di tipo mbox",    /* Maintain an mbox archive    -STDOUT*/
  "Impossibile leggere da/scrivere su mbox.", /* "Can't both read from and write to an mbox." -STDOUT*/
  "Visualizza allegato",			  /* Attachment view       -HTML*/
  "Per messaggi con allegato",	  /* By messages with attachments -HTML*/
  "Scrittura indice allegato su",	  /* Writing attachment index to -STDOUT*/
  "bytes",		    /* file size, so far only for attachments  -HTML */
  "Impossibile creare link simbolico", /* MSG_CANNOT_CREATE_SYMLINK     -STDOUT */
  "Impossibile eliminare file", /* MSG_CANNOT_UNLINK                       -STDOUT */
  "Cartella precedente",  /* MSG_PREV_DIRECTORY                     -HTML */
  "Prossima cartella",      /* MSG_NEXT_DIRECTORY                     -HTML */
  "Lista delle cartelle",  /* MSG_FOLDERS_INDEX                      -HTML */
  "Questo messaggio Ë stato cancellato dall'archivio", /* MSG_DELETED -HTML */
  "Il messaggio Ë scaduto",                       /* MSG_EXPIRED -HTML */
  "(messaggio cancellato)", /* MSG_DEL_SHORT                            -HTML */
  "Testo originale di questo messaggio", /* MSG_TXT_VERSION                          -HTML */
  "Questo messaggio Ë stato filtrato",        /* MSG_FILTERED_OUT -HTML */
  "From",                       /* MSG_FROM                    	 -HTML*/
  "Write haof XML files",       /* Write hoaf XML files       -STDOUT */
  "Writing haof to",            /* Writing haof to               -HTML*/
  "This message",                 /* This message -HTML */
  "Message body",                 /* Message body -HTML */
  "Respond",                      /* Respond -HTML */
  "More options",                 /* More options -HTML */
  "Related messages",             /* Related messages -HTML */
  "Next",                         /* Next -HTML */
  "Previous",                     /* Previous -HTML */
  "Replies",                      /* Replies -HTML */
  "Contemporary messages sorted", /* Contemporary messages sorted -HTML */
  "Help",                         /* Help  -HTML */
  "Message to which this message replies", /* In Reply To - HTML link*/
  "Next message in this discussion thread", /* Next message in thread - HTML link*/
  "Message sent in reply to this message", /* Replies to this message - HTML link*/
  "Contemporary messages by date", /* Contemporary messages by date */
  "Contemporary discussion threads", /* Contemporary messages by threads -  HTML */
  "Contemporary messages by subject", /* Contemporary messages by subject - HTML */
  "Contemporary messages by author", /* Contemporary messages by author - HTML*/
  "Next message in the list", /* Next message - HTML */
  "Previous message in the list", /* Previous message - HTML */
  "Contemporary messages by attachment", /* Contemporary messages by attachments - HTML*/
  "Navigation bar to upper levels", /* Navigation bar, upper levels - HTML*/
  "Navigation bar", /* Navigation bar - HTML*/
  "sort by", /* Sort by - HTML*/
  "Other periods", /* Other periods - HTML */
  "Next", /* Next folder - HTML */
  "Messages archived in the next period, sorted by date", /* Next folder, by date - HTML link */
  "Previous", /* Previous folder - HTML */
  "Messages archived in the previous period, sorted by date", /* Previous folder, by date - HTML Link */
  "List of all periods", /* List of all periods - HTML*/
  "This period", /* This period - HTML*/
  "Received on", /* Received on - HTML */
  "Nearby", /* Nearby - HTML */
  "re-sorted", /* Resorted - HTML */
  "listed by date", /* listed by date - HTML */
  "by discussion threads" , /* discussion threads - HTML */
  "listed by author", /* listed by author - HTML */
  "by subject", /* listed by subject - HTML */
  "by attachment", /* by attachment - HTML */
  "period", /* period - HTML */
  " to ", /* to - HTML */
  "from", /* from - HTML */
  "on", /* on - HTML */
  "message archived in another list or period", /* unknown in reply to - HTML */
  "Questo messaggio Ë stato cancellato dall'archivio", /* MSG_DELETED_OTHER -HTML */
  "Note: this message has been edited and differs from the originally archived copy.", /* MSG_EDITED -HTML */
  NULL,                          /* End Of Message Table      - NOWHERE*/
};

/*
** French version of the language table
** Translation by Nicolas Noble <pixels@chez.com>
*/

char *fr[] = {       /* French */
  "Nouveau Message",                       /* New Message        -HTML*/
  "R&eacute;ponse",                        /* Reply              -HTML*/
  "A propos de cette liste",               /* About this list    -HTML*/
  "Fin des Messages",                      /* End of Messages    -HTML*/
  "D&eacute;but des Messages",             /* Start of Messages  -HTML*/
  "Liste par Date",                        /* Date view          -HTML*/
  "Liste par Th&egrave;me",                /* Thread view        -HTML*/
  "Liste par Sujet",                       /* Subject view       -HTML*/
  "Liste par Auteur",                      /* Author view        -HTML*/
  "Liste par fichier attachÈs",            /* Attachment view    -HTML*/
  "Messages",                              /* Messages           -HTML*/
  "D&eacute;but",                          /* Starting           -HTML*/
  "Fin",                                   /* Ending             -HTML*/
  "A propos de cette archive",             /* About this archive -HTML*/
  ", class&eacute;s par",                  /* Messages sorted by -HTML*/
  "Autres archives",                       /* Other mail archives -HTML*/
  "par Date",                              /* By Date             -HTML*/
  "Messages les plus r&eacute;cents",      /* Most recent messages-HTML*/
  "auteur",                                /* author              -HTML*/
  "date",                                  /* date                -HTML*/
  "th&egrave;me",                          /* thread              -HTML*/
  "sujet",                                 /* subject             -HTML*/
  "pour les options",                      /* for options       -STDOUT*/
  "Ecriture des messages vers",            /* Writing messages to-STDOUT*/
  "Ecriture de l'index 'date' dans",       /* Writing date index to-STDOUT*/
  "Ecriture de l'index 'thËme' dans",      /* Writing thread index to -STDOUT*/
  "Ecriture de l'index 'sujet' dans",      /* Writing subject index to-STDOUT*/
  "Ecriture de l'index 'auteur' dans",     /* Writing author index to-STDOUT*/
  "Date du dernier message",               /* Last message date   -HTML*/
  "Archiv&eacute; le",                     /* Archived on         -HTML*/
  "Ne peut effectuer de chmod",            /* Can not chmod     -STDERR*/
  "Ne peut Ècrire",                        /* Could not write   -STDERR*/
  "Message suivant",                       /* Next message        -HTML*/
  "Message pr&eacute;c&eacute;dent",       /* Previous message    -HTML*/
  "Sans doute en r&eacute;ponse &agrave;", /* Maybe in reply to   -HTML*/
  "En r&eacute;ponse &agrave;",             /* In reply to         -HTML*/
  "Suivant dans le th&egrave;me",          /* Next in thread      -HTML*/
  "Sans doute en r&eacute;ponse",          /* Maybe reply         -HTML*/
  "par Th&eagrave;me",                      /* By Thread           -HTML*/
  "par Sujet",                             /* By Subject          -HTML*/
  "par Auteur",                            /* By Author           -HTML*/
  "Ne peut crÈer le rÈpertoire",           /* Can not create directory -STDERR*/
  "CrÈation du rÈpertoire",                /* Creating directory -STDOUT*/
  "Valeurs configurÈes",                   /* Configuration Values -STDOUT*/
  "chemin",                                /* path              -STDOUT*/
  "Plus assez de mÈmoire!",                 /* Ran out of memory!-STDERR*/
  "Cette archive a &eacute;t&eacute; cr&eacute;&eacute;e par",  /* This archive was generated by-HTML*/
  "ElÈments",                              /* Elements          -STDOUT*/
  "Aucun ElÈment",                         /* No Elements       -STDOUT*/
  "Ne peut ouvrir l'archive de mail",      /* Cannot open mail archive */
  "Lecture du nouvel en-tÍte...",          /* Reading new header...-STDOUT   */
  "Chargement de la boite aux lettres",    /* Loading mailbox      -STDOUT   */
  "encodage non support&eacute;, stock&eacute; tel quel",/* encoding is not supported, stored as-is -HTML*/
  "messages",                              /* messages             -HTML*/
  "Version",                               /* Version            -STDOUT*/
  "Revision",                              /* Patchlevel         -STDOUT*/
  "Documents",                             /* Docs               -STDOUT*/
  "Variables de Commande et de ContrÙle",  /* Command and Control Variables-STDOUT*/
  "Oui",                                   /* Yes                -STDOUT*/
  "Non",                                   /* No                 -STDOUT*/
  "A propos de l'archive: non utilisÈ",    /* About the archive: not used-STDOUT */
  "Autres archives: non utilisÈ",          /* Other archives: not used-STDOUT*/
  "adresse non utilisÈe",                  /* address not used        -STDOUT*/
  "Utilisation de la dÈclaration <BODY>",  /* Builtin <BODY> statement used-STDOUT*/
  "Impossible de lire simulatanÈment un fichier et le flot d'entrÈe standard (stdin)", /* Cannot read from both file and stdin. -STDERR*/
  "Options",                               /* Options                -STDOUT*/
  "URL vers les autres archives",          /* URL to other archives  -STDOUT*/
  "URL vers les informations sur l'archive",  /* URL to archive information -STDOUT*/
  "Fichier de configuration ‡ lire",       /* Configuration file to read in -STDOUT*/
  "RÈpertoire o˘ sauver les fichiers HTML",/* The directory to save HTML files in -STDOUT*/
  "Lecture des messages depuis le flot d'entrÈe standard (stdin)", /* Read messages from standard input -STDOUT*/
  "Nom de l'archive de sortie",            /* What to name the output archive -STDOUT*/
  "Archive d'email ‡ lire",                   /* Mail archive to read in -STDOUT*/
  "Voir la progression",                   /* Show progress           -STDOUT*/
  "Voir les variables de configuration seulement", /* Show configuration variables only -STDOUT*/
  "Afficher la version et quitter",        /* Show version information and exit -STDOUT*/
  "Mettre ‡ jour l'archive d'un article",  /* Update archive by one article -STDOUT*/
  "Ecraser les messages prÈcÈdents",       /* Overwrite previous messages -STDOUT*/
  "SpÈcifier la langue ‡ utiliser",        /* Specify language to use     -STDOUT*/
  "Utilisation",                           /* Usage           -STDOUT*/
  "Langue non supportÈe",                  /* Language not supported -STDERR*/
  "Non dÈfini",                            /* Not set        -STDOUT*/
  "Non utilisÈ",                           /* Not used       -STDOUT*/
  "Fichier attachÈ crÈÈ",                  /* Created attachment file -STDOUT*/
  "fichier attach&eacute;",                /* attachment     -HTML*/
  "mode",                                  /* mode           -STDOUT*/
  "Lecture des anciens en-tÍtes",          /* Reading old headers -STDOUT*/
  "",                                      /* for alignment only       -STDOUT*/
  "ERREUR",                                /* ERROR                    -STDERR*/
  "Adresse d'envoi ‡ la liste", 
                                           /* The submission address of the list-STDERR*/
  "Lire seulement un mail depuis l'entrÈe",
  "Auteur",                                /* author              -HTML*/
  "Date",                                  /* date                -HTML*/
  "Sujet",                                 /* subject             -HTML*/
  "Actions sur les mails",                 /* Mail actions (MA) header -HTML*/
  "cr&eacute;er un nouveau th&egrave;me",  /* MA New Message      -HTML*/
  "r&eacute;pondre &agrave; ce message",   /* MA Reply            -HTML*/
  "R&eacute;capitulatif des fichiers Index mensuels",   /* monthly             -HTML*/
  "R&eacute;capitulatif des fichiers Index annuels",         /* yearly              -HTML*/
  "Creation d'un cache GDBM pour les en-tÍtes",  /* Build a GDBM header cache   -STDOUT*/
  "GDBM header cache option not build in",
  		      /* GDBM header cache option not build in  -STDERR*/
  "Generation de l'index GDBM... ",               /* Creating gdbm index         -STDOUT*/
  "Impossible de crÈer un fichier GDBM... ",    /* Can't create gdbm index     -STDOUT*/
  "Maintenance d'une archive email (mbox)",     /* Maintain an mbox archive    -STDOUT*/
  "Impossible de lire et d'Ècrire simultanÈment dans une archive email (mbox)", /* "Can't both read from and write to an mbox." -STDOUT*/
  "Liste par fichier attach&eacute;s",            /* Attachment view  -HTML*/
  "par messages avec fichiers attach&eacute;s",    /* By messages with attachments -HTML*/
  "Ecriture de l'index 'fichiers attachÈs' dans",/* Writing attachment index to -STDOUT*/
  "octets",		                   /* file size, so far only for attachments  -HTML */
  "Impossible de crÈer un lien symbolique",/* MSG_CANNOT_CREATE_SYMLINK     -STDOUT */
  "Impossible de supprimer le fichier",    /* MSG_CANNOT_UNLINK                       -STDOUT */
  "Dossier pr&eacute;c&eacute;dent",                     /* MSG_PREV_DIRECTORY                     -HTML */
  "Dossier suivant",                       /* MSG_NEXT_DIRECTORY                     -HTML */
  "Liste des dossiers",                    /* MSG_FOLDERS_INDEX                      -HTML */
  "Ce message a &eacute;t&eacute; supprim&eacute; de l'archive", /* MSG_DELETED -HTML */
  "Ce message est trop vieux",             /* MSG_EXPIRED -HTML */
  "(message supprim&eacute;)",              /* MSG_DEL_SHORT                            -HTML */
  "Texte original de ce message",          /* MSG_TXT_VERSION                          -HTML */
  "Ce message a &eacute;t&eacute; supprimÈ par filtrage",    /* MSG_FILTERED_OUT -HTML */
  "Auteur",                                /* MSG_FROM              -HTML*/
  "Ecrire fichiers XML haof (archive overview files)",       /* Write hoaf XML files       -STDOUT */
  "Ecriture des fichiers haof (archive overview files) dans",     /* Writing haof to               -HTML*/
  "Ce message",                 /* This message -HTML */
  "Corps du message",                 /* Message body -HTML */
  "R&eacute;pondre",                      /* Respond -HTML */
  "Autres options",                 /* More options -HTML */
  "Messages en rapport",             /* Related messages -HTML */
  "Suivant",                         /* Next -HTML */
  "Pr&eacute;cedent",                     /* Previous -HTML */
  "R&eacute;ponses",                      /* Replies -HTML */
  "Messages r&eacute;cents tri&eacute;s", /* Contemporary messages sorted -HTML */
  "Aide",                         /* Help  -HTML */
  "Message auquels ce message r&eacute;pond", /* In Reply To - HTML link*/
  "Message suivant dans la discussion", /* Next message in thread - HTML link*/
  "Message envoy&eacute; en r&eacute;ponse au message courant", /* Replies to this message - HTML link*/
  "Messages r&eacute;cents par date", /* Contemporary messages by date */
  "Discussions r&eacute;centes", /* Contemporary messages by threads -  HTML */
  "Messages r&eacute;cents par sujet", /* Contemporary messages by subject - HTML */
  "Messages r&eacute;cents par auteur", /* Contemporary messages by author - HTML*/
  "Message suivant dans la liste", /* Next message - HTML */
  "Message pr&eacute;cedent dans la liste", /* Previous message - HTML */
  "Messages r&eacute;cents par fichier attach&eacute;", /* Contemporary messages by attachments - HTML*/
  "Barre de navigation vers le niveau sup&eacute;rieur", /* Navigation bar, upper levels - HTML*/
  "Barre de navigation", /* Navigation bar - HTML*/
  "Trier par", /* Sort by - HTML*/
  "Autres p&eacute;riodes", /* Other periods - HTML */
  "Suivant", /* Next folder - HTML */
  "Messages archives dans la p&eacute;riode suivante, tri&eacute;s par date", /* Next folder, by date - HTML link */
  "Pr&eacute;cedent", /* Previous folder - HTML */
  "Messages archives dans la p&eacute;riode pr&eacute;cedente, tri&eacute;s par date", /* Previous folder, by date - HTML Link */
  "Liste de toutes les p&eacute;riodes", /* List of all periods - HTML*/
  "Cette p&eacute;riode", /* This period - HTML*/
  "Re&Ccedil;u le", /* Received on - HTML */
  "A cot&eacute; de", /* Nearby - HTML */
  "re-sorted", /* Resorted - HTML */
  "list&eacute; par date", /* listed by date - HTML */
  "par fil de discussion" , /* discussion threads - HTML */
  "list&eacute; par auteur", /* listed by author - HTML */
  "par sujet", /* listed by subject - HTML */
  "par fichier attach&eacute;", /* by attachment - HTML */
  "p&eacute;riode", /* period - HTML */
  " &eagrave; ", /* to - HTML */
  "from", /* from - HTML */
  "on", /* on - HTML */
  "message archived in another list or period", /* unknown in reply to - HTML */
  "Ce message a &eacute;t&eacute; supprim&eacute; de l'archive", /* MSG_DELETED_OTHER -HTML */
  "Note: this message has been edited and differs from the originally archived copy.", /* MSG_EDITED -HTML */
  NULL,                                    /* End Of Message Table  - NOWHERE*/
};

/*
** Icelandic version of the language table
** Translation by Bjarni R. Einarsson <bre@netverjar.is>
*/

char *is[] = {       /* Icelandic */
  "N˝tt brÈf",                   /* New Message        -HTML*/
  "Svar",                              /* Reply              -HTML*/
  "Um ˛ennan lista",                /* About this list    -HTML*/
  "Endir safns",                       /* End of Messages    -HTML*/
  "Upphaf safns",                      /* Start of Messages  -HTML*/
  "Raa e. dagssetningum",      /* Date view          -HTML*/
  "Raa e. umrÊum",      /* Thread view        -HTML*/
  "Raa e. vifangsefnum",   /* Subject view       -HTML*/
  "Raa e. hˆfundum",        /* Author view        -HTML*/
  "Attachment view",                  /* Attachment view    -HTML*/
  "BrÈfum",                         /* Messages           -HTML*/
  "Fr·",                            /* Starting           -HTML*/
  "Til",                               /* Ending             -HTML*/
  "Um ˛etta safn",                  /* About this archive -HTML*/
  "Raa eftir",                     /* Messages sorted by -HTML*/
  "÷nnur sˆfn",                  /* Other mail archives -HTML*/
  "eftir dagssetningum",               /* By Date             -HTML*/
  "N˝justu brÈfin",              /* Most recent messages-HTML*/
  "hˆfundum",                       /* author              -HTML*/
  "dags.",                             /* date                -HTML*/
  "umrÊum",                     /* thread              -HTML*/
  "vifangsefnum",                  /* subject             -HTML*/
  "for options",                       /* for options       -STDOUT*/
  "Writing messages to",               /* Writing messages to-STDOUT*/
  "Writing date index to",             /* Writing date index to-STDOUT*/
  "Writing thread index to",           /* Writing thread index to -STDOUT*/
  "Writing subject index to",          /* Writing subject index to-STDOUT*/
  "Writing author index to",           /* Writing author index to-STDOUT*/
  "Dags. sÌasta brÈfs",      /* Last message date   -HTML*/
  "SÌasta brÈf mÛtteki", /* Archived on         -HTML*/
  "Can not chmod",                     /* Can not chmod     -STDERR*/
  "Could not write",                   /* Could not write   -STDERR*/
  "NÊsta brÈf",                  /* Next message        -HTML*/
  "Fyrra brÈf",                     /* Previous message    -HTML*/
  "Mˆgulega Ì frh. af",          /* Maybe in reply to   -HTML*/
  "Õ framhaldi af",                 /* In reply to         -HTML*/
  "NÊst Ì umrÊu",         /* Next in thread      -HTML*/
  "Mˆgulegt svar",                  /* Maybe reply         -HTML*/
  "eftir umrÊum",               /* By Thread           -HTML*/
  "eftir vifangsefnum",            /* By Subject          -HTML*/
  "eftir hˆfundum",                 /* By Author           -HTML*/
  "Can not create directory",          /* Can not create directory -STDERR*/
  "Creating directory",                /* Creating directory -STDOUT*/
  "Configuration Values",              /* Configuration Values -STDOUT*/
  "path",                              /* path              -STDOUT*/
  "Ran out of memory!",                /* Ran out of memory!-STDERR*/
  "Framreitt af",                      /* This archive was generated by-HTML*/
  "Elements",                          /* Elements          -STDOUT*/
  "No Elements",                       /* No Elements       -STDOUT*/
  "Cannot open mail archive",          /* Cannot open mail archive */
  "Reading new header...",             /* Reading new header...-STDOUT   */
  "Loading mailbox",                   /* Loading mailbox      -STDOUT   */
  "framandi kÛun, geymt Ûbreytt", /* encoding is not supported, stored as-is -HTML*/
  "brÈf.",                          /* messages             -HTML*/
  "Version",                           /* Version            -STDOUT*/
  "Patchlevel",                        /* Patchlevel         -STDOUT*/
  "Docs",                              /* Docs               -STDOUT*/
  "Command and Control Variables",     /* Command and Control Variables-STDOUT*/
  "Yes",                               /* Yes                -STDOUT*/
  "No",                                /* No                 -STDOUT*/
  "About the archive: not used",       /* About the archive: not used-STDOUT */
  "Other archives: not used",          /* Other archives: not used-STDOUT*/
  "address not used",                  /* address not used        -STDOUT*/
  "Builtin <BODY> statement used",     /* Builtin <BODY> statement used-STDOUT*/
  "Cannot read from both file and stdin.", /* Cannot read from both file and stdin. -STDERR*/
  "Options",                           /* Options                -STDOUT*/
  "URL to other archives",             /* URL to other archives  -STDOUT*/
  "URL to archive information",        /* URL to archive information -STDOUT*/
  "Configuration file to read in",    /* Configuration file to read in -STDOUT*/
  "The directory to save HTML files in", /* The directory to save HTML files in -STDOUT*/
  "Read messages from standard input", /* Read messages from standard input -STDOUT*/
  "What to name the output archive",   /* What to name the output archive -STDOUT*/
  "Mail archive to read in",           /* Mail archive to read in -STDOUT*/
  "Show progress",                     /* Show progress           -STDOUT*/
  "Show configuration variables only", /* Show configuration variables only -STDOUT*/
  "Show version information and exit", /* Show version information and exit -STDOUT*/
  "Update archive by one article",    /* Update archive by one article -STDOUT*/
  "Overwrite previous messages",      /* Overwrite previous messages -STDOUT*/
  "Specify language to use",          /* Specify language to use     -STDOUT*/
  "Usage",                            /* Usage                       -STDOUT*/
  "Language not supported",           /* Language not supported      -STDERR*/
  "Not set",                          /* Not set                     -STDOUT*/
  "Not used",                         /* Not used                    -STDOUT*/
  "Created attachment file",          /* Created attachment file     -STDOUT*/
  "vihengi",                      /* attachment                    -HTML*/
  "mode",                             /* mode                        -STDOUT*/
  "Reading old headers",              /* Reading old headers         -STDOUT*/
  "",                                 /* for alignment only       -STDOUT*/
  "ERROR",                            /* ERROR                       -STDERR*/
  "The submission address of the list", 
                           /* The submission address of the list-STDERR*/
  "Read only one mail from input",
  "Hˆfundur",                      /* author              -HTML*/
  "Dagssetning",                      /* date                -HTML*/
  "Vifangsefni",                  /* subject             -HTML*/
  "Mail actions",                     /* Mail actions (MA) header -HTML*/
  "mail a new topic",                 /* MA New Message      -HTML*/
  "respond to this message",          /* MA Reply            -HTML*/
  "Summary of Monthly Index Files",   /* monthly             -HTML*/
  "Summary of Yearly Index Files",    /* yearly              -HTML*/
  "Build a GDBM header cache",   /* Build a GDBM header cache   -STDOUT*/
  "GDBM header cache option not build in",
  		      /* GDBM header cache option not build in  -STDERR*/
  "GDBM header cache option not build in",
  		      /* GDBM header cache option not build in  -STDERR*/
  "Creating gdbm index... ",     /* Creating gdbm index         -STDOUT*/
  "Can't create gdbm file... ",  /* Can't create gdbm index     -STDOUT*/
  "Maintain an mbox archive",    /* Maintain an mbox archive    -STDOUT*/
  "Can't both read from and write to an mbox.", /* "Can't both read from and write to an mbox." -STDOUT*/
  "Attachment view",                      /* Attachment view  -HTML*/
  "By messages with attachments",	  /* By messages with attachments -HTML*/
  "Writing attachment index to",	  /* Writing attachment index to -STDOUT*/
  "bytes",		    /* file size, so far only for attachments  -HTML */
  "Cannot create symbolic link", /* MSG_CANNOT_CREATE_SYMLINK     -STDOUT */
  "Cannot remove file", /* MSG_CANNOT_UNLINK                       -STDOUT */
  "Previous Folder",  /* MSG_PREV_DIRECTORY                     -HTML */
  "Next Folder",      /* MSG_NEXT_DIRECTORY                     -HTML */
  "List of Folders",  /* MSG_FOLDERS_INDEX                      -HTML */
  "This message has been deleted from the archive", /* MSG_DELETED -HTML */
  "This message has expired",                       /* MSG_EXPIRED -HTML */
  "(deleted message)", /* MSG_DEL_SHORT                            -HTML */
  "Original text of this message", /* MSG_TXT_VERSION                          -HTML */
  "This message has been filtered out",        /* MSG_FILTERED_OUT -HTML */
  "Hˆfundur",                         /* MSG_FROM              -HTML*/
  "Write haof XML files",       /* Write hoaf XML files       -STDOUT */
  "Writing haof to",            /* Writing haof to               -HTML*/
  "This message",                 /* This message -HTML */
  "Message body",                 /* Message body -HTML */
  "Respond",                      /* Respond -HTML */
  "More options",                 /* More options -HTML */
  "Related messages",             /* Related messages -HTML */
  "Next",                         /* Next -HTML */
  "Previous",                     /* Previous -HTML */
  "Replies",                      /* Replies -HTML */
  "Contemporary messages sorted", /* Contemporary messages sorted -HTML */
  "Help",                         /* Help  -HTML */
  "Message to which this message replies", /* In Reply To - HTML link*/
  "Next message in this discussion thread", /* Next message in thread - HTML link*/
  "Message sent in reply to this message", /* Replies to this message - HTML link*/
  "Contemporary messages by date", /* Contemporary messages by date */
  "Contemporary discussion threads", /* Contemporary messages by threads -  HTML */
  "Contemporary messages by subject", /* Contemporary messages by subject - HTML */
  "Contemporary messages by author", /* Contemporary messages by author - HTML*/
  "Next message in the list", /* Next message - HTML */
  "Previous message in the list", /* Previous message - HTML */
  "Contemporary messages by attachment", /* Contemporary messages by attachments - HTML*/
  "Navigation bar to upper levels", /* Navigation bar, upper levels - HTML*/
  "Navigation bar", /* Navigation bar - HTML*/
  "sort by", /* Sort by - HTML*/
  "Other periods", /* Other periods - HTML */
  "Next", /* Next folder - HTML */
  "Messages archived in the next period, sorted by date", /* Next folder, by date - HTML link */
  "Previous", /* Previous folder - HTML */
  "Messages archived in the previous period, sorted by date", /* Previous folder, by date - HTML Link */
  "List of all periods", /* List of all periods - HTML*/
  "This period", /* This period - HTML*/
  "Received on", /* Received on - HTML */
  "Nearby", /* Nearby - HTML */
  "re-sorted", /* Resorted - HTML */
  "listed by date", /* listed by date - HTML */
  "by discussion threads" , /* discussion threads - HTML */
  "listed by author", /* listed by author - HTML */
  "by subject", /* listed by subject - HTML */
  "by attachment", /* by attachment - HTML */
  "period", /* period - HTML */
  " to ", /* to - HTML */
  "from", /* from - HTML */
  "on", /* on - HTML */
  "message archived in another list or period", /* unknown in reply to - HTML */
  "This message has been deleted from the archive", /* MSG_DELETED_OTHER -HTML */
  "Note: this message has been edited and differs from the originally archived copy.", /* MSG_EDITED -HTML */
  NULL,                               /* End Of Message Table      - NOWHERE*/
};

/*
** Swedish version of the language table. Daniel Stenberg translation.
** (a little secret for free: we don't say bork ;-)
*/

char *sv[] = {
  "Nytt brev",                      /* New Message        -HTML*/
  "Svar" ,                          /* Reply              -HTML*/
  "Om den h&auml;r listan",      /* About this list    -HTML*/
  "Slut p&aring; brevlistan",          /* End of Messages    -HTML*/
  "Start p&aring; brevlistan",         /* Start of Messages  -HTML*/
  "Datumvy",                        /* Date view          -HTML*/
  "Tr&aring;dvy",                      /* Thread view        -HTML*/
  "&Auml;mnesvy",                     /* Subject view       -HTML*/
  "F&ouml;rfattarvy",                 /* Author view        -HTML*/
  "Bilagevy",                  /* Attachment view    -HTML*/
  "Brev",                           /* Messages           -HTML*/
  "Startar",                        /* Starting           -HTML*/
  "Slutar",                         /* Ending             -HTML*/
  "Om det h&auml;r arkivet", /* About this archive -HTML*/
  "sorterade efter",                 /* Messages sorted by -HTML*/
  "Andra brevarkiv",                /* Other mail archives-HTML*/
  "Datumsorterat",                    /* By Date            -HTML*/
  "Nyast brev",                     /* Most recent messages-HTML*/
  "f&ouml;rfattare",                  /* author             -HTML*/
  "datum",                          /* date               -HTML*/
  "tr&aring;d",                        /* thread             -HTML*/
  "&auml;mne",                        /* subject            -HTML*/
  "fˆr inst‰llningar",        /* for options        -STDOUT*/
  "Skriver brev till",              /* Writing messages to-STDOUT*/
  "Skriver datumindex till",        /* Writing date index to-STDOUT*/
  "Skriver trÂdindex till",      /* Writing thread index to -STDOUT*/
  "Skriver ‰mnesindex till",     /* Writing subject index to-STDOUT*/
  "Skriver fˆrfattarindex till", /* Writing author index to-STDOUT*/
  "Senaste brevdatum",             /* Last message date  -HTML*/
  "Arkiverat",                      /* Archived on        -HTML*/
  "Kan inte chmod",                 /* Can not chmod      -STDERR*/
  "Kunde inte skriva",              /* Could not write    -STDERR*/
  "N&auml;sta brev",                  /* Next message       -HTML*/
  "Tidigare brev",                  /* Previous message   -HTML*/
  "Kanske ett svar till",           /* Maybe in reply to  -HTML*/
  "Svar till",                      /* In reply to        -HTML*/
  "N&auml;sta i tr&aring;den",               /* Next in thread     -HTML*/
  "Kanske svar",                    /* Maybe reply        -HTML*/
  "TrÂdsorterat",                    /* By Thread          -HTML*/
  "ƒmnessorterat",                    /* By Subject         -HTML*/
  "Fˆrfattarsorterat",              /* By Author          -HTML*/
  "Kan inte skapa katalog",       /* Can not create directory -STDERR*/
  "Skapar katalog",               /* Creating directory -STDOUT*/
  "Konfigureringsv‰rden",        /* Configuration Values -STDOUT*/
  "path",                           /* path               -STDOUT*/
  "Fick slut pÂ minne!",         /* Ran out of memory!-STDERR*/
  "Det h&auml;r arkivet skapades av", /* This archive was generated by-HTML*/
  "Delar",                          /* Elements           -STDOUT*/
  "Inga delar",                     /* No Elements        -STDOUT*/
  "Kan inte ˆppna brevarkivet",  /* Cannot open mail archive */
  "L‰ser nytt huvud...",          /* Reading new header...-STDOUT*/
  "Laddar mailbox",                 /* Loading mailbox   -STDOUT   */
  "kodformatet st&ouml;ds inte, sparat som det &auml;r", 
                           /* encoding is not supported, stored as-is -HTML*/
  "brev",                           /* messages          -HTML*/
  "Version",                        /* Version           -STDOUT*/
  "PatchnivÂ",                   /* Patchlevel        -STDOUT*/
  "Dokumentation",                  /* Docs              -STDOUT*/
  "Kommando- och styrvariabler", /* Command and Control Variables-STDOUT*/
  "Ja",                             /* Yes               -STDOUT*/
  "Nej",                            /* No                -STDOUT*/
  "Om arkivet: ej anv‰nd",      /* About the archive: not used-STDOUT */
  "Andra arkiv: ej anv‰nd",      /* Other archives: not used-STDOUT*/
  "adress ej anv‰nd",            /* address not used  -STDOUT*/
  "Inbyggd <BODY> anv‰nd",       /* Builtin <BODY> statement used-STDOUT*/
  "Kan inte l‰sa bde frÂn fil och stdin.", 
                           /* Cannot read from both file and stdin. -STDERR*/
  "Alternativ",                     /* Options           -STDOUT*/
  "URL till andra arkiv",           /* URL to other archives  -STDOUT*/
  "URL till arkivinformation",      /* URL to archive information -STDOUT*/
  "Konfigurationsfil att l‰sa in",   /* Configuration file to read in -STDOUT*/
  "Katalog att spara HTML-filerna i", 
                            /* The directory to save HTML files in -STDOUT*/
  "L‰s breven fr&aring;n standard in", /* Read messages from standard input-STDOUT*/
  "Namnet pÂ utarkivet",         /* What to name the output archive-STDOUT*/
  "Brevarkiv att l‰sa in",       /* Mail archive to read in -STDOUT*/
  "Visa fˆrlopp",                  /* Show progress      -STDOUT*/
  "Visa konfigurationsvariabler enbart",
                                /* Show configuration variables only-STDOUT*/
  "Visa versionsinformation och sluta", 
                                /* Show version information and exit -STDOUT*/
  "Updatera arkivet med ett brev", /*Update archive by one article -STDOUT*/
  "Skriv ˆver tidigare brev",    /* Overwrite previous messages -STDOUT*/
  "Ange sprÂk att anv‰nda",   /* Specify language to use -STDOUT*/
  "Anv‰ndning",                  /* Usage                   -STDOUT*/
  "SprÂket stˆds ej",          /* Language not supported  -STDERR*/
  "Ej angivet",                     /* Not set                 -STDOUT*/
  "Ej anv‰nt",                   /* Not used                -STDOUT*/
  "Skapade bilagefil",          /* Created attachment file -STDOUT*/
  "bilaga",                      /* attachment              -HTML*/
  "l‰ge",                           /* mode                    -STDOUT*/
  "L‰ser gamla huvuden",         /* Reading old headers     -STDOUT*/
  "endast fˆr justering",        /* for alignment only   -STDOUT*/
  "FEL",                            /* ERROR                   -STDERR*/
  "Adressen fˆr att posta till listan", 
                           /* The submission address of the list-STDERR*/
  "L‰s bara ett brev frÂn indata",
  "F&ouml;rfattare",                  /* author             -HTML*/
  "Datum",                          /* date               -HTML*/
  "&Auml;mne",                        /* subject            -HTML*/
  "E-postfunktioner",                   /* Mail actions (MA) header -HTML*/
  "s&auml;nd ett nytt &auml;mne",               /* MA New Message      -HTML*/
  "svara p&aring; brevet",        /* MA Reply            -HTML*/
  "Sammanfattning &ouml;ver m&aring;natliga indexfiler", /* monthly             -HTML*/
  "Sammanfattning &ouml;ver &aring;rliga indexfiler",  /* yearly              -HTML*/
  "Bygger en GDBM-huvudcache",   /* Build a GDBM header cache   -STDOUT*/
  "Tillval fˆr GDBM-huvudcache inte inkompilerat",
  		      /* GDBM header cache option not build in  -STDERR*/
  "Skapar gdbm-index... ",     /* Creating gdbm index         -STDOUT*/
  "Kan inte skapa gdbm-fil... ",  /* Can't create gdbm index     -STDOUT*/
  "UnderhÂll ett mbox-arkiv",    /* Maintain an mbox archive    -STDOUT*/
  "Kan inte bÂde l‰sa frÂn och skriva till en mbox.", /* "Can't both read from and write to an mbox." -STDOUT*/
  "Bilagevy",                      /* Attachment view  -HTML*/
  "Efter brev med bilaga",	  /* By messages with attachments -HTML*/
  "Skriver bilageindex till",	  /* Writing attachment index to -STDOUT*/
  "byte",		    /* file size, so far only for attachments  -HTML */
  "Kan inte skapa symbolisk l‰nk", /* MSG_CANNOT_CREATE_SYMLINK     -STDOUT */
  "Kan inte ta bort fil", /* MSG_CANNOT_UNLINK                       -STDOUT */
  "F&ouml;reg&aring;ende mapp",  /* MSG_PREV_DIRECTORY                     -HTML */
  "N&auml;sta mapp",      /* MSG_NEXT_DIRECTORY                     -HTML */
  "Mapplista",  /* MSG_FOLDERS_INDEX                      -HTML */
  "Detta brev har tagits bort fr&aring;n arkivet", /* MSG_DELETED -HTML */
  "Detta brev har utg&aring;tt",                       /* MSG_EXPIRED -HTML */
  "(borttaget brev)", /* MSG_DEL_SHORT                            -HTML */
  "Ursprunglig brevtext", /* MSG_TXT_VERSION                          -HTML */
  "Detta brev har filtrerats",        /* MSG_FILTERED_OUT -HTML */
  "F&ouml;rfattare",                     /* MSG_FROM             -HTML*/
  "Skriv haof-XML-filer",       /* Write hoaf XML files       -STDOUT */
  "Skriver haof till",            /* Writing haof to               -HTML*/
  "This message",                 /* This message -HTML */
  "Message body",                 /* Message body -HTML */
  "Respond",                      /* Respond -HTML */
  "More options",                 /* More options -HTML */
  "Related messages",             /* Related messages -HTML */
  "Next",                         /* Next -HTML */
  "Previous",                     /* Previous -HTML */
  "Replies",                      /* Replies -HTML */
  "Contemporary messages sorted", /* Contemporary messages sorted -HTML */
  "Help",                         /* Help  -HTML */
  "Message to which this message replies", /* In Reply To - HTML link*/
  "Next message in this discussion thread", /* Next message in thread - HTML link*/
  "Message sent in reply to this message", /* Replies to this message - HTML link*/
  "Contemporary messages by date", /* Contemporary messages by date */
  "Contemporary discussion threads", /* Contemporary messages by threads -  HTML */
  "Contemporary messages by subject", /* Contemporary messages by subject - HTML */
  "Contemporary messages by author", /* Contemporary messages by author - HTML*/
  "Next message in the list", /* Next message - HTML */
  "Previous message in the list", /* Previous message - HTML */
  "Contemporary messages by attachment", /* Contemporary messages by attachments - HTML*/
  "Navigation bar to upper levels", /* Navigation bar, upper levels - HTML*/
  "Navigation bar", /* Navigation bar - HTML*/
  "sort by", /* Sort by - HTML*/
  "Other periods", /* Other periods - HTML */
  "Next", /* Next folder - HTML */
  "Messages archived in the next period, sorted by date", /* Next folder, by date - HTML link */
  "Previous", /* Previous folder - HTML */
  "Messages archived in the previous period, sorted by date", /* Previous folder, by date - HTML Link */
  "List of all periods", /* List of all periods - HTML*/
  "This period", /* This period - HTML*/
  "Received on", /* Received on - HTML */
  "Nearby", /* Nearby - HTML */
  "re-sorted", /* Resorted - HTML */
  "listed by date", /* listed by date - HTML */
  "by discussion threads" , /* discussion threads - HTML */
  "listed by author", /* listed by author - HTML */
  "by subject", /* listed by subject - HTML */
  "by attachment", /* by attachment - HTML */
  "period", /* period - HTML */
  " to ", /* to - HTML */
  "from", /* from - HTML */
  "on", /* on - HTML */
  "message archived in another list or period", /* unknown in reply to - HTML */
  "Detta brev har tagits bort fr&aring;n arkivet", /* MSG_DELETED_OTHER -HTML */
  "Note: this message has been edited and differs from the originally archived copy.", /* MSG_EDITED -HTML */
  NULL,                             /* End Of Message Table    - NOWHERE*/
};

/*
** Norwegian version of the language table.
** Translation by Bosse Klykken <bosse@klykken.com>
*/

char *no[] = {
  "Ny melding",                      /* New Message        -HTML*/
  "Svar" ,                          /* Reply              -HTML*/
  "Om denne listen",     /* About this list    -HTML*/
  "Slutt pÂ brevlisten",          /* End of Messages    -HTML*/
  "Start pÂ brevlisten",         /* Start of Messages  -HTML*/
  "Oversikt dato",                        /* Date view          -HTML*/
  "Oversikt trÂd",                      /* Thread view        -HTML*/
  "Oversikt emne",                     /* Subject view       -HTML*/
  "Oversikt forfatter",                 /* Author view        -HTML*/
  "Andre grupper",                  /* Other groups       -HTML*/
  "Meldinger",                           /* Messages           -HTML*/
  "Starter",                        /* Starting           -HTML*/
  "Slutter",                         /* Ending             -HTML*/
  "Om dette arkivet", /* About this archive -HTML*/
  "sortert etter",                 /* Messages sorted by -HTML*/
  "Andre meldingsarkiv",                /* Other mail archives-HTML*/
  "Sortert etter dato",                    /* By Date            -HTML*/
  "Nyeste melding",                     /* Most recent messages-HTML*/
  "forfatter",                  /* author             -HTML*/
  "dato",                          /* date               -HTML*/
  "trÂd",                        /* thread             -HTML*/
  "emne",                        /* subject            -HTML*/
  "for innstillinger",        /* for options        -STDOUT*/
  "Skriver meldinger til",              /* Writing messages to-STDOUT*/
  "Skriver datoindeks til",        /* Writing date index to-STDOUT*/
  "Skriver trÂdindeks til",      /* Writing thread index to -STDOUT*/
  "Skriver emneindeks til",     /* Writing subject index to-STDOUT*/
  "Skriver forfatterindeks til", /* Writing author index to-STDOUT*/
  "Siste meldingsdato",             /* Last message date  -HTML*/
  "Arkivert",                      /* Archived on        -HTML*/
  "Kan ikke chmod",                 /* Can not chmod      -STDERR*/
  "Kunne ikke skrive",              /* Could not write    -STDERR*/
  "Neste melding",                  /* Next message       -HTML*/
  "Forrige melding",                  /* Previous message   -HTML*/
  "Muligens et svar til",           /* Maybe in reply to  -HTML*/
  "Som svar til",                      /* In reply to        -HTML*/
  "Neste i trÂden",               /* Next in thread     -HTML*/
  "Kanskje svar",                    /* Maybe reply        -HTML*/
  "Sortert etter trÂd",                    /* By Thread          -HTML*/
  "Sortert etter emne",                    /* By Subject         -HTML*/
  "Sortert etter forfatter",              /* By Author          -HTML*/
  "Kan ikke opprette katalog",       /* Can not create directory -STDERR*/
  "Oppretter katalog",               /* Creating directory -STDOUT*/
  "Konfigureringsverdier",        /* Configuration Values -STDOUT*/
  "bane",                           /* path               -STDOUT*/
  "Gikk tom for minne!",         /* Ran out of memory!-STDERR*/
  "Dette arkivet er generert av", /* This archive was generated by-HTML*/
  "Elementer",                          /* Elements           -STDOUT*/
  "Ingen elementer",                     /* No Elements        -STDOUT*/
  "Kan ikke Âpne meldingsarkivet",  /* Cannot open mail archive */
  "Leser nytt meldingshode...",          /* Reading new header...-STDOUT*/
  "Leser mailboks",                 /* Loading mailbox   -STDOUT   */
  "kodeformatet er usupportet, lagret som den er",
                           /* encoding is not supported, stored as-is -HTML*/
  "meldinger",                           /* messages          -HTML*/
  "Versjon",                        /* Version           -STDOUT*/
  "PatchnivÂ",                   /* Patchlevel        -STDOUT*/
  "Dokumentasjon",                  /* Docs              -STDOUT*/
  "Kommando- og kontrollvariabler", /* Command and Control Variables-STDOUT*/
  "Ja",                             /* Yes               -STDOUT*/
  "Nei",                            /* No                -STDOUT*/
  "Om arkivet: ikke i bruk",      /* About the archive: not used-STDOUT */
  "Andre arkiv: ikke i bruk",      /* Other archives: not used-STDOUT*/
  "adresse ikke benyttet",            /* address not used  -STDOUT*/
  "Innebygget <BODY> benyttet",       /* Builtin <BODY> statement used-STDOUT*/
  "Kan ikke lese bÂde fra fil og stdin.",
                           /* Cannot read from both file and stdin. -STDERR*/
  "Alternativer",                     /* Options           -STDOUT*/
  "URL til andre arkiv",           /* URL to other archives  -STDOUT*/
  "URL til arkivinformasjon",      /* URL to archive information -STDOUT*/
  "Konfigurasjonsfil til Â lese inn",   /* Configuration file to read in -STDOUT*/
  "Katalog til Â spare HTML-filene i",
                            /* The directory to save HTML files in -STDOUT*/
  "Les brevene fra stdin", /* Read messages from standard input-STDOUT*/
  "Navn pÂ utarkivet",         /* What to name the output archive-STDOUT*/
  "Meldingsarkiv som skal leses inn",       /* Mail archive to read in -STDOUT*/
  "Vise fremdrift",                  /* Show progress      -STDOUT*/
  "Vise kun konfigurasjonsvariablene",
                                /* Show configuration variables only-STDOUT*/
  "Vise versjonsinformasjon og avslutte",
                                /* Show version information and exit -STDOUT*/
  "Oppdatere arkivet med Èn melding", /*Update archive by one article -STDOUT*/
  "Skriv over andre meldinger",    /* Overwrite previous messages -STDOUT*/
  "Angi sprÂk som skal benyttes",   /* Specify language to use -STDOUT*/
  "Bruk",                  /* Usage                   -STDOUT*/
  "SprÂket er ikke st¯ttet",          /* Language not supported  -STDERR*/
  "Ikke angitt",                     /* Not set                 -STDOUT*/
  "Ikke benyttet",                   /* Not used                -STDOUT*/
  "Lagde vedleggfil",          /* Created attachment file -STDOUT*/
  "vedlegg",                      /* attachment              -HTML*/
  "modus",                           /* mode                    -STDOUT*/
  "Leser gamle meldingshoder",         /* Reading old headers     -STDOUT*/
  "",                               /* for alignment only   -STDOUT*/
  "FEIL",                            /* ERROR                   -STDERR*/
  "Adressen som brukes for innlegg til listen", 
                           /* The submission address of the list-STDERR*/
  "Les bare en melding fra inn",
  "Forfatter",                  /* author             -HTML*/
  "Dato",                          /* date               -HTML*/
  "Emne",                        /* subject            -HTML*/
  "E-postfunksjoner",                   /* Mail actions (MA) header -HTML*/
  "Lag ny trÂd",               /* MA New Message      -HTML*/
  "besvare meldingen",        /* MA Reply            -HTML*/
  "Sammenfatning over mÂnedlige indeksfiler", /* monthly             -HTML*/
  "Sammenfatning over Ârlige indeksfiler",  /* yearly              -HTML*/
  "Bygger en GDBM-headercache",   /* Build a GDBM header cache   -STDOUT*/
  "GDBM header cache opsjon ikke innebygget",
  		      /* GDBM header cache option not build in  -STDERR*/
  "Lager gdbm-index... ",     /* Creating gdbm index         -STDOUT*/
  "Kan ikke lage gdbm-fil... ",  /* Can't create gdbm index     -STDOUT*/
  "Opprettholde et mbox-arkiv",    /* Maintain an mbox archive    -STDOUT*/
  "Kan ikke bÂde lese fra og skrive til en mbox.", /* "Can't both read from and write to an mbox."-STDOUT*/
  "Oversikt vedlegg",                      /* Attachment view  -HTML*/
  "Etter meldinger med vedlegg",	  /* By messages with attachments -HTML*/
  "Skriver vedleggsindeks til",	  /* Writing attachment index to -STDOUT*/
  "bytes",		    /* file size, so far only for attachments  -HTML */
  "Kan ikke lage symbolisk link", /* MSG_CANNOT_CREATE_SYMLINK     -STDOUT */
  "Kan ikke fjerne link til fil", /* MSG_CANNOT_UNLINK                       -STDOUT */
  "Forrige mappe",  /* MSG_PREV_DIRECTORY                     -HTML */
  "Neste mappe",      /* MSG_NEXT_DIRECTORY                     -HTML */
  "Mappeliste",  /* MSG_FOLDERS_INDEX                      -HTML */
  "Denne meldingen er fjernet fra arkivet", /* MSG_DELETED -HTML */
  "Dette meldingen har utgÂtt",                       /* MSG_EXPIRED -HTML */
  "(slettet melding)", /* MSG_DEL_SHORT                            -HTML */
  "Opprinnelig brevtekst", /* MSG_TXT_VERSION                          -HTML */
  "Denne meldingen har blitt filtrert bort",        /* MSG_FILTERED_OUT -HTML */
  "Forfatter",                     /* MSG_FROM             -HTML*/
  "Skrive haof XML filer",       /* Write hoaf XML files       -STDOUT */
  "Skriver haof til",            /* Writing haof to               -HTML*/
  "This message",                 /* This message -HTML */
  "Message body",                 /* Message body -HTML */
  "Respond",                      /* Respond -HTML */
  "More options",                 /* More options -HTML */
  "Related messages",             /* Related messages -HTML */
  "Next",                         /* Next -HTML */
  "Previous",                     /* Previous -HTML */
  "Replies",                      /* Replies -HTML */
  "Contemporary messages sorted", /* Contemporary messages sorted -HTML */
  "Help",                         /* Help  -HTML */
  "Message to which this message replies", /* In Reply To - HTML link*/
  "Next message in this discussion thread", /* Next message in thread - HTML link*/
  "Message sent in reply to this message", /* Replies to this message - HTML link*/
  "Contemporary messages by date", /* Contemporary messages by date */
  "Contemporary discussion threads", /* Contemporary messages by threads -  HTML */
  "Contemporary messages by subject", /* Contemporary messages by subject - HTML */
  "Contemporary messages by author", /* Contemporary messages by author - HTML*/
  "Next message in the list", /* Next message - HTML */
  "Previous message in the list", /* Previous message - HTML */
  "Contemporary messages by attachment", /* Contemporary messages by attachments - HTML*/
  "Navigation bar to upper levels", /* Navigation bar, upper levels - HTML*/
  "Navigation bar", /* Navigation bar - HTML*/
  "sort by", /* Sort by - HTML*/
  "Other periods", /* Other periods - HTML */
  "Next", /* Next folder - HTML */
  "Messages archived in the next period, sorted by date", /* Next folder, by date - HTML link */
  "Previous", /* Previous folder - HTML */
  "Messages archived in the previous period, sorted by date", /* Previous folder, by date - HTML Link */
  "List of all periods", /* List of all periods - HTML*/
  "This period", /* This period - HTML*/
  "Received on", /* Received on - HTML */
  "Nearby", /* Nearby - HTML */
  "re-sorted", /* Resorted - HTML */
  "listed by date", /* listed by date - HTML */
  "by discussion threads" , /* discussion threads - HTML */
  "listed by author", /* listed by author - HTML */
  "by subject", /* listed by subject - HTML */
  "by attachment", /* by attachment - HTML */
  "period", /* period - HTML */
  " to ", /* to - HTML */
  "from", /* from - HTML */
  "on", /* on - HTML */
  "message archived in another list or period", /* unknown in reply to - HTML */
  "Denne meldingen er fjernet fra arkivet", /* MSG_DELETED_OTHER -HTML */
  "Note: this message has been edited and differs from the originally archived copy.", /* MSG_EDITED -HTML */
  NULL,                             /* End Of Message Table    - NOWHERE*/
};

/*
** Greek version of the language table
** Translation by Akis Karnouskos <akis@ceid.upatras.gr> 
** ÃÂÒÈÍ›Ú ÂÍˆÒ‹ÛÂÈÚ Ôı ‚„·ﬂÌÔıÌ Ï¸ÌÔ ÛÙÔ STDOUT/STDERROR
** ÂﬂÌ·È ÛÍ¸ÈÏ· ·ÏÂÙ‹ˆÒ·ÛÙÂÚ ...
** ºÔÈÔÚ Ó›ÒÂÈ ÙÈÚ ·ÍÒÈ‚›Ú ÏÂÙ·ˆÒ‹ÛÂÈÚ „È· Ù· ·Ò·Í‹Ù˘ ·Ú
** ÏÔı ÙÈÚ ÛÙÂﬂÎÂÈ „È· Ì· Í‹ÌÔıÏÂ update !
*/

char *gr[] = {       /* Greek */
  "Õ›Ô ÃﬁÌıÏ·",              		/* New Message        -HTML*/
  "¡‹ÌÙÁÛÁ",                   	/* Reply              -HTML*/
  "”˜ÂÙÈÍ‹ ÏÂ ·ıÙﬁ ÙÁ ÎﬂÛÙ·",         	/* About this list    -HTML*/
  "‘›ÎÔÚ ÃÁÌıÏ‹Ù˘Ì",                   	/* End of Messages    -HTML*/
  "¡Ò˜ﬁ ÃÁÌıÏ‹Ù˘Ì",                 	/* Start of Messages  -HTML*/
  "–ÒÔ‚ÔÎﬁ ·Ì· ÁÏ›Ò·",            	/* Date view          -HTML*/
  "–ÒÔ‚ÔÎﬁ ·Ì‹ thread",                	/* Thread view        -HTML*/
  "–ÒÔ‚ÔÎﬁ ·Ì‹ Ë›Ï·",                	/* Subject view       -HTML*/
  "–ÒÔ‚ÔÎﬁ ·Ì‹ Ûı„„Ò·ˆ›·",          	/* Author view        -HTML*/
  "–ÒÔ‚ÔÎﬁ ≈ÈÛ˝Ì·¯ÁÚ",                	/* Attachment view    -HTML*/
  "ÃÁÌ˝Ï·Ù·",                          	/* Messages           -HTML*/
  "¡Ò˜ﬁ",                               /* Starting           -HTML*/
  "‘›ÎÔÚ",                            	/* Ending             -HTML*/
  "”˜ÂÙÈÍ‹ ÏÂ ·ıÙ¸ ÙÔ ·Ò˜ÂﬂÔ",       	/* About this archive -HTML*/
  "Ù·ÓÈÌÔÏÁÏ›Ì· ·Ì‹",            	/* sorted by 	      -HTML*/
  "AÎÎ· Mail ¡Ò˜Âﬂ·",                 	/* Other mail archives -HTML*/
  "¡Ì‹ «ÏÂÒÔÏÁÌﬂ·",                 	/* By Date             -HTML*/
  "‘· ÈÔ Í·ÈÌÔ˝ÒÈ· ÏÁÌ˝Ï·Ù·",        	/* Most recent messages-HTML*/
  "Ûı„„Ò·ˆ›·",                        	/* author              -HTML*/
  "ÁÏÂÒÔÏÁÌﬂ·",          		/* date                -HTML*/
  "thread",                       	/* thread              -HTML*/
  "Ë›Ï·",                              	/* subject             -HTML*/
  "„È· ÂÈÎÔ„›Ú",                      	/* for options       -STDOUT*/
  "√Ò‹ˆ˘ Ù· ÃÁÌ˝Ï·Ù· ÛÙÔ",              /* Writing messages to-STDOUT*/
  "√Ò‹ˆ˘ ÙÔ ≈ıÒÂÙﬁÒÈÔ ·Ì‹ ÁÏ›Ò·",    	/* Writing date index to-STDOUT*/
  "√Ò‹ˆ˘ ÙÔ ≈ıÒÂÙﬁÒÈÔ ·Ì‹ thread",    	/* Writing thread index to -STDOUT*/
  "√Ò‹ˆ˘ ÙÔ ≈ıÒÂÙﬁÒÈÔ ·Ì‹ »›Ï·",     	/* Writing subject index to-STDOUT*/
  "√Ò‹ˆ˘ ÙÔ ≈ıÒÂÙﬁÒÈÔ ·Ì‹ Ûı„„Ò·ˆ›·",  	/* Writing author index to-STDOUT*/
  "‘ÂÎÂıÙ·ﬂ· ÁÏÂÒÔÏÁÌﬂ·",              	/* Last message date   -HTML*/
  "¡Ò˜ÂÈÔÔÈﬁËÁÍÂ",                   	/* Archived on         -HTML*/
  "ƒÂÌ ÏÔÒ˛ Ì· Í‹Ì˘ chmod",      	/* Can not chmod     -STDERR*/
  "ƒÂÌ ÏÔÒ˛ Ì· „Ò‹¯˘",           	/* Could not write   -STDERR*/
  "≈¸ÏÂÌÔ ÏﬁÌıÏ·",                 	/* Next message        -HTML*/
  "–ÒÔÁ„Ô˝ÏÂÌÔ ÏﬁÌıÏ·",                  /* Previous message    -HTML*/
  "∫Û˘Ú Û·Ì ·‹ÌÙÁÛÁ ÛÙÔ",           	/* Maybe in reply to   -HTML*/
  "”·Ì ·‹ÌÙÁÛÁ ÛÙÔ",               	/* In reply to         -HTML*/
  "≈¸ÏÂÌÔ ÛÙÔ thread",                 /* Next in thread      -HTML*/
  "–ÈË·Ìﬁ ·‹ÌÙÁÛÁ",                   	/* Maybe reply         -HTML*/
  "¡Ì‹ Thread",                     	/* By Thread           -HTML*/
  "¡Ì‹ »›Ï·",                         	/* By Subject          -HTML*/
  "¡Ì‹ Ûı„„Ò·ˆ›·",                     	/* By Author           -HTML*/
  "ƒÂÌ ÏÔÒ˛ Ì· ‰ÁÏÈÔıÒ„ﬁÛ˘ ÙÔ directory", /* Can not create directory -STDERR*/
  "ƒÁÏÈÔıÒ„˛ ÙÔ directory",          	/* Creating directory -STDOUT*/
  "configuration",                 	/* Configuration Values -STDOUT*/
  "path",                            	/* path              -STDOUT*/
  "ƒÂÌ ı‹Ò˜ÂÈ ‹ÎÎÁ ‰È·Ë›ÛÈÏÁ ÏÌﬁÏÁ", 	/* Ran out of memory!-STDERR*/
  "¡ıÙ¸ ÙÔ ·Ò˜ÂﬂÔ ‰ÁÏÈÔıÒ„ﬁËÁÍÂ ·Ô",  	/* This archive was generated by-HTML*/
  "”ÙÔÈ˜Âﬂ·",                        	/* Elements          -STDOUT*/
  " ·Ì›Ì· ÛÙÔÈ˜ÂﬂÔ",                	/* No Elements       -STDOUT*/
  "ƒÂÌ ÏÔÒ˛ Ì· ·ÌÔﬂÓ˘ ÙÔ ·Ò˜ÂﬂÔ ÏÂ Ù· ÏÁÌ˝Ï·Ù·",     /* Cannot open mail archive */
  "ƒÈ·‚‹Ê˘ Ù· Í·ÈÌÔ˝ÒÈ· headers",    	/* Reading new header...-STDOUT*/
  "÷ÔÒÙ˛Ì˘ ÙÔ mailbox",               	/* Loading mailbox      -STDOUT*/
  "Á Í˘‰ÈÍÔÔﬂÁÛÁ ‰ÂÌ ıÔÛÙÁÒﬂÊÂÙ·È, Ù· ·ÔËÁÍÂ˝˘ Û·Ì", 
                        		/* encoding is not supported, stored as-is -HTML*/
  "ÏÁÌ˝Ï·Ù·",                          	/* messages             -HTML*/
  "∏Í‰ÔÛÁ",                        	/* Version            -STDOUT*/
  "Patchlevel",               		/* Patchlevel         -STDOUT*/
  " ›ÈÏÂÌ·",                          	/* Docs               -STDOUT*/
  "≈ÌÙÔÎﬁ Í·È ‰ÈÔÈÍÁÙÈÍ›Ú ÏÂÙ·‚ÎÁÙ›Ú", 	/* Command and Control Variables-STDOUT*/
  "Õ·ﬂ",                            	/* Yes                -STDOUT*/
  "º˜È",                         	/* No                 -STDOUT*/
  "”˜ÂÙÈÍ‹ ÏÂ ÙÔ ·Ò˜ÂﬂÔ: ƒ›Ì ˜ÒÁÛÈÏÔÔÈﬁËÁÍÂ",    /* About the archive: not used-STDOUT */
  "AÎÎÔ ·Ò˜ÂﬂÔ: ƒ›Ì ˜ÒÁÛÈÏÔÔÈﬁËÁÍÂ", 	/* Other archives: not used-STDOUT*/
  "ƒÈÂ˝ËıÌÛÁ: ƒ›Ì ˜ÒÁÛÈÏÔÔÈﬁËÁÍÂ", 	/* address not used        -STDOUT*/
  "‘Ô ﬁ‰Á ı‹Ò˜ÔÌ <BODY> ˜ÒÁÛÈÏÔÔÈﬁËÁÍÂ",   	/* Builtin <BODY> statement used-STDOUT*/
  "ƒÂÌ ÏÔÒ˛ Ì· ‰È·‚‹Û˘ ·Ô ÙÔ ·Ò˜ÂﬂÔ Í·È ÙÔ stdin.", /* Cannot read from both file and stdin. -STDERR*/
  "≈ÈÎÔ„›Ú",                      	/* Options                -STDOUT*/
  "URL „È· ‹ÎÎ· ·Ò˜Âﬂ·",           	/* URL to other archives  -STDOUT*/
  "URL „È· ÎÁÒÔˆÔÒﬂÂÚ „È· ·ıÙ¸ ÙÔ ·Ò˜ÂﬂÔ",  
                                      	/* URL to archive information -STDOUT*/
  "¡Ò˜ÂﬂÔ ÏÂ ·Ò·Ï›ÙÒÔıÚ „È· Ì· ‰È·‚‹Û˘", 
                                    	/* Configuration file to read in -STDOUT*/
  "‘Ô directory „È· Ì· ·ÔËÁÍ›ıÛ˘ Ù· HTML ·Ò˜Âﬂ· ", 
                              		/* The directory to save HTML files in -STDOUT*/
  "ƒÈ‹‚·ÛÂ Ù· ÏÁÌ˝Ï·Ù· ·Ô ÙÁ ÛÙ‹ÌÙ·Ò ÂﬂÛÔ‰Ô",
                                	/* Read messages from standard input -STDOUT*/
  "–˘Ú Ò›ÂÈ Ì· ÔÌÔÏ·ÛÙÂﬂ ÙÔ ·Ò˜ÂﬂÔ",   
                                  	/* What to name the output archive -STDOUT*/
  "ºÌÔÏ· ÙÔı Mail-·Ò˜ÂﬂÔı „È· Ì· ‰È·‚·ÛÙÂﬂ",            /* Mail archive to read in -STDOUT*/
  "ƒÂﬂ˜ÌÂÈ ÙÁ Ò¸Ô‰Ô",                	/* Show progress           -STDOUT*/
  "ƒÂﬂ˜ÌÂÈ Ï¸ÌÔ ÙÈÚ ·Ò·Ï›ÙÒÔıÚ", 	/* Show configuration variables only -STDOUT*/
  "ƒÂﬂ˜ÌÂÈ ÎÁÒÔˆÔÒﬂÂÚ „È· ÙÁÌ ›Í‰ÔÛÁ Í·È ÙÂÎÂÈ˛ÌÂÈ",             /* Show version information and exit -STDOUT*/
  "¡Ì·ÌÂ˛ÌÂÈ ÙÔ ·Ò˜ÂﬂÔ ÏÂ ›Ì· ‹ÒËÒÔ",  	/* Update archive by one article -STDOUT*/
  "√Ò‹ˆÂÈ ‹Ì˘ ·Ô Ù· ÒÔÁ„Ô˝ÏÂÌ· ÏÁÌ˝Ï·Ù·", /* Overwrite previous messages -STDOUT*/
  " ·ËÔÒﬂÊÂÈ ÙÁ „Î˛ÛÛ· Ôı Ë· ˜ÒÁÛÈÏÔÔÈÁËÂﬂ",                      /* Specify language to use -STDOUT*/
  "◊ÒﬁÛÁ",                           	/* Usage -STDOUT*/
  "« „Î˛ÛÛ· ‰ÂÌ ıÔÛÙÁÒﬂÊÂÙ·È",		/* Language not supported  -STDERR*/
  "ƒÂÌ Ù›ËÁÍÂ",                        	/* Not set               -STDOUT*/
  "ƒÂÌ ˜ÒÁÛÈÏÔÔÈﬁËÁÍÂ",             	/* Not used              -STDOUT*/
  "ƒÁÏÈÔıÒ„ﬁËÁÍÂ ÂÈÛıÌ·Ù¸ÏÂÌÔ ·Ò˜ÂﬂÔ",           /* Created attachment file -STDOUT*/
  "≈ÈÛıÌ·Ù¸ÏÂÌÔ",                   	/* attachment           -HTML*/
  " ·Ù‹ÛÙ·ÛÁ",                        	/* mode                 -STDOUT*/
  "ƒÈ·‚‹Ê˘ Ù· ·ÎÈ‹ headers",          	/* Reading old headers  -STDOUT*/
  "",       				/* for alignment only -STDOUT*/
  "À¡»œ”",                       	/* ERROR                       -STDERR*/
  "« ‰ÈÂ˝ËıÌÛÁ ÙÁÚ ÎﬂÛÙ·Ú „È· ıÔ‚ÔÎﬁ emails", 
                           		/* The submission address of the list-STDERR*/
  "ƒÈ‹‚·ÛÂ Ï¸ÌÔ ›Ì· email ·Ô ÙÁÌ ÂﬂÛÔ‰Ô", /* Read only one mail from input */
  "”ı„„Ò·ˆ›·Ú",                        	/* author              -HTML*/
  "«ÏÂÒÔÏÁÌﬂ·",                      	/* date                -HTML*/
  "»›Ï·",                              	/* subject             -HTML*/
  "Mail ÂÌ›Ò„ÂÈÂÚ",                  	/* Mail actions (MA) header -HTML*/
  "”ÙÂﬂÎÂ ÂÌ· Í·ÈÌÔ˝ÒÈÔ ÏﬁÌıÏ· ",    	/* MA New Message      -HTML*/
  "¡‹ÌÙÁÛÂ ÛÂ ·ıÙ¸ ÙÔ ÏﬁÌıÏ·",      	/* MA Reply            -HTML*/
  "–ÂÒﬂÎÁ¯Á Ù˘Ì ÏÁÌÈ·ﬂ˘Ì ÂıÒÂÙﬁÒÈ˘Ì",  	/* monthly             -HTML*/
  "–ÂÒﬂÎÁ¯Á Ù˘Ì ÂÙﬁÛÈ˘Ì ÂıÒÂÙﬁÒÈ˘Ì",  	/* yearly              -HTML*/
  "Build a GDBM header cache",   	/* Build a GDBM header cache   -STDOUT*/
  "ƒÁÏÈÔıÒ„˛ ÙÔ gdbm ÂıÒÂÙﬁÒÈÔ... ",   	/* Creating gdbm index         -STDOUT*/
  "ƒÂÌ ÏÔÒ˛ Ì· ‰ÁÏÈÔıÒ„ﬁÛ˘ ÙÔ gdbm ·Ò˜ÂﬂÔ... ",  /* Can't create gdbm index     -STDOUT*/
  "ƒÈ·ÙﬁÒÁÛÂ ÙÔ mbox ·Ò˜ÂﬂÔ",    	/* Maintain an mbox archive    -STDOUT*/
  "ƒÂÌ ÂﬂÌ·È ‰ıÌ·Ù¸ Ì· ‰È·‚‹Û˘ Í·È Ì· „Ò‹¯˘ ÛÙÔ mbox Ù·ıÙ¸˜ÒÔÌ·", /* "Can't both read from and write to an mbox." -STDOUT*/
  "–ÒÔ‚ÔÎﬁ ≈ÈÛ˝Ì·¯ÁÚ",             	/* Attachment view       -HTML*/
  "¡Ì‹ ÏÁÌ˝Ï·Ù· ÏÂ ÂÈÛıÌ‹¯ÂÈÚ",	/* By messages with attachments -HTML*/
  "√Ò‹ˆ˘ ÙÔ ÂıÒÂÙﬁÒÈÔ Ù˘Ì ÂÈÛıÌ·Ù˛ÏÂÌ˘Ì",	  /* Writing attachment index to -STDOUT*/
  "bytes",		    		/* file size, so far only for attachments  -HTML */
  "ƒÂÌ ÏÔÒ˛ Ì· ‰ÁÏÈÔıÒ„ﬁÛ˘ ÙÔ ÛıÏ‚ÔÎÈÍ¸ Û˝Ì‰ÂÛÏÔ", /* MSG_CANNOT_CREATE_SYMLINK     -STDOUT */
  "ƒÂÌ ÏÔÒ˛ Ì· ·ÔÏ·ÍÒ˝Ì˘ ÙÔ ÏﬁÌıÏ·", 	/* MSG_CANNOT_UNLINK                       -STDOUT */
  "–ÒÔÁ„Ô˝ÏÂÌÔÚ Í·Ù‹ÎÔ„ÔÚ",  		/* MSG_PREV_DIRECTORY                     -HTML */
  "≈¸ÏÂÌÔÚ Í·Ù‹ÎÔ„ÔÚ",      		/* MSG_NEXT_DIRECTORY                     -HTML */
  "ÀﬂÛÙ· Í·Ù·Î¸„˘Ì",  			/* MSG_FOLDERS_INDEX                      -HTML */
  "¡ıÙ¸ ÙÔ ÏﬁÌıÏ· ›˜ÂÈ Û‚ÁÛÙÂﬂ ·Ô ÙÔ ·Ò˜ÂﬂÔ", /* MSG_DELETED -HTML */
  "¡ıÙ¸ ÙÔ ÏﬁÌıÏ· ›˜ÂÈ ÎﬁÓÂÈ",      	/* MSG_EXPIRED -HTML */
  "(Û‚ÁÛÏ›ÌÔ ÏﬁÌıÏ·)", 			/* MSG_DEL_SHORT                            -HTML */
  "–Ò¸ÙıÔ ÍÂﬂÏÂÌÔ ÙÔı ÏÁÌ˝Ï·ÙÔÚ", 	/* MSG_TXT_VERSION                          -HTML */
  "¡ıÙ¸ ÙÔ ÏﬁÌıÏ· ›˜ÂÈ ˆÈÎÙÒ·ÒÈÛÙÂﬂ ",  /* MSG_FILTERED_OUT -HTML */
  "”ı„„Ò·ˆ›·Ú",       			/* MSG_FROM              -HTML*/
  "This message",                 /* This message -HTML */
  "Message body",                 /* Message body -HTML */
  "Respond",                      /* Respond -HTML */
  "More options",                 /* More options -HTML */
  "Related messages",             /* Related messages -HTML */
  "Next",                         /* Next -HTML */
  "Previous",                     /* Previous -HTML */
  "Replies",                      /* Replies -HTML */
  "Contemporary messages sorted", /* Contemporary messages sorted -HTML */
  "Help",                         /* Help  -HTML */
  "Message to which this message replies", /* In Reply To - HTML link*/
  "Next message in this discussion thread", /* Next message in thread - HTML link*/
  "Message sent in reply to this message", /* Replies to this message - HTML link*/
  "Contemporary messages by date", /* Contemporary messages by date */
  "Contemporary discussion threads", /* Contemporary messages by threads -  HTML */
  "Contemporary messages by subject", /* Contemporary messages by subject - HTML */
  "Contemporary messages by author", /* Contemporary messages by author - HTML*/
  "Next message in the list", /* Next message - HTML */
  "Previous message in the list", /* Previous message - HTML */
  "Contemporary messages by attachment", /* Contemporary messages by attachments - HTML*/
  "Navigation bar to upper levels", /* Navigation bar, upper levels - HTML*/
  "Navigation bar", /* Navigation bar - HTML*/
  "sort by", /* Sort by - HTML*/
  "Other periods", /* Other periods - HTML */
  "Next", /* Next folder - HTML */
  "Messages archived in the next period, sorted by date", /* Next folder, by date - HTML link */
  "Previous", /* Previous folder - HTML */
  "Messages archived in the previous period, sorted by date", /* Previous folder, by date - HTML Link */
  "List of all periods", /* List of all periods - HTML*/
  "This period", /* This period - HTML*/
  "Received on", /* Received on - HTML */
  "Nearby", /* Nearby - HTML */
  "re-sorted", /* Resorted - HTML */
  "listed by date", /* listed by date - HTML */
  "by discussion threads" , /* discussion threads - HTML */
  "listed by author", /* listed by author - HTML */
  "by subject", /* listed by subject - HTML */
  "by attachment", /* by attachment - HTML */
  "period", /* period - HTML */
  " to ", /* to - HTML */
  "from", /* from - HTML */
  "on", /* on - HTML */
  "message archived in another list or period", /* unknown in reply to - HTML */
  "¡ıÙ¸ ÙÔ ÏﬁÌıÏ· ›˜ÂÈ Û‚ÁÛÙÂﬂ ·Ô ÙÔ ·Ò˜ÂﬂÔ", /* MSG_DELETED_OTHER -HTML */
  "Note: this message has been edited and differs from the originally archived copy.", /* MSG_EDITED -HTML */
  NULL,                           	/* End Of Message Table - NOWHERE*/
};

/*
** Russian version of the language table. 
** Translated by Igor Solovyoff (siv@intear.com.ua).
*/

char *ru[] = {     /* Russian */
  "Óœ◊œ≈ ”œœ¬›≈Œ…≈",             /* New Message        -HTML*/
  "Ô‘◊≈‘",                       /* Reply              -HTML*/
  "Ô ”–…”À≈",                    /* About this list    -HTML*/
  "ÎœŒ≈√ ”œœ¬›≈Œ… ",             /* End of Messages    -HTML*/
  "Ó¡ﬁ¡Ãœ ”œœ¬›≈Œ… ",            /* Start of Messages  -HTML*/
  "œ ƒ¡‘≈",                     /* Date view          -HTML*/
  "œ Œ…‘—Õ",                    /* Thread view        -HTML*/
  "œ ‘≈Õ¡Õ",                    /* Subject view       -HTML*/
  "œ ¡◊‘œ“¡Õ",                  /* Author view        -HTML*/
  "‰“’«…≈ «“’––Ÿ",               /* Other groups       -HTML*/
  "Ûœœ¬›≈Œ…—",                   /* Messages           -HTML*/
  "Ó¡ﬁ¡Ãœ",                      /* Starting           -HTML*/
  "ÎœŒ≈√",                       /* Ending             -HTML*/
  "Ô¬ ‹‘œÕ ¡“»…◊≈",              /* About this archive -HTML*/
  "œ‘”œ“‘…“œ◊¡ŒŸ –œ",            /* Messages sorted by -HTML*/
  "‰“’«…≈ –œﬁ‘œ◊Ÿ≈ ¡“»…◊Ÿ",      /* Other mail archives -HTML*/
  "œ ƒ¡‘≈",                     /* By Date             -HTML*/
  "Û¡ÕŸ≈ –œ”Ã≈ƒŒ…≈ ”œœ¬›≈Œ…—",   /* Most recent messages-HTML*/
  "¡◊‘œ“",                       /* author              -HTML*/
  "ƒ¡‘¡",                        /* date                -HTML*/
  "Œ…‘ÿ",                        /* thread              -HTML*/
  "‘≈Õ¡",                        /* subject             -HTML*/
  "ƒÃ— œ–√… ",                   /* for options       -STDOUT*/
  "…€’ ”œœ¬›≈Œ…≈ ◊",           /* Writing messages to-STDOUT*/
  "…€’ date index ◊",          /* Writing date index to-STDOUT*/
  "…€’ thread index ◊",        /* Writing thread index to -STDOUT*/
  "…€’ subject index ◊",       /* Writing subject index to-STDOUT*/
  "…€’ author index ◊",        /* Writing author index to-STDOUT*/
  "‰¡‘¡ –œ”Ã≈ƒŒ≈«œ ”œœ¬›≈Œ…—",   /* Last message date   -HTML*/
  "·“»…◊…“œ◊¡Œœ ◊",             /* Archived on         -HTML*/
  "Ó≈ Õœ«’ chmod",               /* Can not chmod     -STDERR*/
  "Ó≈ Õœ«’ ⁄¡–…”¡‘ÿ",              /* Could not write   -STDERR*/
  "ÛÃ≈ƒ’¿›≈≈ ”œœ¬›≈Œ…≈",         /* Next message        -HTML*/
  "“≈ƒŸƒ’›≈≈ ”œœ¬›≈Œ…≈",        /* Previous message    -HTML*/
  "˜œ⁄Õœ÷Œœ ‹‘œ œ‘◊≈‘ Œ¡",        /* Maybe in reply to   -HTML*/
  "¸‘œ œ‘◊≈‘ Œ¡",                 /* In reply to         -HTML*/
  "ÛÃ≈ƒ’¿›≈≈ ◊ Œ…‘…",            /* Next in thread      -HTML*/
  "˜œ⁄Õœ÷Œœ œ‘◊≈‘",              /* Maybe reply         -HTML*/
  "œ Œ…‘—Õ",                    /* By Thread           -HTML*/
  "œ ‘≈Õ¡Õ",                    /* By Subject          -HTML*/
  "œ ¡◊‘œ“¡Õ",                  /* By Author           -HTML*/
  "Ó≈ Õœ«’ ”œ⁄ƒ¡‘ÿ ƒ…“≈À‘œ“…¿",  /* Can not create directory -STDERR*/
  "Ûœ⁄ƒ¡¿ ƒ…“≈À‘œ“…¿",           /* Creating directory -STDOUT*/
  "¡“¡Õ≈‘“Ÿ ÀœŒ∆…«’“¡√……",      /* Configuration Values -STDOUT*/
  "–’‘ÿ",                        /* path              -STDOUT*/
  "Ó≈ »◊¡‘¡≈‘ –¡Õ—‘…!",          /* Ran out of memory!-STDERR*/
  "¸‘œ‘ ¡“»…◊ ¬ŸÃ ”«≈Œ≈“…“œ◊¡Œ", /* This archive was generated by-HTML*/
  "¸Ã≈Õ≈Œ‘œ◊",                   /* Elements          -STDOUT*/
  "Ó≈‘ ‹Ã≈Õ≈Œ‘œ◊",               /* No Elements       -STDOUT*/
  "Ó≈ Õœ«’ œ‘À“Ÿ‘ÿ –œﬁ‘œ◊Ÿ  ¡“»…◊", /* Cannot open mail archive */
  "˛…‘¡¿ Œœ◊Ÿ  ⁄¡«œÃœ◊œÀ...",    /* Reading new header...-STDOUT   */
  "˙¡«“’÷¡¿ mailbox",            /* Loading mailbox      -STDOUT   */
  "Àœƒ…“œ◊À¡ Œ≈ –œƒƒ≈“÷…◊¡≈‘”—, ⁄¡–…”¡Œœ À¡ ≈”‘ÿ", /* encoding is not supported, stored as-is -HTML*/
  "”œœ¬›.",                      /* messages             -HTML*/
  "˜≈“”…—",                      /* Version            -STDOUT*/
  "Patchlevel",                  /* Patchlevel         -STDOUT*/
  "‰œÀ’Õ≈Œ‘œ◊",                  /* Docs               -STDOUT*/
  "ÎœÕ¡ŒƒŒŸ≈ … ’–“¡◊Ã—¿›…≈ –¡“¡Õ≈‘“Ÿ",       /* Command and Control Variables-STDOUT*/
  "‰¡",                          /* Yes                -STDOUT*/
  "Ó≈‘",                         /* No                 -STDOUT*/
  "Ô¬ ¡“»…◊≈: Œ≈ …”–œÃÿ⁄œ◊¡Œ", /* About the archive: not used-STDOUT */
  "‰“’«…≈ ¡“»…◊Ÿ: Œ≈ …”–œÃÿ⁄œ◊¡ŒŸ",    /* Other archives: not used-STDOUT*/
  "¡ƒ“≈” Œ≈ …”–œÃÿ⁄œ◊¡Œ",            /* address not used        -STDOUT*/
  "È”–œÃÿ⁄œ◊¡Œ ◊”‘“œ≈ŒŒŸ  œ–≈“¡‘œ“ <BODY>", /* Builtin <BODY> statement used-STDOUT*/
  "Ó≈ Õœ«’ œƒŒœ◊“≈Õ≈ŒŒœ ﬁ…‘¡‘ÿ … ∆¡ Ã and stdin.", /* Cannot read from both file and stdin. -STDERR*/
  "Ô–√……",                     /* Options                -STDOUT*/
  "URL À ƒ“’«…Õ ¡“»…◊¡Õ",       /* URL to other archives  -STDOUT*/
  "URL À ¡“»…◊Œœ  …Œ∆œ“Õ¡√……",  /* URL to archive information -STDOUT*/
  "ÎœŒ∆…«’“¡√…œŒŒŸ  ∆¡ Ã ƒÃ— ﬁ‘≈Œ…— ”", /* Configuration file to read in -STDOUT*/
  "‰…“≈À‘œ“…— ƒÃ— ”œ»“¡Œ≈Œ…— HTML ∆¡ Ãœ◊ ◊", /* The directory to save HTML files in -STDOUT*/
  "˛‘≈Œ…≈ ”œœ¬›≈Œ…  ”œ ”‘¡Œƒ¡“Œ‘Œœ«œ ◊◊œƒ¡",/* Read messages from standard input -STDOUT*/
  "Î¡Àœ≈ …Õ— ◊Ÿ»œƒŒœ«œ ¡“»…◊¡", /* What to name the output archive -STDOUT*/
  "œﬁ‘œ◊Ÿ  ¡“»…◊ ƒÃ— ﬁ‘≈Œ…— ”",     /* Mail archive to read in -STDOUT*/
  "œÀ¡⁄¡‘ÿ –“œ«“≈””",               /* Show progress           -STDOUT*/
  "œÀ¡⁄¡‘ÿ ‘œÃÿÀœ –¡“¡Õ≈‘“Ÿ ÀœŒ∆…«’“¡√……", /* Show configuration variables only -STDOUT*/
  "œÀ¡⁄¡‘ÿ ◊≈“”…¿ … ◊Ÿ ‘…", /* Show version information and exit -STDOUT*/
  "Ô¬Œœ◊…‘ÿ ¡“»…◊ –œ œƒŒœ  ”‘¡‘ÿ≈", /* Update archive by one article -STDOUT*/
  "≈“≈⁄¡–…”¡‘ÿ –“≈ƒŸƒ’›…≈ ”œœ¬›≈Œ…—", /* Overwrite previous messages -STDOUT*/
  "ıÀ¡÷…‘≈ …”–œÃÿ⁄’≈ÕŸ  —⁄ŸÀ",     /* Specify language to use     -STDOUT*/
  "È”–œÃÿ⁄œ◊¡Œ…≈",                       /* Usage                       -STDOUT*/
  "Ò⁄ŸÀ Œ≈ –œƒƒ≈“÷¡Œ",      /* Language not supported      -STDERR*/
  "Ó≈ ’”‘¡Œœ◊Ã≈Œœ",                     /* Not set                     -STDOUT*/
  "Ó≈ …”–œÃÿ⁄’≈‘”—",                    /* Not used                    -STDOUT*/
  "Ûœ⁄ƒ¡Œ ∆¡ Ã-◊Ãœ÷≈Œ…≈",     /* Created attachment file     -STDOUT*/
  "◊Ãœ÷≈Œ…≈",                  /* attachment                    -HTML*/
  "“≈÷…Õ",                        /* mode                        -STDOUT*/
  "˛…‘¡¿ ”‘¡“Ÿ≈ ⁄¡«œÃœ◊À…",         /* Reading old headers         -STDOUT*/
  "", /* for alignment only       -STDOUT*/
  "Ô˚È‚Î·",                       /* ERROR                       -STDERR*/
  "“≈ƒ”‘¡◊Ã≈ŒŒŸ≈ ◊ ◊–…”À≈ ¡ƒ“≈”¡", /* The submission address of the list-STDERR*/
  "˛‘≈Œ…≈ ‘œÃÿÀœ œƒŒœ«œ ”œœ¬›≈Œ…— ”œ ◊◊œƒ¡",
  "·◊‘œ“",                      /* author              -HTML*/
  "‰¡‘¡",                        /* date                -HTML*/
  "Ù≈Õ¡",                     /* subject             -HTML*/
  "‰≈ ”‘◊…— ” –œﬁ‘œ ",                /* Mail actions (MA) header -HTML*/
  "œ”Ã¡‘ÿ Œœ◊’¿ ”‘¡‘ÿ¿",            /* MA New Message      -HTML*/
  "Ô‘◊≈‘…‘ÿ Œ¡ ‹‘œ ”œœ¬›≈Œ…≈",     /* MA Reply            -HTML*/
  "Û’ÕÕ¡“Œœ ⁄¡ Õ≈”—√ …Œƒ≈À”ŒŸ» ∆¡ Ãœ◊",       /* monthly             -HTML*/
  "Û’ÕÕ¡“Œœ ⁄¡ «œƒ …Œƒ≈À”ŒŸ» ∆¡ Ãœ◊",        /* yearly              -HTML*/
  "œ”‘“œ≈Œ…≈ GDBM ⁄¡«œÃœ◊À¡ À‹€¡",   /* Build a GDBM header cache   -STDOUT*/
  "Ô–√…— GDBM header cache Œ≈ ◊”‘“œ≈Œ¡",/* GDBM header cache option not build in  -STDERR*/
  "Û‘“œ¿ gdbm …Œƒ≈À”... ",     /* Creating gdbm index         -STDOUT*/
  "Ó≈ Õœ«’ ”œ⁄ƒ¡‘ÿ gdbm ∆¡ Ã... ",  /* Can't create gdbm index     -STDOUT*/
  "Ô¬”Ã’÷…◊¡Œ…≈ mbox ¡“»…◊¡",    /* Maintain an mbox archive    -STDOUT*/
  "Ó≈ Õœ«’ œƒŒœ◊“≈Õ≈ŒŒœ … ﬁ…‘¡‘ÿ … –…”·‘ÿ …⁄ mbox-¡.", /* "Can't both read from and write to an mbox." -STDOUT*/
  "œ ◊Ãœ÷≈Œ…—Õ",   /* Attachment view       -HTML*/
  "œ ”œœ¬›≈Œ…—Õ ” ◊Ãœ÷≈Œ…—Õ…",   /* By messages with attachments -HTML*/
  "˙¡–…”Ÿ◊¡¿ …Œƒ≈À” –œ ◊Ãœ÷≈Œ…—Õ ◊",   /* Writing attachment index to -STDOUT*/
  "¬¡ ‘",     /* file size, so far only for attachments  -HTML */
  "Ó≈ Õœ«’ ”œ⁄ƒ¡‘ÿ ”…Õ◊œÃ…ﬁ≈”À’¿ ””ŸÃÀ’", /* MSG_CANNOT_CREATE_SYMLINK     -STDOUT */
  "Ó≈ Õœ«’ ’ƒ¡Ã…‘ÿ ∆¡ Ã", /* MSG_CANNOT_UNLINK                       -STDOUT */
  "“≈ƒŸƒ’›¡— ƒ…“≈À‘œ“…—",  /* MSG_PREV_DIRECTORY                     -HTML */
  "ÛÃ≈ƒ’¿›¡— ƒ…“≈À‘œ“…—",      /* MSG_NEXT_DIRECTORY                     -HTML */
  "Û–…”œÀ ƒ…“≈À‘œ“… ",  /* MSG_FOLDERS_INDEX                      -HTML */
  "¸‘œ ”œœ¬›≈Œ…≈ ¬ŸÃœ ’ƒ¡Ã≈Œœ …» ¡“»…◊¡", /* MSG_DELETED -HTML */
  "¸‘œ ”œœ¬›≈Œ…≈ –“œ”“œﬁ≈Œœ",                       /* MSG_EXPIRED -HTML */
  "(’ƒ¡Ã≈ŒŒœ≈ ”œœ¬›≈Œ…≈)", /* MSG_DEL_SHORT                            -HTML */
  "È”»œƒŒŸ  ‘≈À”‘ ƒ¡ŒŒœ«œ ”œœ¬›≈Œ…—", /* MSG_TXT_VERSION                          -HTML */
  "¸‘œ ”œœ¬›≈Œ…≈ ¬ŸÃœ œ‘∆…Ãÿ‘“œ◊¡Œœ",        /* MSG_FILTERED_OUT -HTML */
  "Ô‘",                        /* MSG_FROM                      -HTML*/
  "˙¡–…”ÿ haof XML ∆¡ Ãœ◊",       /* Write hoaf XML files       -STDOUT */
  "˙¡–…”Ÿ◊¡¿ haof ◊",            /* Writing haof to               -HTML*/
  "This message",                 /* This message -HTML */
  "Message body",                 /* Message body -HTML */
  "Respond",                      /* Respond -HTML */
  "More options",                 /* More options -HTML */
  "Related messages",             /* Related messages -HTML */
  "Next",                         /* Next -HTML */
  "Previous",                     /* Previous -HTML */
  "Replies",                      /* Replies -HTML */
  "Contemporary messages sorted", /* Contemporary messages sorted -HTML */
  "Help",                         /* Help  -HTML */
  "Message to which this message replies", /* In Reply To - HTML link*/
  "Next message in this discussion thread", /* Next message in thread - HTML link*/
  "Message sent in reply to this message", /* Replies to this message - HTML link*/
  "Contemporary messages by date", /* Contemporary messages by date */
  "Contemporary discussion threads", /* Contemporary messages by threads -  HTML */
  "Contemporary messages by subject", /* Contemporary messages by subject - HTML */
  "Contemporary messages by author", /* Contemporary messages by author - HTML*/
  "Next message in the list", /* Next message - HTML */
  "Previous message in the list", /* Previous message - HTML */
  "Contemporary messages by attachment", /* Contemporary messages by attachments - HTML*/
  "Navigation bar to upper levels", /* Navigation bar, upper levels - HTML*/
  "Navigation bar", /* Navigation bar - HTML*/
  "sort by", /* Sort by - HTML*/
  "Other periods", /* Other periods - HTML */
  "Next", /* Next folder - HTML */
  "Messages archived in the next period, sorted by date", /* Next folder, by date - HTML link */
  "Previous", /* Previous folder - HTML */
  "Messages archived in the previous period, sorted by date", /* Previous folder, by date - HTML Link */
  "List of all periods", /* List of all periods - HTML*/
  "This period", /* This period - HTML*/
  "Received on", /* Received on - HTML */
  "Nearby", /* Nearby - HTML */
  "re-sorted", /* Resorted - HTML */
  "listed by date", /* listed by date - HTML */
  "by discussion threads" , /* discussion threads - HTML */
  "listed by author", /* listed by author - HTML */
  "by subject", /* listed by subject - HTML */
  "by attachment", /* by attachment - HTML */
  "period", /* period - HTML */
  " to ", /* to - HTML */
  "from", /* from - HTML */
  "on", /* on - HTML */
  "message archived in another list or period", /* unknown in reply to - HTML */
  "¸‘œ ”œœ¬›≈Œ…≈ ¬ŸÃœ ’ƒ¡Ã≈Œœ …» ¡“»…◊¡", /* MSG_DELETED_OTHER -HTML */
  "Note: this message has been edited and differs from the originally archived copy.", /* MSG_EDITED -HTML */
  NULL,                          /* End Of Message Table      - NOWHERE*/
};


/*
** list of supported languages
*/

struct language_entry ltable[] = {
{    "de",      de,    "de_DE"   },     /* German  */
{    "en",      en,    "en_US"   },     /* English */
{    "es",      es,    "es_ES"   },     /* Spanish */
{    "fi",      fi,    "fi_FI"   },     /* Finnish */
{    "fr",      fr,    "fr_FR"   },     /* French  */
{    "is",      is,    "is_IS"   },     /* Icelandic */
{    "pl",      pl,    "pl_PL"   },     /* Polish  */
{    "pt",      pt,    "pt_BR"   },     /* Brazilian Portuguese */
{    "sv",      sv,    "sv_SE"   },     /* Swedish */
{    "no",      no,    "no_NO"   },     /* Norwegian */
{    "el",      gr,    "el"   	 },     /* Greek */
{    "gr",      gr,    "el_GR"   },     /* Greek */
{    "ru",      ru,    "ru_RU"   },     /* Russian */
{    "it",      it,    "it_IT"   },     /* Italian */
{     NULL,     NULL,  NULL	 },     /* EOL    */
};

/*
** Default language table
*/
char **lang = en;

#else

extern char **lang;
extern struct language_entry ltable[];

#endif
