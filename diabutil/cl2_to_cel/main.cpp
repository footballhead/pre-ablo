#include <array>
#include <cinttypes>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <limits>
#include <optional>
#include <stdexcept>
#include <vector>

namespace {

#ifndef NDEBUG
#define LOG(msg) fprintf(stderr, msg "\n")
#define LOGV(msg, ...) fprintf(stderr, msg "\n", __VA_ARGS__)
#else
#define LOG(msg) fprintf(stderr, msg "\n")
#define LOGV(msg, ...)
#endif

using byte_vector = std::vector<std::byte>;

constexpr auto usage =
    R"help(Usage: %s -i input.cl2 -w X [-g Y] [-s 0..4] -o output.cel
-i input.cl2     input .CL2 file
-w X             the width of all frames (assumed constant)
-g Y             (optional) number of groups in input
-s 0..4          (optional) the skip offset to use (valid 0 to 4)
                 presense of the flag causes the header to be considered
                 if you don't want a skip, but do want a header, use -s 0
-o output.cel    output .CEL file
)help";

struct arguments {
  char const *in_cl2 = nullptr;
  char const *out_cel = nullptr;
  std::optional<int> num_groups = std::nullopt;
  std::optional<int> skip = std::nullopt;
  int width = 0;
};

std::optional<arguments> parse_args(int argc, char **argv) {
  auto builder = arguments{};

  // assume argv[0] is program name and skip it
  int shift = 1;

  while (shift < argc) {
    char const *option = argv[shift];
    ++shift;

    if (strcmp(option, "-i") == 0) {
      if (shift >= argc) {
        fprintf(stderr, "Provide an input file!\n");
        return std::nullopt;
      }

      builder.in_cl2 = argv[shift];
      ++shift;
      LOGV("-i %s", builder.in_cl2);
    } else if (strcmp(option, "-o") == 0) {
      if (shift >= argc) {
        fprintf(stderr, "Provide an output file!\n");
        return std::nullopt;
      }

      builder.out_cel = argv[shift];
      ++shift;
      LOGV("-o %s", builder.in_cl2);
    } else if (strcmp(option, "-g") == 0) {
      if (shift >= argc) {
        fprintf(stderr, "Provide number of groups!\n");
        return std::nullopt;
      }

      builder.num_groups = std::stoi(argv[shift]);
      ++shift;
      LOGV("-g %d", builder.num_groups.value());
    } else if (strcmp(option, "-s") == 0) {
      if (shift >= argc) {
        fprintf(stderr, "Provide skip!\n");
        return std::nullopt;
      }

      builder.skip = std::stoi(argv[shift]);
      ++shift;
      LOGV("-s %d", builder.skip.value());
    } else if (strcmp(option, "-w") == 0) {
      if (shift >= argc) {
        fprintf(stderr, "Provide width!\n");
        return std::nullopt;
      }

      builder.width = std::stoi(argv[shift]);
      ++shift;
      LOGV("-w %d", builder.width);
    }
  }

  if (!builder.in_cl2) {
    fprintf(stderr, "Missing input file\n");
    return std::nullopt;
  }

  if (!builder.out_cel) {
    fprintf(stderr, "Missing output file\n");
    return std::nullopt;
  }

  if (builder.num_groups && builder.num_groups.value() <= 0) {
    fprintf(stderr, "Invalid number of groups\n");
    return std::nullopt;
  }

  if (builder.skip) {
    if (builder.skip.value() < 0 || builder.skip.value() > 4) {
      fprintf(stderr, "Invalid skip\n");
      return std::nullopt;
    }
  }

  if (builder.width <= 0) {
    fprintf(stderr, "Invalid width\n");
    return std::nullopt;
  }

  return builder;
}

