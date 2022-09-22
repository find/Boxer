#include <boxer/boxer.h>

#include <stdlib.h>
#include <array>
#include <sstream>
#include <string>

namespace boxer {

static std::string escape(const char* str)
{
  std::string r;
  for (;*str ; ++str) {
    switch(*str) {
      case '"': 
        r += "\\\""; break;
      case '\'': 
        r += "\\\'"; break;
      case '\n':
        r += "\\n"; break;
      case '\r':
        break;
      default:
        r += *str;
    }
  }
  return r;
}

Selection show(const char *message, const char *title, Style style, Buttons buttons)
{
  std::stringstream cmdline;
  cmdline
    << "zenity --title=\""
    << escape(title)
    << "\" --text=\""
    << escape(message)
    << "\"";
  std::array<Selection,2> trans = {Selection::OK, Selection::Cancel};
  const char* type = " --info";
  if (style==Style::Warning)
    type = " --warning";
  else if (style==Style::Error)
    type = " --error";
  
  if (buttons==Buttons::OK) {
    cmdline << type;
    trans = {Selection::OK, Selection::None};
  } else if (buttons==Buttons::OKCancel) {
    cmdline
      << " --question"
      << " --ok-label=OK"
      << " --cancel-label=Cancel";
  } else if (buttons==Buttons::YesNo) {
    cmdline << " --question";
    trans = {Selection::Yes, Selection::No};
  } else if (buttons==Buttons::Quit) {
    cmdline
      << type
      << " --ok-label=Quit";
    trans = {Selection::Quit, Selection::None};
  }
  printf("cmdline: %s\n", cmdline.str().c_str());
  int code=system(cmdline.str().c_str());
  if (code==0)
    return trans[0];
  else if (code==1 || code==256) // os.system() may left-shift return code by 8
    return trans[1];
  else
    return Selection::None;
}

} // namespace boxer
