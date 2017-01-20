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
#include <string>

const char Tape::EMPTY;

Tape::Tape(const string &input) {
    
    if (input[0] == DELIMITER) {
        // Multiple tapes so...
        stringstream ss(input);
        string item;
        while (getline(ss, item, DELIMITER)) {
            
            if (item.empty()) {
                continue;
            }
            
            if (current_ == '\0') {
                initialize(item);
            } else {
                virtual_tapes_.push_back(Tape(item));
            }
        }
        return;
    }
    
    initialize(input);
}

void Tape::initialize(const string &input) {
    for (int e = (int) (input.length() - 1); e >= 0; --e) {
        right_.push_back(input[e]);
    }
    current_ = right_.back();
    right_.pop_back();
}

Tape::Tape(const Tape &other) {
    left_ = other.left_;
    right_ = other.right_;
    current_ = other.current_;
    virtual_tapes_ = other.virtual_tapes_;
}

void Tape::move_left(int index) {
    
    // Handle moving left of virtual tape
    if (index != -1 && index < virtual_tapes_.size()) {
        virtual_tapes_[index].move_left();
        return;
    }
    
    right_.push_back(current_);
    if (left_.empty()) {
        left_.push_back(EMPTY);
    }
    current_ = left_.back();
    left_.pop_back();
}

void Tape::move_right(int index) {
    
    // Handle moving right of virtual tape
    if (index != -1 && index < virtual_tapes_.size()) {
        virtual_tapes_[index].move_right();
        return;
    }
    
    left_.push_back(current_);
    if (right_.empty()) {
        right_.push_back(EMPTY);
    }
    current_ = right_.back();
    right_.pop_back();
}

void Tape::write(char symbol, int index) {
    
    // Handle write of virtual tape
    if (index != -1 && index < virtual_tapes_.size()) {
        virtual_tapes_[index].write(symbol);
        return;
    }
    
    current_ = symbol;
}

char Tape::read(int index) const {
    
    if (index != -1 && index < virtual_tapes_.size()) {
        return virtual_tapes_[index].read();
    }
    
    return current_;
}

ostream& operator<<(ostream& out, Tape &tape) {
    
    if (tape.virtual_tapes_.size() > 0) {
        cout << '#';
    }
   
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
    
    for (auto tape : tape.virtual_tapes_) {
        out << '#';
        out << tape;
    }
    
    return out;
}



Transition::Transition(const string& current_state, const string& read, const string& write, const string& command, const string& next_state) :
    current_state_(current_state), read_(read), write_(write), command_(command), next_state_(next_state)
{}

Transition::Transition(const Transition &other) {
    current_state_ = other.current_state_;
    read_ = other.read_;
    write_ = other.write_;
    next_state_ = other.next_state_;
    command_ = other.command_;
}

char Transition::get_command(int tape = 0) const {
    return tape > command_.size() - 1 ? '\0' : command_[tape];
}

string Transition::get_command() const {
    return command_;
}

char Transition::get_read_symbol(int tape = 0) const {
    return tape > read_.size() - 1 ? '\0' :read_[tape];
}

string Transition::get_read_symbols() const {
    return read_;
}

char Transition::get_write_symbol(int tape = 0) const {
    return tape > write_.size() - 1 ? '\0' :write_[tape];
}

string Transition::get_write_symbols() const {
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

TuringMachine::TuringMachine(const TuringMachine &other) {
    current_state_ = other.current_state_;
    
    for (const auto& e : other.tapes_) {
        tapes_.push_back(make_unique<Tape>(*e));
    }
    
    for (const auto& imap: other.mapping_) {
        for (const auto& itrans: imap.second) {
             mapping_[imap.first].push_back(make_unique<Transition>(*itrans));
        }
    }
}

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
            
            string read_symbols, write_symbols;
            string old_state, new_state;
            string command;
            
            stringstream ss(replaced);
            
            ss >> read_symbols >> old_state >> write_symbols >> new_state >> command;
            
            tm.add_transition(unique_ptr<Transition>(new Transition(old_state, read_symbols, write_symbols, command, new_state)));
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

void TuringMachine::compose(TuringMachine another) {
    auto another_states = another.get_states();
    
    for(auto const& state: get_states()) {
        for (auto const& transition: mapping_[state]) {
            if (transition->get_next_state().compare("halt") == 0) {
                transition->change_next_state(another.current_state_);
            }
        }
    }
    
    for (const auto& state : another_states) {
        const auto& another_trans = another.get_transitions(state);
        
        for (const auto& trans : another_trans) {
            mapping_[state].push_back(make_unique<Transition>(*trans));
        }
    }
}

void TuringMachine::to_single_tape() {
    if (tapes_.size() == 1) {
        return;
    }
    
    stringstream ss("", ios_base::app | ios_base::out);
    
    for (const auto &tape : tapes_) {
        ss << '#';
        ss << *tape;
    }
    
    tapes_.erase(tapes_.begin(), tapes_.end());
    Tape* tape = new Tape(ss.str());
    add_tape(make_unique<Tape>(*tape));
}

void TuringMachine::step() {
    
    Transition *next = find_transitions(tapes_[0]->read());
    
    if (nullptr == next) {
        current_state_ = "";
        return;
    }
    
    cout << *next << endl;
    current_state_ = next->get_next_state();
    
    if (tapes_.size() == 1) {
        for (int r = 0, t = -1; r < next->get_read_symbols().size(); ++t, ++r) {
            if (tapes_[0]->read(t) == next->get_read_symbols()[r] && next->get_write_symbol(r) != '\0') {
                tapes_[0]->write(next->get_write_symbol(r), t);
            }
            
            switch (next->get_command(t)) {
                case 'R':
                    tapes_[0]->move_right(t);
                    break;
                case 'L':
                    tapes_[0]->move_left(t);
                    break;
            }
        }
        return;
    }
    
    for (int t = 0; t < tapes_.size(); ++t) {
        if (tapes_[t]->read() == next->get_read_symbol(t) && next->get_write_symbol(t) != '\0') {
            tapes_[t]->write(next->get_write_symbol(t));
        }
        
        switch (next->get_command(t)) {
            case 'R':
                tapes_[t]->move_right();
                break;
            case 'L':
                tapes_[t]->move_left();
                break;
        }
    }
}

void TuringMachine::run() {
    while(current_state_ != "" && current_state_ != "halt") {
        step();
    }
}

void TuringMachine::print() {
    cout << *tapes_[0] << endl;
}
