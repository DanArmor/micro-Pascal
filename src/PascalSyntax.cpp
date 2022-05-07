#include "PascalSyntax.hpp"

#include <fmt/format.h>

#include "ASTFactory.hpp"
#include "SyntExp.hpp"
#include "Syntax.hpp"
#include "supAlgs.hpp"

PascalSyntaxAnalyzer::PascalSyntaxAnalyzer(){}

PascalSyntaxAnalyzer::PascalSyntaxAnalyzer(List<Token> const &tokens)
    : tokens(tokens) {}

std::unique_ptr<IAST> PascalSyntaxAnalyzer::analyzeTokens(void) {
    std::unique_ptr<IAST> root = std::move(syntaxProgram());
    if (getCurTok().getType() != BaseToken::ENDOFSTREAM)
        throw SyntaxException(getCurTok(),
                              "Анализ программы завершился до встречи "
                              "специального символа <конца потока токенов>!");
    return root;
}

std::unique_ptr<IAST> PascalSyntaxAnalyzer::syntaxProgram(void) {
    /* syntaxProgram ::= 'program' '<ID>' ';' (syntaxFuncDef)? syntaxBlock '.'  */
    eat(BaseToken::PROGRAM);
    Token &nameTok = getCurTok();
    nameTok.setAdvType(BaseToken::PROGRAM_NAME);

    eat(BaseToken::ID);
    eat(BaseToken::SEMI);
    std::vector<std::unique_ptr<IAST>> functions;

    while (isIn(getCurTok().getType(), {BaseToken::PROCEDURE, BaseToken::FUNCTION})) {
        functions.emplace_back(std::move(syntaxFuncDef()));
        eat(BaseToken::SEMI);
    }

    std::unique_ptr<IAST> blockPTR = syntaxBlock();
    eat(BaseToken::DOT);
    return ASTFactory::createAST(nameTok, std::move(functions),
                                 std::move(blockPTR));
}

std::unique_ptr<IAST> PascalSyntaxAnalyzer::syntaxFuncDef(void) {
    /* (('procedure' '<ID>' '(' syntaxVarDecl?* ')') 
       | ('function' '<ID>' '(' syntaxVarDecl?* ')' ':' syntaxTypeSpec)) ';' syntaxBlock ';' */
    Token funcTok = getCurTok();
    if (funcTok.getType() == BaseToken::FUNCTION)
        eat(BaseToken::FUNCTION);
    else
        eat(BaseToken::PROCEDURE);

    Token &name = getCurTok();

    name.setAdvType(BaseToken::FUNCTION_NAME);
    eat(BaseToken::ID);
    eat(BaseToken::LPAREN);
    std::vector<std::unique_ptr<IAST>> params;
    while (getCurTok().getType() != BaseToken::RPAREN) {
        std::vector<std::unique_ptr<IAST>> sup = std::move(syntaxVarDecl());
        for (auto &ptr : sup) params.emplace_back(std::move(ptr));
        if (getCurTok().getType() == BaseToken::COMMA) eat(BaseToken::COMMA);
    }
    eat(BaseToken::RPAREN);

    std::unique_ptr<IAST> returnType;
    if (funcTok.getType() == BaseToken::FUNCTION) {
        eat(BaseToken::COLON);
        returnType = syntaxTypeSpec();
    } else {
        Token typeTok = {"VOID", BaseToken::VOID, BaseToken::TYPE_SPEC};
        returnType = ASTFactory::createAST(typeTok);
    }
    eat(BaseToken::SEMI);
    std::unique_ptr<IAST> body = std::move(syntaxBlock());
    return ASTFactory::createAST(name, std::move(params), std::move(returnType),
                                 std::move(body));
}

std::unique_ptr<IAST> PascalSyntaxAnalyzer::syntaxBlock(void) {
    /* syntaxConsts? syntaxVars? syntaxCompoundSt */
    std::vector<std::unique_ptr<IAST>> constsList = syntaxConsts();
    std::vector<std::unique_ptr<IAST>> declsList = syntaxVars();
    std::unique_ptr<IAST> compoundPTR = syntaxCompoundSt();
    return ASTFactory::createAST({"", BaseToken::BLOCK, BaseToken::UNKNOWN},
                                 std::move(constsList), std::move(declsList),
                                 std::move(compoundPTR));
}

