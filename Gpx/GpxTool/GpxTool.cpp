
#include "../../Sdk/Interface.hpp"
#include "../Algebra/Interface.hpp"
#include "HandleConvert.hpp"
#include "HandleHelp.hpp"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        print("Input [help] for manual indication\n");
        return -1;
    }
    if (str_equal_case_insensitive(argv[1], "help"))
    {
        return handle_help(argc - 2, argv + 2);
    }
    else if (str_equal_case_insensitive(argv[1], "convert"))
    {
        return handle_convert(argc - 2, argv + 2);
    }
    else
    {
        print("Cannot parse indicator.\n", argv[1]);
        print("[help] for more information.\n");
        return -1;
    }
    return 0;
}
