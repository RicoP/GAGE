#include <vector>
#include <assert.h>

struct string_part {
	int begin = 0;
	int length = 0;
};

static const string_part string_part_empty = { 0, 0 };

struct Block {
    std::vector<struct Statement> statements;
};

struct Command {
	string_part name;
	std::vector<string_part> params;
};

struct Branch {
    string_part head;
    Block block;
};

struct Statement {
enum class Type {
    none = 0,
    command,
    branch
};

private:
    Command activecommand;
    Branch activebranch;
    Type activetype = Type::none;

public:
    Statement(Command && _command) : activecommand(_command), activetype(Type::command) {}
    Statement(Branch && _branch)   : activebranch(_branch), activetype(Type::branch) {}

    Type type() const { return activetype; }

    const Command & command() const {
        assert(type() == Type::command);
        return activecommand;
    }

    const Branch & branch() const {
        assert(type() == Type::branch);
        return activebranch;
    }
};

struct Program {
    Block block;
};

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

bool islower(char c) {
    if(c >= 'a' && c <= 'z') return true;
    return false;
}

bool isupper(char c) {
    if(c >= 'A' && c <= 'Z') return true;
    return false;
}

bool isalpha(char c) {
    if(islower(c)) return true;
    if(isupper(c)) return true;
    return false;
}

void to_upper(char * str) {
    while(*str) {
        if(islower(*str)) {
            *str = 'A' + (*str - 'a');
        }
        ++str;
    }
}

