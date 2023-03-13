#include <assert.h>

#include "struct.h"

static struct sh *sh;

void sh_instance_init(struct sh *instance)
{
    sh = instance;
}

struct sh *sh_instance_get(void)
{
    assert(sh);
    return sh;
}
