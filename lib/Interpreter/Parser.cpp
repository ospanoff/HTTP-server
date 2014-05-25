//
//  Parser.cpp
//  Interpreter
//
//  Created by Ayat Ospanov on 15.05.14.
//  Copyright (c) 2014 MSU CMC. All rights reserved.
//

#include "Parser.h"

void Parser::analyze()
{
	gl();
	H();
//	cout << prog;
//	cout << endl << "Compilation ended!!!" << endl;
}

void Parser::H()
{
	while (c_type != LEX_FIN) {
		if (c_type == LEX_FUNC)
			A();
		else
			O();
	}
}

void Parser::A()
{
	if (c_type == LEX_FUNC) {
		prog.put_lex(Lex (LEX_FUNC, LEX_FUNC));
		inFunc = true;
		gl();
		isFuncName = true;
		N();
		B();
		if (!funcReturn) {
			throw "No return in func";
		}
		funcReturn = false;
		inFunc = false;
	} else
		throw curr_lex;
}

void Parser::O()
{
	switch (c_type) {
		case LEX_VAR:
			V();
			if (c_type != LEX_SEMICOLON)
				throw curr_lex;
			gl();
			break;
		case LEX_SEMICOLON:
			P();
			gl();
			break;
		case LEX_BEGIN:
			B();
			break;
		case LEX_IF:
			I();
			break;
		case LEX_WHILE:
		case LEX_FOR:
		case LEX_DO:
			inCycle = true;
			F();
			inCycle = false;
			break;
		case LEX_BREAK:
		case LEX_CONT:
		case LEX_RET:
			J();
			break;
		case LEX_ID:
		case LEX_PLPL:
		case LEX_MINMIN:
		case LEX_NOT:
			E();
			if (c_type != LEX_SEMICOLON)
				throw curr_lex;
			prog.put_lex(curr_lex);
			gl();
			break;
		case LEX_TYPEOF:
			throw "Can't call \"typeof\" function without assigning";
			break;
		default:
			throw curr_lex;
			break;
	}
}

void Parser::TO() {
	if (c_type == LEX_TYPEOF) {
		gl();
		if (c_type != LEX_LPAREN)
			throw curr_lex;
		gl();
		F2();
		if (c_type != LEX_RPAREN)
			throw curr_lex;
		prog.put_lex(Lex (LEX_TYPEOF, LEX_TYPEOF));
	}
	gl();
}

void Parser::V()
{
	if (c_type == LEX_VAR) {
		gl();
		N();
		if (c_type == LEX_ASSIGN) {
			gl();
			N();
			prog.put_lex(Lex (LEX_ASSIGN, c_val));
		}
		while (c_type == LEX_COMMA) {
			gl();
			N();
			if (c_type == LEX_ASSIGN) {
				gl();
				N();
				prog.put_lex(Lex (LEX_ASSIGN, c_val));
			}
		}
	} else
		throw curr_lex;
}

void Parser::P()
{
	if (c_type == LEX_SEMICOLON) {
		prog.put_lex(curr_lex);
		return;
	} else
		throw curr_lex;
}

void Parser::B()
{
	if (c_type == LEX_BEGIN) {
		gl();
		O();
		while (c_type != LEX_END) {
			O();
		}
		gl();
	} else
		throw curr_lex;
}

void Parser::I()
{
	int pl0, pl1;
	if (c_type == LEX_IF) {
		gl();
		if (c_type == LEX_LPAREN) {
			gl();
			E();
		} else
			throw curr_lex;
		pl0 = prog.get_free();
		prog.blank();
		prog.put_lex(Lex (POLIZ_FGO, POLIZ_FGO));
		if (c_type != LEX_RPAREN)
			throw curr_lex;
		gl();
		if (c_type == LEX_BEGIN)
			B();
		else
			O();
		prog.put_lex(Lex (POLIZ_LABEL, prog.get_free()), pl0);
		if (c_type == LEX_ELSE) {
			pl1 = prog.get_free();
			prog.blank();
			prog.put_lex(Lex (POLIZ_GO, POLIZ_GO));
			prog.put_lex(Lex (POLIZ_LABEL, prog.get_free()), pl0);
			gl();
			if (c_type == LEX_BEGIN)
				B();
			else
				O();
			prog.put_lex(Lex (POLIZ_LABEL, prog.get_free()), pl1);
		}
	} else
		throw curr_lex;
}

