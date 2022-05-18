#include <iostream>
#include <fstream>
#include "HTTPRequest.hpp"
#include <string>
#include <time.h> 
#include <sstream>

using namespace std; 

clock_t t, deltaT;
string sCompactState ;
string extendedState ; 

char compactState[6]; 

class State {
public: 
    char motor0, motor1, servo0, servo1, servo2; 
    State(); 
    void updateState(const char*);
    int getMotor0(void) ;
    int getMotor1(void);
    int getServo0(void);
    int getServo1(void);
    int getServo2(void);
    void getExtendedState(void); 
};

State::State() {
    motor0 = 0; 
    motor1 = 0; 
    servo0 = 0;
    servo1 = 0;
    servo2 = 0; 
}

void State::updateState(const char* sCompact) {
    motor0 = sCompact[0];
    motor1 = sCompact[1];
    servo0 = sCompact[2];
    servo1 = sCompact[3];
    servo2 = sCompact[4];
}

int State::getMotor0(void) {
    return (255*(int)this->motor0)/180 + 1; 
}

int State::getMotor1(void) {
    return (255*(int)this->motor1)/180 + 1;
}

int State::getServo0(void) {
    int angle = this->servo0;
    angle = angle - 90; 
    return angle;
}

int State::getServo1(void) {
    int angle = (int)this->servo1; 
    angle = angle - 90;
    return angle;
}

int State::getServo2(void) {
    int angle = (int)this->servo2;
    angle = angle - 90;
    return angle;
}

void State::getExtendedState(void) {
    string extendedState; 
    ostringstream osExtendedState;
    osExtendedState.str(""); 
    osExtendedState << this->getMotor0() << " " << this->getMotor1() << " " << this->getServo0() << " " << this->getServo1() << " " << this->getServo2() << endl ;
    cout << osExtendedState.str() << endl ;
    
}

int main(int argc, const char* argv[])
{
    sCompactState = "00000"; 

    State state;
    
    t = clock(); // time restart
    int ms = 1000;

    try
    {
        // you can pass http::InternetProtocol::V6 to Request to make an IPv6 request
        http::Request request{ "http://esp32.local/getData" };

        // send a get request
        const auto response = request.send("GET");

        sCompactState = string{ response.body.begin(), response.body.end() }; // Storage recieved data into a string

        cout << sCompactState << endl; 

        state.updateState(sCompactState.c_str()); // Stirage the data into an object

        cout << state.getMotor0() << endl;

        state.getExtendedState(); 

    }
    catch (const std::exception& e)
    {
        std::cerr << "Request failed, error: " << e.what() << '\n';
    }

    return EXIT_SUCCESS;
}
