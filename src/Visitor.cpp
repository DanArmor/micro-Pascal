#include "Visitor.hpp"

#include <fmt/format.h>

#include <iostream>

#include "AST.hpp"
#include "SyntExp.hpp"
#include "supAlgs.hpp"

/*Определения GraphvizVisitor
==================*/
GraphvizVisitor::GraphvizVisitor(std::string filename) {
    file.open(filename, std::ios::out);
    file << "digraph name{\n";
}

void GraphvizVisitor::visit(ProgramAST &node) {
    std::size_t backup = nodeIndex;
    addDef(node.name.getStr());
    for (auto &child : node.functions) connectToNode(backup, child.get());
    connectToNode(backup, node.block.get());
    write();
}

void GraphvizVisitor::visit(FunctionAST &node) {
    std::size_t backup = nodeIndex;
    addDef(node.name.getStr());
    for (auto &child : node.params) connectToNode(backup, child.get());
    connectToNode(backup, node.returnType.get());
    connectToNode(backup, node.body.get());
}

void GraphvizVisitor::visit(BlockAST &node) {
    std::size_t backup = nodeIndex;
    addDef(node.token.getStr());
    for (auto &child : node.consts) connectToNode(backup, child.get());
    for (auto &child : node.declarations) connectToNode(backup, child.get());
    connectToNode(backup, node.compound.get());
}

void GraphvizVisitor::visit(VarDeclAST &node) {
    std::size_t backup = nodeIndex;
    addDef(node.token.getStr());
    connectToNode(backup, node.var.get());
    connectToNode(backup, node.type.get());
}

void GraphvizVisitor::visit(TypeSpecAST &node) { addDef(node.token.getStr()); }

void GraphvizVisitor::visit(ArrSpecAST &node) {
    std::size_t backup = nodeIndex;
    std::string arrStr =
        fmt::format("{}[{} .. {}]", node.token.getStr(), node.lHandTok.getStr(),
                    node.rHandTok.getStr());
    addDef(arrStr);
    connectToNode(backup, node.subType.get());
}

void GraphvizVisitor::visit(ConstAST &node) {
    std::size_t backup = nodeIndex;
    addDef(node.token.getStr());
    connectToNode(backup, node.constName.get());
    connectToNode(backup, node.constValue.get());
}

void GraphvizVisitor::visit(CompoundAST &node) {
    std::size_t backup = nodeIndex;
    addDef(node.token.getStr());
    for (auto &child : node.children) connectToNode(backup, child.get());
}

void GraphvizVisitor::visit(NumberAST &node) { addDef(node.token.getStr()); }

void GraphvizVisitor::visit(StringAST &node) { addDef(node.token.getStr()); }

void GraphvizVisitor::visit(BinOpAST &node) {
    std::size_t backup = nodeIndex;
    addDef(node.token.getStr());
    connectToNode(backup, node.left.get());
    connectToNode(backup, node.right.get());
}

void GraphvizVisitor::visit(UnOpAST &node) {
    std::size_t backup = nodeIndex;
    addDef(node.token.getStr());
    connectToNode(backup, node.down.get());
}

void GraphvizVisitor::visit(NoOpAST &node) { addDef(node.token.getStr()); }

void GraphvizVisitor::visit(AssignAST &node) {
    std::size_t backup = nodeIndex;
    addDef(node.token.getStr());
    connectToNode(backup, node.var.get());
    connectToNode(backup, node.value.get());
}

void GraphvizVisitor::visit(VarAST &node) { addDef(node.token.getStr()); }

void GraphvizVisitor::visit(SelectAST &node) {
    std::size_t backup = nodeIndex;
    addDef(node.token.getStr());
    connectToNode(backup, node.from.get());
    connectToNode(backup, node.index.get());
}

void GraphvizVisitor::visit(CallAST &node) {
    std::size_t backup = nodeIndex;
    addDef(node.token.getStr());
    for (auto &child : node.params) connectToNode(backup, child.get());
}

void GraphvizVisitor::visit(ReturnAST &node) {
    std::size_t backup = nodeIndex;
    addDef(node.token.getStr());
    connectToNode(backup, node.toReturn.get());
}

void GraphvizVisitor::visit(IfAST &node) {
    std::size_t backup = nodeIndex;
    addDef(node.token.getStr());
    connectToNode(backup, node.body.get());
    connectToNode(backup, node.condition.get());
    if (node.elseBody != nullptr) connectToNode(backup, node.elseBody.get());
}

