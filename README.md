# BOIDS DOING BOID THINGS

# Instructions and other information

.gitignore is in solution folder so it doesn't get rid of the libs.

You have to build it for x64, x86 libs aren't included because they are bloat.

Note that on my machine, sometimes glfwInit can take an ungodly amount of time to return, seemingly due to some DInput problem, a restart fixes this. Nobody else has reported this problem, so I assume it's something wrong with my machine.

You have to extract asset.zip into the directory where the exe is or else there will be problems. On this project I rolled the assets out independently of the build, because I couldn't find a reliable way to have it always copy assets (even if the build was up-to-date otherwise)

Hold right click to mouselook and then use WASD while holding it down to move around (Unity-style controls)

I don't recommend going over 1,000 boids, unless you disable all movement except "Target", which can go up to 20,000 holding 60fps on my machine due to the disabling of nearby boid detection in this mode, which saps a lot of performance.

Exceptions are switched off for performance purposes. The compiler complains about this. I'm not switching them back on.

Running in fullscreen gives greatly enhanced performance for some reason.

A cool thing you can do: disable all movement except coalesce until they're all together, create an obstacle and get behind it, and then tick "Target Player" and enable Target movement.
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