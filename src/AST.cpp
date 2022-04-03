#include "AST.hpp"

/*Определения AST
==================*/
//AST::AST() {};
AST::AST(Token token) : token(token){};

/*Определения BinOp
==================*/
BinOp::BinOp(Token token) : AST(token) {};
BinOp::BinOp(AST *left, Token token, AST *right) : AST(token), left(left), right(right) {};

void BinOp::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения UnOp
==================*/
UnOp::UnOp(Token token) : AST(token), down(nullptr){};
UnOp::UnOp(Token token, AST *down) : AST(token), down(down){};

void UnOp::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения Number
==================*/
Number::Number(Token token) : AST(token) {};

void Number::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения Compound
==================*/
Compound::Compound() : AST({"COMPOUND", IToken::EMPTY}) {};

void Compound::addChild(ASTptr child){
    children.push_back(child);
}

void Compound::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения Assign
==================*/
Assign::Assign(Token token) : AST(token) {};
Assign::Assign(AST *var, Token token, AST *value) : AST(token), var(var), value(value) {};

void Assign::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения Var
==================*/
Var::Var(Token token) : AST(token) {};
Var::Var(Token token, int value) : AST(token), value(value) {};

void Var::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения NoOp
==================*/
NoOp::NoOp(Token token) : AST(token) {};

void NoOp::accept(IVisitor &visitor){
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

/*Определения VarDeclaration
==================*/
VarDeclaration::VarDeclaration(ASTptr var, ASTptr type) : AST({"DEFINITION", IToken::VARDECL}), var(var), type(type) {};

void VarDeclaration::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения Type
==================*/
Type::Type(Token token) : AST(token){};

void Type::accept(IVisitor &visitor){
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

/*Определения ProcedureCall
==================*/
ProcedureCall::ProcedureCall(Token token, std::vector<ASTptr> params) : AST(token), params(params){};

void ProcedureCall::accept(IVisitor &visitor){
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