void Parser::F()
{
	int pl0(0), pl1(0), pl2(0), pl3(0);
	
	switch (c_type) {
		case LEX_WHILE:
			pl0 = prog.get_free();
			gl();
			if (c_type != LEX_LPAREN)
				throw curr_lex;
			gl();
			E();
			pl1 = prog.get_free();
			prog.blank();
			prog.put_lex(Lex (POLIZ_FGO, POLIZ_FGO));
			if (c_type != LEX_RPAREN)
				throw curr_lex;
			gl();
			if (c_type == LEX_BEGIN)
				B();
			else
				O();
			if (con_pl) {
				prog.put_lex(Lex (POLIZ_LABEL, prog.get_free()), con_pl);
				prog.put_lex(Lex (POLIZ_GO, POLIZ_GO), con_pl + 1);
			}
			prog.put_lex(Lex (POLIZ_LABEL, pl0));
			prog.put_lex(Lex (POLIZ_GO, POLIZ_GO));
			prog.put_lex(Lex (POLIZ_LABEL, prog.get_free()), pl1);
			if (br_pl) {
				prog.put_lex(Lex (POLIZ_LABEL, prog.get_free()), br_pl);
				prog.put_lex(Lex (POLIZ_GO, POLIZ_GO), br_pl + 1);
			}
			break;
		case LEX_FOR:
			gl();
			if (c_type != LEX_LPAREN)
				throw curr_lex;
			gl();
			if (c_type == LEX_VAR)
				gl();
			pl0 = prog.get_free();
			N();
			if (c_type == LEX_IN) {
				gl();
				N();
			} else {
				if (c_type != LEX_ASSIGN)
					throw curr_lex;
				prog.put_lex(Lex (POLIZ_ADDRESS, prog[pl0].get_value()), pl0);
				gl();
				N();
				prog.put_lex(Lex (LEX_ASSIGN, LEX_ASSIGN));
				pl0 = prog.get_free();
				if (c_type != LEX_SEMICOLON)
					throw curr_lex;
				gl();
				N();
				pl1 = prog.get_free();
				prog.blank();
				prog.put_lex(Lex (POLIZ_FGO, POLIZ_FGO));
				pl2 = prog.get_free();
				prog.blank();
				prog.put_lex(Lex (POLIZ_GO, POLIZ_GO));
				pl3 = prog.get_free();
				if (c_type != LEX_SEMICOLON)
					throw curr_lex;
				gl();
				E();
				prog.put_lex(Lex (POLIZ_LABEL, pl0));
				prog.put_lex(Lex (POLIZ_GO, POLIZ_GO));
				prog.put_lex(Lex (POLIZ_LABEL, prog.get_free()), pl2);
			}
			if (c_type != LEX_RPAREN)
				throw curr_lex;
			gl();
			if (c_type == LEX_BEGIN)
				B();
			else
				O();
			if (con_pl) {
				prog.put_lex(Lex (POLIZ_LABEL, prog.get_free()), con_pl);
				prog.put_lex(Lex (POLIZ_GO, POLIZ_GO), con_pl + 1);
			}
			prog.put_lex(Lex (POLIZ_LABEL, pl3));
			prog.put_lex(Lex (POLIZ_GO, POLIZ_GO));
			prog.put_lex(Lex (POLIZ_LABEL, prog.get_free()), pl1);
			if (br_pl) {
				prog.put_lex(Lex (POLIZ_LABEL, prog.get_free()), br_pl);
				prog.put_lex(Lex (POLIZ_GO, POLIZ_GO), br_pl + 1);
			}
			break;
		case LEX_DO:
			pl0 = prog.get_free();
			gl();
			if (c_type == LEX_BEGIN)
				B();
			else
				O();
			if (c_type != LEX_WHILE)
				throw curr_lex;
			if (con_pl) {
				prog.put_lex(Lex (POLIZ_LABEL, prog.get_free()), con_pl);
				prog.put_lex(Lex (POLIZ_GO, POLIZ_GO), con_pl + 1);
			}
			gl();
			if (c_type != LEX_LPAREN)
				throw curr_lex;
			gl();
			E();
			if (c_type != LEX_RPAREN)
				throw curr_lex;
			prog.put_lex(Lex (LEX_NOT, LEX_NOT));
			prog.put_lex(Lex (POLIZ_LABEL, pl0));
			prog.put_lex(Lex (POLIZ_FGO, POLIZ_FGO));
			gl();
			if (c_type != LEX_SEMICOLON)
				throw curr_lex;
			prog.put_lex(curr_lex);
			if (br_pl) {
				prog.put_lex(Lex (POLIZ_LABEL, prog.get_free()), br_pl);
				prog.put_lex(Lex (POLIZ_GO, POLIZ_GO), br_pl + 1);
			}
			gl();
			break;
		default:
			throw curr_lex;
	}
}

