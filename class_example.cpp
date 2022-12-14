#include <iostream>
#include <cstdint>

using namespace std;

class point{
    public:
        point(double x=0.0, double y=0.0):x(x),y(y){}
        double getx(){return x;}
        double gety(){return y;}
        void   setx(double v){x=v;}
        void   sety(double v){y=v;}
    private:
        double x,y;
};

point operator+(point& p1, point& p2){
    point sum{p1.getx() + p2.getx(), p1.gety() + p2.gety()};
    return sum;
}

ostream& operator<<(ostream& out, point& p){
    out << "(" << p.getx() << ", " << p.gety() << ")";
    return out;
}

int main(){
    point p1{2,8}, p2{24,1}, p3;
    

    p3 = p1 + p2;
    cout << "class test" << endl;
    cout << p1 << endl;
    cout << p2 << endl;
    cout << p3 << endl;
    return 0;
}