void GraphvizVisitor::visit(WhileAST &node) {
    std::size_t backup = nodeIndex;
    addDef(node.token.getStr());
    connectToNode(backup, node.body.get());
    connectToNode(backup, node.condition.get());
}

void GraphvizVisitor::visit(ForAST &node) {
    std::size_t backup = nodeIndex;
    addDef(node.token.getStr());
    connectToNode(backup, node.iterSt.get());
    connectToNode(backup, node.body.get());
}

void GraphvizVisitor::visit(IterationAST &node) {
    std::size_t backup = nodeIndex;
    addDef(node.token.getStr());
    connectToNode(backup, node.assign.get());
    connectToNode(backup, node.condition.get());
    connectToNode(backup, node.postAction.get());
}

void GraphvizVisitor::addDef(std::string const &str) {
    declarations.push_back(std::make_pair(std::to_string(nodeIndex), str));
    nodeIndex++;
}

void GraphvizVisitor::connectToNode(std::size_t index, IAST *ptr) {
    links.push_back(
        std::make_pair(std::to_string(index), std::to_string(nodeIndex)));
    ptr->accept(*this);
}

void GraphvizVisitor::done(void) {
    file << "}\n";
    file.close();
    int statusGraphviz = system("dot -V >nul 2>nul");
    if (WEXITSTATUS(statusGraphviz) == 0) {
        system(fmt::format("dot out.dot -Tsvg > {}", "output.svg").c_str());
    } else {
        std::cout << "Проблема при определении Graphviz. .dot описание "
                     "изображения сгенерировано, но генерация самого "
                     "изображения требует Graphviz";
    }
}

void GraphvizVisitor::write(void) {
    for (auto p : declarations) {
        file << fmt::format("n{}[label=\"{}\"]\n", p.first, p.second);
    }
    for (auto p : links) {
        file << fmt::format("n{}->n{}\n", p.first, p.second);
    }
    done();
}

/*Определения TypeViewVisitor
==================*/
TypeViewVisitor::TypeViewVisitor() {}

void TypeViewVisitor::visit(ProgramAST &node) {
    typesStrings.push_back(node.token.getStr());
    for (auto &child : node.functions) child->accept(*this);
    node.block->accept(*this);
}

void TypeViewVisitor::visit(FunctionAST &node) {
    typesStrings.push_back(node.token.getStr());
    for (auto &child : node.params) child->accept(*this);
    node.returnType->accept(*this);
    node.body->accept(*this);
}

void TypeViewVisitor::visit(BlockAST &node) {
    typesStrings.push_back(node.token.getStr());
    for (auto &child : node.consts) child->accept(*this);
    for (auto &child : node.declarations) child->accept(*this);
    node.compound->accept(*this);
}

void TypeViewVisitor::visit(VarDeclAST &node) {
    typesStrings.push_back(node.token.getStr());
    node.var->accept(*this);
    node.type->accept(*this);
}

void TypeViewVisitor::visit(TypeSpecAST &node) {
    typesStrings.push_back(node.token.getStr());
}

void TypeViewVisitor::visit(ArrSpecAST &node) {
    std::string arrStr =
        fmt::format("{}[{} .. {}]", node.token.getStr(), node.lHandTok.getStr(),
                    node.rHandTok.getStr());
    typesStrings.push_back(arrStr);
    node.subType->accept(*this);
}

void TypeViewVisitor::visit(ConstAST &node) {
    typesStrings.push_back(node.token.getStr());
    node.constName->accept(*this);
    node.constValue->accept(*this);
}

void TypeViewVisitor::visit(CompoundAST &node) {
    typesStrings.push_back(node.token.getStr());
    for (auto &child : node.children) child->accept(*this);
}

void TypeViewVisitor::visit(NumberAST &node) {
    typesStrings.push_back(node.token.getStr());
}

void TypeViewVisitor::visit(StringAST &node) {
    typesStrings.push_back(node.token.getStr());
}

void TypeViewVisitor::visit(BinOpAST &node) {
    typesStrings.push_back(node.token.getStr());
    node.left->accept(*this);
    node.right->accept(*this);
}

void TypeViewVisitor::visit(UnOpAST &node) {
    typesStrings.push_back(node.token.getStr());
    node.down->accept(*this);
}

