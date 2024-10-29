#include <fstream>
#include <ios>
#include <iostream>
#include <string>

#include <algorithm>
#include <array>
#include <memory>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Node {
  Node(size_t a, std::string n) : addr(a), name(std::move(n)) {}

  size_t addr;
  std::string name;
  std::unordered_map<Node *, uint64_t> children;
};

void fill_results(
    Node *cur, uint64_t parent_use_count,
    std::unordered_map<std::string, uint64_t> &result,
    const std::array<std::pair<std::string, uint64_t>, 5> &length_to_string,
    std::unordered_set<Node *> &visited, uint8_t intersection_ttl) {
  auto visited_it = visited.find(cur);
  if (visited_it != visited.end()) {
    if (!intersection_ttl) {
      fill_results(cur, parent_use_count, result, length_to_string, visited, 4);
    }
  } else {
    visited.insert(cur);
  }

  const std::string &name = cur->name;

  auto copy = length_to_string;

  for (uint8_t i = 4; i != 0; --i) {
    auto cur_pair = copy[i - 1];
    if (cur_pair.first.empty()) {
      continue;
    }

    auto curs = std::move(cur_pair);
    curs.first += "->" + name;
    curs.second = std::min(curs.second, parent_use_count);
    copy[i] = std::move(curs);
  }

  // 0,1,2,3,4 -> 4,1,2,3,4
  for (size_t i = 5 - (((intersection_ttl - 1) & 3) + 1); i < 5; ++i) {
    const auto &current = copy[i];
    if (current.first.empty()) {
      continue;
    }

    auto it = result.find(current.first);

    if (it == result.end()) {
      it = result.insert(std::pair<std::string, uint64_t>{current.first, 0})
               .first;
    }

    std::cout << "Found entry with current stats: " << it->first << ", "
              << it->second << '\n';
    std::cout << "Adding " << current.second << " to count. Resulting in "
              << it->second + current.second << '\n';
    it->second += current.second;
  }

  copy[0] = {name, -1};

  if (intersection_ttl) {
    --intersection_ttl;

    if (!intersection_ttl) {
      return;
    }
  }

  for (auto [ch, num_uses] : cur->children) {
    fill_results(ch, num_uses, result, copy, visited, intersection_ttl);
  }
}

void find_roots(Node *cur, std::unordered_set<Node *> &roots,
                std::unordered_set<Node *> &visited) {
  if (visited.find(cur) != visited.end()) {
    auto roots_it = roots.find(cur);
    if (roots_it != roots.end()) {
      roots.erase(roots_it);
    }

    return;
  }
  visited.insert(cur);

  for (const auto &[ch, _] : cur->children) {
    find_roots(ch, roots, visited);
  }
}

int main() {
  std::ifstream input("trace.log", std::ios_base::in);

  std::unordered_map<size_t, std::unique_ptr<Node>> nodes_map;

  std::string cur_line;
  while (std::getline(input, cur_line)) {
    size_t first_space_pos = cur_line.find(' ');
    std::string op_addr_str = cur_line.substr(1, first_space_pos - 2);

    size_t second_space_pos = cur_line.find(' ', first_space_pos + 1);
    std::string us_addr_str = cur_line.substr(
        second_space_pos + 2, cur_line.size() - second_space_pos - 3);

    size_t left_angle_pos = cur_line.find('<', first_space_pos + 1);
    std::string operand = cur_line.substr(first_space_pos + 1,
                                          left_angle_pos - first_space_pos - 1);

    std::string user = cur_line.substr(left_angle_pos + 2,
                                       second_space_pos - left_angle_pos - 2);

    size_t us_addr = static_cast<size_t>(std::stoull(us_addr_str));
    size_t op_addr = static_cast<size_t>(std::stoull(op_addr_str));

    auto user_it = nodes_map.find(us_addr);
    if (user_it == nodes_map.end()) {
      auto node = std::make_unique<Node>(us_addr, user);
      user_it = nodes_map
                    .insert(std::pair<size_t, std::unique_ptr<Node>>{
                        us_addr, std::move(node)})
                    .first;
    }

    auto op_it = nodes_map.find(op_addr);
    if (op_it == nodes_map.end()) {
      auto node = std::make_unique<Node>(op_addr, operand);
      op_it = nodes_map
                  .insert(std::pair<size_t, std::unique_ptr<Node>>{
                      op_addr, std::move(node)})
                  .first;
    }

    auto &children = user_it->second->children;
    auto children_it = children.find(op_it->second.get());
    if (children_it == children.end()) {
      children_it =
          children.insert(std::pair<Node *, uint64_t>{op_it->second.get(), 0})
              .first;
    }
    ++children_it->second;
  }

  std::unordered_set<Node *> roots;
  std::unordered_set<Node *> visited;
  for (const auto &[_, node] : nodes_map) {
    roots.insert(node.get());
    find_roots(node.get(), roots, visited);
  }

  auto roots_str = std::accumulate(roots.begin(), roots.end(), std::string(),
                                   [](std::string &acc, Node *cur) {
                                     auto addr_str = std::to_string(cur->addr);
                                     if (acc.empty()) {
                                       return addr_str;
                                     }
                                     acc += ", " + std::to_string(cur->addr);
                                     return acc;
                                   });

  std::cout << "Roots: " << roots_str << '\n';
  input.close();

  std::unordered_map<std::string, uint64_t> results;

  std::vector<std::unique_ptr<Node>> nodes;
  nodes.reserve(nodes_map.size());
  for (auto &node_pair : nodes_map) {
    nodes.push_back(std::move(node_pair.second));
  }

  visited.clear();
  for (auto root : roots) {
    fill_results(root, -1, results, {}, visited, 0);
  }

  std::vector<std::vector<std::pair<std::string, uint64_t>>> strings_by_length(
      4, std::vector<std::pair<std::string, uint64_t>>{});

  for (auto &entry : results) {
    size_t count = std::count(entry.first.begin(), entry.first.end(), '>');
    if (count == 0) {
      std::cout << "Invalid entry: " << entry.first << '\n';
      throw std::runtime_error("Unexpected zero count");
    }
    strings_by_length[count - 1].push_back(std::move(entry));
  }

  std::ofstream out("analysis.txt", std::ios_base::out);

  for (auto &strings : strings_by_length) {
    std::sort(strings.begin(), strings.end(),
              [](const auto &left, const auto &right) {
                return left.second > right.second;
              });
  }

  for (size_t i = 1; i <= 4; ++i) {
    out << "LENGTH: " << i + 1 << '\n';

    const auto &strings = strings_by_length[i - 1];
    for (const auto &entry : strings) {
      out << entry.first << ": " << entry.second << '\n';
    }

    out << '\n';
  }

  out.close();

  return 0;
}