#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int addr5; // Data/section.bss
int addr6; // Data/section.bss

//text segment->function declaration
int foo(); 
void point_at(void *p);
void foo1();
void foo2();

//text segment->function 
int main (int argc, char** argv){
    long a;
    printf("\n%lu\n", sizeof(a)); //=> 4

    int addr2; //stack
    int addr3; //stack
    char* yos="ree"; // yos pointer in stack-> text segment
    int * addr4 = (int*)(malloc(50)); // addr4 pointer in stack -> heap
    printf("- &addr2: %p\n",&addr2);
    printf("- &addr3: %p\n",&addr3);
    printf("- foo: %p\n",foo);
    printf("- &addr5: %p\n",&addr5);
    
	point_at(&addr5);
	
    printf("- &addr6: %p\n",&addr6);
    printf("- yos: %p\n",yos);
    printf("- addr4: %p\n",addr4);
    printf("- &addr4: %p\n",&addr4);
    
    printf("- &foo1: %p\n" ,&foo1);
    printf("- &foo2: %p\n" ,&foo2);
    printf("- &foo2 - &foo1: %ld\n" ,&foo2 - &foo1);
    return 0;
}

//text segment
int foo(){
    return -1;
}

//text segment
void point_at(void *p){
    int local; //stack
	static int addr0 = 2; //Data sedment/initialized data
    static int addr1;     //Data sedment/bss

    long dist1 = (size_t)&addr6 - (size_t)p;
    long dist2 = (size_t)&local - (size_t)p;
    long dist3 = (size_t)&foo - (size_t)p;
    
    printf("dist1: (size_t)&addr6 - (size_t)p: %ld\n",dist1);
    printf("dist2: (size_t)&local - (size_t)p: %ld\n",dist2);
    printf("dist3: (size_t)&foo - (size_t)p:  %ld\n",dist3);
	
	printf("- addr0: %p\n", & addr0);
    printf("- addr1: %p\n",&addr1);
}

//text segment
void foo1 (){    
    printf("foo1\n"); 
}

//text segment
void foo2 (){    
    printf("foo2\n");    
}