std::optional<byte_vector> read_file(char const *filename) {
  // Open file (`ate` puts internal marker at EOF so we can tellg for size)
  std::ifstream in{filename, std::ios_base::binary | std::ios_base::ate};
  if (!in.good()) {
    fprintf(stderr, "Open fail\n");
    return std::nullopt;
  }

  // Get size. (StackOverflow will try to convince you that this is not the way
  // to go but it's literally in cplusplus.com:
  // https://www.cplusplus.com/reference/istream/istream/tellg/)
  auto const file_size = in.tellg();
  in.seekg(0, std::ios::beg);

  // Read file contents
  auto buffer = byte_vector(file_size, std::byte{0});
  if (!in.read(reinterpret_cast<char *>(buffer.data()), file_size)) {
    fprintf(stderr, "Read fail\n");
    return std::nullopt;
  }

  return buffer;
}

// A data/size aggregate with some smarts. Referenced data is read-only.
struct const_byte_span {
  std::byte const *data = nullptr;
  size_t size = 0;

  static constexpr const_byte_span null_span() {
    return {
        .data = nullptr,
        .size = 0,
    };
  }

  bool operator==(const_byte_span const &rhs) const {
    return data == rhs.data && size == rhs.size;
  }

  // Advance the span like an iterator
  const_byte_span &operator+=(size_t i) {
    if (i > size) {
      throw std::runtime_error{"+=: i > size"};
    }
    data += i;
    size -= i;
    return *this;
  }

  // Advance the span like an iterator, returning new span iterator
  friend const_byte_span operator+(const_byte_span lhs, size_t rhs) {
    lhs += rhs;
    return lhs;
  }

  // Reinterpret some new data from the span
  template <typename T>
  operator T() const {
    if (sizeof(T) > size) {
      throw std::runtime_error{"T: i > size"};
    }
    return *reinterpret_cast<T const *>(data);
  }

  // Treat like a byte iterator and look at front
  std::byte operator*() const { return *data; }

  // Treat like a byte iterator and move forward one spot
  const_byte_span &operator++() {
    *this += sizeof(std::byte);
    return *this;
  }
};

inline const_byte_span make_span(byte_vector const &vec) {
  return {
      .data = vec.data(),
      .size = vec.size(),
  };
}

std::vector<const_byte_span> find_groups(const_byte_span const cl2,
                                         size_t const num_groups) {
  // create a copy to mutate like an iterator
  auto cl2_I = cl2;

  auto groups = std::vector<const_byte_span>{};
  groups.reserve(num_groups);

  for (size_t i = 0; i < num_groups; ++i) {
    auto const start = static_cast<uint32_t>(cl2_I);
    cl2_I += sizeof(uint32_t);
    // The last group doesn't have an end offset so use total size
    auto const end =
        (i == num_groups - 1) ? cl2.size : static_cast<uint32_t>(cl2_I);
    // don't advance, stay here for next read

    // `start` is relative to file beginning so reference `cl2` (not cl2_I)
    groups.emplace_back(const_byte_span{
        .data = cl2.data + start,
        .size = end - start,
    });
  }

  return groups;
}

std::vector<const_byte_span> find_frames(const_byte_span const cl2) {
  if (cl2.size == 0) {
    return {};
  }

  // create a copy to mutate like an iterator
  auto cl2_I = cl2;

  auto const num_frames = static_cast<uint32_t>(cl2_I);
  cl2_I += sizeof(uint32_t);
  LOGV("numframes: %u", num_frames);

  auto frames = std::vector<const_byte_span>{};
  frames.reserve(num_frames);

  for (uint32_t i = 0; i < num_frames; ++i) {
    if (sizeof(uint32_t) > cl2_I.size) {
      fprintf(stderr, "Ran out of frame offset table\n");
      return {};
    }
    auto const start = static_cast<uint32_t>(cl2_I);
    cl2_I += sizeof(uint32_t);

    if (sizeof(uint32_t) > cl2_I.size) {
      fprintf(stderr, "Ran out of frame offset table\n");
      return {};
    }
    auto const end = static_cast<uint32_t>(cl2_I);
    // don't advance, stay here for next read

    // `start` is relative to group beginning so reference `cl2` (not cl2_I)
    frames.emplace_back(const_byte_span{
        .data = cl2.data + start,
        .size = end - start,
    });
    LOGV("%u size: %zu", i, frames.back().size);
  }

  return frames;
}

