#pragma once

s64 handle_convert(s64 argc, char** argv)
{
    if (argc != 2)
    {
        print("Parameter number error, refer to --help for more information\n");
        return -1;
    }
    string input_file = argv[0];
    string output_file = argv[1];
    if (!input_file.end_with(gpx::vertices_buffer::vertices_json_extension_name))
    {
        print("Input_file should be '%s' extension file.\n",
            gpx::vertices_buffer::vertices_json_extension_name.data());
        return -1;
    }
    if (!output_file.end_with(gpx::vertices_buffer::vertices_binary_extension_name))
    {
        print("Output_file should be '%s' extension file.\n",
            gpx::vertices_buffer::vertices_binary_extension_name.data());
        return -1;
    }
    auto vb = gpx::vertices_buffer::load(input_file);
    if (vb.empty())
    {
        print("Error happens when parsing '%s' file.\n", input_file.data());
        return -1;
    }
    if (!vb->write_binary(output_file))
    {
        print("Error happens when writing to '%s' file.\n", output_file.data());
        return -1;
    }
    print("Convert succeed.\n");
    return 0;
}
