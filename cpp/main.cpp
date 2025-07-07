#include <iostream>

#include "Tlog.h"
#include "calc.h"

int main(int argc, char *argv[])
{
    vlogd("main");

    calc cl;
    int sum = cl.add(100,200);
    vlogd("calc:100 + 200 = " $(sum));

    return 0;
}