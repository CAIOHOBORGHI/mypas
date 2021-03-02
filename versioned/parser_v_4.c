/**@<parser.c>::**/

/*
 * LR(1)
 *
 * Production rules||  Semantic rules
 * ----------------||-----------------------------
 * E ->  E¹ (+) T  ||  E.t <- E¹.t // T.t // (+).t
 *     | T         ||  E.t <- T.t
 * T ->  T¹ (x) F  ||  T.t <- T¹.t // F.t // (x).t
 *     | F         ||  T.t <- F.t
 * F -> ( E )      ||  F.t <- E.t
 *     |  n        ||  F.t <- n.t
 *     |  v        ||  F.t <- v.t
 *
 * Semantic-actions grammar
 * semantic actions: (1) -> [ print (+).t ]
 * 	 	     (1a)-> [ store (+).t ]
 *                   (2) -> [ print (*).t ]
 * 	 	     (1a)-> [ store (*).t ]
 *                   (3) -> [ print n.t ]
 *                   (3a)-> [ store n.t ]
 *                   (4) -> [ print v.t ]
 *                   (4a)-> [ store v.t ]
 *
 *  E -> E (1a) (+) T (1)  |  T
 *  T -> T (2a) (*) F (2)  |  F
 *  F -> ( E ) | (3a) n  | (4a) v
 *
 *  LL(1)
 *
 *  E -> T { (1a) (+) T (1) }
 *  T -> F { (2a) (*) F (2) }
 * F -> ( E )
 *     |  (3a) n
 *     |  (4a) v
 *
 */

#include <parser.h>

void mybc(void)
{
	E(); match(EOF); /**/printf("\n");/**/
}
extern char lexeme[];
void E(void) 
{
	/**/int oplus = 0, otimes = 0, signal = 0;/**/
	if ( lookahead == '+' || lookahead == '-' ) {
		signal = lookahead;
		match (lookahead);
	}

	_T:

	_F:

	switch (lookahead) {
		case '(':
			match('('); E(); match(')');
			break;
		case UINT:
		case FLOAT:
		case HEX:
		case OCT:
			match(lookahead);
			break;
		default:
			/**/printf("%s ", lexeme);/**/
			match(ID);
	}

	/**/if(otimes){ printf("%c ", otimes); otimes = 0;}/**/
	if ( lookahead == '*' || lookahead == '/' ) {
		/**/otimes = lookahead;/**/
		match (lookahead); goto _F;
	}
	/**/if(signal){ if(signal == '-'){printf("negate ");} signal = 0;}/**/

	/**/if(oplus){ printf("%c ", oplus); oplus = 0;}/**/
	if ( lookahead == '+' || lookahead == '-' ) { 
		/**/oplus = lookahead;/**/
		match (lookahead); goto _T;
	}

}

void match(int expected)
{
	if (lookahead == expected) {
		lookahead = gettoken(source);
	} else {
		fprintf(stderr, "token mismatch: expected %d whereas found %d\n",
		expected, lookahead);
		exit(-2);
	}
}
