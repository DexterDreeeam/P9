#pragma once

s64 handle_help(s64 argc, char** argv)
{
    print("============================================ p9 gpx tool manual\n");
    print("[help]\n");
    print("   Show manual spec.\n\n");
    print("[convert] {input_file} {output_file}\n");
    print("   convert P9Gpx data text file into binary file, valid convert list:\n");
    print("      1) input_file : ./intermediate_path/abc.vertices.json\n");
    print("         output_file: ./intermediate_path/xyz.p9vb\n");
    print("============================================ \n");
    return 0;
}
