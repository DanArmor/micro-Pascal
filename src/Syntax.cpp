#include "Syntax.hpp"

#include <fmt/format.h>

#include "AST.hpp"
#include "ASTFactory.hpp"
#include "SyntExp.hpp"

SyntaxAnalyzer::SyntaxAnalyzer(List<Token> const &tokens) : tokens(tokens) {};

std::unique_ptr<AST> SyntaxAnalyzer::parseTokens(void){
    std::unique_ptr<AST> root = std::move(syntaxProgram());
    if(getCurTok().getType() != IToken::ENDOFSTREAM)
        throw SyntaxException(getCurTok(), "Анализ программы завершился до встречи специального символа <конца потока токенов>!");
    return root;
}

std::unique_ptr<AST> SyntaxAnalyzer::syntaxProgram(void){
    /* syntaxProgram ::= 'PROGRAM' 'ID' ';' syntaxBlock '.'  */
    eat(IToken::PROGRAM);
    Token &nameTok = getCurTok();
    nameTok.setAdvType(IToken::PROGRAM_NAME);

    eat(IToken::ID);
    eat(IToken::SEMI);
    std::vector<std::unique_ptr<AST>> functions;

    while(isIn(getCurTok().getType(), {IToken::PROCEDURE, IToken::FUNCTION})){
        functions.emplace_back(std::move(syntaxFuncDef()));
        eat(IToken::SEMI);
    }

    std::unique_ptr<AST> blockPTR = syntaxBlock();
    eat(IToken::DOT);
    return ASTFactory::createAST(nameTok, std::move(functions), std::move(blockPTR));
}

std::unique_ptr<AST> SyntaxAnalyzer::syntaxFuncDef(void){
    Token funcTok = getCurTok();
    if(funcTok.getType() == IToken::FUNCTION)
        eat(IToken::FUNCTION);
    else
        eat(IToken::PROCEDURE);
    
    Token &name = getCurTok();

    name.setAdvType(IToken::FUNCTION_NAME);
    eat(IToken::ID);
    eat(IToken::LPAREN);
    std::vector<std::unique_ptr<AST>> params;
    while(getCurTok().getType() != IToken::RPAREN){
        std::vector<std::unique_ptr<AST>> sup = std::move(syntaxVarDecl());
        for(auto &ptr : sup)
            params.emplace_back(std::move(ptr));
        if(getCurTok().getType() == IToken::COMMA)
            eat(IToken::COMMA);
    }
    eat(IToken::RPAREN);

    std::unique_ptr<AST> returnType;
    if(funcTok.getType() == IToken::FUNCTION){
        eat(IToken::COLON);
        returnType = syntaxTypeSpec();
    } else{
        Token typeTok = {"VOID", IToken::VOID, IToken::TYPE_SPEC};
        returnType = ASTFactory::createAST(typeTok);
    }
    eat(IToken::SEMI);
    std::unique_ptr<AST> body = std::move(syntaxBlock());
    return ASTFactory::createAST(name, std::move(params), std::move(returnType), std::move(body));
}

std::unique_ptr<AST> SyntaxAnalyzer::syntaxBlock(void){
    /* syntaxBlock ::= syntaxConsts* syntaxVars* syntaxCompoundSt */
    std::vector<std::unique_ptr<AST>> constsList = syntaxConsts();
    std::vector<std::unique_ptr<AST>> declsList = syntaxVars();
    std::unique_ptr<AST> compoundPTR = syntaxCompoundSt();
    return ASTFactory::createAST({"", IToken::BLOCK, IToken::UNKNOWN}, std::move(constsList), std::move(declsList), std::move(compoundPTR));
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
    Token &tok = getCurTok();
    tok.setAdvType(IToken::VAR_NAME);
    std::unique_ptr<AST> constNamePTR = ASTFactory::createAST(getCurTok());
    eat(IToken::ID);
    eat(IToken::EQ);

    Token constValTok = getCurTok();
    std::unique_ptr<AST> constValPTR = syntaxExpr();

    constValTok.setAdvType(IToken::AdvType::CONSTDECL);

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
    Token &tok = getCurTok();
    tok.setAdvType(IToken::VAR_NAME);
    varsList.emplace_back(ASTFactory::createAST(tok));
    eat(IToken::ID);

    while(getCurTok().getType() == IToken::COMMA){
        eat(IToken::COMMA);
        Token &tok = getCurTok();
        tok.setAdvType(IToken::VAR_NAME);
        varsList.emplace_back(ASTFactory::createAST(tok));
        eat(IToken::ID);
    }
    eat(IToken::COLON);

    std::unique_ptr<AST> typeNodePTR(syntaxTypeSpec());

    std::vector<std::unique_ptr<AST>> varDeclsList;
    for(auto &var : varsList){
        varDeclsList.emplace_back(ASTFactory::createAST({"", IToken::EMPTY, IToken::VARDECL}, std::move(var), createCopyOfType(typeNodePTR.get())));
    }
    return varDeclsList;
}

