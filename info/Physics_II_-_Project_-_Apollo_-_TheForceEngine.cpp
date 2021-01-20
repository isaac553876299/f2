// The Force Engine
// Just an embarrassingly crude example of a simple 2D Linear Physics Engine.
// This is just pseudo-prototype code to guide you; it does NOT run and it is not complete.
// Do not blindly copy-paste anything in here or you'll find yourselves in a world of pain.
// Just try to understand what the algorithm is trying to do and adapt it to your game.
// David de la Torre - 2020


// Physics body class, 2D Linear Dynamics
// This is the basic block of the physics engine: a simple object that stores position, velocity, acceleration, forces, etc.
// What would you need to add if you wanted to implement 3D Linear+Angular Dynamics?
class Body
{
    public: // Properties (public? private? protected? You decide!)
    float x; // Can use: x,y components, vector, array, etc. Same with the other magnitudes (except the mass, obviously).
    float v;
    float a;
    float f;
    float mass;
    // You might want to add an angle to define the rotation/orientation of the object.
    
    public: // Methods
    void addForce(df); // f += df;
    void addMomentum(dv); // v += dv; // ...why would this not be conceptually correct?
}


// Spaceship class
// Inherits all properties and methods of Body, then expands it with player-related stuff (health, fuel, etc.)
class Spaceship : public Body
{
    public: // Properties
    float health; // Spaceship's hull structural integrity. You could also add another variable for energy shields.
    float fuel; // Could also be an integer, if you do not need too much precision.
    int ammo; // Number of remaining torpedoes. The method launchTorpedo() reduces this by 1.
    
    public: // Methods
    void launchTorpedo();
}


// Game physics engine (GPE) class
// This contains all the GENERIC stuff related to physics: forces (equations), integrator(s), collision solver(s), etc.
// As an exception to the previous rule, it also contains specific lists of physics objects (spaceship, asteroids, etc.). These could be best defined outside this class, but... whatever.
class PhysicsEngine
{
    public: // Constructor/Destructor
    PhysicsEngine();
    ~PhysicsEngine();
    
    public: // Bodies with physics enabled (will react to gravity, aerodynamics, etc.)
    Spaceship xwing; // T-65B space superiority fighter
    vector<Body> asteroids; // That's no Moon...
    vector<Body> torpedoes; // Proton torpedoes
    // Could you generalize the above objects? How?
    
    public: // May the Force be with you
    float forceGrav(/* inputs? */); // Compute Gravity force. You may want to return a vector. Same with the other forces.
    float forceAeroDrag(/* inputs? */); // Compute Aerodynamic Drag force
    float forceAeroLift(/* inputs? */); // Compute Aerodynamic Lift force
    float forceHydroBuoy(/* inputs? */); // Compute Hydrodynamic Buoyancy force
    float forceHydroDrag(/* inputs? */); // Compute Hydrodynamic Drag force
    
    public: // Step physics
    // Apply current physics & integrate & solve collisions --> advance one frame.
    // Remember to reset current forces/momentum of each body after the step!
    // Would you add the integration scheme(s) (Euler, Verlet, ...) inside this class/function? Why? Why not?
    void step(dt);
    
    public: // Collisions
    void detectCollision(object1, object2);
    void solveCollisions();
    
} TheForce; // Instantiate PhysicsEngine as a global variable. Why? Why not?


// Game loop
// Obviously, you will have this loop defined somewhere within your code.
// If you use the SDL template code from the Box2D training pack, you'll need to split each section (User I/O, Game Logic, etc.) and put it in their corresponding module.
void gameLoop()
{
    while(game_continue)
    {
        // Process User I/O
        if (user_presses_left)  TheForce.xwing.addMomentum(-20.0f,   0.0f); // This could also be "turn left". Also, no hard-coded values! You might also want to use variable momentum (press Alt => Boost!). Also, this should spend fuel.
        if (user_presses_right) TheForce.xwing.addMomentum(+20.0f,   0.0f); // etc.
        if (user_presses_up)    TheForce.xwing.addMomentum(  0.0f, -20.0f); // 
        if (user_presses_down)  TheForce.xwing.addMomentum(  0.0f, +20.0f); // 
        if (user_presses_space) TheForce.xwing.launchTorpedo(); // Might want to add some cool-down, so you don't shoot all torpedoes in a couple of frames of the game.
        
        // Process Game Logic
        if (TheForce.xwing.health <= 0.0f) become_force_ghost(); // Now, young Skywalker, you will die.
        if (luke_returns_to_Yavin) emperor_has_died(); // You've failed, your highness.
        
        // Process Physics
        
            // Add Forces to scenario objects
            TheForce.xwing.addForce(forceGrav(/* inputs? */)); // Earth gravity. How would you add the gravity of the Moon?
            if (TheForce.xwing.x[2] < earth_atmosphere_altitude) TheForce.xwing.addForce(forceAeroLift(/* inputs? */));
            if (TheForce.xwing.x[2] < earth_atmosphere_altitude) TheForce.xwing.addForce(forceAeroDrag(/* inputs? */));
            // ...and others.
            
            // Integrate (step world)
            // Re-compute position and velocity of all bodies on the next frame, accounting for the forces applied to them.
            // Which integration scheme (Euler, Verlet, RungeKutta-2-4-6-8, ...) will you use? Why? Maybe you want to use different integrator schemes for different objects?
            // Which "dt" time-step strategy (fixed, variable, fixed + sub-stepping, ...) will you use? Why?
            TheForce.step(dt);
            
            // Detect collision events with asteroids/planets
            // Depending on WHEN you do this checks (ex: GameLogic section), different stuff will happen on your game!
            for (...TheForce.asteroids...)
            {
                if (TheForce.detectCollision(TheForce.torpedoes[k], TheForce.asteroids[i])) delete(TheForce.asteroids[i]); // Great, kid. Don't get cocky.
                if (TheForce.detectCollision(TheForce.xwing, TheForce.asteroids[i])) TheForce.xwing.health -= 2.0f; // This is why I hate flying!
            }
            
            // Resolve collisions
            TheForce.solveCollisions(); // LOTS of insanely complex stuff may be done inside this apparently innocent function...
            
        // Process Graphics
        // Be careful with the drawing order!
        drawOnScreen(YavinIV); // Earth, Moon, Atmosphere, clouds, etc. Everything static.
        drawOnScreen(TheForce.asteroids); // Asteroids can move! Or not.
        drawOnScreen(TheForce.xwing);
        drawOnScreen(TheForce.torpedoes);
    }
}