#include <vector>

struct string_part {
	int begin = 0;
	int length = 0;
};

struct Command {
	string_part name;
	string_part param1;
	string_part param2;
};

std::vector<Command> commands;

// read the entire text file including one 
// zero character and return the memory address.
char * read_file_as_string(const char * file_path);

char * source = nullptr;
int pi = 0;

char peek() {
	return source[pi];
}

char read_char() {
	char c = peek();
	if(c != 0) {
		++pi;
	}
    return c;
}

enum class States {
	ReadLine,
	ReadCommand,
	ReadParam,

    Error,
	END
};

States current_state = States::ReadLine;

bool iswhitespace(char c) {
    return c == ' ' || c == '\t';
}

bool isalpha(char c) {
    if(c >= 'a' && c <= 'z') return true;
    if(c >= 'A' && c <= 'Z') return true;
    return false;
}

string_part read_statement() {
    int begin = pi;
    int length = 0;

    while(!iswhitespace(read_char())) {
        length++;
    }

    return string_part { begin, length };
}

int main(int argn, char ** argv) {
	/*
	if(argn != 2) return -1;
	source = read_file_as_string(argv[1]);
	*/

	source =
		"show lisa happy\n"
		"say lisa \"Hello\"\n"
		"scene bg desert\n";
	pi = 0;

    puts(source);

    for(;;) {
        switch(current_state) {
            case States::ReadLine: {
                puts("ReadLine");

                char c = peek();
                if(isalpha(c)) {
                    current_state = States::ReadCommand;
                }
                else {
                    current_state = States::Error;
                }
            } break;
            case States::ReadCommand: {
                puts("ReadCommand");

                string_part command_name = read_statement();
                std::printf("Command begin %d, length %d, pi %d \n", command_name.begin, command_name.length, pi);

                //TODO:
                return 0;
                string_part param1;
                string_part param2;
                while(peek() != '\n') {
                    // ...
                }
            } break;
            case States::ReadParam: {

            } break;

            default:
                puts("Unknown state!");
                exit(1);
        }
    }

    return 0;
}
