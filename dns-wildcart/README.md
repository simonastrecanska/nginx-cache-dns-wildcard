# DNS Wildcard

This project implements a DNS Wildcard Matcher using a trie structure. It allows adding DNS wildcard records and validating if a given DNS query matches any of the stored wildcard records.

## Installation

1. Clone the repository if you haven't already:
```sh
git clone https://github.com/simonastrecanska/nginx-cache-dns-wildcard.git
```

2. Navigate to the project directory:
```sh
cd nginx-cache-dns-wildcard
cd dns-wildcart
```

3. Build the project:
```sh
make
```

## Usage

After building the project, you can run the test executable to validate the implementation and see the matcher in action.

### Running the Tests

```sh
./test_dns_wildcard
```

## Project Structure

- `dns_wildcard.h`: Header file containing the class declarations.
- `dns_wildcard.cpp`: Implementation of the DNS wildcard matcher.
- `test_dns_wildcard.cpp`: Test cases to validate the functionality of the DNS wildcard matcher.
