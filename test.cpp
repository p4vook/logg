#include "logg.h"

int main()
{
    logg::Logger<logg::LogLevel::INFO> l;
    l << logg::INFO << "STARTED\n";
    l << logg::DEBUG << "HELLO?\n";
    for (int i = 0; i < 1'000'000; ++i) {
        l << logg::TRACE << "Testing " << i << "\n";
    }
    l << logg::INFO << "FINISHED\n";
}
