# Flow Log Parser - README

## Overview

This C++ program parses flow logs and tags each log entry based on a lookup table provided in a CSV file. The program outputs two reports:
1. **Tag Counts**: The number of matches for each tag based on the destination port and protocol.
2. **Port/Protocol Combination Counts**: The number of occurrences of each port/protocol combination.

The program supports AWS VPC flow log format (default format, version 2 only). It reads flow logs from a text file (`flow_logs.txt`) and tag mappings from a CSV file (`lookup_table.csv`).

## Features

- Supports version 2 of the AWS VPC default log format.
- Matches flow log entries to tags based on destination port and protocol from a lookup table.
- Generates reports with counts for each tag and port/protocol combination.

## Assumptions

1. **Log Format**: The program only supports the default log format for AWS VPC flow logs, version 2. Custom log formats are **not supported**.
2. **Log Version**: Only version 2 logs are supported. Other versions are ignored or may cause undefined behavior.
3. **Lookup Table**: The lookup table must be in a CSV format with three columns: `dstport`, `protocol`, and `tag`. The protocol values must be consistent with the log format (e.g., `tcp`, `udp`, `icmp`).
4. **Case Insensitivity**: The matching is case insensitive, meaning that `TCP` and `tcp` are treated the same.
5. **Supported Protocols**: The program supports the following protocols by converting the protocol number in the log to its string equivalent:
  - `6` → `tcp`
  - `17` → `udp`
  - `1` → `icmp`
6. **Unknown Protocols**: Log entries with unsupported or unknown protocols are ignored.

## Compilation and Execution Instructions

### Prerequisites

- C++11 or later
- A standard C++ compiler (e.g., g++, clang++)

### Compilation

To compile the program, navigate to the directory containing the source code and run the following command:

```bash
g++ -std=c++11 flow_log_parser.cpp -o flow_log_parser

```
This will generate an executable file named flow_log_parser.

### Running the Program
Once compiled, the program can be run from the command line. It requires two input files:

A lookup table in CSV format (e.g., lookup_table.csv).
A flow log file in text format (e.g., flow_logs.txt).
To run the program, use the following command:

```bash
./flow_log_parser
```

The program will read from the lookup_table.csv and flow_logs.txt files in the current directory. The output will be written to output.txt.


## Testing

The program was tested with a sample set of flow logs and lookup table entries. The following scenarios were tested:

1. Exact Matches: Flow logs where the destination port and protocol matched a lookup table entry, and the correct tag was assigned.
2. No Match: Flow logs where no match was found in the lookup table, resulting in the entry being categorized as "Untagged."
3. Case Insensitivity: Ensured that protocol matching was case-insensitive (e.g., TCP vs tcp).
4. Port/Protocol Combination Counting: Verified that the program correctly counted unique combinations of ports and protocols.


## Potential Enhancements

1. Custom Log Format Support: Extend the program to support different flow log formats and versions.
2. Extended Protocol Support: Add support for additional protocols beyond TCP, UDP, and ICMP.
3. Improved Error Handling: Provide better error messages for invalid input files or formats.
4. Performance Optimization: For large flow log files, optimize the parsing and lookup operations.