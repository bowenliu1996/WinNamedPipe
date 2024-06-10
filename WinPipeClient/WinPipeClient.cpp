#include <iostream>
#include "NamePipe.h"

int main() {
    std::wstring pipeName = L"\\\\.\\pipe\\my_named_pipe";

    // Server
    NamedPipe serverPipe(pipeName, 2048);
    if (!serverPipe.createServer()) {
        std::wcerr << L"Failed to create named pipe server." << std::endl;
        return 1;
    }
    std::wcout << L"Named pipe server created successfully." << std::endl;

    // Server reads the message from the pipe
    std::wstring receivedMessage;
    if (!serverPipe.read(receivedMessage)) {
        std::wcerr << L"Failed to read from named pipe." << std::endl;
        return 1;
    }
    std::wcout << L"Message read from named pipe: " << receivedMessage << std::endl;

    serverPipe.close();
    system("pause");
    return 0;
}