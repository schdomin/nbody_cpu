#ifndef CVECTOR_H_
#define CVECTOR_H_

#include "CCubicDomainException.h" //ds exceptions

#include <ostream> //ds << operator
#include <math.h>  //ds sqrt



namespace NBody
{

//ds basic structure
class CVector
{

//ds not accessible from outside
private:

    //ds three elements
    double m_dElement0;
    double m_dElement1;
    double m_dElement2;

//ds ctor/dtor
public:

    CVector( const double& p_dElement0, const double& p_dElement1, const double& p_dElement2 ): m_dElement0( p_dElement0 ),
                                                                                                m_dElement1( p_dElement1 ),
                                                                                                m_dElement2( p_dElement2 )
    {
        //ds nothing to do
    }

    //ds default constructor - we need to have this since operator overloading for pointers is not allowed..
    CVector( ): m_dElement0( 0.0 ),
                m_dElement1( 0.0 ),
                m_dElement2( 0.0 )
    {
        //ds nothing to do
    }

    ~CVector( )
    {
        //ds nothing to do
    }


//ds operators
public:

    //ds indexing - ( ) is used instead of [ ] to mark the difference that this is not a "real" array - CAREFUL this allows manipulation of the data
    double& operator( )( const unsigned int& p_uIndex )
    {
        //ds map the index operator to the element
        if     ( 0 == p_uIndex ){ return m_dElement0; }
        else if( 1 == p_uIndex ){ return m_dElement1; }
        else if( 2 == p_uIndex ){ return m_dElement2; }

        //ds if an index greater 2 is required throw an exception
        else
        {
            throw NBody::CCubicDomainException( "invalid index access" );
        }
    }

    //ds setting
    void operator=( const CVector& p_cRightHandSide )
    {
        //ds get all the elements
        m_dElement0 = p_cRightHandSide.m_dElement0;
        m_dElement1 = p_cRightHandSide.m_dElement1;
        m_dElement2 = p_cRightHandSide.m_dElement2;
    }

    //ds adding and assign
    CVector& operator+=( const CVector& p_cRightHandSide )
    {
        //ds add all the elements
        m_dElement0 += p_cRightHandSide.m_dElement0;
        m_dElement1 += p_cRightHandSide.m_dElement1;
        m_dElement2 += p_cRightHandSide.m_dElement2;

        return *this;
    }

    //ds dividing and assign
    CVector& operator/=( const double& p_dRightHandSide )
    {
        //ds add all the elements
        m_dElement0 /= p_dRightHandSide;
        m_dElement1 /= p_dRightHandSide;
        m_dElement2 /= p_dRightHandSide;

        return *this;
    }

    //ds dividing
    CVector& operator/( const double& p_dRightHandSide )
    {
        //ds add all the elements
        m_dElement0 /= p_dRightHandSide;
        m_dElement1 /= p_dRightHandSide;
        m_dElement2 /= p_dRightHandSide;

        return *this;
    }

    //ds simple addition
    friend const CVector operator+( const CVector& p_cLeftHandSide, const CVector& p_cRightHandSide )
    {
        return CVector( p_cLeftHandSide.m_dElement0 + p_cRightHandSide.m_dElement0,
                        p_cLeftHandSide.m_dElement1 + p_cRightHandSide.m_dElement1,
                        p_cLeftHandSide.m_dElement2 + p_cRightHandSide.m_dElement2 );
    }

    //ds simple subtraction (needed for distance calculations)
    friend const CVector operator-( const CVector& p_cLeftHandSide, const CVector& p_cRightHandSide )
    {
        return CVector( p_cLeftHandSide.m_dElement0 - p_cRightHandSide.m_dElement0,
                        p_cLeftHandSide.m_dElement1 - p_cRightHandSide.m_dElement1,
                        p_cLeftHandSide.m_dElement2 - p_cRightHandSide.m_dElement2 );
    }

    //ds simple multiplication
    friend const CVector operator*( const double& p_dLeftHandSide, const CVector& p_cRightHandSide )
    {
        //ds add all the elements
        return CVector( p_dLeftHandSide*p_cRightHandSide.m_dElement0,
                        p_dLeftHandSide*p_cRightHandSide.m_dElement1,
                        p_dLeftHandSide*p_cRightHandSide.m_dElement2 );
    }

    //ds printing
    friend std::ostream& operator<<( std::ostream& p_cLeftHandSide, const CVector& p_cRightHandSide )
    {
        //ds build the string
        p_cLeftHandSide << "0: " << p_cRightHandSide.m_dElement0 << "\n"
                        << "1: " << p_cRightHandSide.m_dElement1 << "\n"
                        << "2: " << p_cRightHandSide.m_dElement2;

        return p_cLeftHandSide;
    }

//ds static functions
public:

    static double absoluteValue( const CVector& p_cVector )
    {
        return sqrt( pow( p_cVector.m_dElement0, 2 ) + pow( p_cVector.m_dElement1, 2 ) + pow( p_cVector.m_dElement2, 2 ) );
    }

    static const CVector crossProduct( const CVector& p_cVector1, const CVector& p_cVector2 )
    {
        return CVector( p_cVector1.m_dElement1*p_cVector2.m_dElement2-p_cVector1.m_dElement2*p_cVector2.m_dElement1,
                        p_cVector1.m_dElement2*p_cVector2.m_dElement0-p_cVector1.m_dElement0*p_cVector2.m_dElement2,
                        p_cVector1.m_dElement0*p_cVector2.m_dElement1-p_cVector1.m_dElement1*p_cVector2.m_dElement0 );
    }

};

} //ds namespace NBody



#endif //ds CVECTOR_H_
