//
//  tm.cpp
//  Turing Machine
//
//  Created by Asen Lekov on 12/11/16.
//  Copyright © 2016 fmi. All rights reserved.
//

#include "tm.hpp"

#include <algorithm>

Tape::Tape(const string &input) {
    right_.push_back(EMPTY);
    for (int e = (int) (input.length() - 1); e >= 0; --e) {
        right_.push_back(input[e]);
    }
    current_ = right_.back();
    right_.pop_back();
}

Tape::~Tape() {}

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
        if ((*it) != ' ') {
            out << *it;
        }
    }
    
    if (tape.current_ != ' ')
        out << tape.current_;
    
    for (vector<char>::iterator it = tape.left_.end(); it != tape.left_.begin(); --it) {
        if ((*it) != ' ') {
            out << *it;
        }
    }
    
    return out;
}

State::State(const string &name) : name_(name) {}

State::~State() {}

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

Transition::Transition(char read, char write, char command, State* state) :
    read_(read), write_(write), command_(command), state_(state)
{}

Transition::~Transition() {}

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
    tape_(Tape(input)), current_(start)
{}

TuringMachine::~TuringMachine() {}

void TuringMachine::add_state(State *state) {
    states_.push_back(state);
}

bool TuringMachine::is_there_state(const string &name) {
    for (vector<State*>::iterator it = states_.begin() ; it != states_.end(); ++it) {
        State *s = *it;
        if (s->get_name().compare(name) == 0) {
            return true;
        }
    }
    
    throw false;
}

State* TuringMachine::find_state(const string &name) {
    for (vector<State*>::iterator it = states_.begin() ; it != states_.end(); ++it) {
        State *s = *it;
        if (s->get_name().compare(name) == 0) {
            return s;
        }
    }
    
    return nullptr;
}

void TuringMachine::step() {
    
    cout << *current_;
    
    Transition *next = current_->find_transition(tape_.read());
    
    cout << *next;
    
    tape_.write(next->get_write_symbol());
    
    switch (next->get_command()) {
        case 'R':
            tape_.move_right();
            break;
        case 'L':
            tape_.move_left();
            break;
    }
    
    current_ = next->get_next_state();
}

void TuringMachine::run() {
    while(current_->get_name().compare("halt") != 0) {
        step();
    }
}

void TuringMachine::print() {
    cout << tape_ << endl;
}
