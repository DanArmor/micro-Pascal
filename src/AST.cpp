#include "AST.hpp"

/*Определения AST
==================*/
AST::AST() {};
AST::AST(Token token) : token(token){};

/*Определения BinOpAST
==================*/
BinOpAST::BinOpAST(Token token, AST *left, AST *right) : AST(token), left(left), right(right) {};

void BinOpAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения UnOpAST
==================*/
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
CompoundAST::CompoundAST(Token token) : AST(token) {};

void CompoundAST::addChild(AST *child){
    children.push_back(std::unique_ptr<AST>(child));
}

void CompoundAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения AssignAST
==================*/
AssignAST::AssignAST(Token token, AST *var, AST *value) : AST(token), var(var), value(value) {};

void AssignAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения VarAST
==================*/
VarAST::VarAST(Token token) : AST(token) {};

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
ProgramAST::ProgramAST(Token name, AST *block) : AST(name), name(name), block(block){};

void ProgramAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения BlockAST
==================*/
void BlockAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

BlockAST::BlockAST(Token token, std::vector<AST*> consts, std::vector<AST*> declarations, ASTptr compound) : AST(token), compound(compound) {
    for(std::size_t i = 0; i < consts.size(); i++)
        this->consts.push_back(std::unique_ptr<AST>(consts[i]));
    for(std::size_t i = 0; i < declarations.size(); i++)
        this->declarations.push_back(std::unique_ptr<AST>(declarations[i]));
};

/*Определения VarDeclAST
==================*/
VarDeclAST::VarDeclAST(Token token, AST *var, AST *type) : AST(token), var(var), type(type) {};

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
ConstAST::ConstAST(Token token, ASTptr constName, ASTptr constValue) : AST(token), constName(constName), constValue(constValue){};

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
CallAST::CallAST(Token token, std::vector<AST*> params) : AST(token){
    for(std::size_t i = 0; i < params.size(); i++)
        this->params.push_back(std::unique_ptr<AST>(params[i]));
};

void CallAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения IfAST
==================*/
IfAST::IfAST(Token token, ASTptr condition, ASTptr body, ASTptr elseBody) : AST(token), condition(condition), body(body), elseBody(elseBody){};

void IfAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения WhileAST
==================*/
WhileAST::WhileAST(Token token, ASTptr condition, ASTptr body) : AST(token), condition(condition), body(body){};

void WhileAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}