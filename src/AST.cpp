#include "AST.hpp"

/*Определения AST
==================*/
IAST::IAST() {};
IAST::IAST(Token token) : token(token){};

/*Определения BinOpAST
==================*/
BinOpAST::BinOpAST(Token token, std::unique_ptr<IAST> left, std::unique_ptr<IAST> right) : IAST(token), left(std::move(left)), right(std::move(right)) {};

void BinOpAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения UnOpAST
==================*/
UnOpAST::UnOpAST(Token token, std::unique_ptr<IAST> down) : IAST(token), down(std::move(down)){};

void UnOpAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения NumberAST
==================*/
NumberAST::NumberAST(Token token) : IAST(token) {};

void NumberAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения CompoundAST
==================*/
CompoundAST::CompoundAST(Token token) : IAST(token) {};

void CompoundAST::addChild(std::unique_ptr<IAST> child){
    children.emplace_back(std::move(child));
}

void CompoundAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения AssignAST
==================*/
AssignAST::AssignAST(Token token, std::unique_ptr<IAST> var, std::unique_ptr<IAST> value) : IAST(token), var(std::move(var)), value(std::move(value)) {};

void AssignAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения VarAST
==================*/
VarAST::VarAST(Token token) : IAST(token) {};

void VarAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения NoOpAST
==================*/
NoOpAST::NoOpAST(Token token) : IAST(token) {};

void NoOpAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения ProgramAST
==================*/
ProgramAST::ProgramAST(Token name, std::vector<std::unique_ptr<IAST>> functions, std::unique_ptr<IAST> block) : IAST(name), name(name), functions(std::move(functions)), block(std::move(block)){};

void ProgramAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения BlockAST
==================*/
void BlockAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

BlockAST::BlockAST(Token token, std::vector<std::unique_ptr<IAST>> consts, std::vector<std::unique_ptr<IAST>> declarations, std::unique_ptr<IAST> compound) : IAST(token), compound(std::move(compound)) {
    for(std::size_t i = 0; i < consts.size(); i++)
        this->consts.emplace_back(std::move(consts[i]));
    for(std::size_t i = 0; i < declarations.size(); i++)
        this->declarations.emplace_back(std::move(declarations[i]));
};

/*Определения VarDeclAST
==================*/
VarDeclAST::VarDeclAST(Token token, std::unique_ptr<IAST> var, std::unique_ptr<IAST> type) : IAST(token), var(std::move(var)), type(std::move(type)) {};

void VarDeclAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения TypeSpecAST
==================*/
TypeSpecAST::TypeSpecAST(Token token) : IAST(token){};

void TypeSpecAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

ArrSpecAST::ArrSpecAST(Token arrTok, Token lHandTok, Token rHandTok, std::unique_ptr<IAST> subType) : IAST(arrTok), lHandTok(lHandTok), rHandTok(rHandTok), subType(std::move(subType)){}

void ArrSpecAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения ConstAST
==================*/
ConstAST::ConstAST(Token token, std::unique_ptr<IAST> constName, std::unique_ptr<IAST> constValue) : IAST(token), constName(std::move(constName)), constValue(std::move(constValue)){};

void ConstAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения StringAST
==================*/
StringAST::StringAST(Token token) : IAST(token) {};

void StringAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения CallAST
==================*/
CallAST::CallAST(Token token, std::vector<std::unique_ptr<IAST>> params) : IAST(token){
    for(std::size_t i = 0; i < params.size(); i++)
        this->params.emplace_back(std::move(params[i]));
};

void CallAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения IfAST
==================*/
IfAST::IfAST(Token token, std::unique_ptr<IAST> condition, std::unique_ptr<IAST> body, std::unique_ptr<IAST> elseBody) : IAST(token), condition(std::move(condition)), body(std::move(body)), elseBody(std::move(elseBody)){};

void IfAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения WhileAST
==================*/
WhileAST::WhileAST(Token token, std::unique_ptr<IAST> condition, std::unique_ptr<IAST> body) : IAST(token), condition(std::move(condition)), body(std::move(body)){};

void WhileAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения ForAST
==================*/
ForAST::ForAST(Token token, std::unique_ptr<IAST> iterSt, std::unique_ptr<IAST> body) : IAST(token), iterSt(std::move(iterSt)), body(std::move(body)){};
void ForAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения IterationAST
==================*/
IterationAST::IterationAST(Token token, std::unique_ptr<IAST> assign, std::unique_ptr<IAST> condition, std::unique_ptr<IAST> postAction) : IAST(token), assign(std::move(assign)), condition(std::move(condition)), postAction(std::move(postAction)){};
void IterationAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения FunctionAST
==================*/
FunctionAST::FunctionAST(Token token, std::vector<std::unique_ptr<IAST>> params, std::unique_ptr<IAST> returnType, std::unique_ptr<IAST> body) : 
    IAST(token), params(std::move(params)), returnType(std::move(returnType)), body(std::move(body)){
        this->name = token;
        this->name.setStr("function:\n" + token.getStr());
    };

void FunctionAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения ReturnAST
==================*/
ReturnAST::ReturnAST(Token token, std::unique_ptr<IAST> toReturn) : IAST(token), toReturn(std::move(toReturn)){};

void ReturnAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}

/*Определения SelectAST
==================*/
SelectAST::SelectAST(Token token, std::unique_ptr<IAST> from, std::unique_ptr<IAST> index) : IAST(token), from(std::move(from)), index(std::move(index)) {};

void SelectAST::accept(IVisitor &visitor){
    visitor.visit(*this);
}