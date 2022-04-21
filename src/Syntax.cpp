#include "Syntax.hpp"

#include <fmt/format.h>

#include "AST.hpp"
#include "ASTFactory.hpp"

SyntaxAnalyzer::SyntaxAnalyzer(List<Token> const &tokens) : tokens(tokens) {};

std::unique_ptr<AST> SyntaxAnalyzer::syntaxProgram(void){
    /* syntaxProgram ::= 'PROGRAM' 'ID' ';' syntaxBlock '.'  */
    eat(IToken::PROGRAM);
    Token nameTok = getCurTok();
    nameTok.setAdvType(IToken::PROGSTART);
    eat(IToken::ID);
    eat(IToken::SEMI);
    std::unique_ptr<AST> blockPTR = syntaxBlock();
    eat(IToken::DOT);
    return ASTFactory::createAST(nameTok, std::move(blockPTR));
}

std::unique_ptr<AST> SyntaxAnalyzer::syntaxBlock(void){
    /* syntaxBlock ::= syntaxConsts* syntaxVars* syntaxCompoundSt */
    std::vector<std::unique_ptr<AST>> constsList = syntaxConsts();
    std::vector<std::unique_ptr<AST>> declsList = syntaxVars();
    std::unique_ptr<AST> compoundPTR = syntaxCompoundSt();
    return ASTFactory::createAST(IToken::Type::BLOCK, std::move(constsList), std::move(declsList), std::move(compoundPTR));
}

std::vector<std::unique_ptr<AST>> SyntaxAnalyzer::syntaxConsts(void){
    /* syntaxConsts ::= 'CONST' (syntaxConstDecl ';')+ */
    std::vector<std::unique_ptr<AST>> constsList;
    if(getCurTok().getType() == IToken::CONST){
        eat(IToken::CONST);
        while(getCurTok().getType() == IToken::ID){
            constsList.emplace_back(syntaxConstDecl());
            eat(IToken::SEMI);
        }
    }
    return constsList;
}

std::unique_ptr<AST> SyntaxAnalyzer::syntaxConstDecl(void){
    /* syntaxConstDecl ::= '<ID>' '=' ('<INTEGER_CONST>'|'<REAL_CONST>'|'<STRING_CONST>') */
    std::unique_ptr<AST> constNamePTR = ASTFactory::createAST(getCurTok());
    eat(IToken::ID);
    eat(IToken::EQ);

    Token constValTok = getCurTok();
    std::unique_ptr<AST> constValPTR = ASTFactory::createAST(constValTok);

    constValTok.setAdvType(IToken::AdvType::CONSTDECL);
    eatAdv(IToken::SOME_CONST);

    return ASTFactory::createAST(constValTok, std::move(constNamePTR), std::move(constValPTR));
}

std::vector<std::unique_ptr<AST>> SyntaxAnalyzer::syntaxVars(void){
    /* syntaxVars ::= 'VAR' (syntaxVarDecl ';')+ */
    std::vector<std::unique_ptr<AST>> declsList;
    if(getCurTok().getType() == IToken::VAR){
        eat(IToken::VAR);
        while(getCurTok().getType() == IToken::ID){
            std::vector<std::unique_ptr<AST>> varDeclList = syntaxVarDecl();
            for(auto &varDeclPTR : varDeclList)
                declsList.emplace_back(std::move(varDeclPTR));
            eat(IToken::SEMI);
        }
    }
    return declsList;
}

std::vector<std::unique_ptr<AST>> SyntaxAnalyzer::syntaxVarDecl(void){
    /* syntaxVarDecl ::= ('<ID>' ','*)+ ':' syntaxTypeSpec */
    std::vector<std::unique_ptr<AST>> varsList;
    varsList.emplace_back(ASTFactory::createAST(getCurTok()));
    eat(IToken::ID);

    while(getCurTok().getType() == IToken::COMMA){
        eat(IToken::COMMA);
        varsList.emplace_back(ASTFactory::createAST(getCurTok()));
        eat(IToken::ID);
    }
    eat(IToken::COLON);

    std::unique_ptr<AST> typeNodePTR(syntaxTypeSpec());

    std::vector<std::unique_ptr<AST>> varDeclsList;
    for(auto &var : varsList){
        varDeclsList.emplace_back(ASTFactory::createAST(IToken::VARDECL, std::move(var), ASTFactory::createAST(typeNodePTR->token)));
    }
    return varDeclsList;
}

std::unique_ptr<AST> SyntaxAnalyzer::syntaxTypeSpec(void){
    /* syntaxTypeSpec ::= ('INTEGER'|'REAL') */
    Token typeTok = getCurTok();
    eatAdv(IToken::TYPE_SPEC);
    return ASTFactory::createAST(typeTok);
}

std::unique_ptr<AST> SyntaxAnalyzer::syntaxCompoundSt(void){
    /* syntaxCompoundSt ::= 'BEGIN' (syntaxSt ';')* 'END' */
    Token beginTok = getCurTok();
    beginTok.setAdvType(IToken::AdvType::COMPOUND);
    eat(IToken::BEGIN);
    std::vector<std::unique_ptr<AST>> stList = syntaxStList();
    eat(IToken::END);

    return ASTFactory::createAST(beginTok, std::move(stList));
}

