#include "AST.hpp"

/*Определения AST
==================*/
AST::AST() {};
AST::AST(Token token) : token(token){};

/*Определения BinOpAST
==================*/
BinOpAST::BinOpAST(Token token, std::unique_ptr<AST> left, std::unique_ptr<AST> right) : AST(token), left(std::move(left)), right(std::move(right)) {};

void BinOpAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения UnOpAST
==================*/
UnOpAST::UnOpAST(Token token, std::unique_ptr<AST> down) : AST(token), down(std::move(down)){};

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

void CompoundAST::addChild(std::unique_ptr<AST> child){
    children.emplace_back(std::move(child));
}

void CompoundAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения AssignAST
==================*/
AssignAST::AssignAST(Token token, std::unique_ptr<AST> var, std::unique_ptr<AST> value) : AST(token), var(std::move(var)), value(std::move(value)) {};

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
ProgramAST::ProgramAST(Token name, std::unique_ptr<AST> block) : AST(name), name(name), block(std::move(block)){};

void ProgramAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения BlockAST
==================*/
void BlockAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

BlockAST::BlockAST(Token token, std::vector<std::unique_ptr<AST>> consts, std::vector<std::unique_ptr<AST>> declarations, std::unique_ptr<AST> compound) : AST(token), compound(std::move(compound)) {
    for(std::size_t i = 0; i < consts.size(); i++)
        this->consts.emplace_back(std::move(consts[i]));
    for(std::size_t i = 0; i < declarations.size(); i++)
        this->declarations.emplace_back(std::move(declarations[i]));
};

/*Определения VarDeclAST
==================*/
VarDeclAST::VarDeclAST(Token token, std::unique_ptr<AST> var, std::unique_ptr<AST> type) : AST(token), var(std::move(var)), type(std::move(type)) {};

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
ConstAST::ConstAST(Token token, std::unique_ptr<AST> constName, std::unique_ptr<AST> constValue) : AST(token), constName(std::move(constName)), constValue(std::move(constValue)){};

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
CallAST::CallAST(Token token, std::vector<std::unique_ptr<AST>> params) : AST(token){
    for(std::size_t i = 0; i < params.size(); i++)
        this->params.emplace_back(std::move(params[i]));
};

void CallAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения IfAST
==================*/
IfAST::IfAST(Token token, std::unique_ptr<AST> condition, std::unique_ptr<AST> body, std::unique_ptr<AST> elseBody) : AST(token), condition(std::move(condition)), body(std::move(body)), elseBody(std::move(elseBody)){};

void IfAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения WhileAST
==================*/
WhileAST::WhileAST(Token token, std::unique_ptr<AST> condition, std::unique_ptr<AST> body) : AST(token), condition(std::move(condition)), body(std::move(body)){};

void WhileAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения ForAST
==================*/
ForAST::ForAST(Token token, std::unique_ptr<AST> iterSt, std::unique_ptr<AST> body) : AST(token), iterSt(std::move(iterSt)), body(std::move(body)){};
void ForAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения IterationAST
==================*/
IterationAST::IterationAST(Token token, std::unique_ptr<AST> assign, std::unique_ptr<AST> condition, std::unique_ptr<AST> postAction) : AST(token), assign(std::move(assign)), condition(std::move(condition)), postAction(std::move(postAction)){};
void IterationAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}