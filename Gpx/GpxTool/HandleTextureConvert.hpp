#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "../External/stb/stb_image.h"

s64 handle_texture_convert(s64 argc, char** argv)
{
    if (argc != 2)
    {
        print("Parameter number error, refer to [help] for more information\n");
        return -1;
    }
    string input_file = argv[0];
    string output_file = argv[1];

    if (!output_file.end_with(".p9tt"))
    {
        print("Output file must be '.p9tt' when convert texture.\n", output_file.data());
        return -1;
    }

    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(input_file.data(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    if (pixels == nullptr)
    {
        print("Error happens when parsing input file %s.\n", input_file.data());
        return -1;
    }

    gpx::texture_header header = {};
    header.width = texWidth;
    header.height = texHeight;
    header.memory_length = texWidth * texHeight * 4;

    file f;
    if (!f.init_output(output_file.data()))
    {
        print("Error happens when open output file %s.\n", output_file.data());
        return -1;
    }
    if (!f.output(&header, sizeof(header)))
    {
        print("Error happens when write file %s.\n", output_file.data());
        return -1;
    }
    if (!f.output(pixels, header.memory_length))
    {
        print("Error happens when write file %s.\n", output_file.data());
        return -1;
    }
    if (!f.uninit())
    {
        print("Error happens when close output file %s.\n", output_file.data());
        return -1;
    }
    stbi_image_free(pixels);

    print("Texture %s [%llux%llu] convert succeed.\n", input_file.data(), header.width, header.height);
    return 0;
}
