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
    subprogram_header 'Endline' var_section 'inicio' 'Endline' statements 'fin' 'Endline' subprogram_decl
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
    |'llamar' 'Iden' args_call more_statements
    |'escriba' string_args more_statements
    |'lea' lvalue more_statements
    |if_statement more_statements
    ;

if_statement:
    'si' expr 'EndLine' 'entonces' 'EndLine' if_stmt
    ;

if_stmt:
    lvalue 'assign' expr more_if_stmt
    |'llamar' 'Iden' args_call more_if_stmt
    |'escriba' string_args more_if_stmt
    |'lea' lvalue more_if_stmt
    |if_statement more_if_stmt
    ;

more_if_stmt:
    'EndLine' more_if_stmt_p
    ;

more_if_stmt_p:
    lvalue 'assign' expr more_if_stmt
    |'llamar' 'Iden' args_call more_if_stmt
    |'escriba' string_args more_if_stmt
    |'lea' lvalue more_if_stmt
    |if_statement more_if_stmt
    |'fin' 'si'
    ;

string_args:
    'StringConst' more_string_args
    |expr more_string_args
    ;

more_string_args:
    'comma' string_args
    |/*eps */
    ;

more_statements:
    'EndLine' statements
    |/*eps */
    ;

lvalue:
    'Iden' lvalue_p
    ;

lvalue_p:
    'OpenBra' expr 'CloseBra'//acepta ID[verdadero] ??
    |/*eps */
    ;

rvalue:
    'Iden' rvalue_p
    ;

rvalue_p:
    'OpenBra' expr 'CloseBra'
    |args_call
    |/*eps */
    ;

args_call:
    'OpenParens' arg_decl 'CloseParens'
    |/*epsilon */
    ;

arg_decl:
    expr more_arg_decl
    |/*eps */
    ;

more_arg_decl:
    'comma' arg_decl
    |/*eps */
    ;

expr:
    rvalue
    |constant
    //|expr bin_op expr
    |'sub' expr //conflicto con los firsts(bin_op)
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
    //|'StringConst' conflicto con more_strings_args_p
    |bool_const
    ;

bool_const:
    'verdadero'
    |'falso'
    ;