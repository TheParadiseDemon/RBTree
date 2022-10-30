#include <iostream>
#include <queue>

struct TNode;
struct ListNode;
enum class Color;
class RBTree;
class ClassList;

enum class Color {
    Red = 0,
    Black = 1
};

struct Room{
    int number;
    char letter;
};

struct ListNode{
private:
    ListNode* next;
    Room room;
public:

    ListNode(){
        next = nullptr;
    };
    ListNode(char let, int num, ListNode* node){
        next = node;
        room.number = num;
        room.letter = let;
    };

    ~ListNode() = default;

    ListNode* getNext(){
        return this->next;
    }
    [[nodiscard]] int getRoomNumber() const{
        return this->room.number;
    }
    [[nodiscard]] char getRoomLetter() const{
        return this->room.letter;
    }

    void setRoomLetter(char let){
        this->room.letter = let;
    }
    void setRoomNumber(int num){
        this->room.number = num;
    }
    void setNext(ListNode* node){
        this->next = node;
    }
};

class ClassList{
private:
    ListNode* root;
public:

    ClassList(){
        this->root = new ListNode();
    }
    ClassList(char let, int num){
        this->root = new ListNode(let, num, nullptr);
    }
    ~ClassList() = default;


    ListNode* getRoot(){
        return this->root;
    }

    void setRoot(ListNode* node){
        this->root = node;
    }

    void RemoveOne(){
        auto* p = this->root;
        if (this->root->getNext())
        setRoot(p->getNext());
        delete(p);
    }
    void AddOne(){
        auto* p = new ListNode();
        p->setRoomLetter(this->root->getRoomLetter());
        p->setRoomNumber(this->root->getRoomNumber());
        p->setNext(this->root);
        this->setRoot(p);
    }

    int NodeNumber(){
        ListNode* p = this->root;
        int count = 0;
        while (p != nullptr){
            count++;
            p = p->getNext();
        }
        return count;
    }
};

struct TNode{
private:
    ClassList* room;
    TNode* parent;
    TNode* left;
    TNode* right;
    Color color;
public:

    TNode(char let, int num){
        color = static_cast<Color>(0);
        parent = nullptr;
        left = nullptr;
        right = nullptr;
        room = new ClassList();
        this->setRoom(let, num);
    }

    TNode(){
        room = new ClassList();
        color = static_cast<Color>(0);
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }

    ~TNode() = default;

    Color getColor(){
        return this->color;
    }
    TNode* getParent(){
        return this->parent;
    }
    TNode* getLeft(){
        return this->left;
    }
    TNode* getRight(){
        return this->right;
    }
    ClassList* getRoom(){
        return this->room;
    }
    int getRoomNumber(){
        return this->getRoom()->getRoot()->getRoomNumber();
    }
    char getRoomLetter(){
        return this->getRoom()->getRoot()->getRoomLetter();
    }

    void setColor(Color colour){
        this->color = colour;
    }
    void setParent(TNode* node){
        this->parent = node;
    }
    void setLeft(TNode* node){
        this->left = node;
    }
    void setRight(TNode* node){
        this->right = node;
    }
    void setRoom(char let, int num){
        if (this->getRoom()->getRoot() != nullptr) {
            this->getRoom()->getRoot()->setRoomLetter(let);
            this->getRoom()->getRoot()->setRoomNumber(num);
        }
    }
    void setRoom(ClassList* list){
        this->getRoom()->setRoot(list->getRoot());
    }

    void PrintPreOrder(){
        if (this->getRoomNumber() == -1) return;
        char col;
        col = this->getColor() == static_cast<Color>(0) ? 'R' : 'B';
        std::cout << this->getRoomLetter() << this->getRoomNumber() << col << this->getRoom()->NodeNumber() << "  ";
        this->getLeft()->PrintPreOrder();
        this->getRight()->PrintPreOrder();
    }

    explicit operator bool() const {
        return (this != nullptr);
    }

    friend bool operator==(TNode a, TNode b){
        if (!(bool)a && !(bool)b) return true;
        return (((bool)a == (bool)b) && ((bool)a.getRight() == (bool)b.getRight()) && ((bool)a.getLeft() == (bool)b.getLeft()));
    }

};

class RBTree {
private:
    TNode* Nil;
    TNode* root;
public:

    RBTree(){
        Nil = new TNode();
        Nil->setRoom('z', -1);
        Nil->setColor(static_cast<Color>(1));
        root = this->Nil;
    }
    ~RBTree() = default;

    TNode* getRoot(){
        return this->root;
    }
    TNode* getNil(){
        return this->Nil;
    }

    void setRoot(TNode* node){
        this->root = node;
    }

