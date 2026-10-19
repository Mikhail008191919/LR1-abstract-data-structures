#include "ds.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
using namespace std;

map<string, IntArray> arrays;
map<string, SinglyList> flists;
map<string, DoublyList> llists;
map<string, StackDS> stacks;
map<string, QueueDS> queues;
map<string, Tree> trees;

string normalizeCommand(const string& line) {
    istringstream input(line);
    string cmd;

    if (!(input >> cmd))
        return line;

    string args;
    getline(input, args);

    // Короткие команды с именами структур по умолчанию.
    static const map<string, string> aliases = {
        // Массив: ap=добавить в конец, ai=вставить, ag=получить,
        // as=заменить, ad=удалить, al=длина, av=вывод
        {"ap", "MPUSH myarray"}, {"ai", "MINSERT myarray"},
        {"ag", "MGET myarray"}, {"as", "MSET myarray"},
        {"ad", "MDEL myarray"}, {"al", "MLEN myarray"},
        {"av", "MPRINT myarray"},

        // Односвязный список
        {"fp", "FPUSH mylist"}, {"fh", "FPUSHFRONT mylist"},
        {"fb", "FBEFORE mylist"}, {"fa", "FAFTER mylist"},
        {"fd", "FDEL mylist"}, {"ff", "FPOPFRONT mylist"},
        {"ft", "FPOPBACK mylist"}, {"fx", "FBEFOREDEL mylist"},
        {"fy", "FAFTERDEL mylist"}, {"fg", "FGET mylist"},
        {"fs", "FSEARCH mylist"}, {"fv", "FPRINT mylist"},

        // Двусвязный список
        {"lp", "LPUSH mylist"}, {"lh", "LPUSHFRONT mylist"},
        {"lb", "LBEFORE mylist"}, {"la", "LAFTER mylist"},
        {"ld", "LDEL mylist"}, {"lf", "LPOPFRONT mylist"},
        {"lt", "LPOPBACK mylist"}, {"lx", "LBEFOREDEL mylist"},
        {"ly", "LAFTERDEL mylist"}, {"lg", "LGET mylist"},
        {"ls", "LSEARCH mylist"}, {"lv", "LPRINT mylist"},

        // Стек и очередь
        {"sp", "SPUSH mystack"}, {"so", "SPOP mystack"},
        {"sv", "SPRINT mystack"},
        {"qp", "QPUSH myqueue"}, {"qo", "QPOP myqueue"},
        {"qv", "QPRINT myqueue"},

        // Complete Binary Tree, вариант 3
        {"ti", "TINSERT mytree"}, {"ts", "ISMEMBER mytree"},
        {"tc", "ISCOMPLETE mytree"}, {"tv", "TPRINT mytree"}
    };

    auto it = aliases.find(cmd);
    if (it == aliases.end())
        return line;

    return it->second + args;
}

