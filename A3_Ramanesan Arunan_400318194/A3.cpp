//Ramanesan Arunan 
//400318194
#include <list>
#include <queue>
#include <iostream> 
using std::queue;
using namespace std;
queue<int> myqueue;

typedef int Elem;					                        // base element type

struct Node {					                            // a node of the tree
    Elem* k_list;
    Elem  k_list_size;

    Elem    elt;					                        // element value
    Node*   par;					                        // parent
    Node*   left;					                        // left child
    Node*   right;					                        // right child
    Node() : elt(), par(NULL), left(NULL), right(NULL), k_list(NULL), k_list_size(0) { }  // constructor

};


class Position {	 				                        // position in the tree
    private:
        Node* v;						                    // pointer to the node
    public:
        Position(Node* _v = NULL) : v(_v) { }		        // constructor
        Elem& operator*()					                // get element2
            { return v->elt; }
        Position left() const				                // get left child
            { return Position(v->left); }
        Position right() const				                // get right child
            { return Position(v->right); }
        Position parent() const				                // get parent
            { return Position(v->par); }
        bool isRoot() const				                    // root of the tree?
            { return v->par == NULL; }
        bool isNull() const				                    // root of the tree?
            { return v == NULL; }
        bool isExternal() const				                // an external node?
            { return v->left == NULL && v->right == NULL; }
        bool operator==(const Position& p) const	        // are equal?
            { return v == p.v; }
        bool operator!=(const Position& p) const			// are not equal?
            { return v != p.v; }
        Elem& getElem()	const				                // get element
            { return v->elt; }
        void k_list_setter(int k){
           v->k_list_size += 1;
            Elem* list = new Elem[v->k_list_size];
            for (int i = 0 ; i < (v->k_list_size-1);i++){
                list[i] = v->k_list[i];
            }
            list[v->k_list_size-1] = k;
            delete[] v->k_list;
            v->k_list = list;         
        }
        void k_list_getter (){
            for (int i = 0 ; i < v->k_list_size;i++){
                cout << " " << v->k_list[i];
            }
        }
        bool node_exist(){
            return v->k_list != NULL;
        }      
   
    friend class BinaryTree;			                    // give tree access
};

typedef list<Position> PositionList;                        // list of positions

class BinaryTree {
    public:
        BinaryTree();					                    // constructor
        int size() const;					                // number of nodes
        bool empty() const;					                // is tree empty?
        Position root() const;				                // get the root
        PositionList positions() const;  			        // list of nodes
        void addRoot();					                    // add root to empty tree
        void expandExternal(const Position& p);		        // expand external node
        Position removeAboveExternal(const Position& p);	// remove p and parent
        // housekeeping functions omitted...
    protected: 						                        // local utilities
        void preorder(const Position& v, PositionList& pl) const;	// preorder utility
    private:
        Node* _root;					                    // pointer to the root
        int n;						                        // number of nodes
};

BinaryTree::BinaryTree()			                        // constructor
    : _root(NULL), n(0) { }

int BinaryTree::size() const			                    // number of nodes
    { return n; }

bool BinaryTree::empty() const			                    // is tree empty?
    { return size() == 0; }

Position BinaryTree::root() const                           // get the root
    { return Position(_root); }

void BinaryTree::addRoot()			                        // add root to empty tree
    { _root = new Node; n = 1; }

void BinaryTree::expandExternal(const Position& p) {        // expand external node
    Node* v = p.v;					                        // p's node
    v->left = new Node;					                    // add a new left child
    v->left->par = v;					                    // v is its parent
    v->right = new Node;				                    // and a new right child
    v->right->par = v;					                    // v is its parent
    n += 2;						                            // two more nodes
}

PositionList BinaryTree::positions() const {                // list of all nodes
    PositionList pl;
    preorder(root(), pl);					                // preorder traversal
    return PositionList(pl);				                // return resulting list
}

