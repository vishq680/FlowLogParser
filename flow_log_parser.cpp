#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

// Utility function to trim whitespace
string trim(const string &str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

// Load lookup table from CSV file
unordered_map<string, unordered_map<int, string>> loadLookupTable(const string &filename) {
    unordered_map<string, unordered_map<int, string>> lookupMap;
    ifstream file(filename);
    string line;

    // Skip the header line
    getline(file, line);

    if (!file.is_open()) {
        cerr << "Error: Could not open the file " << filename << endl;
        return {};
    }

    while (getline(file, line)) {
        stringstream ss(line);
        string dst_port_str, protocol, tag;

        getline(ss, dst_port_str, ',');
        getline(ss, protocol, ',');
        getline(ss, tag, ',');

        int dst_port = stoi(trim(dst_port_str));
        protocol = trim(protocol);
        tag = trim(tag);

        // Convert protocol to lowercase for case-insensitive matching
        transform(protocol.begin(), protocol.end(), protocol.begin(), ::tolower);

        lookupMap[protocol][dst_port] = tag;
    }

    return lookupMap;
}

// Parse a flow log entry from the log file
pair<int, string> parseFlowLogEntry(const string &logLine) {
    stringstream ss(logLine);
    string token;

    // Skip the first few fields (version, account-id, interface-id, srcaddr, dstaddr)
    for (int i = 0; i < 5; ++i) {
        ss >> token;
    }

    // Extract the destination port
    int dst_port;
    ss >> dst_port;

    // Skip the src_port
    ss >> token;

    // Extract the protocol (as a number)
    int protocol_number;
    ss >> protocol_number;

    // Convert protocol number to its string equivalent
    string protocol;
    if (protocol_number == 6) protocol = "tcp";
    else if (protocol_number == 17) protocol = "udp";
    else if (protocol_number == 1) protocol = "icmp";

    return {dst_port, protocol};
}

// Main function
int main() {
    // Load the lookup table
    auto lookupMap = loadLookupTable("lookup_table.csv");

    // Create maps for counting tags and port/protocol combinations
    unordered_map<string, int> tagCount;
    unordered_map<string, int> portProtocolCount;

    // Initialize "Untagged" category
    tagCount["Untagged"] = 0;

    // Process the flow logs
    ifstream logFile("flow_logs.txt");
    if (!logFile.is_open()) {
        cerr << "Error: Could not open the flow log file" << endl;
        return 1;
    }
    
    string logLine;

    while (getline(logFile, logLine)) {
        auto [dst_port, protocol] = parseFlowLogEntry(logLine);

        if (protocol.empty()) {
            continue;
        }

        string tag = "Untagged";
        if (lookupMap.count(protocol) && lookupMap[protocol].count(dst_port)) {
            tag = lookupMap[protocol][dst_port];
        }
        tagCount[tag]++;

        string portProtocolKey = to_string(dst_port) + "," + protocol;
        portProtocolCount[portProtocolKey]++;
    }

    // Output the results
    ofstream outputFile("output.txt");

    outputFile << "Tag Counts:\n";
    outputFile << "Tag,Count\n";
    for (const auto &entry : tagCount) {
        outputFile << entry.first << "," << entry.second << "\n";
    }

    outputFile << "\nPort/Protocol Combination Counts:\n";
    outputFile << "Port,Protocol,Count\n";
    for (const auto &entry : portProtocolCount) {
        outputFile << entry.first << "," << entry.second << "\n";
    }

    cout << "Processing complete. Output written to output.txt.\n";

    return 0;
}