void TypeViewVisitor::visit(NoOpAST &node) {
    typesStrings.push_back(node.token.getStr());
}

void TypeViewVisitor::visit(AssignAST &node) {
    typesStrings.push_back(node.token.getStr());
    node.var->accept(*this);
    node.value->accept(*this);
}

void TypeViewVisitor::visit(VarAST &node) {
    typesStrings.push_back(node.token.getStr());
}

void TypeViewVisitor::visit(SelectAST &node) {
    typesStrings.push_back(node.token.getStr());
    node.from->accept(*this);
    node.index->accept(*this);
}

void TypeViewVisitor::visit(CallAST &node) {
    typesStrings.push_back(node.token.getStr());
    for (auto &child : node.params) {
        child->accept(*this);
    }
}

void TypeViewVisitor::visit(ReturnAST &node) {
    typesStrings.push_back(node.token.getStr());
    node.toReturn->accept(*this);
}

void TypeViewVisitor::visit(IfAST &node) {
    typesStrings.push_back(node.token.getStr());
    node.body->accept(*this);
    node.condition->accept(*this);
    if (node.elseBody != nullptr) node.elseBody->accept(*this);
}

void TypeViewVisitor::visit(WhileAST &node) {
    typesStrings.push_back(node.token.getStr());
    node.body->accept(*this);
    node.condition->accept(*this);
}

void TypeViewVisitor::visit(ForAST &node) {
    typesStrings.push_back(node.token.getStr());
    node.iterSt->accept(*this);
    node.body->accept(*this);
}

void TypeViewVisitor::visit(IterationAST &node) {
    typesStrings.push_back(node.token.getStr());
    node.assign->accept(*this);
    node.condition->accept(*this);
    node.postAction->accept(*this);
}

std::vector<std::string> TypeViewVisitor::getData(void) { return typesStrings; }

/*Определения SemanticVisitor
==================*/

SemanticVisitor::SemanticVisitor()
    : vars(new std::map<std::string, VarData>),
      consts(new std::map<std::string, VarData>),
      functions(new std::map<std::string, FunctionData>) {}

void SemanticVisitor::visit(ProgramAST &node) {
    addProgName(node.token.getStr());
    for (auto &child : node.functions) child->accept(*this);
    node.block->accept(*this);
}

void SemanticVisitor::visit(FunctionAST &node) {
    addFunc(node.token);
    currCheckFunc = node.token.getStr();
    for (auto &ptr : node.params)
        getFunc(node.token).params.push_back(getValue(ptr.get()));
    getFunc(node.token).returnType = getValue(node.returnType.get());
    node.body->accept(*this);
    currCheckFunc = "";
}

void SemanticVisitor::visit(BlockAST &node) {
    for (auto &child : node.consts) child->accept(*this);
    for (auto &child : node.declarations) child->accept(*this);
    node.compound->accept(*this);
    clearBlock();
}

void SemanticVisitor::visit(VarDeclAST &node) {
    addVar(node.var->token, node.type.get());
    getDefined(node.var->token).type = getValue(node.type.get());
    Return(getDefined(node.var->token).type);
}

void SemanticVisitor::visit(TypeSpecAST &node) { Return(node.token.getStr()); }

void SemanticVisitor::visit(ArrSpecAST &node) {
    Return(fmt::format("{}[{} .. {}] of ", node.token.getStr(),
                       node.lHandTok.getStr(), node.rHandTok.getStr()) +
           getValue(node.subType.get()));
}

void SemanticVisitor::visit(ConstAST &node) {
    addConst(node.constName->token);
    isConst = true;
    getDefined(node.constName->token).type = getValue(node.constValue.get());
    isConst = false;
}

void SemanticVisitor::visit(CompoundAST &node) {
    for (auto &child : node.children) child->accept(*this);
}

void SemanticVisitor::visit(NumberAST &node) {
    if (node.token.getType() == BaseToken::INTEGER_CONST)
        Return("integer");
    else
        Return("real");
}

void SemanticVisitor::visit(StringAST &node) { Return("string"); }

