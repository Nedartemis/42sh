#ifndef SH_INSTANCE_H
#define SH_INSTANCE_H

#include "struct.h"

void sh_instance_init(struct sh *instance);
struct sh *sh_instance_get(void);

#endif /* !SH_INSTANCE_H */
