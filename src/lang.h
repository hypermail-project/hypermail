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
#define MSG_OTHER_GROUPS           9
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
#define MSG_CREATING_GDBM_INDEX                  100
#define MSG_CANT_CREATE_GDBM_INDEX               101
#define MSG_OPTION_ARCHIVE                       102
#define MSG_CANNOT_BOTH_READ_AND_WRITE_TO_MBOX   103

/* XXX Move these up with the others */
#define MSG_ATTACHMENT_VIEW			 104
#define MSG_BY_ATTACHMENT			 105
#define MSG_WRITING_ATTACHMENT_INDEX  		 106
#define MSG_BYTES                                107

#define MSG_CANNOT_CREATE_SYMLINK                108
#define MSG_CANNOT_UNLINK                        109
#define MSG_PREV_DIRECTORY                       110
#define MSG_NEXT_DIRECTORY                       111
#define MSG_FOLDERS_INDEX                        112
#define MSG_DELETED                              113
#define MSG_EXPIRED                              114
#define MSG_DEL_SHORT                            115
#define MSG_TXT_VERSION                          116
#define MSG_FILTERED_OUT                         117

#ifdef MAIN_FILE

/*
** German version of the language table
** Translation by Martin Schulze <joey@kuolema.Infodrom.North.DE> 
*/

