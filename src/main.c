#include "../include/common.h"
#include "../include/vm.h"

int main(int argc, const char* argv[])
{
    Bytecode bytecode;

    initVM();
    interpret(&bytecode);
    
    freeVM();

    return 0;
}