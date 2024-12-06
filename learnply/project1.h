#pragma once
#include "polyhedron.h"

// Final
void applyCustomSingleHue(Polyhedron* poly, icVector3& color);

void applyCustomDivergent(Polyhedron* poly, icVector3& color1, icVector3& color2);

void initializeStats(Polyhedron* poly);
 
struct DataStats {
	double median;
	double min;
	double max;
	bool isInitialized = false;
};

// q1
void greyscale(Polyhedron* poly);

//q1 b
void multi(Polyhedron* poly);

// q1 c
void heatmap(Polyhedron* poly);

//convert RGBtoHSV
void RGBtoHSV(icVector3& hsv, icVector3& rgb);

//convert HSVtoRGB
void HSVtoRGB(icVector3& hsv, icVector3& rgb);

//q2
void height(Polyhedron* poly);
void resetHeight(Polyhedron* poly);

void findMm(Polyhedron* poly, double& M, double& m);
void findMedian(Polyhedron* poly, double& m);
