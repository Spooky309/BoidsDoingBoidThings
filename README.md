# BOIDS DOING BOID THINGS

.gitignore is in solution folder so it doesn't get rid of the libs

You have to build it for x64, x86 libs aren't included because they are bloat.

Note that on my machine, sometimes glfwInit can take an ungodly amount of time to pass, seemingly due to some DInput problem, a restart fixes this. Nobody else has reported this problem, so I assume it's something wrong with my machine.

```
Specification: gl
APIs: gl=4.1
Profile: core
Extensions:
    GL_ARB_debug_output,
    GL_ARB_texture_non_power_of_two,
    GL_ARB_uniform_buffer_object
```

Libraries Used:

* GLFW
* glad
* GLM
* stb
* imgui