void Parser::J()
{
	switch (c_type) {
		case LEX_BREAK:
			br_pl = 0;
			if (!inCycle)
				throw "Break is not in cycle";
//			prog.put_lex(curr_lex);
			br_pl = prog.get_free();
			prog.blank();
			prog.blank();
			gl();
			if (c_type != LEX_SEMICOLON)
				throw curr_lex;
			prog.put_lex(curr_lex);
			break;
		case LEX_CONT:
			con_pl = 0;
			if (!inCycle)
				throw "Continue is not in cycle";
//			prog.put_lex(curr_lex);
			con_pl = prog.get_free();
			prog.blank();
			prog.blank();
			gl();
			if (c_type != LEX_SEMICOLON)
				throw curr_lex;
			prog.put_lex(curr_lex);
			break;
		case LEX_RET:
			gl();
			if (!inFunc)
				throw "Return is not in function";
			if (c_type != LEX_SEMICOLON) {
				E();
			}
			prog.put_lex(Lex (LEX_RET, LEX_RET));
			if (c_type != LEX_SEMICOLON)
				throw curr_lex;
			prog.put_lex(curr_lex);
			funcReturn = true;
			break;
		default:
			throw curr_lex;
	}
	gl();
}

void Parser::E()
{
	int i = prog.get_free();
	N();
	if (c_type == LEX_ASSIGN || c_type == LEX_PLEQ ||
		c_type == LEX_MINEQ || c_type == LEX_MULEQ ||
		c_type == LEX_DIVEQ || c_type == LEX_MODEQ) {
		prog.put_lex(Lex (POLIZ_ADDRESS, prog[i].get_value()), i);
		type_of_lex tmp = curr_lex.get_type();
		if (c_type != LEX_ASSIGN)
			prog.put_lex(Lex (LEX_ID, prog[i].get_value()));
		gl();
		if (c_type == LEX_TYPEOF)
			TO();
		else
			N();
		switch (tmp) {
			case LEX_PLEQ:
				prog.put_lex(Lex (LEX_PLUS, LEX_PLUS));
				break;
			case LEX_MINEQ:
				prog.put_lex(Lex (LEX_MINUS, LEX_MINUS));
				break;
			case LEX_MULEQ:
				prog.put_lex(Lex (LEX_MULT, LEX_MULT));
				break;
			case LEX_DIVEQ:
				prog.put_lex(Lex (LEX_DIV, LEX_DIV));
				break;
			case LEX_MODEQ:
				prog.put_lex(Lex (LEX_MOD, LEX_MOD));
				break;
			default:
				break;
		}
		prog.put_lex(Lex (LEX_ASSIGN, LEX_ASSIGN));
	}
}

void Parser::N()
{
	if (isFuncName || (c_type == LEX_ID && (!strcmp(TID[curr_lex.get_value()].get_name(), "write")
		|| !strcmp(TID[curr_lex.get_value()].get_name(), "read")))) {
		TID[curr_lex.get_value()].set_isFuncName();
		isFuncName = false;
	}
	N1();
	if (c_type == LEX_EQ || c_type == LEX_NEQ || c_type == LEX_LSS ||
		c_type == LEX_LEQ || c_type == LEX_GTR || c_type == LEX_GEQ ||
		c_type == LEX_ABEQ || c_type == LEX_ABNEQ) {
		Lex tmp = curr_lex;
		gl();
		N1();
		switch (tmp.get_type()) {
			case LEX_GEQ:
				prog.put_lex(Lex (LEX_LSS));
				prog.put_lex(Lex (LEX_NOT));
				break;
			case LEX_LEQ:
				prog.put_lex(Lex (LEX_GTR));
				prog.put_lex(Lex (LEX_NOT));
				break;
			case LEX_NEQ:
				prog.put_lex(Lex (LEX_EQ));
				prog.put_lex(Lex (LEX_NOT));
				break;
			case LEX_ABNEQ:
				prog.put_lex(Lex (LEX_ABEQ));
				prog.put_lex(Lex (LEX_NOT));
				break;
			default:
				prog.put_lex(tmp);
				break;
		}
	}
}

