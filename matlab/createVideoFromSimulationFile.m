function [ ] = createVideoFromSimulationFile( p_strFilepath )

%ds open the file
fileID = fopen( '/home/dominik/workspace/nbody_cpu/bin/simulation1.txt' );

%ds get the first line
cCell = textscan( fileID, '%u %u', 1 );

%ds get number of particles N and timesteps T
N = cCell{1};
T = cCell{2};

%ds informative
disp( ['N: ', num2str( N ) ] );
disp( ['T: ', num2str( T ) ] );

%ds get the next lines for the initial frame
cCell = textscan( fileID, '%f %f %f %f %f %f', N );

%ds prepare video writer
%ds information and code snippets from:
%   http://www.mathworks.ch/ch/help/matlab/ref/videowriterclass.html
writerObj = VideoWriter('simulation.avi');
writerObj.FrameRate = 500;
open( writerObj );

%ds generate initial data
scatter3( cCell{1}, cCell{2}, cCell{3}, 'fill', 'blue' );
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
    
    %ds create a figure
    scatter3( X, Y, Z, 'fill', 'blue' );
    frame = getframe;
    writeVideo( writerObj, frame );
    
end

%ds write video file
close( writerObj );

end