std::vector<std::unique_ptr<IAST>> PascalSyntaxAnalyzer::syntaxConsts(void) {
    /* syntaxConsts ::= 'const' (syntaxConstDecl ';')+ */
    std::vector<std::unique_ptr<IAST>> constsList;
    if (getCurTok().getType() == BaseToken::CONST) {
        eat(BaseToken::CONST);
        while (getCurTok().getType() == BaseToken::ID) {
            constsList.emplace_back(syntaxConstDecl());
            eat(BaseToken::SEMI);
        }
    }
    return constsList;
}

std::unique_ptr<IAST> PascalSyntaxAnalyzer::syntaxConstDecl(void) {
    /* syntaxConstDecl ::= '<ID>' '=' syntaxExpr */
    Token &tok = getCurTok();
    tok.setAdvType(BaseToken::VAR_NAME);
    std::unique_ptr<IAST> constNamePTR = ASTFactory::createAST(getCurTok());
    eat(BaseToken::ID);
    eat(BaseToken::EQ);

    Token constValTok = getCurTok();
    std::unique_ptr<IAST> constValPTR = syntaxExpr();

    constValTok.setAdvType(BaseToken::AdvType::CONSTDECL);

    return ASTFactory::createAST(constValTok, std::move(constNamePTR),
                                 std::move(constValPTR));
}

std::vector<std::unique_ptr<IAST>> PascalSyntaxAnalyzer::syntaxVars(void) {
    /* syntaxVars ::= 'var' (syntaxVarDecl ';')+ */
    std::vector<std::unique_ptr<IAST>> declsList;
    if (getCurTok().getType() == BaseToken::VAR) {
        eat(BaseToken::VAR);
        while (getCurTok().getType() == BaseToken::ID) {
            std::vector<std::unique_ptr<IAST>> varDeclList = syntaxVarDecl();
            for (auto &varDeclPTR : varDeclList)
                declsList.emplace_back(std::move(varDeclPTR));
            eat(BaseToken::SEMI);
        }
    }
    return declsList;
}

std::vector<std::unique_ptr<IAST>> PascalSyntaxAnalyzer::syntaxVarDecl(void) {
    /* '<ID>' (',' '<ID>')* ':' syntaxTypeSpec */
    std::vector<std::unique_ptr<IAST>> varsList;
    Token &tok = getCurTok();
    tok.setAdvType(BaseToken::VAR_NAME);
    varsList.emplace_back(ASTFactory::createAST(tok));
    eat(BaseToken::ID);

    while (getCurTok().getType() == BaseToken::COMMA) {
        eat(BaseToken::COMMA);
        Token &tok = getCurTok();
        tok.setAdvType(BaseToken::VAR_NAME);
        varsList.emplace_back(ASTFactory::createAST(tok));
        eat(BaseToken::ID);
    }
    eat(BaseToken::COLON);

    std::unique_ptr<IAST> typeNodePTR(syntaxTypeSpec());

    std::vector<std::unique_ptr<IAST>> varDeclsList;
    for (auto &var : varsList) {
        varDeclsList.emplace_back(ASTFactory::createAST(
            {"", BaseToken::EMPTY, BaseToken::VARDECL}, std::move(var),
            ASTFactory::createCopyOfType(typeNodePTR.get())));
    }
    return varDeclsList;
}