const_byte_span apply_skip(const_byte_span const frame, int const skip) {
  if (skip < 0 || skip > 4) {
    fprintf(stderr, "Invalid skip\n");
    return const_byte_span::null_span();
  }

  if (skip * sizeof(uint16_t) > frame.size) {
    fprintf(stderr, "Not enough data for skip\n");
    return const_byte_span::null_span();
  }

  uint16_t const *const skip_table =
      reinterpret_cast<uint16_t const *const>(frame.data);
  uint16_t const skip_offset = skip_table[skip];

  if (skip_offset == 0) {
    fprintf(stderr, "Skip table entry is 0\n");
    return const_byte_span::null_span();
  }

  if (skip_offset > frame.size) {
    fprintf(stderr, "Read skip, not enough data to apply it\n");
    return const_byte_span::null_span();
  }

  // cast is required to resolve overload ambiguity in Apple Clang
  return frame + static_cast<size_t>(skip_offset);
}

class cel_builder {
 public:
  explicit cel_builder(int const width) : _width(width) {}

  void copy(std::byte const *pixels, int8_t n) {
    // TODO const_byte_stpan
    // TODO extra data validation

    while (n > 0) {
      // Chunk operation into multiple based on image width (allows proper
      // skip header generation)
      auto const remaining = _width - _column;
      // This is min(n, remaining, int8_t::max) but won't overflow
      auto const subsize = static_cast<int8_t>(std::min<int>(
          n, std::min<int>(remaining, std::numeric_limits<int8_t>::max())));

      // Translate to CEL copy instruction
      _data.push_back(static_cast<std::byte>(subsize));
      for (int8_t copyi = 0; copyi < n; ++copyi) {
        _data.push_back(pixels[copyi]);
      }

      // Advance internal iterators to ensure we generate a skip header entry
      advance_column(subsize);

      // Advance the provided iterators
      n -= subsize;
      pixels += subsize;
    }
  }

  void fill(std::byte const pixel, int8_t n) {
    // TODO extra data validation

    while (n > 0) {
      // Chunk operation into multiple based on image width (allows proper
      // skip header generation)
      auto const remaining = _width - _column;
      // This is min(n, remaining, int8_t::max) but won't overflow
      auto const subsize = static_cast<int8_t>(std::min<int>(
          n, std::min<int>(remaining, std::numeric_limits<int8_t>::max())));

      // Translate to CEL copy instruction
      _data.push_back(static_cast<std::byte>(subsize));
      _data.resize(_data.size() + n, pixel);

      // Advance internal iterators to ensure we generate a skip header entry
      advance_column(subsize);

      // Advance the provided iterators
      n -= subsize;
    }
  }

  void transparent(int8_t n) {
    // TODO extra data validation

    while (n > 0) {
      // Chunk operation into multiple based on image width (allows proper
      // skip header generation)
      auto const remaining = _width - _column;
      // This is min(n, remaining, int8_t::max) but won't overflow
      auto const subsize = static_cast<int8_t>(std::min<int>(
          n, std::min<int>(remaining, std::numeric_limits<int8_t>::max())));

      _data.push_back(static_cast<std::byte>(-subsize));

      // Advance internal iterators to ensure we generate a skip header entry
      advance_column(subsize);

      // Advance the provided iterators
      n -= subsize;
    }
  }

  byte_vector build(bool generate_skip_header) {
    if (generate_skip_header) {
      // pad out missing entries with 0
      static size_t constexpr header_size = 5;
      _skip_header.resize(header_size, 0);

      // insert header at beginning
      std::byte const *const raw_skip_header =
          reinterpret_cast<std::byte const *const>(_skip_header.data());
      size_t const raw_skip_header_size =
          _skip_header.size() * sizeof(uint16_t);
      _data.insert(begin(_data), raw_skip_header,
                   raw_skip_header + raw_skip_header_size);
    }
    return std::move(_data);
  }

