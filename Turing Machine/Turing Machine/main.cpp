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
    
    Transition q01('0', ' ', 'R', q1.get());
    Transition q02('1', ' ', 'R', q2.get());
    
    Transition q11('0', '0', 'R', q1.get());
    Transition q12('1', '0', 'R', q2.get());
    
    Transition q21('0', '0', 'R', q2.get());
    Transition q22('1', ' ', 'R', halt.get());
    
    TuringMachine m("0010001", q0.get());
    m.add_state(move(q0));
    m.find_state("q0")->add_transition(&q01);
    m.find_state("q0")->add_transition(&q02);
    m.add_state(move(q1));
    m.find_state("q1")->add_transition(&q11);
    m.find_state("q1")->add_transition(&q12);
    m.add_state(move(q2));
    m.find_state("q2")->add_transition(&q21);
    m.find_state("q2")->add_transition(&q22);
    m.add_state(move(halt));
    
    m.print();
    m.run();
    m.print();
    cout << "Finish successful: " << m.is_finished_successfuly() << endl;
    
    TuringMachine machine = TuringMachine::load_machine("dummy.txt");
    cout << machine.find_state("increment") << endl;
    
    return 0;
}