std::unique_ptr<IAST> PascalSyntaxAnalyzer::syntaxTypeSpec(void) {
    /* syntaxTypeSpec ::= ('integer'|'real'|'string'
    |  'array' '[' '<INTEGER_CONST>' '..' '<INTEGER_CONST>' ']' of syntaxTypeSpec) */
    Token typeTok = getCurTok();

    if (typeTok.getType() != BaseToken::ARRAY) {
        if(getCurTok().getType() == BaseToken::VOID)
            throw SyntaxException(getCurTok(), "Нельзя объявлять явным типом void! ");
        eatAdv(BaseToken::TYPE_SPEC);
        return ASTFactory::createAST(typeTok);
    } else {
        Token arrTok = getCurTok();
        eat(BaseToken::ARRAY);
        eat(BaseToken::LSQBRACKET);
        Token lHandTok = getCurTok();
        eat(BaseToken::INTEGER_CONST);
        eat(BaseToken::RANGE);
        Token rHandTok = getCurTok();
        eat(BaseToken::INTEGER_CONST);
        eat(BaseToken::RSQBRACKET);
        eat(BaseToken::OF);
        std::unique_ptr<IAST> subType = syntaxTypeSpec();
        return ASTFactory::createAST(arrTok, lHandTok, rHandTok,
                                     std::move(subType));
    }
}

std::unique_ptr<IAST> PascalSyntaxAnalyzer::syntaxCompoundSt(void) {
    /* syntaxCompoundSt ::= 'begin' syntaxStList 'end' */
    Token beginTok = getCurTok();
    beginTok.setAdvType(BaseToken::AdvType::COMPOUND);
    eat(BaseToken::BEGIN);
    std::vector<std::unique_ptr<IAST>> stList = syntaxStList();
    eat(BaseToken::END);

    return ASTFactory::createAST(beginTok, std::move(stList));
}

std::vector<std::unique_ptr<IAST>> PascalSyntaxAnalyzer::syntaxStList(void) {
    /* syntaxStList ::= syntaxSt (';' syntaxSt)* */
    std::unique_ptr<IAST> stPTR = syntaxSt();

    std::vector<std::unique_ptr<IAST>> stList;
    stList.emplace_back(std::move(stPTR));

    while (getCurTok().getType() == BaseToken::SEMI) {
        eat(BaseToken::SEMI);
        stList.emplace_back(syntaxSt());
    }

    if (getCurTok().getType() == BaseToken::ID)
        throw SyntaxException(getCurTok(), "Неожиданный индентификатор:");

    return stList;
}

std::unique_ptr<IAST> PascalSyntaxAnalyzer::syntaxSt(void) {
    /* syntaxSt ::= (syntaxCompoundSt
    |(syntaxAssignSt|syntaxCallSt)
    |syntaxWhileSt|syntaxIfSt
    |syntaxForSt|syntaxReturnSt
    |syntaxEmptySt) */
    switch (getCurTok().getType()) {
        case BaseToken::BEGIN:
            return syntaxCompoundSt();
        case BaseToken::ID: {
            if (lookFoward().getType() == BaseToken::ASSIGN ||
                lookFoward().getType() == BaseToken::LSQBRACKET)
                return syntaxAssignSt();
            else if (lookFoward().getType() == BaseToken::LPAREN)
                return syntaxCallSt();
            else {
                // Специально форсим, будто ждали присваивание для вызова исключения
                eat(BaseToken::ID);
                eat(BaseToken::ASSIGN);
            }
        }
        case BaseToken::WHILE:
            return syntaxWhileSt();
        case BaseToken::IF:
            return syntaxIfSt();
        case BaseToken::FOR:
            return syntaxForSt();
        case BaseToken::RETURN:
            return syntaxReturnSt();
        default:
            return syntaxEmptySt();
    }
}

std::unique_ptr<IAST> PascalSyntaxAnalyzer::syntaxEmptySt(void) {
    /* syntaxEmptySt ::= '$NONE$' */
    return ASTFactory::createAST({"", BaseToken::EMPTY, BaseToken::UNKNOWN});
}

std::unique_ptr<IAST> PascalSyntaxAnalyzer::syntaxAssignSt(void) {
    /* syntaxAssignSt ::= 
        (syntaxVariable|syntaxSelect) ':=' syntaxExpr */
    std::unique_ptr<IAST> lValue;
    if (lookFoward().getType() == BaseToken::LSQBRACKET)
        lValue = std::move(syntaxSelect());
    else
        lValue = std::move(syntaxVariable());
    Token assignTok = getCurTok();
    eat(BaseToken::ASSIGN);

    std::unique_ptr<IAST> toAssign = std::move(syntaxExpr());
    return ASTFactory::createAST(assignTok, std::move(lValue),
                                 std::move(toAssign));
}

