#include "Syntax.hpp"


SyntaxAnalyzer::SyntaxAnalyzer(List<Token> const &tokens) : tokens(tokens) {};

AST *SyntaxAnalyzer::syntaxProgram(void){
    eat(IToken::PROGRAM);
    Token name;
    eat(IToken::ID);
    eat(IToken::SEMI);
    ASTptr node = syntaxBlock();
    eat(IToken::DOT);
    return new ProgramAST(name, node);
}

AST *SyntaxAnalyzer::syntaxCompoundSt(void){
    eat(IToken::BEGIN);
    std::vector<ASTptr> nodes = syntaxStList();
    eat(IToken::END);

    Compound *root = new Compound();
    for(auto node : nodes)
        root->addChild(node);
    
    return root;
}

std::vector<ASTptr> SyntaxAnalyzer::syntaxStList(void){
    AST *node = syntaxSt();

    std::vector<ASTptr> nodes = {node};

    while(getCurTok().getType() == IToken::SEMI){
        eat(getCurTok().getType());
        nodes.push_back(syntaxSt());
    }

    if(getCurTok().getType() == IToken::ID)
        throw std::invalid_argument(fmt::format("Неожиданный индентификатор: {}", getCurTok().getInfo()));

    return nodes;
}

ASTptr SyntaxAnalyzer::syntaxSt(void){
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

ASTptr SyntaxAnalyzer::syntaxCallSt(void){
    Token procName = getCurTok();
    eat(IToken::ID);
    eat(IToken::LPAREN);
    std::vector<ASTptr> params;
    if(getCurTok().getType() != IToken::RPAREN){
        ASTptr param = syntaxExpr();
        params.push_back(param);
    }
    while(getCurTok().getType() == IToken::COMMA){
        eat(IToken::COMMA);
        ASTptr param = syntaxExpr();
        params.push_back(param);
    }
    
    eat(IToken::RPAREN);
    return new ProcedureCall(procName, params);
}

ASTptr SyntaxAnalyzer::syntaxAssignSt(void){
    ASTptr left = syntaxVariable();
    Token token = getCurTok();

    eat(IToken::ASSIGN);

    ASTptr right = syntaxExpr();
    return new Assign(left, token, right);
}

ASTptr SyntaxAnalyzer::syntaxVariable(void){
    ASTptr node = new Var(getCurTok());
    eat(IToken::ID);
    return node;
}

ASTptr SyntaxAnalyzer::syntaxEmptySt(void){
    return new NoOp({"$", IToken::EMPTY});
}

AST *SyntaxAnalyzer::syntaxFactor(){
    Token token = getCurTok();
    switch(token.getType()) {
        case IToken::INTEGER_CONST :{
            eat(IToken::INTEGER_CONST);
            return new Number(token);
        }
        case IToken::REAL_CONST :{
            eat(IToken::REAL_CONST);
            return new Number(token);
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
            return new UnOp(token, syntaxFactor());
        }
        case IToken::MINUS :{
            eat(IToken::MINUS);
            return new UnOp(token, syntaxFactor());
        }
        case IToken::NOT :{
            eat(IToken::NOT);
            return new UnOp(token, syntaxFactor());
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


AST *SyntaxAnalyzer::syntaxIfSt(void){
    eat(IToken::IF);
    ASTptr condition = syntaxExpr();
    eat(IToken::THEN);
    ASTptr body = syntaxSt();
    ASTptr elseBody = nullptr;
    if(getCurTok().getType() == IToken::ELSE){
        eat(IToken::ELSE);
        elseBody = syntaxSt();
    }
    return new ifAST(condition, body, elseBody);
}

AST *SyntaxAnalyzer::syntaxWhileSt(void){
    eat(IToken::WHILE);
    ASTptr condition = syntaxExpr();
    eat(IToken::DO);
    ASTptr body = syntaxSt();
    return new whileAST(condition, body);
}

AST *SyntaxAnalyzer::syntaxTerm(void){
    AST *node = syntaxFactor();

    while(isIn(getCurTok().getType(), {IToken::FLOAT_DIV, IToken::MUL, IToken::INTEGER_DIV, IToken::MOD, IToken::AND})){
        Token token = getCurTok();
        eat(token.getType());
        node = new BinOp(node, token, syntaxFactor());
    }

    return node;
}

AST *SyntaxAnalyzer::syntaxExpr(void){
    AST *node = syntaxSimpleExpr();

    Token token;
    if(isIn(getCurTok().getType(), {IToken::LESS, IToken::LESS_EQ, IToken::NEQ, IToken::EQ, IToken::MORE, IToken::MORE_EQ})){
        token = getCurTok();
        eat(token.getType());
        return new BinOp(node, token, syntaxSimpleExpr());
    } else{
        return node;
    }
//    else 
//        throw std::invalid_argument(fmt::format("Ожидалась операция отношения, а получен токен типа {}", magic_enum::enum_name(getCurTok().getType())));
}

AST *SyntaxAnalyzer::syntaxSimpleExpr(void){
    AST *node = syntaxTerm();

    while(isIn(getCurTok().getType(), {IToken::PLUS, IToken::MINUS, IToken::OR})){
        Token token = getCurTok();
        eat(token.getType());

        node = new BinOp(node, token, syntaxTerm());
    }

    return node;
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

ASTptr SyntaxAnalyzer::syntaxBlock(void){
    std::vector<ASTptr> consts = syntaxConsts();
    std::vector<ASTptr> decls = syntaxVars();
    ASTptr compound = syntaxCompoundSt();
    return new BlockAST(consts, decls, compound);
}

std::vector<ASTptr> SyntaxAnalyzer::syntaxConsts(void){
    std::vector<ASTptr> consts;
    if(getCurTok().getType() == IToken::CONST){
        eat(IToken::CONST);
        while(getCurTok().getType() == IToken::ID){
            consts.push_back(syntaxConstDecl());
            eat(IToken::SEMI);
        }
    }
    return consts;

}

ASTptr SyntaxAnalyzer::syntaxConstDecl(void){
    ASTptr constName = new Var(getCurTok());
    eat(IToken::ID);

    eat(IToken::EQ);

    Token constValue = getCurTok();
    ASTptr ptr;
    if(isIn(getCurTok().getType(), {IToken::INTEGER_CONST, IToken::REAL_CONST, IToken::STRING_CONST})){
        if(isIn(getCurTok().getType(), {IToken::INTEGER_CONST, IToken::REAL_CONST})){
            ptr = new Number(constValue);
        } else{
            ptr = new StringAST(constValue);
        }
        eat(getCurTok().getType());
    }

    return new ConstAST(constName, ptr);
}

std::vector<ASTptr> SyntaxAnalyzer::syntaxVars(void){
    std::vector<ASTptr> decls;
    if(getCurTok().getType() == IToken::VAR){
        eat(IToken::VAR);
        while(getCurTok().getType() == IToken::ID){
            std::vector<ASTptr> varDecl = syntaxVarDecl();
            for(auto ptr : varDecl)
                decls.push_back(ptr);
            eat(IToken::SEMI);
        }
    }
    return decls;
}

std::vector<ASTptr> SyntaxAnalyzer::syntaxVarDecl(void){
    std::vector<ASTptr> vars;
    vars.push_back(new Var(getCurTok()));
    eat(IToken::ID);

    while(getCurTok().getType() == IToken::COMMA){
        eat(IToken::COMMA);
        vars.push_back(new Var(getCurTok()));
        eat(IToken::ID);
    }

    eat(IToken::COLON);

    ASTptr typeNode = syntaxTypeSpec();
    std::vector<ASTptr> varDecls;
    for(auto var : vars){
        varDecls.push_back(new VarDeclaration(var, typeNode));
    }
    return varDecls;
}

ASTptr SyntaxAnalyzer::syntaxTypeSpec(void){
    Token token = getCurTok();
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
    return new Type(token);
}