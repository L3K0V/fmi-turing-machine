//
//  main.cpp
//  Turing Machine
//
//  Created by Asen Lekov on 12/8/16.
//  Copyright © 2016 fmi. All rights reserved.
//

#include "tm.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    
    Tape t("#0001#0000#0000");
    t.write('X');
    t.move_right(0);
    t.write('X', 0);
    t.move_right(1);
    t.write('X', 1);
    
    cout << t << endl;
    
    TuringMachine tm;
    tm.add_tape(unique_ptr<Tape>(new Tape("0001")));
    tm.add_tape(unique_ptr<Tape>(new Tape("0000")));
    
    tm.to_single_tape();
    
    tm.print();
    
    tm.start_state("start");
    tm.add_transition(unique_ptr<Transition>(new Transition("start", "0", "0", "R", "halt")));
//    tm.add_transition(unique_ptr<Transition>(new Transition("start", '1', 'X', 'R', "halt")));
    
//    tm.loop_over("start", new Transition("start", '1', 'X', 'R', "halt"));

    TuringMachine tm2;
    tm2.start_state("start2");
    tm2.add_transition(unique_ptr<Transition>(new Transition("start2", "0", "X", "R", "start")));
    tm2.add_transition(unique_ptr<Transition>(new Transition("start2", "1", "1", "N", "halt")));
    
    tm.compose(tm2);
    
    tm.print();
    tm.run();
    tm.print();
    
    return 0;
}
