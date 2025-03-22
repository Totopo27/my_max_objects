#include "ext.h"
#include "ext_obex.h"
#include <string.h>

// Object structure
typedef struct _jweb_processor {
    t_object obj;
    void *outlet;
} t_jweb_processor;

// Function prototypes
void *jweb_processor_new(t_symbol *s, long argc, t_atom *argv);
void jweb_processor_free(t_jweb_processor *x);
void jweb_processor_assist(t_jweb_processor *x, void *b, long m, long a, char *s);
void jweb_processor_anything(t_jweb_processor *x, t_symbol *s, long argc, t_atom *argv);

static t_class *jweb_processor_class;

// Main entry point
void ext_main(void *r)
{
    t_class *c;
    
    c = class_new("jweb_processor", (method)jweb_processor_new, (method)jweb_processor_free, sizeof(t_jweb_processor), 0L, A_GIMME, 0);
    
    class_addmethod(c, (method)jweb_processor_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)jweb_processor_anything, "anything", A_GIMME, 0);
    
    class_register(CLASS_BOX, c);
    jweb_processor_class = c;
}

// Object constructor
void *jweb_processor_new(t_symbol *s, long argc, t_atom *argv)
{
    t_jweb_processor *x = (t_jweb_processor *)object_alloc(jweb_processor_class);
    
    if (x) {
        x->outlet = outlet_new((t_object *)x, NULL);
    }
    
    return x;
}

// Object destructor
void jweb_processor_free(t_jweb_processor *x)
{
    // Nothing to free
}

// Assist method for inlets/outlets
void jweb_processor_assist(t_jweb_processor *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_INLET) {
        sprintf(s, "Messages from jweb object");
    }
    else {
        sprintf(s, "Processed messages (id velocity)");
    }
}

// Process incoming messages from jweb
void jweb_processor_anything(t_jweb_processor *x, t_symbol *s, long argc, t_atom *argv)
{
    // Debug: Print the incoming message
    post("jweb_processor received message with %ld arguments", argc);
    post("Message selector: %s", s->s_name);
    
    for (long i = 0; i < argc; i++) {
        t_atom *arg = argv + i;
        if (atom_gettype(arg) == A_LONG) {
            post("Arg %ld: %ld (long)", i, atom_getlong(arg));
        } else if (atom_gettype(arg) == A_FLOAT) {
            post("Arg %ld: %f (float)", i, atom_getfloat(arg));
        } else if (atom_gettype(arg) == A_SYM) {
            post("Arg %ld: %s (symbol)", i, atom_getsym(arg)->s_name);
        }
    }
    
    // Check if we have at least one argument
    if (argc > 0) {
        // Get the message type
        t_symbol *msg_type = atom_getsym(argv);
        post("Processing message type: %s", msg_type->s_name);
        
        // Process key events
        if (msg_type == gensym("click") || msg_type == gensym("release")) {
            post("Processing key event");
            
            // We expect at least two more arguments
            if (argc >= 3) {
                // Check the types of the arguments
                if (atom_gettype(argv + 1) == A_LONG) {
                    long key_id = atom_getlong(argv + 1);
                    post("Key ID: %ld", key_id);
                    
                    // Try to get velocity from the third argument
                    long velocity = 0;
                    if (atom_gettype(argv + 2) == A_LONG) {
                        velocity = atom_getlong(argv + 2);
                    } else if (atom_gettype(argv + 2) == A_SYM) {
                        // If it's "do", look for velocity in the next argument
                        t_symbol *note_sym = atom_getsym(argv + 2);
                        if (note_sym == gensym("do") && argc >= 4 && atom_gettype(argv + 3) == A_LONG) {
                            velocity = atom_getlong(argv + 3);
                        }
                    }
                    post("Velocity: %ld", velocity);
                    
                    t_atom out[2];
                    atom_setlong(out, key_id);
                    atom_setlong(out + 1, velocity);
                    outlet_list(x->outlet, NULL, 2, out);
                }
            }
        }
        // Process chord events
        else if (msg_type == gensym("chord")) {
            post("Processing chord event");
            
            if (argc > 2) {
                t_symbol *event_type = atom_getsym(argv + 1);
                long velocity = (event_type == gensym("pressed")) ? 127 : 0;
                post("Chord event type: %s, velocity: %ld", event_type->s_name, velocity);
                
                // Process each key ID in the chord
                for (long i = 2; i < argc; i++) {
                    if (atom_gettype(argv + i) == A_LONG) {
                        long key_id = atom_getlong(argv + i);
                        post("Chord key ID: %ld", key_id);
                        
                        t_atom out[2];
                        atom_setlong(out, key_id);
                        atom_setlong(out + 1, velocity);
                        outlet_list(x->outlet, NULL, 2, out);
                    }
                }
            }
        }
    }
}