char *de[] = {       /* German */
  "Neue Nachricht",                       /* New Message        -HTML*/
  "Antwort",                              /* Reply              -HTML*/
  "Über diese Liste",                  /* About this list    -HTML*/
  "Ende der Nachricht",                   /* End of Messages    -HTML*/
  "Anfang der Nachricht",                 /* Start of Messages  -HTML*/
  "Datumsansicht",                         /* Date view          -HTML*/
  "Baumansicht",                         /* Thread view        -HTML*/
  "Betreffansicht",                        /* Subject view       -HTML*/
  "Autorenansicht",                          /* Author view        -HTML*/
  "Weitere Gruppen",                      /* Other groups       -HTML*/
  "Nachrichten",                          /* Messages           -HTML*/
  "Anfang",                               /* Starting           -HTML*/
  "Ende",                                 /* Ending             -HTML*/
  "Über dieses Archiv",                /* About this archive -HTML*/
  "sortiert nach",            /* sorted by -HTML*/
  "Weitere Mail-Archive",                 /* Other mail archives -HTML*/
  "Datum",                                /* By Date             -HTML*/
  "Neueste Nachrichten",                  /* Most recent messages-HTML*/
  "Autor",                                /* author              -HTML*/
  "Datum",                                /* date                -HTML*/
  "Thread",                               /* thread              -HTML*/
  "Subject",                              /* subject             -HTML*/
  "für Optionen",                      /* for options       -STDOUT*/
  "Schreibe Nachrichten in",              /* Writing messages to-STDOUT*/
  "Schreibe Datums-Index in",             /* Writing date index to-STDOUT*/
  "Schreibe Thread-Index in",             /* Writing thread index to -STDOUT*/
  "Schreibe Subject-Index in",            /* Writing subject index to-STDOUT*/
  "Schreibe Autoren-Index in",            /* Writing author index to-STDOUT*/
  "Letztes Datum",                        /* Last message date   -HTML*/
  "Archiviert auf",                       /* Archived on         -HTML*/
  "Kann Dateimodus nicht ändern",      /* Can not chmod     -STDERR*/
  "Kann nicht schreiben",                 /* Could not write   -STDERR*/
  "Nächste Nachricht",                 /* Next message        -HTML*/
  "Vorherige Nachricht",                  /* Previous message    -HTML*/
  "Vielleicht als Antwort auf",           /* Maybe in reply to   -HTML*/
  "Als Antwort auf",                      /* In reply to         -HTML*/
  "Nächste im Thread",                 /* Next in thread      -HTML*/
  "Eventuelle Antwort",                   /* Maybe reply         -HTML*/
  "Nach Thread",                          /* By Thread           -HTML*/
  "Nach Subject",                         /* By Subject          -HTML*/
  "Nach Autor",                           /* By Author           -HTML*/
  "Kann das Verzeichnis nicht erstellen", /* Can not create directory -STDERR*/
  "Erstelle Verzeichnis",                 /* Creating directory -STDOUT*/
  "Konfiguration",                        /* Configuration Values -STDOUT*/
  "Pfad",                                 /* path              -STDOUT*/
  "Kein Speicher mehr!",                  /* Ran out of memory!-STDERR*/
  "Dieses Archiv wurde generiert von",    /* This archive was generated by-HTML*/
  "Elemente",                             /* Elements          -STDOUT*/
  "Keine Elemente",                       /* No Elements       -STDOUT*/
  "Kann Mail-Archiv nicht öffnen",     /* Cannot open mail archive */
  "Lese neue Header...",                  /* Reading new header...-STDOUT*/
  "Lade Mailbox",                         /* Loading mailbox      -STDOUT*/
  "Kodierung wird nicht unterstützt, speichere so", 
                        /* encoding is not supported, stored as-is -HTML*/
  "Nachrichten",                          /* messages             -HTML*/
  "Version",                              /* Version            -STDOUT*/
  "Patchlevel",                           /* Patchlevel         -STDOUT*/
  "Dokus",                                /* Docs               -STDOUT*/
  "Befehls- und Kontrollvariablen",       /* Command and Control Variables-STDOUT*/
  "Ja",                                   /* Yes                -STDOUT*/
  "Nein",                                 /* No                 -STDOUT*/
  "Über das Archiv: nicht benutzt",    /* About the archive: not used-STDOUT */
  "Andere Archive: nicht benutzt",        /* Other archives: not used-STDOUT*/
  "Adresse nicht benutzt",                /* address not used        -STDOUT*/
  "Eingebautes <BODY> benutzt",           /* Builtin <BODY> statement used-STDOUT*/
  "Kann nicht von Datei und stdin lesen.", /* Cannot read from both file and stdin. -STDERR*/
  "Optionen",                             /* Options                -STDOUT*/
  "URL für weitere Archive",           /* URL to other archives  -STDOUT*/
  "URL für Informationen über Archiv",  
                                      /* URL to archive information -STDOUT*/
  "Konfigurationsdatei zum Einlesen", 
                                    /* Configuration file to read in -STDOUT*/
  "Das Verzeichnis, um die HTML-Dateien zu speichern", 
                              /* The directory to save HTML files in -STDOUT*/
  "Lese Nachrichten von der Standard Eingabe",
                                /* Read messages from standard input -STDOUT*/
  "Wie soll das Archiv genannt werden",   
                                  /* What to name the output archive -STDOUT*/
  "Mail-Archive zum Einlesen",            /* Mail archive to read in -STDOUT*/
  "Zeige Zwischenbericht",                /* Show progress           -STDOUT*/
  "Zeigt nur die Konfigurationsvariablen", /* Show configuration variables only -STDOUT*/
  "Zeigt Version und beende",             /* Show version information and exit -STDOUT*/
  "Jeweils einen Artikel hinzufügen",  /* Update archive by one article -STDOUT*/
  "Vorherige Nachrichten überschreiben", /* Overwrite previous messages -STDOUT*/
  "Sprache angeben",                      /* Specify language to use -STDOUT*/
  "Usage",                                /* Usage -STDOUT*/
  "Die Sprache wird nicht unterstützt",/* Language not supported  -STDERR*/
  "Nicht gesetzt",                        /* Not set               -STDOUT*/
  "Nicht benutzt",                        /* Not used              -STDOUT*/
  "Attachement-Datei angelegt",           /* Created attachment file -STDOUT*/
  "Attachement",                          /* attachment           -HTML*/
  "Mode",                                 /* mode                 -STDOUT*/
  "Lese alte Header",                     /* Reading old headers  -STDOUT*/
  "",       /* for alignment only -STDOUT*/
  "ERROR",                       /* ERROR                       -STDERR*/
  "The submission address of the list", 
                           /* The submission address of the list-STDERR*/
  "Read only one mail from input",
  "Autor",                                /* author              -HTML*/
  "Datum",                                /* date                -HTML*/
  "Subject",                              /* subject             -HTML*/
  "Mail actions",                         /* Mail actions (MA) header -HTML*/
  "mail a new topic",                     /* MA New Message      -HTML*/
  "respond to this message",              /* MA Reply            -HTML*/
  "Summary of Monthly Index Files",       /* monthly             -HTML*/
  "Summary of Yearly Index Files",        /* yearly              -HTML*/
  "Build a GDBM header cache",   /* Build a GDBM header cache   -STDOUT*/
  "Creating gdbm index... ",     /* Creating gdbm index         -STDOUT*/
  "Can't create gdbm file... ",  /* Can't create gdbm index     -STDOUT*/
  "Maintain an mbox archive",    /* Maintain an mbox archive    -STDOUT*/
  "Can't both read from and write to an mbox.", /* "Can't both read from and write to an mbox." -STDOUT*/
  "Attachement-Sicht",                    /* Attachment view       -HTML*/
  "Nach Nachtrichten mit Attachement",	  /* By messages with attachments -HTML*/
  "Schreibe Attachements-Index in",	  /* Writing attachment index to -STDOUT*/
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
  NULL,                                   /* End Of Message Table - NOWHERE*/
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
  "Other groups",                /* Other groups       -HTML*/
  "Messages",                    /* Messages           -HTML*/
  "Starting",                    /* Starting           -HTML*/
  "Ending",                      /* Ending             -HTML*/
  "About this archive",          /* About this archive -HTML*/
  "sorted by",                   /* Messages sorted by -HTML*/
  "Other mail archives",         /* Other mail archives -HTML*/
  "By Date",                     /* By Date             -HTML*/
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
  "By Thread",                   /* By Thread           -HTML*/
  "By Subject",                  /* By Subject          -HTML*/
  "By Author",                   /* By Author           -HTML*/
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
  "Creating gdbm index... ",     /* Creating gdbm index         -STDOUT*/
  "Can't create gdbm file... ",  /* Can't create gdbm index     -STDOUT*/
  "Maintain an mbox archive",    /* Maintain an mbox archive    -STDOUT*/
  "Can't both read from and write to an mbox.", /* "Can't both read from and write to an mbox." -STDOUT*/
  "Attachment view",			  /* Attachment view       -HTML*/
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
  NULL,                          /* End Of Message Table      - NOWHERE*/
};

/*
** Spanish version of the language table
** Translation by Francisco Iacobelli <fiacobelli@ibersis.cl>.
*/

