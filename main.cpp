
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include "Server.h"
#include "Ip.h"


int main(int argc, char* argv[])
{
    try
    {
        std::string ipdbFile = "";
        std::string port = "8888";
        std::string ip = "0.0.0.0";

        if(argc < 2) {
            std::cerr << "please input ipdb file path: \n";
            exit(0);
        }

        ipdbFile = argv[1];
        if(argc == 3) {
            port = argv[2];
        }

        if(argc > 3) {
            ip = argv[3];
        }

        Ip::init(ipdbFile.c_str());
        Server s(port,ip);
        s.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
