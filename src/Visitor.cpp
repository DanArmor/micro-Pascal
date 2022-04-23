#include "Visitor.hpp"

#include <fmt/format.h>

#include "AST.hpp"

/*Определения CalcVisitor
==================*/
//CalcVisitor::CalcVisitor(){};
//
//extern std::map<std::string, int> globalVariables;
//
//void CalcVisitor::visit(BinOpAST &node){
//    switch(node.token.getType()){
//        case IToken::PLUS :
//            Return(getValue(node.left) + getValue(node.right));
//            break;
//        case IToken::MINUS :
//            Return(getValue(node.left) - getValue(node.right));
//            break;
//        case IToken::DIV :
//            Return(getValue(node.left) / getValue(node.right));
//            break;
//        case IToken::MUL :
//            Return(getValue(node.left) * getValue(node.right));
//            break;
//    }
//}
//
//void CalcVisitor::visit(UnOpAST &node){
//    switch(node.token.getType()){
//        case IToken::PLUS :
//            Return(getValue(node.down));
//            break;
//        case IToken::MINUS :
//            Return(-getValue(node.down));
//            break;
//    }
//}
//
//void CalcVisitor::visit(NumberAST &node){
//    Return(std::stoi(node.token.getStr()));
//}
//
//void CalcVisitor::visit(CompoundAST &node){
//    for(auto child : node.children){
//        CalcVisitor vis;
//        child->accept(vis);
//    }
//}
//
//void CalcVisitor::visit(AssignAST &node){
//    globalVariables[node.var->token.getStr()] = getValue(node.value);
//}
//
//void CalcVisitor::visit(VarAST &node){
//    std::string name = node.token.getStr();
//    if(globalVariables.count(name) == 0)
//        throw std::invalid_argument(fmt::format("Использование переменной до её инициализации! VarName: {}", name));
//    Return(globalVariables[name]);
//}
//
//void CalcVisitor::visit(NoOpAST &node){
//    return;
//}

/*Определения GraphvizVisitor
==================*/
GraphvizVisitor::GraphvizVisitor(std::string filename){
    file.open(filename, std::ios::out);
    file << "digraph name{\n";
}

void GraphvizVisitor::done(void){
    file << "}\n";
    file.close();
}

void GraphvizVisitor::visit(BinOpAST &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.left->accept(*this);
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.right->accept(*this);
}

void GraphvizVisitor::visit(UnOpAST &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.down->accept(*this);
}

void GraphvizVisitor::visit(NumberAST &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
}

void GraphvizVisitor::visit(CompoundAST &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
    for(auto &child : node.children){
        links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
        child->accept(*this);
    }
}

void GraphvizVisitor::visit(AssignAST &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.var->accept(*this);
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.value->accept(*this);
}

void GraphvizVisitor::visit(VarAST &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
}

void GraphvizVisitor::visit(NoOpAST &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
}

void GraphvizVisitor::write(void){
    for(auto p : declarations){
        file << fmt::format("n{}[label=\"{}\"]\n", p.first, p.second);
    }
    for(auto p : links){
        file << fmt::format("n{}->n{}\n", p.first, p.second);
    }
    done();
}

void GraphvizVisitor::visit(ProgramAST &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
    for(auto &child : node.functions){
        links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
        child->accept(*this);
    }
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.block->accept(*this);
}

void GraphvizVisitor::visit(BlockAST &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
    for(auto &child : node.declarations){
        links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
        child->accept(*this);
    }
    for(auto &child : node.consts){
        links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
        child->accept(*this);
    }
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.compound->accept(*this);
}

void GraphvizVisitor::visit(VarDeclAST &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.var->accept(*this);
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.type->accept(*this);

}

void GraphvizVisitor::visit(TypeSpecAST &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
}

void GraphvizVisitor::visit(ConstAST &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.constName->accept(*this);
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.constValue->accept(*this);
}

void GraphvizVisitor::visit(StringAST &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
}

void GraphvizVisitor::visit(CallAST &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
    for(auto &child : node.params){
        links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
        child->accept(*this);
    }
}

void GraphvizVisitor::visit(IfAST &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.body->accept(*this);
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.condition->accept(*this);
    if(node.elseBody != nullptr){
        links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
        node.elseBody->accept(*this);
    }
}

void GraphvizVisitor::visit(WhileAST &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.body->accept(*this);
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.condition->accept(*this);
}

void GraphvizVisitor::visit(ForAST &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.iterSt->accept(*this);
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.body->accept(*this);
}

void GraphvizVisitor::visit(IterationAST &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.assign->accept(*this);
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.condition->accept(*this);
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.postAction->accept(*this);
}

void GraphvizVisitor::visit(FunctionAST &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
    for(auto &child : node.params){
        links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
        child->accept(*this);
    }
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.returnType->accept(*this);
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.body->accept(*this);
}

void GraphvizVisitor::visit(ReturnAST &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.toReturn->accept(*this);
}

/*Определения TypeViewVisitor
==================*/
TypeViewVisitor::TypeViewVisitor(){};