bool runCommand(const string& line, bool silent, bool& changed) {
    changed = false;

    istringstream in(normalizeCommand(line));
    string cmd, name;
    int value, index, target;

    if (!(in >> cmd))
        return true;

    auto say = [&](const string& s) {
        if (!silent) cout << s << '\n';
    };

    auto status = [&](bool ok) {
        say(ok ? "-> OK" : "-> NOT_FOUND");
    };

    // ARRAY
    if (cmd == "MPUSH") {
        if (!(in >> name >> value)) return false;
        arrays[name].pushBack(value);
        changed = true;
        say("-> " + to_string(value));
    }
    else if (cmd == "MINSERT") {
        if (!(in >> name >> index >> value)) return false;
        changed = arrays[name].insert(index, value);
        status(changed);
    }
    else if (cmd == "MGET") {
        if (!(in >> name >> index)) return false;
        int result;
        if (arrays[name].get(index, result))
            say("-> " + to_string(result));
        else
            say("-> NOT_FOUND");
    }
    else if (cmd == "MSET") {
        if (!(in >> name >> index >> value)) return false;
        changed = arrays[name].set(index, value);
        status(changed);
    }
    else if (cmd == "MDEL") {
        if (!(in >> name >> index)) return false;
        changed = arrays[name].remove(index);
        status(changed);
    }
    else if (cmd == "MLEN") {
        if (!(in >> name)) return false;
        say("-> " + to_string(arrays[name].length()));
    }
    else if (cmd == "MPRINT") {
        if (!(in >> name)) return false;
        arrays[name].print();
    }

    // SINGLY LINKED LIST
    else if (cmd == "FPUSH" || cmd == "FPUSHBACK") {
        if (!(in >> name >> value)) return false;
        flists[name].pushBack(value);
        changed = true;
        say("-> " + to_string(value));
    }
    else if (cmd == "FPUSHFRONT") {
        if (!(in >> name >> value)) return false;
        flists[name].pushFront(value);
        changed = true;
        say("-> " + to_string(value));
    }
    else if (cmd == "FBEFORE" || cmd == "FAFTER") {
        if (!(in >> name >> target >> value)) return false;
        if (cmd == "FBEFORE")
            changed = flists[name].insertBefore(target, value);
        else
            changed = flists[name].insertAfter(target, value);
        status(changed);
    }
    else if (cmd == "FDEL") {
        if (!(in >> name >> value)) return false;
        changed = flists[name].removeValue(value);
        status(changed);
    }
    else if (cmd == "FPOPFRONT" || cmd == "FPOPBACK") {
        if (!(in >> name)) return false;
        auto& list = flists[name];
        int result;
        int idx = cmd == "FPOPFRONT" ? 0 : list.length() - 1;
        changed = list.get(idx, result) &&
            (cmd == "FPOPFRONT" ? list.popFront() : list.popBack());
        say(changed ? "-> " + to_string(result) : "-> EMPTY");
    }
    else if (cmd == "FBEFOREDEL" || cmd == "FAFTERDEL") {
        if (!(in >> name >> target)) return false;
        changed = cmd == "FBEFOREDEL"
            ? flists[name].eraseBefore(target)
            : flists[name].eraseAfter(target);
        status(changed);
    }
    else if (cmd == "FGET") {
        if (!(in >> name >> index)) return false;
        int result;
        if (flists[name].get(index, result))
            say("-> " + to_string(result));
        else
            say("-> NOT_FOUND");
    }
    else if (cmd == "FSEARCH") {
        if (!(in >> name >> value)) return false;
        say(flists[name].contains(value) ? "-> TRUE" : "-> FALSE");
    }
    else if (cmd == "FLEN") {
        if (!(in >> name)) return false;
        say("-> " + to_string(flists[name].length()));
    }
    else if (cmd == "FPRINT") {
        if (!(in >> name)) return false;
        flists[name].print();
    }

    // DOUBLY LINKED LIST
    else if (cmd == "LPUSH" || cmd == "LPUSHBACK") {
        if (!(in >> name >> value)) return false;
        llists[name].pushBack(value);
        changed = true;
        say("-> " + to_string(value));
    }
    else if (cmd == "LPUSHFRONT") {
        if (!(in >> name >> value)) return false;
        llists[name].pushFront(value);
        changed = true;
        say("-> " + to_string(value));
    }
    else if (cmd == "LBEFORE" || cmd == "LAFTER") {
        if (!(in >> name >> target >> value)) return false;
        changed = cmd == "LBEFORE"
            ? llists[name].insertBefore(target, value)
            : llists[name].insertAfter(target, value);
        status(changed);
    }
    else if (cmd == "LDEL") {
        if (!(in >> name >> value)) return false;
        changed = llists[name].removeValue(value);
        status(changed);
    }
    else if (cmd == "LPOPFRONT" || cmd == "LPOPBACK") {
        if (!(in >> name)) return false;
        auto& list = llists[name];
        int result;
        int idx = cmd == "LPOPFRONT" ? 0 : list.length() - 1;
        changed = list.get(idx, result) &&
            (cmd == "LPOPFRONT" ? list.popFront() : list.popBack());
        say(changed ? "-> " + to_string(result) : "-> EMPTY");
    }
    else if (cmd == "LBEFOREDEL" || cmd == "LAFTERDEL") {
        if (!(in >> name >> target)) return false;
        changed = cmd == "LBEFOREDEL"
            ? llists[name].eraseBefore(target)
            : llists[name].eraseAfter(target);
        status(changed);
    }
    else if (cmd == "LGET") {
        if (!(in >> name >> index)) return false;
        int result;
        if (llists[name].get(index, result))
            say("-> " + to_string(result));
        else
            say("-> NOT_FOUND");
    }
    else if (cmd == "LSEARCH") {
        if (!(in >> name >> value)) return false;
        say(llists[name].contains(value) ? "-> TRUE" : "-> FALSE");
    }
    else if (cmd == "LLEN") {
        if (!(in >> name)) return false;
        say("-> " + to_string(llists[name].length()));
    }
    else if (cmd == "LPRINT") {
        if (!(in >> name)) return false;
        llists[name].print();
    }

    // STACK
    else if (cmd == "SPUSH") {
        if (!(in >> name >> value)) return false;
        stacks[name].push(value);
        changed = true;
        say("-> " + to_string(value));
    }
    else if (cmd == "SPOP") {
        if (!(in >> name)) return false;
        int result;
        changed = stacks[name].pop(result);
        say(changed ? "-> " + to_string(result) : "-> EMPTY");
    }
    else if (cmd == "SPRINT") {
        if (!(in >> name)) return false;
        stacks[name].print();
    }

    // QUEUE
    else if (cmd == "QPUSH") {
        if (!(in >> name >> value)) return false;
        queues[name].push(value);
        changed = true;
        say("-> " + to_string(value));
    }
    else if (cmd == "QPOP") {
        if (!(in >> name)) return false;
        int result;
        changed = queues[name].pop(result);
        say(changed ? "-> " + to_string(result) : "-> EMPTY");
    }
    else if (cmd == "QPRINT") {
        if (!(in >> name)) return false;
        queues[name].print();
    }

    // COMPLETE BINARY TREE (VARIANT 3)
    else if (cmd == "TINSERT") {
        if (!(in >> name >> value)) return false;
        trees[name].insert(value);
        changed = true;
        say("-> " + to_string(value));
    }
    else if (cmd == "TGET") {
        if (!(in >> name)) return false;
        if (in >> value)
            say(trees[name].search(value) ? "-> TRUE" : "-> FALSE");
        else
            trees[name].print();
    }
    else if (cmd == "ISMEMBER") {
        if (!(in >> name >> value)) return false;
        say(trees[name].search(value) ? "-> TRUE" : "-> FALSE");
    }
    else if (cmd == "ISCOMPLETE") {
        if (!(in >> name)) return false;
        say(trees[name].isComplete() ? "-> TRUE" : "-> FALSE");
    }
    else if (cmd == "TPRINT") {
        if (!(in >> name)) return false;
        trees[name].print();
    }

    // PRINT ANY STRUCTURE
    else if (cmd == "PRINT") {
        string type;
        if (!(in >> type >> name)) return false;

        if (type == "M") arrays[name].print();
        else if (type == "F") flists[name].print();
        else if (type == "L") llists[name].print();
        else if (type == "S") stacks[name].print();
        else if (type == "Q") queues[name].print();
        else if (type == "T") trees[name].print();
        else {
            say("ERROR: use M, F, L, S, Q or T");
            return false;
        }
    }
    else {
        say("ERROR: unknown command " + cmd);
        return false;
    }

    return true;
}

