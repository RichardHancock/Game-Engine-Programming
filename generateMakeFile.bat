SET mkgen_srcDir="Engine"
SET mkgen_compiler="g++"
SET mkgen_flags="-std=c++11 -std=gnu++11 -lGLEW -lGLU -lGL -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf -lpugixml -lLinearMath -lBulletCollision -lBullet3Collision -lBulletDynamics -lBullet3Dynamics -lassimp $(shell sdl2-config --cflags) -I/usr/include/SDL2"
SET mkgen_debugFlags="-g"
SET mkgen_releaseFlags="-O2"
SET mkgen_outputPath="bin/"
SET mkgen_outputFile="Engine"

call genMakeFile.exe %mkgen_srcDir% %mkgen_compiler% %mkgen_flags% %mkgen_debugFlags% %mkgen_releaseFlags% %mkgen_outputPath% %mkgen_outputFile%