void TypeViewVisitor::visit(BinOpAST &node){
    typesStrings.push_back(node.token.getStr());
    node.left->accept(*this);
    node.right->accept(*this);
}

void TypeViewVisitor::visit(UnOpAST &node){
    typesStrings.push_back(node.token.getStr());
    node.down->accept(*this);
}

void TypeViewVisitor::visit(NumberAST &node){
    typesStrings.push_back(node.token.getStr());
}

void TypeViewVisitor::visit(CompoundAST &node){
    typesStrings.push_back(node.token.getStr());
    for(auto &child : node.children){
        child->accept(*this);
    }
}

void TypeViewVisitor::visit(AssignAST &node){
    typesStrings.push_back(node.token.getStr());
    node.var->accept(*this);
    node.value->accept(*this);
}

void TypeViewVisitor::visit(VarAST &node){
    typesStrings.push_back(node.token.getStr());
}

void TypeViewVisitor::visit(NoOpAST &node){
    typesStrings.push_back(node.token.getStr());
}

void TypeViewVisitor::visit(ProgramAST &node){
    typesStrings.push_back(node.token.getStr());
    for(auto &child : node.functions)
        child->accept(*this);
    node.block->accept(*this);
}

void TypeViewVisitor::visit(BlockAST &node){
    typesStrings.push_back(node.token.getStr());
    for(auto &child : node.consts){
        child->accept(*this);
    }
    for(auto &child : node.declarations){
        child->accept(*this);
    }
    node.compound->accept(*this);
}

void TypeViewVisitor::visit(VarDeclAST &node){
    typesStrings.push_back(node.token.getStr());
    node.var->accept(*this);
    node.type->accept(*this);
}

void TypeViewVisitor::visit(TypeSpecAST &node){
    typesStrings.push_back(node.token.getStr());
}

void TypeViewVisitor::visit(ConstAST &node){
    typesStrings.push_back(node.token.getStr());
    node.constName->accept(*this);
    node.constValue->accept(*this);
}

void TypeViewVisitor::visit(StringAST &node){
    typesStrings.push_back(node.token.getStr());
}

void TypeViewVisitor::visit(CallAST &node){
    typesStrings.push_back(node.token.getStr());
    for(auto &child : node.params){
        child->accept(*this);
    }
}

void TypeViewVisitor::visit(IfAST &node){
    typesStrings.push_back(node.token.getStr());
    node.body->accept(*this);
    node.condition->accept(*this);
    if(node.elseBody != nullptr)
        node.elseBody->accept(*this);
}

void TypeViewVisitor::visit(WhileAST &node){
    typesStrings.push_back(node.token.getStr());
    node.body->accept(*this);
    node.condition->accept(*this);
}

void TypeViewVisitor::visit(ForAST &node){
    typesStrings.push_back(node.token.getStr());
    node.body->accept(*this);
    node.iterSt->accept(*this);
}

void TypeViewVisitor::visit(IterationAST &node){
    typesStrings.push_back(node.token.getStr());
    node.assign->accept(*this);
    node.condition->accept(*this);
    node.postAction->accept(*this);
}

void TypeViewVisitor::visit(FunctionAST &node){
    typesStrings.push_back(node.token.getStr());
    for(auto &child : node.params)
        child->accept(*this);
    node.returnType->accept(*this);
    node.body->accept(*this);
}
void TypeViewVisitor::visit(ReturnAST &node){
    typesStrings.push_back(node.token.getStr());
    node.toReturn->accept(*this);
}

std::vector<std::string> TypeViewVisitor::getData(void){
    return typesStrings;
}



