### micro-Pascal

Синт. анализатор, который формирует абстрактное дерево синтаксиса по коду на языке Pascal (пока что?)

Состоит из:
* Лексера - разбивате на токены полученную программу
* Синтаксического анализатора - из токенов формирует дерево программы
* Различные посетители (наследники IVisitor) - обходят дерево, выполняя какую-то задачу (Например, GraphizVisitor формирует изображения деревьев, как ниже)

![Изображение дерева, если вы видите этот текст - у вас не прогрузилось](forReadme/exmp1.svg)

Текст программы, по которому построено дерево:
```Pascal
PROGRAM test;

CONST eps = 0.0001;

VAR a, b, fa, fb, x, fx : REAL;
BEGIN
  write('Inter numbers a and b (a<b): ');
  readln(a,b);
  assert(a<b);

  fa := sin(a);
  fb := sin(b);
  assert(fb*fa<0);
  { comment, hello 
  
  
  }
  WHILE (b-a) > eps DO
    BEGIN
        x := (b+a)/2;
        fx := sin(x);
        IF fa*fx <= 0 THEN
            b := x
        ELSE
            BEGIN
                a := x;
                fa := fx;
            END;
    END;

  writeln('Root of function on [a,b] is equal ',(b+a)/2);
END.
```

Если убрать точку в конце программы, то анализатор сообщит об этом:
![Изображение с сообщением об ошибке](forReadme/exmp1Error.png)