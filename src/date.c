/*
** Copyright (C) 1994, 1995 Enterprise Integration Technologies Corp.
**         VeriFone Inc./Hewlett-Packard. All Rights Reserved.
** Kevin Hughes, kev@kevcom.com 3/11/94
** Kent Landfield, kent@landfield.com 4/6/97
**
** This program and library is free software; you can redistribute it and/or
** modify it under the terms of the GNU (Library) General Public License
** as published by the Free Software Foundation; either version 2
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

/*
** Move to wrappers calling standard functions - perhaps slower
** but easier to maintain
*/

#include "hypermail.h"
#include "setup.h"

/* 
** Given a long date string, it returns the number of seconds
** since BASEYEAR. (Y2K ok)
*/

extern time_t get_date(const char *, const time_t *);

static time_t ydhms_tm_diff(int, int, int, int, int, const struct tm *);
static time_t my_mktime(struct tm *);

time_t convtoyearsecs(char *date)
{
    time_t yearsecs;
    char *p, *s = date;

    /* the (non-standard) timezone specs GMT0 and BST-1
     * confuse the get_date routines (GMT0 sets the year to 0).
     * Rather than altering the standard routine, we try to
     * put them into a standard format here.
     */
    if ((p = strstr(date, "GMT0")) != NULL) {
	s = malloc(strlen(date) + 1);
	strcpy(s, date);
	*(s + (p - date) + 3) = '\0';
    }
    else if ((p = strstr(date, "BST-1")) != NULL) {
	s = malloc(strlen(date) + 1);
	strcpy(s, date);
	strcpy(s + (p - date), "-1");
    }

    yearsecs = get_date(s, (time_t *)NULL);
    if (s != date) {
	free(s);
    }

    /* if we can't parse the date string, the calling
     * routine needs to know as it may have another
     * way of getting the date.
     */
    return yearsecs;
}

/* 
** Gets the local time and returns it formatted.
*/

char *getlocaltime(void)
{
    static char s[DATESTRLEN + 5];
    time_t tp;
    struct tm *tmptr;

    time(&tp);
    tmptr = (set_gmtime ? gmtime(&tp) : localtime(&tp));

    s[0] = '\0';

    if (set_dateformat != NULL) {
	strftime(s, DATESTRLEN, set_dateformat, tmptr);
        /* 
	 * Need to check if the timezone %Z was specified. 
	 * If so do not print it out automatically.
	 */
       if (strstr(set_dateformat, "%Z") == NULL)
           sprintf(s, "%s %s", s, timezonestr);
    }
    else {
	if (set_eurodate)
	    strftime(s, DATESTRLEN, "%a %d %b %Y - %H:%M:%S", tmptr);
	else if (set_isodate) {
	    if (set_gmtime)
	        strftime(s, DATESTRLEN, "%Y-%m-%dZ%H:%M:%S", tmptr);
	    else
	        strftime(s, DATESTRLEN, "%Y-%m-%d %H:%M:%S", tmptr);
	}
	else
	    strftime(s, DATESTRLEN, "%a %b %d %Y - %H:%M:%S", tmptr);

        sprintf(s, "%s %s", s, timezonestr);
    }
    return s;
}

/* 
** Gets the local time zone.
*/

void gettimezone(void)
{
    time_t tp;

    time(&tp);
    strftime(timezonestr, TIMEZONELEN, "%Z",
	     set_gmtime ? gmtime(&tp) : localtime(&tp));
}

/*
** Gets the current year.
*/

void getthisyear(void)
{
    time_t tp;

    time(&tp);
    strftime(thisyear, YEARLEN, "%Y", set_gmtime ? gmtime(&tp) : localtime(&tp));
}

int year_of_datenum(time_t t)
{
  struct tm *tptr = (set_gmtime ? gmtime(&t) : localtime(&t));
  return tptr->tm_year + 1900;
}

