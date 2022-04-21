#include "ASTFactory.hpp"

#include <fmt/format.h>

#include "ASTclasses.hpp"
#include "AST.hpp"
#include "Token.hpp"

    AST *ASTFactory::createAST(Token token){
        switch (token.getAdvType()){
            case IToken::TYPE_SPEC: return new TypeSpecAST(token);
        }
        switch (token.getType()){
            case IToken::STRING_CONST: return new StringAST(token);
            case IToken::INTEGER_CONST: return new NumberAST(token);
            case IToken::REAL_CONST: return new NumberAST(token);
            case IToken::ID: return new VarAST(token);
            case IToken::EMPTY: {
                token.setStr("$");
                return new NoOpAST(token);
            }
        }
    }

    AST *ASTFactory::createAST(Token token, AST *first){
        if(token.getAdvType() == IToken::OPERATOR)
            return new UnOpAST(token, first);
        else if(token.getAdvType() == IToken::PROGSTART){
            ProgramAST *programPTR = new ProgramAST(token, first);
            programPTR->token.setStr(fmt::format("PROGRAM:\n{}", programPTR->token.getStr()));
            return programPTR;
        }
    }

    AST *ASTFactory::createAST(Token token, AST *first, AST *second){
        switch(token.getType()){
            case IToken::ASSIGN : return new AssignAST(token, first, second);
            case IToken::WHILE : return new WhileAST(token, first, second);
        }
        switch (token.getAdvType()){
            case IToken::OPERATOR : return new BinOpAST(token, first, second);
            case IToken::VARDECL:{
                token.setStr("VARDECL");
                return new VarDeclAST(token, first, second);
            }
            case IToken::CONSTDECL:{
                token.setStr("CONSTDECL");
                return new ConstAST(token, first, second);
            }
        }
    }

    AST *ASTFactory::createAST(Token token, AST *first, AST *second, AST *third){
        switch(token.getType()){
            case IToken::IF : return new IfAST(token, first, second, third);
        }
    }

    AST *ASTFactory::createAST(Token token, std::vector<AST*> params){
        switch (token.getAdvType()){
            case IToken::AdvType::COMPOUND : {
                token.setStr("COMPOUND");
                CompoundAST *compound = new CompoundAST(token);
                for(auto &ptr : params)
                    compound->addChild(ptr);
                return compound;
            }
        }
        return new CallAST(token, params);
    }

    AST *ASTFactory::createAST(Token token, std::vector<AST*> constsDecls, std::vector<AST*> varDecls, AST *compound){
        if(token.getType() == IToken::BLOCK){
            token.setStr("BLOCK");
            return new BlockAST(token, constsDecls, varDecls, compound);
        }
    }