    TNode* Find(char let, int num){
        TNode* p = this->root;
        if (p == this->Nil){
            std::cout << "List is empty.\n";
            return nullptr;
        }
        while (let > p->getRoomLetter() && p->getRight() != this->Nil)
            p = p->getRight();

        while (let < p->getRoomLetter() && p->getLeft() != this->Nil)
            p = p->getLeft();

        while (num > p->getRoomNumber() && p->getRight() != this->Nil)
            p = p->getRight();

        while (num < p->getRoomNumber() && p->getLeft() != this->Nil)
            p = p->getLeft();

        if (p->getRoomNumber() == num && p->getRoomLetter() == num) return p;
        else {
            std::cout << "Such room hasn't been found.";
            return nullptr;
        }
    }

    void LeftRotate(TNode* x) {
        TNode *y = x->getRight();
        x->setRight(y->getLeft());
        if (y->getLeft())
            y->getLeft()->setParent(x);
        y->setParent(x->getParent());
        if (x->getParent() == this->Nil)
            this->setRoot(y);
        else if (x == x->getParent()->getLeft())
            x->getParent()->setLeft(y);
        else
            x->getParent()->setRight(y);
        y->setLeft(x);
        x->setParent(y);
    }

    void RightRotate(TNode* x) {
        TNode *y = x->getLeft();
        x->setLeft(y->getRight());
        if (y->getRight())
            y->getRight()->setParent(x);
        y->setParent(x->getParent());
        if (x->getParent() == this->Nil)
            this->setRoot(y);
        else if (x == x->getParent()->getRight())
            x->getParent()->setRight(y);
        else
            x->getParent()->setLeft(y);
        y->setRight(x);
        x->setParent(y);
    }

    void RBInsert(TNode*& z){
        TNode* x = this->root;
        TNode* y = this->Nil;
        while (x != this->Nil && (z->getRoomNumber() != x->getRoomNumber() || z->getRoomLetter() != x->getRoomLetter())){
            y = x;
            if ((z->getRoomNumber() < x->getRoomNumber() && z->getRoomLetter() == x->getRoomLetter())||
            (z->getRoomLetter() < x->getRoomLetter()))
                x = x->getLeft();
            else x = x->getRight();
        }
        if (z->getRoomNumber() == x->getRoomNumber() && z->getRoomLetter() == x->getRoomLetter())
            x->getRoom()->AddOne();
        else{
        z->setParent(y);
        if (y == this->Nil)
            this->root = z;
        else if (z->getRoomNumber() < y->getRoomNumber() ||
                 (z->getRoomNumber() == y->getRoomNumber() && z->getRoomLetter() < y->getRoomLetter()))
            y->setLeft(z);
        else  y->setRight(z);
        z->setLeft(this->Nil);
        z->setRight(this->Nil);
        RBInsertFixup(z);
        }
    }

    void RBInsert(char let, int num){
        TNode* node = new TNode(let, num);
        this->RBInsert(node);
    }

    void RBInsertFixup(TNode*& z){
        while (z->getParent()->getColor() == static_cast<Color>(0)) {
            if (z->getParent() == z->getParent()->getParent()->getLeft()) {
                TNode *y = z->getParent()->getParent()->getRight();
                if (y->getColor() == static_cast<Color>(0)){
                    z->getParent()->setColor(static_cast<Color>(1));
                    y->setColor(static_cast<Color>(1));
                    z->getParent()->getParent()->setColor(static_cast<Color>(0));
                    z = z->getParent()->getParent();
                }
                else {
                    if (z == z->getParent()->getRight()) {
                        z = z->getParent();
                        LeftRotate(z);
                    }
                    z->getParent()->setColor(static_cast<Color>(1));
                    z->getParent()->getParent()->setColor(static_cast<Color>(0));
                    RightRotate(z->getParent()->getParent());
                    }
                }
            else {
                TNode *y = z->getParent()->getParent()->getLeft();
                if (y->getColor() == static_cast<Color>(0)){
                    z->getParent()->setColor(static_cast<Color>(1));
                    y->setColor(static_cast<Color>(1));
                    z->getParent()->getParent()->setColor(static_cast<Color>(0));
                    z = z->getParent()->getParent();
                }
                else {
                    if (z == z->getParent()->getLeft()) {
                        z = z->getParent();
                        RightRotate(z);
                    }
                    z->getParent()->setColor(static_cast<Color>(1));
                    z->getParent()->getParent()->setColor(static_cast<Color>(0));
                    LeftRotate(z->getParent()->getParent());
                }
            }
        }
        this->root->setColor(static_cast<Color>(1));
     }

     void RBTransplant(TNode* u, TNode* v){
        if (u->getParent() == this->Nil)
            this->root = v;
        else if (u == u->getParent()->getLeft())
            u->getParent()->setLeft(v);
        else u->getParent()->setRight(v);
        v->setParent(u->getParent());
    }

    TNode* TreeMin(TNode* x){
        while (x->getLeft() != this->Nil)
            x = x->getLeft();
        return x;
    }

    TNode* TreeMax(TNode* x){
        while (x->getRight() != this->Nil)
            x = x->getRight();
        return x;
    }