void SemanticVisitor::visit(BinOpAST &node) {
    std::string lHand = getValue(node.left.get());
    std::string rHand = getValue(node.right.get());
    if (!isIn(lHand, {std::string("integer"), std::string("real")})) {
        throw TypeException(
            node.token,
            fmt::format("Ожидался числовой тип, а получен {}. ", lHand));
    }
    if (!isIn(rHand, {std::string("integer"), std::string("real")})) {
        throw TypeException(
            node.token,
            fmt::format("Ожидался числовой тип, а получен {}. ", rHand));
    }
    switch (node.token.getType()) {
        case BaseToken::MOD:
            if (lHand != "integer" || rHand != "integer") {
                throw TypeException(
                    node.token, fmt::format("MOD определен только над целыми "
                                            "операндами, а получены {} и {}. ",
                                            lHand, rHand));
            }
        case BaseToken::LESS:
        case BaseToken::LESS_EQ:
        case BaseToken::EQ:
        case BaseToken::MORE:
        case BaseToken::MORE_EQ:
        case BaseToken::OR:
        case BaseToken::AND:
        case BaseToken::NOT:
            Return("integer");
            break;
        default:
            if (lHand == "real" || rHand == "real")
                Return("real");
            else
                Return("integer");
    }
}

void SemanticVisitor::visit(UnOpAST &node) {
    std::string valType = getValue(node.down.get());
    if (!isIn(valType, {std::string("integer"), std::string("real")})) {
        throw TypeException(
            node.token,
            fmt::format("Ожидался числовой тип, а получен {}. ", valType));
    }
    if (isIn(node.token.getType(), {BaseToken::NOT})) {
        Return("integer");
    } else {
        Return(valType);
    }
}

void SemanticVisitor::visit(NoOpAST &node) { Return("VOID"); }

void SemanticVisitor::visit(AssignAST &node) {
    if (!compareTypes(getValue(node.var.get()), getValue(node.value.get())))
        throw TypeException(
            node.token,
            fmt::format("Присваивание несовместимых типов {} = {}. ",
                        getValue(node.var.get()), getValue(node.value.get())));
}

void SemanticVisitor::visit(VarAST &node) {
    Return(getDefined(node.token).type);
}

void SemanticVisitor::visit(SelectAST &node) {
    if (node.from->token.getAdvType() == BaseToken::AdvType::SELECT) {
        typeIndex++;
        node.from->accept(*this);
        typeIndex--;
    } else {
        IAST *type = getDefined(node.from->token).typePtr;
        for (std::size_t i = 0; i < typeIndex + 1; i++) {
            if (!isIn(type->token.getStr(),
                      {std::string("integer"), std::string("real"),
                       std::string("string")})) {
                type = static_cast<ArrSpecAST *>(type)->subType.get();
            }
        }
        if (isIn(type->token.getStr(),
                 {std::string("integer"), std::string("real"),
                  std::string("string")})) {
            Return(type->token.getStr());
        } else {
            Return(getValue(static_cast<ArrSpecAST *>(type)));
        }
    }
}

void SemanticVisitor::visit(CallAST &node) {
    checkFunc(node.token);
    std::vector<std::string> check = getFunc(node.token).params;
    if (check.size() != node.params.size())
        throw SemanticException(
            node.token, fmt::format("Ожидалось {} аргументов, а получено {}. ",
                                    check.size(), node.params.size()));
    for (std::size_t i = 0; i < check.size(); i++)
        if (!compareTypes(check[i], getValue(node.params[i].get()))) {
            throw TypeException(
                node.token,
                fmt::format("{} из {} аргументов функции {} имеет неверный "
                            "тип. Ожидался {}, а получен {}. ",
                            i + 1, check.size(), node.token.getStr(), check[i],
                            getValue(node.params[i].get())));
        }
    Return(getFunc(node.token).returnType);
}

void SemanticVisitor::visit(ReturnAST &node) {
    std::string returnType = getValue(node.toReturn.get());
    std::string expected = "integer";
    if (currCheckFunc != "")
        expected = getFunc({currCheckFunc, BaseToken::ID, BaseToken::FUNCTION_NAME})
                       .returnType;
    if (!compareTypes(expected, returnType)) {
        throw TypeException(
            node.token,
            fmt::format("Ожидался {}, а получен {}. ", expected, returnType));
    }
    Return(returnType);
}

void SemanticVisitor::visit(IfAST &node) {
    node.condition->accept(*this);
    node.body->accept(*this);
    if (node.elseBody) node.elseBody->accept(*this);
}

void SemanticVisitor::visit(WhileAST &node) {
    node.condition->accept(*this);
    node.body->accept(*this);
}

void SemanticVisitor::visit(ForAST &node) {
    node.iterSt->accept(*this);
    node.body->accept(*this);
}

