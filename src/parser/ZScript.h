#ifndef ZSCRIPT_H
#define ZSCRIPT_H

#include "DataStructs.h"
#include "Scope.h"

namespace ZScript
{
	class Program
	{
	public:
		Program(ASTProgram* ast);
		~Program();
		ASTProgram* node;
		SymbolTable table;
		GlobalScope globalScope;

		vector<Script*> scripts;
		Script* getScript(string const& name) const;
		Script* getScript(ASTScript* node) const;

		// Gets the global user-defined and (deprecated) script level variables.
		vector<Variable*> getUserGlobalVariables() const;

		// Gets the non-internal (user-defined) global scope functions.
		vector<Function*> getUserGlobalFunctions() const;

		bool hasError() const;
		void printErrors() const;

	private:
		map<string, Script*> scriptsByName;
		map<ASTScript*, Script*> scriptsByNode;
	};

	struct Script
	{
		Script(Program& program, ASTScript* script);

		ASTScript* node;
		ScriptScope* scope;

		string getName() const;
		ScriptType getType() const;
		Function* getRun() const;

		bool hasError() const;
		void printErrors() const;
	};

	struct Variable
	{
		Variable(ASTDecl* node, ZVarType const* type, string const& name, int id)
				: node(node), type(type), name(name), id(id) {}
		ASTDecl* node;
		ZVarType const* type;
		string name;
		int id;
	};

	struct Function
	{
		Function(ZVarType const* returnType, string const& name, vector<ZVarType const*> paramTypes, int id)
				: node(NULL), internalScope(NULL), thisVar(NULL),
				  returnType(returnType), name(name), paramTypes(paramTypes), id(id)
			{}
		ZVarType const* returnType;
		string name;
		vector<ZVarType const*> paramTypes;
		int id;

		ASTFuncDecl* node;
		Scope* internalScope;
		Variable* thisVar;
	};
}

#endif
