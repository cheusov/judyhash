#ifndef SLOW_LEVEL
#error SLOW_LEVEL is undefined
#endif

int dummy = 0;
static void func (int slowness)
{
	// the following code must be compiled with no optimization,
	// i.e. with -O0 flag
	for (int i=0; i<slowness; ++i){
		dummy += slowness;
	}
}

int slow_compare_int (int a, int b, int slowness)
{
	func (slowness);
	return a - b;
}