void Parser::N1()
{
	T();
	while (c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR) {
		Lex tmp = curr_lex;
		gl();
		T();
		prog.put_lex(tmp);
	}
}

void Parser::T()
{
	F1();
	while (c_type == LEX_MULT || c_type == LEX_DIV ||
		   c_type == LEX_AND || c_type == LEX_MOD) {
		Lex tmp = curr_lex;
		gl();
		F1();
		prog.put_lex(tmp);
	}
}

void Parser::F1()
{
	if (c_type == LEX_PLPL || c_type == LEX_MINMIN || c_type == LEX_NOT) {
		Lex tmp = curr_lex;
		gl();
		int i = prog.get_free();
		F2(); // ++f2, --f2
		if (tmp.get_type() != LEX_NOT) {
			prog.put_lex(Lex (POLIZ_ADDRESS, prog[i].get_value()), i);
			prog.put_lex(Lex (LEX_ID, prog[i].get_value()));
			prog.put_lex(Lex (LEX_NUM, 1));
			if (tmp.get_type() == LEX_PLPL)
				prog.put_lex(Lex (LEX_PLUS, LEX_PLUS));
			else
				prog.put_lex(Lex (LEX_MINUS, LEX_MINUS));
			prog.put_lex(Lex (LEX_ASSIGN, LEX_ASSIGN));
			prog.put_lex(Lex (LEX_ID, prog[i].get_value()));
		} else {
			prog.put_lex(tmp);
		}
	} else {
		int i = prog.get_free();
		F2();
		if (c_type == LEX_PLPL || c_type == LEX_MINMIN) {
			prog.put_lex(Lex (POLIZ_ADDRESS, prog[i].get_value()));
			prog.put_lex(Lex (LEX_ID, prog[i].get_value()));
			prog.put_lex(Lex (LEX_NUM, 1));
			if (c_type == LEX_PLPL)
				prog.put_lex(Lex (LEX_PLUS, LEX_PLUS));
			else
				prog.put_lex(Lex (LEX_MINUS, LEX_MINUS));
			prog.put_lex(Lex (LEX_ASSIGN, LEX_ASSIGN));
			gl(); // f2++, f2--
		}
	}
}

void Parser::F2()
{
	if (c_type == LEX_ID) {
		prog.put_lex(curr_lex);
		gl();
		Nam();
	} else if (c_type == LEX_NUM || c_type == LEX_STR || c_type == LEX_DOUBLE ||
			   c_type == LEX_TRUE || c_type == LEX_FALSE) {
		prog.put_lex(curr_lex);
		gl();
	} else if (c_type == LEX_LPAREN) {
		gl();
		E();
		if (c_type != LEX_RPAREN)
			throw curr_lex;
		gl();
	} else
		throw curr_lex;
}

void Parser::Nam()
{
	if (c_type == LEX_LSQBR) {
		gl();
		N();
		prog.put_lex(Lex (LEX_LSQBR, LEX_LSQBR));
		if (c_type != LEX_RSQBR)
			throw curr_lex;
	} else if (c_type == LEX_LPAREN) {
		prog.put_lex(curr_lex);
		gl();
		if (c_type == LEX_ID || c_type == LEX_NUM || c_type == LEX_STR ||
			c_type == LEX_DOUBLE || c_type == LEX_TRUE || c_type == LEX_FALSE)
			E();
		while (c_type == LEX_COMMA) {
			gl();
			E();
		}
		if (c_type != LEX_RPAREN)
			throw curr_lex;
		prog.put_lex(curr_lex);
	} else {
		return;
	}
	gl();
	return;
}