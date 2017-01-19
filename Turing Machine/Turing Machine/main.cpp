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
    tm.add_tape(unique_ptr<Tape>(new Tape("0001")));
    tm.start_state("start");
    tm.add_transition(unique_ptr<Transition>(new Transition("start", '0', '0', 'R', "halt")));
    tm.add_transition(unique_ptr<Transition>(new Transition("start", '1', 'X', 'R', "halt")));
    
    tm.loop_over("start", new Transition("start", '1', 'X', 'R', "halt"));
    
    tm.print();
    tm.run();
    tm.print();
    
    return 0;
}
