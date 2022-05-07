#include "ASTFactory.hpp"

#include <fmt/format.h>

#include "AST.hpp"
#include "ASTclasses.hpp"
#include "Token.hpp"

std::unique_ptr<IAST> ASTFactory::createAST(Token token) {
    switch (token.getAdvType()) {
        case BaseToken::TYPE_SPEC:
            return std::unique_ptr<IAST>(new TypeSpecAST(token));
    }
    switch (token.getType()) {
        case BaseToken::STRING_CONST:
            return std::unique_ptr<IAST>(new StringAST(token));
        case BaseToken::INTEGER_CONST:
            return std::unique_ptr<IAST>(new NumberAST(token));
        case BaseToken::REAL_CONST:
            return std::unique_ptr<IAST>(new NumberAST(token));
        case BaseToken::ID:
            return std::unique_ptr<IAST>(new VarAST(token));
        case BaseToken::EMPTY: {
            token.setStr("$");
            return std::unique_ptr<IAST>(new NoOpAST(token));
        }
    }
    throw std::runtime_error(fmt::format(
        "Попытка создания AST узла из неверного токена и набора аргументов! {}",
        token.getInfo()));
}

std::unique_ptr<IAST> ASTFactory::createAST(Token token,
                                            std::unique_ptr<IAST> first) {
    if (token.getAdvType() == BaseToken::OPERATOR)
        return std::unique_ptr<IAST>(new UnOpAST(token, std::move(first)));
    else if (token.getType() == BaseToken::RETURN)
        return std::unique_ptr<IAST>(new ReturnAST(token, std::move(first)));
    throw std::runtime_error(fmt::format(
        "Попытка создания AST узла из неверного токена и набора аргументов! {}",
        token.getInfo()));
}

std::unique_ptr<IAST> ASTFactory::createAST(Token token,
                                            std::unique_ptr<IAST> first,
                                            std::unique_ptr<IAST> second) {
    switch (token.getType()) {
        case BaseToken::ASSIGN:
            return std::unique_ptr<IAST>(
                new AssignAST(token, std::move(first), std::move(second)));
        case BaseToken::WHILE:
            return std::unique_ptr<IAST>(
                new WhileAST(token, std::move(first), std::move(second)));
        case BaseToken::FOR:
            return std::unique_ptr<IAST>(
                new ForAST(token, std::move(first), std::move(second)));
    }
    switch (token.getAdvType()) {
        case BaseToken::OPERATOR:
            return std::unique_ptr<IAST>(
                new BinOpAST(token, std::move(first), std::move(second)));
        case BaseToken::VARDECL: {
            token.setStr("VARDECL");
            return std::unique_ptr<IAST>(
                new VarDeclAST(token, std::move(first), std::move(second)));
        }
        case BaseToken::SELECT: {
            token.setStr("SELECT");
            return std::unique_ptr<IAST>(
                new SelectAST(token, std::move(first), std::move(second)));
        }
        case BaseToken::CONSTDECL: {
            token.setStr("CONSTDECL");
            return std::unique_ptr<IAST>(
                new ConstAST(token, std::move(first), std::move(second)));
        }
    }
    throw std::runtime_error(fmt::format(
        "Попытка создания AST узла из неверного токена и набора аргументов! {}",
        token.getInfo()));
}

std::unique_ptr<IAST> ASTFactory::createAST(Token token,
                                            std::unique_ptr<IAST> first,
                                            std::unique_ptr<IAST> second,
                                            std::unique_ptr<IAST> third) {
    switch (token.getType()) {
        case BaseToken::IF:
            return std::unique_ptr<IAST>(new IfAST(
                token, std::move(first), std::move(second), std::move(third)));
        case BaseToken::TO:
            return std::unique_ptr<IAST>(new IterationAST(
                token, std::move(first), std::move(second), std::move(third)));
        case BaseToken::DOWNTO: {
            token.setStr("down to");
            return std::unique_ptr<IAST>(new IterationAST(
                token, std::move(first), std::move(second), std::move(third)));
        }
    }
    throw std::runtime_error(fmt::format(
        "Попытка создания AST узла из неверного токена и набора аргументов! {}",
        token.getInfo()));
}

