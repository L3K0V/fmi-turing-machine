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
    
    TuringMachine tm;
    tm.add_tape(Tape("0001"));
    tm.start_state("start");
    tm.add_transition(unique_ptr<Transition>(new Transition("start", '0', 'X', 'R', "start2")));
    tm.add_transition(unique_ptr<Transition>(new Transition("start2", '0', 'X', 'R', "start3")));
    tm.add_transition(unique_ptr<Transition>(new Transition("start3", '0', 'X', 'R', "halt")));
    tm.add_transition(unique_ptr<Transition>(new Transition("start", '1', 'X', 'R', "halt")));
    
    tm.print();
    tm.run();
    tm.print();
    
    return 0;
}
