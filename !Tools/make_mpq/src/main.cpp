#include <StormLib.h>

#include <cinttypes>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace
{
    constexpr auto max_mpq_files = 4096;

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
        std::cerr << "Assumes listfile matches subdirectories of current working directory\n";
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
    // create mpq archive
    //

    HANDLE mpq = NULL;
    if (!SFileCreateArchive(mpq_filename, MPQ_CREATE_ARCHIVE_V1, max_mpq_files, &mpq))
    {
        std::cerr << "Failed to create MPQ: " << mpq_filename << '\n';
        return 1;
    }

    //
    // add files
    //

    for (auto const file : listfile)
    {
        // TODO try MPQ_FILE_IMPLODE, MPQ_FILE_IMPLODE | MPQ_FILE_ENCRYPTED
        if (!SFileAddFileEx(mpq, windows_to_unix_path(file).c_str(), file.c_str(), 0, 0, 0))
        {
            std::cout << "Failed to add file: " << file << '\n';
            continue;
        }
    }

    SFileCloseArchive(mpq);
    return 0;
}