char *es[] = {       /* Espanol/Spanish */
  "Nuevo Mensaje",                    /* New Message - HTML */
  "Responder",                        /* Reply  - HTML*/
  "Acerca de la Lista",               /* About this list  - HTML*/
  "Fin de los Mensajes",              /* End of Messages  - HTML*/
  "Inicio de Mensajes",               /* Start of Messages  - HTML*/
  "Por Fecha",                        /* Date view  - HTML*/
  "Por Seguimiento",                  /* Thread view  - HTML*/
  "Por Tema",                         /* Subject view  - HTML*/
  "Por Autor",                        /* Author view  - HTML*/
  "Otros Grupos",                     /* Other groups  - HTML*/
  "Mensajes",                         /* Messages  - HTML*/
  "Inicio",                           /* Starting  - HTML*/
  "Fin",                              /* Ending  - HTML*/
  "Acerca de este archivo",           /* About this archive - HTML */
  "Ordenados Por",                    /* Messages sorted by  - HTML*/
  "Otros archivos de correo",         /* Other mail archives  - HTML*/
  "Por Fecha",                        /* By Date  - HTML*/
  "Mensajes recientes",               /* Most recent messages - HTML */
  "autor",                            /* author - HTML*/
  "fecha",                            /* date - HTML*/
  "seguimiento",                      /* thread - HTML*/
  "tema",                             /* subject - HTML*/
  "para las opciones",                /* for options - STDOUT*/
  "Escribiendo Mensajes en",          /* Writing articles to - STDOUT*/
  "Escribiendo Indice de Fechas en",  /* Writing date index to - STDOUT*/
  "Escribiendo Indice de Seguimientos en", /* Writing thread index to  - STDOUT*/
  "Escribiendo Indice de Temas en",   /* Writing subject index to - STDOUT*/
  "Escribiendo Indice de Autores en", /* Writing author index to  - STDOUT*/
  "Fecha del mensaje final",          /* Last message date - HTML */
  "Archivado el ",                    /* Archived on  - HTML*/
  "No se puede ejecutar chmod",       /* Can not chmod - STDERR */
  "No se pudo escribir",              /* Could not write - STDERR */
  "Mensaje siguiente",                /* Next message  - HTML*/
  "Mensaje anterior",                 /* Previous message  - HTML*/
  "Respuesta posible a ",             /* Maybe in reply to  - HTML*/
  "En Respuesta a",                   /* In reply to  - HTML*/
  "Siguiente en el seguimiento",      /* Next in thread  - HTML*/
  "Posible respuesta",                /* Maybe reply  - HTML*/
  "Por Seguimiento",                  /* By Thread  - HTML*/
  "Por Tema",                         /* By Subject  - HTML*/
  "Por Autor",                        /* By Author  - HTML*/
  "No se puede crear el directorio",  /* Can not create directory - STDERR */
  "Creando el directorio",            /* Creating directory - STDOUT */
  "Valores de Conf.",                 /* Configuration Values - STDOUT */
  "ruta",                             /* path - STDOUT */
  "No hay suficiente memoria!",       /* Ran out of memory! - STDERR */
  "Este archivo fue generado por",    /* This archive was generated by - HTML*/
  "Elementos",                        /* Elements - STDOUT */
  "No Existen Elementos",             /* No Elements - STDOUT */
  "No se puede abrir el archivo de correo", /* Cannot open mail archive - STDERR */
  "Leyendo el encabezado nuevo...",   /* Reading new header... - STDOUT */
  "Cargando la casilla",              /* Loading mailbox - STDOUT */
  "No se puede codificar en este momento, se graba -tal cual-", 
                          /* encoding is not supported, stored as-is - HTML */

  "mensajes",                         /* articles */
  "Ver.   ",                             /* Version - STDOUT */
  "Parche No.",                       /* Patchlevel - STDOUT  */
  "Docs",                             /* Docs - STDOUT */
  "Variables de Comandos y Control",  /* Command and Control Variables - STDOUT */
  "Si",                               /* Yes - STDOUT */
  "No",                               /* No - STDOUT */
  "Acerca del Archivo:no utilizado", /* About the archive: not used - STDOUT */
  "Otros Archivos: no utilizado",     /* Other archives: not used - STDOUT */
  "Variable no utilizada",            /* address not used - STDOUT */
  "Comando interno <BODY> utilizado", /* Builtin <BODY> statement used - STDOUT */
  "No se puede leer de archivo y de stdin a la vez.", 
                         /* Cannot read from both file and stdin. - STDERR */
  "Opciones",                         /* Options - STDOUT */
  "URL de otros Archivos",            /* URL to other archives - STDOUT */
  "URL de Información de Archivos", /* URL to archive information - STDOUT */
  "Archivo de conf. para leer",  /* Configuration file to read in - STDOUT */
  "Directorio donde grabar archivos HTML", 
                           /* The directory to save HTML files in - STDOUT */
  "Leer mensajes de la entrada por defecto",   
                           /* Read messages from standard input - STDOUT */
  "Nombre del archivo de salida",   /* What to name the output archive - STDOUT */
  "Archivo de correo para leer",    /* Mail archive to read in - STDOUT */
  "Mostrar avance",                 /* Show progress - STDOUT */
  "Mostrar solamente variables de conf.",   
                           /* Show configuration variables only - STDOUT*/
  "Mostrar info. de version y salir",   
                           /* Show version information and exit - STDOUT*/
  "Actualizar el archivo de a un mensaje",  
                           /* Update archive by one article - STDOUT*/
  "Sobreescribir mensajes anteriores",  /* Overwrite previous messages - STDOUT*/
  "Especifique un idioma a utilizar", /* Specify language to use - STDOUT*/
  "Modo de uso",                      /* Usage                    -STDOUT*/
  "Lenguaje no incorporado",          /* Language not supported   -STDERR*/
  "No establecido",                   /* Not set                  -STDOUT*/
  "No utilizado",                     /* Not used                 -STDOUT*/
  "Archivo Añadido creado",         /* Created attachment file  -STDOUT*/
  "A&ntilde;adido",                   /* attachment                 -HTML*/
  "modo",                             /* mode                     -STDOUT*/
  "Leyendo encabezados antiguos",     /* Reading old headers      -STDOUT*/
  "", /* (for alignment only) -STDOUT*/
  "ERROR",                            /* ERROR                    -STDERR*/
  "The submission address of the list", 
                           /* The submission address of the list-STDERR*/
  "Read only one mail from input",
  "Autor",                            /* author - HTML*/
  "Fecha",                            /* date - HTML*/
  "Tema",                             /* subject - HTML*/
  "Mail actions",                     /* Mail actions (MA) header -HTML*/
  "mail a new topic",                 /* MA New Message      -HTML*/
  "respond to this message",          /* MA Reply            -HTML*/
  "Summary of Monthly Index Files",   /* monthly             -HTML*/
  "Summary of Yearly Index Files",    /* yearly              -HTML*/
  "Build a GDBM header cache",   /* Build a GDBM header cache   -STDOUT*/
  "Creating gdbm index... ",     /* Creating gdbm index         -STDOUT*/
  "Can't create gdbm file... ",  /* Can't create gdbm index     -STDOUT*/
  "Maintain an mbox archive",    /* Maintain an mbox archive    -STDOUT*/
  "Can't both read from and write to an mbox.", /* "Can't both read from and write to an mbox." -STDOUT*/
  "Por Añadido",                      /* Attachment view  -HTML*/
  "Por mensajes con añadidos",	  /* By messages with attachments -HTML*/
  "Escribiendo Indice de Añadidos en",	  /* Writing attachment index to -STDOUT*/
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
  NULL,                               /* End Of Message Table */
};

