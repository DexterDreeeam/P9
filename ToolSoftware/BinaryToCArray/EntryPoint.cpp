
#include "../../Sdk/Interface.hpp"

const u64 line_u8_count = 16;

char hex_bit_to_char(u8 v)
{
    v = v % 16;
    if (v < 10)
    {
        return '0' + v;
    }
    else
    {
        return 'a' + v - 10;
    }
}

string u8_to_string(u8 v)
{
    u8 dec = v / 16;
    u8 dig = v % 16;

    string r = "";
    r += hex_bit_to_char(dec);
    r += hex_bit_to_char(dig);
    return r;
}

int main(int argc, const char* argv[])
{
    if (argc < 2)
    {
        return 1;
    }
    string input_path = argv[1];
    file f_input;
    f_input.init_input(input_path.c_str());

    u64 ts = date::timestamp();
    char ts_cstr[40] = {};
    u64_to_text(ts, ts_cstr);
    string output_path;
    output_path += "./";
    output_path += ts_cstr;
    output_path += ".h";

    if (!file::create(output_path.c_str()))
    {
        print("Create output file failed.\n");
    }

    file f_output;
    f_output.init_output(output_path.c_str());
    f_output.output("\n");
    f_output.output("unsigned char BinaryArray[] =\n");
    f_output.output("{\n");

    u64 line = 0;
    while (1)
    {
        u8 buf[line_u8_count] = {};
        s64 actual_read = 0;
        boole read_rst = f_input.input(buf, line_u8_count, actual_read);
        if (!read_rst)
        {
            print("Read failed.\n");
        }
        if (actual_read <= 0)
        {
            break;
        }

        boole write_rst;
        write_rst = f_output.output("    ");
        if (!write_rst)
        {
            print("Write failed.\n");
        }
        for (s64 i = 0; i < actual_read; ++i)
        {
            u8 value = buf[i];
            string value_str = "";
            value_str += i > 0 ? " 0x" : "0x";
            value_str += u8_to_string(value).c_str();
            value_str += ",";

            write_rst = f_output.output(value_str.c_str());
            if (!write_rst)
            {
                print("Write failed.\n");
            }
        }
        write_rst = f_output.output("\n");
        ++line;

        if (actual_read < line_u8_count)
        {
            break;
        }
    }
    f_output.output("};\n");
    return 0;
}