std::unique_ptr<AST> SyntaxAnalyzer::syntaxTypeSpec(void){
    /* syntaxTypeSpec ::= ('integer'|'real'|'string'|'array' '[' '<INTEGER_CONST>' '..' '<INTEGER_CONST>' ']' of syntaxTypeSpec) */
    Token typeTok = getCurTok();

    if(typeTok.getType() != IToken::ARRAY){
        eatAdv(IToken::TYPE_SPEC);
        return ASTFactory::createAST(typeTok);
    }else{
        Token arrTok = getCurTok();
        eat(IToken::ARRAY);
        eat(IToken::LSQBRACKET);
        Token lHandTok = getCurTok();
        eat(IToken::INTEGER_CONST);
        eat(IToken::RANGE);
        Token rHandTok = getCurTok();
        eat(IToken::INTEGER_CONST);
        eat(IToken::RSQBRACKET);
        eat(IToken::OF);
        std::unique_ptr<AST> subType = syntaxTypeSpec();
        return ASTFactory::createAST(arrTok, lHandTok, rHandTok, std::move(subType));
    }
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
        eat(IToken::SEMI);
        stList.emplace_back(syntaxSt());
    }

    if(getCurTok().getType() == IToken::ID)
        throw SyntaxException(getCurTok(), "Неожиданный индентификатор:");

    return stList;
}

