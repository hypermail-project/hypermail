#ifdef LANG_PROG
#define MAIN_FILE
#endif

#include "hypermail.h"

char **valid_language(char *lg, char **locale_code)
{
    struct language_entry *lte;

    if (!strcmp(lg, "se")) {
	printf("Warning: language code 'se' not supported. Using 'sv' (Swedish) instead.\n");
	lg = "sv";
    }
    lte = &ltable[0];
    while (lte->langcode != NULL) {
	if (strcmp(lg, lte->langcode) == 0) {
	    if (locale_code != NULL && *locale_code == NULL)
		*locale_code = lte->locale_code;
	    return (lte->mtable);
	}
	lte++;
    }
    return (NULL);
}

#ifdef LANG_PROG

extern char *optarg;
extern int opterr;

int main(int argc, char **argv)
{
    char *progname;
    char *language = "en";

    int c;
    int verbose = 0;
    int print_table = 0;


    opterr = 0;
    progname = argv[0];

    if (argc > 1) {
	while ((c = getopt(argc, argv, "L:tv")) != EOF) {
	    switch (c) {
	    case 'v':
		verbose++;
		break;
	    case 'L':
		language = optarg;
		break;
	    case 't':
		print_table++;
		break;
	    default:
		fprintf(stderr,
			"usage: %s [-tv] [-L lang] [msg-num [..]]\n",
			progname);
		return (1);
	    }
	}
    }

    if ((lang = valid_language(language, NULL)) == NULL) {
	(void)fprintf(stderr, "%s: %s: invalid language specified\n",
		      progname, language);
	return (1);
    }

    if (print_table || (argc <= optind)) {
	int cnt;
	for (cnt = 0; lang[cnt] != NULL; cnt++) {
	    printf("%02d: %s\n", cnt, lang[cnt]);
	}
    }

    else {
	for (; optind < argc; optind++)	/* process files to print */
	    printf("%s: \n", lang[atoi(argv[optind])]);
    }

    return (0);			/* terminate this process */
}

#endif