std::unique_ptr<IAST> PascalSyntaxAnalyzer::syntaxIfSt(void) {
    /* syntaxIfSt ::= 'if' syntaxExpr 'then' syntaxSt ('else' syntaxSt)? */
    Token ifTok = getCurTok();
    eat(BaseToken::IF);
    std::unique_ptr<IAST> conditionPTR = syntaxExpr();

    eat(BaseToken::THEN);
    std::unique_ptr<IAST> bodyPTR = syntaxSt();

    std::unique_ptr<IAST> elseBodyPTR;
    if (getCurTok().getType() == BaseToken::ELSE) {
        eat(BaseToken::ELSE);
        elseBodyPTR = std::move(syntaxSt());
    }
    return ASTFactory::createAST(ifTok, std::move(conditionPTR),
                                 std::move(bodyPTR), std::move(elseBodyPTR));
}

std::unique_ptr<IAST> PascalSyntaxAnalyzer::syntaxWhileSt(void) {
    /* syntaxWhileSt ::= 'while' syntaxExpr 'do' syntaxSt */
    Token whileTok = getCurTok();
    eat(BaseToken::WHILE);
    std::unique_ptr<IAST> conditionPTR = syntaxExpr();
    eat(BaseToken::DO);
    std::unique_ptr<IAST> bodyPTR = syntaxSt();
    return ASTFactory::createAST(whileTok, std::move(conditionPTR),
                                 std::move(bodyPTR));
}

std::unique_ptr<IAST> PascalSyntaxAnalyzer::syntaxForSt(void) {
    /* syntaxForSt ::= 'for' syntaxIter 'do' syntaxSt */
    Token token = getCurTok();
    eat(BaseToken::FOR);
    std::unique_ptr<IAST> iterSt = syntaxIter();
    eat(BaseToken::DO);
    std::unique_ptr<IAST> body = syntaxSt();
    return ASTFactory::createAST(token, std::move(iterSt), std::move(body));
}

std::unique_ptr<IAST> PascalSyntaxAnalyzer::syntaxCallSt(void) {
    /* syntaxCallSt ::= 
        '<ID>' '(' (syntaxExpr (',' syntaxExpr)*)? ')' */
    Token &nameTok = getCurTok();
    nameTok.setAdvType(BaseToken::FUNCTION_NAME);
    eat(BaseToken::ID);
    eat(BaseToken::LPAREN);
    std::vector<std::unique_ptr<IAST>> paramsList;
    if (getCurTok().getType() != BaseToken::RPAREN) {
        std::unique_ptr<IAST> paramPTR = std::move(syntaxExpr());
        paramsList.emplace_back(std::move(paramPTR));
    }
    while (getCurTok().getType() == BaseToken::COMMA) {
        eat(BaseToken::COMMA);
        std::unique_ptr<IAST> paramPTR = std::move(syntaxExpr());
        paramsList.emplace_back(std::move(paramPTR));
    }

    eat(BaseToken::RPAREN);
    return ASTFactory::createAST(nameTok, std::move(paramsList));
}

std::unique_ptr<IAST> PascalSyntaxAnalyzer::syntaxReturnSt(void) {
    /* syntaxReturnSt ::= 'return' syntaxExpr */
    Token retTok = getCurTok();
    eat(BaseToken::RETURN);
    std::unique_ptr<IAST> toReturn = std::move(syntaxExpr());
    return ASTFactory::createAST(retTok, std::move(toReturn));
}

std::unique_ptr<IAST> PascalSyntaxAnalyzer::syntaxVariable(void) {
    /* syntaxVariable ::= '<ID>' */
    getCurTok().setAdvType(BaseToken::VAR_NAME);
    std::unique_ptr<IAST> varPTR = ASTFactory::createAST(getCurTok());
    eat(BaseToken::ID);
    return varPTR;
}

