#include "MorfologicalFilter.h"

using namespace Service::Image;

void MorfologicalFilter::OpenFunction()
{
	ErosionFilter();

	DilationFilter();
}

void MorfologicalFilter::CloseFunction()
{
	DilationFilter();

	ErosionFilter();
}

void MorfologicalFilter::OpenCloseFunction()
{
	CloseFunction();

	OpenFunction();
}

void MorfologicalFilter::CloseOpenFunction()
{
	OpenFunction();

	CloseFunction();
}
