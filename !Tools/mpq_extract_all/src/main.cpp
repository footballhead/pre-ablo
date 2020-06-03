#include <StormLib.h>

#include <cinttypes>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace
{
    std::string windows_to_unix_path(std::string path)
    {
        while (auto place = path.find('\\'))
        {
            if (place == std::string::npos)
            {
                break;
            }
            path = path.replace(place, 1, "/");
        }
        return path;
    }

    std::vector<std::string> read_all_lines(std::ifstream &in)
    {
        std::vector<std::string> builder;
        std::string line;
        while (std::getline(in, line))
        {
            builder.push_back(line);
        }
        return builder;
    }
} // namespace

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " file.mpq listfile.txt\n";
        std::cerr << "Will create subdirectories in the current working directory\n";
        return 1;
    }

    auto const mpq_filename = argv[1];
    auto const list_filename = argv[2];

    //
    // read list file into memory
    //

    std::ifstream list_in{list_filename};
    if (!list_in.good())
    {
        std::cerr << "Failed to open listfile: " << list_filename << '\n';
        return 1;
    }

    std::vector<std::string> listfile = read_all_lines(list_in);
    list_in.close();

    //
    // open mpq archive
    //

    HANDLE mpq = NULL;
    if (!SFileOpenArchive(mpq_filename, 0, BASE_PROVIDER_FILE, &mpq))
    {
        std::cerr << "Failed to open MPQ: " << mpq_filename << '\n';
        return 1;
    }

    //
    // extract files
    //

    for (auto const file : listfile)
    {
        HANDLE handle = nullptr;
        if (!SFileOpenFileEx(mpq, file.c_str(), SFILE_OPEN_FROM_MPQ, &handle))
        {
            std::cout << "Failed to open: " << file << '\n';
            continue;
        }

        uint32_t size = 0;
        if (SFileGetFileInfo(handle, SFileInfoFileSize, &size, sizeof(size), nullptr))
        {
            std::unique_ptr<char[]> buffer{new char[size]};
            if (SFileReadFile(handle, buffer.get(), size, NULL, NULL))
            {
                auto const fs_path = std::filesystem::path{windows_to_unix_path(file)};
                std::filesystem::create_directories(fs_path.parent_path());
                std::ofstream out{fs_path.c_str()};
                out.write(buffer.get(), size);
            }
            else
            {
                std::cout << "Failed to read: " << file << '\n';
            }
        }
        else
        {
            std::cout << "Failed to get size: " << file << '\n';
        }

        SFileCloseFile(handle);
    }

    // SFileCloseArchive(mpq);
    return 0;
}