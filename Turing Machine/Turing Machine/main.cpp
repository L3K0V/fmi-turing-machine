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
    
    State q0("q0");
    State q1("q1");
    State q2("q2");
    State halt("halt");
    
    Transition q01('0', ' ', 'R', &q1);
    Transition q02('1', ' ', 'R', &q2);
    
    Transition q11('0', '0', 'R', &q1);
    Transition q12('1', '0', 'R', &q2);
    
    Transition q21('0', '0', 'R', &q2);
    Transition q22('1', ' ', 'R', &halt);
    
    TuringMachine m("0010001", &q0);
    m.add_state(&q0);
    m.find_state("q0")->add_transition(&q01);
    m.find_state("q0")->add_transition(&q02);
    m.add_state(&q1);
    m.find_state("q1")->add_transition(&q11);
    m.find_state("q1")->add_transition(&q12);
    m.add_state(&q2);
    m.find_state("q2")->add_transition(&q21);
    m.find_state("q2")->add_transition(&q22);
    m.add_state(&halt);
    
    m.print();
    m.run();
    m.print();
    
    return 0;
}
