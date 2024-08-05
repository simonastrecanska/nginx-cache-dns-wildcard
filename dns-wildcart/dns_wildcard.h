#ifndef DNS_WILDCARD_H
#define DNS_WILDCARD_H

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

class TrieNode {
public:
    std::unordered_map<std::string, std::shared_ptr<TrieNode>> children;
    bool isEndOfRecord;

    TrieNode();
    ~TrieNode() = default;
};

class DNSWildcard {
private:
    std::shared_ptr<TrieNode> root;

    void insert(std::shared_ptr<TrieNode> node, const std::vector<std::string>& parts, int index);
    bool search(const std::shared_ptr<TrieNode> node, const std::vector<std::string>& parts, int index) const;
    std::vector<std::string> split(const std::string& s, char delimiter) const;

public:
    DNSWildcard();
    ~DNSWildcard() = default;

    void addRecord(const std::string& record);
    bool isValid(const std::string& query) const;
};

#endif // DNS_WILDCARD_H