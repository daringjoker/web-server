#pragma once
#include "color_helper.h"

#define rgb_fg(r, g, b) "\033[38;2;" #r ";" #g ";" #b "m"
#define rgb_bg(r, g, b) "\033[48;2;" #r ";" #g ";" #b "m"
#define escape(n) "\033[" #n "m"
typedef char const* EscapeSequence;
namespace Color {
constexpr EscapeSequence red = "#B70404"_fg;
constexpr EscapeSequence brightRed = "#FF0032"_fg;
constexpr EscapeSequence lightRed = "#FA9494"_fg;
constexpr EscapeSequence darkRed = "#890F0D"_fg;

constexpr EscapeSequence yellow = "#FBDF07"_fg;
constexpr EscapeSequence lightYellow = "#FFF89C"_fg;
constexpr EscapeSequence darkYellow = "#FAC213"_fg;

constexpr EscapeSequence black = "#000000"_fg;
constexpr EscapeSequence white = "#ffffff"_fg;

constexpr EscapeSequence lightGray = "#808080"_fg;
constexpr EscapeSequence darkGray = "#484848"_fg;
constexpr EscapeSequence gray = "#A9A9A9"_fg;

constexpr EscapeSequence pink = "#FF55BB"_fg;
constexpr EscapeSequence lightPink = "#FA2FB5"_fg;
constexpr EscapeSequence darkPink = "#F94892"_fg;

constexpr EscapeSequence lightGreen = "#03C988"_fg;
constexpr EscapeSequence darkGreen = "#54B435"_fg;
constexpr EscapeSequence green = "#16FF00"_fg;

constexpr EscapeSequence blue = "#0000DD"_fg;
constexpr EscapeSequence lightBlue = "#0000FF"_fg;
constexpr EscapeSequence darkBlue = "#000077";

constexpr EscapeSequence purple = "#FC28FB"_fg;
constexpr EscapeSequence lightPurple = "#EA8FEA"_fg;
constexpr EscapeSequence darkPurple = "#E15FED"_fg;
constexpr EscapeSequence reset = escape(39);

namespace esc {
constexpr EscapeSequence resetAll = escape(0);
constexpr EscapeSequence bold = escape(1);
constexpr EscapeSequence blink = escape(5);
constexpr EscapeSequence fastBlink = escape(6);
constexpr EscapeSequence reset_blink = escape(25);

constexpr EscapeSequence reset_bold = escape(22);
constexpr EscapeSequence italic = escape(3);
constexpr EscapeSequence reset_italic = escape(23);
constexpr EscapeSequence faint = escape(2);
constexpr EscapeSequence reset_faint = escape(22);
constexpr EscapeSequence underline = escape(4);
constexpr EscapeSequence reset_underline = escape(24);
constexpr EscapeSequence invert_color = escape(7);
constexpr EscapeSequence reset_invert_color = escape(27);

}  // namespace esc

namespace bg {
constexpr EscapeSequence red = "#B70404"_bg;
constexpr EscapeSequence brightRed = "#FF0032"_bg;
constexpr EscapeSequence lightRed = "#FA9494"_bg;
constexpr EscapeSequence darkRed = "#890F0D"_bg;

constexpr EscapeSequence yellow = "#FBDF07"_bg;
constexpr EscapeSequence lightYellow = "#FFF89C"_bg;
constexpr EscapeSequence darkYellow = "#FAC213"_bg;

constexpr EscapeSequence black = "#000000"_bg;
constexpr EscapeSequence white = "#ffffff"_bg;

constexpr EscapeSequence lightGray = "#808080"_bg;
constexpr EscapeSequence darkGray = "#484848"_bg;
constexpr EscapeSequence gray = "#A9A9A9"_bg;

constexpr EscapeSequence pink = "#FF55BB"_bg;
constexpr EscapeSequence lightPink = "#FA2FB5"_bg;
constexpr EscapeSequence darkPink = "#F94892"_bg;

constexpr EscapeSequence lightGreen = "#03C988"_bg;
constexpr EscapeSequence darkGreen = "#54B435"_bg;
constexpr EscapeSequence green = "#16FF00"_bg;

constexpr EscapeSequence blue = "#0000DD"_bg;
constexpr EscapeSequence lightBlue = "#0000FF"_bg;
constexpr EscapeSequence darkBlue = "#000077";

constexpr EscapeSequence purple = "#FC28FB"_bg;
constexpr EscapeSequence lightPurple = "#EA8FEA"_bg;
constexpr EscapeSequence darkPurple = "#E15FED"_bg;
constexpr EscapeSequence reset = escape(49);
}  // namespace bg
};  // namespace Color
