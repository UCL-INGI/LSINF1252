#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "student_code.h"
#include "CTester/CTester.h"

void test_success(){
    
}

int main(int argc,char* argv[])
{
    BAN_FUNCS();
    RUN(test_success,test_fail_first,test_fail_twice,test_fail_five,test_fail_eight,test_fail_ten,test_fail_always);
}
