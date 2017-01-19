//
//  testTape.cpp
//  Turing Machine
//
//  Created by Asen Lekov on 12/8/16.
//  Copyright © 2016 fmi. All rights reserved.
//

#include "catch.hpp"
#include "tm.hpp"

SCENARIO("Navigate trhough tape") {
    GIVEN("Tape with some initial data") {
        Tape t("123");
        
        REQUIRE( t.read() == '1');
        
        WHEN("Move tape left") {
            t.move_left();
            THEN("Value must be empty") {
                REQUIRE( t.read() == ' ' );
            }
        }
        
        AND_WHEN("Move tape right") {
            THEN("Value must be again initial") {
                REQUIRE( t.read() == '1');
            }
        }
        
        WHEN("Move tape right 3 times") {
            t.move_right();
            t.move_right();
            t.move_right();
            
            AND_THEN("Current value must be empty") {
                REQUIRE( t.read() == ' ');
            }
        }
    }
}

SCENARIO("Change values of tape") {
    GIVEN("Tape with some initial data") {
        Tape t("42");
        
        WHEN("We change initial value to X") {
            
            t.write('X');
            
            THEN("Value must be X") {
                REQUIRE( t.read() == 'X');
            }
            
            AND_THEN("We change the last value to X") {
                t.move_right();
                t.write('X');
                
                WHEN("We checked current value must be X") {
                    REQUIRE(t.read() == 'X');
                    
                    AND_WHEN("We go left and check initial value also must be X") {
                        t.move_left();
                        
                        REQUIRE(t.read() == 'X');
                    }
                }
            }
        }
        
    }
}
