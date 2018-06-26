#ifndef ast_specifier_qualifier_hpp
#define ast_specifier_qualifier_hpp

#include "ast_node.hpp"

class specifier_store_class : public Node {
    //SPECIFIER_STORE_CLASS : TYPEDEF
    //                      | EXTERN
    //                      | STATIC
    //                      | AUTO
    //                      | REGISTER
    private:
        std::string val;
    public:
        specifier_store_class(std::string _arg1)
            : val(_arg1)
        {}
        
    public:
    
        std::string name = "specifier_store_class";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            dst<<val<<" ";
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            if(val == "extern"){
                // global variable redeclare
                context.tmp_v = "extern";
                context.extern_global = true;
            }
        }
};

class specifier_type : public Node {
    //SPECIFIER_TYPE : VOID
    //               | CHAR
    //               | SHORT
    //               | INT
    //               | LONG
    //               | FLOAT
    //               | DOUBLE
    //               | SIGNED
    //               | UNSIGNED
    //               | SPECIFIER_UNION_OR_STRUCT
    //               | SPECIFIER_ENUM 
    private:
        std::string ter;
    public:
    public:

        specifier_type(std::string _arg1)
            : ter(_arg1)
        {}
        std::string name = "specifier_type";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            dst<<ter<<" ";
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
           // dst<<ter<<" ";
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            context.tmp_v = ter; // setting the tMP_V to the terminal string
        }
};

class specifier_union_or_struct : public Node {
    //SPECIFIER_UNION_OR_STRUCT : UNION_OR_STRUCT IDENTIFIER L_BRACE DECLARATION_LIST_STRUCT R_BRACE
    //                          | UNION_OR_STRUCT L_BRACE DECLAR_LIST_STRUCT R_BRACE
    //                          | UNION_OR_STRUCT IDENTIFIER
    private:
        NodePtr us;
        std::string id;
        NodePtr list;
    public:
        specifier_union_or_struct(NodePtr _arg1, std::string _arg2)
            : us(_arg1)
            , id(_arg2)
            , list(NULL)
        {}

        specifier_union_or_struct(NodePtr _arg1, NodePtr _arg2)
            : us(_arg1)
            , id(" ")
            , list(_arg2)
        {}

        specifier_union_or_struct(NodePtr _arg1, std::string _arg2, NodePtr _arg3)
            : us(_arg1)
            , id(_arg2)
            , list(_arg3)
        {}
        
    public:
    
        std::string name = "specifier_union_or_struct";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            us->PrettyPrint(dst);
            if(id != " ") dst<<id<<" ";
            if(list != NULL){
                dst<<"{ ";
                list->PrettyPrint(dst);
                dst<<" } ";
            }
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class union_or_struct : public Node {
    //UNION_OR_STRUCT : STRUCT
    //                | UNION
    private:
        std::string val;
    public:
        union_or_struct(std::string _arg1)
            : val(_arg1)
        {}
        
    public:
    
        std::string name = "union_or_struct";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            dst<<val<<" ";
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class list_spec_qual : public Node {
    //LIST_SPEC_QUAL : SPECIFIER_TYPE LIST_SPEC_QUAL
    //               | SPECIFIER_TYPE 
    //               | QUALIFIER_TYPE LIST_SPEC_QUAL
    //               | QUALIFIER_TYPE
    private:
        NodePtr sq;
        NodePtr rec;
    public:
        list_spec_qual(NodePtr _arg1, NodePtr _arg2)
            : sq(_arg1)
            , rec(_arg2)
        {}
        
    public:
    
        std::string name = "list_spec_qual";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            sq->PrettyPrint(dst);
            rec->PrettyPrint(dst);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            std::string res;

            sq->compile(dst,context); // store in tmp_v

            res = context.tmp_v;

            rec->compile(dst,context); // stores in tmp_v

            res = res + " " + context.tmp_v;

            context.tmp_v = res; // send back
        }
};

class specifier_enum : public Node {
    // ENUM - keyword
    //SPECIFIER_ENUM : ENUM L_BRACE LIST_ENUMERATOR R_BRACE
    //               | ENUM IDENTIFIER L_BRACE LIST_ENUMERATOR R_BRACE
    //               | ENUM IDENTIFIER
    private:
        NodePtr list;
        std::string id;
    public:
        specifier_enum(NodePtr _arg2) // ENUM L_BRACE LIST_ENUMERATOR R_BRACE
            : list(_arg2)
            , id(" ")
        {}
        specifier_enum(std::string _arg2, NodePtr _arg3) // ENUM IDENTIFIER L_BRACE LIST_ENUMERATOR R_BRACE
            : list(_arg3)
            , id(_arg2)
        {}
        specifier_enum(std::string _arg2) // ENUM IDENTIFIER
            : list(NULL)
            , id(_arg2)
        {}
    public:
    
