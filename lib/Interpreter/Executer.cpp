//
//  Executer.cpp
//  Interpreter
//
//  Created by Ayat Ospanov on 15.05.14.
//  Copyright (c) 2014 MSU CMC. All rights reserved.
//

#include "Executer.h"

void Executer::execute(Poliz & prog)
{
	int i, j, jump(0), index(0), size = prog.get_free();
	T t1, t2;
	while (index < size) {
		pc_el = prog[index];
		switch (pc_el.get_type()) {
			// PUSHES
			case LEX_STR:
				args.push(pc_el.get_svalue());
				break;
			case LEX_NUM:
			case POLIZ_ADDRESS:
			case POLIZ_LABEL:
				args.push(pc_el.get_value());
				break;
			case LEX_DOUBLE:
				args.push(pc_el.get_dvalue());
				break;
			case LEX_TRUE:
				args.push(true);
				break;
			case LEX_FALSE:
				args.push(false);
				break;
			// POPS, WORK WITH POLIZ
			// FUNCTION
			case LEX_TYPEOF: {
				t1 = args.pop();
				args.push(lex_names[t1.tl]);
				break;
			}
			case LEX_FUNC:
				index++;
				pc_el = prog[index];
				TID[pc_el.get_value()].set_plInPoliz(index+2);
				while (prog[index].get_type() != LEX_RET)
					index++;
				break;
			// ID
			case LEX_ID:
				i = pc_el.get_value();
				if (!strcmp(TID[i].get_name(), "write")) {
					index += 2;
					switch (prog[index].get_type()) {
						case LEX_ID:
							j = prog[index].get_value();
							switch (TID[j].get_type()) {
								case LEX_STR:
									cout << TID[j].get_svalue();
									break;
								case LEX_INT:
									cout << TID[j].get_value();
									break;
								case LEX_BOOL:
									if (TID[j].get_value())
										cout << "true";
									else
										cout << "false";
									break;
								case LEX_DOUBLE:
									cout << TID[j].get_dvalue();
									break;
								default:
									cout << "undefined";
									break;
							}
							break;
						case LEX_NUM:
							cout << prog[index].get_value();
							break;
						case LEX_STR:
							cout << prog[index].get_svalue();
							break;
						case LEX_DOUBLE:
							cout << prog[index].get_dvalue();
							break;
						default:
							throw "wrong parametres";
							break;
					}
				} else if (!strcmp(TID[i].get_name(), "read")) {
					index += 2;
					if (prog[index].get_type() != LEX_ID)
						throw "Can't read to not ID";
					char s[128];
					bool isNum = true;
					cin >> s;
					int i = 0;
					while (i < strlen(s)) {
						if ('9' < s[i] || s[i++] < '0') {
							isNum = false;
							break;
						}
					}
					if (isNum) {
						TID[prog[index].get_value()].put_value(atoi(s));
					} else {
						TID[prog[index].get_value()].put_svalue(s);
					}
				} else if (TID[i].get_isFuncName()) {
					index += 2;
					int k = 0;
					while (prog[index].get_type() != LEX_RPAREN) {
						switch (prog[index].get_type()) {
							case LEX_NUM:
								func_args[k].tl = LEX_INT;
								func_args[k++].i = prog[index].get_value();
								break;
							case LEX_TRUE:
								func_args[k].tl = LEX_BOOL;
								func_args[k++].i = prog[index].get_value();
								break;
							case LEX_FALSE:
								func_args[k].tl = LEX_BOOL;
								func_args[k++].i = prog[index].get_value();
								break;
							case LEX_STR:
								func_args[k].tl = LEX_STR;
								func_args[k++].s = prog[index].get_svalue();
								break;
							case LEX_DOUBLE:
								func_args[k].tl = LEX_DOUBLE;
								func_args[k++].d = prog[index].get_dvalue();
								break;
							case LEX_ID: {
								Ident id = TID[prog[index].get_value()];
								func_args[k].tl = id.get_type();
								switch (id.get_type()) {
									case LEX_INT:
									case LEX_BOOL:
										func_args[k++].i = id.get_value();
										break;
									case LEX_DOUBLE:
										func_args[k++].d = id.get_dvalue();
										break;
									case LEX_STR:
										func_args[k++].s = id.get_svalue();
										break;
									default:
										break;
								}
								break;
							}
							default:
								break;
						}
						index++;
					}
					jump = index;
					index = TID[i].get_plInPoliz();
					for (int i = 0; i < k; i++, index++) {
						T tmp = func_args[i];
						int j = prog[index].get_value();
						TID[j].put_type(tmp.tl);
						switch (tmp.tl) {
							case LEX_INT:
							case LEX_BOOL:
								TID[j].put_value(tmp.i);
								break;
							case LEX_DOUBLE:
								TID[j].put_dvalue(tmp.d);
								break;
							case LEX_STR:
								TID[j].put_svalue(tmp.s);
								break;
							default:
								break;
						}
					}
					if (prog[index].get_type() != LEX_RPAREN)
						throw "Wrong function";
				} else {
					if (!TID[i].get_isFuncName() && prog[index+1].get_type() == LEX_LPAREN)
						throw "No function";
					if (TID[i].get_type() == LEX_STR)
						args.push(TID[i].get_svalue());
					else if (TID[i].get_type() == LEX_INT)
						args.push(TID[i].get_value());
					else if (TID[i].get_type() == LEX_BOOL)
						args.push((bool) TID[i].get_value());
					else if (TID[i].get_type() == LEX_DOUBLE)
						args.push(TID[i].get_dvalue());
					else if (TID[i].get_type() == LEX_NULL)
						throw "Undefined var";
				}
				break;
			// PLUS +
			case LEX_PLUS: {
				t2 = args.pop();
				t1 = args.pop();
				char *s = new char [128];
				if (t1.tl == LEX_INT) {
					switch (t2.tl) {
						case LEX_INT:
						case LEX_BOOL:
							args.push(t1.i + t2.i);
							break;
						case LEX_DOUBLE:
							args.push((double) t1.i + t2.d);
							break;
						case LEX_STR:
							sprintf(s, "%d", t1.i);
							args.push(strcat(s, t2.s));
							break;
						default:
							throw "unknown type";
							break;
					}
				} else if (t1.tl == LEX_STR) {
					switch (t2.tl) {
						case LEX_STR:
							args.push(strcat(t1.s, t2.s));
							break;
						case LEX_BOOL:
							if (t2.i)
								sprintf(s, "true");
							else
								sprintf(s, "false");
							args.push(strcat(t1.s, s));
							break;
						case LEX_DOUBLE:
							sprintf(s, "%lf", t2.d);
							args.push(strcat(t1.s, s));
							break;
						case LEX_INT:
							sprintf(s, "%d", t2.i);
							args.push(strcat(t1.s, s));
							break;
						default:
							throw "unknown type";
							break;
					}
				} else if (t1.tl == LEX_DOUBLE) {
					switch (t2.tl) {
						case LEX_DOUBLE:
							args.push(t1.d + t2.d);
							break;
						case LEX_INT:
						case LEX_BOOL:
							args.push((double) t1.d + t2.i);
							break;
						case LEX_STR:
							sprintf(s, "%lf", t1.d);
							args.push(strcat(s, t2.s));
							break;
						default:
							break;
					}
				} else if (t1.tl == LEX_BOOL) {
					switch (t2.tl) {
						case LEX_STR:
							t2.i ? strcpy(s, "true") : strcpy(s, "false");
							args.push(strcat(s, t2.s));
							break;
						case LEX_BOOL:
							if (!t2.i && !t1.i)
								args.push(false);
							else
								args.push(true);
							break;
						case LEX_DOUBLE:
							args.push(t1.i + t2.d);
							break;
						case LEX_INT:
							args.push(t1.i + t2.i);
							break;
						default:
							throw "unknown type";
							break;
					}
				}
				delete [] s;
				break;
			}
			// MINUS -
			case LEX_MINUS: {
				t2 = args.pop();
				t1 = args.pop();
				if (t1.tl == LEX_INT) {
					switch (t2.tl) {
						case LEX_INT:
						case LEX_BOOL:
							args.push((int) t1.i - t2.i);
							break;
						case LEX_DOUBLE:
							args.push((double) t1.i - t2.d);
							break;
						default:
							throw "unknown or wrong type";
							break;
					}
				} else if (t1.tl == LEX_DOUBLE) {
					switch (t2.tl) {
						case LEX_DOUBLE:
							args.push(t1.d - t2.d);
							break;
						case LEX_INT:
						case LEX_BOOL:
							args.push(t1.d - t2.i);
							break;
						default:
							break;
					}
				} else if (t1.tl == LEX_BOOL) {
					switch (t2.tl) {
						case LEX_BOOL:
							args.push((bool) t1.i - t2.i);
							break;
						case LEX_DOUBLE:
							args.push(t1.i - t2.d);
							break;
						case LEX_INT:
							args.push(t1.i - t2.i);
							break;
						default:
							throw "unknown type";
							break;
					}
				} else
					throw "unknown or wrong type";
				break;
			}
			// MULT *
			case LEX_MULT: {
				t2 = args.pop();
				t1 = args.pop();
				if (t1.tl == LEX_INT) {
					switch (t2.tl) {
						case LEX_INT:
						case LEX_BOOL:
							args.push(t1.i * t2.i);
							break;
						case LEX_DOUBLE:
							args.push((double) t1.i * t2.d);
							break;
						default:
							throw "unknown type";
							break;
					}
				} else if (t1.tl == LEX_DOUBLE) {
					switch (t2.tl) {
						case LEX_DOUBLE:
							args.push(t1.d * t2.d);
							break;
						case LEX_INT:
						case LEX_BOOL:
							args.push((double) t1.d * t2.i);
							break;
						default:
							break;
					}
				} else if (t1.tl == LEX_BOOL) {
					switch (t2.tl) {
						case LEX_BOOL:
							if (t2.i && t1.i)
								args.push(true);
							else
								args.push(false);
							break;
						case LEX_DOUBLE:
							args.push(t1.i * t2.d);
							break;
						case LEX_INT:
							args.push(t1.i * t2.i);
							break;
						default:
							throw "unknown type";
							break;
					}
				} else
					throw "unknown or wrong type";
				break;
			}
			// DIV /
			case LEX_DIV: {
				t2 = args.pop();
				t1 = args.pop();
				if (t1.tl == LEX_INT) {
					switch (t2.tl) {
						case LEX_INT:
						case LEX_BOOL:
							if (!t2.i)
								throw "Div by 0";
							args.push((int) t1.i / t2.i);
							break;
						case LEX_DOUBLE:
							if (!t2.d)
								throw "Div by 0";
							args.push((double) t1.i / t2.d);
							break;
						default:
							throw "unknown or wrong type";
							break;
					}
				} else if (t1.tl == LEX_DOUBLE) {
					switch (t2.tl) {
						case LEX_DOUBLE:
							if (!t2.d)
								throw "Div by 0";
							args.push((double) t1.d / t2.d);
							break;
						case LEX_INT:
						case LEX_BOOL:
							if (!t2.i)
								throw "Div by 0";
							args.push((double) t1.d / t2.i);
							break;
						default:
							break;
					}
				} else if (t1.tl == LEX_BOOL) {
					switch (t2.tl) {
						case LEX_BOOL:
						case LEX_INT:
							if (!t2.i)
								throw "Div by 0";
							if (t1.i)
								args.push(true);
							else
								args.push(false);
							break;
						case LEX_DOUBLE:
							if (!t2.d)
								throw "Div by 0";
							args.push(t1.i / t2.d);
							break;
						default:
							throw "unknown type";
							break;
					}
				} else
					throw "unknown or wrong type";
				break;
			}
			// MOD %
			case LEX_MOD: {
				t2 = args.pop();
				t1 = args.pop();
				if (t1.tl == LEX_INT) {
					switch (t2.tl) {
						case LEX_INT:
						case LEX_BOOL:
							if (!t2.i)
								throw "Div by 0";
							args.push((int) t1.i % t2.i);
							break;
						default:
							throw "unknown or wrong type";
							break;
					}
				} else if (t1.tl == LEX_BOOL) {
					switch (t2.tl) {
						case LEX_BOOL:
						case LEX_INT:
							if (!t2.i)
								throw "Div by 0";
//							args.push((bool) t1.i % t2.i);
							if (t1.i && t2.i > 1)
								args.push(true);
							else
								args.push(false);
							break;
						default:
							throw "unknown type";
							break;
					}
				} else
					throw "unknown or wrong type";
				break;
			}
			// ASSIGN =
			case LEX_ASSIGN:
				t2 = args.pop();
				j = args.pop().i;
				TID[j].put_type(t2.tl);
				if (t2.tl == LEX_STR) {
					TID[j].put_svalue(t2.s);
				} else if (t2.tl == LEX_INT) {
					TID[j].put_value(t2.i);
				} else if (t2.tl == LEX_BOOL) {
					TID[j].put_value(t2.i);
				} else if (t2.tl == LEX_DOUBLE) {
					TID[j].put_dvalue(t2.d);
				}
				break;
			case LEX_RET:
				index = jump;
				break;
			case POLIZ_GO:
				index = args.pop().i - 1;
				break;
			case POLIZ_FGO:
				i = args.pop().i;
				if (!args.pop().i)
					index = i-1;
				break;
			case LEX_NOT:
				t1 = args.pop();
				if (t1.tl == LEX_INT || t1.tl == LEX_BOOL)
					args.push(!t1.i);
				else
					throw "Op. not(!) is unable to this type";
				break;
			case LEX_OR:
				t2 = args.pop();
				t1 = args.pop();
				if (t1.tl == LEX_INT || t1.tl == LEX_BOOL)
					args.push(t1.i || t2.i);
				else
					throw "Op. or(||) is unable to this type";
				break;
			case LEX_AND:
				t2 = args.pop();
				t1 = args.pop();
				if (t1.tl == LEX_INT || t1.tl == LEX_BOOL)
					args.push(t1.i && t2.i);
				else
					throw "Op. and(&&) is unable to this type";
				break;
			// Equal ==
			case LEX_EQ: {
				t2 = args.pop();
				t1 = args.pop();
				char *s = new char [128];
				if (t1.tl == LEX_INT) {
					switch (t2.tl) {
						case LEX_INT:
						case LEX_BOOL:
							args.push(t1.i == t2.i);
							break;
						case LEX_DOUBLE:
							args.push(((double) t1.i) == t2.d);
							break;
						case LEX_STR:
							sprintf(s, "%d", t1.i);
							args.push(strcmp(s, t2.s) == 0);
							break;
						default:
							throw "unknown type";
							break;
					}
				} else if (t1.tl == LEX_STR) {
					switch (t2.tl) {
						case LEX_STR:
							args.push(strcmp(t1.s, t2.s) == 0);
							break;
						case LEX_BOOL:
							if (t2.i)
								sprintf(s, "true");
							else
								sprintf(s, "false");
							args.push(strcmp(t1.s, s) == 0);
							break;
						case LEX_DOUBLE:
							sprintf(s, "%lf", t2.d);
							args.push(strcmp(t1.s, s) == 0);
							break;
						case LEX_INT:
							sprintf(s, "%d", t2.i);
							args.push(strcmp(t1.s, s) == 0);
							break;
						default:
							throw "unknown type";
							break;
					}
				} else if (t1.tl == LEX_DOUBLE) {
					switch (t2.tl) {
						case LEX_DOUBLE:
							args.push(t1.d == t2.d);
							break;
						case LEX_INT:
						case LEX_BOOL:
							args.push(t1.d == ((double)  t2.i));
							break;
						case LEX_STR:
							sprintf(s, "%lf", t1.d);
							args.push(strcmp(s, t2.s) == 0);
							break;
						default:
							break;
					}
				} else if (t1.tl == LEX_BOOL) {
					switch (t2.tl) {
						case LEX_STR:
							t2.i ? strcpy(s, "true") : strcpy(s, "false");
							args.push(strcmp(s, t2.s) == 0);
							break;
						case LEX_BOOL:
							args.push(t1.i == t2.i);
							break;
						case LEX_DOUBLE:
							args.push(((double)t1.i) == t2.d);
							break;
						case LEX_INT:
							args.push(t1.i == t2.i);
							break;
						default:
							throw "unknown type";
							break;
					}
				}
				delete [] s;
				break;
			}
			// Less <
			case LEX_LSS: {
				t2 = args.pop();
				t1 = args.pop();
				char *s = new char [128];
				if (t1.tl == LEX_INT) {
					switch (t2.tl) {
						case LEX_INT:
						case LEX_BOOL:
							args.push(t1.i < t2.i);
							break;
						case LEX_DOUBLE:
							args.push(((double) t1.i) < t2.d);
							break;
						case LEX_STR:
							sprintf(s, "%d", t1.i);
							args.push(strcmp(s, t2.s) < 0);
							break;
						default:
							throw "unknown type";
							break;
					}
				} else if (t1.tl == LEX_STR) {
					switch (t2.tl) {
						case LEX_STR:
							args.push(strcmp(t1.s, t2.s) < 0);
							break;
						case LEX_BOOL:
							if (t2.i)
								sprintf(s, "true");
							else
								sprintf(s, "false");
							args.push(strcmp(t1.s, s) < 0);
							break;
						case LEX_DOUBLE:
							sprintf(s, "%lf", t2.d);
							args.push(strcmp(t1.s, s) < 0);
							break;
						case LEX_INT:
							sprintf(s, "%d", t2.i);
							args.push(strcmp(t1.s, s) < 0);
							break;
						default:
							throw "unknown type";
							break;
					}
				} else if (t1.tl == LEX_DOUBLE) {
					switch (t2.tl) {
						case LEX_DOUBLE:
							args.push(t1.d < t2.d);
							break;
						case LEX_INT:
						case LEX_BOOL:
							args.push(t1.d < ((double)  t2.i));
							break;
						case LEX_STR:
							sprintf(s, "%lf", t1.d);
							args.push(strcmp(s, t2.s) < 0);
							break;
						default:
							break;
					}
				} else if (t1.tl == LEX_BOOL) {
					switch (t2.tl) {
						case LEX_STR:
							t2.i ? strcpy(s, "true") : strcpy(s, "false");
							args.push(strcmp(s, t2.s) < 0);
							break;
						case LEX_BOOL:
							args.push(t1.i < t2.i);
							break;
						case LEX_DOUBLE:
							args.push(((double)t1.i) < t2.d);
							break;
						case LEX_INT:
							args.push(t1.i < t2.i);
							break;
						default:
							throw "unknown type";
							break;
					}
				}
				delete [] s;
				break;
			}
			// greater >
			case LEX_GTR: {
				t2 = args.pop();
				t1 = args.pop();
				char *s = new char [128];
				if (t1.tl == LEX_INT) {
					switch (t2.tl) {
						case LEX_INT:
						case LEX_BOOL:
							args.push(t1.i > t2.i);
							break;
						case LEX_DOUBLE:
							args.push(((double) t1.i) > t2.d);
							break;
						case LEX_STR:
							sprintf(s, "%d", t1.i);
							args.push(strcmp(s, t2.s) > 0);
							break;
						default:
							throw "unknown type";
							break;
					}
				} else if (t1.tl == LEX_STR) {
					switch (t2.tl) {
						case LEX_STR:
							args.push(strcmp(t1.s, t2.s) > 0);
							break;
						case LEX_BOOL:
							if (t2.i)
								sprintf(s, "true");
							else
								sprintf(s, "false");
							args.push(strcmp(t1.s, s) > 0);
							break;
						case LEX_DOUBLE:
							sprintf(s, "%lf", t2.d);
							args.push(strcmp(t1.s, s) > 0);
							break;
						case LEX_INT:
							sprintf(s, "%d", t2.i);
							args.push(strcmp(t1.s, s) > 0);
							break;
						default:
							throw "unknown type";
							break;
					}
				} else if (t1.tl == LEX_DOUBLE) {
					switch (t2.tl) {
						case LEX_DOUBLE:
							args.push(t1.d > t2.d);
							break;
						case LEX_INT:
						case LEX_BOOL:
							args.push(t1.d > ((double)  t2.i));
							break;
						case LEX_STR:
							sprintf(s, "%lf", t1.d);
							args.push(strcmp(s, t2.s) > 0);
							break;
						default:
							break;
					}
				} else if (t1.tl == LEX_BOOL) {
					switch (t2.tl) {
						case LEX_STR:
							t2.i ? strcpy(s, "true") : strcpy(s, "false");
							args.push(strcmp(s, t2.s) > 0);
							break;
						case LEX_BOOL:
							args.push(t1.i > t2.i);
							break;
						case LEX_DOUBLE:
							args.push(((double)t1.i) > t2.d);
							break;
						case LEX_INT:
							args.push(t1.i > t2.i);
							break;
						default:
							throw "unknown type";
							break;
					}
				}
				delete [] s;
				break;
			}
			// Absolutely Equal ===
			case LEX_ABEQ: {
				t2 = args.pop();
				t1 = args.pop();
				if (t1.tl == LEX_INT) {
					switch (t2.tl) {
						case LEX_INT:
							args.push(t1.i == t2.i);
							break;
						case LEX_DOUBLE:
						case LEX_BOOL:
						case LEX_STR:
							args.push(false);
							break;
						default:
							throw "unknown type";
							break;
					}
				} else if (t1.tl == LEX_STR) {
					switch (t2.tl) {
						case LEX_STR:
							args.push(strcmp(t1.s, t2.s) == 0);
							break;
						case LEX_BOOL:
						case LEX_DOUBLE:
						case LEX_INT:
							args.push(false);
							break;
						default:
							throw "unknown type";
							break;
					}
				} else if (t1.tl == LEX_DOUBLE) {
					switch (t2.tl) {
						case LEX_DOUBLE:
							args.push(t1.d == t2.d);
							break;
						case LEX_INT:
						case LEX_BOOL:
						case LEX_STR:
							args.push(false);
							break;
						default:
							break;
					}
				} else if (t1.tl == LEX_BOOL) {
					switch (t2.tl) {
						case LEX_BOOL:
							args.push(t1.i == t2.i);
							break;
						case LEX_DOUBLE:
						case LEX_STR:
						case LEX_INT:
							args.push(false);
							break;
						default:
							throw "unknown type";
							break;
					}
				}
				break;
			}
			default:
				break;
		}//end of switch
		++index;
	};//end of while
//	cout << endl << "Finish of executing!!!" << endl;
}