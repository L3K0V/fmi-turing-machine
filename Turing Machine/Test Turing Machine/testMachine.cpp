//
//  testMachine.cpp
//  Turing Machine
//
//  Created by Asen Lekov on 1/19/17.
//  Copyright © 2017 fmi. All rights reserved.
//

#include "catch.hpp"
#include "tm.hpp"

#include <sstream>

SCENARIO("Try simple rewrite single char of the tape") {
    GIVEN("Initialized machine with simple tape and start state") {
        TuringMachine m;
        m.start_state("start");
        m.add_tape(unique_ptr<Tape>(new Tape("00000")));
        m.add_transition(unique_ptr<Transition>(new Transition("start", "0", "X", "R", "halt")));
        
        WHEN("check tape value") {
            
            std::stringstream sstream;
            sstream << *m.get_tape(0);
            
            THEN("tape must be 00000") {
                REQUIRE(sstream.str().compare("00000") == 0);
            }
        }
        
        WHEN("Run the machine") {
            m.run();
            
            THEN("Machine must be exit successfuly") {
                REQUIRE(m.is_finished_successfuly());
            }
            
            AND_THEN("Tape value must be with only one changed 0 to X") {
                std::stringstream sstream;
                sstream << *m.get_tape(0);
                
                REQUIRE(sstream.str().compare("X0000") == 0);
            }
            
        }
    }
}

SCENARIO("Try loop over machine rewriting zeros values with X") {
    GIVEN("Initialized loop machine with simple tape and start state") {
        TuringMachine m;
        m.start_state("start");
        m.add_tape(unique_ptr<Tape>(new Tape("00001")));
        m.add_transition(unique_ptr<Transition>(new Transition("start", "0", "X", "R", "halt")));
        m.loop_over("start", new Transition("start", "1", "1", "N", "halt"));
        
        WHEN("Start the machine") {
            m.run();
            
            m.print();
            
            THEN("All zeros must be replaces by X") {
                std::stringstream sstream;
                sstream << *m.get_tape(0);
                
                REQUIRE(sstream.str().compare("XXXX1") == 0);
            }
        }
    }
}

SCENARIO("Convert multiple to single tape machine") {
    GIVEN("Multiple tape machine") {
        TuringMachine m;
        m.add_tape(unique_ptr<Tape>(new Tape("1010")));
        m.add_tape(unique_ptr<Tape>(new Tape("0000")));
        m.add_tape(unique_ptr<Tape>(new Tape("0000")));
        m.start_state("start");
        
        m.add_transition(unique_ptr<Transition>(new Transition("start", "100", "1X0", "RRR", "next")));
        m.add_transition(unique_ptr<Transition>(new Transition("next", "000", "00X", "RRR", "halt")));
        
        WHEN("Convert the machine to single tape") {
            m.run();
            m.print();
            m.to_single_tape();
            m.print();
            
            THEN("The tape must be compined") {
                std::stringstream sstream;
                sstream << *m.get_tape(0);
                
                REQUIRE(sstream.str().compare("#1010#X000#0X00") == 0);
            }
        }
    }
}
