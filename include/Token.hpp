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
    enum Type{
        ERROR, ///< Ошибка
        ENDOFSTREAM, ///< Конец потока токенов (конец файла)
        EMPTY, ///< Пустое выражение
        BLOCK, ///< Блок
        VARDECL, ///< Объявление переменной
        PROGSTART, ///< Старт программы
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

    /// @brief Получение типа токена
    virtual Type getType(void){
        return type;
    }

    private:
    /// @brief Тип токена
    Type type;
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
    Token(std::string str, Type type);
    Token(std::string str, Type type, std::size_t lineNum);

    std::size_t line(void);
    std::size_t rowLine(void);
    std::size_t pos(void);

    /// @brief Возвращает константную ссылку на строковое представление токена
    std::string const & getStr(void);

    std::string getInfo(void);

    private:
    /// @brief Строковое представление токена
    std::string str;
    std::size_t lineNum;
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
    TokenTemplate(std::string str, Type type);

    /// @brief Возвращает константную ссылку на regex-выражение токена
    std::regex const & getRegex(void);

    private:
    /// @brief Строковое представление regex-шаблона
    std::string regStr;
    /// @brief Regex-шаблон токена
    std::regex regTemplate;

};

#endif