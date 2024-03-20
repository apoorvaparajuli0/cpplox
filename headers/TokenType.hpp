#ifndef TOKENTYPE_HPP
#define TOKENTYPE_HPP

#include "string"

enum TokenType {
  // Single-character tokens.
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  // One or two character tokens.
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // Literals.
  IDENTIFIER, STRING, NUMBER,

  // Keywords.
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

  EOF_,

  NUM_TOKEN_TYPES
};

static const std::string typeNames[] = {
  "left_paren",  "right_paren",   "left_brace",
  "right_brace", "comma",         "dot",
  "minus",       "plus",          "semicolon",
  "slash",       "star",          "bang",
  "bang_equal",  "equal",         "equal_equal",
  "greater",     "greater_equal", "less",
  "less_equal",  "identifier",    "string",
  "number",      "and",           "class",
  "else",        "false",         "fun",
  "for",         "if",            "nil",
  "or",          "print",         "return",
  "super",       "this",          "true",
  "var",         "while",         "eof_"
};

static_assert(sizeof(typeNames) / sizeof(typeNames[0]) == NUM_TOKEN_TYPES, 
              "enum list of TokenTypes' size doesn't match that of it's respective string array");

#endif