void BinaryTree::preorder(const Position& v, PositionList& pl) const {  // preorder traversal
    pl.push_back(Position(v));				                // add this node
    if (!v.left().isExternal())					            // traverse left subtree
        preorder(v.left(), pl);
    if (!v.right().isExternal())					        // traverse right subtree
        preorder(v.right(), pl);
}

Position BinaryTree::removeAboveExternal(const Position& p) {   // this is needed for removal
    Node* w = p.v;  Node* v = w->par;			                // get p's node and parent
    Node* sib = (w == v->left ?  v->right : v->left);
    if (v == _root) {					                        // child of root?
      _root = sib;					                            // ...make sibling root
      sib->par = NULL;
    }
    else {
      Node* gpar = v->par;				                        // w's grandparent
      if (v == gpar->left) gpar->left = sib; 		            // replace parent by sib
      else gpar->right = sib;
      sib->par = gpar;
    }
    delete w; delete v;					                        // delete removed nodes
    n -= 2;						                                // two fewer nodes
    return Position(sib);
}

class RuntimeException {  
    private:
        string errorMsg; 
    public:
        RuntimeException(const string& err)  
        {
            errorMsg = err; 
        }
        string getMessage() const {  
            return errorMsg; 
        }
};

class NonExistentElement : public RuntimeException {  
    public:
        NonExistentElement(const string& err) : RuntimeException(err) {}
};

class SearchTree {					                            // a binary search tree
    public: 						                            // public types
        class Iterator {	                      		        // an iterator/position
            private:
                Position v;						                // which entry
            public:
                Iterator(const Position& vv) : v(vv) { }		// constructor
                Elem& operator*() { return *v; }			    // get entry
                bool operator==(const Iterator& p) const		// are iterators equal?
                    { return v == p.v; }
                bool operator!=(const Iterator& p) const		// are iterators not equal?
                    { return v != p.v; }
                Iterator& operator++();				            // inorder successor
                Iterator nextInPostorder();            
                Iterator nextInLevelorder();
            friend class SearchTree;				            // give search tree access
        };
    public:						                                // public functions
        SearchTree();					                        // constructor
        int size() const; 					                    // number of entries
        bool empty() const;					                    // is the tree empty?
        SearchTree::Iterator find(const Elem& k);				// find entry with key k
        SearchTree::Iterator insert(const Elem& k, const Elem& x);		// insert (k,x)
        void erase(const Elem& k); //throw(NonexistentElement);	// remove key k entry
        void erase( SearchTree::Iterator& p);			        // remove entry at p
        SearchTree::Iterator begin();					        // iterator to first entry
        SearchTree::Iterator end();					            // iterator to end entry

/****************** ASSIGNMENT DEFINITION OF FUNCTIONS **********************/
        SearchTree::Iterator min();					      
        SearchTree::Iterator max();
        int rank(Elem key);                                 // get the rank of a key in the tree
        SearchTree::Iterator floor(Elem key);               // get the floor of a key
        SearchTree::Iterator ceil(Elem key);                // get the ceiling of a key
        void eraseMin();
        void eraseMax();
        SearchTree::Iterator selectAtRank(Elem key);
        int countKeysBetween(Elem l, Elem h);
        PositionList entriesBetween(Elem l, Elem h);
        SearchTree::Iterator median();
        int height();
        int depth(const SearchTree::Iterator& p);


        
        PositionList getPositionList();                         // get the positions of the binary tree

    protected:						                            // local utilities
        Position root() const;					                // get virtual root
        Position finder(const Elem& k, const Position& v);		// find utility
        Position inserter(const Elem& k, const Elem& x);		// insert utility
        Position eraser(Position& w);				            // erase utility
        int _height(const Position& v);                         //height utility
        
        
    private: 						                            // member data
        BinaryTree T;					                        // the binary tree
        int n;						                            // number of entries
        
};

//min key finder
SearchTree::Iterator SearchTree::min(){
    return begin();
}

