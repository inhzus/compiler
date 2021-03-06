
#ifndef COMPILER_SEMANTIC_H_
#define COMPILER_SEMANTIC_H_

#include <stdbool.h>
#include "intercode.h"
#include "node.h"

#define HASH_SIZE 0x3fffu

#define _INT 1
#define _FLOAT 2

typedef enum Kind_ { KBASIC, KARRAY, KSTRUCT, KFUNC } Kind;
typedef struct Type_ *Type;
typedef struct FieldList_ *FieldList;

typedef struct Type_ {
  Kind kind;
  union {
    int basic_;  // _INT || _FLOAT
    struct {
      int size;
      Type type;
    } array_;
    FieldList struct_;
    struct {
      FieldList params;  // array of params
      Type funcType;
      int paramNum;  // number of params
    } func_;
  };
} Type_;

typedef struct FieldList_ {
  char *name;
  Type type;
  FieldList tail;
  bool isArg;
} FieldList_;

object *getChild(object *root, int i);
int countChild(object *root);
unsigned int hashFunc(char *name);
void initHashTable();
bool insertSymbol(FieldList field);
FieldList findSymbol(char *name, bool isFunc);
bool isTypeEqual(Type p1, Type p2);
FieldList goVarDec(object *root, Type type);
Type goSpecifier(object *root);
void goExtDefList(object *root);
void goCompSt(object *compSt, Type funcType);
void goDefList(object *defList);
void goStmt(object *stmt, Type funcType);
Type goExp(object *exp, Operand upshot);
Type goCondition(object *exp, Operand trueLabelOperand, Operand falseLabelOperand);
void traverseTree(object *root);

Operand genLabelOperand();
InterCode genLabelInterCode(Operand operand);
InterCode genGotoLabelInterCode(Operand operand);
int getSize(Type type, bool isArray);

InterCode genInterCode(_InterCodeKind kind);
Operand genEmptyOperand();
Operand genOperand(_OperandKind kind);
void setOperandTemp(Operand operand);
Operand genTempOperand();
Operand genOperandInt(_OperandKind kind, int value);
Operand genOperandStr(_OperandKind kind, const char *src);
InterCode genInterCodeUnary(_InterCodeKind kind, Operand operand);
InterCode genInterCodeBinary(_InterCodeKind kind, Operand left, Operand right);
InterCode genInterCodeTernary(_InterCodeKind kind, Operand res, Operand left, Operand right);
InterCode genInterCodeIfGoto(Operand label, Operand left, Operand right, const char *relop);
InterCode genInterCodeDec(Operand op, int size);
#endif