 private:
  static constexpr size_t raw_header_size = 0xA;

  void advance_column(int n) {
    if (_column + n < 0) {
      throw std::runtime_error{"Bad column!"};
    }
    _column += n;
    if (_column >= _width) {
      _column -= _width;
      advance_row();
    }
  }

  void advance_row() {
    ++_row;

    // emit header entry at the breakpoint
    auto constexpr static header_breakpoint = 32;
    if (_row % header_breakpoint == 0) {
      // TODO validate _data.size() + raw_header_size fits into a uint16_t
      _skip_header.push_back(_data.size() + raw_header_size);
    }
  }

  int const _width = 0;
  std::vector<uint16_t> _skip_header{raw_header_size};
  byte_vector _data;
  int _column = 0;
  int _row = 0;
};

byte_vector convert_frame(const_byte_span cl2_frame, int const width,
                          bool const generate_skip_header) {
  // treat cl2_frame like an iterator and is mutated

  auto builder = cel_builder{width};

  int row_size = 0;
  while (cl2_frame.size > 0) {
    auto instruction = static_cast<int8_t>(cl2_frame);
    cl2_frame += sizeof(int8_t);

    if (instruction == 0) {
      fprintf(stderr, "Detected bad instruction\n");
      return {};
    }

    // instruction encodes two things: opcode and size

    enum class opcodes {
      transparency,
      fill,
      copy,
    };
    auto const opcode = instruction >= 0    ? opcodes::transparency
                        : -instruction > 65 ? opcodes::fill
                                            : opcodes::copy;
    switch (opcode) {
      case opcodes::transparency: {
        auto const size = instruction;  // 0..127
        builder.transparent(size);
        break;
      }
      case opcodes::fill: {
        auto const size = (-instruction) - 65;  // 1..63 ?

        if (cl2_frame.size == 0) {
          fprintf(stderr, "Ran out of data for fill\n");
          return {};
        }
        auto const pixel = static_cast<std::byte>(cl2_frame);
        cl2_frame += sizeof(std::byte);

        builder.fill(pixel, size);
        break;
      }
      case opcodes::copy: {
        auto const size = -instruction;  // 1..65

        if (size > cl2_frame.size) {
          fprintf(stderr, "Ran out of data for copy\n");
          return {};
        }

        builder.copy(cl2_frame.data, size);
        cl2_frame += size;
        break;
      }
    }
  }

  return builder.build(generate_skip_header);
}

template <typename T>
void append(byte_vector &vec, T val) {
  auto const *reinterp = reinterpret_cast<std::byte const *const>(&val);
  auto const size = sizeof(T);
  vec.insert(end(vec), reinterp, reinterp + size);
}

template <>
void append(byte_vector &vec, const_byte_span val) {
  vec.insert(end(vec), val.data, val.data + val.size);
}

byte_vector encode_group(std::vector<byte_vector> const &cel_frames) {
  auto data = byte_vector{};

  // number of frames
  uint32_t const num_frames = static_cast<uint32_t const>(cel_frames.size());
  append(data, num_frames);

  // frame offset table
  uint32_t const frame_offset_table_size = (num_frames + 1) * sizeof(uint32_t);
  uint32_t frame_offset = sizeof(num_frames) + frame_offset_table_size;
  for (auto const &cel_frame : cel_frames) {
    append(data, frame_offset);
    frame_offset += cel_frame.size();
  }
  append(data, frame_offset);

  // frame data
  for (auto const &cel_frame : cel_frames) {
    append(data, make_span(cel_frame));
  }

  return data;
}

