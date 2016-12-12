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
#include <iostream>

using namespace std;

class Tape;
class State;
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

class State {
private:
    string name_;
    vector<Transition*> transitions_;
public:
    State(const string &name);
    
    string get_name() const;
    void add_transition(Transition *trans);
    bool is_there_transition(const char &input);
    Transition* find_transition(const char &input);
    friend ostream& operator<<(ostream&, State&);
    friend bool operator== (const State &s, const State &another);
    friend bool operator!= (const State &s, const State &another);
};

class Transition {
private:
    char read_;
    char write_;
    char command_;
    State* state_;
public:
    Transition(char, char, char, State*);
    
    char get_command() const;
    char get_write_symbol() const;
    char get_read_symbol() const;
    State* get_next_state();
    friend ostream& operator<<(ostream&, Transition&);
};

class TuringMachine {
private:
    vector<unique_ptr<State>> states_;
    State* current_;
    Tape tape_;
public:
    TuringMachine(const string&, State*);
    
    void add_state(unique_ptr<State>);
    State* find_state(const string&);
    bool is_there_state(const string&) const;
    bool is_finished_successfuly() const;
    
    void step();
    void run();
    void print();
};
#endif /* tm_hpp */
