#include_directories(
#    ${PROJECT_SOURCE_DIR}/parser
#    ${PROJECT_SOURCE_DIR}/parser/functions
#    ${PROJECT_SOURCE_DIR}/parser/parser
#)

set(PARSER_FILES
${PROJECT_SOURCE_DIR}/parser/functions/Func_source.cpp
${PROJECT_SOURCE_DIR}/parser/functions/Func_source.h
${PROJECT_SOURCE_DIR}/parser/functions/UserFunction.cpp
${PROJECT_SOURCE_DIR}/parser/functions/UserFunction.h
${PROJECT_SOURCE_DIR}/parser/parser/grammar.tab.cpp
${PROJECT_SOURCE_DIR}/parser/parser/grammar.tab.h
${PROJECT_SOURCE_DIR}/parser/parser/lex.yy.cpp
${PROJECT_SOURCE_DIR}/parser/parser/Parser.cpp
${PROJECT_SOURCE_DIR}/parser/parser/Parser.h
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxAssignExpr.cpp
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxAssignExpr.h
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxBinaryExpr.cpp
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxBinaryExpr.h
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxClassDef.cpp
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxClassDef.h
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxConstant.cpp
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxConstant.h
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxElement.cpp
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxElement.h
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxForCondition.cpp
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxForCondition.h
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxFormal.cpp
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxFormal.h
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxFunctionCall.cpp
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxFunctionCall.h
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxFunctionDef.cpp
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxFunctionDef.h
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxLabeledExpr.cpp
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxLabeledExpr.h
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxStatement.cpp
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxStatement.h
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxUnaryExpr.cpp
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxUnaryExpr.h
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxVariable.cpp
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxVariable.h
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxVariableDecl.cpp
${PROJECT_SOURCE_DIR}/parser/parser/SyntaxVariableDecl.h
#${PROJECT_SOURCE_DIR}/parser/RbMain.cpp
)

add_library(rb-parser ${PARSER_FILES})
