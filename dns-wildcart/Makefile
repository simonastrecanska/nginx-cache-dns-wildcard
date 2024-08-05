CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -I.

all: test_dns_wildcard

dns_wildcard.o: dns_wildcard.cpp dns_wildcard.h
	$(CXX) $(CXXFLAGS) -c dns_wildcard.cpp

test_dns_wildcard.o: test_dns_wildcard.cpp dns_wildcard.h
	$(CXX) $(CXXFLAGS) -c test_dns_wildcard.cpp

test_dns_wildcard: dns_wildcard.o test_dns_wildcard.o
	$(CXX) $(CXXFLAGS) -o test_dns_wildcard dns_wildcard.o test_dns_wildcard.o

clean:
	rm -f *.o test_dns_wildcard

.PHONY: all clean
