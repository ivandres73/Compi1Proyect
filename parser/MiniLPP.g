grammar MiniLPP;

 program:
    subtypes_section var_section subprogram_decl* 'inicio' statements+ 'fin'
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

    ;

statements:

    ;

var_decl:
    type 'Iden' more_var
    ;

more_var:
    ',' 'Iden' more_var
    |/*eps */
    ;

type:
    'entero'
    |'real'
    |'cadena'
    |'booleano'
    |'caracter'
    ;