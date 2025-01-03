#include <StormLib.h>

#include <filesystem>
#include <iostream>
#include <string>

namespace {

std::string remove_all(std::string str, std::string const &to_remove) {
  for (auto place = str.find(to_remove); place != std::string::npos;
       place = str.find(to_remove)) {
    str = str.erase(place, to_remove.length());
  }
  return str;
}

void print_usage(char *const program_name) {
  std::cerr << "Usage: " << program_name << " file.mpq\n"
            << "Reads listfile from stdin then, for each line in stdin, "
               "extracts that file relative to the working directory.\n"
            << "\n"
            << "Suggested usage (on Linux):\n"
            << "    cat listfile.txt | " << program_name << " DIABDAT.MPQ\n";
}

}  // namespace

int main(int argc, char **argv) {
  if (argc < 2) {
    print_usage(argv[0]);
    return 1;
  }

  auto const mpq_filename = argv[1];

  HANDLE mpq = NULL;
  if (!SFileOpenArchive(mpq_filename, 0, BASE_PROVIDER_FILE, &mpq)) {
    std::cerr << "Failed to open MPQ: err=" << GetLastError()
              << " file=" << mpq_filename << '\n';
    return 1;
  }

  int return_code = 0;
  std::string line;
  while (std::getline(std::cin, line)) {
    // This is more for my sanity (especially on Docker with Windows files
    // mounted)
    line = remove_all(line, "\r");

    // StormLib expects parent paths to already exist
    auto const localdir = std::filesystem::path{line}.parent_path();
    if (!std::filesystem::exists(localdir)) {
      if (!std::filesystem::create_directories(localdir)) {
        std::cerr << "Failed to create directory. Continuing... localdir="
                  << localdir << '\n';
        continue;
      }
    }

    // More helpful errors
    if (!SFileHasFile(mpq, line.c_str())) {
      std::cerr << "No file in MPQ: " << line << ". Continuing...\n";
      continue;
    }

    if (!SFileExtractFile(mpq, line.c_str(), line.c_str(),
                          SFILE_OPEN_FROM_MPQ)) {
      std::cerr << "Failed to extract file. Continuing...\n"
                << "  err=" << GetLastError() << '\n'
                << "  line=" << line << '\n';
      return_code = 1;
      continue;
    }
  }

  SFileCloseArchive(mpq);
  return return_code;
}
