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
    // Check if we have at least one argument
    if (argc > 0) {
        // Get the message type
        t_symbol *msg_type = atom_getsym(argv);
        
        // Process click events
        if (msg_type == gensym("click")) {
            // We expect at least one more argument for the key ID
            if (argc > 1 && atom_gettype(argv + 1) == A_LONG) {
                long key_id = atom_getlong(argv + 1);
                t_atom out[2];
                
                // Output key ID and velocity 1 (press)
                atom_setlong(out, key_id);
                atom_setlong(out + 1, 1);
                outlet_list(x->outlet, NULL, 2, out);
            }
        }
        // Process release events
        else if (msg_type == gensym("release")) {
            // We expect at least one more argument for the key ID
            if (argc > 1 && atom_gettype(argv + 1) == A_LONG) {
                long key_id = atom_getlong(argv + 1);
                t_atom out[2];
                
                // Output key ID and velocity 0 (release)
                atom_setlong(out, key_id);
                atom_setlong(out + 1, 0);
                outlet_list(x->outlet, NULL, 2, out);
            }
        }
        // Process chord events
        else if (msg_type == gensym("chord")) {
            // We expect at least two more arguments: event type and array of key IDs
            if (argc > 2) {
                t_symbol *event_type = atom_getsym(argv + 1);
                long velocity = (event_type == gensym("pressed")) ? 1 : 0;
                
                // Process each key ID in the chord
                for (long i = 2; i < argc; i++) {
                    if (atom_gettype(argv + i) == A_LONG) {
                        long key_id = atom_getlong(argv + i);
                        t_atom out[2];
                        
                        // Output key ID and velocity
                        atom_setlong(out, key_id);
                        atom_setlong(out + 1, velocity);
                        outlet_list(x->outlet, NULL, 2, out);
                    }
                }
            }
        }
    }
}