#include<iostream>
#include <fstream>
#include<string>
#include<vector>

class collumn {
	std::string table_name;
	std::string name;
	std::string type;
	int size;
	std::string default_value;
public:
	void set_table(std::string table_name) {
		this->table_name = table_name;
	}
	void set_name(std::string name) {
		this->name = name;
	}
	void set_type(std::string type) {
		if (type != "text" && type != "integer" && type != "float")
			throw "Wrong Type";
		else
			this->type = type;
	}
	void set_size(int size) {
		this->size = size;
	}
	void set_default_value(std::string def_value) {
		this->default_value = def_value;
	}
	void set_collumn(std::string table_name, std::string name, std::string type, int size, std::string def_value) {
		this->set_table(table_name);
		this->set_name(name);
		this->set_type(type);
		this->set_size(size);
		this->set_default_value(def_value);
	}
};

class table {
	std::string name;
	std::vector<collumn*> collumns;
public:
	void set_name(std::string name) {
		this->name = name;
	}
	void add_collumn(collumn* col) {
		if (col != nullptr)
			collumns.pushback(col);
	}
};

class database {
	std::vector<table*> tables;
	std::vector<collum*> collumns;
public:
	void add_table(table* tab) {
		int o = 0;
		for (table* table_cur : tables) {
			if (table_cur->name == tab->name)
				o = 1;
		}
		if (o == 0) {
			tables.pushback(tab);
		}
		else {
			throw Table with this name already exists;
		}
	}
};


std::vector<std::string> split_up_input(std::string input) {
	std::string word;
	std::vector<std::string> words_v;
	for (char c : input) {
		if (c == ' ' || c == ',' || c == '(') {
			if (!word.empty()) {
				words_v.push_back(word);
				word.clear();
			}
		}
		else {
			if (c != ')')
				word += c;
		}
	}
	if (!word.empty())
		words_v.push_back(word);
	return words_v;
}

void process_create_table(std::vector<std::string> input) {
	std::cout << std::endl << "This command creates the table: " << input[2] << " with the collumns: ";
	int n = 3;
	int no = 1;
	while (n < input.size()) {
		std::string col_name = input[n];
		std::string col_type = input[n + 1];
		std::string col_size = input[n + 2];
		std::string col_def_val = input[n + 3];
		if (col_type != "text" && col_type != "integer" && col_type != "float") {
			std::cout << std::endl << "Error: wrong input type for collumn " << no;
		}
		else {
			std::cout << std::endl << "Collumn number " << no << " with the name " << col_name << ", type " <<
				col_type << ", size " << col_size << " and default value ";
			for (char c : col_def_val) {
				if (c != '"')
					std::cout << c;
			}
		}
		n += 4;
		no++;
	}
}
void process_create_index(std::vector<std::string> input) {
	std::cout << std::endl << "This command creates an index: " << input[2] << " on the table: " << input[4] << ", collumn: "
		<< input[5];
}

void process_drop_table(std::vector<std::string> input) {
	std::cout << std::endl << "This command deletes the table: " << input[2];
}

void process_drop_index(std::vector<std::string> input) {
	std::cout << std::endl << "This command deletes the index: " << input[2];
}

void process_display_table(std::vector<std::string> input) {
	std::cout << std::endl << "This command displays the table: " << input[2];
}
int main() {
	std::ifstream file("Text.txt");
	if (!file) {
		std::cout << "could not read file";
		return 1;
	}
	std::string line;
	std::vector<std::string> words;
	int i = 1;
	while (std::getline(file, line)) {
		words.clear();
		words = split_up_input(line);
		std::cout << "the words I found for line " << i << " are: ";
		for (std::string word : words) {
			std::cout << word << "; ";
		}
		std::cout << std::endl;
		i++;
		if (words[0] == "CREATE") {
			if (words[1] == "TABLE")
				process_create_table(words);
			else
				if (words[1] == "INDEX")
					process_create_index(words);
		}
		else {
			if (words[0] == "DROP") {
				if (words[1] == "TABLE")
					process_drop_table(words);
				else
					if (words[1] == "INDEX")
						process_drop_index(words);
			}
			else
				if (words[0] == "DISPLAY" && words[1] == "TABLE")
					process_display_table(words);
				else
					std::cout << std::endl << "ERROR: This command does not follow the accepted syntax for a DB management command!!!";
		}
		std::cout << std::endl << "**********";
	}

	return 0;

}