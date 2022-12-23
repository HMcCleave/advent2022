#ifndef STRINGSTUFF_HPP
#define STRINGSTUFF_HPP

#include <string>
#include <tuple>
#include <vector>

std::string joinv(const std::vector<std::string>& parts, const std::string& inter) {
  if (parts.empty())
    return "";
  std::string out = parts.front();
  for (auto iter = parts.begin() + 1; iter != parts.end(); ++iter)
    out = out + inter + *iter;
  return out;
}

std::string to_string(const std::string& s) {
  return s;
}

template <typename... Args>
std::string join(const std::string& inter, Args... args) {
  std::vector<std::string> asvec{to_string(args)...};
  return joinv(asvec, inter);
}

template<typename... Args>
std::string joint(const std::string& inter, const std::tuple<Args...>& t) {
  auto binder = [&](auto... args) { return join(inter, args...); };
  return std::apply(binder, t);
}

std::vector<std::string> split(const std::string& large, const std::string& marker) {
  std::vector<std::string> out;
  size_t iter = 0;
  size_t pos = 0;
  while ((iter = large.find(marker, pos)) != std::string::npos) {
    out.emplace_back(large.begin() + pos, large.begin() + iter);
    pos = iter + marker.length();
  }
  out.push_back(large.substr(pos));
  return out;
}

#endif // STRINGSTUFF_HPP