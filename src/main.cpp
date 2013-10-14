//ds domain structure
#include "CCubicDomain.h"



int main( int argc, char** argv )
{
    //ds domain configuration
    const std::pair< double, double > pairBoundaries( -1, 1 );
    const unsigned int uNumberOfParticles( 100 );

    //ds allocate a domain to work with specifying number of particles and timing
    NBody::CCubicDomain cDomain( pairBoundaries, uNumberOfParticles );

    //ds create particles uniformly from a normal distribution
    cDomain.createParticlesUniformFromNormalDistribution( 1.0, 0.0, 0.1 );

    //ds current simulation configuration
    const double dTimeStepSize( 0.1 );
    const unsigned int uNumberOfTimeSteps( 1000 );

    //ds start simulation
    for( unsigned int uCurrentTimeStep = 0; uCurrentTimeStep < uNumberOfTimeSteps; ++uCurrentTimeStep )
    {
        //ds update particles
        cDomain.updateParticlesVelocityVerlet( dTimeStepSize, 1.0, 1.0 );

        //ds dump first integrals
        std::cout << "------------------------------------------------------------" << std::endl;
        std::cout << "step: " << uCurrentTimeStep << std::endl;
        std::cout << "total energy: " << std::endl;
        std::cout << cDomain.getTotalEnergy( 1.0, 1.0 ) << std::endl;
        std::cout << "center of mass: " << std::endl;
        std::cout << cDomain.getCenterOfMass( ) << std::endl;
        std::cout << "angular momentum: " << std::endl;
        std::cout << cDomain.getAngularMomentum( ) << std::endl;
        std::cout << "linear momentum: " << std::endl;
        std::cout << cDomain.getLinearMomentum( ) << std::endl;
    }

    return 0;
}
