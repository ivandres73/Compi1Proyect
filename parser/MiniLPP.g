grammar MiniLPP;

 program:
    subtypes_section var_section subprogram_decl 'inicio' statements 'fin'
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
    subprogram_header 'Endline' var_section 'inicio' statements 'Endline' 'fin' 'Endline' subprogram_decl
    |/*eps */
    ;

subprogram_header:
    function_header
    |procedure_header
    ;

function_header:
    'funcion' 'Iden' argument_list 'colon' type
    ;

procedure_header:
    'procedimiento' 'Iden' argument_list
    ;

argument_list:
    'OpenParens' argument_decl 'CloseParens'
    |/*eps */
    ;

argument_decl:
    type 'Iden' more_args
    |'var' type 'Iden' more_args
    |/*eps */
    ;

more_args:
    'comma' more_args_p
    |/*eps */
    ;

more_args_p:
    type 'Iden' more_args
    |'var' type 'Iden' more_args
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