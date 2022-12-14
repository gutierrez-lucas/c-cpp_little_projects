#include <iostream>
#include <cstdint>

using namespace std;

typedef enum days{SUN, MON, TUE, WED, THU, FRI, SAT} days;

inline days operator++(days& d){
    d = static_cast<days>((static_cast<int>(d) + 1) % 7); 
    return d;
}

inline days operator++(days& d, int){
    days temp = d;
    d = static_cast<days>((static_cast<int>(d) + 1) % 7);
    return temp;
}

ostream& operator<<(ostream& out, const days& d){
    switch(d){
        case SUN: out << "SUN"; break;
        case MON: out << "MON"; break;
        case TUE: out << "TUE"; break;
        case WED: out << "WED"; break;
        case THU: out << "THU"; break;
        case FRI: out << "FRI"; break;
        case SAT: out << "SAT"; break;
        default: out << "UNDEF"; break;
    }
    return out;
}

int main(){
    days today{days::MON};

    cout << "alta ppasdasdasdasdija esto" << endl;
    cout << ++today << endl;
    cout << today++ << endl;
    cout << today << endl;

    return 0;
}

