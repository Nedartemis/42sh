#ifndef EXPENSION_HELPER_H
#define EXPENSION_HELPER_H

void concat_list_string_to_vector(struct vector *vector, struct list *words,
                                  char separator);
char *concat_list_string(struct list *words, char separator);

void concat_vector_to_last_element(struct list *words, struct vector *vector);
void concat_special_list(struct list *l1, struct list *l2);

#endif /* !EXPENSION_HELPER_H */
