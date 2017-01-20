//
//  tm.hpp
//  Turing Machine
//
//  Created by Asen Lekov on 12/11/16.
//  Copyright © 2016 fmi. All rights reserved.
//

#ifndef tm_hpp
#define tm_hpp

#include <vector>
#include <string>
#include <map>
#include <iostream>

using namespace std;

class Tape;
class Transition;

class Tape {
public:
    
    const static char EMPTY = ' ';
    const static char DELIMITER = '#';
    
    Tape(const string &);
    Tape(const Tape&);
    
    void move_left(int = -1);
    void move_right(int = -1);
    
    char read(int = -1) const;
    void write(char, int = -1);
    friend ostream& operator<<(ostream&, Tape&);
private:
    vector<Tape> virtual_tapes_;
    vector<char> left_;
    vector<char> right_;
    char current_ = '\0';
    
    void initialize(const string&);
};

class Transition {
private:
    string read_;
    string write_;
    string command_;
    string current_state_;
    string next_state_;
    
public:
    Transition(const string& current_state, const string&, const string&, const string&, const string& next_state);
    Transition(const Transition&);
    
    char get_command(int) const;
    string get_command() const;
    
    char get_write_symbol(int) const;
    string get_write_symbols() const;
    
    char get_read_symbol(int) const;
    string get_read_symbols() const;
    
    string get_next_state() const;
    string get_current_state() const;
    
    void change_next_state(const string&);
    
    friend ostream& operator<<(ostream&, Transition&);
};

class TuringMachine {
private:
    map<string, vector<unique_ptr<Transition>>> mapping_;
    vector<unique_ptr<Tape>> tapes_;
    string current_state_;
    
    Transition* find_transitions(const char &input);
public:
    TuringMachine();
    TuringMachine(const TuringMachine&);

    void add_tape(unique_ptr<Tape>);
    Tape* get_tape(int index);
    void remove_tape(int index);
    void start_state(const string&);
    
    vector<string> get_states();
    vector<unique_ptr<Transition>>& get_transitions(const string&);
    
    bool is_finished_successfuly() const;
   
    static TuringMachine load_machine(const string&);
    void loop_over(const string&, Transition*);
    void compose(TuringMachine);
    void to_single_tape();
    
    void add_transition(unique_ptr<Transition>);
    
    void step();
    void run();
    void print();
    
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
};
#endif /* tm_hpp */
