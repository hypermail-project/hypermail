
void
txt2html(FILE *fp, const struct body *bp, char *id, char *subject,
	 int msgnum, char *inreply, const char *dir, bool replace_quoted,
	 int maybe_reply);
void init_txt2html(void);
void end_txt2html(FILE *fp);
