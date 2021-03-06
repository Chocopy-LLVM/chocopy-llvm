
//----------------------------------------------------
// The following code was generated by CUP v0.11b 20160615 (GIT 4ac7450)
//----------------------------------------------------

package chocopy.pa1;

import java.util.ArrayList;
import java.util.List;
import java_cup.runtime.*;
import chocopy.common.astnodes.*;
import java_cup.runtime.ComplexSymbolFactory.Location;
import java_cup.runtime.XMLElement;

/** CUP v0.11b 20160615 (GIT 4ac7450) generated parser.
  */
@SuppressWarnings({"rawtypes"})
public class ChocoPyParser extends java_cup.runtime.lr_parser {

 public final Class getSymbolContainer() {
    return ChocoPyTokens.class;
}

  /** Default constructor. */
  @Deprecated
  public ChocoPyParser() {super();}

  /** Constructor which sets the default scanner. */
  @Deprecated
  public ChocoPyParser(java_cup.runtime.Scanner s) {super(s);}

  /** Constructor which sets the default scanner. */
  public ChocoPyParser(java_cup.runtime.Scanner s, java_cup.runtime.SymbolFactory sf) {super(s,sf);}

  /** Production table. */
  protected static final short _production_table[][] = 
    unpackFromStrings(new String[] {
    "\000\015\000\002\002\004\000\002\002\004\000\002\003" +
    "\002\000\002\005\002\000\002\005\003\000\002\004\003" +
    "\000\002\004\004\000\002\004\004\000\002\006\004\000" +
    "\002\007\003\000\002\010\003\000\002\010\003\000\002" +
    "\011\005" });

  /** Access to production table. */
  public short[][] production_table() {return _production_table;}

  /** Parse-action table. */
  protected static final short[][] _action_table = 
    unpackFromStrings(new String[] {
    "\000\020\000\006\002\uffff\006\uffff\001\002\000\004\002" +
    "\022\001\002\000\006\002\ufffe\006\007\001\002\000\006" +
    "\004\ufff7\005\ufff7\001\002\000\006\004\ufff6\005\ufff6\001" +
    "\002\000\010\002\ufffc\003\ufffc\006\ufffc\001\002\000\006" +
    "\004\ufff8\005\020\001\002\000\004\004\017\001\002\000" +
    "\004\002\000\001\002\000\010\002\ufffd\003\015\006\007" +
    "\001\002\000\010\002\ufffa\003\ufffa\006\ufffa\001\002\000" +
    "\010\002\ufffb\003\ufffb\006\ufffb\001\002\000\010\002\ufff9" +
    "\003\ufff9\006\ufff9\001\002\000\004\006\007\001\002\000" +
    "\006\004\ufff5\005\ufff5\001\002\000\004\002\001\001\002" +
    "" });

  /** Access to parse-action table. */
  public short[][] action_table() {return _action_table;}

  /** <code>reduce_goto</code> table. */
  protected static final short[][] _reduce_table = 
    unpackFromStrings(new String[] {
    "\000\020\000\006\002\003\003\004\001\001\000\002\001" +
    "\001\000\016\004\013\005\012\006\007\007\011\010\010" +
    "\011\005\001\001\000\002\001\001\000\002\001\001\000" +
    "\002\001\001\000\002\001\001\000\002\001\001\000\002" +
    "\001\001\000\012\006\015\007\011\010\010\011\005\001" +
    "\001\000\002\001\001\000\002\001\001\000\002\001\001" +
    "\000\006\010\020\011\005\001\001\000\002\001\001\000" +
    "\002\001\001" });

  /** Access to <code>reduce_goto</code> table. */
  public short[][] reduce_table() {return _reduce_table;}

  /** Instance of action encapsulation class. */
  protected CUP$ChocoPyParser$actions action_obj;

  /** Action encapsulation object initializer. */
  protected void init_actions()
    {
      action_obj = new CUP$ChocoPyParser$actions(this);
    }

  /** Invoke a user supplied parse action. */
  public java_cup.runtime.Symbol do_action(
    int                        act_num,
    java_cup.runtime.lr_parser parser,
    java.util.Stack            stack,
    int                        top)
    throws java.lang.Exception
  {
    /* call code in generated class */
    return action_obj.CUP$ChocoPyParser$do_action(act_num, parser, stack, top);
  }

