#ifndef EXPANSION_QUOTE_H
#define EXPANSION_QUOTE_H

void expansion_single_quote(char *word, size_t *i, struct vector *vector);
void expension_escape_char(char *word, size_t *i, struct vector *vector);
void expension_double_quote(char *word, size_t *i, struct vector *vector);

#endif /* !EXPANSION_QUOTE_H */
