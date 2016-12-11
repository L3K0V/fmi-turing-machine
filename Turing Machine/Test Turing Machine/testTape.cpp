//
//  testTape.cpp
//  Turing Machine
//
//  Created by Asen Lekov on 12/8/16.
//  Copyright © 2016 fmi. All rights reserved.
//

#include "catch.hpp"
#include "tm.hpp"

SCENARIO("Navigate trhough tape", "[tape]") {
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
    }
}
