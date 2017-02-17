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

//
// Tape class
//
// Essential of Turing machines is that they are using single or multiple tapes.
// Tapes represent potencial endless buffer of symbols and head which can
// read/write symbols and navigate through tape.
//
class Tape {
public:
    
    const static char EMPTY = ' ';
    const static char DELIMITER = '#';
    
    Tape(const string &);
    Tape(const Tape&);
    
    //
    // Move the tape head to right
    //
    // If valid index is supplied to the method,
    // will move virtual tape to the right.
    // By default this method will be executed on the
    // main tape, the real one.
    //
    // Virtual tape is inner tape result of converting
    // Multiple tape machine to single tape.
    //
    void move_left(int = -1);
    
    //
    // Move the tape head to right
    //
    // If valid index is supplied to the method,
    // will move virtual tape to the right.
    // By default this method will be executed on the
    // main tape, the real one.
    //
    // Virtual tape is inner tape result of converting
    // Multiple tape machine to single tape.
    //
    void move_right(int = -1);
    
    //
    // Read the tape head
    //
    // If valid index is supplied to the method,
    // will read from virtual tape with given index.
    // By default this method will be executed on the
    // main tape, the real one.
    //
    // Virtual tape is inner tape result of converting
    // Multiple tape machine to single tape.
    //
    char read(int = -1) const;
    
    //
    // Write to the tape head
    //
    // If valid index is supplied to the method,
    // will read from virtual tape with given index.
    // By default this method will be executed on the
    // main tape, the real one.
    //
    // Virtual tape is inner tape result of converting
    // Multiple tape machine to single tape.
    //
    void write(char, int = -1);
    
    friend ostream& operator<<(ostream&, Tape&);
private:
    vector<Tape> virtual_tapes_;
    vector<char> left_;
    vector<char> right_;
    char current_ = '\0';
    
    //
    // Helper fuction which initialize tape
    // with given input string.
    //
    void initialize(const string&);
};


//
// Transition class
//
// The second main point in Turing machines besides Tape
// are the transitions between states. Every transistion has:
//
// 1.1 Read symbol
// 1.2 Current state
// 2.1 Write symbol
// 2.2 Next state
// 3. Command
//
// A transistion can be explaned with:
// "When read {read_} symbol on tape and current state of the machine is {current_state_}
// then write {write_} symbol to the tape and go to {next_state_} of the machine.
// Also move the tape left or right based on {command_}."
//
// Because the transition supports multiple tapes machines, we use strings to save the read, write and command symbols.

//
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
    
    //
    // Get command for tape
    //
    char get_command(int) const;
    
    //
    // Get commands
    //
    // This will return single command for single tape
    // machine transition, otherwise will return
    // commands for all tapes
    string get_command() const;
    
    //
    // Get read symbol for tape
    //
    char get_write_symbol(int) const;
    
    //
    // Get read symbols
    //
    // This will return single char for single tape
    // machine transition, otherwise will return
    // chars for all tapes
    string get_write_symbols() const;
    
    //
    // Get read symbol for tape
    //
    char get_read_symbol(int) const;
    
    //
    // Get read symbols
    //
    // This will return single char for single tape
    // machine transition, otherwise will return
    // chars for all tapes
    string get_read_symbols() const;
    
    //
    // Get next state of transition
    //
    string get_next_state() const;
    
    //
    // Get current state of transition
    //
    string get_current_state() const;
    
    //
    // Change next state of transition
    //
    void change_next_state(const string&);
    
    friend ostream& operator<<(ostream&, Transition&);
};

//
// Turing machine class
//
// Turing machines must be configured before use.
// Must-do things are to attach single or multiple tape;
// To define start state of the machine;
// and to load all transitions between states
//
class TuringMachine {
private:
    map<string, vector<unique_ptr<Transition>>> mapping_;
    vector<unique_ptr<Tape>> tapes_;
    string current_state_;
   
    //
    // Find transistions based on current state and input char from tape
    //
    Transition* find_transitions(const char &input);
public:
    TuringMachine();
    TuringMachine(const TuringMachine&);

    //
    // Add tape to machine
    //
    void add_tape(unique_ptr<Tape>);
    
    //
    // Get machine tape by index
    //
    Tape* get_tape(int index);
    
    //
    // Remove machine tape by index
    //
    void remove_tape(int index);
    
    //
    // Set start state of the machine
    //
    void start_state(const string&);
    
    //
    // Get vector of all machine states
    //
    vector<string> get_states();
    
    //
    // Get all state transistions for given state
    //
    vector<unique_ptr<Transition>>& get_transitions(const string&);
    
    
    //
    // Return true if machine finished successfuly, false otherwise
    //
    // Machine can finish not successfuly if cannot get to halt state
    //
    bool is_finished_successfuly() const;
   
    //
    // Return machine loaded by file
    //
    static TuringMachine load_machine(const string&);
    
    //
    // Loop over machine
    //
    // Loop over machine defining start state for the loop
    // and new transition when machine must stop
    //
    void loop_over(const string&, Transition*);
    
    //
    // Compose machines
    //
    // Compose current machine with given.
    // The result is current machine become composition of two machines
    //
    void compose(TuringMachine);
    
    
    //
    // Convert multiple to single tape machine
    //
    void to_single_tape();
    
    
    //
    // Add state transistion to the machine
    //
    void add_transition(unique_ptr<Transition>);
    
    //
    // Execute single step of the machine
    //
    void step();
    
    //
    // Run all the steps of the machine until halt or no exit
    //
    void run();
    
    //
    // Print tapes of the machine
    //
    void print();
    
    //
    // Save tapes to file using given filename
    //
    void save_tapes(const string&);
    
    // Implementation of C++ 14
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
};
#endif /* tm_hpp */