/*
** Finnish version of the language table
*/

char *fi[] = {       /* Finnish */
  "Lähetä uusi viesti",              /* New Message        -HTML*/
  "Kommentoi",                             /* Reply              -HTML*/
  "Tietoa listasta",                       /* About this list    -HTML*/
  "Viestien loppuun",                      /* End of Messages    -HTML*/
  "Viestien alkuun",                       /* Start of Messages  -HTML*/
  "Päiväysnäkymä",             /* Date view          -HTML*/
  "Ketjunäkymä",                     /* Thread view        -HTML*/
  "Aihenäkymä",                      /* Subject view       -HTML*/
  "Kirjoittajanäkymä",               /* Author view        -HTML*/
  "Muut arkistot",                         /* Other groups       -HTML*/
  "Viestit",                               /* Messages           -HTML*/
  "alkaa",                                 /* Starting           -HTML*/
  "loppuu",                                /* Ending             -HTML*/
  "Tietoa arkistosta",                     /* About this archive -HTML*/
  "lajiteltuna",                           /* Messages sorted by -HTML*/
  "Muita arkistoja",                       /* Other mail archives -HTML*/
  "Päiväyksittäin",               /* By Date             -HTML*/
  "Tuoreimmat viestit",                    /* Most recent messages-HTML*/
  "kirjoittajittain",                      /* author              -HTML*/
  "päiväyksittäin",               /* date                -HTML*/
  "ketjuittain",                           /* thread              -HTML*/
  "aiheittain",                            /* subject             -HTML*/
  "asetuksiin",                            /* for options       -STDOUT*/
  "Kirjoitan viestejä",                 /* Writing messages to-STDOUT*/
  "Kirjoitan hakemistoa päiväyksittäin", /* Writing date index to-STDOUT*/
  "Kirjoitan hakemistoa ketjuittain",      /* Writing thread index to -STDOUT*/
  "Kirjoitan hakemistoa aiheittain",       /* Writing subject index to-STDOUT*/
  "Kirjoitan hakemistoa kirjoittajittain", /* Writing author index to-STDOUT*/
  "Viimeinen viesti päivätty",       /* Last message date   -HTML*/
  "Arkistoitu",                            /* Archived on         -HTML*/
  "Ei voi muuttaa oikeuksia ",             /* Can not chmod     -STDERR*/
  "Kirjoitus epäonnistui",              /* Could not write   -STDERR*/
  "Seuraava viesti",                       /* Next message        -HTML*/
  "Edellinen viesti",                      /* Previous message    -HTML*/
  "Ehkä kommentti viestiin",            /* Maybe in reply to   -HTML*/
  "Kommentti viestiin",                    /* In reply to         -HTML*/
  "Seuraava ketjussa",                     /* Next in thread      -HTML*/
  "Ehkä kommentti",                     /* Maybe reply         -HTML*/
  "Ketjuittain",                           /* By Thread           -HTML*/
  "Aiheittain",                            /* By Subject          -HTML*/
  "Kirjoittajittain",                      /* By Author           -HTML*/
  "Ei voi luoda hakemistoa",               /* Can not create directory -STDERR*/
  "Luon hakemistoa",                       /* Creating directory -STDOUT*/
  "Asetukset",                             /* Configuration Values -STDOUT*/
  "polku",                                 /* path              -STDOUT*/
  "Muisti loppui!",                        /* Ran out of memory!-STDERR*/
  "Tämän arkiston loi",            /* This archive was generated by-HTML*/
  "Osaset",                                /* Elements          -STDOUT*/
  "Ei osasia",                             /* No Elements       -STDOUT*/
  "Ei voi avata postiarkistoa",            /* Cannot open mail archive */
  "Luen uusia otsikkorivejä...",        /* Reading new header...-STDOUT   */
  "Lataan postilaatikkoa",                 /* Loading mailbox      -STDOUT   */
  "koodaus ei tuettu, talletettu sellaisenaan", /* encoding is not supported, stored as-is -HTML*/
  "viestiä",                            /* messages             -HTML*/
  "Versio",                                /* Version            -STDOUT*/
  "Patchlevel",                            /* Patchlevel         -STDOUT*/
  "Ohjeet",                                /* Docs               -STDOUT*/
  "Komento- ja ohjausmuuttujat",       /* Command and Control Variables-STDOUT*/
  "Kyllä",                              /* Yes                -STDOUT*/
  "Ei",                                    /* No                 -STDOUT*/
  "Tietoa arkistosta: ei käytössä", /* About the archive: not used-STDOUT */
  "Muut arkistot: ei käytössä",   /* Other archives: not used-STDOUT*/
  "osoite ei käytössä",           /* address not used        -STDOUT*/
  "Käytetään oletus-<BODY>-rakennetta", /* Builtin <BODY> statement used-STDOUT*/
  "Ei voi lukea sekä tiedostosta että syöttövirrasta (stdin).", /* Cannot read from both file and stdin. -STDERR*/
  "Asetukset",                             /* Options                -STDOUT*/
  "URL muihin arkistoihin",                /* URL to other archives  -STDOUT*/
  "URL arkiston tietoihin",              /* URL to archive information -STDOUT*/
  "Asetustiedosto",                   /* Configuration file to read in -STDOUT*/
  "HTML-tiedostojen talletushakemisto", /* The directory to save HTML files in -STDOUT*/
  "Lue viestit syöttövirrasta (stdin)",/* Read messages from standard input -STDOUT*/
  "Talletettavan arkiston nimi",   /* What to name the output archive -STDOUT*/
  "Luettava postiarkisto",                /* Mail archive to read in -STDOUT*/
  "Näytä eteneminen",               /* Show progress           -STDOUT*/
  "Näytä vain asetusmuuttujat", /* Show configuration variables only -STDOUT*/
  "Näytä versiotieto ja lopeta ohjelma", /* Show version information and exit -STDOUT*/
  "Päivitä arkistoa yhdellä viestillä", /* Update archive by one article -STDOUT*/
  "Korvaa aiemmat viestit", /* Overwrite previous messages -STDOUT*/
  "Määritä käytettävä kieli",     /* Specify language to use     -STDOUT*/
  "Käyttöohje",                    /* Usage                   -STDOUT*/
  "Kieli ei tuettu",                     /* Language not supported  -STDERR*/
  "Ei asetettu",                         /* Not set                 -STDOUT*/
  "Ei käytössä",                /* Not used                -STDOUT*/
  "Luotu liitetiedosto",                 /* Created attachment file -STDOUT*/
  "liite",                               /* attachment              -HTML*/
  "suojaus",                             /* mode                    -STDOUT*/
  "Luen vanhoja otsikkorivejä",       /* Reading old headers     -STDOUT*/
  "",                                    /* for alignment only      -STDOUT*/
  "VIRHE",                               /* ERROR                   -STDERR*/
  "Listan lähetysosoite",   /* The submission address of the list-STDERR*/
  "Lue vain yksi viesti",
  "Kirjoittajan mukaan",                 /* author                    -HTML*/
  "Päiväyksen mukaan",             /* date                      -HTML*/
  "Aiheen mukaan",                       /* subject                   -HTML*/
  "Mail actions",                    /* Mail actions (MA) header -HTML*/
  "mail a new topic",                /* MA New Message      -HTML*/
  "respond to this message",         /* MA Reply            -HTML*/
  "Summary of Monthly Index Files",      /* monthly             -HTML*/
  "Summary of Yearly Index Files",       /* yearly              -HTML*/
  "Build a GDBM header cache",   /* Build a GDBM header cache   -STDOUT*/
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
  NULL,                              /* End Of Message Table      - NOWHERE*/
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
  "Autres groupes",                        /* Other groups       -HTML*/
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
  "Ecriture de l'index 'thème' dans",      /* Writing thread index to -STDOUT*/
  "Ecriture de l'index 'sujet' dans",      /* Writing subject index to-STDOUT*/
  "Ecriture de l'index 'auteur' dans",     /* Writing author index to-STDOUT*/
  "Date du dernier message",               /* Last message date   -HTML*/
  "Archiv&eacute; le",                     /* Archived on         -HTML*/
  "Ne peut effectuer de chmod",            /* Can not chmod     -STDERR*/
  "Ne peut écrire",                        /* Could not write   -STDERR*/
  "Message suivant",                       /* Next message        -HTML*/
  "Message pr&eacute;c&eacute;dent",       /* Previous message    -HTML*/
  "Sans doute en r&eacute;ponse &agrave;", /* Maybe in reply to   -HTML*/
  "En r&eacute;ponse &agrave;",             /* In reply to         -HTML*/
  "Suivant dans le th&egrave;me",          /* Next in thread      -HTML*/
  "Sans doute en r&eacute;ponse",          /* Maybe reply         -HTML*/
  "par Thème",                      /* By Thread           -HTML*/
  "par Sujet",                             /* By Subject          -HTML*/
  "par Auteur",                            /* By Author           -HTML*/
  "Ne peut créer le répertoire",           /* Can not create directory -STDERR*/
  "Création du répertoire",                /* Creating directory -STDOUT*/
  "Valeurs configurées",                   /* Configuration Values -STDOUT*/
  "chemin",                                /* path              -STDOUT*/
  "Plus assez de mémoire!",                 /* Ran out of memory!-STDERR*/
  "Cette archive a &eacute;t&eacute; cr&eacute;&eacute;e par",  /* This archive was generated by-HTML*/
  "Eléments",                              /* Elements          -STDOUT*/
  "Aucun Elément",                         /* No Elements       -STDOUT*/
  "Ne peut ouvrir l'archive de mail",      /* Cannot open mail archive */
  "Lecture du nouvel en-tête...",          /* Reading new header...-STDOUT   */
  "Chargement de la boite aux lettres",    /* Loading mailbox      -STDOUT   */
  "encodage non supporté, stocké tel quel",/* encoding is not supported, stored as-is -HTML*/
  "messages",                              /* messages             -HTML*/
  "Version",                               /* Version            -STDOUT*/
  "Revision",                              /* Patchlevel         -STDOUT*/
  "Documents",                             /* Docs               -STDOUT*/
  "Variables de Commande et de Contrôle",  /* Command and Control Variables-STDOUT*/
  "Oui",                                   /* Yes                -STDOUT*/
  "Non",                                   /* No                 -STDOUT*/
  "A propos de l'archive: non utilisé",    /* About the archive: not used-STDOUT */
  "Autres archives: non utilisé",          /* Other archives: not used-STDOUT*/
  "adresse non utilisée",                  /* address not used        -STDOUT*/
  "Utilisation de la déclaration <BODY>",  /* Builtin <BODY> statement used-STDOUT*/
  "Impossible de lire simulatanément un fichier et le flot d'entrée standard (stdin)", /* Cannot read from both file and stdin. -STDERR*/
  "Options",                               /* Options                -STDOUT*/
  "URL vers les autres archives",          /* URL to other archives  -STDOUT*/
  "URL vers les informations sur l'archive",  /* URL to archive information -STDOUT*/
  "Fichier de configuration à lire",       /* Configuration file to read in -STDOUT*/
  "Répertoire où sauver les fichiers HTML",/* The directory to save HTML files in -STDOUT*/
  "Lecture des messages depuis le flot d'entrée standard (stdin)", /* Read messages from standard input -STDOUT*/
  "Nom de l'archive de sortie",            /* What to name the output archive -STDOUT*/
  "Archive d'email à lire",                   /* Mail archive to read in -STDOUT*/
  "Voir la progression",                   /* Show progress           -STDOUT*/
  "Voir les variables de configuration seulement", /* Show configuration variables only -STDOUT*/
  "Afficher la version et quitter",        /* Show version information and exit -STDOUT*/
  "Mettre à jour l'archive d'un article",  /* Update archive by one article -STDOUT*/
  "Ecraser les messages précédents",       /* Overwrite previous messages -STDOUT*/
  "Spécifier la langue à utiliser",        /* Specify language to use     -STDOUT*/
  "Utilisation",                           /* Usage           -STDOUT*/
  "Langue non supportée",                  /* Language not supported -STDERR*/
  "Non défini",                            /* Not set        -STDOUT*/
  "Non utilisé",                           /* Not used       -STDOUT*/
  "Fichier attaché créé",                  /* Created attachment file -STDOUT*/
  "fichier attach&eacute;",                /* attachment     -HTML*/
  "mode",                                  /* mode           -STDOUT*/
  "Lecture des anciens en-têtes",          /* Reading old headers -STDOUT*/
  "",                                      /* for alignment only       -STDOUT*/
  "ERREUR",                                /* ERROR                    -STDERR*/
  "Adresse d'envoi à la liste", 
                                           /* The submission address of the list-STDERR*/
  "Lire seulement un mail depuis l'entrée",
  "Auteur",                                /* author              -HTML*/
  "Date",                                  /* date                -HTML*/
  "Sujet",                                 /* subject             -HTML*/
  "Actions sur les mails",                 /* Mail actions (MA) header -HTML*/
  "cr&eacute;er un nouveau th&egrave;me",  /* MA New Message      -HTML*/
  "r&eacute;pondre &agrave; ce message",   /* MA Reply            -HTML*/
  "Summary of Monthly Index Files",        /* monthly             -HTML*/
  "Summary of Yearly Index Files",         /* yearly              -HTML*/
  "Creation d'un cache GDBM pour les en-têtes",  /* Build a GDBM header cache   -STDOUT*/
  "Generation de l'index GDBM... ",               /* Creating gdbm index         -STDOUT*/
  "Impossible de créer un fichier GDBM... ",    /* Can't create gdbm index     -STDOUT*/
  "Maintenance d'une archive email (mbox)",     /* Maintain an mbox archive    -STDOUT*/
  "Impossible de lire et d'écrire simultanément dans une archive email (mbox)", /* "Can't both read from and write to an mbox." -STDOUT*/
  "Liste par fichier attachés",            /* Attachment view  -HTML*/
  "par messages avec fichiers attachés",    /* By messages with attachments -HTML*/
  "Ecriture de l'index 'fichiers attachés' dans",/* Writing attachment index to -STDOUT*/
  "octets",		                   /* file size, so far only for attachments  -HTML */
  "Impossible de créer un lien symbolique",/* MSG_CANNOT_CREATE_SYMLINK     -STDOUT */
  "Impossible de supprimer le fichier",    /* MSG_CANNOT_UNLINK                       -STDOUT */
  "Dossier précédent",                     /* MSG_PREV_DIRECTORY                     -HTML */
  "Dossier suivant",                       /* MSG_NEXT_DIRECTORY                     -HTML */
  "Liste des dossiers",                    /* MSG_FOLDERS_INDEX                      -HTML */
  "Ce message a &eacute;t&eacute; supprim&eacute; de l'archive", /* MSG_DELETED -HTML */
  "Ce message est trop vieux",             /* MSG_EXPIRED -HTML */
  "(message supprim&eacute)",              /* MSG_DEL_SHORT                            -HTML */
  "Texte original de ce message",          /* MSG_TXT_VERSION                          -HTML */
  "Ce message a &eacute;t&eacute; supprimé par filtrage",    /* MSG_FILTERED_OUT -HTML */
  NULL,                                    /* End Of Message Table  - NOWHERE*/
};

