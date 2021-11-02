#include "../Interface/Interface.hpp"

const s64 port = 39901;
const char* ip;

struct client_context
{
    network_connect connect;
    ref<P9::Platform::platform> platform;
};

static void client_handler(void* p)
{
    client_context* p_context = (client_context*)p;
    network_connect connect = p_context->connect;
    auto r_platform = p_context->platform;

    char* buf = new char[2048];
    while (1)
    {
        print("Waiting client query....\n");
        s64 msg_len = network_connect_wait_receive(connect, buf);
        if (msg_len <= 0)
        {
            break;
        }
        string query = buf;
        string rst;
        boole is_error_happens = boole::False;
        print("Received client query, start process...\n");
        try
        {
            auto op1 = r_platform->parse_operation_message(query);
            rst = r_platform->handle_operation(op1);
        }
        catch (P9_assert_exception ex)
        {
            is_error_happens = boole::True;
            rst = "Assert error happens when process query.";
            err(rst.data());
            err("error_code: %ll, expected_condition: %s, information: %s, file: %s, line: ll.",
                ex.error_code, ex.expect_condition, ex.information, ex.file, ex.line);
            print("%s\n", rst.data());
        }
        catch (...)
        {
            is_error_happens = boole::True;
            rst = "Unknown error happens when process query.";
            log(rst.data());
            print("%s\n", rst.data());
        }
        print("Finish client query, send result back to client...\n");
        if (rst.size() == 0 || !network_connect_send(connect, rst.data(), rst.size() + 1))
        {
            is_error_happens = boole::True;
            log("Error happens when send result back.");
            print("Error happens when send result back.\n");
        }
        print("Complete query.\n");
    }
    print("Client disconnect to server.\n");
    network_connect_destroy(connect);
    delete[] buf;
    delete p_context;
}

void server_entry_point()
{
    tick_start();

    print("Init DB server...\n");
    auto r_platfrom = ref<P9::Platform::platform>::new_instance();
    r_platfrom->load(P9_FOLDER "p1/");

    auto server = network_server_create(port);
    print("Complete DB init...\n");
    print("Waiting Client connect...\n");
    while (1)
    {
        client_context* p_context = new client_context();
        p_context->connect = network_connect_create(server);
        print("Client connected to server.\n");
        p_context->platform = r_platfrom;
        thrd_create(client_handler, p_context);
    }
    network_server_destroy(server);
}

void read_file_send_query(const char* path)
{
    char buf[8000];
    auto f = input_file_create(path);
    s64 len = input_file_read(f, buf, 8000);
    buf[len] = 0;
    input_file_destroy(f);
    string query(buf);

    auto client = network_client_create(ip, port);
    network_client_send(client, query.data(), query.size() + 1);
    len = network_client_wait_receive(client, buf);
    print("Result: %s.\n", buf);
    network_client_destroy(client);
}

void client_entry_point(const char* json_file_path)
{
    read_file_send_query(json_file_path);
}

int main(int argc, char* argv[])
{
    if (argc == 2 && str_equal_case_insensitive(argv[1], "host"))
    {
        server_entry_point();
        return 0;
    }
    else if (argc == 3)
    {
        ip = argv[1];
        client_entry_point(argv[2]);
        return 0;
    }
    print("input parameters: \n \"Host\" or \"20.40.99.127 ./json_query_file.json\"\n");
    return -1;
}
