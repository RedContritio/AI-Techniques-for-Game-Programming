#ifndef __DEF__INC
#define __DEF__INC
#include "resource.h"
#define __MUTATION__RATE 0.8
#define __CROSSOVER__RATE 0.75
#define __POPULATION__SIZE 100
#define __CITIES__NUM 60
#define __NUM__BEST__KEEP 4

#define MutationType_Exchange ID_MENU_MUTATION01
#define MutationType_Displacement ID_MENU_MUTATION02
#define MutationType_Scramble ID_MENU_MUTATION03
#define MutationType_Insert ID_MENU_MUTATION04
#define MutationType_Automatic ID_MENU_MUTATION05
#define MutationType_BEGIN MutationType_Exchange
#define MutationType_END MutationType_Automatic

#define CrossoverType_PartiallyMapping ID_MENU_XOVER01
#define CrossoverType_OrderBasedOperator ID_MENU_XOVER02
#define CrossoverType_PositionBasedOperator ID_MENU_XOVER03
#define CrossoverType_BEGIN CrossoverType_PartiallyMapping
#define CrossoverType_END CrossoverType_PositionBasedOperator

#endif