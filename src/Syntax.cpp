#include "Syntax.hpp"


SyntaxAnalyzer::SyntaxAnalyzer(List<Token> const &tokens) : tokens(tokens) {};

AST *SyntaxAnalyzer::syntaxProgram(void){
    /* syntaxProgram ::= 'PROGRAM' 'ID' ';' syntaxBlock '.'  */
    eat(IToken::PROGRAM);
    Token nameTok;
    eat(IToken::ID);
    eat(IToken::SEMI);
    ASTptr blockPTR = syntaxBlock();
    eat(IToken::DOT);
    return new ProgramAST(nameTok, blockPTR);
}

ASTptr SyntaxAnalyzer::syntaxBlock(void){
    /* syntaxBlock ::= syntaxConsts* syntaxVars* syntaxCompoundSt */
    std::vector<ASTptr> constsList = syntaxConsts();
    std::vector<ASTptr> declsList = syntaxVars();
    ASTptr compoundPTR = syntaxCompoundSt();
    return new BlockAST(constsList, declsList, compoundPTR);
}

std::vector<ASTptr> SyntaxAnalyzer::syntaxConsts(void){
    /* syntaxConsts ::= 'CONST' (syntaxConstDecl ';')+ */
    std::vector<ASTptr> constsList;
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
    ASTptr constNamePTR = new VarAST(getCurTok());
    eat(IToken::ID);

    eat(IToken::EQ);

    ASTptr constValPTR;
    Token constValTok = getCurTok();
    if(isIn(getCurTok().getType(), {IToken::INTEGER_CONST, IToken::REAL_CONST, IToken::STRING_CONST})){
        if(isIn(getCurTok().getType(), {IToken::INTEGER_CONST, IToken::REAL_CONST})){
            constValPTR = new NumberAST(constValTok);
        } else{
            constValPTR = new StringAST(constValTok);
        }
        eat(getCurTok().getType());
    }

    return new ConstAST(constNamePTR, constValPTR);
}

std::vector<ASTptr> SyntaxAnalyzer::syntaxVars(void){
    /* syntaxVars ::= 'VAR' (syntaxVarDecl ';')+ */
    std::vector<ASTptr> declsList;
    if(getCurTok().getType() == IToken::VAR){
        eat(IToken::VAR);
        while(getCurTok().getType() == IToken::ID){
            std::vector<ASTptr> varDeclList = syntaxVarDecl();
            for(auto varDeclPTR : varDeclList)
                declsList.push_back(varDeclPTR);
            eat(IToken::SEMI);
        }
    }
    return declsList;
}

std::vector<ASTptr> SyntaxAnalyzer::syntaxVarDecl(void){
    /* syntaxVarDecl ::= ('<ID>' ','*)+ ':' syntaxTypeSpec */
    std::vector<ASTptr> varsList;
    varsList.push_back(new VarAST(getCurTok()));
    eat(IToken::ID);

    while(getCurTok().getType() == IToken::COMMA){
        eat(IToken::COMMA);
        varsList.push_back(new VarAST(getCurTok()));
        eat(IToken::ID);
    }

    eat(IToken::COLON);

    ASTptr typeNodePTR = syntaxTypeSpec();
    std::vector<ASTptr> varDeclsList;
    for(auto var : varsList){
        varDeclsList.push_back(new VarDeclAST(var, typeNodePTR));
    }
    return varDeclsList;
}

ASTptr SyntaxAnalyzer::syntaxTypeSpec(void){
    /* syntaxTypeSpec ::= ('INTEGER'|'REAL') */
    Token typeTok = getCurTok();
    switch (getCurTok().getType()){
        case IToken::INTEGER:
            eat(IToken::INTEGER);
            break;
        case IToken::REAL:
            eat(IToken::REAL);
            break;
        default:
            throw std::invalid_argument(fmt::format("Ожидалось объявление типа, а получен токен типа \033[31m {}\033[0m", getCurTok().getInfo()));
            break;
    }
    return new TypeSpecAST(typeTok);
}

AST *SyntaxAnalyzer::syntaxCompoundSt(void){
    /* syntaxCompoundSt ::= 'BEGIN' (syntaxSt ';')* 'END' */
    eat(IToken::BEGIN);
    std::vector<ASTptr> stList = syntaxStList();
    eat(IToken::END);

    CompoundAST *compoundPTR = new CompoundAST();
    for(auto stPTR : stList)
        compoundPTR->addChild(stPTR);
    
    return compoundPTR;
}

