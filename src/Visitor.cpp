#include "Visitor.hpp"


/*Определения CalcVisitor
==================*/
//CalcVisitor::CalcVisitor(){};
//
//extern std::map<std::string, int> globalVariables;
//
//void CalcVisitor::visit(BinOp &node){
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
//void CalcVisitor::visit(UnOp &node){
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
//void CalcVisitor::visit(Number &node){
//    Return(std::stoi(node.token.getStr()));
//}
//
//void CalcVisitor::visit(Compound &node){
//    for(auto child : node.children){
//        CalcVisitor vis;
//        child->accept(vis);
//    }
//}
//
//void CalcVisitor::visit(Assign &node){
//    globalVariables[node.var->token.getStr()] = getValue(node.value);
//}
//
//void CalcVisitor::visit(Var &node){
//    std::string name = node.token.getStr();
//    if(globalVariables.count(name) == 0)
//        throw std::invalid_argument(fmt::format("Использование переменной до её инициализации! VarName: {}", name));
//    Return(globalVariables[name]);
//}
//
//void CalcVisitor::visit(NoOp &node){
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

void GraphvizVisitor::visit(BinOp &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.left->accept(*this);
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.right->accept(*this);
}

void GraphvizVisitor::visit(UnOp &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.down->accept(*this);
}

void GraphvizVisitor::visit(Number &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
}

void GraphvizVisitor::visit(Compound &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
    for(auto child : node.children){
        links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
        child->accept(*this);
    }
}

void GraphvizVisitor::visit(Assign &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.var->accept(*this);
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.value->accept(*this);
}

void GraphvizVisitor::visit(Var &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
}

void GraphvizVisitor::visit(NoOp &node){
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
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.block->accept(*this);
}

void GraphvizVisitor::visit(BlockAST &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
    for(auto child : node.declarations){
        links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
        child->accept(*this);
    }
    for(auto child : node.consts){
        links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
        child->accept(*this);
    }
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.compound->accept(*this);
}

void GraphvizVisitor::visit(VarDeclaration &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.var->accept(*this);
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.type->accept(*this);

}

void GraphvizVisitor::visit(Type &node){
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

void GraphvizVisitor::visit(ProcedureCall &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
    for(auto child : node.params){
        links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
        child->accept(*this);
    }
}

void GraphvizVisitor::visit(ifAST &node){
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

void GraphvizVisitor::visit(whileAST &node){
    std::size_t backup = nodeIndex;
    declarations.push_back(std::make_pair(std::to_string(backup), node.token.getStr()));
    nodeIndex++;
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.body->accept(*this);
    links.push_back(std::make_pair(std::to_string(backup), std::to_string(nodeIndex)));
    node.condition->accept(*this);
}

/*Определения TypeViewVisitor
==================*/
    TypeViewVisitor::TypeViewVisitor(){};

    void TypeViewVisitor::visit(BinOp &node){
        typesStrings.push_back(node.token.getStr());
        node.left->accept(*this);
        node.right->accept(*this);
    }

    void TypeViewVisitor::visit(UnOp &node){
        typesStrings.push_back(node.token.getStr());
        node.down->accept(*this);
    }

    void TypeViewVisitor::visit(Number &node){
        typesStrings.push_back(node.token.getStr());
    }

    void TypeViewVisitor::visit(Compound &node){
        typesStrings.push_back(node.token.getStr());
        for(auto child : node.children){
            child->accept(*this);
        }
    }

    void TypeViewVisitor::visit(Assign &node){
        typesStrings.push_back(node.token.getStr());
        node.var->accept(*this);
        node.value->accept(*this);
    }

    void TypeViewVisitor::visit(Var &node){
        typesStrings.push_back(node.token.getStr());
    }

    void TypeViewVisitor::visit(NoOp &node){
        typesStrings.push_back(node.token.getStr());
    }

    void TypeViewVisitor::visit(ProgramAST &node){
        typesStrings.push_back(node.token.getStr());
        node.block->accept(*this);
    }

    void TypeViewVisitor::visit(BlockAST &node){
        typesStrings.push_back(node.token.getStr());
        for(auto child : node.consts){
            child->accept(*this);
        }
        for(auto child : node.declarations){
            child->accept(*this);
        }
        node.compound->accept(*this);
    }

    void TypeViewVisitor::visit(VarDeclaration &node){
        typesStrings.push_back(node.token.getStr());
        node.var->accept(*this);
        node.type->accept(*this);
    }

    void TypeViewVisitor::visit(Type &node){
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

    void TypeViewVisitor::visit(ProcedureCall &node){
        typesStrings.push_back(node.token.getStr());
        for(auto child : node.params){
            child->accept(*this);
        }
    }

    void TypeViewVisitor::visit(ifAST &node){
        typesStrings.push_back(node.token.getStr());
        node.body->accept(*this);
        node.condition->accept(*this);
        if(node.elseBody != nullptr)
            node.elseBody->accept(*this);
    }

    void TypeViewVisitor::visit(whileAST &node){
        typesStrings.push_back(node.token.getStr());
        node.body->accept(*this);
        node.condition->accept(*this);
    }

    std::vector<std::string> TypeViewVisitor::getData(void){
        return typesStrings;
    }