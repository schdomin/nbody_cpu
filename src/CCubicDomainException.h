#ifndef CCUBICDOMAINEXCEPTION_H_
#define CCUBICDOMAINEXCEPTION_H_

#include <exception> //ds exception handling
#include <string>    //ds string handling



namespace NBody
{

//ds standart exception
class CCubicDomainException: public std::exception
{

//ds error information
private:

    const std::string m_strErrorInformation;

//ds ctor/dtor
public:

    CCubicDomainException( const std::string& p_strErrorInformation ): m_strErrorInformation( p_strErrorInformation )
    {
        //ds nothing to do
    }

//ds virtual
public:

    const char* what( )
    {
        return m_strErrorInformation.c_str( );
    }
};

} //ds namespace NBody



#endif //ds CCUBICDOMAINEXCEPTION_H_
