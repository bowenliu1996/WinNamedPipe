#include <iostream>
#include "NamePipe.h"

int main() {
    std::wstring pipeName = L"\\\\.\\pipe\\my_named_pipe";


    NamedPipe clientPipe(pipeName, 2048);
    if (!clientPipe.createClient()) {
        std::wcerr << L"Failed to connect to named pipe server." << std::endl;
        return 1;
    }
    std::wcout << L"Connected to named pipe server successfully." << std::endl;

    // Client writes a message to the pipe
    std::wstring message = L"Hello, named pipe!";
    if (!clientPipe.write(message)) {
        std::wcerr << L"Failed to write to named pipe." << std::endl;
        return 1;
    }
    std::wcout << L"Message written to named pipe: " << message << std::endl;
    // Close the pipes
    clientPipe.close();


    return 0;
}
