#include "dns_wildcard.h"
#include <stdexcept>
#include <iostream>
#include <regex>
#include <memory>

TrieNode::TrieNode() : isEndOfRecord(false) {}

DNSWildcard::DNSWildcard() {
    root = std::make_shared<TrieNode>();
}

void DNSWildcard::insert(std::shared_ptr<TrieNode> node, const std::vector<std::string>& parts, int index) {
    if (index == -1) {
        node->isEndOfRecord = true;
        return;
    }
    const std::string& part = parts[index];
    if (node->children.find(part) == node->children.end()) {
        node->children[part] = std::make_shared<TrieNode>();
    }
    insert(node->children[part], parts, index - 1);
}

bool DNSWildcard::search(const std::shared_ptr<TrieNode> node, const std::vector<std::string>& parts, int index) const {
    if (node->isEndOfRecord) {
        return true;
    }
    if (index == -1) {
        return false;
    }
    const std::string& part = parts[index];
    auto it = node->children.find(part);
    if (it != node->children.end() && search(it->second, parts, index - 1)) {
        return true;
    }
    it = node->children.find("*");
    if (it != node->children.end() && search(it->second, parts, index - 1)) {
        return true;
    }
    return false;
}

std::vector<std::string> DNSWildcard::split(const std::string& s, char delimiter) const {
    std::vector<std::string> parts;
    std::string part;
    for (char ch : s) {
        if (ch == delimiter) {
            if (!part.empty()) {
                parts.push_back(part);
                part.clear();
            }
        } else {
            part += ch;
        }
    }
    if (!part.empty()) {
        parts.push_back(part);
    }
    return parts;
}

void DNSWildcard::addRecord(const std::string& record) {
    if (record.empty()) {
        throw std::invalid_argument("Record cannot be empty");
    }
    // Check for consecutive dots, dots at the end, and a single wildcard
    if (record == "*" || record.find("..") != std::string::npos || record.back() == '.') {
        throw std::invalid_argument("Record must be a valid DNS name with no consecutive dots, no dots at the end, and not just a single wildcard");
    }
    std::regex validRecordPattern(R"(^([a-zA-Z0-9*]+(\.[a-zA-Z0-9*]+)*)$)");
    if (!std::regex_match(record, validRecordPattern)) {
        throw std::invalid_argument("Record must be a valid DNS name");
    }
    std::vector<std::string> parts = split(record, '.');
    if (parts.empty()) {
        throw std::invalid_argument("Record must contain at least one part");
    }
    insert(root, parts, parts.size() - 1);
}

bool DNSWildcard::isValid(const std::string& query) const {
    if (query.empty()) {
        throw std::invalid_argument("Query cannot be empty");
    }
    // Check for consecutive dots and dots at the end
    if (query.find("..") != std::string::npos || query.back() == '.') {
        throw std::invalid_argument("Query must be a valid DNS name with no consecutive dots or dots at the end");
    }
    std::regex validQueryPattern(R"(^([a-zA-Z0-9]+(\.[a-zA-Z0-9]+)*)$)");
    if (!std::regex_match(query, validQueryPattern)) {
        throw std::invalid_argument("Query must be a valid DNS name");
    }
    std::vector<std::string> parts = split(query, '.');
    if (parts.empty()) {
        throw std::invalid_argument("Query must contain at least one part");
    }
    return search(root, parts, parts.size() - 1);
}