int main(int argc, char* argv[]) {
    string fileName = "file.data";
    string query;

    // Optional command-line arguments
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];

        if (arg == "--file" && i + 1 < argc)
            fileName = argv[++i];
        else if (arg == "--query" && i + 1 < argc)
            query = argv[++i];
        else if (arg == "--help") {
            cout << "Usage: ./main --file file.data --query 'COMMAND'\n";
            return 0;
        }
        else {
            cerr << "Unknown argument: " << arg << '\n';
            return 1;
        }
    }

    // Restore structures by replaying saved commands
    ifstream input(fileName);
    string savedCommand;

    while (getline(input, savedCommand)) {
        bool ignored;
        runCommand(savedCommand, true, ignored);
    }
    input.close();

    // Execute a command and save it if it changes data
    auto execute = [&](const string& command) {
        bool changed = false;

        if (!runCommand(command, false, changed))
            return;

        if (changed) {
            ofstream output(fileName, ios::app);
            if (!output) {
                cerr << "Cannot write to " << fileName << '\n';
                return;
            }
            output << command << '\n';
        }
    };

    // Single-command mode, matching the lab manual example
    if (!query.empty()) {
        execute(query);
        return 0;
    }

    // Interactive mode
    cout << "LR1 - Abstract Data Structures\n";
    cout << "Enter 'help' for examples, 'exit' to quit.\n";

    string line;
    while (true) {
        cout << "LR1> ";
        if (!getline(cin, line))
            break;

        if (line == "exit")
            break;

        if (line == "help") {
            cout << "Array: ap VALUE | ai INDEX VALUE | ag INDEX | as INDEX VALUE | ad INDEX | al | av\n";
            cout << "Singly list: fp VALUE | fh VALUE | fb TARGET VALUE | fa TARGET VALUE | fd VALUE | ff | ft | fg INDEX | fs VALUE | fv\n";
            cout << "Doubly list: lp VALUE | lh VALUE | lb TARGET VALUE | la TARGET VALUE | ld VALUE | lf | lt | lg INDEX | ls VALUE | lv\n";
            cout << "Stack: sp VALUE | so | sv\n";
            cout << "Queue: qp VALUE | qo | qv\n";
            cout << "Tree: ti VALUE | ts VALUE | tc | tv\n";
            cout << "Use exit to quit.\n";
            continue;
        }

        if (!line.empty())
            execute(line);
    }

    return 0;
}