std::unique_ptr<IAST> PascalSyntaxAnalyzer::syntaxSelect(void) {
    /* syntaxSelect ::= 
        syntaxVariable ('[' syntaxExpr ']')+ */
    Token tok = getCurTok();
    tok.setAdvType(BaseToken::SELECT);
    std::unique_ptr<IAST> var = std::move(syntaxVariable());
    std::vector<std::unique_ptr<IAST>> selections;
    while (getCurTok().getType() == BaseToken::LSQBRACKET) {
        eat(BaseToken::LSQBRACKET);
        std::unique_ptr<IAST> index = std::move(syntaxExpr());
        eat(BaseToken::RSQBRACKET);
        selections.emplace_back(std::move(index));
    }
    if (selections.size() != 1)
        for (std::size_t i = 0; i < selections.size() - 1; i++) {
            var = ASTFactory::createAST(tok, std::move(var),
                                        std::move(selections[i]));
        }
    return ASTFactory::createAST(tok, std::move(var),
                                 std::move(selections[selections.size() - 1]));
}

std::unique_ptr<IAST> PascalSyntaxAnalyzer::syntaxExpr(void) {
    /* syntaxExpr ::= syntaxSimpleExpr (('<'|'<='|'<>'|'='|'>'|'>=')
       syntaxSimpleExr)? */
    std::unique_ptr<IAST> simpleExprPTR = syntaxSimpleExpr();

    Token token;
    if (isIn(getCurTok().getType(),
             {BaseToken::LESS, BaseToken::LESS_EQ, BaseToken::NEQ, BaseToken::EQ,
              BaseToken::MORE, BaseToken::MORE_EQ})) {
        token = getCurTok();
        eat(token.getType());
        return ASTFactory::createAST(token, std::move(simpleExprPTR),
                                     syntaxSimpleExpr());
    } else {
        return simpleExprPTR;
    }
}

std::unique_ptr<IAST> PascalSyntaxAnalyzer::syntaxSimpleExpr(void) {
    /* syntaxSimpleExpr ::= syntaxTerm (('+'|'-'|'OR') syntaxTerm)? */
    std::unique_ptr<IAST> termPTR = syntaxTerm();

    while (isIn(getCurTok().getType(),
                {BaseToken::PLUS, BaseToken::MINUS, BaseToken::OR})) {
        Token token = getCurTok();
        eat(token.getType());

        termPTR =
            ASTFactory::createAST(token, std::move(termPTR), syntaxTerm());
    }

    return termPTR;
}

std::unique_ptr<IAST> PascalSyntaxAnalyzer::syntaxTerm(void) {
    /* syntaxTerm ::= syntaxFactor (('/'|'*'|'DIV'|'MOD'|'AND') syntaxFactor)?
     */
    std::unique_ptr<IAST> factorPTR = syntaxFactor();

    while (isIn(getCurTok().getType(),
                {BaseToken::FLOAT_DIV, BaseToken::MUL, BaseToken::INTEGER_DIV,
                 BaseToken::MOD, BaseToken::AND})) {
        Token token = getCurTok();
        eat(token.getType());
        factorPTR =
            ASTFactory::createAST(token, std::move(factorPTR), syntaxFactor());
    }

    return factorPTR;
}

std::unique_ptr<IAST> PascalSyntaxAnalyzer::syntaxFactor(void) {
    /* syntaxFactor ::= 
        ('<INTEGER_CONST>'|'<REAL_CONST>'
        |'<STRING_CONST>'|'(' syntaxExpr ')'
        |'+' syntaxFactor| '-' syntaxFactor| 'not' syntaxFactor
        | syntaxCallSt| syntaxVariable| syntaxSelect) */
    Token token = getCurTok();
    if (token.getAdvType() == BaseToken::SOME_CONST) {
        eatAdv(BaseToken::SOME_CONST);
        return ASTFactory::createAST(token);
    }
    switch (token.getType()) {
        case BaseToken::LPAREN: {
            eat(BaseToken::LPAREN);
            std::unique_ptr<IAST> node = syntaxExpr();
            eat(BaseToken::RPAREN);
            return node;
        }

        case BaseToken::PLUS:
        case BaseToken::MINUS:
        case BaseToken::NOT: {
            eatAdv(BaseToken::OPERATOR);
            return ASTFactory::createAST(token, syntaxFactor());
        }

        case BaseToken::ID: {
            if (lookFoward().getType() == BaseToken::LPAREN)
                return syntaxCallSt();
            else if (lookFoward().getType() == BaseToken::LSQBRACKET)
                return syntaxSelect();
            else
                return syntaxVariable();
        }
        default:
            throw SyntaxException(token,
                                  "Во время обработки выражения syntaxFactor "
                                  "встречен неожиданный токен. ");
    }
    return nullptr;
}

