#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <iomanip>

using namespace std;

/*
int getEventID(const string &evt){
    //if( evt == "stop_inject" ) return 1;
    //else if( evt == "rej_scri" ) return 2;
    //else if( evt == "pm_scri" ) return 3;
    //else if( evt == "rej_sreg" ) return 4;
    //else if( evt == "rej_correct_color" ) return 5;
    else if( evt == "hor_inj_close" ) return 6;
    //else if( evt == "pm_sreg" ) return 7;
    // else if( evt == "button_stop" ) return 8;
    else if( evt == "ver_inj_open" ) return 9;
    //else if( evt == "mold_low_temp" ) return 10;
    else if( evt == "button_color" ) return 11;
    else if( evt == "mold_close" ) return 12;
    else if( evt == "rej_reset_color" ) return 13;
    else if( evt == "mold_open" ) return 14;
    //else if( evt == "mold_high_temp" ) return 15;
    else if( evt == "pig_close" ) return 16;
    //else if( evt == "pig_sreg" ) return 17;
    else if( evt == "pig_open" ) return 18;
    else if( evt == "inject" ) return 19;
    //else if( evt == "pig_scri" ) return 20;
    else if( evt == "ver_inj_close" ) return 21;
    else if( evt == "hor_inj_open" ) return 22;
    //else if( evt == "button_recyc" ) return 23;
    return 0;
}
*/

string getEventByID(const int &id){
    switch( id ){
        case 13: return "REJECT_RESET_COLOR";
        case 22: return "HORIZONTAL_INJECTOR_OPEN";
        case  9: return "VERTICAL_INJECTOR_OPEN";
        case 18: return "PIGMENT_OPEN";
        case 12: return "MOLD_CLOSE";
        case 19: return "INJECT";
        case 14: return "MOLD_OPEN";
        case 16: return "PIGMENT_CLOSE";
        case 21: return "VERTICAL_INJECTOR_CLOSE";
        case  6: return "HORIZONTAL_INJECTOR_CLOSE";
    }
    return "null - "+id;
}




const vector<string> explode(const string& s, const char& c){
	string buff{""};
	vector<string> v;
	for(auto n:s){
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);
	return v;
}




// /* ***********************
// --- FOR UNCONTROLLABLE EVENTS
int main(int argc, char** argv){
    if( argc < 2 ){
        cerr << "Inform the eventID to be checked!" << endl;
        return 1;
    }

    int id=1;
    map<int,int> stateMap;
    vector<string> pieces;
    vector<string> subpieces;

    int eventID = atoi( argv[1] );
    string line;
    while( getline( cin, line ) ){
        unsigned found = line.find("<state");
        if( found != std::string::npos ){
            pieces = explode( line, ' ' );
            subpieces = explode( pieces[1], '"' );
            int idState = atoi( subpieces[1].c_str() );
            stateMap[idState] = id;
            id++;
        }
        else{
            found = line.find("<transition");
            if( found != std::string::npos ){
                pieces = explode( line, ' ' );
                subpieces = explode( pieces[2], '"' );
                int source = atoi( subpieces[1].c_str() );
                source = stateMap[ source ];
                subpieces = explode( pieces[3], '"' );
                int target = atoi( subpieces[1].c_str() );
                target = stateMap[ target ];
                subpieces = explode( pieces[4], '"' );
                int event = atoi( subpieces[1].c_str() );
                // string eventDescription = getEventByID( event );
                //cerr << "source: " << source << " ";
                //cerr << "target: " << target << " ";
                //cerr << "event: " << event << endl;

                if( event == eventID )
                    cout << "case " << setw(4) << source << ": cs = " << target << "; break;" << endl;
            }
        }
    }
    return 0;
}
// ********************** */














bool isControllableEvent(int event){
    if( event == 6 or event == 9 or event == 12 or event == 13 or event == 14
       or event == 16 or event == 18 or event == 19 or event == 21 or event == 22 )
        return true;
    return false;
}


//  1st - "rej_reset_color" (13)
//  2nd - "hor_inj_open" (22)
//  3rd - "ver_inj_open"  (9)
//  4th - "pig_open" (18)
//  5th - "mold_close" (12)
//  6th - "inject" (19)
//  7th - "mold_open" (14)
//  8th - "pig_close" (16)
//  9th - "ver_inj_close" (21)
// 10th - "hor_inj_close" (6)
bool mostPriority( int candidate, int event ){
    switch( candidate ){
        case 13: return true;
        case 22: if( event!=13 ) return true; break;
        case  9: if( event!=13 && event!=22 ) return true; break;
        case 18: if( event!=13 && event!=22 && event!=9 ) return true; break;
        case 12: if( event!=13 && event!=22 && event!=9 && event!=18 ) return true; break;
        case 19: if( event==6 or event==21 or event==16 or event==14 ) return true; break;
        case 14: if( event==6 or event==21 or event==16 ) return true; break;
        case 16: if( event==6 or event==21 ) return true; break;
        case 21: if( event==6 ) return true; break;
    }
    return false;
}

/* ***********************
// --- FOR CONTROLLABLE EVENTS
int main(int argc, char** argv){
    string line;
    int id=1;
    map<int,int> stateMap;
    map<int,int> stateEvent;
    map<int,int> stateTarget;
    vector<string> pieces;
    vector<string> subpieces;

    while( getline( cin, line ) ){
        unsigned found = line.find("<state");
        if( found != std::string::npos ){
            pieces = explode( line, ' ' );
            subpieces = explode( pieces[1], '"' );
            int idState = atoi( subpieces[1].c_str() );
            stateMap[idState] = id;
            id++;
        }
        else{
            found = line.find("<transition");
            if( found != std::string::npos ){
                pieces = explode( line, ' ' );
                subpieces = explode( pieces[2], '"' );
                int source = atoi( subpieces[1].c_str() );
                source = stateMap[ source ];
                subpieces = explode( pieces[3], '"' );
                int target = atoi( subpieces[1].c_str() );
                target = stateMap[ target ];
                subpieces = explode( pieces[4], '"' );
                int event = atoi( subpieces[1].c_str() );

                if( isControllableEvent(event) ){
                    auto it = stateEvent.find( source );
                    if( it == stateEvent.end() ){
                        stateEvent[ source ] = event;
                        stateTarget[ source ] = target;
                    }
                    else{
                        // if event has most priority over *it
                        if( mostPriority( event, it->second ) ){
                            stateEvent[ source ] = event;
                            stateTarget[ source ] = target;
                        }
                    }
                }
            }
        }
    }

    auto itEvent = stateEvent.begin();
    auto itTarget = stateTarget.begin();
    for( ; itEvent!=stateEvent.end(); ++itEvent, ++itTarget ){
        cout << "case " << itEvent->first << ":  action(Plant.PlantEvent." << getEventByID( itEvent->second ) << "); cs = ";
        cout << itTarget->second << "; break;" << endl;
    }


    return 0;
}
// ***********************/

