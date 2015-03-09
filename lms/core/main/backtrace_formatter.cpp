#include "backtrace_formatter.h"
#include <iostream>

#ifdef _WIN32
    // TODO
#else
    #include <execinfo.h>
    #include <cxxabi.h>
    #include <sys/wait.h>
    #include <unistd.h>
#endif

namespace lms {

void BacktraceFormatter::print(){
#ifdef _WIN32
    // TODO
#else
    // Get the backtrace.
    std::cerr << "\n\033[31mBacktrace:\033[0m" << std::endl;
    void **array = (void**)alloca (256 * sizeof (void *));
    int size = backtrace (array, 256);
    /*
     * Now try to locate the PC from signal context in the backtrace.
     * Normally it will be found at arr[2], but it might appear later
     * if there were some signal handler wrappers.  Allow a few bytes
     * difference to cope with as many arches as possible.
     * */
    uintptr_t pc = (uintptr_t) array[3]; //GET_PC (ctx);
    int i;
    for (i = 0; i < size; ++i)
        if ((uintptr_t) array[i] >= pc - 16 && (uintptr_t) array[i] <= pc + 16)
            break;

    /* If we haven't found it, better dump full backtrace even including
    the signal handler frames instead of not dumping anything.  */
    if (i == size)
        i = 0;
    /* Now generate nicely formatted output.  */

    char** messages = backtrace_symbols (array + i, size - i);

    for (int i = 1; i < size && messages != NULL; ++i)
    {
        char *mangled_name = 0, *offset_begin = 0, *offset_end = 0;

        // find parantheses and +address offset surrounding mangled name
        for (char *p = messages[i]; *p; ++p)
        {
            if (*p == '(')
            {
                mangled_name = p;
            }
            else if (*p == '+')
            {
                offset_begin = p;
            }
            else if (*p == ')')
            {
                offset_end = p;
                break;
            }
        }

        // if the line could be processed, attempt to demangle the symbol
        if (mangled_name && offset_begin && offset_end &&
            mangled_name < offset_begin)
        {
            *mangled_name++ = '\0';
            *offset_begin++ = '\0';
            *offset_end++ = '\0';

            int status;
            char * real_name = abi::__cxa_demangle(mangled_name, 0, 0, &status);

            // if demangling is successful, output the demangled function name
            if (status == 0)
            {
                std::cerr << "[bt]: (" << i << ") " << messages[i] << " : "
                          << real_name << "+" << offset_begin << offset_end
                          << std::endl;

            }
            // otherwise, output the mangled function name
            else
            {
                std::cerr << "[bt]: (" << i << ") " << messages[i] << " : "
                          << mangled_name << "+" << offset_begin << offset_end
                          << std::endl;
            }
            free(real_name);
        }
        // otherwise, print the whole line
        else
        {
            std::cerr << "[bt]: (" << i << ") " << messages[i] << std::endl;
        }

    }
    std::cerr << std::endl;
    std::cerr << std::endl;
    std::cerr << std::endl;
    free (messages);
#endif
}

}
