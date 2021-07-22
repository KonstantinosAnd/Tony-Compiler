#ifndef __AST_HPP___
#define __AST_HPP___

#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>


class AST{
public:
  virtual ~AST() {}
  virtual void printOn(std::ostream &out) const = 0;
  virtual void compile() const = 0;
};

inline std::ostream& operator<< (std::ostream &out, const AST &t) {
  t.printOn(out);
  return out;
}

// Virtual to avoid "diamond of death"
class Expr: virtual public AST {
};

class Stmt: virtual public AST {
};

class Atom: public Expr { 
};

class RValue: public Expr {  
};

class Simple: public Stmt {  
};







// Done
class Type: public AST{
public:
  Type(std::string str): type(str), hasSubtype(false){}
  Type(std::string str, Type * typ): type(str), hasSubtype(true), subtype(typ){}
  ~Type(){
    if(hasSubtype) delete subtype;
  }

  virtual void printOn(std::ostream &out) const override {
    out << " <Type> " << type;
    if (hasSubtype){
      out << *subtype;
    }
    out << " </Type> ";

  }
  virtual void compile() const override {
    printf("Type (");

    if(type=="int"){
      printf("int ");
    }
    else if(type=="char"){
      printf("char ");
    }
    else if(type=="bool"){
      printf("bool ");
    }
    else if(type=="array"){
      printf("array ");
    }
    else if(type=="list"){
      printf("list ");
    } else{
      printf("invalid type ");
    }

    if (hasSubtype){
      subtype->compile();
    }
    printf(")");
  }
private:
  std::string type;
  bool hasSubtype;
  Type * subtype;
};

class Id: public Atom {
public:
  Id(std::string v): var(v) {}
  virtual void printOn(std::ostream &out) const override {
    out << "<Id name=\"" << var << "\"> ";
  }
  virtual void compile() const override {
    printf("i smell a variable : %s\n", var.c_str());    
  }
private:
  std::string var;
};


// Done
class Array: public Atom {
public:
  Array(Atom *a, Expr *e): atom(a), expr(e) {}
  virtual void printOn(std::ostream &out) const override {
    out << "\n<Array>\n" << *atom << "\n" << *expr << "\n</Array>\n";
  }
  virtual void compile() const override {

    printf("i am arraying huehue\n");
    atom->compile();
    expr->compile();

  }
private:
  Atom *atom;
  Expr *expr;
};

// Done
class StringLiteral: public Atom {
public:
  StringLiteral(std::string str): strlit(str) {}
  virtual void printOn(std::ostream &out) const override {
    out << "<String value=\"" << strlit << "\"> ";
  }
  virtual void compile() const override {
    /*Εδώ θα μπει ο ενδιάμεσος κώδικας μετά*/
    printf("string_literal : %s\n", strlit.c_str());
  }
private:
  std::string strlit;
};


// Done
class CharConst: public RValue {
public:
  CharConst(char c): char_const(c) {}
  virtual void printOn(std::ostream &out) const override {
    out << "<CharConst value='" << char_const << "'> ";
  }
  virtual void compile() const override {
    /*Εδώ θα μπει ο ενδιάμεσος κώδικας μετά*/
    printf("char const : %c\n", char_const);
  }
private:
  char char_const;
};

// Done
class IntConst: public RValue {
public:
  IntConst(int n): num(n) {}
  virtual void printOn(std::ostream &out) const override {
    out << "<IntConst value=" << num << "> ";
  }
  virtual void compile() const override {
    /*Εδώ θα μπει ο ενδιάμεσος κώδικας μετά*/
    printf("int_const : %d\n", num);
  }
private:
  int num;
};

// Done
class New: public RValue {
public:
  New(Type *t, Expr *right): typ(t), expr(right){}
  ~New() {delete expr;}
  virtual void printOn(std::ostream &out) const override {
    out << "<New> " << *typ << *expr << "</New> ";
  }
  virtual void compile() const override {
    printf("New \n");
    typ->compile();
    expr->compile();
  }
private:
  Type *typ;
  Expr *expr;
};



class Nil: public RValue {
public:
  Nil() {}
  virtual void printOn(std::ostream &out) const override {
    out << "<Nil> ";
  }
  virtual void compile() const override {
    printf("Nil\n");
  }
};

// Done
class Boolean: public RValue {
public:
  Boolean(std::string b): boo(b) {} //wink wink pap
  virtual void printOn(std::ostream &out) const override {
    out << "<Boolean value=" << boo << "> ";
  }
  virtual void compile() const override {
    if (boo == "true")
      printf("true\n");
    else if (boo == "false")
      printf("false\n");
    else
      printf("Wrong bool value dummy");
  }
private:
  std::string boo;
};

