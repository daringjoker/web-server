
#include <cstddef>
constexpr unsigned char hexCharTod(const char c) {
  if (c == '#') {
    return 0;  // this is ok i guess
  }

  if (c >= '0' && c <= '9') {
    return c - '0';
  }

  if (c >= 'A' && c <= 'F') {
    return 10 + (c - 'A');
  }

  if (c >= 'a' && c <= 'f') {
    return 10 + (c - 'a');
  }

  return 0;  // waszup dawg
}
constexpr unsigned long long colorCode(const char* hexStream,
                                       unsigned int len) {
  unsigned long long value = 0;
  unsigned index = 0;
  unsigned v = len <= 6 ? 4 : 0;
  while (hexStream[index] != 0 && index < len) {
    unsigned int a = hexCharTod(hexStream[index]);
    value = (value << 4) | (a & 0xf);
    value = value << v;
    value |= (v == 4 && a > 0) ? 0xf : 0;
    index++;
  }
  return value;
}

typedef char const* EscapeSequence;

template <std::size_t N>
struct FG_COLOR {
  char color[21] = {'\033', '[', '3', '8', ';', '2', ';', 'X', 'X', 'X',
                    ';',    'X', 'X', 'X', ';', 'X', 'X', 'X', 'm', '\0'};

  constexpr FG_COLOR(char const (&pp)[N]) {
    unsigned long n = colorCode(pp, N);
    const unsigned char r = n >> 16 & 0xff;
    auto r1 = r % 10;
    auto r2 = (r / 10) % 10;
    auto r3 = (r / 100) % 10;
    color[7] = '0' + r3;
    color[8] = '0' + r2;
    color[9] = '0' + r1;
    const unsigned char g = n >> 8 & 0xff;
    auto g1 = g % 10;
    auto g2 = (g / 10) % 10;
    auto g3 = (g / 100) % 10;
    color[11] = '0' + g3;
    color[12] = '0' + g2;
    color[13] = '0' + g1;
    const unsigned char b = n & 0xff;
    auto b1 = b % 10;
    auto b2 = (b / 10) % 10;
    auto b3 = (b / 100) % 10;
    color[15] = '0' + b3;
    color[16] = '0' + b2;
    color[17] = '0' + b1;
  };
};

template <std::size_t N>
struct BG_COLOR {
  char color[21] = {'\033', '[', '4', '8', ';', '2', ';', 'X', 'X', 'X',
                    ';',    'X', 'X', 'X', ';', 'X', 'X', 'X', 'm', '\0'};

  constexpr BG_COLOR(char const (&pp)[N]) {
    unsigned long n = colorCode(pp, N);
    const unsigned char r = n >> 16 & 0xff;
    auto r1 = r % 10;
    auto r2 = (r / 10) % 10;
    auto r3 = (r / 100) % 10;
    color[7] = '0' + r3;
    color[8] = '0' + r2;
    color[9] = '0' + r1;
    const unsigned char g = n >> 8 & 0xff;
    auto g1 = g % 10;
    auto g2 = (g / 10) % 10;
    auto g3 = (g / 100) % 10;
    color[11] = '0' + g3;
    color[12] = '0' + g2;
    color[13] = '0' + g1;
    const unsigned char b = n & 0xff;
    auto b1 = b % 10;
    auto b2 = (b / 10) % 10;
    auto b3 = (b / 100) % 10;
    color[15] = '0' + b3;
    color[16] = '0' + b2;
    color[17] = '0' + b1;
  };
};
template <FG_COLOR colorString>
constexpr EscapeSequence operator""_fg() {
  return colorString.color;
}

template <BG_COLOR colorString>
constexpr EscapeSequence operator""_bg() {
  return colorString.color;
}