std::vector<std::unique_ptr<AST>> SyntaxAnalyzer::syntaxStList(void){
    std::unique_ptr<AST> stPTR = syntaxSt();

    std::vector<std::unique_ptr<AST>> stList;
    stList.emplace_back(std::move(stPTR));

    while(getCurTok().getType() == IToken::SEMI){
        eat(getCurTok().getType());
        stList.emplace_back(syntaxSt());
    }

    if(getCurTok().getType() == IToken::ID)
        throw std::invalid_argument(fmt::format("Неожиданный индентификатор: {}", getCurTok().getInfo()));

    return stList;
}

std::unique_ptr<AST> SyntaxAnalyzer::syntaxSt(void){
    /* syntaxSt ::= (syntaxCompoundSt|'<ID>'(syntaxAssignSt|syntaxCallSt)|syntaxWhileSt|syntaxIfSt|syntaxEmptySt) */
    switch(getCurTok().getType()) {
        case IToken::BEGIN :
            return syntaxCompoundSt();
        case IToken::ID :{
            if(lookFoward().getType() == IToken::ASSIGN)
                return syntaxAssignSt();
            else if(lookFoward().getType() == IToken::LPAREN)
                return syntaxCallSt();
            else{
                // Специально форсим, будто ждали присваивание
                eat(IToken::ID);
                eat(IToken::ASSIGN);
            }
        }
        case IToken::WHILE :
            return syntaxWhileSt();
        case IToken::IF :
            return syntaxIfSt();
        default:
            return syntaxEmptySt();
    }
}

std::unique_ptr<AST> SyntaxAnalyzer::syntaxIfSt(void){
    /* syntaxIfSt ::= 'IF' syntaxExpr 'THEN' syntaxSt ('ELSE' syntaxSt)? */
    Token ifTok = getCurTok();
    eat(IToken::IF);
    std::unique_ptr<AST> conditionPTR = syntaxExpr();

    eat(IToken::THEN);
    std::unique_ptr<AST> bodyPTR = syntaxSt();

    std::unique_ptr<AST> elseBodyPTR;
    if(getCurTok().getType() == IToken::ELSE){
        eat(IToken::ELSE);
        elseBodyPTR = std::move(syntaxSt());
    }
    return ASTFactory::createAST(ifTok, std::move(conditionPTR), std::move(bodyPTR), std::move(elseBodyPTR));
}

std::unique_ptr<AST> SyntaxAnalyzer::syntaxWhileSt(void){
    /* syntaxWhileSt ::= 'WHILE' syntaxExpr 'DO' syntaxExpr */
    Token whileTok = getCurTok();
    eat(IToken::WHILE);
    std::unique_ptr<AST> conditionPTR = syntaxExpr();
    eat(IToken::DO);
    std::unique_ptr<AST> bodyPTR = syntaxSt();
    return ASTFactory::createAST(whileTok, std::move(conditionPTR), std::move(bodyPTR));
}

std::unique_ptr<AST> SyntaxAnalyzer::syntaxCallSt(void){
    /* syntaxCallSt ::= '<ID>' '(' (syntaxExpr ','*)* ')' */
    Token nameTok = getCurTok();
    eat(IToken::ID);
    eat(IToken::LPAREN);
    std::vector<std::unique_ptr<AST>> paramsList;
    if(getCurTok().getType() != IToken::RPAREN){
        std::unique_ptr<AST> paramPTR = std::move(syntaxExpr());
        paramsList.emplace_back(std::move(paramPTR));
    }
    while(getCurTok().getType() == IToken::COMMA){
        eat(IToken::COMMA);
        std::unique_ptr<AST> paramPTR = std::move(syntaxExpr());
        paramsList.emplace_back(std::move(paramPTR));
    }
    
    eat(IToken::RPAREN);
    return ASTFactory::createAST(nameTok, std::move(paramsList));
}

std::unique_ptr<AST> SyntaxAnalyzer::syntaxAssignSt(void){
    /* syntaxAssignSt ::= syntaxVariable ':=' syntaxExpr */
    std::unique_ptr<AST> lValue = std::move(syntaxVariable());
    Token assignTok = getCurTok();

    eat(IToken::ASSIGN);

    std::unique_ptr<AST> toAssign = std::move(syntaxExpr());
    return ASTFactory::createAST(assignTok, std::move(lValue), std::move(toAssign));
}

std::unique_ptr<AST> SyntaxAnalyzer::syntaxVariable(void){
    /* syntaxVariable ::= '<ID>' */
    std::unique_ptr<AST> varPTR = ASTFactory::createAST(getCurTok());
    eat(IToken::ID);
    return varPTR;
}

std::unique_ptr<AST> SyntaxAnalyzer::syntaxEmptySt(void){
    /* syntaxEmptySt ::= '$NONE$' */
    return ASTFactory::createAST(IToken::EMPTY);
}

