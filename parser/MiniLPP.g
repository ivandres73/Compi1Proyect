grammar MiniLPP;

 program:
    subtypes_section var_section subprogram_decl 'inicio' statements+ 'fin'
    ;

subtypes_section:
    subtype_decl subtypes_section
    |/*eps */
    ;

subtype_decl:
    |'tipo' 'Iden' 'es' type 'Endline'
    ;

var_section:
    var_decl var_section
    |/*eps */
    ;

subprogram_decl:
    subprogram_header 'Endline' var_section 'inicio' /*seguiria statement */
    |/*eps */
    ;

subprogram_header:

    ;

statements:

    ;

var_decl:
    type 'Iden' more_var 'Endline'
    ;

more_var:
    'comma' 'Iden' more_var
    |/*eps */
    ;

type:
    'entero'
    |'real'
    |'cadena'
    |'booleano'
    |'caracter'
    ;