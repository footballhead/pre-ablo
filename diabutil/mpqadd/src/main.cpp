#include <StormLib.h>

#include <iostream>
#include <string>

namespace
{
    std::string unix_to_windows(std::string path)
    {
        constexpr auto tofind = "/";
        for (auto place = path.find(tofind); place != std::string::npos; place = path.find(tofind))
        {
            path = path.replace(place, 1, "\\");
        }
        return path;
    }
} // namespace

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " file.mpq\n";
        std::cerr << "Reads listfile from stdin\n";
        return 1;
    }

    auto const mpq_filename = argv[1];

    //
    // open or create MPQ
    //

    HANDLE mpq = NULL;
    if (!SFileOpenArchive(mpq_filename, 0, BASE_PROVIDER_FILE, &mpq))
    {
        auto const err = GetLastError();
        if (err != ERROR_FILE_NOT_FOUND)
        {
            std::cerr << "Failed to open MPQ: err=" << err << " file=" << mpq_filename << '\n';
            return 1;
        }

        std::cout << mpq_filename << " doesn't exist, creating...\n";
        constexpr auto max_files = 1024; // Arbitrary...
        if (!SFileCreateArchive(mpq_filename, MPQ_CREATE_ARCHIVE_V1, max_files, &mpq))
        {
            std::cerr << "Failed to create MPQ: err=" << GetLastError() << " file=" << mpq_filename << '\n';
            return 1;
        }
    }

    //
    // add files specified on stdin
    //

    std::string line;
    while (std::getline(std::cin, line))
    {
        auto const winpath = unix_to_windows(line);

        // remove the file if it already exists (we will replace)
        if (SFileHasFile(mpq, winpath.c_str()))
        {
            if (!SFileRemoveFile(mpq, winpath.c_str(), 0))
            {
                std::cerr << "Failed to remove already existant file: " << line << " (" << winpath << ")\n";
                continue;
            }
        }

        // Add file. Don't bother compressing, the game loads fine and the ZIP is actually smaller
        if (!SFileAddFileEx(mpq, line.c_str(), winpath.c_str(), 0, 0, 0))
        {
            std::cerr << "Failed to add file: " << line << " (" << winpath << ")\n";
        }
    }

    SFileCloseArchive(mpq);
    return 0;
}
