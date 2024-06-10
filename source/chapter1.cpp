#include "gage.h"
#include "chapter1.h"

//Custom DSL:
/* 
label chapter1:
    scene bg washington

    show eileen happy at Left
    say eileen "Hello, nice meeting you!"
    say eileen "How are you?"

    "Doing fine, thank you.":
        show eileen vhappy
        say eileen "I am glad."
    "Not so good honestly.":
        show eileen concerned
        say eileen "I am sorry to hear that."

    show eileen happy
    say eileen "Anyway let's do our best for today, okay?"

    return
*/

void Chapter1(bool next) {
    static int    sm_line = -1;
    static bool   sm_did_choice = false;
    static double sm_wait_time = 0;

    switch(sm_line) 
    {
        default:
        case -1:

        //scene bg washington
        SCENE("bg washington");

        TraceLog(LOG_INFO, "Trace 1");
        WAIT(2.0);
        TraceLog(LOG_INFO, "Trace 2");

        //show eileen happy at Left
        SHOW("eileen", "happy");
        //say eileen "Hello, nice meeting you!"
        SAY("eileen", "Hello, nice meeting you!");
        //say eileen "How are you?"
        SAY("eileen", "How are you");

        CHOICE_BEGIN();
        //"Doing fine, thank you.":
        if(CHOICE("Doing fine, thank you."))
        {
            //    show eileen vhappy
            SHOW("eileen", "vhappy");
            //    say eileen "I am glad."
            SAY("eileen", "I am glad.");
        }
        //"Not so good honestly.":
        else if(CHOICE("Not so good honestly."))
        {
            //    show eileen concerned
            SHOW("eileen", "concerned");
            //    say eileen "I am sorry to hear that."
            SAY("eileen", "I am sorry to hear that.");
        }
        CHOICE_END();
        
        //show eileen happy
        SHOW("eileen", "happy");
        //say eileen "Anyway let's do our best for today, okay?"
        SAY("eileen", "Anyway let's do our best for today, okay?");

        RETURN();
    }
}