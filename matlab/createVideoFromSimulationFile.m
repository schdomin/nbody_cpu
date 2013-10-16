function createVideoFromSimulationFile( )

%ds maximum absolute velocity (empiric value)
dAbsoluteVelocity = 5.0;

%ds open the file
fileID = fopen( '../bin/simulation.txt' );

%ds get the first line
cCell = textscan( fileID, '%u %u', 1 );

%ds get number of particles N and timesteps T
N = cCell{1};
T = cCell{2};

%ds informative
disp( ['N: ', num2str( N ) ] );
disp( ['T: ', num2str( T ) ] );

%ds get the next lines for the initial frame (read first timestep)
cCell = textscan( fileID, '%f %f %f %f %f %f', N );

%ds prepare video writer
%ds information and code snippets from:
%   http://www.mathworks.ch/ch/help/matlab/ref/videowriterclass.html
writerObj = VideoWriter('simulation.avi');
writerObj.FrameRate = 500;
open( writerObj );

%ds get absolute velocity
vecAbsoluteVelocity = sqrt( cCell{4}.^2 + cCell{5}.^2 + cCell{6}.^2 );

%ds scale it
vecAbsoluteVelocity = vecAbsoluteVelocity./dAbsoluteVelocity;

%ds color vector
vecColor = [ vecAbsoluteVelocity, zeros( N, 1 ), 1-vecAbsoluteVelocity ];

%ds generate initial data
scatter3( cCell{1}, cCell{2}, cCell{3}, 25, vecColor, 'fill', 'MarkerEdgeColor', 'black' );
axis tight;
set( gca, 'nextplot' ,'replacechildren' );
set( gcf, 'Renderer' ,'zbuffer' );

%ds for each remaining timestep
for u = 2:1:T
    
    %ds get the next line
    cCell = textscan( fileID, '%f %f %f %f %f %f', N );

    %ds save the information in the respective vectors
    X = cCell{1};
    Y = cCell{2};
    Z = cCell{3};
    U = cCell{4};
    V = cCell{5};
    W = cCell{6};
    
    %ds get absolute velocity
    vecAbsoluteVelocity = sqrt( U.^2 + V.^2 + W.^2 );

    %ds scale it
    vecAbsoluteVelocity = vecAbsoluteVelocity./dAbsoluteVelocity;

    %ds color vector
    vecColor = [ vecAbsoluteVelocity, zeros( N, 1 ), 1-vecAbsoluteVelocity ];
    
    %ds create a figure
    scatter3( X, Y, Z, 25, vecColor, 'fill', 'MarkerEdgeColor', 'black' );
    frame = getframe;
    writeVideo( writerObj, frame );
    
end

%ds write video file
close( writerObj );

end