/*
 * IChurnGenerator.h
 *
 *  Created on: Apr 9, 2012
 *      Author: giang
 */

#ifndef ICHURNGENERATOR_H_
#define ICHURNGENERATOR_H_

class IChurnGenerator
{
public:
    // IChurnGenerator() {};
    virtual ~IChurnGenerator() {};

public:
    virtual double getArrivalTime() = 0;
    virtual double getSessionDuration() = 0;
};

#endif /* ICHURNGENERATOR_H_ */
