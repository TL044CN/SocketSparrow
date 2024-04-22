#define CATCH_CONFIG_RUNNER
#include <catch2/catch_session.hpp>

int main(int argc, char* argv[]) {
    Catch::Session session;
    
    int returnCode = session.applyCommandLine(argc, argv);
    if ( returnCode != 0 ) // Indicates a command line error
        return returnCode;
    
    return session.run(argc, argv);
}