#ifndef CCUBICDOMAIN_H_
#define CCUBICDOMAIN_H_

#include "CCubicDomainException.h" //ds exceptions
#include "CVector.h"               //ds basic structure for 3d information

#include <random>    //ds normal distribution
#include <iostream>



namespace NBody
{

//ds domain to solve n-body problems
class CCubicDomain
{

//ds ctor/dtor
public:

    //ds default constructor requires environmental parameters: N number of bodies, dT time step, T number of time steps
    CCubicDomain( const std::pair< double, double >& p_pairBoundaries,
                  const unsigned int& p_uNumberOfParticles ): m_arrParticles( 0 ),
                                                              m_pairBoundaries( p_pairBoundaries ),
                                                              m_dBoundarySize( labs( m_pairBoundaries.first ) + labs( m_pairBoundaries.second ) ),
                                                              m_uNumberOfParticles( p_uNumberOfParticles )
    {
        //ds nothing to do
    }

    //ds default destructor
    ~CCubicDomain( )
    {
        //ds deallocate memory
        delete[] m_arrParticles;
    }

//ds attributes
private:

    //ds inherent structure for particle information
    struct CParticle
    {
        //ds constant properties
        double m_dMass;

        //ds position and velocity and acceleration information
        NBody::CVector m_cPosition;
        NBody::CVector m_cVelocity;
        NBody::CVector m_cAcceleration;

    } *m_arrParticles;

    //ds domain properties
    const std::pair< double, double > m_pairBoundaries;
    const double m_dBoundarySize;
    const unsigned int m_uNumberOfParticles;

//ds accessors
public:

    void createParticlesUniformFromNormalDistribution( const double& p_dParticleMass = 1.0, const double& p_dMean = 0.0, const double& p_dStandartDeviation = 0.1 )
    {
        //ds allocate an array for the new particles
        m_arrParticles = new CParticle[m_uNumberOfParticles];

        //ds normal and uniform distribution for initial conditions (C++ 11)
        std::default_random_engine cGenerator;
        std::uniform_real_distribution< double > cDistributionUniform( m_pairBoundaries.first, m_pairBoundaries.second );
        std::normal_distribution< double > cDistributionNormal( p_dMean, p_dStandartDeviation );

        //ds set particle information for each
        for( unsigned int u = 0; u < m_uNumberOfParticles; ++u )
        {
            //ds set the particle mass (same for all particles in this case)
            m_arrParticles[u].m_dMass = p_dParticleMass;

            //ds set the position: uniformly distributed between boundaries in this case
            m_arrParticles[u].m_cPosition = NBody::CVector( cDistributionUniform( cGenerator ),
                                                            cDistributionUniform( cGenerator ),
                                                            cDistributionUniform( cGenerator ) );

            //ds set velocity values: from normal distribution
            m_arrParticles[u].m_cVelocity = NBody::CVector( cDistributionNormal( cGenerator ) ,
                                                            cDistributionNormal( cGenerator ) ,
                                                            cDistributionNormal( cGenerator ) );

            //ds nothing to do for the accelerations in this case
        }
    }

    void updateParticlesVelocityVerlet( const double& p_dTimeStep, const double& p_dMinimumDistance, const double& p_dPotentialDepth )
    {
        //ds allocate a temporary array to hold the accelerations
        CVector *arrAccelerations = new CVector[m_uNumberOfParticles];

        //ds for each particle
        for( unsigned int u = 0; u < m_uNumberOfParticles; ++u )
        {
            //ds sum of forces acting on the particle
            CVector cTotalForce;

            //ds loop over all other particles
            for( unsigned int v = 0; v < m_uNumberOfParticles; ++v )
            {
                //ds if its not the same particle
                if( u != v )
                {
                    //ds collect the force from the current particle and add it
                    CVector tempForce= getLennardJonesForce( m_arrParticles[u], m_arrParticles[v], p_dMinimumDistance, p_dPotentialDepth );

                    cTotalForce += tempForce;

                    /*if( 10 < tempForce( 0 ) || 10 < tempForce( 1 ) || 10 < tempForce( 2 ) )
                    {
                        std::cout << "u: " << u << " v: " << v << std::endl;
                        std::cout << "single force: \n" << tempForce << std::endl;

                        std::cout << "particle 1: \n" << m_arrParticles[u].m_cPosition << std::endl;
                        std::cout << "particle 2: \n" << m_arrParticles[v].m_cPosition << std::endl;
                        std::cout << "particle 3: \n" << m_arrParticles[v-1].m_cPosition << std::endl;

                        getchar( );
                    }*/
                }
            }

            //ds if we got the total force calculate the resulting acceleration and save it to our array
            arrAccelerations[u] = cTotalForce/m_arrParticles[u].m_dMass;

            //std::cout << arrAccelerations[u] << std::endl;
        }

        //ds for each particle we have to calculate the effect of the acceleration now
        for( unsigned int u = 0; u < m_uNumberOfParticles; ++u )
        {
            //ds velocity-verlet for position
            m_arrParticles[u].m_cPosition = m_arrParticles[u].m_cPosition + p_dTimeStep*m_arrParticles[u].m_cVelocity + 1/2*pow( p_dTimeStep, 2 )*m_arrParticles[u].m_cAcceleration;

            /*ds check if we have to shift the particle
            for( unsigned int v = 0; v < 3; ++v )
            {
                //ds check if we are below the boundary
                if( m_pairBoundaries.first > m_arrParticles[u].m_cPosition( v ) )
                {
                    //std::cout << m_arrParticles[u].m_cPosition( v ) << "to 1" << std::endl;

                    //ds map the particle to the other boundary by shifting it up to the boundary
                    m_arrParticles[u].m_cPosition( v ) = m_pairBoundaries.second;
                }

                //ds check if we are above the boundary
                else if( m_pairBoundaries.second < m_arrParticles[u].m_cPosition ( v ) )
                {
                    //std::cout << m_arrParticles[u].m_cPosition( v ) << "to -1" << std::endl;

                    //ds map the particle to the other boundary by shifting it back to the boundary
                    m_arrParticles[u].m_cPosition( v ) = m_pairBoundaries.first;
                }
            }*/

            //ds velocity-verlet for velocity
            m_arrParticles[u].m_cVelocity = m_arrParticles[u].m_cVelocity + p_dTimeStep/2*( arrAccelerations[u] + m_arrParticles[u].m_cAcceleration );

            //ds update the acceleration
            m_arrParticles[u].m_cAcceleration = arrAccelerations[u];
        }

        //ds deallocate the array
        delete[] arrAccelerations;
    }