//    CodeGenVisitor::CodeGenVisitor(std::string filename){
//        file.open(filename, std::ios::out);
//    }
//
//    void CodeGenVisitor::done(void){
//        file.close();
//    }
//
//    void CodeGenVisitor::visit(BinOpAST &node){
//        node.left->accept(*this);
//        node.right->accept(*this);
//        file << "pop \%ecx" << "\n";
//        file << "pop \%eax" << "\n";
//        switch (node.token.getType()){
//            case IToken::PLUS : 
//                file << "addl \%ecx, \%eax" << "\n";
//                break;
//            case IToken::MUL : 
//                file << "imul \%ecx, \%eax" << "\n";
//                break;
//        }
//        file << "push \%eax" << "\n";
//    }
//
//    void CodeGenVisitor::visit(UnOpAST &node){
//
//    }
//
//    void CodeGenVisitor::visit(NumberAST &node){
//        file << "push $" << node.token.getStr() << "\n";
//    }
//
//    void CodeGenVisitor::visit(CompoundAST &node){
//        for(auto &child : node.children){
//            child->accept(*this);
//        }
//    }
//
//    void CodeGenVisitor::visit(AssignAST &node){
//
//    }
//
//    void CodeGenVisitor::visit(VarAST &node){
//
//    }
//
//    void CodeGenVisitor::visit(NoOpAST &node){
//
//    }
//
//    void CodeGenVisitor::visit(ProgramAST &node){
//        file << ".file \"main.pas\"" << "\n";
//        file << ".text" << "\n";
//        file << ".globl main" << "\n";
//        file << ".type main, @function" << "\n";
//        file << "main:\n";
//        node.block->accept(*this);
//    }
//
//    void CodeGenVisitor::visit(BlockAST &node){
//        node.compound->accept(*this);
//        file << "ret";
//        file << "\n";
//        if(node.consts.size() != 0){
//            file << ".data" << "\n";
//            for(auto &child : node.consts){
//                child->accept(*this);
//            }
//        }
//        file << "\n";
//    }
//
//    void CodeGenVisitor::visit(VarDeclAST &node){
//
//    }
//
//    void CodeGenVisitor::visit(TypeSpecAST &node){
//
//    }
//
//    void CodeGenVisitor::visit(ConstAST &node){
//        file << node.constName->token.getStr() << ":\n";
//        node.constValue->accept(*this);
//    }
//
//    void CodeGenVisitor::visit(StringAST &node){
//        file << ".string " << "\""  << node.token.getStr().substr(1, node.token.getStr().size()-2) << "\"" << "\n";
//    }
//
//    void CodeGenVisitor::visit(CallAST &node){
//        if(node.token.getStr() == "RETURN"){
//            node.params[0]->accept(*this);
//            file << "pop \%eax" << "\n";
//        } else if(node.token.getStr() == "writeln"){
//            file << "movl " << "$" << node.params[0]->token.getStr() << ", " << "\%ebx" << "\n";
//            file << "push \%ebx" << "\n";
//            file << "call puts" << "\n";
//            file << "add $4, \%esp" << "\n";
//        }
//
//    }
//
//    void CodeGenVisitor::visit(IfAST &node){
//
//    }
//
//    void CodeGenVisitor::visit(WhileAST &node){
//
//    }
//
//    void CodeGenVisitor::write(void){
//
//    }

    HighlightAccurateVisitor::HighlightAccurateVisitor(List<Token> tokens) : tokens(tokens){};

    void HighlightAccurateVisitor::visit(BinOpAST &node){
        node.left->accept(*this);
        node.right->accept(*this);
    }

    void HighlightAccurateVisitor::visit(UnOpAST &node){
        node.down->accept(*this);
    }

    void HighlightAccurateVisitor::visit(NumberAST &node){
        return;
    }

    void HighlightAccurateVisitor::visit(CompoundAST &node){
        for(auto &child : node.children)
            child->accept(*this);
    }

    void HighlightAccurateVisitor::visit(AssignAST &node){
        node.var->accept(*this);
        node.value->accept(*this);
    }

    void HighlightAccurateVisitor::visit(VarAST &node){
        for(auto &tok : tokens){
            if(tok.getStr() == node.token.getStr())
                tok.setAdvType(IToken::VAR_NAME);
        }
    }

    void HighlightAccurateVisitor::visit(NoOpAST &node){
        return;
    }

    void HighlightAccurateVisitor::visit(ProgramAST &node){
        for(auto &tok : tokens){
            if(tok.getStr() == node.name.getStr()){
                tok.setAdvType(IToken::PROGRAM_NAME);
                break;
            }
        }
        for(auto &child : node.functions)
            child->accept(*this);
        node.block->accept(*this);
    }

    void HighlightAccurateVisitor::visit(BlockAST &node){
        for(auto &child : node.consts)
            child->accept(*this);
        for(auto &child : node.declarations)
            child->accept(*this);
        node.compound->accept(*this);
    }

    void HighlightAccurateVisitor::visit(VarDeclAST &node){
        node.var->accept(*this);
        node.type->accept(*this);
    }

    void HighlightAccurateVisitor::visit(TypeSpecAST &node){
        return;
    }

    void HighlightAccurateVisitor::visit(ConstAST &node){
        return;
    }

    void HighlightAccurateVisitor::visit(StringAST &node){
        return;
    }

    void HighlightAccurateVisitor::visit(CallAST &node){
        for(auto &tok : tokens){
            if(tok.getStr() == node.token.getStr())
                tok.setAdvType(IToken::FUNCTION_NAME);
        }
    }

    void HighlightAccurateVisitor::visit(IfAST &node){
        node.condition->accept(*this);
        node.body->accept(*this);
        if(node.elseBody)
            node.elseBody->accept(*this);
    }

    void HighlightAccurateVisitor::visit(WhileAST &node){
        node.condition->accept(*this);
        node.body->accept(*this);
    }

    void HighlightAccurateVisitor::visit(ForAST &node){
        node.iterSt->accept(*this);
        node.body->accept(*this);
    }

    void HighlightAccurateVisitor::visit(IterationAST &node){
        node.assign->accept(*this);
        node.condition->accept(*this);
        node.postAction->accept(*this);
    }

    void HighlightAccurateVisitor::visit(FunctionAST &node){
        for(auto &child : node.params)
            child->accept(*this);
        node.returnType->accept(*this);
        node.body->accept(*this);
    }

    void HighlightAccurateVisitor::visit(ReturnAST &node){
        node.toReturn->accept(*this);
    }

    List<Token> HighlightAccurateVisitor::getTokens(void){
        return tokens;
    }