  /** Indicates start state. */
  public int start_state() {return 0;}
  /** Indicates start production. */
  public int start_production() {return 0;}

  /** <code>EOF</code> Symbol index. */
  public int EOF_sym() {return 0;}

  /** <code>error</code> Symbol index. */
  public int error_sym() {return 1;}




    /* The following fields and methods deal with error reporting
     * Avoid changing these unless you know what you are doing. */

    /** Node that accumulates error messages to be added to the Program
     *  node produced as a result. */
    public final Errors errors = new Errors(new ArrayList<>());

    /** Return the Program node that results from parsing the stream of
     *  tokens produced by lexical analysis.  In the case of syntax errors,
     *  the program may be empty, but will have error messages. */
    public Program parseProgram(boolean debug) {
        try {
            Symbol result = debug ? debug_parse() : parse();
            if (result == null || !(result.value instanceof Program)) {
                return new Program(new Location(0, 0), new Location(0, 0),
                                   new ArrayList<Declaration>(),
                                   new ArrayList<Stmt>(),
                                   errors);
            } else {
                return (Program) result.value;
            }
        } catch (RuntimeException excp) {
            throw excp;
        } catch (Exception excp) {
            String msg =
                String.format("Internal parser error detected: %s%n", excp);
            throw new AssertionError(msg);
        }
    }

    @Override
    public SymbolFactory getSymbolFactory() {
        return ((ChocoPyLexer) getScanner()).symbolFactory;
    }

    @Override
    public void syntax_error(Symbol cur_token) {
                String token = symbl_name_from_id(cur_token.sym);
                String text = ((ChocoPyLexer) getScanner()).yytext();
                errors.syntaxError(
                    ((ComplexSymbolFactory.ComplexSymbol) cur_token).xleft,
                    ((ComplexSymbolFactory.ComplexSymbol) cur_token).xright,
                    "Parse error near token %s: %s", token, text);
    }

    @Override
    public void unrecovered_syntax_error(Symbol cur_token) {
        /* Do not die */
    }


/** Cup generated class to encapsulate user supplied action code.*/
@SuppressWarnings({"rawtypes", "unchecked", "unused"})
class CUP$ChocoPyParser$actions {



    /** Return a mutable list initially containing the single value ITEM. */
    <T> List<T> single(T item) {
        List<T> list = new ArrayList<>();
        if (item != null) {
            list.add(item);
        }
        return list;
    }

    /** If ITEM is non-null, appends it to the end of LIST.  Then returns
     *  LIST. */
    <T> List<T> combine(List<T> list, T item) {
        if (item != null) {
            list.add(item);
        }
        return list;
    }

    /** Return a mutable empty list. */
    <T> List<T> empty() {
        return new ArrayList<T>();
    }

    /** Return the leftmost non-whitespace location in NODES, or null if NODES
     *  is empty.  Assumes that the nodes of NODES are ordered in increasing
     *  order of location, from left to right. */
    ComplexSymbolFactory.Location getLeft(List<? extends Node> nodes) {
        if (nodes.isEmpty()) {
            return null;
        }
        Node first = nodes.get(0);
        return new ComplexSymbolFactory.Location(first.getLocation()[0],
                                                 first.getLocation()[1]);
    }


  private final ChocoPyParser parser;

  /** Constructor */
  CUP$ChocoPyParser$actions(ChocoPyParser parser) {
    this.parser = parser;
  }

