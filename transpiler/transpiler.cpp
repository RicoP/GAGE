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


bool iswhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool isalpha(char c) {
    if(c >= 'a' && c <= 'z') return true;
    if(c >= 'A' && c <= 'Z') return true;
    return false;
}

const char * to_string(const char * source, string_part part) {
    static char buffer[1024] = "";

    char * p = buffer;
    while(part.length > 0) {
        *p = source[part.begin];
        part.begin++;
        part.length--;
        p++;
    }
    *p = 0;
    return buffer;
}

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

void next_line() {
    while(iswhitespace(peek())) {
        pi++;
    }
}

enum class States {
	ReadLine,
	ReadCommand,
	ReadParam,

    Error,
	END
};

States current_state = States::ReadLine;

string_part read_statement() {
    int begin = pi;
    int length = 0;

    while(!iswhitespace(read_char())) {
        length++;
    }

    return string_part { begin, length };
}

string_part read_parameter() {
    if(peek() != '\"') {
        return read_statement();
    }

    int begin = pi;
    int length = 1;

    read_char();

    for(;;) {
        char c = read_char();
        length++;
        if(c == '\"') break;
        if(c == '\\') {
            read_char();
            length++;
        }
    }

    return string_part { begin, length };
}

void parse_source() {
    for(;;) {
        switch(current_state) {
            case States::ReadLine: {
                char c = peek();

                if(c == 0) {
                    return;
                }

                if(isalpha(c)) {
                    current_state = States::ReadCommand;
                }
                else {
                    current_state = States::Error;
                }
            } break;
            case States::ReadCommand: {
                string_part command_name = read_statement();
                string_part param1 = read_parameter();
                string_part param2 = read_parameter();
                next_line();

                Command command = { command_name, param1, param2 };
                commands.push_back(command);

                current_state = States::ReadLine;
            } break;
            case States::ReadParam: {

            } break;

            default:
                puts("Unknown state!");
                exit(1);
        }
    }
}

void transpile_commands() {
    // ... implement me
    #if 0
    // Example output
    #include "gage.h"
    #include "chapter1.h"
    void Chapter1() {
        static int    sm_line = -1;
        static bool   sm_did_choice = false;
        static double sm_wait_time = 0;

        switch(sm_line)
        {
            default:
            case -1:

            SHOW("lisa", "happy");
            SAY("lisa", "Hello \"World");
            SCENE("bg desert");

            RETURN();
        }
    }
    #endif    
}

int main(int argn, char ** argv) {
	/*
	if(argn != 2) return -1;
	source = read_file_as_string(argv[1]);
	*/

	source =
		"show lisa happy\n"
		"say lisa \"Hello \\\"World\"\n"
		"scene bg desert\n";
	pi = 0;

    puts(source);

    parse_source();

    /*
    for(Command command : commands) {
        string_part command_name = command.name;
        string_part param1 = command.param1;
        string_part param2 = command.param2;
        std::printf("Command begin %d, length %d, pi %d, string %s \n", command_name.begin, command_name.length, pi, to_string(source, command_name));
        std::printf("Param1 begin %d, length %d, pi %d, string %s \n", param1.begin, param1.length, pi, to_string(source, param1));
        std::printf("Param2 begin %d, length %d, pi %d, string %s \n", param2.begin, param2.length, pi, to_string(source, param2));
    }
    */

    transpile_commands();

    return 0;
}
