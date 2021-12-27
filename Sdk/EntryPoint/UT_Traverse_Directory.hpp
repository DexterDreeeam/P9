#pragma once

void UT_Traverse_Directory()
{
    const char* func_name = __FUNCTION__;
    print("\n>>>>>>>>> [%s] Start >>>>>>>>>\n\n", func_name);
    escape_function ef = [=]()
    {
        print("\n<<<<<<<<< [%s] End <<<<<<<<<\n\n", func_name);
    };

    traverse_folders_files(
        "./",
        [](const string& s)
        {
            print("%s\n", s.data());
        },
        [](const string& s)
        {
            print("%s\n", s.data());
        });
}