byte_vector convert_group(const_byte_span cl2_group, int width,
                          std::optional<int> skip) {
  auto const cl2_frames = find_frames(cl2_group);
  if (cl2_frames.empty()) {
    fprintf(stderr, "Failed to find frames\n");
    return {};
  }

  auto cel_frames = std::vector<byte_vector>{};
  cel_frames.reserve(cl2_frames.size());

  for (auto cl2_frame : cl2_frames) {
    if (skip) {
      cl2_frame = apply_skip(cl2_frame, *skip);
      if (cl2_frame == const_byte_span::null_span()) {
        fprintf(stderr, "Failed to skip\n");
        return {};
      }
    }

    auto cel_frame = convert_frame(cl2_frame, width, !!skip);
    if (cel_frame.empty()) {
      fprintf(stderr, "Failed to convert a frame\n");
      return {};
    }

    cel_frames.emplace_back(std::move(cel_frame));
  }

  return encode_group(cel_frames);
}

bool write_file(const_byte_span data, char const *filename) {
  std::ofstream out(filename, std::ios_base::binary);
  if (!out.good()) {
    fprintf(stderr, "Failed to open for write: %s\n", filename);
    return false;
  }

  out.write(reinterpret_cast<char const *const>(data.data), data.size);
  if (!out.good()) {
    fprintf(stderr, "Failed to write bytes: %s\n", filename);
    return false;
  }

  out.close();
  if (!out.good()) {
    fprintf(stderr, "Failed to close file: %s\n", filename);
    return false;
  }

  return true;
}

byte_vector encode_all_groups(std::vector<byte_vector> const &cel_all_groups) {
  auto data = byte_vector{};

  // group header
  auto const num_groups = cel_all_groups.size();
  uint32_t const group_offset_table_size = num_groups * sizeof(uint32_t);
  uint32_t group_offset = group_offset_table_size;
  for (auto const &cel_group : cel_all_groups) {
    append(data, group_offset);
    group_offset += cel_group.size();
  }

  // groups
  for (auto const &cel_group : cel_all_groups) {
    append(data, make_span(cel_group));
  }

  return data;
}

byte_vector convert_all_groups(const_byte_span cl2_all_groups, int groups,
                               int width, std::optional<int> skip) {
  auto const cl2_groups = find_groups(cl2_all_groups, groups);
  if (cl2_groups.empty()) {
    fprintf(stderr, "Failed to find groups\n");
    return {};
  }

  auto cel_all_groups = std::vector<byte_vector>{};
  for (auto const &cl2_group : cl2_groups) {
    auto cel_group = convert_group(cl2_group, width, skip);
    if (cel_group.empty()) {
      fprintf(stderr, "Failed to convert goup\n");
      return {};
    }
    cel_all_groups.emplace_back(std::move(cel_group));
  }

  return encode_all_groups(cel_all_groups);
}

}  // namespace

int main(int argc, char **argv) {
  if (argc <= 1) {
    fprintf(stderr, usage, argv[0]);
    return 1;
  }

  if (strcmp(argv[1], "--help") == 0) {
    fprintf(stderr, usage, argv[0]);
    return 0;
  }

  auto const args = parse_args(argc, argv);
  if (!args) {
    fprintf(stderr, "Failed to parse args\n");
    return 2;
  }

  auto const in_cl2_contents = read_file(args->in_cl2);
  if (!in_cl2_contents) {
    fprintf(stderr, "Failed to read: %s\n", args->in_cl2);
    return 3;
  }

  auto cel_data = byte_vector{};

  if (args->num_groups) {
    cel_data =
        convert_all_groups(make_span(*in_cl2_contents),
                           args->num_groups.value(), args->width, args->skip);
    if (cel_data.empty()) {
      fprintf(stderr, "Failed to perform group conversion\n");
      return 6;
    }
  } else {
    // if !num_groups then treat the cel as one "group" just don't include a
    // group header
    cel_data =
        convert_group(make_span(*in_cl2_contents), args->width, args->skip);
    if (cel_data.empty()) {
      fprintf(stderr, "Failed to perform conversion\n");
      return 5;
    }
  }

  if (!write_file(make_span(cel_data), args->out_cel)) {
    fprintf(stderr, "Failed to write output\n");
    return 4;
  }

  return 0;
}