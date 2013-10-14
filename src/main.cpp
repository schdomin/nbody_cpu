//ds domain structure
#include "CCubicDomain.h"



int main( int argc, char** argv )
{
    //ds domain configuration
    const std::pair< double, double > pairBoundaries( -1.0, 1.0 );
    const unsigned int uNumberOfParticles( 100 );

    //ds allocate a domain to work with specifying number of particles and timing
    NBody::CCubicDomain cDomain( pairBoundaries, uNumberOfParticles );

    //ds particle configuration
    const double dParticleMass( 1.0 );
    const double dNormalDistributionMean( 0.0 );
    const double dNormalDistributionStandartDeviation( 1.0 );

    //ds create particles uniformly from a normal distribution
    cDomain.createParticlesUniformFromNormalDistribution( dParticleMass, dNormalDistributionMean, dNormalDistributionStandartDeviation );

    //ds current simulation configuration
    const double dTimeStepSize( 0.0001 );
    const unsigned int uNumberOfTimeSteps( 5000 );
    const double dMinimumDistance( 0.05 );
    const double dPotentialDepth( 0.01 );

    //ds start simulation
    for( unsigned int uCurrentTimeStep = 0; uCurrentTimeStep < uNumberOfTimeSteps; ++uCurrentTimeStep )
    {
        //ds update particles
        cDomain.updateParticlesVelocityVerlet( dTimeStepSize, dMinimumDistance, dPotentialDepth );

        //ds record situation (we will write the stream to the file in one operation)
        cDomain.saveParticlesToStream( );

        //ds dump first integrals
        std::cout << "------------------------------------------------------------" << std::endl;
        std::cout << "step: " << uCurrentTimeStep << std::endl;
        std::cout << "total energy: " << std::endl;
        std::cout << cDomain.getTotalEnergy( dMinimumDistance, dPotentialDepth ) << std::endl;
        std::cout << "center of mass: " << std::endl;
        std::cout << cDomain.getCenterOfMass( ) << std::endl;
        std::cout << "angular momentum: " << std::endl;
        std::cout << cDomain.getAngularMomentum( ) << std::endl;
        std::cout << "linear momentum: " << std::endl;
        std::cout << cDomain.getLinearMomentum( ) << std::endl;
    }

    //ds save the particle stream to a file
    cDomain.writeParticlesToFile( "bin/simulation.txt", uNumberOfTimeSteps );

    std::cout << "------------------------------------------------------------" << std::endl;

    return 0;
}
