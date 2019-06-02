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
    statement
    |/*eps */
    ;

statement:
    lvalue 'assign' expr more_statements
    ;

more_statements:
    'EndLine' more_statements_p
    |/*eps */
    ;

more_statements_p:
    lvalue 'assign' expr more_statements
    ;

lvalue:
    'Iden' lvalue_p
    ;

lvalue_p:
    'OpenBra' expr 'CloseBra'//acepta ID[verdadero] ??
    |/*eps */
    ;

expr:
    lvalue
    |constant
    //|expr bin_op expr
    |'sub' expr //va a haber conflicto con los firsts(bin_op)
    |'no' expr
    |'openParens' expr 'closeParens'
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
    |array_type
    ;

array_type:
    'arreglo' 'openBra' 'intConst' 'closeBra' 'de' type
    ;

constant:
    'intConst'
    |'charConst'
    |'StringConst'
    |bool_const
    ;

bool_const:
    'verdadero'
    |'falso'
    ;