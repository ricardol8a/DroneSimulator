//Default Compilation Options
#ifdef VIDEO
#if ((VIDEO != NONE) && (VIDEO != LOW) && (VIDEO != MEDIUM) && (VIDEO != HIGH))
#undef VIDEO
#endif
#endif
#ifndef VIDEO
#define VIDEO HIGH
#endif

#ifdef MATH
#if ((MATH != FASTEST) && (MATH != QUICK) && (MATH != BALANCED) && (MATH != BEST))
#undef MATH
#endif
#endif
#ifndef MATH
#define MATH BALANCED
#endif

#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif

#ifdef MODEL
#if ((MODEL != REAL) && (MODEL != VIRTUAL) && (MODEL != VIRTUAL2) && (MODEL != VIRTUAL3))
#undef MODEL
#endif
#endif
#ifndef MODEL
#define MODEL VIRTUAL
#endif

#ifdef CONTROL
#if ((CONTROL != MANUAL) && (CONTROL != AUTOMATIC) && (CONTROL != AUTOMATIC2) && (CONTROL != AUTOMATIC3) && (CONTROL != AUTOMATIC4))
#undef CONTROL
#endif
#endif
#ifndef CONTROL
#define CONTROL AUTOMATIC
#endif

//Table of Quality of Video
#define NONE 0
#define LOW 1
#define MEDIUM 2
#define HIGH 3

//Table of Accuracy of Calculus
#define FASTEST 1
#define QUICK 2
#define BALANCED 3
#define BEST 4

//Table of Type of Models
#define REAL 0
#define VIRTUAL 1
#define VIRTUAL2 2
#define VIRTUAL3 3

//Table of Type of Controls
#define MANUAL 0
#define AUTOMATIC 1
#define AUTOMATIC2 2
#define AUTOMATIC3 3
#define AUTOMATIC4 4