    void RBDelete(TNode*& z){
        TNode* y = z;
        TNode* x;
        Color yOriginColor = y->getColor();
        if (z->getLeft() == this->Nil){
            x = z->getRight();
            RBTransplant(z, z->getRight());
        }
        else if (z->getRight() == this->Nil){
            x = z->getLeft();
            RBTransplant(z, z->getLeft());
        }
        else {
            y = TreeMin(z->getRight());
            yOriginColor = y->getColor();
            x = y->getRight();
            if (y->getParent() == z)
                x->setParent(z);
            else {
                RBTransplant(y, y->getRight());
                y->setRight(z->getRight());
                y->getRight()->setParent(y);
            }
            RBTransplant(z, y);
            y->setLeft(z->getLeft());
            y->getLeft()->setParent(y);
            y->setColor(z->getColor());
        }
        if (yOriginColor == static_cast<Color>(1))
            RBDeleteFixup(x);
    }

    void RBDeleteFixup(TNode* x){
        while (x != this->root && x->getColor() != static_cast<Color>(1)){
            if (x == x->getParent()->getLeft()){
                TNode* w = x->getParent()->getRight();
                if (w->getColor() == static_cast<Color>(0)) {
                    w->setColor(static_cast<Color>(1));
                    x->getParent()->setColor(static_cast<Color>(0));
                    LeftRotate(x->getParent());
                    w = x->getParent()->getRight();
                    if (w->getLeft()->getColor() == static_cast<Color>(1) &&
                        w->getRight()->getColor() == static_cast<Color>(1)) {
                        w->setColor(static_cast<Color>(0));
                        x = x->getParent();
                    } else if (w->getRight()->getColor() == static_cast<Color>(1)) {
                        w->getLeft()->setColor(static_cast<Color>(1));
                        w->setColor(static_cast<Color>(0));
                        RightRotate(w);
                        w = x->getParent()->getRight();
                    }
                    w->setColor(x->getParent()->getColor());
                    x->getParent()->setColor(static_cast<Color>(1));
                    w->getRight()->setColor(static_cast<Color>(1));
                    LeftRotate(x->getParent());
                    x = this->root;
                }
            }
            else {
                TNode* w = x->getParent()->getRight();
                if (w->getColor() == static_cast<Color>(0)) {
                    w->setColor(static_cast<Color>(1));
                    x->getParent()->setColor(static_cast<Color>(0));
                    RightRotate(x->getParent());
                    w = x->getParent()->getLeft();
                    if (w->getRight()->getColor() == static_cast<Color>(1) &&
                        w->getLeft()->getColor() == static_cast<Color>(1)) {
                        w->setColor(static_cast<Color>(0));
                        x = x->getParent();
                    } else if (w->getLeft()->getColor() == static_cast<Color>(1)) {
                        w->getRight()->setColor(static_cast<Color>(1));
                        w->setColor(static_cast<Color>(0));
                        LeftRotate(w);
                        w = x->getParent()->getLeft();
                    }
                    w->setColor(x->getParent()->getColor());
                    x->getParent()->setColor(static_cast<Color>(1));
                    w->getLeft()->setColor(static_cast<Color>(1));
                    RightRotate(x->getParent());
                    x = this->root;
                }
            }
        }
    }

    void printLevelOrder()
    {
        if (this->root == nullptr)
            return;
        std::queue<TNode*> q;
        q.push(this->root);

        while (!q.empty()) {
            int nodeCount = q.size();
            while (nodeCount > 0) {
                TNode* node = q.front();
                std::cout << node->getRoomLetter() << node->getRoomNumber();
                if (node->getColor() == static_cast<Color>(0))
                    std::cout << "R";
                else std::cout << "B";
                std::cout << node->getRoom()->NodeNumber() << "   ";
                q.pop();
                if (node->getLeft() != this->Nil)
                    q.push(node->getLeft());
                if (node->getRight() != this->Nil)
                    q.push(node->getRight());
                nodeCount--;
            }
            std::cout << std::endl;
        }
    }

    void PrintPreOrder(){
        if (this->root == nullptr) {
            std::cout<<"List is empty.\n";
            return;
        }
        this->root->PrintPreOrder();
    }

    friend bool operator== (RBTree a, RBTree b){
        TNode* roota = a.getRoot();
        TNode* rootb = b.getRoot();
        return (roota == rootb);
    }

};

int main() {

    RBTree* tree = new RBTree();
    TNode* node = new TNode('A', 100);
    char let;
    int num = 0;

    tree->RBInsert(node);
    /*while (true){
        std::cin >> let >> num;
        if (num == -1) break;
        node = new TNode(let, num);
        tree->RBInsert(node);
    }*/
    tree->RBInsert('A', 100);
    RBTree* tree1 = new RBTree();
    node = new TNode('a', 101);
    tree1->RBInsert(node);
    if (tree == tree1) std::cout <<"yes\n";
    tree->printLevelOrder();
    return 0;
}