std::unique_ptr<IAST> ASTFactory::createAST(
    Token token, std::vector<std::unique_ptr<IAST>> params) {
    switch (token.getAdvType()) {
        case BaseToken::AdvType::COMPOUND: {
            token.setStr("COMPOUND");
            std::unique_ptr<CompoundAST> compound(new CompoundAST(token));
            for (auto &ptr : params) compound->addChild(std::move(ptr));
            return std::move(compound);
        }
        case BaseToken::AdvType::FUNCTION_NAME:
            return std::unique_ptr<IAST>(new CallAST(token, std::move(params)));
    }
    throw std::runtime_error(fmt::format(
        "Попытка создания AST узла из неверного токена и набора аргументов! {}",
        token.getInfo()));
}

std::unique_ptr<IAST> ASTFactory::createAST(
    Token token, std::vector<std::unique_ptr<IAST>> params,
    std::unique_ptr<IAST> first) {
    if (token.getAdvType() == BaseToken::PROGRAM_NAME) {
        std::unique_ptr<ProgramAST> programPTR(
            new ProgramAST(token, std::move(params), std::move(first)));
        programPTR->name.setStr(
            fmt::format("PROGRAM:\n{}", programPTR->token.getStr()));
        return programPTR;
    }
    throw std::runtime_error(fmt::format(
        "Попытка создания AST узла из неверного токена и набора аргументов! {}",
        token.getInfo()));
}

std::unique_ptr<IAST> ASTFactory::createAST(
    Token token, std::vector<std::unique_ptr<IAST>> params,
    std::unique_ptr<IAST> first, std::unique_ptr<IAST> second) {
    if (token.getAdvType() == BaseToken::FUNCTION_NAME)
        return std::unique_ptr<IAST>(new FunctionAST(
            token, std::move(params), std::move(first), std::move(second)));
    throw std::runtime_error(fmt::format(
        "Попытка создания AST узла из неверного токена и набора аргументов! {}",
        token.getInfo()));
}

std::unique_ptr<IAST> ASTFactory::createAST(Token firstTok, Token secondTok,
                                            Token thirdTok,
                                            std::unique_ptr<IAST> first) {
    if (firstTok.getType() == BaseToken::ARRAY)
        return std::unique_ptr<IAST>(
            new ArrSpecAST(firstTok, secondTok, thirdTok, std::move(first)));
    throw std::runtime_error(fmt::format(
        "Попытка создания AST узла из неверного токена и набора "
        "аргументов!\n{}\n{}\n{}",
        firstTok.getInfo(), secondTok.getInfo(), thirdTok.getInfo()));
}

std::unique_ptr<IAST> ASTFactory::createAST(
    Token token, std::vector<std::unique_ptr<IAST>> constsDecls,
    std::vector<std::unique_ptr<IAST>> varDecls,
    std::unique_ptr<IAST> compound) {
    if (token.getType() == BaseToken::BLOCK) {
        token.setStr("BLOCK");
        return std::unique_ptr<IAST>(new BlockAST(token, std::move(constsDecls),
                                                  std::move(varDecls),
                                                  std::move(compound)));
    }
    throw std::runtime_error(fmt::format(
        "Попытка создания AST узла из неверного токена и набора аргументов! {}",
        token.getInfo()));
}

std::unique_ptr<IAST> ASTFactory::createCopyOfType(IAST *typePtr) {
    if (typePtr->token.getAdvType() == BaseToken::TYPE_SPEC ||
        typePtr->token.getType() == BaseToken::ARRAY) {
        if (typePtr->token.getType() == BaseToken::ARRAY) {
            ArrSpecAST *rawPtr = static_cast<ArrSpecAST *>(typePtr);
            std::unique_ptr<IAST> arrPtr =
                createAST(rawPtr->token, rawPtr->lHandTok, rawPtr->rHandTok,
                          createCopyOfType(rawPtr->subType.get()));
            return arrPtr;
        } else {
            return createAST(typePtr->token);
        }
    } else {
        throw std::logic_error(
            "Применение createCopyOfType к узлу, не являющемуся типом! " +
            typePtr->token.getInfo());
    }
}