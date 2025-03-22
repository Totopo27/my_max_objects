#include "ext.h"
#include "ext_obex.h"
#include <math.h>

// Object structure
typedef struct _temperaments {
    t_object obj;
    void *outlet;
    long temperament;    // Current temperament (19, 31, 41, 53)
    double base_freq;    // Base frequency (default 27.5 Hz)
    double *frequencies; // Array to store calculated frequencies
    long num_keys;      // Number of keys in current temperament
} t_temperaments;

// Function prototypes
void *temperaments_new(t_symbol *s, long argc, t_atom *argv);
void temperaments_free(t_temperaments *x);
void temperaments_assist(t_temperaments *x, void *b, long m, long a, char *s);
void temperaments_int(t_temperaments *x, long n);
void temperaments_float(t_temperaments *x, double f);
void temperaments_list(t_temperaments *x, t_symbol *s, long argc, t_atom *argv);
void temperaments_calculate(t_temperaments *x);

static t_class *temperaments_class;

// Main entry point
void ext_main(void *r)
{
    t_class *c;
    
    c = class_new("temperaments", (method)temperaments_new, (method)temperaments_free, sizeof(t_temperaments), 0L, A_GIMME, 0);
    
    class_addmethod(c, (method)temperaments_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)temperaments_int, "int", A_LONG, 0);
    class_addmethod(c, (method)temperaments_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)temperaments_list, "list", A_GIMME, 0);
    
    class_register(CLASS_BOX, c);
    temperaments_class = c;
}

// Object constructor
void *temperaments_new(t_symbol *s, long argc, t_atom *argv)
{
    t_temperaments *x = (t_temperaments *)object_alloc(temperaments_class);
    
    if (x) {
        // Create outlet for frequency-velocity pairs
        x->outlet = outlet_new((t_object *)x, NULL);
        
        // Set default values
        x->temperament = 19;
        x->base_freq = 27.5;
        x->frequencies = NULL;
        x->num_keys = 0;
        
        // Process arguments if provided
        if (argc >= 1) {
            if (atom_gettype(argv) == A_LONG) {
                x->temperament = atom_getlong(argv);
            }
        }
        if (argc >= 2) {
            if (atom_gettype(argv + 1) == A_FLOAT) {
                x->base_freq = atom_getfloat(argv + 1);
            }
        }
        
        // Calculate initial frequencies
        temperaments_calculate(x);
    }
    
    return x;
}

// Object destructor
void temperaments_free(t_temperaments *x)
{
    if (x->frequencies) {
        sysmem_freeptr(x->frequencies);
    }
}

// Assist method for inlets/outlets
void temperaments_assist(t_temperaments *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_INLET) {
        switch (a) {
            case 0:
                sprintf(s, "Temperament value (19, 31, 41, 53)");
                break;
            case 1:
                sprintf(s, "Key ID and velocity pairs");
                break;
        }
    }
    else {
        sprintf(s, "Frequency and velocity pairs");
    }
}

// Handle integer input (temperament selection)
void temperaments_int(t_temperaments *x, long n)
{
    // Only process valid temperaments
    if (n == 19 || n == 31 || n == 41 || n == 53) {
        x->temperament = n;
        temperaments_calculate(x);
    }
}

// Handle float input (base frequency)
void temperaments_float(t_temperaments *x, double f)
{
    if (f > 0.0) {
        x->base_freq = f;
        temperaments_calculate(x);
    }
}

// Handle list input (key ID + velocity)
void temperaments_list(t_temperaments *x, t_symbol *s, long argc, t_atom *argv)
{
    if (argc >= 2) {
        long key_id = atom_getlong(argv);
        long velocity = atom_getlong(argv + 1);
        
        // Check if key ID is valid
        if (key_id >= 0 && key_id < x->num_keys) {
            t_atom out[2];
            atom_setfloat(out, x->frequencies[key_id]);
            atom_setlong(out + 1, velocity);
            outlet_list(x->outlet, NULL, 2, out);
        }
    }
}

// Calculate frequencies for current temperament
void temperaments_calculate(t_temperaments *x)
{
    // Free previous array if it exists
    if (x->frequencies) {
        sysmem_freeptr(x->frequencies);
    }
    
    // Set number of keys based on temperament
    x->num_keys = x->temperament * 4;  // 4 octaves
    
    // Allocate memory for frequencies
    x->frequencies = (double *)sysmem_newptr(x->num_keys * sizeof(double));
    
    if (x->frequencies) {
        // Calculate step size based on temperament
        double step = 1.0 / x->temperament;
        
        // Calculate frequencies
        for (long i = 0; i < x->num_keys; i++) {
            x->frequencies[i] = x->base_freq * pow(2.0, step * i);
        }
    }
}