/**
 * calcmod - External object for Max/MSP
 * Performs sum and subtraction operations with modular arithmetic
 * 
 * @author Totopo27
 */

#include "ext.h"
#include "ext_obex.h"

typedef struct _calcmod {
    t_object obj;
    void *p_proxy4;    // Proxy for temperament input (rightmost inlet)
    void *p_proxy3;    // Proxy for second subtraction input
    void *p_proxy2;    // Proxy for first subtraction input
    void *p_proxy1;    // Proxy for second sum input
    long sum_in1;      // First sum input (leftmost inlet)
    long sum_in2;      // Second sum input
    long sub_in1;      // First subtraction input
    long sub_in2;      // Second subtraction input
    long temperament;  // Temperament value for modulo
    long inlet_num;    // Which inlet is being accessed
    void *out;        // Single outlet for all results
} t_calcmod;

// Function prototypes
void *calcmod_new(void);
void calcmod_free(t_calcmod *x);
void calcmod_int(t_calcmod *x, long n);
void calcmod_assist(t_calcmod *x, void *b, long m, long a, char *s);
void calcmod_calculate(t_calcmod *x);

// Class pointer
static t_class *calcmod_class;

void ext_main(void *r)
{
    t_class *c;
    
    c = class_new("calcmod", (method)calcmod_new, (method)calcmod_free, sizeof(t_calcmod), 0L, 0);
    
    // Add methods
    class_addmethod(c, (method)calcmod_int, "int", A_LONG, 0);
    class_addmethod(c, (method)calcmod_assist, "assist", A_CANT, 0);
    
    class_register(CLASS_BOX, c);
    calcmod_class = c;
    
    post("calcmod: External loaded");
}

void *calcmod_new(void)
{
    t_calcmod *x = (t_calcmod *)object_alloc(calcmod_class);
    
    // Create proxies for inlets from right to left
    x->p_proxy4 = proxy_new((t_object *)x, 4, &x->inlet_num);  // Temperament input
    x->p_proxy3 = proxy_new((t_object *)x, 3, &x->inlet_num);  // Second subtraction input
    x->p_proxy2 = proxy_new((t_object *)x, 2, &x->inlet_num);  // First subtraction input
    x->p_proxy1 = proxy_new((t_object *)x, 1, &x->inlet_num);  // Second sum input
    
    // Create single outlet
    x->out = intout((t_object *)x);
    
    // Initialize values
    x->sum_in1 = 0;      // Leftmost inlet
    x->sum_in2 = 0;      // Second inlet
    x->sub_in1 = 0;      // Third inlet
    x->sub_in2 = 0;      // Fourth inlet
    x->temperament = 12; // Default temperament value
    
    post("calcmod: New instance created with default temperament of 12");
    return x;
}

void calcmod_free(t_calcmod *x)
{
    object_free(x->p_proxy4);  // Free rightmost inlet proxy (temperament)
    object_free(x->p_proxy3);  // Free fourth inlet proxy
    object_free(x->p_proxy2);  // Free third inlet proxy
    object_free(x->p_proxy1);  // Free second inlet proxy
    post("calcmod: Instance freed");
}

void calcmod_assist(t_calcmod *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_INLET) {
        switch (a) {
            case 0: // Leftmost inlet
                sprintf(s, "First sum input (int)");
                break;
            case 1: // Second inlet
                sprintf(s, "Second sum input (int)");
                break;
            case 2: // Third inlet
                sprintf(s, "First subtraction input (int)");
                break;
            case 3: // Fourth inlet
                sprintf(s, "Second subtraction input (int)");
                break;
            case 4: // Rightmost inlet
                sprintf(s, "Temperament value for modulo (int)");
                break;
        }
    }
    else if (m == ASSIST_OUTLET) {
        sprintf(s, "All results (int)");
    }
}

void calcmod_int(t_calcmod *x, long n)
{
    int inlet = proxy_getinlet((t_object *)x);
    post("calcmod: Received %ld on inlet %d", n, inlet);
    
    switch (inlet) {
        case 0: // First sum input (leftmost inlet)
            x->sum_in1 = n;
            post("calcmod: Set first sum input to %ld", n);
            calcmod_calculate(x);
            break;
        case 1: // Second sum input
            x->sum_in2 = n;
            post("calcmod: Set second sum input to %ld", n);
            calcmod_calculate(x);
            break;
        case 2: // First subtraction input
            x->sub_in1 = n;
            post("calcmod: Set first subtraction input to %ld", n);
            calcmod_calculate(x);
            break;
        case 3: // Second subtraction input
            x->sub_in2 = n;
            post("calcmod: Set second subtraction input to %ld", n);
            calcmod_calculate(x);
            break;
        case 4: // Temperament input
            if (n <= 0) {
                post("calcmod: Warning - temperament must be positive, using absolute value");
                n = abs(n);
                if (n == 0) n = 12; // Default to 12 if zero
            }
            x->temperament = n;
            post("calcmod: Set temperament to %ld", n);
            calcmod_calculate(x);
            break;
    }
}

long apply_temperament(long value, long temperament) {
    if (value < 0) {
        // For negative numbers, we need to handle the modulo differently
        // to get positive results between 0 and (temperament-1)
        return ((value % temperament) + temperament) % temperament;
    }
    return value % temperament;
}

void calcmod_calculate(t_calcmod *x)
{
    int inlet = proxy_getinlet((t_object *)x);
    post("calcmod: Starting calculations with values:");
    post("  sum_in1 = %ld, sum_in2 = %ld", x->sum_in1, x->sum_in2);
    post("  sub_in1 = %ld, sub_in2 = %ld", x->sub_in1, x->sub_in2);
    post("  temperament = %ld", x->temperament);

    // Determine which operation to perform based on the inlet
    if (inlet == 0 || inlet == 1) {  // Sum inlets
        // Calculate sum and apply temperament
        long raw_sum = x->sum_in1 + x->sum_in2;
        long sum_result = apply_temperament(raw_sum, x->temperament);
        post("  Sum operation: (%ld + %ld) mod %ld = %ld", 
             x->sum_in1, x->sum_in2, x->temperament, sum_result);
        outlet_int(x->out, sum_result);
    }
    else if (inlet == 2 || inlet == 3) {  // Subtraction inlets
        // Calculate subtraction and apply temperament
        long raw_sub = x->sub_in1 - x->sub_in2;
        long sub_result = apply_temperament(raw_sub, x->temperament);
        post("  Subtraction operation: (%ld - %ld) mod %ld = %ld", 
             x->sub_in1, x->sub_in2, x->temperament, sub_result);
        outlet_int(x->out, sub_result);
    }
    else if (inlet == 4) {  // Temperament inlet - recalculate last operation
        // Determine which operation to recalculate based on most recent non-temperament input
        if (x->inlet_num == 0 || x->inlet_num == 1) {
            long raw_sum = x->sum_in1 + x->sum_in2;
            long sum_result = apply_temperament(raw_sum, x->temperament);
            post("  Recalculating sum with new temperament: (%ld + %ld) mod %ld = %ld", 
                 x->sum_in1, x->sum_in2, x->temperament, sum_result);
            outlet_int(x->out, sum_result);
        } else {
            long raw_sub = x->sub_in1 - x->sub_in2;
            long sub_result = apply_temperament(raw_sub, x->temperament);
            post("  Recalculating subtraction with new temperament: (%ld - %ld) mod %ld = %ld", 
                 x->sub_in1, x->sub_in2, x->temperament, sub_result);
            outlet_int(x->out, sub_result);
        }
    }
    
    post("calcmod: Calculation complete");
}