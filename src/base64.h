#ifndef _HYPERMAIL_BASE64_H
#define _HYPERMAIL_BASE64_H
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

/* 
** MIME Decode - base64.c
*/

/* Common state as you feed successive lines into base64_decode_stream() */
struct base64_decoder_state {
    unsigned char ibuf[4]; /* input buffer */
    unsigned char obuf[3]; /* output buffer */
    int lindex;            /* index for ibuf / obuf */    
    char endtext;          /* base64 end detected */
};

/* allocate and free a base64_state structure */
struct base64_decoder_state *base64_decoder_state_new(void);
void base64_decoder_state_free(struct base64_decoder_state *);

/* decode a stream made of multiple base64 lines */
int base64_decode_stream(struct base64_decoder_state *, const char *, char *);

/* decode a single string */
int base64_decode_string(const char *, char *);

#endif /* _HYPERMAIL_BASE64_H */
