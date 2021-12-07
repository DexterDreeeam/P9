
#include "../../Sdk/Interface.hpp"
#include "../Algebra/Interface.hpp"
#include "../ResourceMgr/Interface.hpp"

#include "HandleHelp.hpp"
#include "HandleVerticesConvert.hpp"
#include "HandleTextureConvert.hpp"

map<string, s64 (*)(s64, char* [])> convert_map;

vector<string> vertices_comvert_extension_vec = { ".vertices.json" };
vector<string> texture_comvert_extension_vec = { ".png", ".jpg", ".jpeg", ".bmp" };

int convert(int argc, char* argv[])
{
    if (argc <= 0 || argv[0] == nullptr)
    {
        return -1;
    }

    for (s64 i = 0; i < vertices_comvert_extension_vec.size(); ++i)
    {
        convert_map[vertices_comvert_extension_vec[i]] = &handle_vertices_convert;
    }
    for (s64 i = 0; i < texture_comvert_extension_vec.size(); ++i)
    {
        convert_map[texture_comvert_extension_vec[i]] = &handle_texture_convert;
    }

    string input_file = argv[0];
    for (auto itr = convert_map.begin(); itr != convert_map.end(); ++itr)
    {
        if (input_file.end_with(itr->first))
        {
            return itr->second(argc, argv);
        }
    }
    print("Convert input file extension is not supported.");
    return -1;
}

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
        return convert(argc - 2, argv + 2);
    }
    else
    {
        print("Cannot parse indicator.\n", argv[1]);
        print("[help] for more information.\n");
        return -1;
    }
    return 0;
}
