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
        FOR,
        DO, ///< DO
        IF, ///< IF
        THEN, ///< THEN
        ELSE, ///< ELSE
        DOWN,
        TO,
        DOWNTO,

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
    virtual Type getType(void) const{
        return type;
    }

    /// @return Дополнительный тип токена
    virtual AdvType getAdvType(void) const{
        return advType;
    }

    /// @brief Устанавливает дополнительный тип токена
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

    Token(IToken::AdvType const type);
    Token(IToken::Type const type);

    /**
     * @brief Конструктор из строки токена и типа
     * @param[in] str строка, представляющая токен
     * @param[in] type тип токена
     */
    Token(std::string const &str, Type const type);
    /**
     * @brief Конструктор из строки токена и дополнительного типа
     * @param[in] str строка, представляющая токен
     * @param[in] type дополнительный тип токена
     */
    Token(std::string const &str, AdvType const type);

    Token(std::string const &str, Type const type, AdvType const advType);

    /**
     * @brief Конструктор из строки токена и всех его аттрибутов
     * @param[in] str строка, представляющая токен
     * @param[in] type тип токена
     * @param[in] advType дополнительный тип токена
     * @param[in] lineNum номер строки
     * @param[in] inLinePosNum позиция в строке
     * @param[in] inFilePos позиция в файле
     */
    Token(std::string const &str, Type const type, AdvType const advType, 
    std::size_t const lineNum, std::size_t const inLinePosNum, std::size_t const inFilePos);

    /// @return номер строки, начиная с 1, в которой находится токен
    std::size_t line(void) const;
    /// @return номер строки, начиная с 0, в которой находится токен
    std::size_t rawLine(void) const;

    /// @return Позицию в строке, начиная с 1, в которой находится токен
    std::size_t posInLine(void) const;
    /// @return Позицию в строке, начиная с 0, в которой находится токен
    std::size_t rawPosInLine(void) const;

    /// @return Абсолютную позицию в файле, начиная с 0
    std::size_t pos(void) const;

    /// @brief Возвращает константную ссылку на строковое представление токена
    std::string const & getStr(void) const;

    void setStr(std::string const &toSet);

    std::string getInfo(void) const;

    ///@return Длину строкового представления токена
    std::size_t len(void);


    private:
    /// @brief Строковое представление токена
    std::string str;
    std::size_t lineNum = std::string::npos;
    std::size_t posInLineNum = std::string::npos;
    std::size_t posNum = std::string::npos;
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