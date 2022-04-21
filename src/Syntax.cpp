#include "Syntax.hpp"

#include <fmt/format.h>

#include "AST.hpp"
#include "ASTFactory.hpp"

SyntaxAnalyzer::SyntaxAnalyzer(List<Token> const &tokens) : tokens(tokens) {};

AST *SyntaxAnalyzer::syntaxProgram(void){
    /* syntaxProgram ::= 'PROGRAM' 'ID' ';' syntaxBlock '.'  */
    eat(IToken::PROGRAM);
    Token nameTok = getCurTok();
    nameTok.setAdvType(IToken::PROGSTART);
    eat(IToken::ID);
    eat(IToken::SEMI);
    ASTptr blockPTR = syntaxBlock();
    eat(IToken::DOT);
    return ASTFactory::createAST(nameTok, blockPTR);
}

ASTptr SyntaxAnalyzer::syntaxBlock(void){
    /* syntaxBlock ::= syntaxConsts* syntaxVars* syntaxCompoundSt */
    std::vector<AST*> constsList = syntaxConsts();
    std::vector<AST*> declsList = syntaxVars();
    ASTptr compoundPTR = syntaxCompoundSt();
    return ASTFactory::createAST(IToken::Type::BLOCK, constsList, declsList, compoundPTR);
}

std::vector<AST*> SyntaxAnalyzer::syntaxConsts(void){
    /* syntaxConsts ::= 'CONST' (syntaxConstDecl ';')+ */
    std::vector<AST*> constsList;
    if(getCurTok().getType() == IToken::CONST){
        eat(IToken::CONST);
        while(getCurTok().getType() == IToken::ID){
            constsList.push_back(syntaxConstDecl());
            eat(IToken::SEMI);
        }
    }
    return constsList;
}

ASTptr SyntaxAnalyzer::syntaxConstDecl(void){
    /* syntaxConstDecl ::= '<ID>' '=' ('<INTEGER_CONST>'|'<REAL_CONST>'|'<STRING_CONST>') */
    ASTptr constNamePTR = ASTFactory::createAST(getCurTok());
    eat(IToken::ID);
    eat(IToken::EQ);

    Token constValTok = getCurTok();
    ASTptr constValPTR = ASTFactory::createAST(constValTok);

    constValTok.setAdvType(IToken::AdvType::CONSTDECL);
    eatAdv(IToken::SOME_CONST);

    return ASTFactory::createAST(constValTok, constNamePTR, constValPTR);
}

std::vector<AST*> SyntaxAnalyzer::syntaxVars(void){
    /* syntaxVars ::= 'VAR' (syntaxVarDecl ';')+ */
    std::vector<AST*> declsList;
    if(getCurTok().getType() == IToken::VAR){
        eat(IToken::VAR);
        while(getCurTok().getType() == IToken::ID){
            std::vector<AST*> varDeclList = syntaxVarDecl();
            for(auto varDeclPTR : varDeclList)
                declsList.push_back(varDeclPTR);
            eat(IToken::SEMI);
        }
    }
    return declsList;
}

std::vector<AST*> SyntaxAnalyzer::syntaxVarDecl(void){
    /* syntaxVarDecl ::= ('<ID>' ','*)+ ':' syntaxTypeSpec */
    std::vector<AST*> varsList;
    varsList.push_back(ASTFactory::createAST(getCurTok()));
    eat(IToken::ID);

    while(getCurTok().getType() == IToken::COMMA){
        eat(IToken::COMMA);
        varsList.push_back(ASTFactory::createAST(getCurTok()));
        eat(IToken::ID);
    }
    eat(IToken::COLON);

    std::unique_ptr<AST> typeNodePTR(syntaxTypeSpec());

    std::vector<AST*> varDeclsList;
    for(auto var : varsList){
        varDeclsList.push_back(ASTFactory::createAST(IToken::VARDECL, var, ASTFactory::createAST(typeNodePTR->token)));
    }
    return varDeclsList;
}

ASTptr SyntaxAnalyzer::syntaxTypeSpec(void){
    /* syntaxTypeSpec ::= ('INTEGER'|'REAL') */
    Token typeTok = getCurTok();
    eatAdv(IToken::TYPE_SPEC);
    return ASTFactory::createAST(typeTok);
}

AST *SyntaxAnalyzer::syntaxCompoundSt(void){
    /* syntaxCompoundSt ::= 'BEGIN' (syntaxSt ';')* 'END' */
    Token beginTok = getCurTok();
    beginTok.setAdvType(IToken::AdvType::COMPOUND);
    eat(IToken::BEGIN);
    std::vector<AST*> stList = syntaxStList();
    eat(IToken::END);

    return ASTFactory::createAST(beginTok, stList);
}

std::vector<AST*> SyntaxAnalyzer::syntaxStList(void){
    AST *stPTR = syntaxSt();

    std::vector<AST*> stList;
    stList.push_back(stPTR);

    while(getCurTok().getType() == IToken::SEMI){
        eat(getCurTok().getType());
        stList.push_back(syntaxSt());
    }

    if(getCurTok().getType() == IToken::ID)
        throw std::invalid_argument(fmt::format("Неожиданный индентификатор: {}", getCurTok().getInfo()));

    return stList;
}

