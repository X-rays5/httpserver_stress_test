#include <iostream>
#include <thread>
#include <cpr/cpr.h>
#include "argshandler.hpp"

void spam(std::string host, std::string r, std::string t) {
    int rate = std::stoi(r);
    int64_t times = std::stoi(t);
    if (times == -1) {
        times = 9223372036854775807;
    }
    for (int i = 0; i < times; i++) {
        cpr::Response res = cpr::Get(cpr::Url{host});
        if (res.status_code != 200) {
            std::cout << "Reponse: " << res.status_code << " Thread: " << std::this_thread::get_id() << "\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(rate));
    }
}

int main(int argc, char* argv[]) {
    argshandler args("--", true);
    args.AddKnownArg("host", "Specify address with port if needed of host target");
    args.AddKnownArg("users", "Amount of users that should be connecting");
    args.AddKnownArg("rate", "Rate of connections in ms");
    args.AddKnownArg("times", "How many times the program should open a connection before it's done -1 for infinite");

    args.HandleArgs(argc, argv);

    if (args.GetArg("host").empty()) {
        std::cerr << "Host was not supplied\n";
        return EXIT_FAILURE;
    }
    if (args.GetArg("users").empty()) {
        std::cerr << "Amount of users not specified\n";
        return EXIT_FAILURE;
    }
    if (args.GetArg("rate").empty()) {
        std::cerr << "Rate of connections not specified\n";
        return EXIT_FAILURE;
    }
    if (args.GetArg("times").empty()) {
        std::cerr << "Amount of times a connection should be made not specified\n";
        return EXIT_FAILURE;
    }
    if (std::stoi(args.GetArg("users")) > 300) {
        std::cout << "It isn't adviced to use more then 300 users on most computers\n";
        std::cout << "Press enter to continue";
        std::string buffer;
        std::getline(std::cin, buffer);
    }
    for (int i = 0; i < std::stoi(args.GetArg("users")) - 1; i++) {
        std::thread(spam, args.GetArg("host"), args.GetArg("rate"), args.GetArg("times")).detach();
    }
    std::cout << "Users started\n";
    spam(args.GetArg("host"), args.GetArg("rate"), args.GetArg("times"));

    std::cout << "Done\n";
    return EXIT_SUCCESS;
}