std::unique_ptr<AST> SyntaxAnalyzer::syntaxExpr(void){
    /* syntaxExpr ::= syntaxSimpleExpr (('<'|'<='|'<>'|'='|'>'|'>=') syntaxSimpleExr)? */
    std::unique_ptr<AST> simpleExprPTR = syntaxSimpleExpr();

    Token token;
    if(isIn(getCurTok().getType(), {IToken::LESS, IToken::LESS_EQ, IToken::NEQ, IToken::EQ, IToken::MORE, IToken::MORE_EQ})){
        token = getCurTok();
        eat(token.getType());
        return ASTFactory::createAST(token, std::move(simpleExprPTR), syntaxSimpleExpr());
    } else{
        return simpleExprPTR;
    }
//    else 
//        throw std::invalid_argument(fmt::format("Ожидалась операция отношения, а получен токен типа {}", magic_enum::enum_name(getCurTok().getType())));
}

std::unique_ptr<AST> SyntaxAnalyzer::syntaxSimpleExpr(void){
    /* syntaxSimpleExpr ::= syntaxTerm (('+'|'-'|'OR') syntaxTerm)? */
    std::unique_ptr<AST> termPTR = syntaxTerm();

    while(isIn(getCurTok().getType(), {IToken::PLUS, IToken::MINUS, IToken::OR})){
        Token token = getCurTok();
        eat(token.getType());

        termPTR = ASTFactory::createAST(token, std::move(termPTR), syntaxTerm());
    }

    return termPTR;
}

std::unique_ptr<AST> SyntaxAnalyzer::syntaxTerm(void){
    /* syntaxTerm ::= syntaxFactor (('/'|'*'|'DIV'|'MOD'|'AND') syntaxFactor)? */
    std::unique_ptr<AST> factorPTR = syntaxFactor();

    while(isIn(getCurTok().getType(), {IToken::FLOAT_DIV, IToken::MUL, IToken::INTEGER_DIV, IToken::MOD, IToken::AND})){
        Token token = getCurTok();
        eat(token.getType());
        factorPTR = ASTFactory::createAST(token, std::move(factorPTR), syntaxFactor());
    }

    return factorPTR;
}

std::unique_ptr<AST> SyntaxAnalyzer::syntaxFactor(){
    /* syntaxFactor ::= ('<INTEGER_CONST>'|'<REAL_CONST>'|'<STRING_CONST>'
        |'(' syntaxExpr ')'
        |'+' syntaxFactor| '-' syntaxFactor
        |'NOT' syntaxFactor
        |syntaxCallSt| syntaxVariable) */
    Token token = getCurTok();
    if(token.getAdvType() == IToken::SOME_CONST){
        eatAdv(IToken::SOME_CONST);
        return ASTFactory::createAST(token);
    }
    switch(token.getType()) {
        case IToken::LPAREN :{
            eat(IToken::LPAREN);
            std::unique_ptr<AST> node = syntaxExpr();
            eat(IToken::RPAREN);
            return node;
        }

        case IToken::PLUS :
        case IToken::MINUS :
        case IToken::NOT :{
            eatAdv(IToken::OPERATOR);
            return ASTFactory::createAST(token, syntaxFactor());
        }

        case IToken::ID :{
            if(lookFoward().getType() == IToken::LPAREN)
                return syntaxCallSt();
            else
                return syntaxVariable();
        }
        default:
            throw std::invalid_argument(fmt::format("В syntaxFactor попал лишний токен: {}", token.getInfo()));
    }
    return nullptr;
}

std::unique_ptr<AST> SyntaxAnalyzer::parseTokens(void){
    std::unique_ptr<AST> root = std::move(syntaxProgram());
    if(getCurTok().getType() != IToken::ENDOFSTREAM)
        throw std::invalid_argument(fmt::format("Не вся программа просканированна: ", getCurTok().getInfo()));
    return root;
}

void SyntaxAnalyzer::getNextToken(void){
    currentIndex++;
}

Token &SyntaxAnalyzer::getCurTok(void){
    return tokens[currentIndex];
}

Token &SyntaxAnalyzer::lookFoward(void){
    if(currentIndex + 1 >= tokens.size())
        throw std::invalid_argument("Неожиданный конец файла!");
    return tokens[currentIndex + 1];
}

void SyntaxAnalyzer::eat(IToken::Type const type){
    if(getCurTok().getType() == type){
        getNextToken();
    } else{
        throw std::invalid_argument(fmt::format("Ошибка при обработке синтаксиса! Ожидался {}, а получен {}", magic_enum::enum_name(type), getCurTok().getInfo()));
    }
}

void SyntaxAnalyzer::eatAdv(IToken::AdvType const type){
    if(getCurTok().getAdvType() == type){
        getNextToken();
    } else{
        throw std::invalid_argument(fmt::format("Ошибка при обработке синтаксиса! Ожидался {}, а получен {}", magic_enum::enum_name(type), getCurTok().getInfo()));
    }
}
