
static unsigned long autoSeeder;
static unsigned long currentNumber;

void randomAutoSeederTick (void)
{
	autoSeeder++;
}

void autoSeedRandom (void)
{
	currentNumber = autoSeeder;
}

void seedRandom (unsigned long seed)
{
	currentNumber = seed;
}

unsigned long getRandom (void)
{
	currentNumber = currentNumber * 1664525 + 1013904223;
	return currentNumber;
}