  /** Method 0 with the actual generated action code for actions 0 to 300. */
  public final java_cup.runtime.Symbol CUP$ChocoPyParser$do_action_part00000000(
    int                        CUP$ChocoPyParser$act_num,
    java_cup.runtime.lr_parser CUP$ChocoPyParser$parser,
    java.util.Stack            CUP$ChocoPyParser$stack,
    int                        CUP$ChocoPyParser$top)
    throws java.lang.Exception
    {
      /* Symbol object for return from actions */
      java_cup.runtime.Symbol CUP$ChocoPyParser$result;

      /* select the action based on the action number */
      switch (CUP$ChocoPyParser$act_num)
        {
          /*. . . . . . . . . . . . . . . . . . . .*/
          case 0: // $START ::= program EOF 
            {
              Object RESULT =null;
		Location start_valxleft = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.elementAt(CUP$ChocoPyParser$top-1)).xleft;
		Location start_valxright = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.elementAt(CUP$ChocoPyParser$top-1)).xright;
		Program start_val = (Program)((java_cup.runtime.Symbol) CUP$ChocoPyParser$stack.elementAt(CUP$ChocoPyParser$top-1)).value;
		RESULT = start_val;
              CUP$ChocoPyParser$result = parser.getSymbolFactory().newSymbol("$START",0, ((java_cup.runtime.Symbol)CUP$ChocoPyParser$stack.elementAt(CUP$ChocoPyParser$top-1)), ((java_cup.runtime.Symbol)CUP$ChocoPyParser$stack.peek()), RESULT);
            }
          /* ACCEPT */
          CUP$ChocoPyParser$parser.done_parsing();
          return CUP$ChocoPyParser$result;