std::unique_ptr<AST> SyntaxAnalyzer::syntaxSt(void){
    /* syntaxSt ::= (syntaxCompoundSt|'<ID>'(syntaxAssignSt|syntaxCallSt)|syntaxWhileSt|syntaxIfSt|syntaxEmptySt) */
    switch(getCurTok().getType()) {
        case IToken::BEGIN :
            return syntaxCompoundSt();
        case IToken::ID :{
            if(lookFoward().getType() == IToken::ASSIGN || lookFoward().getType() == IToken::LSQBRACKET)
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
        case IToken::FOR :
            return syntaxForSt();
        case IToken::RETURN :
            return syntaxReturnSt();
        default:
            return syntaxEmptySt();
    }
}

std::unique_ptr<AST> SyntaxAnalyzer::syntaxEmptySt(void){
    /* syntaxEmptySt ::= '$NONE$' */
    return ASTFactory::createAST({"", IToken::EMPTY, IToken::UNKNOWN});
}

std::unique_ptr<AST> SyntaxAnalyzer::syntaxAssignSt(void){
    /* syntaxAssignSt ::= syntaxVariable ':=' syntaxExpr */
    std::unique_ptr<AST> lValue;
    if(lookFoward().getType() == IToken::LSQBRACKET)
        lValue = std::move(syntaxSelect());
    else
        lValue = std::move(syntaxVariable());
    Token assignTok = getCurTok();
    eat(IToken::ASSIGN);

    std::unique_ptr<AST> toAssign = std::move(syntaxExpr());
    return ASTFactory::createAST(assignTok, std::move(lValue), std::move(toAssign));
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

std::unique_ptr<AST> SyntaxAnalyzer::syntaxForSt(void){
    /* syntaxForSt ::= 'for' syntaxIter 'do' syntaxSt */
    Token token = getCurTok();
    eat(IToken::FOR);
    std::unique_ptr<AST> iterSt = syntaxIter();
    eat(IToken::DO);
    std::unique_ptr<AST> body = syntaxSt();
    return ASTFactory::createAST(token, std::move(iterSt), std::move(body));
}

std::unique_ptr<AST> SyntaxAnalyzer::syntaxCallSt(void){
    /* syntaxCallSt ::= '<ID>' '(' (syntaxExpr ','*)* ')' */
    Token &nameTok = getCurTok();
    nameTok.setAdvType(IToken::FUNCTION_NAME);
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

std::unique_ptr<AST> SyntaxAnalyzer::syntaxReturnSt(void){
    Token retTok = getCurTok();
    eat(IToken::RETURN);
    std::unique_ptr<AST> toReturn = std::move(syntaxExpr());
    return ASTFactory::createAST(retTok, std::move(toReturn));
}

std::unique_ptr<AST> SyntaxAnalyzer::syntaxVariable(void){
    /* syntaxVariable ::= '<ID>' */
    getCurTok().setAdvType(IToken::VAR_NAME);
    std::unique_ptr<AST> varPTR = ASTFactory::createAST(getCurTok());
    eat(IToken::ID);
    return varPTR;
}

std::unique_ptr<AST> SyntaxAnalyzer::syntaxSelect(void){
    Token tok = getCurTok();
    tok.setAdvType(IToken::SELECT);
    std::unique_ptr<AST> var = std::move(syntaxVariable());
    std::vector<std::unique_ptr<AST>> selections;
    while(getCurTok().getType() == IToken::LSQBRACKET){
        eat(IToken::LSQBRACKET);
        std::unique_ptr<AST> index = std::move(syntaxExpr());
        eat(IToken::RSQBRACKET);
        selections.emplace_back(std::move(index));
    }
    if(selections.size() != 1)
        for(std::size_t i = 0; i < selections.size() - 1; i++){
            var = ASTFactory::createAST(tok, std::move(var), std::move(selections[i]));
        }
    return ASTFactory::createAST(tok, std::move(var), std::move(selections[selections.size()-1]));
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
            else if (lookFoward().getType() == IToken::LSQBRACKET)
                return syntaxSelect();
            else
                return syntaxVariable();
        }
        default:
            throw SyntaxException(token, "Во время обработки выражения syntaxFactor встречен неожиданный токен. ");
    }
    return nullptr;
}

std::unique_ptr<AST> SyntaxAnalyzer::syntaxIter(void){
    /* syntaxIter ::= syntaxAssignSt ('to'|'down to') ('<INTEGER_CONST>'|syntaxVariable) */
    Token varTok = getCurTok();
    std::unique_ptr<AST> initSt = syntaxAssignSt();

    Token toTok = getCurTok();
    if(toTok.getType() == IToken::DOWNTO)
        eat(IToken::DOWNTO);
    else
        eat(IToken::TO);

    Token finTok = getCurTok();
    if(finTok.getAdvType() != IToken::SOME_CONST and finTok.getType() != IToken::ID)
        throw AnalyzeException(finTok, "Конечным значением цикла for должна быть целая константа или переменная!");
    std::unique_ptr<AST> finPtr = syntaxFactor();

    Token condOp = {"<=", IToken::LESS_EQ, IToken::OPERATOR};
    std::unique_ptr<AST> varPtr_condition = ASTFactory::createAST(varTok);
    std::unique_ptr<AST> finPtr_condition = ASTFactory::createAST(finTok);
    std::unique_ptr<AST> condition = ASTFactory::createAST(condOp, std::move(varPtr_condition), std::move(finPtr_condition));

    Token postOp = {"+", IToken::PLUS, IToken::OPERATOR};
    if(toTok.getType() == IToken::DOWNTO)
        postOp = {"-", IToken::MINUS, IToken::OPERATOR};
    Token forOp = {"1", IToken::INTEGER_CONST, IToken::SOME_CONST};
    std::unique_ptr<AST> varPtr_postOp = ASTFactory::createAST(varTok);
    std::unique_ptr<AST> finPtr_postOp = ASTFactory::createAST(forOp);
    std::unique_ptr<AST> postAction = ASTFactory::createAST(postOp, std::move(varPtr_postOp), std::move(finPtr_postOp));

    return ASTFactory::createAST(toTok, std::move(initSt), std::move(condition), std::move(postAction)); 
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
        throw SyntaxException(getCurTok(), fmt::format("Ошибка при обработке синтаксиса! Ожидался тип {}! Встречен ", magic_enum::enum_name(type)));
    }
}

void SyntaxAnalyzer::eatAdv(IToken::AdvType const type){
    if(getCurTok().getAdvType() == type){
        getNextToken();
    } else{
        throw SyntaxException(getCurTok(), fmt::format("Ошибка при обработке синтаксиса! Ожидался тип {}! Встречен ", magic_enum::enum_name(type)));
    }
}

List<Token> SyntaxAnalyzer::getTokens(void){
    return tokens;
}

std::unique_ptr<AST> SyntaxAnalyzer::createCopyOfType(AST *ptr){
    if(ptr->token.getAdvType() == IToken::TYPE_SPEC || ptr->token.getType() == IToken::ARRAY){
        if(ptr->token.getType() == IToken::ARRAY){
            ArrSpecAST *rawPtr = static_cast<ArrSpecAST*>(ptr);
            std::unique_ptr<AST> arrPtr = ASTFactory::createAST(rawPtr->token, rawPtr->lHandTok, rawPtr->rHandTok, createCopyOfType(rawPtr->subType.get()));
            return arrPtr;
        } else{
            return ASTFactory::createAST(ptr->token);
        }
    } else{
        throw std::logic_error("Применение createCopyOfType к узлу, не являющемуся типом! " + ptr->token.getInfo());
    }
}