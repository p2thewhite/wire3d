**There's a gazillion 3D engines out there. Why did you roll your own?**
> This is just a pet project and on going learning experience for me, as well as my way of giving something back to the Wii homebrew scene. It is also the _only_ open source multiplatform 3D engine supporting the Nintendo Wii (as far as I know).

**Why do you use the [LGPL](http://www.gnu.org/copyleft/lesser.html)?**
> Significant parts of the code were taken and adapted from the [Wild Magic](http://www.geometrictools.com) engine by Dave Eberly, which uses the LGPL itself.

**Why did you roll your own containers instead of using the [STL](http://www.sgi.com/tech/stl/)?**
> Simply to keep dependencies at a minimum. The containers of Wire perfectly suit the job of the engine, however feel free to include and use the STL in your own Wire based projects.

**Why did you roll your own smart pointers instead of using shared\_ptr from [Boost](http://www.boost.org/)?**
> To keep dependencies at a minimum. Wire's smart pointers are also faster than shared\_ptr, since they directly access the reference counter (reference linking). The drawback is that they can only reference objects derived from Wire::Object. Feel free to include Boost in your own projects, however do not use Boost smart pointers to reference Wire::Objects, as this will result in inconsistent reference counting.

**Why did you roll your own run time type information system instead of using the compiler's RTTI?**
> RTTI needs (very little) additional memory. In order to control that, Wire::Objects use their own RTTI (but other Wire classes that were not derived from Wire::Object don't!). Note: the Wire class hierarchy is a single inheritance class tree, therefore the RTTI system does not work with multiple inheritance.

**Why isn't the renderer an abstract interface?**
> Because I wanted to avoid virtual functions in order to maximize performance. So while the scene graph (which is optional for rendering) and its subsystems use virtual functions due to architectural reasons, the renderer and platform dependent renderer related classes do not, linking platform rendering code directly into the application.