          /*. . . . . . . . . . . . . . . . . . . .*/
          case 1: // program ::= program_head opt_stmt_list 
            {
              Program RESULT =null;
		Location dxleft = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.elementAt(CUP$ChocoPyParser$top-1)).xleft;
		Location dxright = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.elementAt(CUP$ChocoPyParser$top-1)).xright;
		List<Declaration> d = (List<Declaration>)((java_cup.runtime.Symbol) CUP$ChocoPyParser$stack.elementAt(CUP$ChocoPyParser$top-1)).value;
		Location sxleft = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.peek()).xleft;
		Location sxright = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.peek()).xright;
		List<Stmt> s = (List<Stmt>)((java_cup.runtime.Symbol) CUP$ChocoPyParser$stack.peek()).value;
		 RESULT = new Program(d.isEmpty() ? getLeft(s) : getLeft(d),
                                sxright, d, s, errors);
        
              CUP$ChocoPyParser$result = parser.getSymbolFactory().newSymbol("program",0, ((java_cup.runtime.Symbol)CUP$ChocoPyParser$stack.elementAt(CUP$ChocoPyParser$top-1)), ((java_cup.runtime.Symbol)CUP$ChocoPyParser$stack.peek()), RESULT);
            }
          return CUP$ChocoPyParser$result;

          /*. . . . . . . . . . . . . . . . . . . .*/
          case 2: // program_head ::= 
            {
              List<Declaration> RESULT =null;
		 RESULT = empty(); 
              CUP$ChocoPyParser$result = parser.getSymbolFactory().newSymbol("program_head",1, ((java_cup.runtime.Symbol)CUP$ChocoPyParser$stack.peek()), RESULT);
            }
          return CUP$ChocoPyParser$result;

          /*. . . . . . . . . . . . . . . . . . . .*/
          case 3: // opt_stmt_list ::= 
            {
              List<Stmt> RESULT =null;
		 RESULT = empty(); 
              CUP$ChocoPyParser$result = parser.getSymbolFactory().newSymbol("opt_stmt_list",3, ((java_cup.runtime.Symbol)CUP$ChocoPyParser$stack.peek()), RESULT);
            }
          return CUP$ChocoPyParser$result;

          /*. . . . . . . . . . . . . . . . . . . .*/
          case 4: // opt_stmt_list ::= stmt_list 
            {
              List<Stmt> RESULT =null;
		Location sxleft = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.peek()).xleft;
		Location sxright = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.peek()).xright;
		List<Stmt> s = (List<Stmt>)((java_cup.runtime.Symbol) CUP$ChocoPyParser$stack.peek()).value;
		 RESULT = s; 
              CUP$ChocoPyParser$result = parser.getSymbolFactory().newSymbol("opt_stmt_list",3, ((java_cup.runtime.Symbol)CUP$ChocoPyParser$stack.peek()), ((java_cup.runtime.Symbol)CUP$ChocoPyParser$stack.peek()), RESULT);
            }
          return CUP$ChocoPyParser$result;

          /*. . . . . . . . . . . . . . . . . . . .*/
          case 5: // stmt_list ::= stmt 
            {
              List<Stmt> RESULT =null;
		Location sxleft = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.peek()).xleft;
		Location sxright = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.peek()).xright;
		Stmt s = (Stmt)((java_cup.runtime.Symbol) CUP$ChocoPyParser$stack.peek()).value;
		 RESULT = single(s); 
              CUP$ChocoPyParser$result = parser.getSymbolFactory().newSymbol("stmt_list",2, ((java_cup.runtime.Symbol)CUP$ChocoPyParser$stack.peek()), ((java_cup.runtime.Symbol)CUP$ChocoPyParser$stack.peek()), RESULT);
            }
          return CUP$ChocoPyParser$result;

          /*. . . . . . . . . . . . . . . . . . . .*/
          case 6: // stmt_list ::= stmt_list stmt 
            {
              List<Stmt> RESULT =null;
		Location lxleft = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.elementAt(CUP$ChocoPyParser$top-1)).xleft;
		Location lxright = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.elementAt(CUP$ChocoPyParser$top-1)).xright;
		List<Stmt> l = (List<Stmt>)((java_cup.runtime.Symbol) CUP$ChocoPyParser$stack.elementAt(CUP$ChocoPyParser$top-1)).value;
		Location sxleft = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.peek()).xleft;
		Location sxright = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.peek()).xright;
		Stmt s = (Stmt)((java_cup.runtime.Symbol) CUP$ChocoPyParser$stack.peek()).value;
		 RESULT = combine(l, s); 
              CUP$ChocoPyParser$result = parser.getSymbolFactory().newSymbol("stmt_list",2, ((java_cup.runtime.Symbol)CUP$ChocoPyParser$stack.elementAt(CUP$ChocoPyParser$top-1)), ((java_cup.runtime.Symbol)CUP$ChocoPyParser$stack.peek()), RESULT);
            }
          return CUP$ChocoPyParser$result;

          /*. . . . . . . . . . . . . . . . . . . .*/
          case 7: // stmt_list ::= stmt_list error 
            {
              List<Stmt> RESULT =null;
		Location lxleft = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.elementAt(CUP$ChocoPyParser$top-1)).xleft;
		Location lxright = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.elementAt(CUP$ChocoPyParser$top-1)).xright;
		List<Stmt> l = (List<Stmt>)((java_cup.runtime.Symbol) CUP$ChocoPyParser$stack.elementAt(CUP$ChocoPyParser$top-1)).value;
		 RESULT = l; 
              CUP$ChocoPyParser$result = parser.getSymbolFactory().newSymbol("stmt_list",2, ((java_cup.runtime.Symbol)CUP$ChocoPyParser$stack.elementAt(CUP$ChocoPyParser$top-1)), ((java_cup.runtime.Symbol)CUP$ChocoPyParser$stack.peek()), RESULT);
            }
          return CUP$ChocoPyParser$result;

          /*. . . . . . . . . . . . . . . . . . . .*/
          case 8: // stmt ::= expr_stmt NEWLINE 
            {
              Stmt RESULT =null;
		Location sxleft = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.elementAt(CUP$ChocoPyParser$top-1)).xleft;
		Location sxright = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.elementAt(CUP$ChocoPyParser$top-1)).xright;
		Stmt s = (Stmt)((java_cup.runtime.Symbol) CUP$ChocoPyParser$stack.elementAt(CUP$ChocoPyParser$top-1)).value;
		 RESULT = s; 
              CUP$ChocoPyParser$result = parser.getSymbolFactory().newSymbol("stmt",4, ((java_cup.runtime.Symbol)CUP$ChocoPyParser$stack.elementAt(CUP$ChocoPyParser$top-1)), ((java_cup.runtime.Symbol)CUP$ChocoPyParser$stack.peek()), RESULT);
            }
          return CUP$ChocoPyParser$result;

          /*. . . . . . . . . . . . . . . . . . . .*/
          case 9: // expr_stmt ::= expr 
            {
              Stmt RESULT =null;
		Location exleft = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.peek()).xleft;
		Location exright = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.peek()).xright;
		Expr e = (Expr)((java_cup.runtime.Symbol) CUP$ChocoPyParser$stack.peek()).value;
		 RESULT = new ExprStmt(exleft, exright, e); 
              CUP$ChocoPyParser$result = parser.getSymbolFactory().newSymbol("expr_stmt",5, ((java_cup.runtime.Symbol)CUP$ChocoPyParser$stack.peek()), ((java_cup.runtime.Symbol)CUP$ChocoPyParser$stack.peek()), RESULT);
            }
          return CUP$ChocoPyParser$result;

          /*. . . . . . . . . . . . . . . . . . . .*/
          case 10: // expr ::= binary_expr 
            {
              Expr RESULT =null;
		Location exleft = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.peek()).xleft;
		Location exright = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.peek()).xright;
		Expr e = (Expr)((java_cup.runtime.Symbol) CUP$ChocoPyParser$stack.peek()).value;
		 RESULT = e; 
              CUP$ChocoPyParser$result = parser.getSymbolFactory().newSymbol("expr",6, ((java_cup.runtime.Symbol)CUP$ChocoPyParser$stack.peek()), ((java_cup.runtime.Symbol)CUP$ChocoPyParser$stack.peek()), RESULT);
            }
          return CUP$ChocoPyParser$result;

          /*. . . . . . . . . . . . . . . . . . . .*/
          case 11: // expr ::= NUMBER 
            {
              Expr RESULT =null;
		Location nxleft = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.peek()).xleft;
		Location nxright = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.peek()).xright;
		Integer n = (Integer)((java_cup.runtime.Symbol) CUP$ChocoPyParser$stack.peek()).value;
		 RESULT = new IntegerLiteral(nxleft, nxright, n); 
              CUP$ChocoPyParser$result = parser.getSymbolFactory().newSymbol("expr",6, ((java_cup.runtime.Symbol)CUP$ChocoPyParser$stack.peek()), ((java_cup.runtime.Symbol)CUP$ChocoPyParser$stack.peek()), RESULT);
            }
          return CUP$ChocoPyParser$result;

          /*. . . . . . . . . . . . . . . . . . . .*/
          case 12: // binary_expr ::= expr PLUS expr 
            {
              Expr RESULT =null;
		Location e1xleft = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.elementAt(CUP$ChocoPyParser$top-2)).xleft;
		Location e1xright = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.elementAt(CUP$ChocoPyParser$top-2)).xright;
		Expr e1 = (Expr)((java_cup.runtime.Symbol) CUP$ChocoPyParser$stack.elementAt(CUP$ChocoPyParser$top-2)).value;
		Location opxleft = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.elementAt(CUP$ChocoPyParser$top-1)).xleft;
		Location opxright = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.elementAt(CUP$ChocoPyParser$top-1)).xright;
		String op = (String)((java_cup.runtime.Symbol) CUP$ChocoPyParser$stack.elementAt(CUP$ChocoPyParser$top-1)).value;
		Location e2xleft = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.peek()).xleft;
		Location e2xright = ((java_cup.runtime.ComplexSymbolFactory.ComplexSymbol)CUP$ChocoPyParser$stack.peek()).xright;
		Expr e2 = (Expr)((java_cup.runtime.Symbol) CUP$ChocoPyParser$stack.peek()).value;
		 RESULT = new BinaryExpr(e1xleft, e2xright,
                                                   e1, op, e2); 
              CUP$ChocoPyParser$result = parser.getSymbolFactory().newSymbol("binary_expr",7, ((java_cup.runtime.Symbol)CUP$ChocoPyParser$stack.elementAt(CUP$ChocoPyParser$top-2)), ((java_cup.runtime.Symbol)CUP$ChocoPyParser$stack.peek()), RESULT);
            }
          return CUP$ChocoPyParser$result;

          /* . . . . . .*/
          default:
            throw new Exception(
               "Invalid action number "+CUP$ChocoPyParser$act_num+"found in internal parse table");

        }
    } /* end of method */

  /** Method splitting the generated action code into several parts. */
  public final java_cup.runtime.Symbol CUP$ChocoPyParser$do_action(
    int                        CUP$ChocoPyParser$act_num,
    java_cup.runtime.lr_parser CUP$ChocoPyParser$parser,
    java.util.Stack            CUP$ChocoPyParser$stack,
    int                        CUP$ChocoPyParser$top)
    throws java.lang.Exception
    {
              return CUP$ChocoPyParser$do_action_part00000000(
                               CUP$ChocoPyParser$act_num,
                               CUP$ChocoPyParser$parser,
                               CUP$ChocoPyParser$stack,
                               CUP$ChocoPyParser$top);
    }
}

}
