#include <stdint.h>

volatile char A[256 * 512];
bool predicate;
char *foo;
/**
 * x is assumed to be user controlleld input 
 * the attacker trains the BP to predict both conditional statments as taken.
 * Then when this function is exectued, statmetns in both the if conditions will execute speculatively 
 * this means the attacker controlled input x will be used to index into V.
 * Hence STC vulnerable gadget  
 */
void victim(char *p, uint64_t x, char *q)
{
    unsigned char v;
    if (predicate)
    {
        p = (char *)x;
        *q |= 1;
    }
    if (!predicate)
    {
        v = A[(*p) * 512];
    }

    foo = p;
}

int main(){
    
    return 0;
}