// Done
class BinOp: public RValue {
public:
  BinOp(Expr *l, std::string o, Expr *r): left(l), op(o), right(r) {}
  ~BinOp() { delete left; delete right; }
  virtual void printOn(std::ostream &out) const override {
    out << "\n<Binop op=\"" << op << "\">\n" << *left << *right << "\n</BinOp>\n";
  }
  virtual void compile() const override {
    left->compile();
    right->compile();
    /*Στην θέση των παρακάτω θα μπει ο ενδιάμεσος κώδικας μετά*/
    if(op == "+")
      printf("operator +\n");
    else if(op == "-")
      printf("operator -\n");
    else if(op == "*")
      printf("operator *\n");
    else if(op == "/")
      printf("operator /\n");
    else if(op == "mod")
      printf("operator mod\n");
    else if(op == "=")
      printf("operator =\n");
    else if(op == ">")
      printf("operator >\n");
    else if(op == "<")
      printf("operator <\n");
    else if(op == ">=")
      printf("operator >=\n");
    else if(op == "<=")
      printf("operator <=\n");
    else if(op == "<>")
      printf("operator <>\n");
    else if(op == "and")
      printf("operator and\n");
    else if(op == "or")
      printf("operator or\n");
    else if(op == "#")
      printf("operator #\n");
    else 
      printf("Wrong operator dummy! Huehuehuehue\n");
    }
private:
  Expr *left;
  std::string op;
  Expr *right;
};

// Done
class UnOp: public RValue {
public:
  UnOp(std::string(o), Expr *r): op(o), right(r) {}
  ~UnOp() { delete right; }
  virtual void printOn(std::ostream &out) const override {
    out << "\n<UnOp op=\"" << op << "\">\n" << *right << "\n</UnOp>\n";
  }
  virtual void compile() const override {
    right->compile();
    if(op == "+")
      printf("operator +\n");
    else if(op == "-")
      printf("operator -\n");
    else if(op == "nil?")
      printf("operator nil?\n");
    else if(op == "not")
      printf("operator not\n");
    else if(op == "head")
      printf("operator head\n");
    else if(op == "tail")
      printf("operator tail\n");
    else 
      printf("Wrong unary operator dummy! Huehuehuehue\n");
    }
private:
  std::string op;
  Expr *right;
};





// Below here 'constructed' stuff

class VarList: public AST {
public:
  VarList() {}
  ~VarList(){
    for (Id * i : ids) delete i;
  }
  void append(Id * id) {
    ids.push_back(id);
  }
  void set_type(Type *t){
    type = t;
  }

  virtual void printOn(std::ostream &out) const override {
    out << "\n<VarList>\n";
    out << *type;
    for (Id * i : ids) {
      out << *i;
    }
    out << "\n</VarList>\n";
  }
  virtual void compile() const override {
    printf("VarList and these are my variantes: \n");
    type->compile();
    for (Id * i : ids)
      i->compile(); 
    }
private:
  std::vector<Id *> ids;
  Type * type;
};


class Formal: public AST {
public:
  Formal(VarList* v, bool isref): varlist(v), isRef(isref) {}
virtual void printOn(std::ostream &out) const override {
    out << "\n<Formal isRef=\"" << (isRef ? "yes" : "no") << "\">\n" << *varlist << "</Formal>";
  }
  virtual void compile() const override {
    printf("Formal, isref = ");
    if(isRef){
      printf("yes, ");
    }else{
      printf("no, ");
    }
    varlist->compile();
    printf("\n");
  }
private:
  VarList* varlist;
  bool isRef;
};


class FormalList: public AST {
public:
  FormalList() {}
  ~FormalList(){
    for (Formal * f : formals) delete f;
  }
  void append(Formal * f) {
    formals.push_back(f);
  }

  virtual void printOn(std::ostream &out) const override {
    out << "\n<FormalList>\n";

    for (Formal * f : formals) {
      out << *f;
    }
    out << "\n</FormalList>\n";
  }
  virtual void compile() const override {
    printf("FormalList and these are my formales: \n");
    for (Formal * f : formals)
      f->compile(); 
    }
private:
  std::vector<Formal *> formals;
};


class Header: public AST {
public:
  Header(Type *t, Id *i, FormalList *f): type(t), formals(f), id(i) {}
virtual void printOn(std::ostream &out) const override {
    out << "<Header>\n"; 
    if(type == NULL){
      out << "<NoType>";
    }else{
      out << *type;
    }
    out << *id; 
    if(formals == NULL){
      out << "<NoFormalList>";
    }else{
      out << *formals;
    }
    out << "\n</Header>\n";
  }
  virtual void compile() const override {
    printf("Header, type = ");
    if(type == NULL){
      printf("none");
    }else{
      type->compile();
    }
    printf(" id:");
    id->compile();
    printf(", params: ");
    if(formals == NULL){
      printf("none )");
    }else{
      formals->compile();
      printf(")");
    }
    printf("\n");
  }
private:
  Type *type;
  FormalList* formals;
  Id *id;
};




