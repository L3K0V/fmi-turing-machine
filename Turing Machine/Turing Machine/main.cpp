//
//  main.cpp
//  Turing Machine
//
//  Created by Asen Lekov on 12/8/16.
//  Copyright © 2016 fmi. All rights reserved.
//

#include <iostream>
#include "tm.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    Tape tape("0010001");
    
    auto q0 = std::unique_ptr<State>(new State("q0"));
    auto q1 = std::unique_ptr<State>(new State("q1"));
    auto q2 = std::unique_ptr<State>(new State("q2"));
    auto halt = std::unique_ptr<State>(new State("halt"));
    
    auto q01 = new Transition('0', ' ', 'R', q1.get());
    auto q02 = new Transition('1', ' ', 'R', q2.get());
    
    auto q11 = new Transition('0', '0', 'R', q1.get());
    auto q12 = new Transition('1', '0', 'R', q2.get());
    
    auto q21 = new Transition('0', '0', 'R', q2.get());
    auto q22 = new Transition('1', ' ', 'R', halt.get());
    
    TuringMachine m("0010001", q0.get());
    m.add_state(move(q0));
    m.find_state("q0")->add_transition(unique_ptr<Transition>(q01));
    m.find_state("q0")->add_transition(unique_ptr<Transition>(q02));
    m.add_state(move(q1));
    m.find_state("q1")->add_transition(unique_ptr<Transition>(q11));
    m.find_state("q1")->add_transition(unique_ptr<Transition>(q12));
    m.add_state(move(q2));
    m.find_state("q2")->add_transition(unique_ptr<Transition>(q21));
    m.find_state("q2")->add_transition(unique_ptr<Transition>(q22));
    m.add_state(move(halt));
    
    m.print();
    m.run();
    m.print();
    cout << "Finish successful: " << m.is_finished_successfuly() << endl;
    
    TuringMachine machine = TuringMachine::load_machine("dummy.txt");
    cout << machine.find_state("increment") << endl;
    
    return 0;
}
