# GraphicEngineOpengl
Code not structured in classes. Code in main script just as draft to check funcionality Stages of the application:
1.- Draw triangle moving it with transformation matrixes 
2.- Draw triangle from GPU. Polygon class and buffer management to draw from GPU 
3.- Draw triangle with shader 
4.- Camera implementation. Instead of moving the triangle we move the camera. 
5.- Triangle textured

# Intallation guide:
Proyect running in visual studio 2017, with libraries already installed and directories and link already configured.

Need to add in VC++ Directories: (relative paths, so this paths are now instead of the previous ones)
	Executable directories: .\glm\lib;.\glew\lib;.\glfw\lib-vc2015;$(ExecutablePath)
	Include directories: .\glm\include;.\glfw\include;.\glew\include;$(IncludePath)
	Library directories: .\glm\lib;.\glew\lib;.\glfw\lib-vc2015;$(LibraryPath)

Need to add in the Linker:(following the default text this was added in the linker)
	Additional dependencies, incluir texto: ;opengl32.lib;glfw3.lib;glew32s.lib;glm_static.lib
