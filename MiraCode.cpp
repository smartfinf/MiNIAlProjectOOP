#include<regex>
#include<iostream>
#include<fstream>
#include<string>
//#include<vector>

class column {
	std::string table_name;  //informatia este duplicata ptr toate coloanele, propun stergere
	std::string name;
	std::string type;
	int size;
	std::string default_value;
public:
	//adding getters
	
	void set_table(std::string table_name) {
		this->table_name = table_name;
	}
	void set_name(std::string name) {
		this->name = name;
	}
	std::string get_name() {
		return this->name;
	}
	void set_type(std::string type) {
		if (type != "text" && type != "integer" && type != "float")
			throw "Wrong Type";
		else
			this->type = type;
	}
	std::string get_type() {
		return this->type;
	}
	void set_size(int size) {
		this->size = size;
	}
	int get_size() {
		return this->size;
	}

	void set_default_value(std::string def_value) {
		this->default_value = def_value;
	}
	std::string get_default_value()
	//daca in insert nu lipsesc coloane atunci nu vom folosi niciodata get_default_value
	{
		return this->default_value;
	}

	void set_column(std::string table_name, std::string name, std::string type, int size, std::string def_value) {
		this->set_table(table_name);
		this->set_name(name);
		this->set_type(type);
		this->set_size(size);
		this->set_default_value(def_value);
	}
	////daca definim un copy constructor da eroare la alocarea new column[x]
	//column(column &c)
	//{
	//	set_column(c.table_name,c.name, c.type,c.size, c.default_value);
	//}
};

class table {
	std::string name;
	column** columns;
	int no_col;
public:
	void set_name(std::string name) {
		this->name = name;
	}
	std::string get_name() {
		return this->name;
	}
	void add_column(column* col) {
		if (col != nullptr) {
			columns[no_col] = new column[sizeof(col)];
			columns[no_col] = col;
			no_col++;
		}
	}
};

class database {
	table** tables;
	int no_tab;
	//column** columns;  coloanele sunt doar in tabele
public:
	void add_table(table* tab) {
		int o = 0;
		for (int i = 0; i < no_tab; i++) {
			table* table_curr = tables[i];
			if (table_curr->get_name() == tab->get_name())
				o = 1;
		}
		if (o == 0) {
			tables[no_tab] = new table[sizeof(tab)];
			tables[no_tab] = tab;
			no_tab++;
		}
		else {
			throw "Table with this name already exists";
		}
	}
};


void split_up_input(std::string input, std::string** words_v, int& no_w) {
	std::string word;
	for (char c : input) {
		if (c == ' ' || c == ',' || c == '(') {
			if (!word.empty()) {
#ifdef OLD
				//words_v nu este alocat de ce scriu in words_v[x]
				words_v[no_w] = new std::string[word.size()];
#else
				//aloc un vector de n+1 stringuri
				std::string *t = new std::string[no_w+1];
				//copiez stringurile vechi
				for (int i = 0; i < no_w; i++)
					t[i] = (*words_v)[i];
				//dealoc spatiul vechi
				if( *words_v) delete[] *words_v;
				*words_v = t;
#endif
				(*words_v)[no_w] = word;
				no_w++;
				word.clear();
			}
		}
		else {
			if (c != ')')
				word += c;
		}
	}
	if (!word.empty()) {
		words_v[no_w] = new std::string[word.size()];
		*words_v[no_w] = word;
		no_w++;
	}
}

class methods {
public:
	static void process_create_table(std::string input[], int input_size) {
		std::cout << std::endl << "This command creates the table: " << input[2] << " with the columns: ";
		int n = 3;
		int no = 1;
		while (n < input_size) {
			std::string col_name = input[n];
			std::string col_type = input[n + 1];
			std::string col_size = input[n + 2];
			std::string col_def_val = input[n + 3];
			if (col_type != "text" && col_type != "integer" && col_type != "float") {
				std::cout << std::endl << "Error: wrong input type for column " << no;
			}
			else {
				std::cout << std::endl << "Column number " << no << " with the name " << col_name << ", type " <<
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
	static void process_create_index(std::string input[], int input_size) {
		std::cout << std::endl << "This command creates an index: " << input[2] << " on the table: " << input[4] << ", column: "
			<< input[5];
	}

	static void process_drop_table(std::string input[], int input_size) {
		std::cout << std::endl << "This command deletes the table: " << input[2];
	}

	static void process_drop_index(std::string input[], int input_size) {
		std::cout << std::endl << "This command deletes the index: " << input[2];
	}

	static void process_display_table(std::string input[], int input_size) {
		std::cout << std::endl << "This command displays the table: " << input[2];
	}

	static void process_management_command(std::string words[], int& size) {
		if (words[0] == "CREATE") {
			if (words[1] == "TABLE")
				process_create_table(words, size);
			else
				if (words[1] == "INDEX")
					process_create_index(words, size);
		}
		else {
			if (words[0] == "DROP") {
				if (words[1] == "TABLE")
					process_drop_table(words, size);
				else
					if (words[1] == "INDEX")
						process_drop_index(words, size);
			}
			else
				if (words[0] == "DISPLAY" && words[1] == "TABLE")
					process_display_table(words, size);
				else
					std::cout << std::endl << "ERROR: This command does not follow the accepted syntax for a DB management command!!!";
		}
	}
};
int main() {
	//std::ifstream file("Text.txt");
	//if (!file) {
	//	std::cout << "could not read file";
	//	return 1;
	//}
	std::string line;
	//std::vector<std::string> words;
	std::string* words=nullptr;
	int no_w = 0;
	const char* l[] = {
		"CREATE TABLE zuzu((nume,text,20,\"\"),(telefon,text,10,\"\"))",
		"CREATE INDEX dupatel ON zuzu(telefon)",
		"DISPLAY TABLE zuzu",
		"DROP TABLE zuzu",
		"LMAO lol\"\",\"\",\"\""
	};
	int i = 0;
	//while (std::getline(file, line)) {
	while (i < sizeof l / sizeof(const char*)) {
		//words.clear();
		line = l[i];
		if (no_w > 0)
		{
			delete words;
		}
		split_up_input(line,&words,no_w);
		std::cout << "the words I found for line " << i << " are: ";
		for (int j = 0; j < no_w; j++) {
			std::cout << words[j] << "; ";//exception thrown when no_w=11, j=10
		}
		std::cout << std::endl;
		i++;
		if (words[0] == "CREATE") {
			if (words[1] == "TABLE")
				methods::process_create_table(words, no_w);
			else
				if (words[1] == "INDEX")
					methods::process_create_index(words, no_w);
		}
		else {
			if (words[0] == "DROP") {
				if (words[1] == "TABLE")
					methods::process_drop_table(words, no_w);
				else
					if (words[1] == "INDEX")
						methods::process_drop_index(words, no_w);
			}
			else
				if (words[0] == "DISPLAY" && words[1] == "TABLE")
					methods::process_display_table(words, no_w);
				else
					std::cout << std::endl << "ERROR: This command does not follow the accepted syntax for a DB management command!!!";
		}
		//methods generic; //daca metodele sunt statice nu au nevoie de instantierea clasei
		methods::process_management_command( words, no_w);
		std::cout << std::endl << "**********";
	}

	return 0;

}