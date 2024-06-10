#ifndef NAMEDPIPE_H
#define NAMEDPIPE_H

#include <windows.h>
#include <string>

class NamedPipe {
public:
    // Constructor and Destructor
    NamedPipe(const std::wstring& pipeName, DWORD bufferSize = 512);
    ~NamedPipe();

    // Create server and client
    bool createServer();
    bool createClient();

    // Write and read methods
    bool write(const std::wstring& message);
    bool read(std::wstring& message);

    // Close the pipe
    void close();

private:
    std::wstring pipeName;
    HANDLE hPipe;
    DWORD bufferSize; // Buffer size in characters
};

#endif // NAMEDPIPE_H