/*
** Icelandic version of the language table
** Translation by Bjarni R. Einarsson <bre@netverjar.is>
*/

char *is[] = {       /* Icelandic */
  "Nýtt bréf",                   /* New Message        -HTML*/
  "Svar",                              /* Reply              -HTML*/
  "Um þennan lista",                /* About this list    -HTML*/
  "Endir safns",                       /* End of Messages    -HTML*/
  "Upphaf safns",                      /* Start of Messages  -HTML*/
  "Raðað e. dagssetningum",      /* Date view          -HTML*/
  "Raðað e. umræðum",      /* Thread view        -HTML*/
  "Raðað e. viðfangsefnum",   /* Subject view       -HTML*/
  "Raðað e. höfundum",        /* Author view        -HTML*/
  "Aðrir flokkar",                  /* Other groups       -HTML*/
  "Bréfum",                         /* Messages           -HTML*/
  "Frá",                            /* Starting           -HTML*/
  "Til",                               /* Ending             -HTML*/
  "Um þetta safn",                  /* About this archive -HTML*/
  "Raða eftir",                     /* Messages sorted by -HTML*/
  "Önnur söfn",                  /* Other mail archives -HTML*/
  "eftir dagssetningum",               /* By Date             -HTML*/
  "Nýjustu bréfin",              /* Most recent messages-HTML*/
  "höfundum",                       /* author              -HTML*/
  "dags.",                             /* date                -HTML*/
  "umræðum",                     /* thread              -HTML*/
  "viðfangsefnum",                  /* subject             -HTML*/
  "for options",                       /* for options       -STDOUT*/
  "Writing messages to",               /* Writing messages to-STDOUT*/
  "Writing date index to",             /* Writing date index to-STDOUT*/
  "Writing thread index to",           /* Writing thread index to -STDOUT*/
  "Writing subject index to",          /* Writing subject index to-STDOUT*/
  "Writing author index to",           /* Writing author index to-STDOUT*/
  "Dags. síðasta bréfs",      /* Last message date   -HTML*/
  "Síðasta bréf móttekið", /* Archived on         -HTML*/
  "Can not chmod",                     /* Can not chmod     -STDERR*/
  "Could not write",                   /* Could not write   -STDERR*/
  "Næsta bréf",                  /* Next message        -HTML*/
  "Fyrra bréf",                     /* Previous message    -HTML*/
  "Mögulega í frh. af",          /* Maybe in reply to   -HTML*/
  "Í framhaldi af",                 /* In reply to         -HTML*/
  "Næst í umræðu",         /* Next in thread      -HTML*/
  "Mögulegt svar",                  /* Maybe reply         -HTML*/
  "eftir umræðum",               /* By Thread           -HTML*/
  "eftir viðfangsefnum",            /* By Subject          -HTML*/
  "eftir höfundum",                 /* By Author           -HTML*/
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
  "framandi kóðun, geymt óbreytt", /* encoding is not supported, stored as-is -HTML*/
  "bréf.",                          /* messages             -HTML*/
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
  "viðhengi",                      /* attachment                    -HTML*/
  "mode",                             /* mode                        -STDOUT*/
  "Reading old headers",              /* Reading old headers         -STDOUT*/
  "",                                 /* for alignment only       -STDOUT*/
  "ERROR",                            /* ERROR                       -STDERR*/
  "The submission address of the list", 
                           /* The submission address of the list-STDERR*/
  "Read only one mail from input",
  "Höfundur",                      /* author              -HTML*/
  "Dagssetning",                      /* date                -HTML*/
  "Viðfangsefni",                  /* subject             -HTML*/
  "Mail actions",                     /* Mail actions (MA) header -HTML*/
  "mail a new topic",                 /* MA New Message      -HTML*/
  "respond to this message",          /* MA Reply            -HTML*/
  "Summary of Monthly Index Files",   /* monthly             -HTML*/
  "Summary of Yearly Index Files",    /* yearly              -HTML*/
  "Build a GDBM header cache",   /* Build a GDBM header cache   -STDOUT*/
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
  NULL,                               /* End Of Message Table      - NOWHERE*/
};

