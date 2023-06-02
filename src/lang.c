/*
** Copyright (C) 1997-2023 Hypermail Project
** 
** This program and library is free software; you can redistribute it and/or 
** modify it under the terms of the GNU (Library) General Public License 
** as published by the Free Software Foundation; either version 3
** of the License, or any later version. 
** 
** This program is distributed in the hope that it will be useful, 
** but WITHOUT ANY WARRANTY; without even the implied warranty of 
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
** GNU (Library) General Public License for more details. 
** 
** You should have received a copy of the GNU (Library) General Public License
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA 
*/

#ifdef LANG_PROG
#define MAIN_FILE
#endif

#include <getopt.h>

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
