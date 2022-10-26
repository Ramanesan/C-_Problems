#include <iostream>
#include <math.h>
#include <sstream>

//Ramanesan Arunan
//400318194



using namespace std;
class Term
{
    public:
        Term(int c=0.0, int e=0) : coef(c), exp(e) {}
        int coef;
        int exp;
};

typedef Term Elem;				// list element type Term
class DNode {					    // doubly linked list node
    private:
        Elem elem;					// node element value
        DNode* prev;				// previous node in list
        DNode* next;				// next node in list
        friend class DLinkedList;	// allow DLinkedList access
        friend class Polynomial;
};

class DLinkedList {				// doubly linked list
    public:
        DLinkedList();				// constructor
        ~DLinkedList();				// destructor
        bool empty() const;				// is list empty?
        const Elem& front() const;			// get front element
        const Elem& back() const;			// get back element
        void addFront(const Elem& e);		// add to front of list
        void addBack(const Elem& e);		// add to back of list
        void removeFront();				// remove from front
        void removeBack();				// remove from back
    private:					// local type definitions
        DNode* header;				// list sentinels
        DNode* trailer;
        friend class Polynomial;
    protected:					// local utilities
        void add(DNode* v, const Elem& e);		// insert new node before v
        void remove(DNode* v);			// remove node v
        
};

DLinkedList::DLinkedList() {			// constructor
    header = new DNode;				// create sentinels
    trailer = new DNode;
    header->next = trailer;			// have them point to each other
    trailer->prev = header;
}

bool DLinkedList::empty() const		// is list empty?
    { return (header->next == trailer); }

const Elem& DLinkedList::front() const	// get front element
    { return header->next->elem; }

const Elem& DLinkedList::back() const		// get back element
    { return trailer->prev->elem; }

DLinkedList::~DLinkedList() {			// destructor
    while (!empty()) removeFront();		// remove all but sentinels
    delete header;				// remove the sentinels
    delete trailer;
}

void DLinkedList::remove(DNode* v) {		// remove node v
    DNode* u = v->prev;				// predecessor
    DNode* w = v->next;				// successor
    u->next = w;				// unlink v from list
    w->prev = u;
    delete v;
}

void DLinkedList::removeFront()		// remove from font
    { remove(header->next); }
  
void DLinkedList::removeBack()		// remove from back
    { remove(trailer->prev); }

void DLinkedList::add(DNode* v, const Elem& e) { // insert new node before v
    DNode* u = new DNode;  
    u->elem = e;		// create a new node for e
    u->next = v;				// link u in between v
    u->prev = v->prev;				// ...and v->prev
    v->prev->next = u;
    v->prev = u;
  }

void DLinkedList::addFront(const Elem& e)	// add to front of list
    { add(header->next, e); }
  
void DLinkedList::addBack(const Elem& e)	// add to back of list
    { add(trailer, e); }


// POLYNOMIAL CLASS DEF

class Polynomial {
    public:
        void insertTerm(int c, int e);
        Polynomial& operator+(Polynomial& temp);
        friend ostream& operator<<(ostream &out, Polynomial& otherP);
        friend istream& operator>>(istream &in, Polynomial& otherP);
        
        double eval(int x);
    private:
        string toString();
        DLinkedList term_list;   
};


void Polynomial::insertTerm(int c, int e){
    int increment = 0;
    if (term_list.empty()){
        if (c != 0){
            term_list.addFront(Term(c,e));
        }
    }
    else{
        Elem temp(c,e);
        DNode* current = term_list.header->next;
        while(current->next != NULL){
            if (current->elem.exp == e){
                if (c != 0){
                    current->elem.coef = c;
                    
                }
                else{
                    term_list.remove(current);
                    
                }
                increment = 1;
                break;
                
            }
            else if (current == term_list.header->next && current->elem.exp < e ){
                if (c != 0 ){
                    term_list.addFront(temp);
                    increment = 1;
                }
                break;
            }

            else if (current->next->elem.exp < e && current->elem.exp > e ){
                if(c!=0 ){
                    term_list.add(current->next,temp);
                } 
                increment = 1;
                break;            
            }
            current = current->next;
        }
        if (increment == 0){
            if(c!=0){
            term_list.addBack(temp);
            }
        }
    }
}  


