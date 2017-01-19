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

Tape::Tape(const string &input) {
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
   
    for (vector<char>::iterator it = tape.left_.begin(); it != tape.left_.end(); ++it) {
        if ((*it) != Tape::EMPTY) {
            out << *it;
        }
    }

    if (tape.current_ != Tape::EMPTY) {
        out << tape.current_;
    }

    for (vector<char>::reverse_iterator it = tape.right_.rbegin(); it != tape.right_.rend(); ++it) {
        if ((*it) != Tape::EMPTY) {
            out << *it;
        }
    }
    return out;
}



Transition::Transition(const string& current_state, char read, char write, char command, const string& next_state) :
    current_state_(current_state), read_(read), write_(write), command_(command), next_state_(next_state)
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

string Transition::get_next_state() const {
    return next_state_;
}

string Transition::get_current_state() const {
    return current_state_;
}

void Transition::change_next_state(const string& next_state) {
    next_state_ = next_state;
}

ostream& operator<<(ostream& out, Transition &transition) {
    out << transition.read_ << "{" << transition.current_state_ << "} -> " << transition.write_ << "{" << transition.next_state_ << "}" << transition.command_;
    return out;
}

TuringMachine::TuringMachine() : current_state_("halt")
{}

void TuringMachine::add_tape(unique_ptr<Tape> tape) {
    tapes_.push_back(std::move(tape));
}

Tape* TuringMachine::get_tape(int index) {
    return tapes_[index].get();
}

void TuringMachine::remove_tape(int index) {
    tapes_.erase(tapes_.begin() + 1);
}

void TuringMachine::start_state(const string& state) {
    current_state_ = state;
}

void TuringMachine::add_transition(unique_ptr<Transition> transition) {
            
    mapping_[transition.get()->get_current_state()].push_back(std::move(transition));
}

Transition* TuringMachine::find_transitions(const char &input) {
    
    const auto& transitions = mapping_[current_state_];
    
    for (const auto& transition : transitions) {
        if (transition->get_read_symbol() == input) {
            return transition.get();
        }
    }
    
    return nullptr;
}

vector<string> TuringMachine::get_states() {
    vector<string> keys;
    
    for(auto const& imap: mapping_)
        keys.push_back(imap.first);
    return keys;
}

vector<unique_ptr<Transition>>& TuringMachine::get_transitions(const string& state) {
    return mapping_[state];
}

bool TuringMachine::is_finished_successfuly() const {
    return current_state_.compare("halt") == 0;
}

TuringMachine TuringMachine::load_machine(const string &filename) {
    
    TuringMachine tm;
    
    ifstream ifs(filename, ios::in);
    
    if(ifs.is_open()) {
        string line;
        
        // The transision format is: read_symbol{old_state} -> write_symbol{new_state}command
        // or...                     6{increment} -> 7{decrement}L
        regex e("[{}(\\->)]");
        while (getline(ifs, line)){
            string replaced = std::regex_replace(line, e, " ");
            
            char read_symbol, write_symbol;
            string old_state, new_state;
            char command;
            
            stringstream ss(replaced);
            
            ss >> read_symbol >> old_state >> write_symbol >> new_state >> command;
            
            tm.add_transition(unique_ptr<Transition>(new Transition(old_state, read_symbol, write_symbol, command, new_state)));
        }
        ifs.close();
    }
    
    return tm;
}

void TuringMachine::loop_over(const string& loop, Transition* halt) {
    
    std::vector<int> vints;
    
    for(auto const& state: get_states()) {
        for (auto const& transition: mapping_[state]) {
            if (transition->get_next_state().compare("halt") == 0) {
                transition->change_next_state(loop);
            }
        }
    }
    
    add_transition(unique_ptr<Transition>(halt));
}

void TuringMachine::compose(TuringMachine *another) {
    auto another_states = another->get_states();
    
    for(auto const& state: get_states()) {
        for (auto const& transition: mapping_[state]) {
            if (transition->get_next_state().compare("halt") == 0) {
                transition->change_next_state(another->current_state_);
            }
        }
    }
    
    for (const auto& state : another_states) {
        const auto& another_trans = another->get_transitions(state);
        
        for (auto& trans : another_trans) {
            mapping_[state].push_back(unique_ptr<Transition>(trans.get()));
        }
    }
}

void TuringMachine::step() {
    
    Transition *next = find_transitions(tapes_[0]->read());
    
    if (nullptr == next) {
        current_state_ = "";
        return;
    }
    
    tapes_[0]->write(next->get_write_symbol());
    
    switch (next->get_command()) {
        case 'R':
            tapes_[0]->move_right();
            break;
        case 'L':
            tapes_[0]->move_left();
            break;
    }
    
    cout << *next << endl;
    
     current_state_ = next->get_next_state();
}

void TuringMachine::run() {
    while(current_state_ != "" && current_state_ != "halt") {
        step();
    }
}

void TuringMachine::print() {
    cout << *tapes_[0] << endl;
}
