//
//  tm.cpp
//  Turing Machine
//
//  Created by Asen Lekov on 12/11/16.
//  Copyright © 2016 fmi. All rights reserved.
//

#include "tm.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <regex>

const char Tape::EMPTY;
const static State HALT("halt");

Tape::Tape(const string &input) {
    right_.push_back(EMPTY);
    for (int e = (int) (input.length() - 1); e >= 0; --e) {
        right_.push_back(input[e]);
    }
    current_ = right_.back();
    right_.pop_back();
}

void Tape::move_left() {
    right_.push_back(current_);
    if (left_.empty()) {
        left_.push_back(EMPTY);
    }
    current_ = left_.back();
    left_.pop_back();
}

void Tape::move_right() {
    left_.push_back(current_);
    if (right_.empty()) {
        right_.push_back(EMPTY);
    }
    current_ = right_.back();
    right_.pop_back();
}

void Tape::write(char symbol) {
    current_ = symbol;
}

char Tape::read() const {
    return current_;
}

ostream& operator<<(ostream& out, Tape &tape) {
    for (vector<char>::iterator it = tape.right_.begin() ; it != tape.right_.end(); ++it) {
        if ((*it) != Tape::EMPTY) {
            out << *it;
        }
    }
    
    if (tape.current_ != Tape::EMPTY)
        out << tape.current_;
    
    for (vector<char>::iterator it = tape.left_.end(); it != tape.left_.begin(); --it) {
        if ((*it) != Tape::EMPTY) {
            out << *it;
        }
    }
    
    return out;
}

State::State(const string &name) : name_(name) {}

void State::add_transition(Transition *trans) {
    transitions_.push_back(trans);
}

bool State::is_there_transition(const char &input) {
    for (vector<Transition*>::iterator it = transitions_.begin() ; it != transitions_.end(); ++it) {
        if ((*it)->get_read_symbol() == input) {
            return true;
        }
    }
    return false;
}

Transition* State::find_transition(const char &input) {
    for (vector<Transition*>::iterator it = transitions_.begin() ; it != transitions_.end(); ++it) {
        Transition *t = *it;
        if (t->get_read_symbol() == input) {
            return t;
        }
    }
    
    return nullptr;
}

string State::get_name() const {
    return name_;
}

ostream& operator<<(ostream& out, State &state) {
    out << state.name_ << " with " << state.transitions_.size() << " transitions" << endl;
    return out;
}

bool operator== (const State &s, const State &another) {
    return s.name_.compare(another.name_) == 0 && s.transitions_.size() == another.transitions_.size();
}

bool operator!= (const State &s, const State &another) {
    return !(s == another);
}

Transition::Transition(char read, char write, char command, State* state) :
    read_(read), write_(write), command_(command), state_(state)
{}

char Transition::get_command() const {
    return command_;
}

char Transition::get_read_symbol() const {
    return read_;
}

char Transition::get_write_symbol() const {
    return write_;
}

State* Transition::get_next_state() {
    return state_;
}

ostream& operator<<(ostream& out, Transition &transition) {
    out << transition.read_ << "/" << transition.get_write_symbol() << "," << transition.command_ << endl;
    return out;
}

TuringMachine::TuringMachine(const string &input, State *start) :
current_(start) {
    tapes_.push_back(Tape(input));
}

TuringMachine::TuringMachine()
{}

void TuringMachine::add_state(unique_ptr<State> state) {
    states_.push_back(move(state));
}

bool TuringMachine::is_there_state(const string &name) const {
    for(const auto& state : states_) {
        if (state->get_name().compare(name) == 0) {
            return true;
        }
        
    }
    
    throw false;
}

bool TuringMachine::is_finished_successfuly() const {
    return current_ != nullptr && current_->get_name().compare("halt") == 0;
}

State* TuringMachine::find_state(const string &name) {
    for(const auto& state : states_) {
        if (state->get_name().compare(name) == 0) {
            return state.get();
        }

    }
    return nullptr;
}

TuringMachine TuringMachine::load_machine(const string &filename) {
    
    TuringMachine tm;
    
    ifstream ifs(filename, ios::in);
    
    if(ifs.is_open()) {
        string line;
        regex e("[{}(\\->)]");
        while (getline(ifs, line)){
            string replaced = std::regex_replace(line, e, " ");
            
            char read_symbol, write_symbol;
            string old_state, new_state;
            char command;
            
            stringstream ss(replaced);
            
            ss >> read_symbol >> old_state >> write_symbol >> new_state >> command;
            
            State *old_state_ptr;
            State *new_state_ptr;
            
            if (tm.find_state(old_state) == nullptr) {
                old_state_ptr = new State(old_state);
                tm.add_state(std::unique_ptr<State>(old_state_ptr));
            } else {
                old_state_ptr = tm.find_state(old_state);
            }
            
            if (tm.find_state(new_state) == nullptr) {
                new_state_ptr = new State(new_state);
                tm.add_state(std::unique_ptr<State>(new_state_ptr));
            } else {
                new_state_ptr = tm.find_state(new_state);
            }
            
            old_state_ptr->add_transition(new Transition(read_symbol, write_symbol, command, new_state_ptr));
        }
        ifs.close();
    }
    
    return tm;
}

void TuringMachine::step() {
    
    cout << *current_;
    
    Transition *next = current_->find_transition(tapes_[0].read());
   
    
    if (nullptr == next) {
        current_ = nullptr;
        return;
    }
    
    cout << *next;
    
    tapes_[0].write(next->get_write_symbol());
    
    switch (next->get_command()) {
        case 'R':
            tapes_[0].move_right();
            break;
        case 'L':
            tapes_[0].move_left();
            break;
    }
    
     current_ = next->get_next_state();
}

void TuringMachine::run() {
    while(nullptr != current_ && *current_ != HALT) {
        step();
    }
}

void TuringMachine::print() {
    cout << tapes_[0] << endl;
}
