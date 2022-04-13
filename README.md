### micro-Pascal

Синт. анализатор, который формирует абстрактное дерево синтаксиса по коду на языке Pascal (пока что?)

Состоит из:
* Лексера - разбивате на токены полученную программу
* Синтаксического анализатора - из токенов формирует дерево программы
* Различные посетители (наследники IVisitor) - обходят дерево, выполняя какую-то задачу (Например, GraphizVisitor формирует изображения деревьев, как ниже)
