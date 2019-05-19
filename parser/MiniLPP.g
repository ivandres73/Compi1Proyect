grammar MiniLPP;

program:
    var_section? subprogram_decl* 'inicio' statements+ 'fin' .
    ;

var_section:
    var_decl var_section
    |/*eps */
    ;

subprogram_decl:

    ;

statements:

    ;

var_decl:
    type 'Iden' more_var
    |/*eps */
    ;

more_var:
    ',' 'Iden'
    |/*eps */
    ;

type:
    'entero'
    |'real'
    |'cadena'
    |'booleano'
    |'caracter'
    ;