// Done
class Return: public Stmt{
public:
  Return(Expr* e): ret_expr(e) {}
virtual void printOn(std::ostream &out) const override {
    out << "\n<Return>\n" << *ret_expr << "\n</Return>\n";
  }
  virtual void compile() const override {
    printf("Return(), expression =");
    ret_expr->compile();
    printf("\n");
  }
private:
  Expr* ret_expr;
};


// Done
class Exit: public Stmt{
public:
  Exit() {}
  virtual void printOn(std::ostream &out) const override {
    out << "\n<Exit>\n";
  }
  virtual void compile() const override {
    printf("Exit\n");
  }
};

// Done
class StmtBody: public AST {
public:
  StmtBody(): stmts() {}
  ~StmtBody() { for (Stmt *s : stmts) delete s; }
  void append(Stmt* stmt) {
    stmts.push_back(stmt);
    return;
  }
  virtual void printOn(std::ostream &out) const override {
    out << "\n<StmtBody>\n";
    for (Stmt *s : stmts) {
      out << *s;
    }
    out << "\n</StmtBody>\n";
  }
  virtual void compile() const override {
    printf("Stmt body, check these out : \n");
    for (Stmt *fwtakaros : stmts)
      fwtakaros->compile();
  }
  
private:
  std::vector<Stmt*> stmts;
};



// Done
class Assign: public Simple{
public:
  Assign(Atom *a, Expr *e): atom(a), expr(e) {}
  virtual void printOn(std::ostream &out) const override {
    out << "\n<Assign>\n" << *atom << *expr << "\n</Assign>\n";
  }
  virtual void compile() const override {
    printf("Assign\n");
    atom->compile();
    expr->compile();
  }
private:
  Atom *atom;
  Expr *expr;
};

// Done
class Skip: public Simple{
public:
  Skip() {}
  virtual void printOn(std::ostream &out) const override {
    out << "\n<Skip>\n";
  }
  virtual void compile() const override {
    printf("Skip\n");
  }
};


class Elsif: public Stmt {
public:
  Elsif() {}
  ~Elsif() {
    for (Expr* e: elsif_conds) delete e;
    for (StmtBody* s: elsif_stmt_bodies) delete s;
  }
  std::vector<Expr *> get_conds() {
    return elsif_conds;
  }
  std::vector<StmtBody *> get_stmt_bodies() {
    return elsif_stmt_bodies;
  }

  bool isEmpty(){
    return elsif_conds.empty();
  }
  void append(Expr* e, StmtBody* s) {
    elsif_conds.push_back(e);
    elsif_stmt_bodies.push_back(s);
  }

  virtual void printOn(std::ostream &out) const override {
    
    // This is never used
    if(!elsif_conds.empty()){
      out << "<Elsif> ";
      for(int i=0; i< (int) elsif_stmt_bodies.size(); i++){
        out << "( " << *elsif_conds[i] << ", " << *elsif_stmt_bodies[i] << ")";
      }
      out << ")\n";
    }
  }
  virtual void compile() const override {
    printf("ElseIf \n");
    for(int i=0; i< (int) elsif_stmt_bodies.size(); i++){
        elsif_conds[i]->compile();
        elsif_stmt_bodies[i]->compile();
    }
  }

private:
  std::vector<Expr *> elsif_conds;
  std::vector<StmtBody *> elsif_stmt_bodies;
};
class Else: public Stmt {
public:
  Else() {}
  Else(StmtBody *s){else_stmt.push_back(s);}
  ~Else() {
    if (else_stmt.empty()){
      delete else_stmt[0];
    }
  }
  StmtBody * get_stmt() {
    return else_stmt[0]; 
  }

  bool isEmpty(){
    return else_stmt.empty();
  }

  // This is never used, only for testing
  virtual void printOn(std::ostream &out) const override {
    
    if(!else_stmt.empty()){
      out << "\n<Else>\n" << *else_stmt[0] << "\n</Else>\n";
    }
  }
  virtual void compile() const override {
    printf("Else \n");
    if(!else_stmt.empty()){
      else_stmt[0]->compile();}
  }

private:
  std::vector<StmtBody *> else_stmt;
};

