#ifndef __INC_ASTFACTORY_H
#define __INC_ASTFACTORY_H

#include "AST.hpp"
#include "Token.hpp"

class ASTFactory{
    public:
    static AST *createAST(Token token){
        switch (token.getAdvType()){
            case IToken::TYPE_SPEC: return new TypeSpecAST(token);
        }
        switch (token.getType()){
            case IToken::STRING_CONST: return new StringAST(token);
            case IToken::INTEGER_CONST: return new NumberAST(token);
            case IToken::REAL_CONST: return new NumberAST(token);
            case IToken::ID: return new VarAST(token);
        }
    }

    static AST *createAST(Token token, AST *first){
        switch (token.getType()){
            case IToken::PROGRAM : return new ProgramAST(token, first);
        }
    }

    static AST *createAST(Token token, AST *first, AST *second){
        switch(token.getType()){
            case IToken::ASSIGN : return new AssignAST(first, token, second);
            case IToken::WHILE : return new whileAST(first, second);
        }
        switch (token.getAdvType()){
            case IToken::OPERATOR : return new BinOpAST(first, token, second);
            case IToken::VARDECL: return new VarDeclAST(first, second);
            case IToken::CONSTDECL: return new ConstAST(first, second);
        }
    }

    static AST *createAST(Token token, std::vector<AST*> params){
        return new CallAST(token, params);
    }

    static AST *createAST(Token token, std::vector<AST*> constsDecls, std::vector<AST*> varDecls, AST *compound){
        return new BlockAST(constsDecls, varDecls, compound);
    }

};


#endif
