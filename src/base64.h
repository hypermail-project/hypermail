/* 
** MIME Decode - base64.c
*/

#ifndef _HYPERMAIL_BASE64_H
#define _HYPERMAIL_BASE64_H

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