//max key finder
SearchTree::Iterator SearchTree::max(){
    Iterator max = begin();
    for(Iterator it = begin(); it != end(); ++it){
        max = it;
    }
    return max;
}

//min key eraser
void SearchTree::eraseMin(){
    Iterator temp = min();
    erase(temp);
} 

//max key eraser
void SearchTree::eraseMax(){
    Iterator temp = max();
    erase(temp);
}

//floor() with exception
SearchTree::Iterator SearchTree::floor(Elem key){
    Iterator final = begin();
    if(empty()){
        throw NonExistentElement ("floor() is called for empty tree");
    }
    else if(key < *min()){
        throw NonExistentElement ("floor() is called for a key smaller than smallest key");
    }
    else{
        for(Iterator it = begin(); it != end(); ++it){
            if(*it <= key){
                final = it;
            }
        }
    return final;
    }
    
}

//ciel() with exception
SearchTree::Iterator SearchTree::ceil(Elem key){
    Iterator final = begin();
    if(empty()){
        throw NonExistentElement ("ceil() is called for empty tree");
    }
    else if(key > *max()){
        throw NonExistentElement ("ceil() is called for a key larger than maximum key");
    }
    else{
        for(Iterator it = begin(); it != end(); ++it){
            if(*it >= key){
                final = it;
                break;
            }
        }
    return final;
    }   
}

//checks for the rank of the element
int SearchTree::rank(Elem key){
    int temp = 0;
    Iterator check = begin();
    // if empty the rank is 0
    if(empty()){
        return 0;
    }
    // if not empty just iterate until the value reaches the key 
    else{
        for(Iterator it = begin(); it != end(); ++it){
            if(*it < key){
                temp++;
            }
            else{
                check = it;
                break;
            } 
        }
        if(*check != key){
            throw NonExistentElement("key is not in this tree");
        }
        else{
            return temp;
        }
    }    
}

//returns iterator to a node with given rank
SearchTree::Iterator SearchTree::selectAtRank(Elem key){
    Iterator final = begin(); int temp = 0;

    if(( key >= n) || (key < 0)) { throw NonExistentElement("Invalid Rank");}

    for(Iterator it = begin(); it != end(); ++it){
        if(temp <= key){
            final = it;
            temp++;            
        }
    }
    return final;
       
} 


//returns number of keys between 2 given integers inclusive
int SearchTree::countKeysBetween(Elem l, Elem h){
    int temp = 0;
    for(Iterator it = begin(); it != end(); ++it){
        if((*it >= l) && (*it <= h)){
            temp++;            
            }
        }
    return temp;
}

//calculate number of entries between 2 numbers
PositionList SearchTree::entriesBetween(Elem l, Elem h){
    queue<Position> q1;
    PositionList final;
    int i = 0;
    for(Iterator it = begin(); it != end(); ++it){
        if((*it >= l) && (*it <= h)){
            q1.push(it.v);           
        }
    }
    while (!q1.empty()){
        final.push_back(q1.front());
        q1.pop();
        i++;
    }

    return final;
} 


SearchTree::Iterator SearchTree::Iterator::nextInPostorder(){
    SearchTree::Iterator root = v;

    if(v == NULL){ throw NonExistentElement("Empty tree");}

     while(!root.v.isRoot()){
        root = root.v.parent();
    }
    root = root.v.left();

    if(*root == *v) { throw NonExistentElement("End of Tree");}
    
    SearchTree::Iterator left = v.parent().left();
    SearchTree::Iterator right = v.parent().right();
    SearchTree::Iterator curr = v;
    if(right == curr){
        //cout << "ran the right";
        SearchTree::Iterator final = v.parent();
        return final;
    }
    else{
        SearchTree::Iterator final = v.parent();
        bool check = v.parent().right().isExternal();
        //cout << "\nthis check: "<< check;
        if(check){
           return final;
        }
        else{
            Position w = v.parent().right();
            while (w.isExternal()) {				                        
                w = w.left();
            }
            final = w;
            return final;
        }
    }
}
    

