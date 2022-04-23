#include "ASTFactory.hpp"

#include <fmt/format.h>

#include "ASTclasses.hpp"
#include "AST.hpp"
#include "Token.hpp"

    std::unique_ptr<AST> ASTFactory::createAST(Token token){
        switch (token.getAdvType()){
            case IToken::TYPE_SPEC: return std::unique_ptr<AST>(new TypeSpecAST(token));
        }
        switch (token.getType()){
            case IToken::STRING_CONST: return std::unique_ptr<AST>(new StringAST(token));
            case IToken::INTEGER_CONST: return std::unique_ptr<AST>(new NumberAST(token));
            case IToken::REAL_CONST: return std::unique_ptr<AST>(new NumberAST(token));
            case IToken::ID: return std::unique_ptr<AST>(new VarAST(token));
            case IToken::EMPTY: {
                token.setStr("$");
                return std::unique_ptr<AST>(new NoOpAST(token));
            }
        }
    }

    std::unique_ptr<AST> ASTFactory::createAST(Token token, std::unique_ptr<AST> first){
        if(token.getAdvType() == IToken::OPERATOR)
            return std::unique_ptr<AST>(new UnOpAST(token, std::move(first)));
        else if(token.getAdvType() == IToken::PROGSTART){
            std::unique_ptr<ProgramAST> programPTR(new ProgramAST(token, std::move(first)));
            programPTR->token.setStr(fmt::format("PROGRAM:\n{}", programPTR->token.getStr()));
            return programPTR;
        }
    }

    std::unique_ptr<AST> ASTFactory::createAST(Token token, std::unique_ptr<AST> first, std::unique_ptr<AST> second){
        switch(token.getType()){
            case IToken::ASSIGN : return std::unique_ptr<AST>(new AssignAST(token, std::move(first), std::move(second)));
            case IToken::WHILE : return std::unique_ptr<AST>(new WhileAST(token, std::move(first), std::move(second)));
            case IToken::FOR : return std::unique_ptr<AST>(new ForAST(token, std::move(first), std::move(second)));
        }
        switch (token.getAdvType()){
            case IToken::OPERATOR : return std::unique_ptr<AST>(new BinOpAST(token, std::move(first), std::move(second)));
            case IToken::VARDECL:{
                token.setStr("VARDECL");
                return std::unique_ptr<AST>(new VarDeclAST(token, std::move(first), std::move(second)));
            }
            case IToken::CONSTDECL:{
                token.setStr("CONSTDECL");
                return std::unique_ptr<AST>(new ConstAST(token, std::move(first), std::move(second)));
            }
        }
    }

    std::unique_ptr<AST> ASTFactory::createAST(Token token, std::unique_ptr<AST> first, std::unique_ptr<AST> second, std::unique_ptr<AST> third){
        switch(token.getType()){
            case IToken::IF : return std::unique_ptr<AST>(new IfAST(token, std::move(first), std::move(second), std::move(third)));
            case IToken::TO : return std::unique_ptr<AST>(new IterationAST(token, std::move(first), std::move(second), std::move(third)));
            case IToken::DOWNTO :{
                token.setStr("down to");
                return std::unique_ptr<AST>(new IterationAST(token, std::move(first), std::move(second), std::move(third)));
            } 
        }
    }

    std::unique_ptr<AST> ASTFactory::createAST(Token token, std::vector<std::unique_ptr<AST>> params){
        switch (token.getAdvType()){
            case IToken::AdvType::COMPOUND : {
                token.setStr("COMPOUND");
                std::unique_ptr<CompoundAST> compound(new CompoundAST(token));
                for(auto &ptr : params)
                    compound->addChild(std::move(ptr));
                return std::move(compound);
            }
        }
        return std::unique_ptr<AST>(new CallAST(token, std::move(params)));
    }

    std::unique_ptr<AST> ASTFactory::createAST(Token token,
        std::vector<std::unique_ptr<AST>> constsDecls, std::vector<std::unique_ptr<AST>> varDecls, std::unique_ptr<AST> compound){
        if(token.getType() == IToken::BLOCK){
            token.setStr("BLOCK");
            return std::unique_ptr<AST>(new BlockAST(token, std::move(constsDecls), std::move(varDecls), std::move(compound)));
        }
    }