ASTptr SyntaxAnalyzer::syntaxSt(void){
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

AST *SyntaxAnalyzer::syntaxIfSt(void){
    /* syntaxIfSt ::= 'IF' syntaxExpr 'THEN' syntaxSt ('ELSE' syntaxSt)? */
    Token ifTok = getCurTok();
    eat(IToken::IF);
    ASTptr conditionPTR = syntaxExpr();

    eat(IToken::THEN);
    ASTptr bodyPTR = syntaxSt();

    ASTptr elseBodyPTR = nullptr;
    if(getCurTok().getType() == IToken::ELSE){
        eat(IToken::ELSE);
        elseBodyPTR = syntaxSt();
    }
    return ASTFactory::createAST(ifTok, conditionPTR, bodyPTR, elseBodyPTR);
}

AST *SyntaxAnalyzer::syntaxWhileSt(void){
    /* syntaxWhileSt ::= 'WHILE' syntaxExpr 'DO' syntaxExpr */
    Token whileTok = getCurTok();
    eat(IToken::WHILE);
    ASTptr conditionPTR = syntaxExpr();
    eat(IToken::DO);
    ASTptr bodyPTR = syntaxSt();
    return ASTFactory::createAST(whileTok, conditionPTR, bodyPTR);
}

ASTptr SyntaxAnalyzer::syntaxCallSt(void){
    /* syntaxCallSt ::= '<ID>' '(' (syntaxExpr ','*)* ')' */
    Token nameTok = getCurTok();
    eat(IToken::ID);
    eat(IToken::LPAREN);
    std::vector<AST*> paramsList;
    if(getCurTok().getType() != IToken::RPAREN){
        ASTptr paramPTR = syntaxExpr();
        paramsList.push_back(paramPTR);
    }
    while(getCurTok().getType() == IToken::COMMA){
        eat(IToken::COMMA);
        ASTptr paramPTR = syntaxExpr();
        paramsList.push_back(paramPTR);
    }
    
    eat(IToken::RPAREN);
    return ASTFactory::createAST(nameTok, paramsList);
}

ASTptr SyntaxAnalyzer::syntaxAssignSt(void){
    /* syntaxAssignSt ::= syntaxVariable ':=' syntaxExpr */
    ASTptr lValue = syntaxVariable();
    Token assignTok = getCurTok();

    eat(IToken::ASSIGN);

    ASTptr toAssign = syntaxExpr();
    return ASTFactory::createAST(assignTok, lValue, toAssign);
}

ASTptr SyntaxAnalyzer::syntaxVariable(void){
    /* syntaxVariable ::= '<ID>' */
    ASTptr varPTR = ASTFactory::createAST(getCurTok());
    eat(IToken::ID);
    return varPTR;
}

ASTptr SyntaxAnalyzer::syntaxEmptySt(void){
    /* syntaxEmptySt ::= '$NONE$' */
    return ASTFactory::createAST(IToken::EMPTY);
}

AST *SyntaxAnalyzer::syntaxExpr(void){
    /* syntaxExpr ::= syntaxSimpleExpr (('<'|'<='|'<>'|'='|'>'|'>=') syntaxSimpleExr)? */
    AST *simpleExprPTR = syntaxSimpleExpr();

    Token token;
    if(isIn(getCurTok().getType(), {IToken::LESS, IToken::LESS_EQ, IToken::NEQ, IToken::EQ, IToken::MORE, IToken::MORE_EQ})){
        token = getCurTok();
        eat(token.getType());
        return ASTFactory::createAST(token, simpleExprPTR, syntaxSimpleExpr());
    } else{
        return simpleExprPTR;
    }
//    else 
//        throw std::invalid_argument(fmt::format("Ожидалась операция отношения, а получен токен типа {}", magic_enum::enum_name(getCurTok().getType())));
}

AST *SyntaxAnalyzer::syntaxSimpleExpr(void){
    /* syntaxSimpleExpr ::= syntaxTerm (('+'|'-'|'OR') syntaxTerm)? */
    AST *termPTR = syntaxTerm();

    while(isIn(getCurTok().getType(), {IToken::PLUS, IToken::MINUS, IToken::OR})){
        Token token = getCurTok();
        eat(token.getType());

        termPTR = ASTFactory::createAST(token, termPTR, syntaxTerm());
    }

    return termPTR;
}

AST *SyntaxAnalyzer::syntaxTerm(void){
    /* syntaxTerm ::= syntaxFactor (('/'|'*'|'DIV'|'MOD'|'AND') syntaxFactor)? */
    AST *factorPTR = syntaxFactor();

    while(isIn(getCurTok().getType(), {IToken::FLOAT_DIV, IToken::MUL, IToken::INTEGER_DIV, IToken::MOD, IToken::AND})){
        Token token = getCurTok();
        eat(token.getType());
        factorPTR = ASTFactory::createAST(token, factorPTR, syntaxFactor());
    }

    return factorPTR;
}

AST *SyntaxAnalyzer::syntaxFactor(){
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
            AST *node = syntaxExpr();
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

AST *SyntaxAnalyzer::parseTokens(void){
    ASTptr root = syntaxProgram();
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