SearchTree::Iterator SearchTree::Iterator::nextInLevelorder(){
    if(v == NULL) { throw NonExistentElement("Tree is empty"); }

    queue<Iterator> q1;
    Iterator root = v;  Iterator x = v;
    while(!root.v.isRoot()){
        root = root.v.parent();
    }
    root = root.v.left(); q1.push(root);

    while((!q1.empty()) && (x.v!=NULL)){
        x = q1.front();
        q1.pop();
        if((!x.v.left().isNull()) &&(x.v.left().node_exist() != 0)){
                q1.push(x.v.left());
        }
        if((!x.v.right().isNull())&&(x.v.right().node_exist() != 0)){
                q1.push(x.v.right());
        }
        if(x.v == v){
            if(q1.empty()){
                throw NonExistentElement("Reached the End");
            }
            break;
        } 
    }
    return q1.front();
    throw NonExistentElement("Reached the End");
} 

//median of the tree, i.e. element at Rank(size/2)
SearchTree::Iterator SearchTree::median(){
    int n = 0; int temp = 0;
    n = size();
    int m = n/2; 
    Iterator final = begin();
    for(Iterator it = begin(); it != end(); ++it){
        if(temp <= m){
            final = it;
            temp ++;
        }
    }
    return final;
}

//finding height of tree
int SearchTree::height(){
    Iterator root = end();
    if (root.v.isExternal()){
        throw NonExistentElement("Tree is Empty");
    }
   return _height(end().v.left());
}


//helper function for height recursion
int SearchTree::_height(const Position& v){
    int L; int R;
    if(v.isExternal()){
        return -1;
    }
    L = _height(v.left());
    R = _height(v.right());
    if(L > R){
        return (L+1);
    }
    else{
        return (R+1);
    }
}

int SearchTree::depth(const SearchTree::Iterator& p){
    int final = 0;
    Position temp = p.v;
    while(!temp.isRoot()){
        temp = temp.parent();
        final ++;
    }
    return (final-1);
}










SearchTree::SearchTree() : T(), n(0)                            // constructor
    { T.addRoot(); T.expandExternal(T.root()); }	            // create the super root
  
Position SearchTree::root() const                               // get virtual root
    { return T.root().left(); }				                    // left child of super root

int SearchTree::size() const                                    // get the size
    { return n; }

bool SearchTree::empty() const			                        // is search tree empty?
    { return size() == 0; }

SearchTree::Iterator SearchTree::begin() {                      // iterator to first entry
    Position v = root();					                    // start at virtual root
    while (!v.isExternal()) v = v.left();		                // find leftmost node
    return Iterator(v.parent());
}


  		
SearchTree::Iterator SearchTree::end()                          // iterator to end entry
    { return Iterator(T.root()); }			                    // return the super root

PositionList SearchTree::getPositionList(){                     // ge the positions of the binary tree
    return T.positions();
}
				
SearchTree::Iterator& SearchTree::Iterator::operator++() {      // inorder successor
    Position w = v.right();
    if (!w.isExternal()) {				                        // have right subtree?
      do { v = w; w = w.left(); }			                    // move down left chain
      while (!w.isExternal());
    }
    else {
      w = v.parent();					                        // get parent
      while (v == w.right())				                    // move up right chain
        { v = w; w = w.parent(); }
      v = w;						                            // and first link to left
    }
    return *this;
}
	
Position SearchTree::finder(const Elem& k, const Position& v) { // find utility
    if (v.isExternal()) return v;			                    // key not found
    if (k < v.getElem()) return finder(k, v.left());	        // search left subtree
    else if (v.getElem() < k) return finder(k, v.right());	    // search right subtree
    else return v;					                            // found it here
}
			