string Polynomial::toString(){
    DNode* current = term_list.header->next;
    string poly = " ";
     while(current->next != term_list.trailer){
        if(current->elem.coef == 1 && current->elem.exp != 0){
            poly += "X^";
            poly += to_string(current->elem.exp);
            poly += " + "; 
            current = current->next;
        }
        else if(current->elem.exp == 0){
            poly += to_string(current->elem.coef);
            poly += " + "; 
            current = current->next;
        }
        else{
            poly += to_string(current->elem.coef);
            poly += "X^";
            poly += to_string(current->elem.exp);
            poly += " + "; 
            current = current->next;
        } 
    }
    if(current->elem.coef == 1 && current->elem.exp != 0 ){
        poly += "X^";
        poly += to_string(current->elem.exp);
    }
    else if(current->elem.exp == 0 ){
        poly += to_string(current->elem.coef);
    }
    else{
        poly += to_string(current->elem.coef);
        poly += "X^";
        poly += to_string(current->elem.exp);
    }
    return poly;
} 

istream& operator>>(istream &in, Polynomial& otherP){
    int c; int e; int terms;
    cout << "Input total num of terms";
    cin >> terms;
    for(int i =0; i<(terms);i++){
        in >> c >> e;
        otherP.insertTerm(c,e);
    }    
    return in;  
    
}

ostream& operator<<(ostream &out, Polynomial& otherP){
    out << otherP.toString();
    return out;
}

Polynomial& Polynomial::operator+(Polynomial &temp){
    Polynomial* final = new Polynomial;
    DNode *t2 = temp.term_list.header->next;        
    DNode *t1 = term_list.header->next;           
    DNode *t2end = temp.term_list.trailer;        
    DNode *t1end = term_list.trailer;  

    while(true){
        if(t2->elem.exp == t1->elem.exp){
            int sum = t2->elem.coef + t1->elem.coef;
            final->insertTerm(sum, t2->elem.exp);
            t1 = t1->next; t2 = t2->next;
        }else if(t2->elem.exp > t1->elem.exp){
            final->insertTerm(t2->elem.coef, t2->elem.exp);
            t2 = t2 ->next;
        }else if(t2->elem.exp < t1->elem.exp){ 
            final->insertTerm(t1->elem.coef, t1->elem.exp);
            t1 = t1->next;
        }if(t1 == t1end && t2 == t2end){  
            return *final;
        }else if(t2 == t2end){             
            while(t1 != t1end){
                final->insertTerm(t1->elem.coef, t1->elem.exp);
                t1 = t1->next;
            }
            return *final; 
        }else if(t1 == t1end){               
            while(t2 != t2end){
                final->insertTerm(t2->elem.coef, t2->elem.exp);
                t2 = t2->next;
            }
            return *final;
        }
    }
} 
   
double Polynomial::eval(int x){
    DNode* current = term_list.header->next;
    double final = 0; double square = 0;
    while (current != term_list.trailer){
        square = pow(x,current->elem.exp);
        final += square*current->elem.coef;
        current = current->next;
    }
    return final;
}


int main( ) {
    
    Polynomial Poly1;
    Polynomial Poly2;

    
    Poly1.insertTerm(1,2);
    Poly1.insertTerm(0,3);
    Poly1.insertTerm(5,7);

    Poly2.insertTerm(1,2);
    Poly2.insertTerm(1,5);
    Poly2.insertTerm(0,3);
    Poly2.insertTerm(6,0);
    Poly2.insertTerm(-5,7);
   
    cout << "P1 =" << Poly1 << endl;
    cout << "P2 = " << Poly2 << endl;

    Polynomial final;
    final = Poly1 + Poly2;
    cout << "PSUM = " << final; 

    
     
    return EXIT_SUCCESS;


}