void SemanticVisitor::visit(IterationAST &node) {
    node.condition->accept(*this);
    node.assign->accept(*this);
    node.postAction->accept(*this);
}

void SemanticVisitor::addProgName(std::string name) { programName = name; }

SemanticVisitor::VarData::VarData(Token token, IAST *typePtr)
    : token(token), typePtr(typePtr){};

void SemanticVisitor::addVar(Token token) {
    if (vars->count(token.getStr()) != 0)
        throw SemanticException(token, "Повторное объявление! ");
    (*vars)[token.getStr()] = VarData(token, false);
}

void SemanticVisitor::addVar(Token token, IAST *ptr) {
    if (vars->count(token.getStr()) != 0)
        throw SemanticException(token, "Повторное объявление! ");
    (*vars)[token.getStr()] = VarData(token, ptr);
}

void SemanticVisitor::addConst(Token token) {
    if (consts->count(token.getStr()) != 0)
        throw SemanticException(token, "Повторное объявление! ");
    (*consts)[token.getStr()] = VarData(token, true);
}

SemanticVisitor::VarData &SemanticVisitor::getVar(Token name) {
    if (!checkVar(name)) {
        throw SemanticException(name, "Использование до объявления! ");
    }
    return (*vars)[name.getStr()];
}

SemanticVisitor::VarData &SemanticVisitor::getConst(Token name) {
    if (!checkConst(name)) {
        throw SemanticException(name, "Использование до объявления! ");
    }
    return (*consts)[name.getStr()];
}

SemanticVisitor::VarData &SemanticVisitor::getDefined(Token name) {
    if (isConst) {
        return getConst(name);
    }

    checkDefined(name);
    if (checkVar(name))
        return (*vars)[name.getStr()];
    else
        return (*consts)[name.getStr()];
}

void SemanticVisitor::checkDefined(Token name) {
    if (!checkVar(name) && !checkConst(name))
        throw SemanticException(name, "Использование до объявления! ");
}

void SemanticVisitor::addFunc(Token token) {
    if (functions->count(token.getStr()) != 0)
        throw SemanticException(token, "Повторное объявление подпрограммы! ");
    (*functions)[token.getStr()] = FunctionData(token);
}

void SemanticVisitor::prebuildFunction(Token tok,
                                       std::vector<std::string> params,
                                       std::string returnType) {
    (*functions)[tok.getStr()] = FunctionData(tok, params, returnType);
}

void SemanticVisitor::prebuildFunctions(List<FunctionData> funcs) {
    for (auto const &func : funcs) {
        prebuildFunction(func.token, func.params, func.returnType);
    }
}

SemanticVisitor::FunctionData &SemanticVisitor::getFunc(Token name) {
    if (isConst) {
        throw SemanticException(
            name, "Попытка вызова функции в константном выражении! ");
    }
    checkFunc(name);
    return (*functions)[name.getStr()];
}

bool SemanticVisitor::checkVar(Token name) {
    return vars->count(name.getStr()) != 0;
}

bool SemanticVisitor::checkConst(Token name) {
    return consts->count(name.getStr()) != 0;
}

void SemanticVisitor::checkFunc(Token token) {
    if (functions->count(token.getStr()) == 0)
        throw SemanticException(token, "Использование до объявления! ");
}

bool SemanticVisitor::compareTypes(std::string A, std::string B, bool strict) {
    if (strict) {
        return A == B;
    } else {
        if (isIn(A, {std::string("integer"), std::string("real")}) &&
            isIn(B, {std::string("integer"), std::string("real")})) {
            return true;
        }
        return A == B;
    }
}

void SemanticVisitor::clearBlock(void) {
    vars->clear();
    consts->clear();
}

std::string SemanticVisitor::getValue(IAST *ptr) {
    auto vis = *this;
    ptr->accept(vis);
    return vis.value;
}

void SemanticVisitor::Return(std::string inValue) { value = inValue; }

SemanticVisitor::FunctionData::FunctionData(Token token,
                                            std::vector<std::string> params,
                                            std::string returnType)
    : token(token), params(params), returnType(returnType){};
SemanticVisitor::FunctionData::FunctionData(Token token) : token(token){};
SemanticVisitor::FunctionData::FunctionData() = default;

SemanticVisitor::VarData::VarData(Token token, bool isConst)
    : token(token), isConst(isConst){};
SemanticVisitor::VarData::VarData() = default;