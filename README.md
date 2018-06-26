# Message Queue

Work in progress messaging system to work with NGL, and eventually a stand alone system, at present this is the NGL version.

# PLAN

1. Design of ctor for NGLMessage is clunky, need multiple parameters for the different modes (especially file, socket etc) need to think of a better way, most likely is a factory method to wrap it all up.

2. Need to check the threading

3. would be good to allow messages to use native NGL types (perhaps using std::optional?)

4. bound to find more things!