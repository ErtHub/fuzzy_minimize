//
// Created by hubert on 16.07.17.
//

#ifndef FUZZY_MINIMIZE_OPERATIONIMPL_H
#define FUZZY_MINIMIZE_OPERATIONIMPL_H

class OperationImpl
{
public:
    virtual double s_norm(double left, double right) = 0;
    virtual double t_norm(double left, double right) = 0;
    virtual double negate(double right) = 0;
};

class ZadehClassic : public OperationImpl
{
public:
    double s_norm(double left, double right)
    {
        return left > right ? left : right;
    }
    double t_norm(double left, double right)
    {
        return left < right ? left : right;
    }
    double negate(double right)
    {
        return 1 - right;
    }
};

#endif //FUZZY_MINIMIZE_OPERATIONIMPL_H