char * to_string(const char * source, string_part part) {
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
int indentation = 0;

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

void skip_ws_till_line_end() {
    for(;;) {
        char c = peek();
        if(c != ' ' && c != '\t' && c != '\r') break;
        if(c == 0) break;
        pi++;
    }
}

void next_line() {
    skip_ws_till_line_end();
    int newlines = 0;
    while(peek() == '\n') {
        pi++;
        newlines++;
    }

    if(newlines == 0) {
        if(peek() != 0) {
            error("expected end of line");
        }
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

string_part read_statement_string(Statement::Type & type) {
    string_part sp = read_parameter();
    if(sp.length == 0) type = Statement::Type::none;
    
    const char * s = to_string(source, sp);
    if(s[0] == '\"') {
        type = Statement::Type::branch;
        //expect to be followed by a ':'
        skip_ws_till_line_end();
        char c = read_char();
        if(c != ':') {
            error("Expected ':' after condition.");
        }
    }
    else {
        type = Statement::Type::command;
    }
    return sp;
}

void parse_source(std::vector<struct Statement> & statements) {
    for(;;) {
        switch(current_state) {
            case States::ReadLine: {
                char c = peek();

                if(c == 0) {
                    return;
                }

                current_state = States::ReadCommand;
            } break;
            case States::ReadCommand: {
                //check indentation
                for(int i = 0; i != indentation; ++i) {
                    //one indentation equals four spaces
                    for(int s = 0; s != 4; ++s) {
                        if(peek() == ' ') {
                            read_char();
                            continue;
                        }
                        
                        if(s == 0) {
                            //no space found then we leave the current branch.
                            return;
                        }
                        error("bad indentation");
                    }
                }

                Statement::Type stype = Statement::Type::none;
                string_part statement_beginning = read_statement_string(stype);

                if(stype == Statement::Type::command) {
                    string_part command_name = statement_beginning;
                    std::vector<string_part> params;
                    for(;;) {
                        string_part param = read_parameter();
                        if(param.length) params.push_back(param);
                        else             break;
                    }
                    next_line();

                    Statement statement(Command { command_name, std::move(params) });
                    statements.push_back(std::move(statement));
                }
                else if(stype == Statement::Type::branch) {
                    next_line();
                    indentation++;
                    Branch branch;
                    branch.head = statement_beginning;
                    parse_source(branch.block.statements);
                    indentation--;
                    Statement statement(std::move(branch));
                    statements.push_back(std::move(statement));
                } else {
                    error("Unexpected state");
                }

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

void dump(const char * str) {
    fputs(str, stdout);
}

void transpile_block(int indent, const Block & block) {
    for(int si = 0; si != block.statements.size(); ++si) {
        const Statement & statement = block.statements[si];
        dump("        ");
        for(int i = 0; i < indent; ++i) dump("  ");

        if(statement.type() == Statement::Type::command) {
            const Command & command = statement.command();
            char * command_name = to_string(source, command.name);
            to_upper(command_name);
            dump(command_name);
            dump("(");
            for(int i = 0; i != command.params.size(); ++i) {
                if(i != 0) dump(", ");
                dump(to_string(source, command.params[i]));
            }
            dump(");\n");
        }
        else if(statement.type() == Statement::Type::branch) {
            bool isfirst = si == 0;
            if(!isfirst) isfirst = block.statements[si-1].type() != Statement::Type::branch;
            if(isfirst) {
                dump("        CHOICE_BEGIN(); \n");
            }

            if(!isfirst) {
                dump(" else ");
            }

            dump("if(CHOICE(");
            dump(to_string(source, statement.branch().head));
            dump(")) {\n");

            transpile_block(indent+1, statement.branch().block);

            dump("        }\n");

            bool islast = si == block.statements.size() - 1;
            if(!islast) islast = block.statements[si+1].type() != Statement::Type::branch;
            if(islast) {
                dump("        CHOICE_END(); \n");
            }
        }
    }
}

void transpile_program(const Program & program) {
    dump("#include \"gage.h\"                             \n");
    dump("#include \"chapter1.h\"                         \n");
    dump("void Chapter1() {                               \n");
    dump("    static int    sm_line = -1;                 \n");
    dump("    static bool   sm_did_choice = false;        \n");
    dump("    static double sm_wait_time = 0;             \n");
    dump("    switch(sm_line)                             \n");
    dump("    {                                           \n");
    dump("        default:                                \n");
    dump("        case -1:                                \n");

    transpile_block(0, program.block);
    
    dump("        RETURN();                               \n");
    dump("    }                                           \n");
    dump("}                                               \n");
}

//Aufgabe: ich rufe transpiler.exe so auf
//transpiler.exe meinscript.txt chapter1.cpp
//dann soll mein script in meinscript.txt transpiliert werden und der 
//transpilierte C++ code soll in chapter1.cpp geschrieben werden.

//argn = 3
//argv[0] = "./mein/pfad/transpiler.exe"
//argv[1] = "meinscript.txt"
//argv[2] = "chapter1.cpp"

char * read_file_as_string(const char * file_path) {
    FILE * f = fopen(file_path, "r");

    int filesize = fseek(f, 0, SEEK_END);

    char * buffer = new char[filesize + 1];

    fread(buffer, filesize, 1, f);

    buffer[filesize] = 0;

    fclose(f);

    return buffer;
}

int main(int argn, char ** argv) {
    /*
	if(argn != 3) return -1;
	source = read_file_as_string(argv[1]);

    puts(source);

    delete source;
    return 0;
    */

	source =
		"show eileen happy\n"
		"say eileen \"Hello \\\"World\"\n"
		"scene bg desert\n";
	pi = 0;

    // HA 1: folgende source
    const char * source1 = 
		/*kein return am schluss */ "show eileen happy";

    // HA 2: folgende source
    const char * source2 = 
		/* Leerzeilen */
        "show eileen happy\n"
		"\n"
		"scene bg desert\n";
    
    // HA 3: mehr whitespace zeichen statements
    const char * source3 = 
        "show    eileen    happy   \n";


    // HA 4: mehr oder weniger parameter
    const char * source4 = 
        "music \"joy\"\n"
        "wait 2.0\n"
        "return\n";

    // Extra: if statements
    const char * source5 = 
        "music \"joy\"\n"
        "wait 2.0\n"
        "show \"eileen\" \"happy\"\n"
		"say \"eileen\" \"How are you\"\n"
        "\"good\":\n"
		"    say \"eileen\" \"Great!\"\n"
        "\"bad\":\n"
		"    say \"eileen\" \"Shame.\"\n"
		"say \"eileen\" \"Anyway\"\n"
        "return\n";

    /*
    source = source1;
    source = source2;
    source = source3;
    source = source4;
    */
    source = source5;

    //puts(source);

    Program program;

    parse_source(program.block.statements);

    /*
    for(Statement & statement : program.block.statements) {
        if(statement.type() == Statement::Type::command) {
            const Command & command = statement.command();
            string_part command_name = command.name;
            std::printf("Command begin %d, length %d, string %s \n", command_name.begin, command_name.length, to_string(source, command_name));
            for(int i = 0; i !=  command.params.size(); ++i) {
                string_part param = command.params[i];
                std::printf("Param%d begin %d, length %d, string %s \n", i, param.begin, param.length, to_string(source, param));
            }
        }
        else if(statement.type() == Statement::Type::branch) {
            puts("TODO!");
        }
    }
    */

    transpile_program(program);

    return 0;
}