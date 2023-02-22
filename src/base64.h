/* 
** MIME Decode - base64.c
*/
#ifndef BASE64_H
#define BASE64_H

/* Common state as you feed successive lines into base64_decode() */
struct base64_state {
    unsigned char b_ibuf[4];
    unsigned char b_obuf[3];
    char b_endtext;
    int b_lindex;
};

/* Get a new one */
extern struct base64_state *base64_alloc(void);

/* Decode some base64, return count of chars placed in "out" */
extern int base64_decode(struct base64_state *st,
    const char *intext, char *out);

/* All done */
extern void base64_free(struct base64_state *);

/* When you're bursting a single line */
extern void base64Decode(const char *intext, char *out, int *length);

#endif /* BASE64_H */
