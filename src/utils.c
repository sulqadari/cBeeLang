#include "../includes/common.h"

void idle_exec(const char *cmd, size_t len)
{
    if (0 == len)
        return;
    
    if (memcmp(cmd, "pause", len) == 0)
    {
        printf("press any key to continue...");
        getchar();
    }
}