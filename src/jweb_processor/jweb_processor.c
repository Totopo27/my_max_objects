#include "ext.h"
#include "ext_obex.h"
#include "ext_strings.h"

typedef struct _jweb_processor {
    t_object obj;
    void *outlet;
    long velocity_source;  // 1 = HTML, 2 = DAW
    t_atom *filtered_input;
    long filtered_input_size;
    char **filter_strings;
    long num_filter_strings;
} t_jweb_processor;

void *jweb_processor_new(t_symbol *s, long argc, t_atom *argv);
void jweb_processor_free(t_jweb_processor *x);
void jweb_processor_assist(t_jweb_processor *x, void *b, long m, long a, char *s);
void jweb_processor_int(t_jweb_processor *x, long n);
void jweb_processor_anything(t_jweb_processor *x, t_symbol *s, long ac, t_atom *av);
void jweb_processor_list(t_jweb_processor *x, t_symbol *s, long ac, t_atom *av);
void jweb_processor_daw_input(t_jweb_processor *x, t_symbol *s, long ac, t_atom *av);

static t_class *jweb_processor_class;

void ext_main(void *r)
{
    t_class *c;
    
    c = class_new("jweb_processor", (method)jweb_processor_new, (method)jweb_processor_free, sizeof(t_jweb_processor), 0L, A_GIMME, 0);
    
    class_addmethod(c, (method)jweb_processor_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)jweb_processor_int, "int", A_LONG, 0);
    class_addmethod(c, (method)jweb_processor_list, "list", A_GIMME, 0);
    class_addmethod(c, (method)jweb_processor_anything, "anything", A_GIMME, 0);
    class_addmethod(c, (method)jweb_processor_daw_input, "in1", A_GIMME, 0);
    
    class_register(CLASS_BOX, c);
    jweb_processor_class = c;
}

void *jweb_processor_new(t_symbol *s, long argc, t_atom *argv)
{
    t_jweb_processor *x = (t_jweb_processor *)object_alloc(jweb_processor_class);
    
    if (x) {
        x->outlet = outlet_new((t_object *)x, NULL);
        x->velocity_source = 1;  // Default to HTML velocity
        
        // Initialize filter strings
        x->num_filter_strings = 8;
        x->filter_strings = (char **)sysmem_newptr(sizeof(char *) * x->num_filter_strings);
        char *filters[] = {"title", "T31.html", "T19.html", "T41.html", "T53.html", 
                          "onloadstart", "url", "onloadend"};
        for (int i = 0; i < x->num_filter_strings; i++) {
            x->filter_strings[i] = (char *)sysmem_newptr(strlen(filters[i]) + 1);
            strcpy(x->filter_strings[i], filters[i]);
        }
        
        x->filtered_input = NULL;
        x->filtered_input_size = 0;
        
        intin((t_object *)x, 1);  // Create right inlet for DAW velocity
    }
    return x;
}

void jweb_processor_free(t_jweb_processor *x)
{
    if (x->filtered_input)
        sysmem_freeptr(x->filtered_input);
        
    for (int i = 0; i < x->num_filter_strings; i++)
        sysmem_freeptr(x->filter_strings[i]);
    sysmem_freeptr(x->filter_strings);
}

void jweb_processor_assist(t_jweb_processor *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_INLET) {
        switch (a) {
            case 0:
                sprintf(s, "Messages from jweb (list)");
                break;
            case 1:
                sprintf(s, "DAW velocity input (list)");
                break;
        }
    }
    else if (m == ASSIST_OUTLET) {
        sprintf(s, "Processed output (list: id velocity)");
    }
}

void jweb_processor_int(t_jweb_processor *x, long n)
{
    if (n >= 1 && n <= 2)
        x->velocity_source = n;
}

void jweb_processor_anything(t_jweb_processor *x, t_symbol *s, long ac, t_atom *av)
{
    // Filter out unwanted messages
    for (int i = 0; i < x->num_filter_strings; i++) {
        if (strcmp(s->s_name, x->filter_strings[i]) == 0)
            return;
    }
    
    // Process valid messages
    if (ac >= 2) {
        t_atom output[2];
        long id = 0;
        long velocity = 127;  // Default to note on
        
        // Extract ID
        if (atom_gettype(av) == A_LONG)
            id = atom_getlong(av);
        else if (atom_gettype(av) == A_SYM)
            id = atol(atom_getsym(av)->s_name);
        
        // Check for note off (velocity 0)
        if (ac >= 3) {
            t_atom *third_arg = av + 2;
            if (atom_gettype(third_arg) == A_LONG) {
                velocity = atom_getlong(third_arg);
            } else if (atom_gettype(third_arg) == A_SYM) {
                const char *val = atom_getsym(third_arg)->s_name;
                if (strcmp(val, "0") == 0) {
                    velocity = 0;
                }
            }
        }
        
        // Debug output
        object_post((t_object *)x, "Processing: id=%ld, args=%ld, third_arg_type=%s, third_arg_val=%s, velocity=%ld", 
                   id,
                   ac,
                   ac >= 3 ? (atom_gettype(av + 2) == A_LONG ? "long" : 
                             atom_gettype(av + 2) == A_SYM ? "symbol" : "unknown") : "none",
                   ac >= 3 ? (atom_gettype(av + 2) == A_LONG ? "number" :
                             atom_gettype(av + 2) == A_SYM ? atom_getsym(av + 2)->s_name : "unknown") : "none",
                   velocity);
        
        atom_setlong(output, id);
        atom_setlong(output + 1, velocity);
        outlet_list(x->outlet, NULL, 2, output);
    }
}

void jweb_processor_list(t_jweb_processor *x, t_symbol *s, long ac, t_atom *av)
{
    jweb_processor_anything(x, gensym("list"), ac, av);
}

void jweb_processor_daw_input(t_jweb_processor *x, t_symbol *s, long ac, t_atom *av)
{
    if (ac >= 2 && x->velocity_source == 2) {
        t_atom output[2];
        atom_setlong(output, atom_getlong(av));
        atom_setlong(output + 1, atom_getlong(av + 1));
        outlet_list(x->outlet, NULL, 2, output);
    }
}