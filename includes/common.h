#ifndef BEELANG_COMMON_H
    #define BEELANG_COMMON_H

    #include <stdbool.h>
    #include <stddef.h>
    #include <stdint.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    // if defined the VM disassembles and prints each instruction right before executing it.
    #define DEBUG_TRACE_EXECUTION

    /**
     * @brief Stops execution. The analogous of system("pause") in C++
     * 
     * @param cmd a stirng with command. Right now the only supported command is "pause"
     * @param len the length of cmd string.
     */
    void idle_exec(const char *cmd, size_t len);

#endif  // BEELANG_COMMON_H