        std::string name = "specifier_enum";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            dst<<"enum ";
            if(id != " ") dst<<id<<" ";
            if(list != NULL){
                dst<<" { ";
                list->PrettyPrint(dst);
                dst<<" } ";
            }
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class list_enumerator : public Node {
    //LIST_ENUMERATOR : ENUMERATOR
    //                | LIST_ENUMERATOR ',' ENUMERATOR
    private:
        NodePtr en;
        NodePtr rec;
    public:
        list_enumerator(NodePtr _arg1, NodePtr _arg2)
            : en(_arg2)
            , rec(_arg1)
        {}

    public:
    
        std::string name = "list_enumerator";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            rec->PrettyPrint(dst);
            dst<<" , ";
            en->PrettyPrint(dst);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class enumerator : public Node {
    //ENUMERATOR : IDENTIFIER 
    //           | IDENTIFIER ASSIGN EXPR_CONST
    private:
        std::string id;
        NodePtr exp;
    public:
        enumerator(std::string _id)
            : id(_id)
            , exp(NULL)
        {}
        enumerator(std::string _id, NodePtr _arg2)
            : id(_id)
            , exp(_arg2)
        {}
    public:
    
        std::string name = "enumerator";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            dst<<id<<" ";
            if(exp != NULL){
                dst<<"= ";
                exp->PrettyPrint(dst);
            }
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class qualifier_type : public Node {
    //QUALIFIER_TYPE : CONST
    //               | VOLATILE
    private:
        std::string val;
    public:
        qualifier_type(std::string _val)
            : val(_val)
        {}
    public:
    
        std::string name = "qualifier_type";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            dst<<val<<" ";
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class pointer : public Node {
    //POINTER : OP_MUL
    //        | OP_MUL LIST_QUALIFIER_TYPE
    //        | OP_MUL POINTER
    //        | OP_MUL LIST_QUALIFIER_TYPE POINTER
    private:
        NodePtr arg1;
        NodePtr arg2;
    public:
        pointer()
            : arg1(NULL)
            , arg2(NULL)
        {}
        pointer(NodePtr _arg1)
            : arg1(_arg1)
            , arg2(NULL)
        {}                
        pointer(NodePtr _arg1, NodePtr _arg2)
            : arg1(_arg1)
            , arg2(_arg2)
        {}
    public:
    
        std::string name = "pointer";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            dst<<'*';
            if(arg1 != NULL) arg1->PrettyPrint(dst);
            if(arg2 != NULL) arg2->PrettyPrint(dst);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class list_qualifier_type : public Node {
    //LIST_QUALIFIER_TYPE : QUALIFIER_TYPE
    //                    | LIST_QUALIFIER_TYPE QUALIFIER_TYPE
    private:
        NodePtr qt;
        NodePtr rec;
    public:
        list_qualifier_type(NodePtr _arg1, NodePtr _qt)
            : qt(_qt)
            , rec(_arg1)
        {}
    public:
    
        std::string name = "list_qualifier_type";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            rec->PrettyPrint(dst);
            qt->PrettyPrint(dst);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class list_param_type : public Node {
    //LIST_PARAM_TYPE : LIST_PARAMETER
    //                | LIST_PARAMETER ',' ELLIPSIS  // ELIPSIS = ...
    private:
        NodePtr list;
    public:
        list_param_type(NodePtr _arg1)
            : list(_arg1)
        {}
    public:
    
        std::string name = "list_param_type";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            list->PrettyPrint(dst);
            dst<<", ...";

        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class list_parameter : public Node {
    //LIST_PARAMETER : DECLARATION_PARAMETER
    //               | LIST_PARAMETER ',' DECLARATION_PARAMETER
    private:
        NodePtr rec;
        NodePtr dp;
        
    public:
        list_parameter(NodePtr _arg1, NodePtr _arg2)
            :  rec(_arg1)
            ,  dp(_arg2)
        {}
    public:
    
        std::string name = "list_parameter";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            rec->PrettyPrint(dst);
            dst<<',';
            dp->PrettyPrint(dst);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            rec->translate(dst,context);
            dst<<",";
            dp->translate(dst,context);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            rec->compile(dst,context);
            
            dp->compile(dst,context);
        
        }
};

class list_identifier : public Node {
    //LIST_IDENTIFIER : IDENTIFIER 
    //                | LIST_IDENTIFIER ',' IDENTIFIER
    private:
        std::string id;
        NodePtr rec;
    public:
        list_identifier(std::string _id)
            : id(_id)
            , rec(NULL)
        {}
        list_identifier(NodePtr _arg1, std::string _id)
            : id(_id)
            , rec(_arg1)
        {}
    public:
    
        std::string name = "list_identifier";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(rec != NULL){
                rec->PrettyPrint(dst);
                dst<<',';
            }
            dst<<id<<" ";
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class type_name : public Node {
    // TYPE_NAME : LIST_SPEC_QUAL
    //           | LIST_SPEC_QUAL DECLARATOR_ABSTRACT
    private:
        NodePtr list;
        NodePtr da;
    public:
        type_name(NodePtr _arg1, NodePtr _arg2)
            : list(_arg1)
            , da(_arg2)
        {}
    public:
    
        std::string name = "type_name";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            list->PrettyPrint(dst);
            da->PrettyPrint(dst);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};
#endif
