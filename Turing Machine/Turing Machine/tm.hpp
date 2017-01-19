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
    
    Tape(const string &);
    
    void move_left();
    void move_right();
    
    char read() const;
    void write(char);
    friend ostream& operator<<(ostream&, Tape&);
private:
    vector<char> left_;
    vector<char> right_;
    char current_;
};

class Transition {
private:
    char read_;
    char write_;
    char command_;
    string current_state_;
    string next_state_;
public:
    Transition(const string& current_state, char, char, char, const string& next_state);
    
    char get_command() const;
    char get_write_symbol() const;
    char get_read_symbol() const;
    string get_next_state() const;
    string get_current_state() const;
    
    void change_next_state(const string&);
    
    friend ostream& operator<<(ostream&, Transition&);
};

class TuringMachine {
private:
    map<string, vector<unique_ptr<Transition>>> mapping_;
    vector<Tape> tapes_;
    string current_state_;
    
    Transition* find_transition(const char &input);
public:
    TuringMachine();

    void add_tape(Tape);
    void start_state(const string&);
    
    vector<string> get_states();
    
    bool is_finished_successfuly() const;
   
    static TuringMachine load_machine(const string&);
    void loop_over(const string&, Transition*);
    
    void add_transition(unique_ptr<Transition>);
    
    void step();
    void run();
    void print();
};
#endif /* tm_hpp */
