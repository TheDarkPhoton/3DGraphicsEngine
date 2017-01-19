/*
 * dpGameState.h
 *
 *  Created on: 7 Aug 2014
 *      Author: darkphoton
 */

#ifndef DPGAMESTATE_H_
#define DPGAMESTATE_H_

#include <vector>
#include "dpEntity.h"
#include "dpUtility.h"

using namespace std;

class dpGameState {
	vector<dpEntity*> _entities;

public:
	dpGameState();
	~dpGameState();

	vector<dpEntity*>& GetEntities();
};

#endif /* DPGAMESTATE_H_ */
