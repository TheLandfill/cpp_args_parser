#include <cstring>
#include "hash_table.h"

Hash_Table_Registration::Hash_Table_Registration() : key(""), value(nullptr) {};

static size_t find_next_prime(const size_t initial_guess) {
	size_t next_prime = initial_guess;
	next_prime += 1 - (next_prime % 2);
	bool is_prime = false;
	std::vector<size_t> list_of_primes = { 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37 };
	while (!is_prime) {
		is_prime = true;
		for (size_t i = 0; i < list_of_primes.size(); i++) {
			size_t remainder = next_prime % list_of_primes[i];
			is_prime = is_prime && (remainder != 0);
			next_prime += 2 * (remainder == 0);
		}
	}
	return next_prime;
}

Hash_Table::Hash_Table(size_t initial_size) : registry_list(std::vector<Hash_Table_Registration>(find_next_prime(2 * initial_size), Hash_Table_Registration())) {}

size_t Hash_Table::hash_element(const char * str) const {
	if (strcmp(str, "") == 0) {
		return (size_t)-1;
	}
	size_t location = 7;
	int cur_char = 0;
	while (str[cur_char] != '\0') {
		location += str[cur_char];
		location *= hash_multiple;
		location %= registry_list.size();
		cur_char++;
	}
	while (strcmp(registry_list[location].key, "") != 0 && strcmp(registry_list[location].key, str) != 0) {
		location++;
		location %= registry_list.size();
	}
	return location;
}

void Hash_Table::insert(const char * str, Command_Line_Var_Interface * clvi) {
	size_t location = hash_element(str);
	registry_list[location].key = str;
	registry_list[location].value = clvi;
}

Command_Line_Var_Interface * Hash_Table::operator[](const char * str) const {
	return registry_list[hash_element(str)].value;
}

int Hash_Table::count(const char * str) const {
	return strcmp(registry_list[hash_element(str)].key, "");
}