std::vector<ASTptr> SyntaxAnalyzer::syntaxStList(void){
    AST *stPTR = syntaxSt();

    std::vector<ASTptr> stList = {stPTR};

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
    eat(IToken::IF);
    ASTptr conditionPTR = syntaxExpr();
    eat(IToken::THEN);
    ASTptr bodyPTR = syntaxSt();
    ASTptr elseBodyPTR = nullptr;
    if(getCurTok().getType() == IToken::ELSE){
        eat(IToken::ELSE);
        elseBodyPTR = syntaxSt();
    }
    return new ifAST(conditionPTR, bodyPTR, elseBodyPTR);
}

AST *SyntaxAnalyzer::syntaxWhileSt(void){
    /* syntaxWhileSt ::= 'WHILE' syntaxExpr 'DO' syntaxExpr */
    eat(IToken::WHILE);
    ASTptr conditionPTR = syntaxExpr();
    eat(IToken::DO);
    ASTptr bodyPTR = syntaxSt();
    return new whileAST(conditionPTR, bodyPTR);
}

ASTptr SyntaxAnalyzer::syntaxCallSt(void){
    /* syntaxCallSt ::= '<ID>' '(' (syntaxExpr ','*)* ')' */
    Token nameTok = getCurTok();
    eat(IToken::ID);
    eat(IToken::LPAREN);
    std::vector<ASTptr> paramsList;
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
    return new CallAST(nameTok, paramsList);
}

ASTptr SyntaxAnalyzer::syntaxAssignSt(void){
    /* syntaxAssignSt ::= syntaxVariable ':=' syntaxExpr */
    ASTptr lValue = syntaxVariable();
    Token assignTok = getCurTok();

    eat(IToken::ASSIGN);

    ASTptr toAssign = syntaxExpr();
    return new AssignAST(lValue, assignTok, toAssign);
}

ASTptr SyntaxAnalyzer::syntaxVariable(void){
    /* syntaxVariable ::= '<ID>' */
    ASTptr varPTR = new VarAST(getCurTok());
    eat(IToken::ID);
    return varPTR;
}

ASTptr SyntaxAnalyzer::syntaxEmptySt(void){
    /* syntaxEmptySt ::= '$NONE$' */
    return new NoOpAST({"$", IToken::EMPTY});
}

AST *SyntaxAnalyzer::syntaxExpr(void){
    /* syntaxExpr ::= syntaxSimpleExpr (('<'|'<='|'<>'|'='|'>'|'>=') syntaxSimpleExr)? */
    AST *simpleExprPTR = syntaxSimpleExpr();

    Token token;
    if(isIn(getCurTok().getType(), {IToken::LESS, IToken::LESS_EQ, IToken::NEQ, IToken::EQ, IToken::MORE, IToken::MORE_EQ})){
        token = getCurTok();
        eat(token.getType());
        return new BinOpAST(simpleExprPTR, token, syntaxSimpleExpr());
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

        termPTR = new BinOpAST(termPTR, token, syntaxTerm());
    }

    return termPTR;
}

AST *SyntaxAnalyzer::syntaxTerm(void){
    /* syntaxTerm ::= syntaxFactor (('/'|'*'|'DIV'|'MOD'|'AND') syntaxFactor)? */
    AST *factorPTR = syntaxFactor();

    while(isIn(getCurTok().getType(), {IToken::FLOAT_DIV, IToken::MUL, IToken::INTEGER_DIV, IToken::MOD, IToken::AND})){
        Token token = getCurTok();
        eat(token.getType());
        factorPTR = new BinOpAST(factorPTR, token, syntaxFactor());
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
    switch(token.getType()) {
        case IToken::INTEGER_CONST :{
            eat(IToken::INTEGER_CONST);
            return new NumberAST(token);
        }
        case IToken::REAL_CONST :{
            eat(IToken::REAL_CONST);
            return new NumberAST(token);
        }
        case IToken::STRING_CONST :{
            eat(IToken::STRING_CONST);
            return new StringAST(token);
        }
        case IToken::LPAREN :{
            eat(IToken::LPAREN);
            AST *node = syntaxExpr();
            eat(IToken::RPAREN);
            return node;
        }
        case IToken::PLUS :{
            eat(IToken::PLUS);
            return new UnOpAST(token, syntaxFactor());
        }
        case IToken::MINUS :{
            eat(IToken::MINUS);
            return new UnOpAST(token, syntaxFactor());
        }
        case IToken::NOT :{
            eat(IToken::NOT);
            return new UnOpAST(token, syntaxFactor());
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

void SyntaxAnalyzer::eat(IToken::Type type){
    if(getCurTok().getType() == type){
        getNextToken();
    } else{
        throw std::invalid_argument(fmt::format("Ошибка при обработке синтаксиса! Ожидался {}, а получен {}", magic_enum::enum_name(type), getCurTok().getInfo()));
    }
}
