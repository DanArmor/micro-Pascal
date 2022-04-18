#include "AST.hpp"

/*Определения AST
==================*/
//AST::AST() {};
AST::AST(Token token) : token(token){};

/*Определения BinOpAST
==================*/
BinOpAST::BinOpAST(Token token) : AST(token) {};
BinOpAST::BinOpAST(AST *left, Token token, AST *right) : AST(token), left(left), right(right) {};

void BinOpAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения UnOpAST
==================*/
UnOpAST::UnOpAST(Token token) : AST(token), down(nullptr){};
UnOpAST::UnOpAST(Token token, AST *down) : AST(token), down(down){};

void UnOpAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения NumberAST
==================*/
NumberAST::NumberAST(Token token) : AST(token) {};

void NumberAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения CompoundAST
==================*/
CompoundAST::CompoundAST() : AST({"COMPOUND", IToken::EMPTY}) {};

void CompoundAST::addChild(ASTptr child){
    children.push_back(child);
}

void CompoundAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения AssignAST
==================*/
AssignAST::AssignAST(Token token) : AST(token) {};
AssignAST::AssignAST(AST *var, Token token, AST *value) : AST(token), var(var), value(value) {};

void AssignAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения VarAST
==================*/
VarAST::VarAST(Token token) : AST(token) {};
VarAST::VarAST(Token token, int value) : AST(token), value(value) {};

void VarAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения NoOpAST
==================*/
NoOpAST::NoOpAST(Token token) : AST(token) {};

void NoOpAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения ProgramAST
==================*/
ProgramAST::ProgramAST(Token name, AST *block) : AST({"__START__", IToken::PROGSTART}), name(name), block(block){};

void ProgramAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения BlockAST
==================*/
void BlockAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

BlockAST::BlockAST(std::vector<ASTptr> consts, std::vector<ASTptr> declarations, ASTptr compound) : AST({"BLOCK", IToken::Type::BLOCK}), consts(consts), declarations(declarations), compound(compound) {};

/*Определения VarDeclAST
==================*/
VarDeclAST::VarDeclAST(ASTptr var, ASTptr type) : AST({"DEFINITION", IToken::VARDECL}), var(var), type(type) {};

void VarDeclAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения TypeSpecAST
==================*/
TypeSpecAST::TypeSpecAST(Token token) : AST(token){};

void TypeSpecAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения ConstAST
==================*/
ConstAST::ConstAST(ASTptr constName, ASTptr constValue) : AST({"=", IToken::CONST}), constName(constName), constValue(constValue){};

void ConstAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения StringAST
==================*/
StringAST::StringAST(Token token) : AST(token) {};

void StringAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения CallAST
==================*/
CallAST::CallAST(Token token, std::vector<ASTptr> params) : AST(token), params(params){};

void CallAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения ifAST
==================*/
ifAST::ifAST(ASTptr condition, ASTptr body, ASTptr elseBody) : AST({"IF", IToken::EMPTY}), condition(condition), body(body), elseBody(elseBody){};

void ifAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения whileAST
==================*/
whileAST::whileAST(ASTptr condition, ASTptr body) : AST({"WHILE", IToken::EMPTY}), condition(condition), body(body){};

void whileAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}