std::unique_ptr<IAST> PascalSyntaxAnalyzer::syntaxIter(void) {
    /* syntaxIter ::= syntaxAssignSt ('to'|'down to')
     * ('<INTEGER_CONST>'|syntaxVariable) */
    Token varTok = getCurTok();
    std::unique_ptr<IAST> initSt = syntaxAssignSt();

    Token toTok = getCurTok();
    if (toTok.getType() == BaseToken::DOWNTO)
        eat(BaseToken::DOWNTO);
    else
        eat(BaseToken::TO);

    Token finTok = getCurTok();
    if (finTok.getAdvType() != BaseToken::SOME_CONST and
        finTok.getType() != BaseToken::ID)
        throw AnalyzeException(finTok,
                               "Конечным значением цикла for должна быть целая "
                               "константа или переменная!");
    // Пропустим константу или переменную вызовом syntaxFactor
    syntaxFactor();

    Token condOp = {"<=", BaseToken::LESS_EQ, BaseToken::OPERATOR};
    std::unique_ptr<IAST> varPtr_condition = ASTFactory::createAST(varTok);
    std::unique_ptr<IAST> finPtr_condition = ASTFactory::createAST(finTok);
    std::unique_ptr<IAST> condition = ASTFactory::createAST(
        condOp, std::move(varPtr_condition), std::move(finPtr_condition));

    Token postOp = {"+", BaseToken::PLUS, BaseToken::OPERATOR};
    if (toTok.getType() == BaseToken::DOWNTO)
        postOp = {"-", BaseToken::MINUS, BaseToken::OPERATOR};
    Token forOp = {"1", BaseToken::INTEGER_CONST, BaseToken::SOME_CONST};
    std::unique_ptr<IAST> varPtr_postOp = ASTFactory::createAST(varTok);
    std::unique_ptr<IAST> finPtr_postOp = ASTFactory::createAST(forOp);
    std::unique_ptr<IAST> postAction = ASTFactory::createAST(
        postOp, std::move(varPtr_postOp), std::move(finPtr_postOp));

    return ASTFactory::createAST(toTok, std::move(initSt), std::move(condition),
                                 std::move(postAction));
}

void PascalSyntaxAnalyzer::getNextToken(void) { currentIndex++; }

Token &PascalSyntaxAnalyzer::getCurTok(void) { return tokens[currentIndex]; }

Token &PascalSyntaxAnalyzer::lookFoward(void) {
    if (currentIndex + 1 >= tokens.size())
        throw std::invalid_argument("Неожиданный конец файла!");
    return tokens[currentIndex + 1];
}

void PascalSyntaxAnalyzer::eat(BaseToken::Type const type) {
    if (getCurTok().getType() == type) {
        getNextToken();
    } else {
        throw SyntaxException(
            getCurTok(),
            fmt::format(
                "Ошибка при обработке синтаксиса! Ожидался тип {}! Встречен ",
                magic_enum::enum_name(type)));
    }
}

void PascalSyntaxAnalyzer::eatAdv(BaseToken::AdvType const type) {
    if (getCurTok().getAdvType() == type) {
        getNextToken();
    } else {
        throw SyntaxException(
            getCurTok(),
            fmt::format(
                "Ошибка при обработке синтаксиса! Ожидался тип {}! Встречен ",
                magic_enum::enum_name(type)));
    }
}

List<Token> PascalSyntaxAnalyzer::getTokens(void) { return tokens; }
void PascalSyntaxAnalyzer::setTokens(List<Token> const &newTokens) {
    tokens = newTokens;
}