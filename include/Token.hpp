#ifndef __INC_TOKEN_H
#define __INC_TOKEN_H

#include <string>
#include <regex>

/**
 * @brief Класс-родитель для классов, работающих с токенами
 */
class IToken{
    public:
    /// @brief Типы токенов
    enum AdvType{
        KEYWORD,
        FUNCTION_NAME,
        VAR_NAME,
        OPERATOR,
        SOME_CONST,
        TYPE_SPEC,
        NOTPROCESS,
        PROGRAM_NAME,
        VARDECL, ///< Объявление переменной
        CONSTDECL, ///< Объявление константы
        COMPOUND,
        PROGSTART, ///< Старт программы
        UNKNOWN
    };

    enum Type{
        ERROR, ///< Ошибка
        ENDOFSTREAM, ///< Конец потока токенов (конец файла)
        EMPTY, ///< Пустое выражение
        BLOCK, ///< Блок
        TERMINATE, ///< Неявная замена SEMI
        NEWLINE,
        COMMENT,

        ID, ///< Индентификатор

        INTEGER_CONST, ///< Целая константа
        REAL_CONST, ///< Вещественная константа
        STRING_CONST,

        LPAREN, ///< Левая скобка
        RPAREN, ///< Правая скобка
        LBRACE, ///< Левая фигурная скобка
        RBRACE, ///< Правая фигурная скобка
        LSQBRACKET, ///< Левая квадратная собка
        RSQBRACKET, ///< Правая квадратная скобка

        PLUS, ///< Плюс
        MINUS, ///< Минус
        MUL, ///< Умножение
        DIV, ///< Depricated!!!!
        MOD, ///< Взятие модуля
        INTEGER_DIV, ///< Целочисленное деление
        FLOAT_DIV, ///< Вещественное деление

        AND, ///< Логическое И
        NOT, ///< Логическое отрицание
        OR, ///< Логическое ИЛИ
        EQ, ///< =
        NEQ, ///< <>
        LESS, ///< <
        LESS_EQ, ///< <=
        MORE, ///< >
        MORE_EQ, ///< >+

        STRING_BOUND, ///< '

        ASSIGN, ///< Присваивание

        BEGIN, ///< Начало блока
        END, ///< Конец блока
        PROGRAM,  ///<Начало программы
        VAR, ///< Ключевое слово VAR
        CONST, ///< Ключевое слово CONST
        INTEGER, ///< Целый тип
        REAL, ///< Вещественный тип
        WHILE, ///< WHILE
        DO, ///< DO
        IF, ///< IF
        THEN, ///< THEN
        ELSE, ///< ELSE

        DOT, ///< Точка
        SEMI, ///< Точка с запятой
        COLON, ///< Двоеточие
        COMMA, ///< Запятая

        // Дополнительные ключевые слова ?

    };

    /// @brief Конструктор по-умолчанию
    IToken(void);

    /**
     * @brief Конструктор из типа
     * @param[in] type тип токена
     */
    IToken(Type type);
    IToken(AdvType type) : advType(type){};
    IToken(Type type, AdvType advType);

    /// @brief Получение типа токена
    virtual Type getType(void){
        return type;
    }

    virtual AdvType getAdvType(void){
        return advType;
    }

    virtual void setAdvType(IToken::AdvType newAdvType){
        advType = newAdvType;
    }

    private:
    /// @brief Тип токена
    Type type = ERROR;
    AdvType advType = UNKNOWN;
};

/// @brief Класс, представляющий токен
class Token : public IToken{
    public:

    /// @brief Конструктор по-умолчанию
    Token();

    /**
     * @brief Конструктор из строки токена и типа
     * @param[in] str строка, представляющая токен
     * @param[in] type тип токена
     */
    Token(IToken::AdvType type) : IToken(type){};
    Token(IToken::Type type) : IToken(type){};
    Token(std::string str) : str(str){};
    Token(std::string str, Type type);
    Token(std::string str, AdvType type) : str(str), IToken(type){};
    Token(std::string str, Type type, std::size_t lineNum);

    Token(std::string str, Type type, std::size_t lineNum, std::size_t inLinePosNum, std::size_t inFilePos);

    Token(std::string str, Type type, AdvType advType, std::size_t lineNum, std::size_t inLinePosNum, std::size_t inFilePos);

    /// @return номер строки, начиная с 1, в которой находится токен
    std::size_t line(void);
    /// @return номер строки, начиная с 0, в которой находится токен
    std::size_t rawLine(void);

    /// @return Позицию в строке, начиная с 1, в которой находится токен
    std::size_t posInLine(void);
    /// @return Позицию в строке, начиная с 0, в которой находится токен
    std::size_t rawPosInLine(void);

    /// @return Абсолютную позицию в файле, начиная с 0
    std::size_t pos(void);

    /// @brief Возвращает константную ссылку на строковое представление токена
    std::string const & getStr(void);

    void setStr(std::string const &toSet);

    std::string getInfo(void);

    ///@return Длину строкового представления токена
    std::size_t len(void);


    private:
    /// @brief Строковое представление токена
    std::string str;
    std::size_t lineNum;
    std::size_t posInLineNum;
    std::size_t posNum;
};

/// @brief Класс, представляющий шаблон токена
class TokenTemplate : public IToken{
    public:

    /// @brief Конструктор по-умолчанию
    TokenTemplate();

    /**
     * @brief Конструктор из regex-строки и типа токена
     * @param str regex строка, представляющая шаблон для определения токена
     * @param type тип токена
     */
    TokenTemplate(std::string str, Type type, AdvType advType);

    /// @brief Возвращает константную ссылку на regex-выражение токена
    std::regex const & getRegex(void);

    private:
    /// @brief Строковое представление regex-шаблона
    std::string regStr;
    /// @brief Regex-шаблон токена
    std::regex regTemplate;

};

#endif