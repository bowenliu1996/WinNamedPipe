#include "NamePipe.h"
#include <vector>

NamedPipe::NamedPipe(const std::wstring& pipeName, DWORD bufferSize)
    : pipeName(pipeName), hPipe(INVALID_HANDLE_VALUE), bufferSize(bufferSize) {}

NamedPipe::~NamedPipe() {
    close();
}

bool NamedPipe::createServer() {
    hPipe = CreateNamedPipe(
        pipeName.c_str(),              // Pipe name
        PIPE_ACCESS_DUPLEX,            // Read/Write access
        PIPE_TYPE_MESSAGE |            // Message type pipe
        PIPE_READMODE_MESSAGE |        // Message-read mode
        PIPE_WAIT,                     // Blocking mode
        PIPE_UNLIMITED_INSTANCES,      // Max instances
        bufferSize * sizeof(wchar_t),  // Output buffer size (in bytes)
        bufferSize * sizeof(wchar_t),  // Input buffer size (in bytes)
        0,                             // Default timeout
        nullptr);                      // Default security attributes

    if (hPipe == INVALID_HANDLE_VALUE) {
        return false;
    }

    return ConnectNamedPipe(hPipe, nullptr) ? true : (GetLastError() == ERROR_PIPE_CONNECTED);
}

bool NamedPipe::createClient() {
    hPipe = CreateFile(
        pipeName.c_str(),              // Pipe name
        GENERIC_READ |                 // Read and write access
        GENERIC_WRITE,
        0,                             // No sharing
        nullptr,                       // Default security attributes
        OPEN_EXISTING,                 // Opens existing pipe
        0,                             // Default attributes
        nullptr);                      // No template file

    if (hPipe == INVALID_HANDLE_VALUE) {
        return false;
    }

    return true;
}

bool NamedPipe::write(const std::wstring& message) {
    if (hPipe == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD bytesWritten;
    BOOL result = WriteFile(
        hPipe,                         // Handle to the pipe
        message.c_str(),               // Buffer to write from
        message.length() * sizeof(wchar_t),  // Number of bytes to write
        &bytesWritten,                 // Number of bytes written
        nullptr);                      // Not overlapped I/O

    return result && bytesWritten == message.length() * sizeof(wchar_t);
}

bool NamedPipe::read(std::wstring& message) {
    if (hPipe == INVALID_HANDLE_VALUE) {
        return false;
    }

    std::vector<wchar_t> buffer(bufferSize);
    DWORD bytesRead;
    BOOL result = ReadFile(
        hPipe,                         // Handle to the pipe
        buffer.data(),                 // Buffer to read into
        buffer.size() * sizeof(wchar_t),// Size of buffer (in bytes)
        &bytesRead,                    // Number of bytes read
        nullptr);                      // Not overlapped I/O

    if (result && bytesRead != 0) {
        buffer[bytesRead / sizeof(wchar_t)] = L'\0';
        message = buffer.data();
        return true;
    }

    return false;
}

void NamedPipe::close() {
    if (hPipe != INVALID_HANDLE_VALUE) {
        CloseHandle(hPipe);
        hPipe = INVALID_HANDLE_VALUE;
    }
}
