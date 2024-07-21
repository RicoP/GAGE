#include <vector>

struct string_part {
	int begin = 0;
	int length = 0;
};

static const string_part string_part_empty = { 0, 0 };

struct Command {
	string_part name;
	std::vector<string_part> params;
};

std::vector<Command> commands;

// read the entire text file including one 
// zero character and return the memory address.
char * read_file_as_string(const char * file_path);

void error(const char * message) {
    fprintf(stderr, "ERROR: %s", message);
    exit(1);
}

bool iswhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\r';
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

const char * source = nullptr;
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

bool is_end_of_line() {
    while(iswhitespace(peek())) pi++;
    return peek() == '\n' || peek() == 0;
}

void next_line() {
    bool eol = is_end_of_line();
    if(!eol) error("expected end of line");
    
    if(peek() == 0) return; //don't shoot over the end of the string
    pi++;
    if(peek() == 0) return; //don't shoot over the end of the string

    // skip empty lines
    while(is_end_of_line()) pi++;
}

void skip_ws_till_line_end() {
    for(;;) {
        char c = peek();
        if(c != ' ' && c != '\t' && c != '\r' && c != 0) break;
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
    if(is_end_of_line()) {
        return string_part_empty;
    }

    int begin = pi;
    int length = 0;

    for(;;) {
        if(peek() == '\n' || peek() == 0) break;
        char c = read_char();
        if(iswhitespace(c)) break;
        length++;
    }

    return string_part { begin, length };
}

string_part read_parameter() {
    if(is_end_of_line()) {
        return string_part_empty;
    }

    if(peek() != '\"') {
        return read_statement();
    }

    skip_ws_till_line_end();

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
                std::vector<string_part> params;
                for(;;) {
                    string_part param = read_parameter();
                    if(param.length) params.push_back(param);
                    else             break;
                }
                next_line();

                Command command = { command_name, std::move(params) };
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

// HA 0: implementieren
//       Funktion sollte auf stddio den folgenden code generieren.
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

    // HA 1: folgende source
    const char * source1 = 
		/*kein return am schluss */ "show lisa happy";

    // HA 2: folgende source
    const char * source2 = 
		/* Leerzeilen */
        "show lisa happy\n"
		"\n"
		"scene bg desert\n";
    
    // HA 3: mehr whitespace zeichen statements
    const char * source3 = 
        "show    lisa    happy   \n";


    // HA 4: mehr oder weniger parameter
    const char * source4 = 
        "music \"joy\"\n"
        "wait 2.0\n"
        "return\n";

    puts(source);

    parse_source();

    for(Command command : commands) {
        string_part command_name = command.name;
        std::printf("Command begin %d, length %d, string %s \n", command_name.begin, command_name.length, to_string(source, command_name));
        for(int i = 0; i !=  command.params.size(); ++i) {
            string_part param = command.params[i];
            std::printf("Param%d begin %d, length %d, string %s \n", i, param.begin, param.length, to_string(source, param));
        }
    }
    /*
    */

    transpile_commands();

    return 0;
}
