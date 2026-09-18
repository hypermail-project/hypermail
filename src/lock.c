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

#include "hypermail.h"
#include "setup.h"

#define LOCKBASE       ".hypermail.lock"

int i_locked_it = 0;

void lock_archive(char *dir)
{
    FILE *fp;
    char buffer[MAXLINE];

    int count = 0;		/* # minutes waited */
    i_locked_it = 0;		/* guilty until proven innocent */

    trio_snprintf(lockfile, sizeof(lockfile), "%s/%s", dir, LOCKBASE);

    while ((fp = fopen(lockfile, "r")) != NULL) {
	fgets(buffer, MAXLINE-1, fp);
	fclose(fp);
	/*  
         * "set_locktime" is the config file item named 'locktime', 
         * default is 3600 seconds 
         */
	if (time(NULL) > (time_t)(atol(buffer) + set_locktime))
	    break;		/* lock over hour old - break it */

	++count;
	if (set_showprogress)
	    fprintf(stderr, "Waiting for lock (file '%s')\n", lockfile);
	sleep(30);
    }
    if ((fp = fopen(lockfile, "w")) != NULL) {
	i_locked_it = 1;
	fprintf(fp, "%ld\n", (long)time(NULL));
	fclose(fp);
    }
    else if (dir[0]) {
	trio_snprintf(errmsg, sizeof(errmsg), "Couldn't create lock file \"%s\".", lockfile);
	progerr(errmsg);
    }
}

void unlock_archive(void)
{
    if (*lockfile != '\0' && i_locked_it)
	remove(lockfile);
    lockfile[0] = '\0';
}