/*
** Swedish version of the language table. Daniel Stenberg translation.
** (a little secret for free: we don't say bork ;-)
*/

char *sv[] = {
  "Nytt brev",                      /* New Message        -HTML*/
  "Svar" ,                          /* Reply              -HTML*/
  "Om den här listan",      /* About this list    -HTML*/
  "Slut på brevlistan",          /* End of Messages    -HTML*/
  "Start på brevlistan",         /* Start of Messages  -HTML*/
  "Datumvy",                        /* Date view          -HTML*/
  "Trådvy",                      /* Thread view        -HTML*/
  "Ämnesvy",                     /* Subject view       -HTML*/
  "Författarvy",                 /* Author view        -HTML*/
  "Andra grupper",                  /* Other groups       -HTML*/
  "Brev",                           /* Messages           -HTML*/
  "Startar",                        /* Starting           -HTML*/
  "Slutar",                         /* Ending             -HTML*/
  "Om det här arkivet", /* About this archive -HTML*/
  "sortede efter",                 /* Messages sorted by -HTML*/
  "Andra brevarkiv",                /* Other mail archives-HTML*/
  "Datumsorterat",                    /* By Date            -HTML*/
  "Nyast brev",                     /* Most recent messages-HTML*/
  "författare",                  /* author             -HTML*/
  "datum",                          /* date               -HTML*/
  "tråd",                        /* thread             -HTML*/
  "ämne",                        /* subject            -HTML*/
  "för inställningar",        /* for options        -STDOUT*/
  "Skriver brev till",              /* Writing messages to-STDOUT*/
  "Skriver datumindex till",        /* Writing date index to-STDOUT*/
  "Skriver trådindex till",      /* Writing thread index to -STDOUT*/
  "Skriver ämnesindex till",     /* Writing subject index to-STDOUT*/
  "Skriver författarindex till", /* Writing author index to-STDOUT*/
  "Senaste brevdatum",             /* Last message date  -HTML*/
  "Arkiverat",                      /* Archived on        -HTML*/
  "Kan inte chmod",                 /* Can not chmod      -STDERR*/
  "Kunde inte skriva",              /* Could not write    -STDERR*/
  "Nästa brev",                  /* Next message       -HTML*/
  "Tidigare brev",                  /* Previous message   -HTML*/
  "Kanske ett svar till",           /* Maybe in reply to  -HTML*/
  "Svar till",                      /* In reply to        -HTML*/
  "Nästa i tråden",               /* Next in thread     -HTML*/
  "Kanske svar",                    /* Maybe reply        -HTML*/
  "Trådsorterat",                    /* By Thread          -HTML*/
  "Ämnessorterat",                    /* By Subject         -HTML*/
  "Författarsorterat",              /* By Author          -HTML*/
  "Kan inte skapa directory",       /* Can not create directory -STDERR*/
  "Skapar katalog",               /* Creating directory -STDOUT*/
  "Konfigureringsvärden",        /* Configuration Values -STDOUT*/
  "path",                           /* path               -STDOUT*/
  "Fick slut på minne!",         /* Ran out of memory!-STDERR*/
  "Det här arkivet skapades av", /* This archive was generated by-HTML*/
  "Delar",                          /* Elements           -STDOUT*/
  "Inga delar",                     /* No Elements        -STDOUT*/
  "Kan inte öppna brevarkivet",  /* Cannot open mail archive */
  "Läser ny header...",          /* Reading new header...-STDOUT*/
  "Laddar mailbox",                 /* Loading mailbox   -STDOUT   */
  "kodformatet stöds inte, sparat som det är", 
                           /* encoding is not supported, stored as-is -HTML*/
  "brev",                           /* messages          -HTML*/
  "Version",                        /* Version           -STDOUT*/
  "Patchnivå",                   /* Patchlevel        -STDOUT*/
  "Dokumentation",                  /* Docs              -STDOUT*/
  "Kommando och kontrollvariabler", /* Command and Control Variables-STDOUT*/
  "Ja",                             /* Yes               -STDOUT*/
  "Nej",                            /* No                -STDOUT*/
  "Om arkivet: ej använd",      /* About the archive: not used-STDOUT */
  "Andra arkiv: ej använd",      /* Other archives: not used-STDOUT*/
  "adress ej använd",            /* address not used  -STDOUT*/
  "Inbyggd <BODY> använd",       /* Builtin <BODY> statement used-STDOUT*/
  "Kan inte läsa bde från fil och stdin.", 
                           /* Cannot read from both file and stdin. -STDERR*/
  "Alternativ",                     /* Options           -STDOUT*/
  "URL till andra arkiv",           /* URL to other archives  -STDOUT*/
  "URL till arkivinformation",      /* URL to archive information -STDOUT*/
  "Konfigurationsfil att läsa in",   /* Configuration file to read in -STDOUT*/
  "Directory att spara HTML-filerna i", 
                            /* The directory to save HTML files in -STDOUT*/
  "Läs breven från standard in", /* Read messages from standard input-STDOUT*/
  "Namnet på utarkivet",         /* What to name the output archive-STDOUT*/
  "Brevarkiv att läsa in",       /* Mail archive to read in -STDOUT*/
  "Visa progress",                  /* Show progress      -STDOUT*/
  "Visa konfigurationsvariabler enbart",
                                /* Show configuration variables only-STDOUT*/
  "Visa versionsinformation och sluta", 
                                /* Show version information and exit -STDOUT*/
  "Updatera arkivet med ett brev", /*Update archive by one article -STDOUT*/
  "Skriv över tidigare brev",    /* Overwrite previous messages -STDOUT*/
  "Ange språk att använda",   /* Specify language to use -STDOUT*/
  "Användning",                  /* Usage                   -STDOUT*/
  "Språk ej supportat",          /* Language not supported  -STDERR*/
  "Ej angivet",                     /* Not set                 -STDOUT*/
  "Ej använt",                   /* Not used                -STDOUT*/
  "Skapade bilagefil",          /* Created attachment file -STDOUT*/
  "bilaga",                      /* attachment              -HTML*/
  "mode",                           /* mode                    -STDOUT*/
  "Läser gamla headers",         /* Reading old headers     -STDOUT*/
  "",                               /* for alignment only   -STDOUT*/
  "FEL",                            /* ERROR                   -STDERR*/
  "Adressen för att posta till listan", 
                           /* The submission address of the list-STDERR*/
  "Läs bara ett brev från in",
  "Författare",                  /* author             -HTML*/
  "Datum",                          /* date               -HTML*/
  "Ämne",                        /* subject            -HTML*/
  "E-postfunktioner",                   /* Mail actions (MA) header -HTML*/
  "sänd ett nytt ämne",               /* MA New Message      -HTML*/
  "svara på brevet",        /* MA Reply            -HTML*/
  "Sammanfattning över månatliga indexfiler", /* monthly             -HTML*/
  "Sammanfattning över årliga indexfiler",  /* yearly              -HTML*/
  "Bygger en GDBM-headercache",   /* Build a GDBM header cache   -STDOUT*/
  "Skapar gdbm-index... ",     /* Creating gdbm index         -STDOUT*/
  "Kan inte skapa gdbm-fil... ",  /* Can't create gdbm index     -STDOUT*/
  "Underhåll ett mbox-arkiv",    /* Maintain an mbox archive    -STDOUT*/
  "Kan inte både läsa från och skriva till en mbox.", /* "Can't both read from and write to an mbox." -STDOUT*/
  "Bilagevy",                      /* Attachment view  -HTML*/
  "Efter brev med bilaga",	  /* By messages with attachments -HTML*/
  "Skriver bilageindex till",	  /* Writing attachment index to -STDOUT*/
  "byte",		    /* file size, so far only for attachments  -HTML */
  "Kan inte skapa symbolisk länk", /* MSG_CANNOT_CREATE_SYMLINK     -STDOUT */
  "Kan inte ta bort fil", /* MSG_CANNOT_UNLINK                       -STDOUT */
  "Föregående mapp",  /* MSG_PREV_DIRECTORY                     -HTML */
  "Nästa mapp",      /* MSG_NEXT_DIRECTORY                     -HTML */
  "Mapplista",  /* MSG_FOLDERS_INDEX                      -HTML */
  "Detta brev har tagits bort från arkivet", /* MSG_DELETED -HTML */
  "Detta brev har utgått",                       /* MSG_EXPIRED -HTML */
  "(borttaget brev)", /* MSG_DEL_SHORT                            -HTML */
  "Ursprunglig brevtext", /* MSG_TXT_VERSION                          -HTML */
  "This message has been filtered out",        /* MSG_FILTERED_OUT -HTML */
  NULL,                             /* End Of Message Table    - NOWHERE*/
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
{    "sv",      sv,    "sv_SE"   },     /* Swedish */
{     NULL,     NULL },     /* EOL     */
};

/*
** Default language table
*/
char **lang = en;

#else

extern char **lang;
extern struct language_entry ltable[];

#endif
