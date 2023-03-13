#ifndef LEXER_ALIAS_H
#define LEXER_ALIAS_H

bool alias_is(const char *word);
void alias_substitution(struct lexer *lexer, const char *key, char next);

#endif /* ! LEXER_ALIAS_H */
