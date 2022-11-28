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
        color = Color::Red;
        parent = nullptr;
        left = nullptr;
        right = nullptr;
        room = new ClassList();
        this->setRoom(let, num);
    }

    TNode(){
        room = new ClassList();
        color = Color::Red;
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }

    ~TNode() {
        ListNode* ptr = this->getRoom()->getRoot();
        ListNode* temp = ptr;
        while (ptr != nullptr){
            ptr = ptr->getNext();
            delete (temp);
            temp = ptr;
        }
    };

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


    void DeleteSubTree(TNode* Nil){
        if (this != Nil){
            this->getLeft()->DeleteSubTree(Nil);
            this->getRight()->DeleteSubTree(Nil);
            this->setLeft(nullptr);
            this->setRight(nullptr);
            delete (this);
        }

    }

    void PrintOnSide(int h, TNode* Nil){
        if (this != Nil){
            this->getRight()->PrintOnSide(h + 4, Nil);
            for (int i = 0; i < h; ++i) std::cout << ' ';
            char col = this->getColor() == Color::Red ? 'R' : 'B';
            std::cout << this->getRoomLetter() << this->getRoomNumber() << col << this->getRoom()->NodeNumber() << std::endl;
            this->getLeft()->PrintOnSide(h + 4, Nil);
        }
    }

    void invertColor(){
        if (this->getColor() == Color::Red)
            this->setColor(Color::Black);
        else this->setColor(Color::Red);
    }

    TNode* getBrother() {
        if (this->getParent()->getRoomNumber() != -1)
            if (this == this->getParent()->getRight()) return this->getParent()->getLeft();
            else return this->getParent()->getRight();
        else return nullptr;
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
        Nil->setColor(Color::Black);
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
        while ((p->getRoomNumber() != num || p->getRoomLetter() != let)&&(p != this->Nil)){
            switch((p->getRoomNumber() < num) + 5*(p->getRoomNumber() > num) + 10*(p->getRoomLetter() < let) + 20*(p->getRoomLetter() > let)){
                case 1:{
                   p = p->getRight();
                   break;
                };
                case 5:{
                    p = p->getLeft();
                    break;
                };
                case 10 ... 15:{
                    p = p->getRight();
                    break;
                };
                case 20 ... 25:{
                    p = p->getLeft();
                    break;
                };
            };
        }
        if (p->getRoomNumber() == num && p->getRoomLetter() == let) return p;
        else {
            std::cout << "Such room hasn't been found.\n";
            return nullptr;
        }
    }

    void LeftRotate(TNode* x) {
        if (x->getRight() != this->Nil) {
            TNode *y = x->getRight();
            x->setRight(y->getLeft());
            if (y->getLeft() != this->Nil)
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
    }

    void RightRotate(TNode* x) {
        if (x->getLeft() != this->Nil) {
            TNode *y = x->getLeft();
            x->setLeft(y->getRight());
            if (y->getRight() != this->Nil)
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
        while (z->getParent()->getColor() == Color::Red) {
            if (z->getParent() == z->getParent()->getParent()->getLeft()) {
                TNode *y = z->getParent()->getParent()->getRight();
                if (y->getColor() == Color::Red){
                    z->getParent()->setColor(Color::Black);
                    y->setColor(Color::Black);
                    z->getParent()->getParent()->setColor(Color::Red);
                    z = z->getParent()->getParent();
                }
                else {
                    if (z == z->getParent()->getRight()) {
                        z = z->getParent();
                        LeftRotate(z);
                    }
                    z->getParent()->setColor(Color::Black);
                    z->getParent()->getParent()->setColor(Color::Red);
                    RightRotate(z->getParent()->getParent());
                    }
                }
            else {
                TNode *y = z->getParent()->getParent()->getLeft();
                if (y->getColor() == Color::Red){
                    z->getParent()->setColor(Color::Black);
                    y->setColor(Color::Black);
                    z->getParent()->getParent()->setColor(Color::Red);
                    z = z->getParent()->getParent();
                }
                else {
                    if (z == z->getParent()->getLeft()) {
                        z = z->getParent();
                        RightRotate(z);
                    }
                    z->getParent()->setColor(Color::Black);
                    z->getParent()->getParent()->setColor(Color::Red);
                    LeftRotate(z->getParent()->getParent());
                }
            }
        }
        this->root->setColor(Color::Black);
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

    void RBDelete(TNode* z){
        if (this->Find(z->getRoomLetter(), z->getRoomNumber())->getRoom()->NodeNumber() > 1)
            this->Find(z->getRoomLetter(), z->getRoomNumber())->getRoom()->RemoveOne();
        else {
            if (z == nullptr) return;
            TNode *y = z;
            TNode *x;
            Color yOriginColor = y->getColor();
            if (z->getLeft() == this->Nil) {
                x = z->getRight();
                RBTransplant(z, z->getRight());
            } else if (z->getRight() == this->Nil) {
                x = z->getLeft();
                RBTransplant(z, z->getLeft());
            } else {
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
            if (yOriginColor == Color::Black)
                RBDeleteFixup(x);
        }
    }

    void RBDelete(char let, int num){
        TNode* z = this->Find(let, num);
        this->RBDelete(z);
    }

    void RBDeleteFixup(TNode* x) {
        while (x != this->root && x->getColor() == Color::Black) {
            if (x == x->getParent()->getLeft()) {
                if (x->getBrother()->getColor() == Color::Red) {
                    x->getBrother()->invertColor();
                    x->getParent()->invertColor();
                    LeftRotate(x->getParent());
                }
                    if (x->getBrother()->getLeft()->getColor() == Color::Black &&
                        x->getBrother()->getRight()->getColor() == Color::Black) {
                        x->getBrother()->invertColor();
                        x = x->getParent();
                    } else if (x->getBrother()->getRight()->getColor() == Color::Black) {
                        x->getBrother()->getLeft()->invertColor();
                        x->getBrother()->invertColor();
                        RightRotate(x->getBrother());
                    }
                    else {
                        x->getBrother()->setColor(x->getParent()->getColor());
                        if (x->getBrother()->getLeft()->getColor() != Color::Red &&
                        x->getParent()->getParent()->getColor() != Color::Black)
                        x->getParent()->invertColor();
                        x->getBrother()->getRight()->invertColor();
                        LeftRotate(x->getParent());
                        x = this->root;
                    }
            }
            else {
                if (x->getBrother()->getColor() == Color::Red) {
                    x->getBrother()->invertColor();
                    x->getParent()->invertColor();
                    RightRotate(x->getParent());
                }
                if (x->getBrother()->getRight()->getColor() == Color::Black &&
                    x->getBrother()->getLeft()->getColor() == Color::Black) {
                    x->getBrother()->invertColor();
                    x = x->getParent();
                } else if (x->getBrother()->getLeft()->getColor() == Color::Black) {
                    x->getBrother()->getRight()->invertColor();
                    x->getBrother()->invertColor();
                    LeftRotate(x->getBrother());
                }
                else {
                    x->getBrother()->setColor(x->getParent()->getColor());
                    if (x->getBrother()->getRight()->getColor() != Color::Red &&
                        x->getParent()->getParent()->getColor() != Color::Black)
                    x->getParent()->invertColor();
                    x->getBrother()->getLeft()->invertColor();
                    RightRotate(x->getParent());
                    x = this->root;
                }
            }
        }
        x->setColor(Color::Black);
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
                if (node->getColor() == Color::Red)
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

    inline bool operator==(RBTree a){
        TNode* roota = a.getRoot();
        TNode* currenta = roota;
        TNode* current = root;
        TNode* aNil = a.getNil();
        bool check = true;
        if (roota == aNil && root == Nil) return true;
        else if ((roota == aNil && root != Nil)||(roota != aNil && root == Nil)) return false;
        else{
            do{
                if (currenta->getLeft() != aNil || current->getLeft() != Nil){
                    currenta = currenta->getLeft();
                    current = current->getLeft();
                }
                else{
                    if (currenta->getRight() != aNil || current->getRight() != Nil){
                        currenta = currenta->getRight();
                        current = current->getRight();
                    }
                    else{
                       if (current == current->getParent()->getLeft()){
                           current = current->getParent()->getRight();
                           currenta = currenta->getParent()->getRight();
                       }
                       else {
                           while (current == current->getParent()->getRight() && current != root) {
                               current = current->getParent();
                               currenta = currenta->getParent();
                           }
                           if (current != root){
                               current = current->getParent()->getRight();
                               currenta = currenta->getParent()->getRight();
                           }
                       }
                    }
                }
                if ((current == Nil && currenta != aNil)||(current != Nil && currenta == aNil))
                    check = false;
            }
            while (currenta != roota && current != root && check);
        }
        return check;
    }

    void RBDeleteTree(){
        if (this->root != nullptr)
        {
            this->root->DeleteSubTree(this->Nil);
            this->setRoot(this->Nil);
        }
    }

    void PrinOnSide(int h){
        if (this->root != this->Nil)
            this->root->PrintOnSide(h, this->Nil);
    }

};

int main() {

    RBTree* tree = new RBTree();
    RBTree* tree1 = new RBTree();


    return 0;
}