int month_of_datenum(time_t t)
{
  struct tm *tptr = (set_gmtime ? gmtime(&t) : localtime(&t));
  return tptr->tm_mon;
}

/* 
** From the number of seconds since BASEYEAR, this pretty-prints
** a date for you. 
*/

char *getdatestr(time_t yearsecs)
{
    static char date[DATESTRLEN];
    struct tm *tmptr = (set_gmtime ? gmtime(&yearsecs) : localtime(&yearsecs));

    if (set_dateformat != NULL) {
	strftime(date, DATESTRLEN, set_dateformat, tmptr);
    }
    else {
	if (set_eurodate)
	    strftime(date, DATESTRLEN, "%a %d %b %Y - %H:%M:%S %Z", tmptr);
	else if (set_isodate) {
	    if (set_gmtime)
	        strftime(date, DATESTRLEN, "%Y-%m-%dZ%H:%M:%S", tmptr);
	    else
	        strftime(date, DATESTRLEN, "%Y-%m-%d %H:%M:%S", tmptr);
	}
	else
	    strftime(date, DATESTRLEN, "%a %b %d %Y - %H:%M:%S %Z", tmptr);
    }
    return date;
}

/* 
** This function calls getdatestr, but with an alternate date format
** that is used for showing dates in the indexs
*/

char *getindexdatestr(time_t yearsecs)
{
  char *previous_dateformat;
  char *date;

  /* store the previous dateformat */
  previous_dateformat = set_dateformat;
  /* if there's an index date format, we use it, otherwise use the
   standard  dateformat */
  if (set_indexdateformat)
    set_dateformat = set_indexdateformat;
  date = getdatestr (yearsecs);
  /* restore the previous dateformat */
  set_dateformat = previous_dateformat;
  return date;
}

/* 
** This function calls getdatestr, but with a fixed  alternate
**  date format that we use for comparing two dates minus the hour.
*/

char *getdateindexdatestr(time_t yearsecs)
{
  char *previous_dateformat;
  char *date;

  /* store the previous dateformat */
  previous_dateformat = set_dateformat;
  set_dateformat = "%A, %e %B";
  date = getdatestr (yearsecs);
  /* restore the previous dateformat */
  set_dateformat = previous_dateformat;
  return date;
}

char *secs_to_iso(time_t t)
{
    /* is passed time_t variable
     * holding number of seconds since EPOCH
     * returns pointer to string holding date in format
     * YYYYMMDDHHMMSS
     * This buffer will be overwritten by next call to secs_to_iso.
     */
    static char s[15];
    struct tm *tm;

    tm = gmtime(&t);
    sprintf(s, "%4.4d%02.2d%02.2d%02.2d%02.2d%02.2d",
	    tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
	    tm->tm_hour, tm->tm_min, tm->tm_sec);
    return s;
}

