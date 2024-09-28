#include <stdint.h>
#include <stdio.h>

// Define the enum Type
enum Type {
    TYPE1,
    TYPE2
};

// Define the Base struct
struct Base {
    enum Type type;
};

// Define Type1 struct
struct Type1 {
    struct Base base;
    uint32_t value;
};

// Define Type2 struct
struct Type2 {
    struct Base base;
    char another_value;
};
int array[256];

// Implementation of the leak function
void leak(uint32_t val) {
    printf("Leaked value: %u\n", array[val]);
}


/**
 * The STC vulnerability:- the attacker is able 
 * to pass in obj of type2 and execute the first conditional statement 
 * speculatively. I.e. 
 * 1. Intially the branch predictor for the  first condition is trained to predict 
 *    the conditional statment as taken then 
 * 2. The attacker will pass obj of type2 to the system call. This will make the the processor 
 *    speculate the branch as taken and cast the new object to type of TYPE1. 
 * 3. then the cpu will read the value of "o->value" which will depend of the memory layout of object of type 2
 * 
 */
void syscall_helper(struct Base* obj) {
    if (obj == NULL) return;  // Safety check

    if (obj->type == TYPE1) {
        struct Type1* o = (struct Type1*) obj;
        leak(o->value);
    }
    if (obj->type == TYPE2) {
        struct Type2* o = (struct Type2*) obj;
        leak(o->another_value);
    }
}

// Main function
int main() {
    //initialize our array
    for(int i=0;i<256;i++){
        array[i] = i*i; 
    }

    // Create an instance of Type1
    struct Type1 obj1;
    obj1.base.type = TYPE1;
    obj1.value = 42;

    // Create an instance of Type2
    struct Type2 obj2;
    obj2.base.type = TYPE2;
    obj2.another_value = 'A';

    // Call syscall_helper with Type1 object
    syscall_helper((struct Base*) &obj1);

    // Call syscall_helper with Type2 object
    syscall_helper((struct Base*) &obj2);

    return 0;
}
