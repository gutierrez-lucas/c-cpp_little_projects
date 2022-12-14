#include <iostream>
#include <cstdint>

using namespace std;

class list_element{
    public:
        list_element(int n=0, list_element* ptr=nullptr):d(n), next(ptr){}
        int d;
        list_element* next;
        list_element* prev;
};

class list{
    public:
        list():head(nullptr), tail(nullptr), cursor(nullptr){} // empty list
        list(const int* arr, int n);            // list from array of ints
        list(const list& lst);                  // deep copy of another list
        ~list();
        list_element* get_head(){return head;}  // get head, useful for overload <<
        list_element* get_tail(){return tail;}  // get head, useful for overload <<
        void prepend(int n);
        int get_element_value(int element_position);
        void advance(){cursor = cursor -> next;}
    private:
        list_element* head;
        list_element* tail;
        list_element* cursor;
};

int list::get_element_value(int element_position){
    int current_position = 0;
    while(current_position++ != element_position){cursor = cursor -> next;}
    return cursor -> d;
}

list::list(const int* arr, int n){
    for(int i=0; i<n; i++){
        prepend(*(arr+i));
    }
    cursor = head;
}

list::list(const list& lst){
    if(lst.head == nullptr){
        // if lst is empty create empty default empty list
        head = nullptr; cursor = nullptr; tail = nullptr;
    }else{
        list_element* h = new list_element;
        list_element* previous;
        cursor = lst.head;
        head = h;                   // this list head to to_copy head
        h -> d = lst.head -> d;     // copy to_copy data into this list data
        previous = h;
        for( cursor = lst.head; cursor != nullptr;){
            h = new list_element;
            h -> d = cursor -> d;
            previous -> next = h;
            cursor = cursor -> next;
            previous = h;
        }
        tail = cursor;
        cursor = head;
    }
}

list::~list(){
    for(cursor = head; cursor != nullptr;){
        cursor = head -> next;
        delete head;
        head = cursor;
    }
    std::cout << "List destroyed" << std::endl;
}

void list::prepend(int n){
    if(head == nullptr){
        cursor = head = new list_element(n, head);
        tail = head;
    }else{
        head = new list_element(n, head);
        head -> next -> prev = head;
    }
}


ostream& operator<<(ostream& out, list& l){
    list_element* h = l.get_head();
    out << "HEAD->" << h->d;
    h = h -> next;
    while(h != nullptr){
        out << "," << h->d;
        h = h -> next;
    }
    out << "->TAIL" << std::endl;
    return out;
}

ostream& operator>>(ostream& out, list& l){
    list_element* h = l.get_tail();
    out << "TAIL->" << h->d;
    h = h -> prev;
    while(h != nullptr){
        out << "," << h->d;
        h = h -> prev;
    }
    out << "->HEAD" << std::endl;
    return out;
}

int main(){

    int vec[]={1,2,3,4,5,6,7,8,9};
    list a(vec, sizeof(vec)/sizeof(vec[0]));
    std::cout << "   foward a is: " << a;
    std::cout << "backwards a is: " >> a;

    return 0;
}