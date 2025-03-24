/**
 * calcmod - External object for Max/MSP
 * Performs sum and subtraction operations with modulo temperament
 * 
 * @author Totopo27
 */

#include "ext.h"
#include "ext_obex.h"

typedef struct _calcmod {
    t_object obj;
    long sum_in1;      // First input for sum
    long sum_in2;      // Second input for sum
    long sub_in1;      // First input for subtraction
    long sub_in2;      // Second input for subtraction
    long temperament;  // Temperament value for modulo
    void *out;        // Outlet
} t_calcmod;

// Function prototypes
void *calcmod_new(void);
void calcmod_free(t_calcmod *x);
void calcmod_sum_in1(t_calcmod *x, long n);
void calcmod_sum_in2(t_calcmod *x, long n);
void calcmod_sub_in1(t_calcmod *x, long n);
void calcmod_sub_in2(t_calcmod *x, long n);
void calcmod_temperament(t_calcmod *x, long n);
void calcmod_calculate(t_calcmod *x);

// Class pointer
static t_class *calcmod_class;

void ext_main(void *r)
{
    t_class *c;
    
    c = class_new("calcmod", (method)calcmod_new, (method)calcmod_free, sizeof(t_calcmod), 0L, 0);
    
    // Add methods for each inlet
    class_addmethod(c, (method)calcmod_sum_in1, "int", A_LONG, 0);
    class_addmethod(c, (method)calcmod_sum_in2, "in2", A_LONG, 0);
    class_addmethod(c, (method)calcmod_sub_in1, "in3", A_LONG, 0);
    class_addmethod(c, (method)calcmod_sub_in2, "in4", A_LONG, 0);
    class_addmethod(c, (method)calcmod_temperament, "in5", A_LONG, 0);
    
    class_register(CLASS_BOX, c);
    calcmod_class = c;
}

void *calcmod_new(void)
{
    t_calcmod *x = (t_calcmod *)object_alloc(calcmod_class);
    
    // Create inlets (first inlet is automatic)
    inlet_new(&x->obj, "in2");
    inlet_new(&x->obj, "in3");
    inlet_new(&x->obj, "in4");
    inlet_new(&x->obj, "in5");
    
    // Create outlet
    x->out = intout((t_object *)x);
    
    // Initialize values
    x->sum_in1 = 0;
    x->sum_in2 = 0;
    x->sub_in1 = 0;
    x->sub_in2 = 0;
    x->temperament = 1; // Default temperament to avoid division by zero
    
    return x;
}

void calcmod_free(t_calcmod *x)
{
    // Nothing to free
}

void calcmod_sum_in1(t_calcmod *x, long n)
{
    x->sum_in1 = n;
    calcmod_calculate(x);
}

void calcmod_sum_in2(t_calcmod *x, long n)
{
    x->sum_in2 = n;
    calcmod_calculate(x);
}

void calcmod_sub_in1(t_calcmod *x, long n)
{
    x->sub_in1 = n;
    calcmod_calculate(x);
}

void calcmod_sub_in2(t_calcmod *x, long n)
{
    x->sub_in2 = n;
    calcmod_calculate(x);
}

void calcmod_temperament(t_calcmod *x, long n)
{
    if (n != 0) { // Prevent division by zero
        x->temperament = n;
        calcmod_calculate(x);
    }
}

void calcmod_calculate(t_calcmod *x)
{
    long sum_result = x->sum_in1 + x->sum_in2;
    long sub_result = x->sub_in1 - x->sub_in2;
    long final_result;
    
    // Apply modulo operation
    if (x->temperament > 0) {
        final_result = ((sum_result + sub_result) % x->temperament + x->temperament) % x->temperament;
        outlet_int(x->out, final_result);
    }
}