class If: public Stmt {
public:
  If(Expr *e, StmtBody *b, Elsif * ei, Else *el){
    conditions.push_back(e);
    statements.push_back(b);
    
    std::vector<Expr *> elsif_conds = ei->get_conds();
    std::vector<StmtBody *> elsif_stmt_bodies = ei->get_stmt_bodies();
    for(int i=0; i< (int) elsif_conds.size();i++){
      conditions.push_back(elsif_conds[i]);
      statements.push_back(elsif_stmt_bodies[i]);
    }

    if(!el->isEmpty()){
      conditions.push_back(new Boolean("true"));
      statements.push_back(el->get_stmt());
      hasElse=true;
    }
  }
  ~If() {
    for (Expr* e: conditions) delete e;
    for (StmtBody* s: statements) delete s; 
  }

  virtual void printOn(std::ostream &out) const override {
    out << "\n<If>\n";

    out << *conditions[0] << *statements[0];
    int size=(int) conditions.size();
    if(hasElse){
      for (int i=1; i< (size-1);i++) {
        out << "\n<ElsIf>\n" << *conditions[i] << *statements[i] << "\n</ElsIf>\n";
      }
      out << "\n<Else>\n" << *conditions[size-1] << *statements[size-1] << "\n</Else>\n";
    }else{
      for (int i=1; i< (size);i++) {
        out << "\n<ElsIf>\n" << *conditions[i] << *statements[i] << "\n</ElsIf>\n";
      }
    }
    out << "\n</If>\n";
  }
  virtual void compile() const override {
    printf("If statement and these are my iffies: \n");
    for (int i=0; i< (int) conditions.size();i++){
      conditions[i]->compile();
      statements[i]->compile();
    }
  }
private:
  std::vector<Expr *> conditions;
  std::vector<StmtBody *> statements;
  bool hasElse = false;
};

class SimpleList: public AST {
public:
  SimpleList(): simples() {}
  ~SimpleList() { for (Simple *s : simples) delete s; }

  void append(Simple *s) { simples.push_back(s); }
  void insert_front(Simple *s) { simples.insert(simples.begin(), s);}

  virtual void printOn(std::ostream &out) const override {
    out << "\n<SimplesList>\n";
    for (Simple *s : simples) {
      out << *s;
    }
    out << "\n</SimpleList>\n";
  }
  virtual void compile() const override {
    printf("SimpleList and these are my stitches: \n");
    for (Simple *s : simples)
      s->compile();
  }
  std::vector<Simple *> get_simples_list(){
    return simples;
  }
private:
  std::vector<Simple *> simples;
};


class For: public Stmt {
public:
  For(SimpleList *sl1, Expr *e, SimpleList *sl2, StmtBody *sb):
    simple_list1(sl1), expr(e), simple_list2(sl2), stmt_body(sb)  {}
  ~For() {
    delete simple_list1;
    delete expr;
    delete simple_list2;
    delete stmt_body; }
  virtual void printOn(std::ostream &out) const override {
    out << "\n<For>\n" << *simple_list1 << *expr << *simple_list2  << *stmt_body << "\n</For>\n";
  }
  virtual void compile() const override {
    printf("For(");
    simple_list1->compile();
    expr->compile();
    simple_list2->compile();
    stmt_body->compile();
    printf(")\n");
  }
private:
  SimpleList *simple_list1;
  Expr *expr;
  SimpleList *simple_list2;
  StmtBody *stmt_body;
};



class ExprList: public AST {
public:
  ExprList(): expressions() {}
  ~ExprList() { for (Expr *e : expressions) delete e; }

  void append(Expr *e) { expressions.push_back(e); }
  void insert_front(Expr *e) { expressions.insert(expressions.begin(), e);}

  virtual void printOn(std::ostream &out) const override {
    out << "\n<ExprList>\n";
    for (Expr *e : expressions) {
      out << *e;
    }
    out << "\n</ExprList>\n";
  }
  virtual void compile() const override {
    printf("ExprList and these are my stitches: \n");
    for (Expr *e : expressions)
      e->compile();
  }
  std::vector<Expr*> get_expr_list(){
    return expressions;
  }
private:
  std::vector<Expr*> expressions;
};

class FunctionCall: public Simple, public Atom {
public:
  FunctionCall(Id *n): name(n), hasParams(false) {}
  FunctionCall(Id *n, ExprList *el): name(n), params(el), hasParams(true) {}
  //~Call(){}
  virtual void printOn(std::ostream &out) const override {
    if(!hasParams)
      out << "\n<FunctionCall>\n" << *name << "\n</FunctionCall>\n";
    else
      out << "\n<FunctionCall>\n" << *name << *params << "\n</FunctionCall>\n";
  }
  virtual void compile() const override {

    printf("Function Call(\n");
    name->compile();
    if(hasParams){
      printf(", ");
      params->compile();
    }
    printf(")\n");
  }
private:
  Id *name;
  ExprList *params;
  bool hasParams;
};




















// Below here not done













#endif