char *secs_to_iso_meta(time_t t)
{
    /* is passed time_t variable
     * holding number of seconds since EPOCH
     * returns pointer to string holding date in format
     * YYYY-MM-DD
     * This buffer will be overwritten by next call to secs_to_iso_meta.
     */
    static char s[11];
    struct tm *tm;

    tm = localtime(&t);
    sprintf(s, "%4.4d-%02.2d-%02.2d",
	    tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
    return s;
}

time_t iso_to_secs(char *isodate)
{
    /* is passed string holding date in format
     * YYYYMMDDHHMMSS
     * returns number of seconds since EPOCH
     */
    struct tm t;
    char s[15];

    strncpy(s, isodate, 14);
    s[14] = '\0';
    t.tm_isdst = 0;
    t.tm_yday = 0;
    t.tm_wday = 0;
    t.tm_sec = atoi(s + 12);
    *(s + 12) = '\0';
    t.tm_min = atoi(s + 10);
    *(s + 10) = '\0';
    t.tm_hour = atoi(s + 8);
    *(s + 8) = '\0';
    t.tm_mday = atoi(s + 6);
    *(s + 6) = '\0';
    t.tm_mon = atoi(s + 4) - 1;
    *(s + 4) = '\0';
    t.tm_year = atoi(s) - 1900;

    return my_mktime(&t);
}


/* Based on mktime.c from the GNU C library (glibc-2.0.6)
 * simplified to ignore timezones completely for use by hypermail when 
 * converting from iso string representation of time to seconds since
 * epoch (interpreting given time as UTC).
 *
 * Paul Haldane
 */

#define TM_YEAR_BASE 1900
#define EPOCH_YEAR 1970

#ifndef __isleap
/* Nonzero if YEAR is a leap year (every 4 years,
   except every 100th isn't, and every 400th is).  */
# define __isleap(year)	\
  ((year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0))
#endif

/* How many days come before each month (0-12).  */
const unsigned short int __mon_yday[2][13] = {
    /* Normal years.  */
    {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},
    /* Leap years.  */
    {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366}
};

/* Yield the difference between (YEAR-YDAY HOUR:MIN:SEC) and (*TP),
   measured in seconds, ignoring leap seconds.
   YEAR uses the same numbering as TM->tm_year.
   All values are in range, except possibly YEAR.
   If overflow occurs, yield the low order bits of the correct answer.  */

static time_t ydhms_tm_diff(int year, int yday, int hour,
			    int min, int sec, const struct tm *tp)
{
    /* Compute intervening leap days correctly even if year is negative.
       Take care to avoid int overflow.  time_t overflow is OK, since
       only the low order bits of the correct time_t answer are needed.
       Don't convert to time_t until after all divisions are done, since
       time_t might be unsigned.  */
    int a4 = (year >> 2) + (TM_YEAR_BASE >> 2) - !(year & 3);
    int b4 = (tp->tm_year >> 2) + (TM_YEAR_BASE >> 2) - !(tp->tm_year & 3);
    int a100 = a4 / 25 - (a4 % 25 < 0);
    int b100 = b4 / 25 - (b4 % 25 < 0);
    int a400 = a100 >> 2;
    int b400 = b100 >> 2;
    int intervening_leap_days = (a4 - b4) - (a100 - b100) + (a400 - b400);
    time_t years = year - (time_t)tp->tm_year;
    time_t ddays = (365 * years + intervening_leap_days + (yday - tp->tm_yday));
    return (60 * (60 * (24 * ddays + (hour - tp->tm_hour))
		  + (min - tp->tm_min)) + (sec - tp->tm_sec));
}

/* Convert *TP to a time_t value */

static time_t my_mktime(struct tm *tp)
{
    time_t t0;
    struct tm tm;

    /* Time requested.  Copy it in case CONVERT modifies *TP; this can
       occur if TP is localtime's returned value and CONVERT is localtime.  */
    int sec = tp->tm_sec;
    int min = tp->tm_min;
    int hour = tp->tm_hour;
    int mday = tp->tm_mday;
    int mon = tp->tm_mon;
    int year_requested = tp->tm_year;

    /* Ensure that mon is in range, and set year accordingly.  */
    int mon_remainder = mon % 12;
    int negative_mon_remainder = mon_remainder < 0;
    int mon_years = mon / 12 - negative_mon_remainder;
    int year = year_requested + mon_years;

    /* The other values need not be in range:
       the remaining code handles minor overflows correctly,
       assuming int and time_t arithmetic wraps around.
       Major overflows are caught at the end.  */

    /* Calculate day of year from year, month, and day of month.
       The result need not be in range.  */
    int yday = ((__mon_yday[__isleap(year + TM_YEAR_BASE)]
		 [mon_remainder + 12 * negative_mon_remainder])
		+ mday - 1);

    tm.tm_year = EPOCH_YEAR - TM_YEAR_BASE;
    tm.tm_yday = tm.tm_hour = tm.tm_min = tm.tm_sec = 0;
    t0 = ydhms_tm_diff(year, yday, hour, min, sec, &tm);

    return t0;
}