SearchTree::Iterator SearchTree::find(const Elem& k) {          // find entry with key k
    Position v = finder(k, root());				                // search from virtual root
    if (!v.isExternal()) return Iterator(v);		            // found it
    else return end();					                        // didn't find it
}
					
Position SearchTree::inserter(const Elem& k, const Elem& x) {   // insert utility
    Position v = finder(k, root());	;		                // search from virtual root
    while (!v.isExternal()){				                        // key already exists
      if(k == v.getElem()){
        v.k_list_setter(x);
        return v;
      }
      else{
         v = finder(k, v.right());				                    // look further 
      }      
    }  
    T.expandExternal(v);				                        // add new internal node
    *v = k;                                                     // set the key!
    v.k_list_setter(x);
    n++;						                                // one more entry
    return v;						                            // return insert position
}

SearchTree::Iterator SearchTree::insert(const Elem& k, const Elem& x)   // insert (k,x)
    { Position v = inserter(k, x); return Iterator(v); }

Position SearchTree::eraser(Position& w) {                          // remove utility
    Position z;
    if (w.left().isExternal()) z = w.left();		                // remove from left case 1
    else if (w.right().isExternal()) z = w.right();	                // remove from right case 1
    else {						                                    // both internal? case 2
      z = w.right();					                            // go to right subtree
      do { z = z.left(); } while (!z.isExternal());	                // get leftmost node
      Position v = z.parent();
      //w->setKey(v->key()); w->setValue(v->value());	            // copy z's parent to w
      *w = v.getElem();	                                            // copy z's parent to v
    }
    n--;						                                    // one less entry
    return T.removeAboveExternal(z);			                    // remove z and its parent
}
					
void SearchTree::erase(const Elem& k){ //throw(NonexistentElement) { // remove key k entry
    Position w = finder(k, root());				                    // search from virtual root
    // if (v.isExternal())					                        // not found?
    //   throw NonexistentElement("Erase of nonexistent");
    eraser(w);						                                // remove it
}
	
void SearchTree::erase( Iterator& p)                                // erase entry at p
    { eraser(p.v); }


int main()
{
    cout << "Hi " << endl;

    SearchTree t;
    t.insert(23, 8);
    t.insert(18, 8);
    t.insert(4, 8);
    t.insert(5, 8);
    t.insert(40, 8);
    t.insert(3,8);
    t.insert(15,8);
 
     
    cout << t.size() << endl;

    t.erase(15);

    cout << t.size() << endl;

    //list<Position> res = t.positions();
    int final;
    typedef SearchTree::Iterator It;
    //typedef list<int>::iterator ITList;
    for(It myit = t.begin() ; myit != t.end() ; ++myit){
            cout << "\n" << *myit << " depth:" << t.depth(myit) ;
    }
    cout << "\n-------this is min:" << *t.min(); 
    cout << "\n-------this is max:" << *t.max(); 
    cout << "\nfloor:" << *t.floor(23);
    cout << "\nceil:" << *t.ceil(24);
    cout << "\nrank:" << t.rank(23) ;
    cout << "\nselect: " << *t.selectAtRank(3);
    cout << "\nrangeofvals:" << t.countKeysBetween(3,18) << endl;
    cout << "\nheight: " << t.height();

    

    t.eraseMax();
    t.eraseMin();
    cout << "\n-------this is new min: " << *t.min(); 
    cout << "\n-------this is new max: " << *t.max();  

    PositionList x;
    x = t.entriesBetween(4,21);
    cout << "Entires between :";
    for(int i = 0; i < t.countKeysBetween(4,21) ;i++){
        cout << " " << *x.front();
        x.pop_front();
    }  
    


    cout << "\nthe median:" << *t.median() << endl;
 
     list<Position> a = t.getPositionList();
    cout << a.front().getElem() << " -- " <<  a.size() << endl;

    cout << " -- " << endl;
    cout << "-------------------------" << *t.median() << endl; 
    cout << *t.max() << endl;

    cout << *t.floor(2) << endl;
}