    void saveParticlesToFile( const std::string& p_strFilename ) const
    {

    }

//ds accessors/helpers
public:

    const double getTotalEnergy( const double& p_dMinimumDistance, const double& p_dPotentialDepth ) const
    {
        //ds total energy to accumulate
        double dTotalEnergy( 0.0 );

        //ds for each particle
        for( unsigned int u = 0; u < m_uNumberOfParticles; ++u )
        {
            //ds loop over all other particles (dont do the same particles twice)
            for( unsigned int v = u+1; v < m_uNumberOfParticles; ++v )
            {
                //ds add the kinetic component
                dTotalEnergy += m_arrParticles[u].m_dMass/2*pow( NBody::CVector::absoluteValue( m_arrParticles[u].m_cVelocity ), 2 );

                //ds add the potential component
                dTotalEnergy += getLennardJonesPotential( m_arrParticles[u], m_arrParticles[v], p_dMinimumDistance, p_dPotentialDepth );
            }
        }

        return dTotalEnergy;
    }

    const CVector getCenterOfMass( ) const
    {
        //ds center to find
        CVector cCenter;

        //ds total mass
        double dMassTotal( 0.0 );

        //ds for each particle
        for( unsigned int u = 0; u < m_uNumberOfParticles; ++u )
        {
            //ds add the current relative mass
            cCenter += m_arrParticles[u].m_dMass*m_arrParticles[u].m_cPosition;

            //ds add the current mass
            dMassTotal += m_arrParticles[u].m_dMass;
        }

        //ds divide by total mass
        cCenter /= dMassTotal;

        return cCenter;
    }

    const CVector getAngularMomentum( ) const
    {
        //ds momentum
        CVector cMomentum;

        //ds for each particle
        for( unsigned int u = 0; u < m_uNumberOfParticles; ++u )
        {
            //ds add the current momentum
            cMomentum += NBody::CVector::crossProduct( m_arrParticles[u].m_cPosition, m_arrParticles[u].m_dMass*m_arrParticles[u].m_cVelocity );
        }

        return cMomentum;
    }

    const CVector getLinearMomentum( ) const
    {
        //ds momentum
        CVector cMomentum;

        //ds for each particle
        for( unsigned int u = 0; u < m_uNumberOfParticles; ++u )
        {
            //ds add the current momentum
            cMomentum += m_arrParticles[u].m_dMass*m_arrParticles[u].m_cVelocity;
        }

        return cMomentum;
    }

//ds helpers
private:

    const double getLennardJonesPotential( const CParticle& p_CParticle1,  const CParticle& p_CParticle2, const double& p_dMinimumDistance, const double& p_dPotentialDepth ) const
    {
        //ds formula
        return 4*p_dPotentialDepth*( pow( p_dMinimumDistance/NBody::CVector::absoluteValue( p_CParticle1.m_cPosition-p_CParticle2.m_cPosition ), 12 )
                                   - pow( p_dMinimumDistance/NBody::CVector::absoluteValue( p_CParticle1.m_cPosition-p_CParticle2.m_cPosition ), 6 ) );
    }

    const CVector getLennardJonesForce( const CParticle& p_CParticle1,  const CParticle& p_CParticle2, const double& p_dMinimumDistance, const double& p_dPotentialDepth )
    {
        //ds get absolute value
        const double dAbsoluteDistance( NBody::CVector::absoluteValue( p_CParticle2.m_cPosition-p_CParticle1.m_cPosition ) );

        //ds formula
        return -24*p_dPotentialDepth*( 2*pow( p_dMinimumDistance/dAbsoluteDistance, 12 )
                                        -pow( p_dMinimumDistance/dAbsoluteDistance, 6  ) )
                                    *1/pow( dAbsoluteDistance, 2 )
                                    *( p_CParticle2.m_cPosition-p_CParticle1.m_cPosition );
    }

};

} //ds namespace NBody



#endif //ds CCUBICDOMAIN_H_